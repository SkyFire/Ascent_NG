/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2010 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the AscentNG Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the AscentNG is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#ifndef G3D_REGISTRYUTIL_H
#define G3D_REGISTRYUTIL_H

#include "Collision/g3dlite/G3D/platform.h"
#include "Collision/g3dlite/G3D/g3dmath.h"

// This file is only used on Windows
#ifdef G3D_WIN32

#include <string>

namespace G3D {

/** 
    Provides generalized Windows registry querying.

    All key names are one string in the format:
        "[base key]\[sub-keys]\value"

    [base key] can be any of the following:
        HKEY_CLASSES_ROOT
        HKEY_CURRENT_CONFIG
        HKEY_CURRENT_USER
        HKEY_LOCAL_MACHINE
        HKEY_PERFORMANCE_DATA
        HKEY_PERFORMANCE_NLSTEXT
        HKEY_PERFORMANCE_TEXT
        HKEY_USERS

    keyExists() should be used to validate a key before reading or writing
    to ensure that a debug assert or false return is for a different error.
*/
class RegistryUtil {

public:
    /** returns true if the key exists */
    static bool keyExists(const std::string& key);

    /** returns false if the key could not be read for any reason. */
    static bool readInt32(const std::string& key, int32& valueData);

    /** 
      Reads an arbitrary amount of data from a binary registry key.
      returns false if the key could not be read for any reason.
    
      @beta
      @param valueData pointer to the output buffer of sufficient size. Pass NULL as valueData in order to have available data size returned in dataSize.
      @param dataSize size of the output buffer.  When NULL is passed for valueData, contains the size of available data on successful return.
    */
    static bool readBytes(const std::string& key, uint8* valueData, uint32& dataSize);

    /** returns false if the key could not be read for any reason. */
    static bool readString(const std::string& key, std::string& valueData);

    /** returns false if the key could not be written for any reason. */
    static bool writeInt32(const std::string& key, int32 valueData);

    /** 
      Writes an arbitrary amount of data to a binary registry key.
      returns false if the key could not be written for any reason.
    
      @param valueData pointer to the input buffer
      @param dataSize size of the input buffer that should be written
    */
    static bool writeBytes(const std::string& key, const uint8* valueData, uint32 dataSize);

    /** returns false if the key could not be written for any reason. */
    static bool writeString(const std::string& key, const std::string& valueData);

};

} // namespace G3D

#endif // G3D_WIN32

#endif // G3D_REGISTRYTUIL_H