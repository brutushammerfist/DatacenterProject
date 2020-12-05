#include "SubJob.h"

SubJob::SubJob() {
    
}

SubJob::SubJob(MapReduceJob* mainJob, int estimated) {
    this->mainJob = mainJob;

    this->currInputDownloaded = 0;
    this->inputDownloaded = false;

    this->estimatedCompletionTime = estimated;
    this->actualCompletionTime = 0;
    this->workingTimeToCompletion = estimated;

    this->isComplete = false;
    this->isMap = false;
    this->uploadOne = false;
    this->uploadTwo = false;
}

SubJob::~SubJob() {

}

void SubJob::work() {
    if (!this->hostVehicle->isMigratingVM()) {
        if (!this->isComplete) {
            // Not done yet!
            if (this->inputDownloaded) {
                // Work!
                this->actualCompletionTime++;
                this->workingTimeToCompletion--;
            } else {
                // Attempt to download input!
                this->actualCompletionTime++;
            }
        } else {
            // Upload results to job manager!
            this->actualCompletionTime++;
        }
    }
}

void SubJob::restart() {
    this->currInputDownloaded = 0;
    this->inputDownloaded = false;

    this->workingTimeToCompletion = this->estimatedCompletionTime;

    this->isComplete = false;

    this->isUploaded = false;
    this->uploadOne = false;
    this->uploadTwo = false;
}

bool SubJob::complete() {
    return this->isComplete;
}

MapReduceJob* SubJob::getMainJob() {
    return this->mainJob;
}

void SubJob::setUploaded() {
    this->isUploaded = true;
}

void SubJob::setVehicle(Vehicle* vehicle) {
    this->hostVehicle = vehicle;
}

bool SubJob::uploaded() {
    if (this->isMap) {
        return (this->uploadOne && this->uploadTwo);
    } else {
        return this->isUploaded;
    }
}

bool SubJob::intermedOneSaved() {
    return this->uploadOne;
}

bool SubJob::intermedTwoSaved() {
    return this->uploadTwo;
}

void SubJob::setIntermedOneSaved(bool saved) {
    this->uploadOne = saved;
}

void SubJob::setIntermedTwoSaved(bool saved) {
    this->uploadTwo = saved;
}