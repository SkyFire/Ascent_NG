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

#ifndef G3D_BOX_H
#define G3D_BOX_H

#include "G3D/platform.h"
#include "G3D/Vector3.h"
#include "G3D/Array.h"
#include "G3D/Plane.h"

namespace G3D {

class CoordinateFrame;

/**
 An arbitrary 3D box, useful as a bounding box. 


  To construct a box from a coordinate frame, center and extent, use the idiom:

 <CODE>Box box = cframe.toObjectSpace(Box(center - extent/2, center + extent/2));</CODE>
 */
class Box {
private:

    static int32 dummy;

    friend class CoordinateFrame;

    /**
      <PRE>
       3    2       7    6
    
       0    1       4    5

       front    back (seen through front)
      </PRE>
     */
    Vector3 _corner[8];

    /**
     Unit axes.
     */
    Vector3 _axis[3];
   
    Vector3 _center;

    /**
     Extent along each axis.
     */
    Vector3 _extent;

    float  _area;
    float  _volume;

    void init(
        const Vector3& min,
        const Vector3& max);

public:

    /**
     Does not initialize the fields.
     */
    Box();

    /**
      Constructs a box from two opposite corners.
     */
    Box(
        const Vector3&      min,
        const Vector3&      max);

    Box(const class AABox& b);


    /**
     Returns the object to world transformation for 
     this box.  localFrame().worldToObject(...) takes
     objects into the space where the box axes are
     (1,0,0), (0,1,0), (0,0,1).  Note that there
     is no scaling in this transformation.
     */
    CoordinateFrame localFrame() const;

    void getLocalFrame(CoordinateFrame& frame) const;

    /**
      Returns the centroid of the box.
     */
    inline Vector3 center() const {
        return _center;
    }

    inline Vector3 getCenter() const {
        return center();
    }

    /**
     Returns a corner (0 <= i < 8)
     @deprecated
     */
    inline Vector3 getCorner(int i) const {
        debugAssert(i < 8);
        return _corner[i];
    }

    inline Vector3 corner(int i) const {
        debugAssert(i < 8);
        return _corner[i];
    }

    /**
     Unit length.
     */
    inline Vector3 axis(int a) const {
        debugAssert(a < 3);
        return _axis[a];
    }

    /**
     Distance from corner(0) to the next corner
     along the box's local axis a.
     */
    inline float extent(int a) const {
        debugAssert(a < 3);
        return (float)_extent[a];
    }

    inline Vector3 extent() const {
        return _extent;
    }

    /**
     Returns the four corners of a face (0 <= f < 6).
     The corners are returned to form a counter clockwise quad facing outwards.
     */
    void getFaceCorners(
        int                 f,
        Vector3&            v0,
        Vector3&            v1,
        Vector3&            v2,
        Vector3&            v3) const;

/**
	 @deprecated Use culledBy(Array<Plane>&)
     */
    bool culledBy(
        const class Plane*  plane,
        int                 numPlanes,
	int32&              cullingPlaneIndex,
	const uint32        testMask,
        uint32&             childMask) const;

    /**
	 @deprecated Use culledBy(Array<Plane>&)
     */
    bool culledBy(
        const class Plane*  plane,
        int                 numPlanes,
	int32&		cullingPlaneIndex = dummy,
	const uint32  	testMask = -1) const;

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

    bool contains(
        const Vector3&      point) const;

    /** @deprecated */
    float surfaceArea() const;

    inline float area() const {
        return surfaceArea();
    }

    float volume() const;

    void getRandomSurfacePoint(Vector3& P, Vector3& N = Vector3::dummy) const;

    /**
      @deprecated
     Uniformly distributed on the surface.
     */
    inline Vector3 randomSurfacePoint() const {
        Vector3 V;
        getRandomSurfacePoint(V);
        return V;
    }

    /**
     Uniformly distributed on the interior (includes surface)
     */
    Vector3 randomInteriorPoint() const;

    void getBounds(class AABox&) const;
};

}

#endif
