#include "libIterativeRobot/commands/TurnWithGyro.h"
#include "libIterativeRobot/Robot.h"

TurnWithGyro::TurnWithGyro(int turnDegrees) {
  requires(Robot::robotBase);
  this->degrees = turnDegrees;
  this->motorSpeed = 0;
  this->timeAtSetpoint = 0;
}

TurnWithGyro::TurnWithGyro(int turnDegrees, int motorSpeed) {
  requires(Robot::robotBase);
  this->degrees = turnDegrees;
  this->motorSpeed = motorSpeed;
  this->timeAtSetpoint = 0;
}

bool TurnWithGyro::canRun() {
  //printf("TurnWithGyro(%d);\n", degrees);
  return true; // This is the default value anyways, so this method can be removed
}

void TurnWithGyro::initialize() {
  // Perform any initialization steps for this command here, not in the
  // constructor
  if (degrees != 0) {
    Robot::robotBase->resetGyro();
  }
  gyroPID = new PIDController(0.1, 0.0 , 0.011);
  gyroPID->setSensorValue(0);
  gyroPID->setSetpoint(degrees);
  //gyroPID->threshold = 30;
  this->timeAtSetpoint = 0;
}

void TurnWithGyro::execute() {
  // Code that runs when this command is scheduled to run
  printf("Gyro value: %f\n", Robot::robotBase->getGyroValue());
  //printf("Hello, world!\n");
  gyroPID->setSensorValue(Robot::robotBase->getGyroValue());
  gyroPID->loop();
  int motorSpeed = (int)((double)gyroPID->getOutput() * 1.5);

  Robot::robotBase->moveBase(-motorSpeed, motorSpeed);
}

bool TurnWithGyro::isFinished() {

  gyroPID->setSensorValue(Robot::robotBase->getGyroValue());
  if (this->timeAtSetpoint == 0 && gyroPID->atSetpoint()) {
    this->timeAtSetpoint = pros::millis() + 250;
  } else if (this->timeAtSetpoint == 0) {
    return false;
  } else if (!gyroPID->atSetpoint()) {
    this->timeAtSetpoint = 0;
    return false;
  }
  return this->timeAtSetpoint > pros::millis();//gyroPID->atSetpoint();
}

void TurnWithGyro::end() {
  // Code that runs when isFinished() returns true.
  Robot::robotBase->moveBase(0, 0);
  delete gyroPID;
  Robot::robotBase->resetGyro();
}

void TurnWithGyro::interrupted() {
  // Code that runs when this command is interrupted by another one
  // with a higher priority.
  delete gyroPID;
}