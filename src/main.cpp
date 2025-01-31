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
bool flag_state = false;

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_drive({-17, 18});  //18 is lower
pros::MotorGroup right_drive({19, -20}); //19 is upper
pros::MotorGroup intake_outtake({1,3});
pros::adi::DigitalOut clamp1('A', clamp_state);
pros::adi::DigitalOut flag('B', flag_state);

bool one_stick = true;
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
void autonomous() {
	auto1();


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
	one_stick = true;
	reverse = 1;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		//drive stuff
		if (one_stick) {
			left_drive.move((int)((master.get_analog(ANALOG_LEFT_Y) * reverse - (master.get_analog(ANALOG_LEFT_X) * 0.5))));
			right_drive.move((int)((master.get_analog(ANALOG_LEFT_Y) * reverse + (master.get_analog(ANALOG_LEFT_X) * 0.5))));
		} else {
			left_drive.move((int)((master.get_analog(ANALOG_LEFT_Y) * reverse - (master.get_analog(ANALOG_RIGHT_X) * 0.5))));
			right_drive.move((int)((master.get_analog(ANALOG_LEFT_Y) * reverse + (master.get_analog(ANALOG_RIGHT_X) * 0.5))));
		}
		
		if (master.get_digital_new_press(DIGITAL_R2) && master.get_digital(DIGITAL_L2)) {
			one_stick = one_stick ^ 0x1;
			master.rumble("---");
			std::cout << master.get_digital_new_press(DIGITAL_R2) << " " << master.get_digital(DIGITAL_L2) << std::endl;
		}


		if (master.get_digital_new_press(DIGITAL_R1) && master.get_digital(DIGITAL_L1)) {
			reverse = reverse * -1;
			master.rumble("...");
		}

		if (master.get_digital(DIGITAL_B)) {
			intake_outtake.move(127 * 0.5);
		} else if (master.get_digital(DIGITAL_DOWN)) {
			intake_outtake.move(-127*0.8);
		} else {
			intake_outtake.move(0);
		}

		if (master.get_digital_new_press(DIGITAL_X)) {
			clamp_state = clamp_state ^ 0x1;
			clamp1.set_value(clamp_state);
		}
		if (master.get_digital_new_press(DIGITAL_UP)) {
			flag_state = flag_state ^ 0x1;
			flag.set_value(flag_state);
		}



		pros::delay(20);                               // Run for 20 ms then update
	}
}