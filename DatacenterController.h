#pragma once

#include "RegionController.h"

class DatacenterController {
    private:
        RegionController regions[4];
    public:
        DatacenterController();
        ~DatacenterController();
};