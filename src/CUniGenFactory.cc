//
// Created by _amp_ on 8/3/24.
//

#include "CUniGenFactory.hh"

cola::VFilter *CUniGenFactory::create(const std::map<std::string, std::string> & paramMap) {
    if (paramMap.find("buff_size") != paramMap.end())
        return new CUniGenWriter(paramMap.at("fname"), std::stoi(paramMap.at("buff_size")));
    return new CUniGenWriter(paramMap.at("fname"));
}