#include "JobManager.h"

JobManager::JobManager() {

}

JobManager::JobManager(int numReducers) {
    this->numReducers = numReducers;
}

JobManager::~JobManager() {

}

MapReduceJob* JobManager::newJob() {
    MapReduceJob* job = new MapReduceJob(this->nextID, this->numReducers);
    runningJobs.push_back(job);
    this->nextID++;
    return job;
}