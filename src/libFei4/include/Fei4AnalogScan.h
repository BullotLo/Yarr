#ifndef FEI4ANALOGSCAN_H
#define FEI4ANALOGSCAN_H

// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: Fei4 Analog Scan
// # Comment: Nothing fancy
// ################################

#include "ScanBase.h"

#include "AllStdActions.h"
#include "AllFei4Actions.h"

class Fei4AnalogScan : public ScanBase {
    public:
		Fei4AnalogScan(Bookkeeper *k);

        void init() override;
        void preScan() override;
        void postScan() override {}

    private:
        enum MASK_STAGE mask;
        enum DC_MODE dcMode;
        unsigned numOfTriggers;
        double triggerFrequency;
        unsigned triggerDelay;
};

#endif
