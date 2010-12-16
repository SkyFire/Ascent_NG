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

#ifndef G3D_TRIANGLE_H
#define G3D_TRIANGLE_H

#include "G3D/platform.h"
#include "G3D/g3dmath.h"
#include "G3D/Vector3.h"
#include "G3D/Plane.h"
#include <string>

namespace G3D {

/**
  A generic triangle representation.  This should not be used
  as the underlying triangle for creating models; it is intended
  for providing fast property queries but requires a lot of
  storage and is mostly immutable.
  */
class Triangle {
private:
    friend class CollisionDetection;
    friend class Ray;

    Vector3                     _vertex[3];

    /** edgeDirection[i] is the normalized vector v[i+1] - v[i] */
    Vector3                     edgeDirection[3];
    double                      edgeMagnitude[3];
    Plane                       _plane;
    Vector3::Axis               _primaryAxis;

    /** vertex[1] - vertex[0] */
    Vector3                     edge01;
    /** vertex[2] - vertex[0] */
    Vector3                     edge02;

    float                       _area;

    void init(const Vector3& v0, const Vector3& v1, const Vector3& v2);

public:
	
    Triangle();
    
    Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2);
    
    ~Triangle();

    /** 0, 1, or 2 */
    inline const Vector3& vertex(int n) const {
        debugAssert((n >= 0) && (n < 3));
        return _vertex[n];
    }

    double area() const;

    Vector3::Axis primaryAxis() const {
        return _primaryAxis;
    }

    const Vector3& normal() const;

    /** Barycenter */
    Vector3 center() const;

    const Plane& plane() const;

    /** Returns a random point in the triangle. */
    Vector3 randomPoint() const;

    /**
     For two triangles to be equal they must have
     the same vertices <I>in the same order</I>.
     That is, vertex[0] == vertex[0], etc.
     */
    inline bool operator==(const Triangle& other) {
        for (int i = 0; i < 3; ++i) {
            if (_vertex[i] != other._vertex[i]) {
                return false;
            }
        }

        return true;
    }

    inline unsigned int hashCode() const {
        return
            _vertex[0].hashCode() +
            (_vertex[1].hashCode() >> 2) +
            _vertex[2].hashCode();
    }

    void getBounds(class AABox&) const;

};

} // namespace

inline unsigned int hashCode(const G3D::Triangle& t) {
	return t.hashCode();
}

#endif
