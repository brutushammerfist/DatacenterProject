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
void DatacenterController::shiftChange() {
    for (int i = 0; i < 4; i++) {
        this->regions[i]->shiftChange(this->shiftToReplace, this->vehicles);
    }

    this->fillVehicles();
    
    if (this->shiftToReplace == 23) {
        this->shiftToReplace = 0;
    } else {
        this->shiftToReplace++;
    }
}

void DatacenterController::work(int time) {
    for (int i = 0; i < 4; i++) {
        this->regions[i]->work(this, time);
    }
}

Vehicle* DatacenterController::getRandomVehicle(bool notBusy) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> random(0, 3);

    if (notBusy) {
        Vehicle* vehicle = this->regions[random(generator)]->getRandomVehicle();

        while (vehicle->busy()) {
            vehicle = this->regions[random(generator)]->getRandomVehicle();
        }

        return vehicle;
    } else {
        return this->regions[random(generator)]->getRandomVehicle();
    }
}

void DatacenterController::scheduleReduce(MapReduceJob* job) {
    std::list<SubJob*> jobs = job->getReduceJobs();
    std::list<SubJob*>::iterator jitr = jobs.begin();
    jitr++;
    
    while (jitr != jobs.end()) {
        this->getRandomVehicle(true)->setJob((*jitr));
        jitr++;
    }
}

void DatacenterController::initializeJobs() {
    MapReduceJob* job = nullptr;

    for (int i = 0; i < this->jobManager->getNumSimulJobs(); i++) {
        job = this->jobManager->newJob();
        this->getRandomVehicle(true)->setJob(job->getMapJob());
    }
}