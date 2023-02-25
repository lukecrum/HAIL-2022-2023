#include "Indexer.h"

Indexer::Indexer() {

}

void Indexer::push() {
    solenoid.set_value(1);
    pros::delay(333);
    solenoid.set_value(0);
}