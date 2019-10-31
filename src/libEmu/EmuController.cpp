#include "EmuController.h"

#include "AllHwControllers.h"

#include "Fei4Emu.h"
#include "Rd53aEmu.h"
#include "StarEmu.h"

template<class FE, class ChipEmu>
std::unique_ptr<HwController> makeEmu() {
  // nikola's hack to use RingBuffer
  std::unique_ptr<RingBuffer> rx(new RingBuffer(128));
  std::unique_ptr<RingBuffer> tx(new RingBuffer(128));

  std::unique_ptr<HwController> ctrl(new EmuController<FE, ChipEmu>(std::move(rx), std::move(tx)));

  return ctrl;
}

bool emu_registered_Fei4 =
  StdDict::registerHwController("emu",
                                makeEmu<Fei4, Fei4Emu>);

bool emu_registered_Rd53a =
  StdDict::registerHwController("emu_Rd53a",
                                makeEmu<Rd53a, Rd53aEmu>);

template<>
void EmuController<Fei4, Fei4Emu>::loadConfig(json &j) {
//    EmuTxCore::setCom(new EmuShm(j["tx"]["id"], j["tx"]["size"], true));
//    EmuRxCore::setCom(new EmuShm(j["rx"]["id"], j["rx"]["size"], true));

  auto tx = EmuTxCore<Fei4>::getCom();
  auto rx = EmuRxCore<Fei4>::getCom();

  //TODO make nice
  std::cout << "-> Starting Emulator" << std::endl;
  std::string emuCfgFile = j["feCfg"];
  std::cout << emuCfgFile << std::endl;
  emu.reset(new Fei4Emu(emuCfgFile, emuCfgFile, rx, tx));
  emuThreads.push_back(std::thread(&Fei4Emu::executeLoop, emu.get()));
}


template<>
void EmuController<Rd53a, Rd53aEmu>::loadConfig(json &j) {
//    EmuTxCore::setCom(new EmuShm(j["tx"]["id"], j["tx"]["size"], true));
//    EmuRxCore::setCom(new EmuShm(j["rx"]["id"], j["rx"]["size"], true));

  auto tx = EmuTxCore<Rd53a>::getCom();
  auto rx = EmuRxCore<Rd53a>::getCom();

  //TODO make nice
  std::cout << "-> Starting Emulator" << std::endl;
  std::string emuCfgFile = j["feCfg"];
  std::cout << emuCfgFile << std::endl;
  emu.reset(new Rd53aEmu( rx_com.get(), tx_com.get(), emuCfgFile ));
  emuThreads.push_back(std::thread(&Rd53aEmu::executeLoop, emu.get()));
}

template<>
void EmuController<StarChips, StarEmu>::loadConfig(json &j) {
//    EmuTxCore::setCom(new EmuShm(j["tx"]["id"], j["tx"]["size"], true));
//    EmuRxCore::setCom(new EmuShm(j["rx"]["id"], j["rx"]["size"], true));

  auto tx = EmuTxCore<StarChips>::getCom();
  auto rx = EmuRxCore<StarChips>::getCom();

  //TODO make nice
  std::cout << "-> Starting Emulator" << std::endl;
  std::string emuCfgFile = j["feCfg"];
  std::cout << emuCfgFile << std::endl;
  emu.reset(new StarEmu( rx_com.get(), tx_com.get(), emuCfgFile ));
  emuThreads.push_back(std::thread(&StarEmu::executeLoop, emu.get()));
}
