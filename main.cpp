#include <iostream>
#include <list>

#include "DatacenterController.h"

/*
 * Command line arguments
 * argv[0] = ./datacenter.exe
 * argv[1] = 
 */
int main(int argc, char** argv) {
    DatacenterController dcController = DatacenterController();

    const long endTime = 1209600; // Two weeks end time.
    long time = 0;

    //dcController.display();

    while (time < endTime) {
        time += 1;

        //std::cout << time << "\n";

        dcController.work();
        
        if (time % 3600 == 0) {
            dcController.shiftChange();
        }
    }

    std::cout << "Simulation complete!\n";

    return 0;
}