#ifndef ALLCHIPS_H
#define ALLCHIPS_H

#include "FrontEnd.h"

#include <memory>
#include <string>
#include <vector>
#include <functional>
namespace StdDict {
    bool registerFrontEnd(std::string name,
                              std::function<std::unique_ptr<FrontEnd>()> f);
    std::unique_ptr<FrontEnd> getFrontEnd(std::string name);

    std::vector<std::string> listFrontEnds();
}

#endif
