/**
* CRoot - COLA Library Module for ROOT data storage support.
* Copyright (C) 2025 Savva Savenkov
*
* This file is part of CRoot
*
* CRoot is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* CRoot is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with CRoot.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CROOT_CROOTWRITER_HH
#define CROOT_CROOTWRITER_HH

#include "URun.hh"
#include "UEvent.hh"
#include "UParticle.hh"

#include "COLA.hh"
#include "Riostream.h"
#include "TTree.h"
#include "TFile.h"

class CRootWriter: public cola::VWriter {
private:
    std::unique_ptr<TFile> outputFile;
    const size_t buffSize;

protected:
    std::unique_ptr<TTree> outputTree;
    size_t count;

    virtual void _write_event(std::unique_ptr<cola::EventData>&&) = 0;

public:
    CRootWriter() = delete;
    CRootWriter(const std::string& fName, size_t buffSize, const char* treeName);

    CRootWriter(const CRootWriter&) = delete;
    CRootWriter(CRootWriter&&) = delete;
    CRootWriter& operator=(const CRootWriter&) = delete;
    CRootWriter& operator=(CRootWriter&&) = delete;
    
    ~CRootWriter() override;

    void operator()(std::unique_ptr<cola::EventData>&&) final;
};
#endif //CROOT_CROOTWRITER_HH
