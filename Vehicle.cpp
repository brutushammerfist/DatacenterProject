#include <iostream>

#include "Vehicle.h"

// Default Constructor
Vehicle::Vehicle() {
    this->id = -1;
    this->isBusy = false;
    this->vm = nullptr;
    this->migrationTarget = nullptr;
    this->arrival = -1;
    this->departure = -1;

    this->finishedMigrating = false;
    this->incomingMigrationSize = -1;
    this->clusterIntermediateTarget = nullptr;
    this->randomIntermediateTarget = nullptr;
}

// Constructor with ID as parameter
Vehicle::Vehicle(int id) {
    this->id = id;
    this->isBusy = false;
    this->vm = nullptr;
    this->migrationTarget = nullptr;
    this->arrival = -1;
    this->departure = -1;
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

void Vehicle::setArrival(int time) {
    this->arrival = time;
}

void Vehicle::setDeparture(int time) {
    this->departure = time;
}

int Vehicle::getDeparture() {
    return this->departure;
}

bool Vehicle::hasVM() {
    if (this->vm == nullptr) {
        return false;
    } else {
        return true;
    }
}

bool Vehicle::busy() {
    return this->isBusy;
}

void Vehicle::leave() {
    this->vm->leave();

    this->isBusy = false;
    this->vm = nullptr;
    
    this->arrival = -1;
    this->departure = -1;

    this->finishedMigrating = false;
    this->migrationTarget = nullptr;
    this->incomingMigrationSize = -1;

    std::list<MapReduceJob*>::iterator jitr = this->savedJobs.begin();

    while (jitr != this->savedJobs.end()) {
        (*jitr)->removeRepo(this);
    }

    this->savedJobs.empty();
}

void Vehicle::work() {
    
}

bool Vehicle::isMigratingVM() {
    return this->vm->migrating();
}

bool Vehicle::uploadToDatacenter(DatacenterController* dcController) {
    dcController->receiveUpload(this->vm->getJob());

    return this->vm->getJob()->uploaded();
}

bool Vehicle::networkWork(DatacenterController* dcController, AccessPoint* acPoint) {
    if (this->isMigratingVM()) {
        // Migrating...
        this->migrationTarget->migrate();

        if (this->migrationTarget->isFinishedMigrating()) {
            // Done Migrating
            this->migrationTarget->setVM(this->vm);
            this->vm = nullptr;
            return true;
        }

        return false;
    } else if (this->vm->getJob()->isMapJob() && this->vm->getJob()->complete() && !this->vm->getJob()->uploaded()) {
        // Saving intermediate data!
        return this->saveIntermediate(dcController, acPoint);
    } else {
        // Uploading data to datacenter!
        return this->uploadToDatacenter(dcController);
    }
}

void Vehicle::migrate() {
    this->finishedMigrating = true;
}

bool Vehicle::isFinishedMigrating() {
    return this->finishedMigrating;
}

void Vehicle::setVM(VirtualMachine* vm) {
    this->vm = vm;
}

bool Vehicle::isUploading() {
    return this->uploading;
}

bool Vehicle::saveIntermediate(DatacenterController* dcController, AccessPoint* acPoint) {
    /*this->vm->getJob()->getMainJob()->addRepo(this);
    this->savedJobs.push_back(this->vm->getJob()->getMainJob());*/

    //ACCOUNT FOR NETWORK!

    /*Vehicle* randomVehicleAC = acPoint->getRandomVehicle();
    this->vm->getJob()->getMainJob()->addRepo(randomVehicleAC);
    randomVehicleAC->saveJob(this->vm->getJob()->getMainJob());

    Vehicle* randomVehicleDC = dcController->getRandomVehicle();
    this->vm->getJob()->getMainJob()->addRepo(randomVehicleDC);
    randomVehicleDC->saveJob(this->vm->getJob()->getMainJob());*/

    if (this->clusterIntermediateTarget == nullptr and this->randomIntermediateTarget == nullptr) {
        this->clusterIntermediateTarget = acPoint->getRandomVehicle();
        this->randomIntermediateTarget = dcController->getRandomVehicle();

        this->intermedOneSaveSize = 0;
        this->intermedTwoSaveSize = 0;
    }

    if (this->vm->getJob()->intermedOneSaved()) {
        this->intermedTwoSaveSize += 54;
        if (this->intermedTwoSaveSize >= this->vm->getJob()->getMainJob()->getIntermedSize()) {
            this->vm->getJob()->getMainJob()->addRepo(this->randomIntermediateTarget);
            this->randomIntermediateTarget->saveJob(this->vm->getJob()->getMainJob());
            this->vm->getJob()->setIntermedTwoSaved(true);
        }
    } else {
        this->intermedOneSaveSize += 54;
        if (this->intermedOneSaveSize >= this->vm->getJob()->getMainJob()->getIntermedSize()) {
            this->vm->getJob()->getMainJob()->addRepo(this->clusterIntermediateTarget);
            this->clusterIntermediateTarget->saveJob(this->vm->getJob()->getMainJob());
            this->vm->getJob()->setIntermedOneSaved(true);
        }       
    }

    if (this->vm->getJob()->intermedOneSaved() && this->vm->getJob()->intermedTwoSaved()) {
        return true;
    } else {
        return false;
    }
}

void Vehicle::saveJob(MapReduceJob* job) {
    this->savedJobs.push_back(job);
}