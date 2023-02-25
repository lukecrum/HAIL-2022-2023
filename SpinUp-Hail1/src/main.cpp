#include "main.h"
#include "ARMS/chassis.h"
#include "ARMS/config.h"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "pros/vision.h"
#include "pros/vision.hpp"
#include "okapi/api.hpp"

enum CataState {
	LIMIT_PRESSED,
	MOVING,
	CATA_IDLE
};

enum IntakeState {
	INTAKING,
	INTAKE_IDLE
};

CataState catastate = CATA_IDLE;
IntakeState intakestate = INTAKE_IDLE;

#define EXAMPLE_SIG 1
pros::Vision vis(16, pros::E_VISION_ZERO_CENTER);

pros::vision_signature_s_t REDGOAL = pros::Vision::signature_from_utility(2, 8847, 9467, 9157, -1093, -497, -795, 11.000, 0);
pros::vision_signature_s_t BLUEGOAL = pros::Vision::signature_from_utility(1, -3203, -2557, -2880, 9231, 12737, 10984, 4.000, 0);


/*std::string TuningToString(okapi::PIDTuner::Output tuning) {
    std::string ret = "Kp: ";
    ret.append(std::to_string(tuning.kP));
    ret.append("\n");

    ret.append("Ki :");
    ret.append(std::to_string(tuning.kI));
    ret.append("\n");

    ret.append("KD :");
    ret.append(std::to_string(tuning.kD));
    ret.append("\n");
    return ret;
}*/

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	arms::init();

	/*okapi::Logger::setDefaultLogger(
	std::make_shared<okapi::Logger>(
		okapi::TimeUtilFactory::createDefault().getTimer(), // It needs a Timer
		"/ser/sout", // Output to the PROS terminal
		okapi::Logger::LogLevel::info // Show errors and warnings
		));

	auto AngleTuner = okapi::PIDTunerFactory::createPtr(
        ct, ct, 4 * okapi::second, 0,
        0.0005, 0.001, 0, 0, 0, 0.0001);

	auto tuning = AngleTuner.autotune();

	std::cout << TuningToString(tuning) << std::endl;
*/
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
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	pros::Motor cataMotor = pros::Motor(12); // TODO: Get cata motor port
	pros::Motor cataMotor2 = pros::Motor(13);
	pros::Motor intakeMotor = pros::Motor(11); // TODO:  Get intake motor port
	pros::ADIDigitalIn cataLimitSwitch = pros::ADIDigitalIn(1); //TODO: Get limit switch port

	pros::Motor expansion = pros::Motor(8);
	pros::Motor expansion2 = pros::Motor(9);
	

	double THRESHOLD = 10.0;
	
	vis.set_signature(EXAMPLE_SIG, &BLUEGOAL);
	vis.set_signature(EXAMPLE_SIG, &REDGOAL);

	
	pros::vision_object_s_t OBJ = vis.get_by_sig(0, 1);
	//std::cout << "target x: " << OBJ.x_middle_coord << " target y: " << OBJ.y_middle_coord << "\n";

	while (true) {

		OBJ = vis.get_by_sig(0, 1);

		double error = OBJ.x_middle_coord;

		if(abs(error) > THRESHOLD && master.get_digital(DIGITAL_X)) {
			double turnSpeed = 0;
			if(error < 0) {
				turnSpeed = -10;
			} else {
				turnSpeed = 10;
			}
			arms::chassis::arcade(0, turnSpeed * (abs(error) / 13));
		} else if(!master.get_digital(DIGITAL_L2)) {
			arms::chassis::arcade(master.get_analog(ANALOG_LEFT_Y) * (double)100 / (double)127,
		                      master.get_analog(ANALOG_RIGHT_X) * (double)100 /
		                          (double)127);
		}

		if(cataLimitSwitch.get_value() && catastate == MOVING && master.get_digital(DIGITAL_A) == false) { // if the cata limit switch is pressed
			cataMotor.move(0);
			cataMotor2.move(0);
			catastate = LIMIT_PRESSED;
		}

		if(cataLimitSwitch.get_value()) {
			std::cout << "limit hit" << std::endl;
		}

		if(catastate == MOVING) {
			std::cout << "MOVING" << std::endl;
		} else if(catastate == CATA_IDLE) {
			std::cout << "CATA IDLE" << std::endl;
		} else if(catastate == LIMIT_PRESSED) {
			std::cout << "LIMIT_PRESSED" << std::endl;
		}

		if(master.get_digital(DIGITAL_R2)) { // if A is pressed, fire cata
			if(catastate == CATA_IDLE || catastate == LIMIT_PRESSED) { // if idle or on limit switch, start firing
				cataMotor.move(127);
				cataMotor2.move(-127);
				catastate = MOVING;
			}
		}

		if(master.get_digital(DIGITAL_B)) {
			intakeMotor.move(127);
			intakestate = INTAKING;
		} else if(master.get_digital(DIGITAL_Y)) {
			intakeMotor.move(-127);
			intakestate = INTAKING;
		} else {
			intakeMotor.move(0);
			intakestate = INTAKE_IDLE;
		}

		if(master.get_digital(DIGITAL_LEFT) && master.get_digital(DIGITAL_RIGHT)) {
			expansion.move(-50);
			expansion2.move(50);
		} else {
			expansion.move(0);
			expansion2.move(0);
		}


		pros::vision_object_s_t OBJ = vis.get_by_sig(0, 1);

		std::cout << "target x: " << OBJ.x_middle_coord << " target y: " << OBJ.y_middle_coord << "\n";
		pros::delay(10);
	}
	//arms::chassis::move(24);
	//arms::chassis::turn(90);
}
