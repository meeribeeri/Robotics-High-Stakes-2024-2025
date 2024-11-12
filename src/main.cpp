#include "main.h"
/*
Important information
use pros terminal
	pros c n [project name] (for a new project)
	pros make clean (clears make or something, its important if make doesn't do anything)
	pros make : builds the program
	pros build-compile-commands (project setup, use if headers fail)
	Add "compileCommands": "${workspaceFolder}/compile_commands.json" in c_cpp_properties.json if headers still fail
	pros mut (to upload to robot)
*/
bool clamp_state = false;

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_drive({-17, 18});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
pros::MotorGroup right_drive({19, -20});  // Creates a motor group with forwards port 5 and reversed ports 4 & 6
pros::MotorGroup intake_outtake({1,2});
pros::ADIDigitalOut clamp1('A', clamp_state);
pros::ADIDigitalOut clamp2('H', clamp_state);

int reverse = 1;

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();


	pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {}

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
	
	reverse = 1;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		//drive stuff
		left_drive.move((int)(master.get_analog(ANALOG_LEFT_Y) * reverse + master.get_analog(ANALOG_RIGHT_X)));
		right_drive.move((int)(master.get_analog(ANALOG_LEFT_Y) * reverse - master.get_analog(ANALOG_RIGHT_X)));
		
		if (master.get_digital_new_press(DIGITAL_R1) && master.get_digital(DIGITAL_L1)) {
			reverse = reverse * -1;
		}

		if (master.get_digital(DIGITAL_B)) {
			intake_outtake.move(127*0.5);
		} else if (master.get_digital(DIGITAL_DOWN)) {
			intake_outtake.move(-127*0.5);
		} else {
			intake_outtake.move(0);
		}

		if (master.get_digital_new_press(DIGITAL_X)) {
			clamp_state = clamp_state * -1;
			clamp1.set_value(clamp_state);
			clamp2.set_value(clamp_state);
		}


		pros::delay(20);                               // Run for 20 ms then update
	}
}