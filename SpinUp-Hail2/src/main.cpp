#include "main.h"

#define STRAIGHT_WHEEL_MAX_SPEED 1
#define ANGLED_WHEEL_MAX_SPEED 1

bool flywheelOn = false;
pros::Motor left_front(16);
pros::Motor left_center(14);
pros::Motor left_rear(7);

pros::Motor right_front(5);
pros::Motor right_center(4);
pros::Motor right_rear(2);

pros::Motor flywheel_front(1);
pros::Motor flywheel_rear(10);

pros::Motor intake(18);
pros::Motor roller(8, pros::E_MOTOR_GEAR_100); 
pros::Motor loader_wheel(20);

pros::Motor kicker(17, pros::E_MOTOR_GEAR_200);

pros::Motor expansion(11);

void toggleFlywheel(int voltage) {
	if(flywheelOn) {
		flywheel_front.move_voltage(0);
		flywheel_rear.move_voltage(0);
		loader_wheel.move_voltage(0);
		flywheelOn = false;
	} else {
		flywheel_front.move_voltage(voltage);
		flywheel_rear.move_voltage(-voltage);
		loader_wheel.move_voltage(12000);
		flywheelOn = true;
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	kicker.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	roller.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
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
	toggleFlywheel(9500);
	pros::delay(10000);
	kicker.move_absolute(75.0, 200);
	pros::delay(500);
	kicker.move_absolute(0.0, 200);
	pros::delay(500);
	kicker.move_absolute(75.0, 200);
	pros::delay(500);
	kicker.move_absolute(0.0, 200);
	pros::delay(500);
	toggleFlywheel(1000);
	right_center.move_voltage(7000);
	left_center.move_voltage(-10000);
	pros::delay(4000);
	right_center.move_voltage(0);
	left_center.move_voltage(0);
	roller.move_absolute(-90, 100);
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

	while(true) {
		double L_X = master.get_analog(ANALOG_LEFT_X) / 127.0; // strafe left < 0, strafe right > 0
		double L_Y = master.get_analog(ANALOG_LEFT_Y) / 127.0; // go forward > 0, go backwards < 0
		double R_X = master.get_analog(ANALOG_RIGHT_X) / 127.0; // turn right < 0, turn left > 0

		left_front.move_voltage(12000 * L_Y + 12000 * L_X + R_X * 12000);
		left_center.move_voltage((12000 * L_Y + R_X * 12000) * STRAIGHT_WHEEL_MAX_SPEED);
		left_rear.move_voltage(12000 * L_Y + -12000 * L_X + R_X * 12000);

		right_front.move_voltage(-12000 * L_Y + 12000 * L_X + R_X * 12000);
		right_center.move_voltage((-12000 * L_Y + R_X * 12000) * STRAIGHT_WHEEL_MAX_SPEED);
		right_rear.move_voltage(-12000 * L_Y + -12000 * L_X + R_X * 12000);

		if(master.get_digital(DIGITAL_R1)) { // flywheel
			toggleFlywheel(10000);
		}

		if(master.get_digital(DIGITAL_R2)) { // intake/indexer
			intake.move_voltage(12000);
		} else if(master.get_digital(DIGITAL_Y)) { // intake/indexer
			intake.move_voltage(-12000);
		} else {
			intake.move_voltage(0);
		}

		if(master.get_digital(DIGITAL_L2)) {
			roller.move_voltage(12000);
		} else {
			roller.move_voltage(0);
		}

		if(master.get_digital(DIGITAL_L1)) {
			kicker.move_absolute(75.0, 200.0);
		} else {
			kicker.move_absolute(0.0, 200.0);
		}

		if(master.get_digital(DIGITAL_LEFT) && master.get_digital(DIGITAL_RIGHT)) {
			expansion.move(50);
		} else {
			expansion.move(0);
		}
	}
	
}
