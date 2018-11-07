#include "libIterativeRobot/commands/AutonGroup2.h"
#include "libIterativeRobot/commands/DriveToPosition.h"
#include "libIterativeRobot/commands/FlywheelRevUp.h"
#include "libIterativeRobot/commands/DelayCommand.h"
#include "libIterativeRobot/commands/CollectorMiddleForwardTimed.h"
#include "libIterativeRobot/commands/CollectorForwardTimed.h"

AutonGroup2::AutonGroup2() {
  //

  //addSequentialCommand(new DriveToPosition(1250, 1250)); // Forwards one tile
  //addSequentialCommand(new DriveToPosition(-208, 208)); // Rotate 45 degrees
  //addSequentialCommand(new DriveToPosition(-30, 30)); // Rotate x degrees

  addSequentialCommand(new FlywheelRevUp());
  addSequentialCommand(new CollectorMiddleForwardTimed(500));
  addSequentialCommand(new DelayCommand(150));

  addSequentialCommand(new DriveToPosition(3050, 3050));
  addSequentialCommand(new DelayCommand(150));

  addSequentialCommand(new DriveToPosition(-3400, -3400));
  addSequentialCommand(new DelayCommand(250));

  // Turn and move forward
  addSequentialCommand(new DriveToPosition(590, -590));
  addSequentialCommand(new DelayCommand(150));
  addSequentialCommand(new DriveToPosition(2700, 2700));
  addParallelCommand(new CollectorForwardTimed(1800));
  addSequentialCommand(new DelayCommand(150));

  // Now we move forward
  addSequentialCommand(new CollectorMiddleForwardTimed(500));
  addParallelCommand(new CollectorForwardTimed(500));

  addSequentialCommand(new DriveToPosition(-400, -400));

  addSequentialCommand(new DriveToPosition(-700, 700));
  addSequentialCommand(new DriveToPosition(-1250, -1250, 200));

}