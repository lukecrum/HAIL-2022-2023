#include "main.h"
#include "../../include/main.h"
#include "../Constants.h"

class Intake {
    public:
        Intake();
        pros::Motor intakeMotor = pros::Motor(8);
};