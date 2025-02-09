/*
 * Author: K. Potamianos <karolos.potamianos@cern.ch>
 * Date: 2016-VI-25
 * Description: this is a port of the FE-I4 emulator for IBLROD (2014-VI-12)
 * Updates: addition of command decoding in execute loop, functions for handling commands, pixel modeling, shared memory communication, etc by N. Whallon <alokin@uw.edu>
 */

#ifndef __FEI4_EMU_H__
#define __FEI4_EMU_H__

#define HEXF(x,y) std::hex << "0x" << std::hex << std::setw(x) << std::setfill('0') << static_cast<int>(y) << std::dec

#include "Fei4Cfg.h"
#include "EmuCom.h"
#include "Gauss.h"
#include "PixelModel.h"
#include "FrontEndGeometry.h"


#include <cstdint>
#include <memory>

class Fei4Emu {
    public:
        Fei4Emu(const json &model_cfg, EmuCom *rx, EmuCom *tx);
        ~Fei4Emu();

        // the main loop which recieves commands from yarr
        void executeLoop();

        // functions for handling the recieved commands
        void handleTrigger();
        void handleWrFrontEnd(uint32_t chipid, uint32_t bitstream[21]);
        void handleWrRegister(uint32_t chipid, uint32_t address, uint32_t value);
        void handleRunMode(uint32_t chipid, int command);
        void handleGlobalPulse(uint32_t chipid);

        // functions for dealing with sending data to yarr
        void pushOutput(uint32_t value) const;

        // Call getFeStream() to see response from FE
        void decodeCommand(uint8_t* cmdStream, std::size_t size);

        // Only public function so far: to be moved to private once command decoder is fully implemented
        void addRandomHits(uint32_t nHits);

        void addPhysicsHits();

        /// Adds hit to output
        /// @tot1, @tot2: expressed in "real" terms (connected to ToT code later)
        void addHit(uint16_t col, uint16_t row, uint8_t tot1, uint8_t tot2);
        uint8_t getToTCode(uint8_t dec_tot);

        void addServiceRecord(bool isInfoSR);
        void startFrame();
        void endFrame();
        void addDataHeader(bool hasErrorFlags);
        void addDataRecord(uint16_t col, uint16_t row, uint8_t tot1, uint8_t tot2);
        void addAddressRecord(uint16_t address, bool isGR);
        void addValueRecord(uint16_t value);

        void processL1A();
        void processBCR();
        void processECR();
        void processCAL();
        void processSLOW(uint8_t *cmdPtr);

        EmuCom * m_txRingBuffer;
        EmuCom * m_rxRingBuffer;
        std::shared_ptr<Fei4Cfg> m_feCfg;

        uint32_t m_modeBits;
        uint32_t m_shiftRegisterBuffer[40][21];

        // these are not quite used - they are just set to 0
	FrontEndGeometry m_feGeo; // todo: put as template arguments (or at least tie to config type
        uint8_t m_feId;
        uint32_t m_l1IdCnt;
        uint32_t m_bcIdCnt;

	PixelModel* m_pixelModelObjects[80][336];
	void initializePixelModelsFromFile(const json &cfg);

        // this is the file path to output the pixel model configuration
        std::string m_output_model_cfg;

        /// Shut down executeLoop when set
        std::atomic<bool> run;
};

#endif //__FEI4_EMU_H__
