#pragma once

#include "AccessPoint.h"

class GroupController {
    private:
        AccessPoint accessPoints[4];
    public:
        GroupController();
        ~GroupController();
};