#include <iostream>
#include <list>

#include "DatacenterController.h"

/*
 * Command line arguments
 * argv[0] = ./datacenter.exe
 * argv[1] = Number of weeks to run simulation
 * argv[2] = Number of reducers to use(i.e. number of subjobs for the second phase of a job)
 */
int main(int argc, char** argv) {
    DatacenterController dcController = DatacenterController();

    const long endTime = atoi(argv[1]) * 604800; // Two weeks end time.
    long time = 0;

    while (time < endTime) {
        time += 1;

        dcController.work(time);
        
        if (time % 3600 == 0) {
            dcController.shiftChange();
        }
    }

    std::cout << "Simulation complete!\n";

    return 0;
}