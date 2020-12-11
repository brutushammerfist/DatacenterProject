#include <iostream>

#include "Vehicle.h"

// Default Constructor
Vehicle::Vehicle() {
    this->id = -1;
    this->isBusy = false;
    this->vm = nullptr;
    this->migrating = false;
    this->currMigrated = 0;
}

// Constructor with ID as parameter
Vehicle::Vehicle(int id) {
    this->id = id;
    this->isBusy = false;
    this->vm = nullptr;
    this->arrival = 0;
    this->departure = 0;
    this->currMigrated = 0;
    this->migrating = false;
    this->migrationTarget = nullptr;
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

void Vehicle::work(DatacenterController* dcController, AccessPoint* acPoint, int time, int migrationType) {
    if (this->vm != nullptr) {
        this->vm->work(dcController, acPoint, this, time, migrationType);
    }
}

bool Vehicle::isMigrating() {
    return this->migrating;
}

void Vehicle::setJob(SubJob* job) {
    if (this->vm == nullptr) {
        this->vm = new VirtualMachine(job);
        this->isBusy = true;
    } else {
        this->vm->setJob(job);
        this->isBusy = true;
    }
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

void Vehicle::migrate(AccessPoint* acPoint) {
    this->currMigrated += 54;

    std::list<MapReduceJob*>::iterator jitr = this->savedJobs.begin();

    int migrateCap = this->vm->migrateSize();

    while (jitr != this->savedJobs.end()) {
        migrateCap += (*jitr)->getIntermediateSize();
        jitr++;
    }

    this->vm->getJob()->setCurrMigrated(this->currMigrated);

    if (this->currMigrated >= migrateCap) {
        this->migrationTarget->migrateVM(this->vm, this->savedJobs);
        this->migrating = false;
        this->vm->getJob()->setMigrating(false);
        this->isBusy = false;
        this->vm = nullptr;
        acPoint->swapBandwidthUser();
        this->currMigrated = 0;
    }
}

void Vehicle::migrateVM(VirtualMachine* vm, std::list<MapReduceJob*> &jobs) {
    this->vm = vm;

    this->savedJobs.merge(jobs);

    this->isBusy = true;
}

void Vehicle::setMigrate(bool migrate) {
    this->migrating = migrate;
}

int Vehicle::getDeparture() {
    return this->departure;
}

void Vehicle::leave() {
    if (this->vm != nullptr) {
        this->vm->restartJob();
    }
    this->vm = nullptr;
    this->isBusy = false;
    
    this->savedJobs.clear();
    this->arrival = -1;
    this->departure = -1;
    this->currMigrated = 0;
    this->migrating = false;
    this->migrationTarget = nullptr;
}

int Vehicle::getTimeUntilDeparture(int currTime) {
    return (this->departure - currTime);
}

int Vehicle::getMigrateSize() {
    int size = 0;
    
    if (this->vm != nullptr) {
        size += this->vm->migrateSize();
    }

    std::list<MapReduceJob*>::iterator jitr = this->savedJobs.begin();

    while (jitr != this->savedJobs.end()) {
        size += (*jitr)->getIntermediateSize();

        jitr++;
    }

    return size;
}

void Vehicle::setMigrationTarget(Vehicle* target) {
    this->migrationTarget = target;
}