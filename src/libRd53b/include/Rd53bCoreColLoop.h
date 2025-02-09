#ifndef RD53BCORECOLLOOP_H
#define RD53BCORECOLLOOP_H

// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: Loop over core columns for RD53B
// # Date: 07/2020
// ################################

#include "LoopActionBase.h"

class Rd53bCoreColLoop : public LoopActionBase {
    public:
        Rd53bCoreColLoop();

        void writeConfig(json &j) override;
        void loadConfig(const json &j) override;

    private:
        unsigned m_cur;
        unsigned m_nSteps;
        unsigned m_minCore;
        unsigned m_maxCore;
        bool m_usePToT;
        bool m_disUnused;

        std::array<uint16_t, 4> m_coreCols;

        void init() override;
        void end() override;
        void execPart1() override;
        void execPart2() override;

        void setCores();
};

#endif
