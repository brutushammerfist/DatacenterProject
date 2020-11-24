#include <random>
#include "MapReduceJob.h"

// Default Constructor
MapReduceJob::MapReduceJob() {
    std::uniform_int_distribution<int> completionTimeDistribution(3, 24);
    std::uniform_int_distribution<int> intermediateDataSizeDistribution(256, 1024);
    
    this->id = 0;
    this->inputSize = 500;
    this->estimatedCompletionTime = completionTimeDistribution(this->generator);
    this->intermediateDataSize = intermediateDataSizeDistribution(this->generator);
}

// Default Destructor
MapReduceJob::~MapReduceJob() {

}