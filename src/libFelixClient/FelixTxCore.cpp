#include "FelixTxCore.h"

#include "logging.h"

namespace {
  auto ftlog = logging::make_log("FelixTxCore");
}

FelixTxCore::FelixTxCore() = default;

FelixTxCore::~FelixTxCore()
{
  if (m_trigProc.joinable()) m_trigProc.join();
}

// Channel control
void FelixTxCore::enableChannel(FelixID_t fid) {
  ftlog->debug("Enable Tx link: 0x{:x}", fid);
  if (checkChannel(fid)) {
    m_enables[fid] = true;
    m_fifo[fid];
  }
}

void FelixTxCore::disableChannel(FelixID_t fid) {
  ftlog->debug("Disable Tx link: 0x{:x}", fid);
  m_enables[fid] = false;
}

FelixTxCore::FelixID_t FelixTxCore::fid_from_channel(uint32_t chn) {
  // Compute FelixID from did, cid, link id elink #, streamId

  // Get link/GBT id and elink # from channel number
  // chn[18:6] is the link ID; chn[5:0] is the e-link number
  uint8_t elink = chn & 0x3f;
  uint16_t link_id = (chn >> 6) & 0x1fff;

  // Hard code is_virtual to false, and streamID to 0 for now
  bool is_virtual = false;
  uint8_t sid = 0;

  return FelixTools::get_fid(
    m_did, m_cid, is_virtual, link_id, elink, true, m_protocol, sid
    );
}

bool FelixTxCore::checkChannel(FelixID_t fid) {
  ftlog->debug("Try sending data to Tx link: 0x{:x}",fid);
  try {
    std::string empty;
    fclient->send_data(fid, (const uint8_t*)empty.c_str(), 1, true);
  } catch (std::runtime_error& e) {
    ftlog->warn("Fail to send to Tx link 0x{:x}: {}", fid, e.what());
    return false;
  }

  return true;
}

void FelixTxCore::setCmdEnable(uint32_t chn) {
  // Switch off all channels first
  disableCmd();

  auto fid = fid_from_channel(chn);
  enableChannel(fid);
}

void FelixTxCore::setCmdEnable(std::vector<uint32_t> chns) {
  // Switch off all channels first
  disableCmd();

  for (auto& c : chns) {
    auto fid = fid_from_channel(c);
    enableChannel(fid);
  }
}

void FelixTxCore::disableCmd() {
  for (auto& e : m_enables) {
    disableChannel(e.first);
  }
}

uint32_t FelixTxCore::getCmdEnable() { // unused
  return 0;
}

bool FelixTxCore::isCmdEmpty() {
  for (const auto& [chn, buffer] : m_fifo) {
    // consider only enabled channels
    if (not m_enables[chn])
      continue;

    if (not buffer.empty())
      return false;
  }

  return true;
  // Is there a way to check this from FelixClient?
}

void FelixTxCore::writeFifo(uint32_t value) {
  // write value to all enabled channels
  for (auto& [chn, buffer] : m_fifo) {
    if (m_enables[chn]) {
      ftlog->trace("FelixTxCore::writeFifo link=0x{:x} val=0x{:08x}", chn, value);
      fillFifo(buffer, value);
    }
  }
}

void FelixTxCore::fillFifo(std::vector<uint8_t>& fifo, uint32_t value) {
  // Break an unsigned int into four bytes
  // MSB first
  fifo.push_back( (value>>24) & 0xff );
  fifo.push_back( (value>>16) & 0xff );
  fifo.push_back( (value>>8) & 0xff );
  fifo.push_back( value & 0xff );
}

void FelixTxCore::prepareFifo(std::vector<uint8_t>& fifo) {

  if (m_flip) {
    ftlog->trace("Swap the top and botton four bits for every byte");
    for (uint8_t &word : fifo) {
      word = ((word & 0x0f) << 4) + ((word & 0xf0) >> 4);
    }
  }

  // padding, manchester still needed?
}

void FelixTxCore::releaseFifo() {
  ftlog->trace("NetioTxCore::releaseFifo");

  for (auto& [chn, buffer] : m_fifo) {
    // skip disabled channels
    if (not m_enables[chn])
      continue;

    ftlog->trace(" send to fid 0x{:x}", chn);

    prepareFifo(buffer);

    ftlog->trace("FIFO[{}][{}]: ", chn, buffer.size());
    for (const auto& word : buffer) {
      ftlog->trace(" {:02x}", word&0xff);
    }

    bool flush = false;
    //fclient->init_send_data(chn);
    fclient->send_data(chn, &buffer[0], buffer.size(), flush);
  }

  // clear buffers
  for (auto& [chn, buffer] : m_fifo) {
    if (not m_enables[chn]) continue;
    buffer.clear();
  }

}

