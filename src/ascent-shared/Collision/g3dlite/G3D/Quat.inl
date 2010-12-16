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

namespace G3D {

inline float& Quat::operator[] (int i) {
    debugAssert(i >= 0);
    debugAssert(i < 4);
    return ((float*)this)[i];
}

inline const float& Quat::operator[] (int i) const {
    debugAssert(i >= 0);
    debugAssert(i < 4);
    return ((float*)this)[i];
}



inline Quat Quat::operator-(const Quat& other) const {
    return Quat(x - other.x, y - other.y, z - other.z, w - other.w);
}

inline Quat Quat::operator+(const Quat& other) const {
    return Quat(x + other.x, y + other.y, z + other.z, w + other.w);
}

}

