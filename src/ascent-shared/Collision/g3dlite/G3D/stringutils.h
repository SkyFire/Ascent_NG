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

#ifndef G3D_STRINGUTILS_H
#define G3D_STRINGUTILS_H

#include "Collision/g3dlite/G3D/platform.h"
#include "Collision/g3dlite/G3D/Array.h"
#include <string>

namespace G3D {

extern const char* NEWLINE;

/**
 Returns true if the test string begins with the pattern string.
 */
bool beginsWith(
    const std::string&          test,
    const std::string&          pattern);

/**
 Returns true if the test string ends with the pattern string.
 */
bool endsWith(
    const std::string&          test,
    const std::string&          pattern);

/**
 Produces a new string that is the input string
 wrapped at a certain number of columns (where
 the line is broken at the latest space before the
 column limit.)  Platform specific NEWLINEs
 are inserted to wrap.
 */
std::string wordWrap(
    const std::string&          input,
    int                         numCols);

/**
 A comparison function for passing to Array::sort.
 */
int stringCompare(
    const std::string&          s1,
    const std::string&          s2);

int stringPtrCompare(
    const std::string*          s1,
    const std::string*          s2);

/**
 Returns a new string that is an uppercase version of x.
 */
std::string toUpper(
    const std::string&          x);

std::string toLower(
    const std::string&          x);

/**
 Splits x at each occurance of splitChar.
 */
G3D::Array<std::string> stringSplit(
    const std::string&          x,
    char                        splitChar);

/**
 joinChar is not inserted at the beginning or end, just in between
 elements.
 */
std::string stringJoin(
    const G3D::Array<std::string>&   a,
    char                        joinChar);

std::string stringJoin(
    const G3D::Array<std::string>&   a,
    const std::string&               joinStr);

/**
 Strips whitespace from both ends of the string.
 */
std::string trimWhitespace(
    const std::string&              s);

/** These standard C functions are renamed for clarity/naming
   conventions and to return bool, not int.
   */
inline bool isWhiteSpace(const char c) {
    return isspace(c) != 0;
}

/** These standard C functions are renamed for clarity/naming
   conventions and to return bool, not int.
   */
inline bool isNewline(const char c) {
    return (c == '\n') || (c == '\r');
}

/** These standard C functions are renamed for clarity/naming
   conventions and to return bool, not int.
   */
inline bool isDigit(const char c) {
    return isdigit(c) != 0;
}

/** These standard C functions are renamed for clarity/naming
   conventions and to return bool, not int.
   */
inline bool isLetter(const char c) {
    return isalpha(c) != 0;
}

inline bool isSlash(const char c) {
    return (c == '\\') || (c == '/');
}

inline bool isQuote(const char c) {
    return (c == '\'') || (c == '\"');
}

}; // namespace

#endif

