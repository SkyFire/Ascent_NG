/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#ifndef G3D_SPHERE_H
#define G3D_SPHERE_H

#include "Collision/g3dlite/G3D/platform.h"
#include "Collision/g3dlite/G3D/Vector3.h"
#include "Collision/g3dlite/G3D/Array.h"
#include "Collision/g3dlite/G3D/Sphere.h"

namespace G3D {

/**
 Sphere.
 */
class Sphere {
private:

    static int32     dummy;

public:
    Vector3          center;
    float            radius;

    Sphere() {
        center = Vector3::zero();
        radius = 0;
    }

    Sphere(
        const Vector3&  center,
        float           radius) {

        this->center = center;
        this->radius = radius;
    }

    virtual ~Sphere() {}

    bool operator==(const Sphere& other) const {
        return (center == other.center) && (radius == other.radius);
    }

    bool operator!=(const Sphere& other) const {
        return !((center == other.center) && (radius == other.radius));
    }

    /**
     Returns true if point is less than or equal to radius away from
     the center.
     */
    bool contains(const Vector3& point) const;

/**
	 @deprecated Use culledBy(Array<Plane>&)
     */
    bool culledBy(
        const class Plane*  plane,
        int                 numPlanes,
		int32&				cullingPlaneIndex,
		const uint32  		testMask,
        uint32&             childMask) const;

    /**
	 @deprecated Use culledBy(Array<Plane>&)
     */
    bool culledBy(
        const class Plane*  plane,
        int                 numPlanes,
		int32&				cullingPlaneIndex = dummy,
		const uint32  		testMask = -1) const;

	/**
      See AABox::culledBy
	 */
	bool culledBy(
		const Array<Plane>&		plane,
		int32&					cullingPlaneIndex,
		const uint32  			testMask,
        uint32&                 childMask) const;

    /**
     Conservative culling test that does not produce a mask for children.
     */
	bool culledBy(
		const Array<Plane>&		plane,
		int32&					cullingPlaneIndex = dummy,
		const uint32  			testMask		  = -1) const;
    virtual std::string toString() const;

    float volume() const;

    /** @deprecated */
    float surfaceArea() const;

    inline float area() const {
        return surfaceArea();
    }

    /**
     Uniformly distributed on the surface.
     */
    Vector3 randomSurfacePoint() const;

    /**
     Uniformly distributed on the interior (includes surface)
     */
    Vector3 randomInteriorPoint() const;

    void getBounds(class AABox& out) const;
};

} // namespace

inline size_t hashCode(const G3D::Sphere& sphere) {
    return (size_t)(hashCode(sphere.center) + (sphere.radius * 13));
}

#endif
