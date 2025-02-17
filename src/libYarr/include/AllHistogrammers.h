#ifndef YARR_ALL_HISTOGRAMMERS_H
#define YARR_ALL_HISTOGRAMMERS_H

#include "HistogramAlgorithm.h"

#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace StdDict {
    bool registerHistogrammer(std::string name,
                              std::function<std::unique_ptr<HistogramAlgorithm>()> f);
    std::unique_ptr<HistogramAlgorithm> getHistogrammer(std::string name);

    std::vector<std::string> listHistogrammers();
}

#endif
