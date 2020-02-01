#include "motors.hpp"

//Creates a chassis controller object. Use this instead of setting individual motor values.
ChassisControllerIntegrated chassis = ChassisControllerFactory::create(
  //The ports of the left then right side motors. Use a negative value if it should be inverted.
  {3, 11}, {-8, -20},
  //The gearset used in the motors.
  AbstractMotor::gearset::green);

ChassisControllerPID chassis2 = ChassisControllerFactory::create(
  //Motors
  {3, 11}, {-8, -20},
  //Distance Control
  IterativePosPIDController::Gains{0.000, 0.0000, 0.0000},
  //Straightening Control
  IterativePosPIDController::Gains{0.000, 0.0000, 0.0000},
  //Turning Control
  IterativePosPIDController::Gains{0.000, 0.0000, 0.0000},
  //Gearset
  AbstractMotor::gearset::green,
  //Wheel diamater and wheelbase width (from center of one wheel to center of other)
  {4.125_in, 12_in}
);

MotorGroup intake({Motor(1, false, AbstractMotor::gearset::green), Motor(10, true, AbstractMotor::gearset::green)});
MotorGroup arms({Motor(2, false, AbstractMotor::gearset::red), Motor(9, true, AbstractMotor::gearset::red)});
MotorGroup lift({Motor(4, false, AbstractMotor::gearset::red), Motor(5, false, AbstractMotor::gearset::red),
  Motor(6, true, AbstractMotor::gearset::red), Motor(7, true, AbstractMotor::gearset::red)}
);

Motor holder = Motor(19, false, AbstractMotor::gearset::red);

/* Motor type information
 *
 * Red:   36:1  100 RPM
 * Green: 18:1  200 RPM
 * Blue:  6:1   600 RPM
*/
