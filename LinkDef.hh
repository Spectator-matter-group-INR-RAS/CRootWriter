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

#ifndef CUNIGEN_LINKDEF_H
#define CUNIGEN_LINKDEF_H
// some C++ header definition
#if defined(__ROOTCLING__) || defined(__MAKECINT__)
// turns off dictionary generation for all
#pragma link off all class;
#pragma link off all function;
#pragma link off all global;
#pragma link off all typedef;

#pragma link C++ class URun+;
#pragma link C++ class UEvent+;
#pragma link C++ class UParticle+;

#pragma link C++ class cola::EventData+;
#pragma link C++ class cola::EventIniState+;
#pragma link C++ class cola::Particle+;
#pragma link C++ class cola::LorentzVector+;

#endif

#endif //CUNIGEN_LINKDEF_H
