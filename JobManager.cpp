#include "JobManager.h"

JobManager::JobManager() {
    this->nextID = 0;
}

JobManager::JobManager(int numReducers) {
    this->numReducers = numReducers;
    this->nextID = 0;
}

JobManager::~JobManager() {
    
}

MapReduceJob* JobManager::newJob() {
    MapReduceJob* job = new MapReduceJob(this->nextID, this->numReducers);
    runningJobs.push_back(job);
    this->nextID++;
    return job;
}

void JobManager::work() {
    /*std::list<MapReduceJob*>::iterator jitr = this->runningJobs.begin();

    while (jitr != this->runningJobs.end()) {
        (*jitr)->work();
        jitr++;
    }*/
}

void JobManager::receiveUpload(SubJob* job) {
    std::list<MapReduceJob*>::iterator jitr = this->runningJobs.begin();

    while (jitr != this->runningJobs.end()) {
        if ((*jitr) == job->getMainJob()) {
            (*jitr)->receiveUpload(job);
        }
        jitr++;
    }
}