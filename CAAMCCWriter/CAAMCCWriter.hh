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

#ifndef CROOT_CAAMCCWRITER_HH
#define CROOT_CAAMCCWRITER_HH

#include "COLA.hh"
#include "CRootWriter.hh"

#include "TTree.h"
#include "TFile.h"

#include <vector>
#include <string>
#include <memory>

namespace caamcc {
    constexpr double nucleonAverMass = 0.93891875434*1e3;
    constexpr double pi = 3.14159265358979323846;
}

struct AAMCCEvent{

    std::vector<float> MassOnSideA;
    std::vector<float> MassOnSideB;
    std::vector<float> ChargeOnSideA;
    std::vector<float> ChargeOnSideB;
    std::vector<double> pXonSideA;
    std::vector<double> pYonSideA;
    std::vector<double> pZonSideA;
    std::vector<double> pXonSideB;
    std::vector<double> pYonSideB;
    std::vector<double> pZonSideB;
    std::vector<double> pseudorapidity_A;
    std::vector<double> pseudorapidity_B;

    float b = 0;
    float ExEnA = 0;
    float ExEnB = 0;
    int id = 0;
    int Nhard = 0;
    int Ncoll = 0;
    int Ncollpp = 0;
    int Ncollpn = 0;
    int Ncollnn = 0;
    int Npart = 0;
    int NpartA = 0;
    int NpartB = 0;

    double FermiMomA_x = 0;
    double FermiMomA_y = 0;
    double FermiMomA_z = 0;
    double FermiMomB_x = 0;
    double FermiMomB_y = 0;
    double FermiMomB_z = 0;

    float PhiRotA = 0;
    float ThetaRotA = 0;
    float PhiRotB = 0;
    float ThetaRotB = 0;
    float Ecc[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int ClustNumA = 0;
    int ClustNumB = 0;
    double d_MstA = 0;
    double d_MstB = 0;
    std::vector<int> A_cl{0};
    std::vector<int> Z_cl{0};
    std::vector<int> Ab_cl{0};
    std::vector<int> Zb_cl{0};

    AAMCCEvent& operator=(const cola::EventData&);
};

struct AAMCCrun{
    int ZinitA = -1;
    int AinitA = -1;
    int ZinitB = -1;
    int AinitB = -1;

    std::string SysA = "";
    std::string SysB = "";
    std::string fileName = "";
    std::string fileRName = "";

    double KinEnPerNucl = -1.0;
    double SqrtSnn = -1.0;
    double pzA = -1.0;
    double pzB = -1.0;
    bool isCollider;

    int iterations = -1;

    double XsectNN = -1.0;
    double XsectTot = -1.0;

    std::string DeExModel = "";
    int ExExStatLabel = -1;

    double lowLimitB = -1.0; // MB if negative
    double upperLimitB = -2.0; // MB if upperLimitB < lowLimitB

    double CritDist = 2.7; //2.7

    bool InFileOrNot = false;

    bool isQMD = false;

    AAMCCrun& operator=(const cola::EventData&);
};

class CAAMCCWriter : CRootWriter {
private:
    std::unique_ptr<TTree> tRun;                // tRun is purposefuly not in map, so auto buffering isn't applied
    AAMCCEvent event;
    AAMCCrun runData;

    bool callflag = true;
    bool write_coord;

    void write_event(std::unique_ptr<cola::EventData>&&) final;

public:
    CAAMCCWriter() = delete;
    CAAMCCWriter(const CAAMCCWriter&) = delete;
    CAAMCCWriter(CAAMCCWriter&&) = delete;
    CAAMCCWriter& operator=(const CAAMCCWriter&) = delete;
    CAAMCCWriter& operator=(CAAMCCWriter&&) = delete;
    
    CAAMCCWriter(const std::string& fName, const size_t buffSize, const bool write_coord);
};


#endif //CROOT_CAAMCCWRITER_HH
