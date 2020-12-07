#include <iostream>

#include "Vehicle.h"

// Default Constructor
Vehicle::Vehicle() {
    this->id = -1;
    this->isBusy = false;
    this->vm = nullptr;
}

// Constructor with ID as parameter
Vehicle::Vehicle(int id) {
    this->id = id;
    this->isBusy = false;
    this->vm = nullptr;
}

// Default Destructor
Vehicle::~Vehicle() {

}

void Vehicle::display() {
    std::cout << "ID: " << this->id << "\n";
}

int Vehicle::getShift() {
    return this->id % 24;
}

void Vehicle::work(DatacenterController* dcController, AccessPoint* acPoint, int time) {
    if (this->vm != nullptr) {
        this->vm->work(dcController, acPoint, this, time);
    }
}

bool Vehicle::isMigrating() {
    return this->migrating;
}

void Vehicle::setJob(SubJob* job) {
    std::cout << "Setting job...\n";
    this->vm->setJob(job);
    std::cout << "Job set...\n";
    this->isBusy = true;
    std::cout << "Busy set...\n";
}

void Vehicle::setBusy(bool busy) {
    this->isBusy = busy;
}

void Vehicle::saveJob(MapReduceJob* job) {
    this->savedJobs.push_back(job);
    this->savedJobs.unique();
}

bool Vehicle::busy() {
    return this->isBusy;
}

void Vehicle::migrate() {
    this->currMigrated += 54;

    std::list<MapReduceJob*>::iterator jitr = this->savedJobs.begin();

    int migrateCap = this->vm->migrateSize();

    while (jitr != this->savedJobs.end()) {
        migrateCap += (*jitr)->getIntermediateSize();
        jitr++;
    }

    if (this->currMigrated >= migrateCap) {
        this->migrationTarget->migrateVM(this->vm, this->savedJobs);
        this->migrating = false;
    }
}

void Vehicle::migrateVM(VirtualMachine* vm, std::list<MapReduceJob*> &jobs) {
    this->vm = vm;

    this->savedJobs.merge(jobs);
}

void Vehicle::setMigrate(bool migrate) {
    this->migrating = migrate;
}

int Vehicle::getDeparture() {
    return this->departure;
}

void Vehicle::leave() {
    this->vm->restartJob();
    this->vm = nullptr;
    this->isBusy = false;
    
    this->savedJobs.clear();
    this->arrival = -1;
    this->departure = -1;
    this->currMigrated = 0;
    this->migrating = false;
    this->migrationTarget = nullptr;
}