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

#include "CNativeRootWriter.hh"

CNativeRootWriter::CNativeRootWriter(const std::string &fName, const size_t buffSize, bool writeCoord) : CRootWriter(fName, buffSize),
    eventData(std::make_unique<cola::EventData>()) {
        outputTreeMap.emplace("ColaNative", std::make_unique<TTree>("ColaNative", "ColaNative"));
        outputTree = outputTreeMap.at("ColaNative").get();
        outputTree->Branch("events", eventData.get());
        //disable unfilled subbrranches
        if (not writeCoord) {
            outputTree->SetBranchStatus("events.EventParticles.position", false);
            outputTree->SetBranchStatus("events.EventParticles.momentum", false);
        }
    }

void CNativeRootWriter::_write_event(std::unique_ptr<cola::EventData>&& data) {
    eventData = std::move(data);
    outputTree->SetBranchAddress("events", eventData.get());
    outputTree->Fill();
}