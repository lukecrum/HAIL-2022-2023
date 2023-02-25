#include "main.h"
#include "../../include/main.h"
#include "../Constants.h"

class Indexer {
    public:
        Indexer();
        pros::ADIDigitalOut solenoid = pros::ADIDigitalOut(INDEXER_PORT);
        void push();
};