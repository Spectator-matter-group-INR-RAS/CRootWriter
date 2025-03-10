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

#define DEFAULT_BUFFER_SIZE 10000

#include "COLA.hh"

#include "CRootWriter.hh"
#include "CRootWriterFactory.hh"

#include "CUniGenWriter.hh"
#include "CNativeRootWriter.hh"

cola::VFilter *CRootWriterFactory::create(const std::map<std::string, std::string> & paramMap) {
    size_t bufferSize = DEFAULT_BUFFER_SIZE;
    bool writeCoord = false;
    std::string format = "COLANative";
    if (paramMap.find("buff_size") != paramMap.end())
        bufferSize = std::stoi(paramMap.at("buff_size"));
    if (paramMap.find("write_coord") != paramMap.end())
        writeCoord = std::stoi(paramMap.at("write_coord"));
    if (paramMap.find("format") != paramMap.end())
        std::string format = paramMap.at("format");
    if (format == "UniGen")
        return new CUniGenWriter(paramMap.at("file_name"), bufferSize, "UniGen", writeCoord);
    else
        return new CNativeRootWriter(paramMap.at("file_name"), bufferSize, "COLANative", writeCoord);
}