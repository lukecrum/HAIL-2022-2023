#include "main.h"
#include "../../include/main.h"
#include "../Constants.h"
#include <memory>
#include <iostream>

class Drive {
   // pros::MotorGroup right;
   // pros::MotorGroup left;
    public:
        std::shared_ptr<OdomChassisController> drive;
        std::shared_ptr<okapi::IterativePosPIDController> turnController;
        pros::IMU imu = pros::IMU(IMU_PORT);
        Drive() {}
        void init();
        void turnToDegrees(double degrees);
        void resetIMU();
        double getHeading();
};