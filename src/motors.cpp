#include "motors.hpp"

//Creates a chassis controller object. Use this instead of setting individual motor values.
auto chassis = ChassisControllerFactory::create(
  //The ports of the left then right side motors. Use a negative value if it should be inverted.
  {1, 11}, {-10, 20},
  //The gearset used in the motors.
  AbstractMotor::gearset::green);
