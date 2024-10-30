//
// Created by _amp_ on 8/3/24.
//

#ifndef CUNIGEN_CUNIGENWRITER_HH
#define CUNIGEN_CUNIGENWRITER_HH

#include <utility>

#include "URun.hh"
#include "UEvent.hh"
#include "UParticle.hh"

#include "COLA.hh"
#include "Riostream.h"
#include "TTree.h"
#include "TFile.h"

class CUniGenWriter final: public cola::VWriter {
private:
    std::unique_ptr<TFile> outputFile;
    std::unique_ptr<TTree> outputTree;
    std::unique_ptr<UEvent> curEvent;
    std::unique_ptr<URun> run;

    bool _runFilled;

    const size_t buffSize;
    size_t count;
public:
    CUniGenWriter() = delete;
    CUniGenWriter(const CUniGenWriter&) = delete;
    CUniGenWriter(CUniGenWriter&&) = delete;
    CUniGenWriter& operator=(const CUniGenWriter&) = delete;
    CUniGenWriter& operator=(CUniGenWriter&&) = delete;
    CUniGenWriter(const std::string& fName, size_t buffSize = 100);
    ~CUniGenWriter() final;

    void operator()(std::unique_ptr<cola::EventData>&&) final;
};
#endif //CUNIGEN_CUNIGENWRITER_HH
