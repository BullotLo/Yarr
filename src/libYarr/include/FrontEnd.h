#ifndef FRONTEND_H
#define FRONTEND_H

// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: Abstract FE class
// # Comment: Combined multiple FE 
// ################################

#include <string>

#include "ClipBoard.h"
#include "HistogramBase.h"
#include "EventDataBase.h"
#include "HwController.h"
#include "FrontEndGeometry.h"

#include "storage.hpp"

class FrontEnd {
    public:
        FrontEnd() {}
        virtual ~FrontEnd() {}
        
        virtual void init(HwController *arg_core, unsigned arg_txChannel, unsigned arg_rxChannel)=0;

        // col/row starting at 0,0
        virtual void maskPixel(unsigned col, unsigned row) = 0;

        /// Enable (disable mask) for all pixels
        virtual void enableAll() = 0;

        bool getActive();
		bool isActive();
		void setActive(bool active);
        virtual void makeGlobal(){};
       
        virtual void configure()=0;
        virtual int checkCom() {return 1;}

        /// Write to a register using a string name (most likely from json)
        virtual void writeNamedRegister(std::string name, uint16_t value) = 0;
        
        virtual void setInjCharge(double, bool, bool) = 0;

        // Set of events
        ClipBoard<EventDataBase> *clipData;
        ClipBoard<HistogramBase> *clipHisto;
        ClipBoard<HistogramBase> *clipResult;

        //Fei4Analysis *ana;
        //Fei4Histogrammer *histogrammer;
        
        FrontEndGeometry geo;

    protected:
        bool active;
        RxCore *m_rxcore;
};

class FrontEndCfg {
    public:
        FrontEndCfg() {
            name = "JohnDoe";
            txChannel = 99;
            rxChannel = 99;
            lockCfg = false;
        }
        virtual ~FrontEndCfg(){}
        

        virtual double toCharge(double)=0;
        virtual double toCharge(double, bool, bool)=0;
        virtual void toFileJson(json&)=0;
        virtual void fromFileJson(json&)=0;

        virtual std::tuple<json, std::vector<json>> getPreset(const std::string& systemType="SingleChip");

        unsigned getChannel() {return rxChannel;}
		unsigned getTxChannel() {return txChannel;}
		unsigned getRxChannel() {return rxChannel;}
        std::string getName() {return name;}
        
        void setChannel(unsigned channel) {txChannel = channel; rxChannel = channel;}
		void setChannel(unsigned arg_txChannel, unsigned arg_rxChannel) {txChannel = arg_txChannel; rxChannel = arg_rxChannel;}
        void setName(std::string arg_name) {name = arg_name;}

        void setConfigFile(std::string arg_configFile) {configFile = arg_configFile;}
        std::string getConfigFile() {return configFile;}
    
        bool isLocked() {return lockCfg;}
        void setLocked(bool v) {lockCfg = v;}
    protected:
        std::string name;
        unsigned txChannel;
        unsigned rxChannel;
        std::string configFile;
        bool lockCfg;
};

#endif
