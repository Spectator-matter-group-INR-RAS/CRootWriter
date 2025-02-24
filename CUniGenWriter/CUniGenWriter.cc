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

#include "CUniGenWriter.hh"

CUniGenWriter::CUniGenWriter(const std::string &fName, const size_t buffSize, const char* treeName, bool writeCoord) : CRootWriter(fName, buffSize, treeName), curEvent(std::make_unique<UEvent>()),
 run(std::make_unique<URun>()), _writeCoord(writeCoord), _runFilled(false) {
    outputTree->Branch("events", curEvent.get());
    //disable unfilled subbrranches
    outputTree->SetBranchStatus("events.fPhi", false);
    outputTree->SetBranchStatus("events.fNes", false);
    outputTree->SetBranchStatus("events.fStepNr", false);
    outputTree->SetBranchStatus("events.fStepT", false);
    outputTree->SetBranchStatus("events.fComment", false);
    outputTree->SetBranchStatus("events.fParticles.fParent", false);
    outputTree->SetBranchStatus("events.fParticles.fParentDecay", false);
    outputTree->SetBranchStatus("events.fParticles.fMate", false);
    outputTree->SetBranchStatus("events.fParticles.fDecay", false);
    outputTree->SetBranchStatus("events.fParticles.fChild[2]", false);
    if (not writeCoord) {
        outputTree->SetBranchStatus("events.fParticles.fPx", false);
        outputTree->SetBranchStatus("events.fParticles.fPy", false);
        outputTree->SetBranchStatus("events.fParticles.fPz", false);
        outputTree->SetBranchStatus("events.fParticles.fE", false);
        outputTree->SetBranchStatus("events.fParticles.fX", false);
        outputTree->SetBranchStatus("events.fParticles.fY", false);
        outputTree->SetBranchStatus("events.fParticles.fZ", false);
        outputTree->SetBranchStatus("events.fParticles.fT", false);
    }
}

void CUniGenWriter::_write_event(std::unique_ptr<cola::EventData> && data) {
   
    const auto& iniState = data->iniState;
    const auto& particles = data->particles;
    int childPlug[2]{-1, -1}; // currently no decay info in COLA, plug is needed

    // fill run data (only once)
    if (not _runFilled) {
        auto nuclA = cola::pdgToAZ(iniState.pdgCodeA);
        auto nuclB = cola::pdgToAZ(iniState.pdgCodeB);

        run = std::make_unique<URun>("", "COLA output, -1 fields mean no info in DO",
                   nuclA.first, nuclA.second, iniState.pZA,
                   nuclB.first, nuclB.second, iniState.pZB,
                   -1, -1, -1, -1, -1, -1, -1);

        run->Write("run");
        _runFilled = true;
    }

    curEvent->SetParameters(static_cast<Int_t>(count), iniState.b, -1, -1, -1, -1);

    // Add particles
    int i = 0;
    for (const auto particle: particles)
        if (_writeCoord)
            curEvent->AddParticle(i++, particle.pdgCode, static_cast<int>(particle.pClass), -1, -1, -1, -1, childPlug,
                              particle.momentum.x, particle.momentum.y, particle.momentum.z, particle.momentum.e,
                              particle.position.x, particle.position.y, particle.position.z, particle.position.t,
                              -1);
        else
            curEvent->AddParticle(i++, particle.pdgCode, static_cast<int>(particle.pClass), -1, -1, -1, -1, childPlug, -1, -1, -1 ,-1, -1, -1, -1, -1, -1);

    outputTree->Fill();
}