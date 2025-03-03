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

#include "CRootWriter.hh"
#include "TFile.h"

CRootWriter::CRootWriter(const std::string &fName, const size_t buffSize, const char* treeName) : outputFile(std::unique_ptr<TFile>(TFile::Open(fName.c_str(), "RECREATE"))),
buffSize(buffSize), outputTree(new TTree(treeName, treeName)), count(0) {}


CRootWriter::~CRootWriter() {
    outputTree->Write();
    outputFile->Save();
    outputFile->Close();
}

void CRootWriter::operator()(std::unique_ptr<cola::EventData>&& data) {
    _write_event(std::move(data));
    count++;
    if (count % buffSize == 0)
        outputTree->Write();
}