void FelixTxCore::setTrigEnable(uint32_t value) {
  if (m_trigProc.joinable()) {
    m_trigProc.join();
  }

  if (value == 0) {
    m_trigEnabled = false;
  } else {
    m_trigEnabled = true;
    switch (m_trigCfg) {
    case INT_TIME:
    case EXT_TRIGGER:
      ftlog->debug("Starting trigger by time ({} seconds)", m_trigTime);
      m_trigProc = std::thread(&FelixTxCore::doTriggerTime, this);
      break;
    case INT_COUNT:
      ftlog->debug("Starting trigger by count ({} triggers)", m_trigCnt);
      m_trigProc = std::thread(&FelixTxCore::doTriggerCnt, this);
      break;
    default:
      ftlog->error("No config for trigger, aborting loop");
      m_trigEnabled = false;
      break;
    }
  }
}

uint32_t FelixTxCore::getTrigEnable() {
  return m_trigEnabled;
}

void FelixTxCore::maskTrigEnable(uint32_t value, uint32_t mask) { // never used
  return;
}

void FelixTxCore::toggleTrigAbort() {
  m_trigEnabled = false;
}

bool FelixTxCore::isTrigDone() {
  return (not m_trigEnabled and isCmdEmpty());
}

void FelixTxCore::setTrigConfig(enum TRIG_CONF_VALUE cfg) {
  m_trigCfg = cfg;
}

void FelixTxCore::setTrigFreq(double freq) {
  m_trigFreq = freq;
}

void FelixTxCore::setTrigCnt(uint32_t count) {
  m_trigCnt = count;
}

void FelixTxCore::setTrigTime(double time) {
  m_trigTime = time;
}

void FelixTxCore::setTrigWordLength(uint32_t length) {
  m_trigWordLength = length;
}

void FelixTxCore::setTrigWord(uint32_t *words, uint32_t size) {
  m_trigWords.clear();

  for (uint32_t i=0; i<size; i++) {
    m_trigWords.push_back(words[i]);
  }
}

void FelixTxCore::setTriggerLogicMask(uint32_t mask) {
  //Nothing to do yet
}

void FelixTxCore::setTriggerLogicMode(enum TRIG_LOGIC_MODE_VALUE mode) {
  //Nothing to do yet
}

void FelixTxCore::resetTriggerLogic() {
  //Nothing to do yet
}

uint32_t FelixTxCore::getTrigInCount() {
  return 0;
}

void FelixTxCore::prepareTrigger() {
  for (const auto& [chn, enable] : m_enables) {
    if (not enable) continue;

    m_trigFifo[chn].clear();

    // Need to send the last word in m_trigWords first
    // (Because of the way TriggerLoop sets up the trigger words)
    for (int j=m_trigWords.size()-1; j>=0; j--) {
      fillFifo(m_trigFifo[chn], m_trigWords[j]);
    }

    prepareFifo(m_trigFifo[chn]);
  }
}

void FelixTxCore::doTriggerCnt() {
  prepareTrigger();

  uint32_t trigs = 0;
  for (uint32_t i=0; i<m_trigCnt; i++) {
    if (not m_trigEnabled) break;
    trigs++;
    trigger();
    std::this_thread::sleep_for(std::chrono::microseconds((int)(1e6/m_trigFreq))); // Frequency in Hz
  }
  m_trigEnabled = false;
  ftlog->debug("Finished trigger count {}/{}", trigs, m_trigCnt);
}

void FelixTxCore::doTriggerTime() {
  prepareTrigger();

  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point cur = start;
  uint32_t trigs=0;
  while (std::chrono::duration_cast<std::chrono::seconds>(cur-start).count() < m_trigTime) {
    if (not m_trigEnabled) break;
    trigs++;
    trigger();
    std::this_thread::sleep_for(std::chrono::microseconds((int)(1000/m_trigFreq))); // Frequency in kHz
    cur = std::chrono::steady_clock::now();
  }
  m_trigEnabled = false;
  ftlog->debug("Finished trigger time {} with {} triggers", m_trigTime, trigs);
}

void FelixTxCore::trigger() {
  ftlog->trace("FelixTxCore::trigger");

  for (auto& [chn, buffer] : m_trigFifo) {
    if (not m_enables[chn]) continue;

    bool flush = false;
    fclient->send_data(chn, &buffer[0], buffer.size(), flush);
  }
}

void FelixTxCore::loadConfig(const json &j) {
  ftlog->info("FelixTxCore:");

  if (j.contains("flip")) {
    m_flip = j["flip"];
    ftlog->info(" flip = {}", m_flip);
  }

  if (j.contains("detector_id")) {
    m_did = j["detector_id"];
    ftlog->info(" did = {}", m_did);
  }
  if (j.contains("connector_id")) {
    m_cid = j["connector_id"];
    ftlog->info(" cid = {}", m_cid);
  }
  if (j.contains("protocol")) {
    m_protocol = j["protocol"];
    ftlog->info(" protocol = {}", m_protocol);
  }
}

void FelixTxCore::writeConfig(json& j) {
  j["detector_id"] = m_did;
  j["connector_id"] = m_cid;
  j["protocol"] = m_protocol;
  j["flip"] = m_flip;
}

void FelixTxCore::setClient(std::shared_ptr<FelixClientThread> client) {
  fclient = client;
}
