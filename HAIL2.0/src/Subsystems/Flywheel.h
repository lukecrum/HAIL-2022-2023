#include "main.h"
#include "../../include/main.h"
#include "../Constants.h"

// enum FlywheelState {
//     IDLE,
//     SPIN_UP,
//     AT_SPEED,
//     SPIN_DOWN,
//     ERROR
// }

class Flywheel {
    public:
        Flywheel();
        void toggleFlywheel();
        void setVoltage(double _voltage);
        void setVelocity(double _velocity);
        bool isRunning();
        void bangBangLoop();
    private:
        bool running;
        double velocity;
        double voltage;
        okapi::MotorGroup fwMotors = {FLYWHEEL_MOTOR_TOP, FLYWHEEL_MOTOR_BOTTOM};

};