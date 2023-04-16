#include "main.h"

int count = 0;
void touch_handle(){
	std::cout << "touch screen " << count <<  std::endl;
	++count;
}


void initialize() {
	pros::screen::touch_callback(touch_handle, TOUCH_PRESSED);

}

void opcontrol() {
	int i = 0;
	pros::screen_touch_status_s_t status;
	int touch_x = 0;
	int touch_y = 0;
	int auton_selection = 0;
	while(1){
		status = pros::screen::touch_status();
		touch_x = status.x;
		touch_y = status.y;
		
		//Auton 1
		pros::screen::set_pen(COLOR_BLUE);
		pros::screen::fill_rect(0, 0, 100, 100);
		//Auton 2
		pros::screen::set_pen(COLOR_YELLOW);
		pros::screen::fill_rect(100, 0, 200, 100);
		//Auton 3
		pros::screen::set_pen(COLOR_BLUE);
		pros::screen::fill_rect(200, 0, 300, 100);
		//Auton 4
		pros::screen::set_pen(COLOR_YELLOW);
		pros::screen::fill_rect(0, 100, 100, 200);
		//Auton 5
		pros::screen::set_pen(COLOR_BLUE);
		pros::screen::fill_rect(100, 100, 200, 200);
		//Auton 6
		pros::screen::set_pen(COLOR_YELLOW);
		pros::screen::fill_rect(200, 100, 300, 200);


		pros::screen::print(pros::E_TEXT_MEDIUM, 20, 20, "Auton 1");
		if(touch_x >= 0 && touch_x <= 100 && touch_y >= 0 && touch_y <= 100){
			std::cout << "Pressed Auton 1" << std::endl;
			auton_selection = 1;
		}
		pros::screen::print(pros::E_TEXT_MEDIUM, 120, 20, "Auton 2");
		if(touch_x >= 100 && touch_x <= 200 && touch_y >= 0 && touch_y <= 100){
			std::cout << "Pressed Auton 2" << std::endl;
			auton_selection = 2;
		}
		pros::screen::print(pros::E_TEXT_MEDIUM, 220, 20, "Auton 3");
		if(touch_x >= 200 && touch_x <= 300 && touch_y >= 0 && touch_y <= 100){
			std::cout << "Pressed Auton 3" << std::endl;
			auton_selection = 3;
		}
		pros::screen::print(pros::E_TEXT_MEDIUM, 20, 120, "Auton 4");
		if(touch_x >= 0 && touch_x <= 100 && touch_y >= 100 && touch_y <= 200){
			std::cout << "Pressed Auton 4" << std::endl;
			auton_selection = 4;
		}
		pros::screen::print(pros::E_TEXT_MEDIUM, 120, 120, "Auton 5");
		if(touch_x >= 100 && touch_x <= 200 && touch_y >= 100 && touch_y <= 200){
			std::cout << "Pressed Auton 5" << std::endl;
			auton_selection = 5;
		}
		pros::screen::print(pros::E_TEXT_MEDIUM, 220, 120, "Auton 6");
		if(touch_x >= 200 && touch_x <= 300 && touch_y >= 100 && touch_y <= 200){
			std::cout << "Pressed Auton 6" << std::endl;
			auton_selection = 6;
		}

		pros::delay(20);
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

