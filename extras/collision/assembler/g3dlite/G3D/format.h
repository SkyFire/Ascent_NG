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

#ifndef G3D_FORMAT_H
#define G3D_FORMAT_H

#include "G3D/platform.h"
#include <string>
#include <stdio.h>
#include <cstdarg>
#include <assert.h>
#ifndef G3D_WIN32
    // Don't include varargs.h for some random
    // gcc reason
    //#include <varargs.h>
    #include <stdarg.h>
#endif

#ifndef _MSC_VER
    #ifndef __cdecl
        #define __cdecl __attribute__((cdecl))
    #endif
#endif

namespace G3D {

/**
  Produces a string from arguments of the style of printf.  This avoids
  problems with buffer overflows when using sprintf and makes it easy
  to use the result functionally.  This function is fast when the resulting
  string is under 160 characters (not including terminator) and slower
  when the string is longer.
 */
std::string format(
    const char*                 fmt
    ...) G3D_CHECK_PRINTF_ARGS;

/**
  Like format, but can be called with the argument list from a ... function.
 */
std::string vformat(
    const char*                 fmt,
    va_list                     argPtr) G3D_CHECK_VPRINTF_ARGS;


}; // namespace

#endif
