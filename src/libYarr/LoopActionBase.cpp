/*
 * Authors: K. Potamianos <karolos.potamianos@cern.ch>,
 *          T. Heim <timon.heim@cern.ch>
 * Date: 2013-Oct-22
 */

#include "LoopActionBase.h"

#include "logging.h"

namespace {
    auto log = logging::make_log("LoopActionBase");
}

LoopActionBase::LoopActionBase() : loopType(typeid(void)){
    g_fe = NULL;
    g_tx = NULL;
    g_rx = NULL;
    g_stat = NULL;
    m_done = false;
}

void LoopActionBase::setup(LoopStatus *stat, Bookkeeper *k) {
    SPDLOG_LOGGER_DEBUG(log);
    g_stat = stat;
    g_fe = k->g_fe;
    g_tx = k->tx;
    g_rx = k->rx;
	keeper = k;
}

void LoopActionBase::setNext(shared_ptr<LoopActionBase>& ptr) {
    m_inner = ptr;
}

void LoopActionBase::execute() {
    this->run();
}

bool LoopActionBase::done() {
    return m_done;
}

void LoopActionBase::execStep() {
    this->execPart1();
    
    if (m_inner) m_inner->execute();
    
    this->execPart2();
}

void LoopActionBase::run() {
    this->init();
    while(!this->done()) this->execStep();
    this->end();
}

unsigned LoopActionBase::getMin() {
    return min;
}

unsigned LoopActionBase::getMax() {
    return max;
}

unsigned LoopActionBase::getStep() {
    return step;
}

void LoopActionBase::setMax(unsigned v) {
    max = v;
}

void LoopActionBase::setMin(unsigned v) {
    min = v;
}

void LoopActionBase::setStep(unsigned v) {
    step = v;
}

bool LoopActionBase::checkGlobalDone() {
	bool done = true;
	for(unsigned int j=0; j<keeper->feList.size(); j++) {
		if(keeper->feList[j]->getActive() == true) {
		done = done & doneMap[dynamic_cast<FrontEndCfg*>(keeper->feList[j])->getChannel()];
		}
	}
	g_done = done;
	return g_done;

}
