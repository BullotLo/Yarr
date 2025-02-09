#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: DataProc base class
// # Comment: Operates on data from the clipboard
// ################################

#include <mutex>
/* #include <thread> */
#include <condition_variable>

#include "ClipBoard.h"
#include "RawData.h"
#include "EventDataBase.h"
#include "HistogramBase.h"
#include "ScanBase.h"

class DataProcessor {
    public:
        DataProcessor();
        virtual ~DataProcessor() = default;;

        // TODO there must be a nicer way for this
        virtual void connect(FrontEndCfg *feCfg, ClipBoard<RawDataContainer> *arg_input, ClipBoard<EventDataBase> *arg_output) {}
        virtual void connect(ClipBoard<EventDataBase> *arg_input, ClipBoard<HistogramBase> *arg_output) {}
        virtual void connect(ScanBase *arg_s, ClipBoard<HistogramBase> *arg_input, ClipBoard<HistogramBase> *arg_output) {}
	virtual void setThreads(unsigned n) {  m_numThreads = n ;}
        virtual void init() {}
        virtual void process() {}
        virtual void run() = 0;
        virtual void join() = 0;

        // TODO make getter/setter
        unsigned m_numThreads;	
    protected:
        std::condition_variable cv;
        std::mutex mtx;

};

#endif
