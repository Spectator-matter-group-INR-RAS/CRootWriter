//
// Created by _amp_ on 9/30/24.
//

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

#endif

#endif //CUNIGEN_LINKDEF_H
