#include <iostream>
#include <random>

#include "DatacenterController.h"

// Default Constructor
DatacenterController::DatacenterController() {
    for (int i = 0; i < 4; i++) {
        this->regions[i] = RegionController(this);
    }

    for (int i = 0; i < 7680; i++) {
        this->vehicles.push_back(new Vehicle(i));
    }

    this->initializeParkingLot();

    this->shiftToReplace = 0;
}

// Alternate Constructor that take percentage of busy cars
DatacenterController::DatacenterController(float percentBusy, int numReducers) {
    for (int i = 0; i < 7680; i++) {
        vehicles.push_back(new Vehicle(i));
    }

    this->initializeParkingLot();

    this->shiftToReplace = 0;
    this->percentBusyMax = percentBusy;
    this->jobManager = JobManager(numReducers);
}

// Default Destructor
DatacenterController::~DatacenterController() {

}

// Check if the datacenter is currently full of vehicles
bool DatacenterController::isFull() {
    for(int i = 0; i < 4; i++) {
        if (this->regions[i].isFull() == false) {
            return false;
        }
    }

    return true;
}

// Fill parking lot at beginning of simulation
void DatacenterController::initializeParkingLot() {
    for (int i = 0; i < 4; i++) {
        this->regions[i].initializeParkingLot(this->vehicles);
    }
}

// Fill any empty parking spots with vehicles
void DatacenterController::fillVehicles(int time) {
    for(int i = 0; i < 4; i++) {
        if (!this->regions[i].isFull()) {
            this->regions[i].fillVehicles(this->shiftToReplace, this->vehicles, time);
        }
    }
}

// Display every vehicle within the datacenter
void DatacenterController::display() {
    for (int i = 0; i < 4; i++) {
        this->regions[i].display();
    }
}

// Make a shift change, swapping 320 vehicles currently in the datacenter with fresh vehicles
void DatacenterController::shiftChange(int time) {
    for (int i = 0; i < 4; i++) {
        this->regions[i].shiftChange(this->shiftToReplace, this->vehicles);
    }

    this->fillVehicles(time);
    
    if (this->shiftToReplace == 23) {
        this->shiftToReplace = 0;
    } else {
        this->shiftToReplace++;
    }
}

// Tick ever "second" of the simulation
void DatacenterController::work() {
    for (int i = 0; i < 4; i++) {
        this->regions[i].work();
    }
    
    //percentBusy = this->calcPercentBusy();

    //this->jobManager.work();
}

// Calculate the percentage of vehicles that are currently busy working on an operation
float DatacenterController::calcPercentBusy() {
    int sumBusy = 0;
    
    for (int i = 0; i < 4; i++) {
        sumBusy += this->regions[i].getNumBusy();
    }

    return (sumBusy / 2560);
}

void DatacenterController::receiveUpload(SubJob* job) {
    this->jobManager.receiveUpload(job);
}

Vehicle* DatacenterController::getRandomVehicle() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> random(0, 3);

    return this->regions[random(generator)].getRandomVehicle();
}