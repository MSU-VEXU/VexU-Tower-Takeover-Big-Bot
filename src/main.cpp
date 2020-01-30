#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	//Sets the arm motors to hold position when not powered.
	arms.setBrakeMode(AbstractMotor::brakeMode::hold);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	chassis.moveDistance(-360);
	pros::delay(2000);
	chassis.moveDistance(360);
	pros::delay(2000);

	autonRed();
	// autonBlue();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	//Toggle for drive speed. Declared outside while loop so value is not reset.
	bool toggleSpeed	= false;
	bool toggleArm		= false;
	bool armsOn				= false;

	flipper.moveAbsolute(130, 200);

	while (true) {
		//If the button is pressed, it toggles the value. Using new_press so it is only toggled once per press.
		if (master.get_digital_new_press(DIGITAL_DOWN)) {
			toggleSpeed = true;
		} else if (master.get_digital_new_press(DIGITAL_UP)) {
			toggleSpeed = false;
		}

		//The chassis controller sets the left and right sides to their respective joysticks.
		//The value has to be between -1 and 1, so get_analog is divided by 127.
		//The ternary operator at the end of each will slow the speed if the toggle is true.
		chassis.tank(master.get_analog(ANALOG_LEFT_Y)/127.0/(toggleSpeed ? 2 : 1),
			master.get_analog(ANALOG_RIGHT_Y)/127.0/(toggleSpeed ? 2 : 1));

		//Runs the intake either in or out depending on the button pushed.
		if (master.get_digital(DIGITAL_R1)) {
			intake.moveVelocity(150);
			arms.moveVelocity(-40);
			armsOn = true;
		} else if (master.get_digital(DIGITAL_R2)) {
			intake.moveVelocity(-150);
			armsOn = false;
		} else {
			intake.moveVelocity(0);
			armsOn = false;
		}

		// //Moves arms in or out depending on the button pushed.
		// if (master.get_digital(DIGITAL_A)) {
		// 	arms.moveVelocity(50);
		// } else if (master.get_digital(DIGITAL_L2)) {
		// 	arms.moveVelocity(-50);
		// } else if (!armsOn) {
		// 	arms.moveVelocity(0);
		// }
		if (master.get_digital_new_press(DIGITAL_A)) {
			toggleArm = !toggleArm;
			toggleArm ? arms.moveAbsolute(230, 50) : arms.moveAbsolute(0, 50);
		}


		//Raises or lowers the lift depending on the button pushed.
		if (master.get_digital(DIGITAL_L1)) {
			lift.moveAbsolute(140, 20);
		} else if (master.get_digital(DIGITAL_L2)) {
			lift.moveAbsolute(0, 20);
		}

		pros::delay(20);
	}
}
