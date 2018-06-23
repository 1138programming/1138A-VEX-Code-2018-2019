#include "main.h"

Puncher* Puncher::instance = 0;

Puncher::Puncher() {
  leftPuncherMotor = Motor::getMotor(leftPuncherPort);
  rightPuncherMotor = Motor::getMotor(rightPuncherPort);

  leftPuncherMotor->addFollower(rightPuncherMotor);

  puncherEncoder = encoderInit(puncherEncoderTopPort, puncherEncoderBottomPort, true);
  resetEncoder();

  controller = new PIDController(leftPuncherMotor, 0.9, 0.01, 0.0);
  controller->setThreshold(20);
}

void Puncher::move(int speed) {
  leftPuncherMotor->setSpeed(speed);
}

void Puncher::setSetpoint(int setpoint) {
  controller->setSetpoint(setpoint);
}

void Puncher::lock() {
  controller->setSetpoint(getEncoderValue());
}

int Puncher::getSetpoint() {
  return controller->getSetpoint();
}

void Puncher::loop() {
  controller->sensorValue(getEncoderValue());
  controller->loop();
}

bool Puncher::atSetpoint() {
  return controller->atSetpoint();
}

int Puncher::getEncoderValue() {
  return encoderGet(puncherEncoder);
}

void Puncher::resetEncoder() {
  encoderReset(puncherEncoder);
}

Puncher* Puncher::getInstance() {
  if (instance == 0) {
    instance = new Puncher();
  }

  return instance;
}