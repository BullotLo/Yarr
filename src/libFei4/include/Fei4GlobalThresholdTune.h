#ifndef FEI4GLOBALTHRESHOLDTUNE_H
#define FEI4GLOBALTHRESHOLDTUNE_H

// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: Fei4 Threshold Scan
// # Comment: Nothing fancy
// ################################

#include "ScanBase.h"

#include "AllStdActions.h"
#include "AllFei4Actions.h"

class Fei4GlobalThresholdTune : public ScanBase {
    public:
		Fei4GlobalThresholdTune(Bookkeeper *k);
        void init() override;
        void preScan() override;
        void postScan() override {}
    private:
        enum MASK_STAGE mask;
        enum DC_MODE dcMode;
        unsigned numOfTriggers;
        double triggerFrequency;
        unsigned triggerDelay;
        bool useScap;
        bool useLcap;

        double target;
};

#endif
