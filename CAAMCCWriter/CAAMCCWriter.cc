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

#include "CAAMCCWriter.hh"

CAAMCCWriter::CAAMCCWriter(const std::string& fName, const size_t buffSize, const bool writeCoord) : CRootWriter(fName, buffSize), writeCoord(writeCoord) {
    //data trees
    outputTreeMap.emplace("Glauber", new TTree("Glauber","Events from glauber modeling"));
    TTree* tGlauber = outputTreeMap.at("Glauber");

    tGlauber->SetDirectory(0);

    if (writeCoord) {
        tGlauber->Branch("pseudorapidity_on_A", "std::vector", &event.pseudorapidity_A);
        tGlauber->Branch("pseudorapidity_on_B", "std::vector", &event.pseudorapidity_B);
        tGlauber->Branch("pX_on_A", "std::vector" ,&event.pXonSideA,128000,1);
        tGlauber->Branch("pY_on_A", "std::vector" ,&event.pYonSideA,128000,1);
        tGlauber->Branch("pZ_on_A", "std::vector" ,&event.pZonSideA,128000,1);
        tGlauber->Branch("pX_on_B", "std::vector" ,&event.pXonSideB,128000,1);
        tGlauber->Branch("pY_on_B", "std::vector" ,&event.pYonSideB,128000,1);
        tGlauber->Branch("pZ_on_B", "std::vector" ,&event.pZonSideB,128000,1);
    }

    tGlauber->Branch("id", &event.id, "id/i");
    tGlauber->Branch("A_on_A", "std::vector" ,&event.MassOnSideA);
    tGlauber->Branch("A_on_B", "std::vector" ,&event.MassOnSideB);
    tGlauber->Branch("Z_on_A", "std::vector" ,&event.ChargeOnSideA);
    tGlauber->Branch("Z_on_B", "std::vector" ,&event.ChargeOnSideB);
    tGlauber->Branch("Ncoll", &event.Ncoll, "Ncoll/I");
    tGlauber->Branch("Ncollpp", &event.Ncollpp, "Ncollpp/I");
    tGlauber->Branch("Ncollpn", &event.Ncollpn, "Ncollpn/I");
    tGlauber->Branch("Ncollnn", &event.Ncollnn, "Ncollnn/I");
    tGlauber->Branch("Npart", &event.Npart, "Npart/I");
    tGlauber->Branch("NpartA", &event.NpartA, "NpartA/I");
    tGlauber->Branch("NpartB", &event.NpartB, "NpartB/I");

    tGlauber->Branch("impact_parameter", &event.b, "impact_parameter/f");

    tGlauber->Branch("PhiRotA", &event.PhiRotA, "PhiRotA/f");
    tGlauber->Branch("ThetaRotA", &event.ThetaRotA, "ThetaRotA/f");
    tGlauber->Branch("PhiRotB", &event.PhiRotB, "PhiRotB/f");
    tGlauber->Branch("ThetaRotB", &event.ThetaRotB, "ThetaRotB/f");

    tRun = std::make_unique<TTree>("Conditions","preconditions for modeling"); // tRun is purposefuly not in map, so auto buffering isn't applied

    tRun->SetDirectory(0);

    tRun->Branch("Xsect_NN", &runData.XsectNN,"Xsect_total/d");
    tRun->Branch("Kinetic_energy_per_nucleon_of_projectile_in_MeV", &runData.KinEnPerNucl,"Kinetic_energy_of_per_nucleon_projectile_in_MeV/d");
    tRun->Branch("SqrtS_nn_in_MeV", &runData.SqrtSnn,"SqrtS_nn_in_MeV/d");
    tRun->Branch("pZ_in_MeV_on_A", &runData.pzA,"pZ_in_MeV_on_A/d");
    tRun->Branch("pZ_in_MeV_on_B", &runData.pzB,"pZ_in_MeV_on_B/d");
    tRun->Branch("Mass_on_A", &runData.AinitA,"Mass_on_A/I");
    tRun->Branch("Mass_on_B", &runData.AinitB,"Mass_on_B/I");
    tRun->Branch("Charge_on_A", &runData.ZinitA,"Charge_on_A/I");
    tRun->Branch("Charge_on_B", &runData.ZinitB,"Charge_on_B/I");
}

