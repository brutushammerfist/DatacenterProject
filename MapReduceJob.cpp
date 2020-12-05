#include <iostream>
#include <random>

#include "MapReduceJob.h"

// Default Constructor
MapReduceJob::MapReduceJob() {
    std::uniform_int_distribution<int> completionTimeDistribution(3, 24);
    std::uniform_int_distribution<int> intermediateDataSizeDistribution(250, 1000);
    
    this->id = 0;
    this->estimatedCompletionTime = completionTimeDistribution(this->generator) * 3600;
    this->intermediateDataSize = intermediateDataSizeDistribution(this->generator);
}

MapReduceJob::MapReduceJob(int id, int numSubJobs) {
    std::uniform_int_distribution<int> completionTimeDistribution(3, 24);
    std::uniform_int_distribution<int> intermediateDataSizeDistribution(250, 1000);
    
    this->id = id;
    this->estimatedCompletionTime = completionTimeDistribution(this->generator) * 3600;
    this->intermediateDataSize = intermediateDataSizeDistribution(this->generator);

    this->numSubJobs = numSubJobs;

    for (int i = 0; i < numSubJobs; i++) {
        this->reduceJobs.push_back(new SubJob(this, ((int)this->estimatedCompletionTime / 2) + 1));
        this->uploadedResults.push_back(0);
    }
}

// Default Destructor
MapReduceJob::~MapReduceJob() {

}

void MapReduceJob::work() {
    if (this->mapJob->complete()) {
        std::list<SubJob*>::iterator sitr = this->reduceJobs.begin();

        while (sitr != this->reduceJobs.end()) {
            (*sitr)->work();
            sitr++;
        }
    } else {
        this->mapJob->work();
    }
}

void MapReduceJob::receiveUpload(SubJob* job) {
    std::list<SubJob*>::iterator sitr = this->reduceJobs.begin();
    std::list<int>::iterator uitr = this->uploadedResults.begin();

    while (sitr != this->reduceJobs.end()) {
        if ((*sitr) == job) {
            (*uitr) += 54;

            if ((*uitr) >= (int)((this->intermediateDataSize / this->numSubJobs) + 1)) {
                (*sitr)->setUploaded();
            }
        }
        sitr++;
        uitr++;
    }
}


void MapReduceJob::removeRepo(Vehicle* repo) {
    this->repositories.remove(repo);
}

void MapReduceJob::addRepo(Vehicle* repo) {
    this->repositories.push_back(repo);
}