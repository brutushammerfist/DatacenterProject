#include <iostream>

#include "SubJob.h"

SubJob::SubJob() {
    this->mainJob = nullptr;

    this->map = false;

    this->currInputDownloaded = 0;
    this->inputDownloaded = false;

    this->estimatedCompletionTime = -1;
    this->actualCompletionTime = 0;
    this->workingTimeToCompletion = -1;

    
    this->workComplete = false;
    this->uploaded = false;
    this->complete = false;
}

SubJob::SubJob(MapReduceJob* mainJob, int estimated, bool map) {
    this->mainJob = mainJob;

    this->map = map;

    this->currInputDownloaded = 0;
    this->inputDownloaded = false;

    this->estimatedCompletionTime = estimated;
    this->actualCompletionTime = 0;
    this->workingTimeToCompletion = estimated;

    this->workComplete = false;
    this->uploaded = false;
    this->complete = false;
}

SubJob::~SubJob() {

}

void SubJob::work() {
    std::cout << "Working!!!\n";
}