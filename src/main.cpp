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
	pros::lcd::initialize();
}

int selectedAuton = AUTON_RED;
bool auton_far = false;
bool auton_delay_switch = false;
bool auton_floor = true;
bool op_control = false;

void set_auton(int auton){
  pros::lcd::clear_line(7);
  if(auton == AUTON_RED){
    selectedAuton = auton;
    pros::lcd::print(7, "AUTON RED");
  }
  else if(auton == AUTON_BLUE){
    selectedAuton = auton;
    pros::lcd::print(7, "AUTON BLUE");
  }
  else
    selectedAuton = AUTON_NONE;
		pros::lcd::print(7, "UH OH, NO AUTON???");
}

void on_center_button() {
	static bool center_pressed = false;
	center_pressed = !center_pressed;
	if (center_pressed && op_control) {
		pros::delay(1000);
		autonomous();
	}
}

void on_left_button(){
	static bool left_pressed = false;
	left_pressed = !left_pressed;
	if(left_pressed){
		  set_auton(AUTON_RED);
	}
}

void on_right_button(){
	static bool right_pressed = false;
	right_pressed = !right_pressed;
	if(right_pressed){
		set_auton(AUTON_BLUE);
	}
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
	// Used for testing in op controller
	bool was_op_control = op_control;
	op_control = false;

	// WHEN ADDING A TURN, ADD IT HERE FOR AUTON RED
	// WILL BE REVERSED FOR BLUE
	// ONLY NEED TO WRITE AUTON ONCE FOR MAX SPEEEEED
	int turns[]{
		0 //FIRST TURN EXAMPLE
	};
	// FLIP TURNS FOR BLUE SIDE
	if(selectedAuton == AUTON_BLUE){
		for(int i = 0; i < std::size(turns); i++){
			turns[i] = -turns[i];
		}
	}
	// NOTE: USE TURNS LIKE SO:
	// chassis.turnAngle(turns[0]);



	/******* AUTON START *******/

	chassis.moveDistance(-360);
	pros::delay(2000);
	chassis.moveDistance(360);
	pros::delay(2000);

	/******* AUTON END *******/



	op_control = was_op_control;
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
	op_control = true;
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	//Toggle for drive speed. Declared outside while loop so value is not reset.
	bool toggleSpeed	= false;
	bool toggleArm		= false;
	bool armsOn				= false;

	bool toggleHolder = false;

	int maxVelocity = 0;

	//flipper.moveAbsolute(130, 200);

	while (true) {
		if(op_control){
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


		armsOn = false;
		if (master.get_digital(DIGITAL_UP)) {
			arms.moveVelocity(-50);
			armsOn = true;
		}
		else if(master.get_digital(DIGITAL_DOWN)){
			arms.moveVelocity(50);
			armsOn = true;
		}

		//Runs the intake either in or out depending on the button pushed.
		if (master.get_digital(DIGITAL_R1)) {
			intake.moveVelocity(150);
			if(!armsOn)
				arms.moveVelocity(40);
		} else if (master.get_digital(DIGITAL_R2)) {
			intake.moveVelocity(-150);
			if(!armsOn)
				arms.moveVelocity(40);
		} else {
			intake.moveVelocity(0);
			if(!armsOn)
				arms.moveVelocity(0);
		}

		// //Moves arms in or out depending on the button pushed.
		// if (master.get_digital(DIGITAL_A)) {
		// 	arms.moveVelocity(50);
		// } else if (master.get_digital(DIGITAL_L2)) {
		// 	arms.moveVelocity(-50);
		// } else if (!armsOn) {
		// 	arms.moveVelocity(0);
		// }

		if (master.get_digital_new_press(DIGITAL_A)){
			toggleHolder = !toggleHolder;
			toggleHolder ? holder.moveAbsolute(48, 50) : holder.moveAbsolute(0, 50);
		}


		//Raises or lowers the lift depending on the button pushed.
		if (master.get_digital(DIGITAL_L1)) {
			// lift.moveAbsolute(140, 20);
			double pos = lift.getPosition();
			double vel = lift.getActualVelocity();
			// pros::lcd::print(3, "Velocity: %f", curVelocity);
			if(pos > 120){
				lift.moveVelocity(0);
			}
			else if(vel > 50 || pos > 70){
				lift.moveVelocity(10);
				maxVelocity = vel;
				pros::lcd::print(4, "Max Velocity: %d", maxVelocity);
				pros::lcd::print(5, "Position: %f", pos);
			}
			else{
				lift.moveVelocity(20);
			}

		} else if (master.get_digital(DIGITAL_L2)) {
			// lift.moveAbsolute(0, 20);
			lift.moveVelocity(-20);
		}
		else{
			lift.moveVoltage(0);
		}
		}
		pros::delay(20);
	}
}
