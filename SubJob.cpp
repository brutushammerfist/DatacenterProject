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

SubJob::SubJob(MapReduceJob* mainJob, int estimated, bool map, int inputSize) {
    this->mainJob = mainJob;

    this->map = map;

    this->inputSize = inputSize;
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

void SubJob::work(DatacenterController* dcController, AccessPoint* acPoint, Vehicle* hostVehicle, int time) {
    if (!complete) {
        // Job not finished running and uploading
        this->actualCompletionTime++;
        
        if (this->inputDownloaded) {
            // Input downloaded
            if (hostVehicle->isMigrating()) {
                // Migrating
                if (hostVehicle == acPoint->getBandwidthUser()) {
                    // Host vehicle able to use network
                    hostVehicle->migrate();
                }
            } else {
                if (workComplete) {
                    // Work completed, uploading now
                    if (hostVehicle == acPoint->getBandwidthUser()) {
                        // Host vehicle able to use network
                        if (this->map) {
                            // Is a Map Job
                            if (this->intermediateOneTransfered) {
                                // First saved, work on second
                                this->currTransferTwo += 54;

                                if (this->currTransferTwo >= this->mainJob->getIntermediateSize()) {
                                    // Transfer complete
                                    this->mainJob->addRepo(this->intermediateTargetTwo);
                                    this->intermediateTargetTwo->saveJob(this->mainJob);
                                    this->intermediateTwoTransfered = true;

                                    this->uploaded = true;
                                    this->complete = true;
                                    acPoint->swapBandwidthUser();
                                    hostVehicle->setJob(this->mainJob->getFirstReduceJob());
                                    dcController->scheduleReduce(this->mainJob);
                                }
                            } else {
                                // First not saved, work on first
                                this->currTransferOne += 54;

                                if (this->currTransferOne >= this->mainJob->getIntermediateSize()) {
                                    this->mainJob->addRepo(this->intermediateTargetOne);
                                    this->intermediateTargetOne->saveJob(this->mainJob);
                                    this->intermediateOneTransfered = true;
                                }
                            }
                        } else {
                            // Is not a Map Job
                            this->currUploaded += 54;

                            if (this->currUploaded >= this->mainJob->getIntermediateSize()) {
                                this->uploaded = true;
                                this->complete = true;
                                acPoint->swapBandwidthUser();
                                hostVehicle->setBusy(false);
                            }
                        }
                    }
                } else {
                    // Work Not Complete
                    this->workingTimeToCompletion--;

                    if (this->workingTimeToCompletion <= 0) {
                        this->workComplete = true;

                        if (this->map) {
                            this->intermediateTargetOne = acPoint->getRandomVehicle();
                            this->intermediateTargetTwo = dcController->getRandomVehicle(false);
                        }
                    }
                }
            }
        } else {
            // Input not downloaded
            if (hostVehicle == acPoint->getBandwidthUser()) {
                // Host vehicle can use network
                this->currInputDownloaded += 54;
                
                if (this->currInputDownloaded >= this->inputSize) {
                    this->inputDownloaded = true;
                }
            }
        }
    }
}

int SubJob::getInputSize() {
    return this->inputSize;
}