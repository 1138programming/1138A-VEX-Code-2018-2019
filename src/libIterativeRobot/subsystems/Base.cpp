#include "libIterativeRobot/subsystems/Base.h"

#include "libIterativeRobot/commands/BaseCommands/DriveWithJoysticks.h"

Base::Base() {
  basePIDController = new PIDController(0.05, 0, 0.00125);
  leftFrontBaseMotor = Motor::getMotor(leftFrontBaseMotorPort);
  leftBackBaseMotor = Motor::getMotor(leftBackBaseMotorPort);

  // TODO: Use our motor's reverse() methods after initial testing

  rightFrontBaseMotor = Motor::getMotor(rightFrontBaseMotorPort);
  rightBackBaseMotor = Motor::getMotor(rightBackBaseMotorPort);

  leftFrontBaseMotor->getMotorObject()->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightFrontBaseMotor->getMotorObject()->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  leftBackBaseMotor->getMotorObject()->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightBackBaseMotor->getMotorObject()->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  baseGyro = new pros::ADIGyro(gyroPort);
  baseGyro->reset();
}

void Base::setBaseMode(pros::motor_brake_mode_e motorMode) {
  leftFrontBaseMotor->getMotorObject()->set_brake_mode(motorMode);
  rightFrontBaseMotor->getMotorObject()->set_brake_mode(motorMode);
  leftBackBaseMotor->getMotorObject()->set_brake_mode(motorMode);
  rightBackBaseMotor->getMotorObject()->set_brake_mode(motorMode);
}

void Base::toggleBase() {
  baseReversed = !baseReversed;
}

void Base::toggleBaseSpeed() {
  baseSlow = !baseSlow;
}

void Base::moveBase(int leftSpeed, int rightSpeed) {
  double left = threshold(leftSpeed, 5);
  double right = threshold(rightSpeed, 5);
  left *= 2;
  right *= 2;

  if (baseReversed) {
    int tmp;
    tmp = left;
    left = -right;
    right = -tmp;
  }

  if (baseSlow) {
    left = slowSpeedMultiplier * left;
    right = slowSpeedMultiplier * right;
  }

  left = (int) left;
  right = (int) right;

  leftFrontBaseMotor->getMotorObject()->move_velocity(-left);
  leftBackBaseMotor->getMotorObject()->move_velocity(-left);

  rightFrontBaseMotor->getMotorObject()->move_velocity(right);
  rightBackBaseMotor->getMotorObject()->move_velocity(right);
}

void Base::moveBaseTo(int leftTarget, int rightTarget, int motorSpeed) {
  leftFrontBaseMotor->getMotorObject()->tare_position();
  leftBackBaseMotor->getMotorObject()->tare_position();
  rightFrontBaseMotor->getMotorObject()->tare_position();
  rightBackBaseMotor->getMotorObject()->tare_position();
  leftFrontBaseMotor->getMotorObject()->move_relative(leftTarget, motorSpeed ? motorSpeed : 150); // TODO: Move last argument to a variable
  leftBackBaseMotor->getMotorObject()->move_relative(leftTarget, motorSpeed ?  motorSpeed : 150);

  rightFrontBaseMotor->getMotorObject()->move_relative(-rightTarget, motorSpeed ? motorSpeed : 150);
  rightBackBaseMotor->getMotorObject()->move_relative(-rightTarget, motorSpeed ? motorSpeed : 150);
}

void Base::moveBaseForward(int target, int motorSpeed) {
  leftFrontBaseMotor->getMotorObject()->tare_position();
  leftBackBaseMotor->getMotorObject()->tare_position();
  rightFrontBaseMotor->getMotorObject()->tare_position();
  rightBackBaseMotor->getMotorObject()->tare_position();

  basePIDController->setSetpoint(target);
  basePIDController->setMaxPIDSpeed(motorSpeed ? motorSpeed : 150);
  basePIDController->setSensorValue(0);
  baseGyro->reset();
}

void Base::updateLinearMovement() {
  basePIDController->setSensorValue(-rightFrontBaseMotor->getMotorObject()->get_position());
  basePIDController->loop();
  int output = reverseThreshold(-basePIDController->getOutput());
  int gyroCorrection = baseGyro->get_value() * 0.12;
  moveBase(output + gyroCorrection, output - gyroCorrection);
}

bool Base::baseAtTarget() {
  return abs(leftFrontBaseMotor->getMotorObject()->get_target_position() - leftFrontBaseMotor->getMotorObject()->get_position()) <= 3; // Tune threshold and make a varaible
}

bool Base::baseAtLinearTarget() {
  return basePIDController->atSetpoint();
}

double Base::getGyroValue() {
  return baseGyro->get_value();
}

void Base::resetGyro() {
  baseGyro->reset();
}

void Base::initDefaultCommand() {
  // Setup up a default command here
  setDefaultCommand(new DriveWithJoysticks());
}
