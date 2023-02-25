#include "Drive.h"

void Drive::init() {

    // =============== external pid ==================
    drive = ChassisControllerBuilder()
    .withMotors({L1_PORT, L2_PORT, L3_PORT, L4_PORT}, {R1_PORT, R2_PORT, R3_PORT, R4_PORT}) // {left side motors}, {right side motors}
    .withDimensions(AbstractMotor::gearset::blue, {{TRACKER_WHEEL_DIAMETER, TRACKER_WHEEL_DISTANCE}, quadEncoderTPR}) // gearset, {{wheel diameter, distance b/w wheels (use tracking wheels)}
    .withSensors(
        ADIEncoder(LEFT_ENC_PORT_1, LEFT_ENC_PORT_2, true), // port 1, port 2, reversed (optional)
        ADIEncoder(RIGHT_ENC_PORT_1, RIGHT_ENC_PORT_2, true)
    )
    .withGains(
        {0.0043, 0, 0.000085}, //distance controller gains {0.003, 0, 0.00019} // 003, 00011 {0.0048, 0, 0.000093}
        {0.0015, 0, 0.00005}) //turn controller gains {0.003, 0, 0.00005})
      //  {0.001, 0, 0.00003}) //angle controller gains (drive straight)
    .withOdometry()
    .withLogger(
    std::make_shared<Logger>(
        TimeUtilFactory::createDefault().getTimer(), // It needs a Timer
        "/ser/sout", // Output to the PROS terminal
        Logger::LogLevel::debug // Most verbose log level
    )
    )
    .withMaxVoltage(6000)
    .buildOdometry();

    turnController = std::make_shared<okapi::IterativePosPIDController>(IterativeControllerFactory::posPID(turnP, turnI, turnD));
    turnController->setOutputLimits(1.0, -1.0);
    
    // =============================================

    // ============== built-in pid =================
    // drive = ChassisControllerBuilder()
    // .withMotors({L1_PORT, L2_PORT, L3_PORT, L4_PORT}, {R1_PORT, R2_PORT, R3_PORT, R4_PORT}) // {left side motors}, {right side motors}
    // .withDimensions(AbstractMotor::gearset::blue, {{DRIVE_WHEEL_DIAMETER, DRIVE_WHEEL_DISTANCE}, 1.25*imev5BlueTPR}) // gearset, {{wheel diameter, distance b/w wheels (use tracking wheels)}
    // .withSensors(
    //     ADIEncoder(LEFT_ENC_PORT_1, LEFT_ENC_PORT_2, true), // port 1, port 2, reversed (optional)
    //     ADIEncoder(RIGHT_ENC_PORT_1, RIGHT_ENC_PORT_2, true)
    // )
    // .withMaxVoltage(9000)
    // .withOdometry({{TRACKER_WHEEL_DIAMETER, TRACKER_WHEEL_DISTANCE}, quadEncoderTPR})
    // .buildOdometry();
    // =============================================
    // =========== no odom =========================
    // drive = ChassisControllerBuilder()
    // .withMotors({L1_PORT, L2_PORT, L3_PORT, L4_PORT}, {R1_PORT, R2_PORT, R3_PORT, R4_PORT}) // {left side motors}, {right side motors}
    // .withDimensions(AbstractMotor::gearset::blue, {{DRIVE_WHEEL_DIAMETER, DRIVE_WHEEL_DISTANCE}, 1.25*imev5BlueTPR}) // gearset, {{wheel diameter, distance b/w wheels (use tracking wheels)}
    // .withMaxVoltage(9000)
    // .build();
    // ===========================================
}

void Drive::turnToDegrees(double degrees) {
//    // imu.reset(); //reset imu
//    // imu.set_heading(120);
//    // pros::delay(2500); // pause 2000 ms, probably will find another solution for this later
//     turnController->setTarget(degrees);

//     std::cout << "[DRIVE] : IMU STARTING @ " << imu.get_heading() << std::endl;
//     std::cout << "[DRIVE] : DEGREES TO ROTATE : "  << degrees - imu.get_heading() << std::endl;
//     std::cout << "[DRIVE] : SET TARGET TO : " << degrees << std::endl << std::endl;

//     double original_angle = imu.get_heading();

//     drive->getModel()->rotate(turnController->step(imu.get_heading()));

//     while(!turnController->isSettled()) {
//         drive->getModel()->rotate(turnController->step(imu.get_heading())); //update turn with output of PID loop.
//         pros::delay(20);
//         std::cout << "controller output: " << turnController->getOutput() << std::endl;
//         std::cout << "current heading: " << imu.get_heading() << std::endl << std::endl;
//     }
//     drive->getModel()->rotate(0);

//     std::cout << "turn complete, settled at " << imu.get_heading() << " degrees. Total turn of " << imu.get_heading() - original_angle << std::endl;
//     pros::lcd::print(1, "turned %f", imu.get_heading() - original_angle);
    while(abs(degrees - imu.get_heading()) > 1) {
        if(abs(degrees - imu.get_heading()) < 10 && degrees > imu.get_heading()) {
            drive->getModel()->rotate(.02);
        } else if(abs(degrees - imu.get_heading()) < 10 && degrees < imu.get_heading()) {
            drive->getModel()->rotate(-.02);
        } else if(degrees > imu.get_heading()) {
            drive->getModel()->rotate(.15);
        } else {
            drive->getModel()->rotate(-.15);
        }
        pros::delay(5);
    }
    drive->getModel()->rotate(0);
    pros::delay(1000);
    pros::lcd::print(1, "error %f", degrees - imu.get_heading());
}

void Drive::resetIMU() {
    imu.reset();
    imu.set_heading(0);
}

double Drive::getHeading() {
    return imu.get_heading();
}