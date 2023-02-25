#include "main.h"
#include "Subsystems/Drive.h"
#include "Subsystems/Flywheel.h"
#include "Subsystems/Intake.h"
#include <iostream>
#include "Subsystems/Indexer.h"
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

Drive drivetrain;
Flywheel flywheel;
Indexer indexer;
// Intake intake;
pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);
pros::Motor intakeMotor(INTAKE_PORT);

void initialize() {
	drivetrain.init();
	flywheel = Flywheel();
	indexer = Indexer();
	// intake = Intake();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	std::shared_ptr<okapi::ChassisModel> chassis = drivetrain.drive->getModel();
	while(true) {
	//	std::cout << "encoder values: " << chassis->getSensorVals()[0] << " "<< chassis->getSensorVals()[1] << std::endl;
	//	std::cout << "odom values: " << drivetrain.drive->getState().str(1_in, 1_deg) << std::endl;
	}
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	// LEAVE =============================
	pros::lcd::initialize();
	drivetrain.resetIMU();
	pros::delay(3000);
	drivetrain.imu.set_heading(180);
	pros::lcd::print(0, "calibration complete");
	// LEAVE =============================
}

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


/// 180 deg is 206
/// 90 deg is 108

void autonomous() {

	// pros::delay(500);

	// //drivetrain.drive->moveDistance(36_in); // pid controls this
	// //drivetrain.drive->driveToPoint({3_ft, 0_ft}); // odom controls this
	// drivetrain.drive->moveDistance(3_ft);
	// drivetrain.drive->waitUntilSettled();
	// pros::delay(500);
	// drivetrain.drive->turnAngle(208_deg);
	// drivetrain.drive->waitUntilSettled();
	// pros::delay(500);
	// drivetrain.drive->moveDistance(3_ft);
	//drivetrain.drive->turnToPoint({0_ft, 0_ft});
	//drivetrain.drive->turnAngle(200_deg);
	//drivetrain.drive->moveDistance(3_ft);
	//drivetrain.drive->driveToPoint({0_ft, 0_ft});
	//drivetrain.drive->turnToPoint({0_ft, 1_ft});
	//drivetrain.drive->turnAngle(180_deg);
	// 
//drivetrain.drive->turnAngle(108_deg);

	// std::cout << "encoder values: " << chassis->getSensorVals()[0] << " "<< chassis->getSensorVals()[1] << std::endl;
	// std::cout << "odom values: " << drivetrain.drive->getState().str(1_in, 1_deg) << std::endl;

	// std::shared_ptr<okapi::ChassisModel> chassis = drivetrain.drive->getModel();
	// while(true) {
	// 	std::cout << "encoder values: " << chassis->getSensorVals()[0] << " "<< chassis->getSensorVals()[1] << std::endl;
	// 	std::cout << "odom values: " << drivetrain.drive->getState().str(1_in, 1_deg) << std::endl;
	// }
	// drivetrain.drive->driveToPoint({1_ft, 1_ft});
	//drivetrain.drive->driveToPoint({1_ft,0_ft});
	//drivetrain.drive->turnToAngle(180_deg);
	//drivetrain.drive->moveDistance(1_ft);
	//drivetrain.drive->turnAngle(90_deg);
	//drivetrain.drive->turnToPoint({1_ft, 1_ft});
	// pros::Task bangBang(flywheel.bangBangLoop);


	drivetrain.drive->setState({0_in, 0_in, 0_deg});
	drivetrain.drive->getModel()->setMaxVelocity(300);

	pros::Task task{[=] {
		flywheel.bangBangLoop();
	}
	};


	// MODIFY ============================

	//drivetrain.drive->turnToAngle(90_deg);

	// BEGIN START FLYWHEEL
	flywheel.toggleFlywheel();
	flywheel.setVelocity(310);
	pros::delay(3000);
	// END START FLYWHEEL

	// BEGIN PRE LOAD DISCS
	indexer.push();
	pros::delay(1000);
	indexer.push();
	pros::delay(1000);
	// END PRE LOAD DISCS

	// BEGIN MATCH LOAD DISCS
	pros::delay(2000);
	indexer.push();
	pros::delay(2000);
	indexer.push();
	pros::delay(2000);
	indexer.push();
	pros::delay(2000);
	indexer.push();
	pros::delay(2000);
	indexer.push();
	pros::delay(2000);
	indexer.push();
	pros::delay(2000);
	indexer.push();
	pros::delay(2000);
	// END MATCH LOAD DISCS
	flywheel.setVelocity(0);

	// BEGIN GO TO ROLLER
	drivetrain.drive->moveDistance(1_ft);
	pros::delay(500);
	drivetrain.turnToDegrees(270);
	pros::delay(500);
	drivetrain.drive->moveDistance(-4.5_ft);
	pros::delay(500);
	drivetrain.turnToDegrees(180);
	pros::delay(500);
	drivetrain.drive->moveDistance(-.5_ft);
	pros::delay(500);
	drivetrain.drive->moveDistance(2_ft);
	pros::delay(500);
	drivetrain.turnToDegrees(270);
	drivetrain.drive->moveDistance(-2_ft);
	pros::delay(500);
	drivetrain.drive->moveDistance(1.5_ft);
	pros::delay(500);
	drivetrain.turnToDegrees(225);


	// BEGIN TURN OFF FLYWHEEL
	flywheel.setVelocity(0);
	flywheel.toggleFlywheel();
	// END TURN OFF FLYWHEEL

	// MODIFY ============================

	// LEAVE =============================
	task.suspend();
	// LEAVE =============================
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
	// pros::ADIEncoder enc1 = pros::ADIEncoder({2, 1});
	// pros::ADIEncoder enc2 = pros::ADIEncoder({4, 3});
	// while(true) {
	// 	std::cout << "left: "  << enc1.get_value() << " right: " << enc2.get_value() << std::endl;
	// }
	pros::Task task{[=] {
		flywheel.bangBangLoop();
	}
	};

	std::shared_ptr<okapi::ChassisModel> chassis = drivetrain.drive->getModel();
	chassis->setMaxVelocity(600);
	chassis->setMaxVoltage(12000);
	while (true) {
	//	drivetrain.arcadeDrive(master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X));
		chassis->tank(master.get_analog(ANALOG_LEFT_Y) / (double)100, master.get_analog(ANALOG_RIGHT_Y) / (double)100);
		//std::cout << "encoder values: " << chassis->getSensorVals()[0] << " "<< chassis->getSensorVals()[1] << std::endl;
		//std::cout << "odom values: " << drivetrain.drive->getState().str(1_in, 1_deg) << std::endl;

		if(master.get_digital_new_press(DIGITAL_R2)) {
			std::cout << "toggling flywheel" << std::endl;
			flywheel.setVelocity(300);
			flywheel.toggleFlywheel();
		}
		if(master.get_digital_new_press(DIGITAL_R1)) {
			indexer.push();
		}
		if(master.get_digital(DIGITAL_L1)) {
			intakeMotor.move_voltage(9000);
		} else if(master.get_digital(DIGITAL_L2)) {
			intakeMotor.move_voltage(-9000);
		} else {
			intakeMotor.move_voltage(0);
		}

		pros::delay(20);
	}
}