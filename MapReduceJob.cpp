#include <iostream>
#include <random>

#include "MapReduceJob.h"

// Default Constructor
MapReduceJob::MapReduceJob() {
    std::uniform_int_distribution<int> completionTimeDistribution(3, 24);
    std::uniform_int_distribution<int> intermediateDataSizeDistribution(256, 1024);
    
    this->mapJob = nullptr;

    this->id = 0;
    this->inputSize = 500;
    this->estimatedCompletionTime = completionTimeDistribution(this->generator);
    this->intermediateDataSize = intermediateDataSizeDistribution(this->generator);
}

MapReduceJob::MapReduceJob(int id, int numReducers) {
    std::uniform_int_distribution<int> completionTimeDistribution(3, 24);
    std::uniform_int_distribution<int> intermediateDataSizeDistribution(256, 1024);
    
    this->id = id;
    this->numSubJobs = numReducers;
    this->inputSize = 500;
    this->estimatedCompletionTime = (completionTimeDistribution(this->generator)) * 3600;
    this->intermediateDataSize = intermediateDataSizeDistribution(this->generator);
    this->isComplete = false;

    this->mapJob = new SubJob(this, (int)(this->estimatedCompletionTime / 2) + 1, true, 500);

    for (int i = 0; i < this->numSubJobs; i++) {
        this->reduceJobs.push_back(new SubJob(this, (int)(this->estimatedCompletionTime / 2) + 1, false, this->intermediateDataSize));
        this->uploadedResults.push_back(0);
    }
}

// Default Destructor
MapReduceJob::~MapReduceJob() {

}

void MapReduceJob::addRepo(Vehicle* repo) {
    this->repositories.push_back(repo);
}

void MapReduceJob::removeRepo(Vehicle* repo) {
    this->repositories.remove(repo);
}

int MapReduceJob::getIntermediateSize() {
    return this->intermediateDataSize;
}

SubJob* MapReduceJob::getFirstReduceJob() {
    return this->reduceJobs.front();
}

std::list<SubJob*>& MapReduceJob::getReduceJobs() {
    return this->reduceJobs;
}

SubJob* MapReduceJob::getMapJob() {
    return this->mapJob;
}

bool MapReduceJob::complete() {
    return this->isComplete;
}

void MapReduceJob::checkComplete() {
    if (this->mapJob->isComplete()) {
        std::list<SubJob*>::iterator jitr = this->reduceJobs.begin();

        while (jitr != this->reduceJobs.end()) {
            if (!(*jitr)->isComplete()) {
                return;
            }
            jitr++;
        }

        this->isComplete = true;
    }
}

void MapReduceJob::printStatus() {
    bool pause = false;
    std::cout << "+------------------------------------+" << std::endl;
    std::cout << this->mapJob->migrating() << " | " << this->mapJob->timeToCompletion() << " | " << this->mapJob->assigned() << " | " << this->mapJob->getActualCompletionTime() << std::endl;

    if (this->mapJob->migrating() ) {
        pause = true;
    }

    std::cout << "--------------------------------------" << std::endl;

    std::list<SubJob*>::iterator jitr = this->reduceJobs.begin();

    while (jitr != this->reduceJobs.end()) {
        std::cout << (*jitr)->migrating() << " | " << (*jitr)->timeToCompletion() << " | " << (*jitr)->assigned() << " | " << (*jitr)->getActualCompletionTime() << std::endl;

        if ((*jitr)->migrating()) {
            pause = true;
        }
        jitr++;
    }

    std::cout << "+------------------------------------+" << std::endl;

    /*if (pause) {
        int temp = 0;
        std::cin >> temp;
    }*/
}