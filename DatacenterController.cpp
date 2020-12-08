#include <ctime>
#include <iostream>
#include <random>

#include "DatacenterController.h"

// Default Constructor
DatacenterController::DatacenterController() {
    for (int i = 0; i < 4; i++) {
        this->regions[i] = new RegionController();
    }
    
    for (int i = 0; i < 7680; i++) {
        vehicles.push_back(new Vehicle(i));
    }

    this->initializeParkingLot();

    this->shiftToReplace = 0;

    this->jobManager = new JobManager();

    this->initializeJobs();
}

// Default Destructor
DatacenterController::~DatacenterController() {

}

DatacenterController::DatacenterController(int numReducers) {
    for (int i = 0; i < 4; i++) {
        this->regions[i] = new RegionController();
    }
    
    for (int i = 0; i < 7680; i++) {
        vehicles.push_back(new Vehicle(i));
    }

    this->initializeParkingLot();

    this->shiftToReplace = 0;

    this->jobManager = new JobManager(numReducers);

    this->initializeJobs();
}

// Check if the datacenter is currently full of vehicles
bool DatacenterController::isFull() {
    for(int i = 0; i < 4; i++) {
        if (this->regions[i]->isFull() == false) {
            return false;
        }
    }

    return true;
}

// Fill parking lot at beginning of simulation
void DatacenterController::initializeParkingLot() {
    for (int i = 0; i < 4; i++) {
        this->regions[i]->initializeParkingLot(this->vehicles);
    }
}

// Fill any empty parking spots with vehicles
void DatacenterController::fillVehicles() {
    for(int i = 0; i < 4; i++) {
        if (!this->regions[i]->isFull()) {
            this->regions[i]->fillVehicles(this->shiftToReplace, this->vehicles);
        }
    }
}

// Display every vehicle within the datacenter
void DatacenterController::display() {
    for (int i = 0; i < 4; i++) {
        this->regions[i]->display();
    }
}

// Make a shift change, swapping 320 vehicles currently in the datacenter with fresh vehicles
void DatacenterController::shiftChange(int time) {
    for (int i = 0; i < 4; i++) {
        this->regions[i]->shiftChange(this->shiftToReplace, this->vehicles, time);
    }

    this->fillVehicles();
    
    if (this->shiftToReplace == 23) {
        this->shiftToReplace = 0;
    } else {
        this->shiftToReplace++;
    }
}

void DatacenterController::work(int time, int migrationType) {
    std::cout << time << std::endl;
    for (int i = 0; i < 4; i++) {
        this->regions[i]->work(this, time, migrationType);
    }
}

Vehicle* DatacenterController::getRandomVehicle(bool notBusy) {
    if (notBusy) {
        Vehicle* vehicle = this->regions[(rand() % 4)]->getRandomVehicle();

        while (vehicle->busy()) {
            vehicle = this->regions[(rand() % 4)]->getRandomVehicle();
        }

        return vehicle;
    } else {
        return this->regions[(rand() % 4)]->getRandomVehicle();
    }
}

void DatacenterController::scheduleReduce(MapReduceJob* job) {
    std::list<SubJob*> jobs = job->getReduceJobs();
    std::list<SubJob*>::iterator jitr = jobs.begin();
    jitr++;
    
    while (jitr != jobs.end()) {
        (*jitr)->setAssigned(true);
        this->getRandomVehicle(true)->setJob((*jitr));
        jitr++;
    }
}

void DatacenterController::initializeJobs() {
    MapReduceJob* job = nullptr;

    for (int i = 0; i < this->jobManager->getNumSimulJobs(); i++) {
        job = this->jobManager->newJob();
        job->getMapJob()->setAssigned(true);
        this->getRandomVehicle(true)->setJob(job->getMapJob());
    }
}

void DatacenterController::checkJobs() {
    this->jobManager->checkJobs(this);

    if (this->jobManager->getNumCurrRunning() < this->jobManager->getNumSimulJobs()) {
        MapReduceJob* job = this->jobManager->newJob();
        job->getMapJob()->setAssigned(true);
        this->getRandomVehicle(true)->setJob(job->getMapJob());
    }
}

int DatacenterController::numCompletedJobs() {
    return this->jobManager->getNumCompleted();
}

void DatacenterController::assignJob(SubJob* job) {
    this->getRandomVehicle(true)->setJob(job);
    job->setAssigned(true);
}

Vehicle* DatacenterController::findMigrationMatch(int timeUntilCompletion, int dataSize, int currTime) {
    Vehicle* vehicle = nullptr;
    
    for (int i = 0; i < 4; i++) {
        vehicle = this->regions[i]->findMigrationMatch(timeUntilCompletion, dataSize, currTime);
        if (vehicle != nullptr) {
            return vehicle;
        }
    }

    return vehicle;
}

void DatacenterController::displayRunningJobStatus() {
    this->jobManager->displayRunningJobStatus();
}