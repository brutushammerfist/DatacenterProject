#include <random>
#include "MapReduceJob.h"

// Default Constructor
MapReduceJob::MapReduceJob() {
    uniform_int_distribution<int> completionTimeDistribution(3, 24);
    uniform_int_distribution<int> intermediateDataSizeDistribution(256, 1024);
    
    this->inputSize = 500;
    this->completionTime = completionTimeDistribution(this->generator);
    this->intermediateDataSize = intermediateDataSizeDistribution(this->generator);
}

// Default Destructor
MapReduceJob::~MapReduceJob() {

}