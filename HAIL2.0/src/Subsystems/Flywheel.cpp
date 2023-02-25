#include "Flywheel.h"

Flywheel::Flywheel() {
    fwMotors.setGearing(AbstractMotor::gearset::blue);
    fwMotors.setBrakeMode(AbstractMotor::brakeMode::coast);
    running = false;
    velocity = 0;
}

void Flywheel::toggleFlywheel() {
    if(running) { // turn off
        //fwMotors.moveVelocity(0);
        velocity = 0;
        running = false;
    } else { // turn on
        //fwMotors.moveVoltage(voltage);
        running = true;
    }
}

void Flywheel::setVoltage(double _voltage) {
    voltage = _voltage;
}

void Flywheel::setVelocity(double _velocity) {
    velocity = _velocity;
}

bool Flywheel::isRunning() {
    return running;
}

void Flywheel::bangBangLoop() {
    while(true) {
        if(velocity != 0 && running) {
           // std::cout << "velocity: " << fwMotors.getActualVelocity() << " target: " << velocity << std::endl;
            if(fwMotors.getActualVelocity() < velocity) {
                fwMotors.moveVoltage(12000);
            } else {
                fwMotors.moveVoltage(0);
            }
        } else {
            fwMotors.moveVoltage(0);
        }
        pros::delay(10);
    }
}