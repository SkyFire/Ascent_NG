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

#ifndef G3D_LINE_H
#define G3D_LINE_H

#include "G3D/platform.h"
#include "G3D/Vector3.h"

namespace G3D {

class Plane;

/**
 An infinite 3D line.
 */
class Line {
protected:

    Vector3 _point;
    Vector3 _direction;

    Line(const Vector3& point, const Vector3& direction) {
        _point     = point;
        _direction = direction.direction();
    }

public:

    /** Undefined (provided for creating Array<Line> only) */
    inline Line() {}

    virtual ~Line() {}

    /**
      Constructs a line from two (not equal) points.
     */
    static Line fromTwoPoints(const Vector3 &point1, const Vector3 &point2) {
        return Line(point1, point2 - point1);
    }

    /**
      Creates a line from a point and a (nonzero) direction.
     */
    static Line fromPointAndDirection(const Vector3& point, const Vector3& direction) {
        return Line(point, direction);
    }

    /**
      Returns the closest point on the line to point.
     */
    Vector3 closestPoint(const Vector3& pt) const;

    /**
      Returns the distance between point and the line
     */
    double distance(const Vector3& point) const {
        return (closestPoint(point) - point).magnitude();
    }

    /** Returns a point on the line */
    Vector3 point() const;

    /** Returns the direction (or negative direction) of the line */
    Vector3 direction() const;

    /**
     Returns the point where the line and plane intersect.  If there
     is no intersection, returns a point at infinity.
     */
    Vector3 intersection(const Plane &plane) const;
};

};// namespace


#endif
