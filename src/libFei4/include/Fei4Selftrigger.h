#ifndef FEI4SELFTRIGGER_H
#define FEI4SELFTRIGGER_H

// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: Fei4 Noise Scan
// # Comment: Nothing fancy
// ################################

#include "ScanBase.h"

#include "AllStdActions.h"
#include "AllFei4Actions.h"

class Fei4Selftrigger : public ScanBase {
    public:
        Fei4Selftrigger(Bookkeeper *k);
        
        void init() override;
        void preScan() override;
        void postScan() override {}

    private:
        double triggerFrequency;
        unsigned triggerTime; 
};

#endif