void CAAMCCWriter::write_event(std::unique_ptr<cola::EventData>&& data) {
    if (callflag) {
        
        auto AZA = cola::pdgToAZ(data->iniState.pdgCodeA);
        auto AZB = cola::pdgToAZ(data->iniState.pdgCodeB);

        runData.AinitA = AZA.first;
        runData.ZinitA = AZA.second;
        runData.AinitB = AZA.first;
        runData.ZinitB = AZA.second;
        
        runData.isCollider = data->iniState.pZB != 0;
        runData.pzA = data->iniState.pZA;
        runData.pzB = data->iniState.pZB;

        if (runData.isCollider) {
            runData.SqrtSnn = data->iniState.energy;
            runData.KinEnPerNucl = runData.SqrtSnn/2.0 - caamcc::nucleonAverMass;
        } else {
            runData.KinEnPerNucl = data->iniState.energy;
            runData.SqrtSnn = pow(2*caamcc::nucleonAverMass*caamcc::nucleonAverMass +
                2*runData.KinEnPerNucl*caamcc::nucleonAverMass, 0.5);
        }

        runData.XsectNN = data->iniState.sectNN;

        tRun->Fill();
        tRun->Write();
        callflag = false;
    }

    event.id = count;
    event.b = data->iniState.b;
    event.Ncoll = data->iniState.nColl;
    event.Ncollnn = data->iniState.nCollNN;
    event.Ncollpn = data->iniState.nCollPN;
    event.Ncollpp = data->iniState.nCollPP;
    event.Npart = data->iniState.nPart;
    event.NpartA = data->iniState.nPartA;
    event.NpartB = data->iniState.nPartB;

    event.PhiRotA = data->iniState.phiRotA;
    event.ThetaRotA = data->iniState.thetaRotA;
    event.PhiRotB = data->iniState.phiRotB;
    event.ThetaRotB = data->iniState.thetaRotB;

    for (const auto& particle: data->particles) {
        switch (particle.pClass)
        {
        case cola::ParticleClass::spectatorA:
            event.MassOnSideA.push_back(static_cast<float>(particle.getAZ().first));
            event.ChargeOnSideA.push_back(static_cast<float>(particle.getAZ().second));
            if (writeCoord) {
                event.pXonSideA.push_back(particle.momentum.x);
                event.pYonSideA.push_back(particle.momentum.y);
                event.pZonSideA.push_back(particle.momentum.z);

                event.pseudorapidity_A.push_back(std::atanh(particle.momentum.z / particle.momentum.mag()));
            }
            break;

        
        case cola::ParticleClass::spectatorB:
            event.MassOnSideB.push_back(static_cast<float>(particle.getAZ().first));
            event.ChargeOnSideB.push_back(static_cast<float>(particle.getAZ().second));
            if (writeCoord) {
                event.pXonSideB.push_back(particle.momentum.x);
                event.pYonSideB.push_back(particle.momentum.y);
                event.pZonSideB.push_back(particle.momentum.z);

                event.pseudorapidity_B.push_back(std::atanh(particle.momentum.z / particle.momentum.mag()));
            }
            break;
        }
    }

    for (const auto& tree: outputTreeMap)
        tree.second->Fill();

    // clear after fillng
    event.MassOnSideA.clear();
    event.MassOnSideB.clear();
    event.ChargeOnSideA.clear();
    event.ChargeOnSideB.clear();
    if (writeCoord) {
        event.pXonSideA.clear();
        event.pYonSideA.clear();
        event.pZonSideA.clear();
        event.pseudorapidity_A.clear();
        event.pXonSideB.clear();
        event.pYonSideB.clear();
        event.pZonSideB.clear();
        event.pseudorapidity_B.clear();
    }

} 