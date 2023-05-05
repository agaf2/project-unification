#include "CustomPlayer.h"
CustomPlayer::CustomPlayer(int index, QThreadPool* threadPool) : Processing(index, threadPool) {
}

void CustomPlayer::buildParameters(Parameters::Handler& parameters) {
}

void CustomPlayer::connectModules(const Modules* modules) {
  connect(modules->vision(),
          &Vision::sendFrame,
          this,
          &CustomPlayer::receiveFrame,
          Qt::DirectConnection);

  connect(modules->vision(),
          &Vision::sendField,
          this,
          &CustomPlayer::receiveField,
          Qt::DirectConnection);
}

void CustomPlayer::init(const Modules* modules) {
}

void CustomPlayer::update() {
  shared->field.extract_to(field);
  if (auto f = shared->frame.get_optional_and_reset()) {
    if (auto it = f->allies().findById(index()); it != f->allies().end()) {
      robot = *it;
    }
    frame.emplace(*f);
  }
}

void CustomPlayer::exec() {
  if (!field || !frame || !robot) {
    return;
  }

  // TODO: here...
  
 if(frame->has_ball()){
    if(robot->id() == 3){
      
      SSLMotion::GoToPoint gotopoint(QPointF(frame->ball().x(),frame->ball().y() ),
                                (frame->ball() - robot->position()).angle(),
                                true);
      auto command = sslNavigation.run(robot.value() ,
      SSLRobotCommand(gotopoint));
      emit sendCommand(command);
                                              // O Behaivor escolhe o robô com id 3 para ir atrás da bola em toda arena
                                              // através de gotopoint segue as coordenadas da bola
    }
  }
  // emit sendCommand(...);
}

void CustomPlayer::receiveField(const Field& field) {
  shared->field = field;
}

void CustomPlayer::receiveFrame(const Frame& frame) {
  shared->frame = frame;
  runInParallel();
}

static_block {
  Factory::processing.insert<CustomPlayer>();
};
