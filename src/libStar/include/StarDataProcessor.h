#ifndef ABC_STAR_DATA_PROCESSOR_H
#define ABC_STAR_DATA_PROCESSOR_H

// #################################
// # Author:
// # Email:
// # Project: Yarr
// # Description: Strip Data Processor
// # Comment:
// ################################

#include <vector>
#include <array>
#include <map>
#include <thread>

#include "DataProcessor.h"
#include "ClipBoard.h"
#include "RawData.h"

class StarDataProcessor : public DataProcessor {
    public:
        // TODO processor should receive whole chip config seperatly
        StarDataProcessor();
        ~StarDataProcessor() override;
        
        void connect(ClipBoard<RawDataContainer> *arg_input, ClipBoard<EventDataBase> *arg_output) override {
            input = arg_input;
            output = arg_output;
        }
    
        void init() override;
        void run() override;
        void join() override;
        void process() override;
        void process_core();

    private:
        ClipBoard<RawDataContainer> *input;
        ClipBoard<EventDataBase> *output;
        std::vector<unsigned> activeChannels;
        std::vector<std::unique_ptr<std::thread>> thread_ptrs;
};

#endif
