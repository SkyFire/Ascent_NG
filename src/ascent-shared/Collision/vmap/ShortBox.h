/*
* Ascent MMORPG Server
* Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef _SHORTBOX_H
#define _SHORTBOX_H

#include <Collision/g3dlite/G3D/Vector3.h>
#include <Collision/g3dlite/G3D/AABox.h>

#include "ShortVector.h"

/**
This is a box and a triangle Class using ShortVectors. Each vector has 16 bit an a fixed point 12.4 representation.
*/

namespace VMAP
{

    class ShortBox
    {
        private:
            ShortVector iV1;
            ShortVector iV2;
        public:
            ShortBox() {}
            inline const ShortVector& getLo() const { return(iV1); }
            inline const ShortVector& getHi() const { return(iV2); }
            inline void setLo(const ShortVector& pV){ iV1 = pV; }
            inline void setHi(const ShortVector& pV){ iV2 = pV; }
            inline void setLo(const Vector3& pV){ iV1 = ShortVector(pV); }
            inline void setHi(const Vector3& pV){ iV2 = ShortVector(pV); }

            inline bool operator==(const ShortBox& b) const
            {
                return ((iV1 == b.iV1) && (iV2 == b.iV2));
            }

            inline bool operator!=(const ShortBox& b) const
            {
                return !((iV1 == b.iV1) && (iV2 == b.iV2));
            }
    };

    //=====================================================================

    static const Vector3 dummyZeroPosition = Vector3(0,0,0);

    class TriangleBox
    {
        private:
            ShortVector _vertex[3];
            //ShortBox iBox;
        public:
            inline TriangleBox() { }
            inline TriangleBox(const ShortVector& pV1, const ShortVector& pV2, const ShortVector& pV3)
            {
                _vertex[0] = pV1;
                _vertex[1] = pV2;
                _vertex[2] = pV3;

            }
            inline const ShortVector& vertex (int n) const
            {
                return(_vertex[n]);
            }

            inline const ShortBox getBounds()const
            {
                ShortBox box;

                ShortVector lo = _vertex[0];
                ShortVector hi = lo;

                for (int i = 1; i < 3; ++i)
                {
                    lo = lo.min(_vertex[i]);
                    hi = hi.max(_vertex[i]);
                }
                box.setLo(lo);
                box.setHi(hi);
                return(box);
            }
            inline const Vector3& getBasePosition() { return(dummyZeroPosition); }

            inline const AABox getAABoxBounds() const { ShortBox box = getBounds(); return(AABox(box.getLo().getVector3(), box.getHi().getVector3())); }

            inline bool operator==(const TriangleBox& t) const
            {
                return ((_vertex[0] == t._vertex[0]) && (_vertex[1] == t._vertex[1]) &&(_vertex[2] == t._vertex[2]));
            }

            inline bool operator!=(const TriangleBox& t) const
            {
                return !((_vertex[0] == t._vertex[0]) && (_vertex[1] == t._vertex[1]) &&(_vertex[2] == t._vertex[2]));
            }
    };

}
#endif
