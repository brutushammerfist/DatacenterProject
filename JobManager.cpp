#include "JobManager.h"

JobManager::JobManager() {

}

JobManager::JobManager(int numReducers) {
    this->numReducers = numReducers;

    int numJobs = 1;

    while ((double)((numJobs * numReducers) / 2560) < 0.8) {
        numJobs++;
    }

    this->numSimulJobs = numJobs;
}

JobManager::~JobManager() {

}

MapReduceJob* JobManager::newJob() {
    MapReduceJob* job = new MapReduceJob(this->nextID, this->numReducers);
    runningJobs.push_back(job);
    this->nextID++;
    return job;
}

int JobManager::getNumSimulJobs() {
    return this->numSimulJobs;
}