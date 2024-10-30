//
// Created by _amp_ on 8/3/24.
//

#ifndef CUNIGEN_CUNIGENFACTORY_HH
#define CUNIGEN_CUNIGENFACTORY_HH

#include "COLA.hh"

#include "CUniGenWriter.hh"

class CUniGenFactory final: public cola::VFactory {
public:
    cola::VFilter* create(const std::map<std::string, std::string>&) final;
};

#endif //CUNIGEN_CUNIGENFACTORY_HH

