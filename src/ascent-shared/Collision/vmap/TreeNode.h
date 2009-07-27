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

#ifndef _TREENODE_H
#define _TREENODE_H

//#include "ShortVector.h"
//#include "ShortBox.h"

#include <Collision/g3dlite/G3D/Vector3.h>
#include <Collision/g3dlite/G3D/AABox.h>

using namespace G3D;

namespace VMAP
{
    /**
    This Class is mainly taken from G3D/AABSPTree.h and modified to match our data structure.
    It is the node within our static BSP-Trees.
    It does not use pointers but indexes to access the values and other nodes.
    */

    //=====================================================

    class TreeNode
    {
        private:
            /** Location along the specified axis */
            float iSplitLocation;
            // Offest or the clients
            int iChilds[2];
            //Position within the TriangleBox array
            unsigned int iStartPosition;
            Vector3::Axis iSplitAxis;
            AABox iBounds;
            unsigned short iNumberOfValues;
        public:
            TreeNode() {}
            TreeNode(unsigned short pNValues, unsigned int pStartPosition)
            {
                iChilds[0] = -1;
                iChilds[1] = -1;
                iStartPosition = pStartPosition;
                iNumberOfValues = pNValues;
            }

            bool hasChilds() const { return(iChilds > 0 || iChilds > 0); }

            TreeNode const* getChild(TreeNode const* pValueArray, int pNo) const;
            // pChildNo = 0 or 1
            inline void setChildPos(int pChildNo, int pChildPosInTreeNodeArray) { iChilds[pChildNo] = pChildPosInTreeNodeArray; }

            inline Vector3::Axis getSplitAxis() const { return(iSplitAxis); }

            inline void setSplitAxis(Vector3::Axis a) { iSplitAxis = a; }
            inline void setSplitLocation(float l) { iSplitLocation = l; }

            inline void setBounds(const AABox& pBox) { iBounds = pBox; }

            inline void setBounds(const Vector3& lo, const Vector3& hi) { iBounds.set(lo,hi); }

            inline void getBounds(AABox& pBox) const { pBox.set(iBounds.low(),iBounds.high()); }

            inline float getSplitLocation() const { return(iSplitLocation); }

            inline unsigned short getNValues() const { return (iNumberOfValues); }

            inline unsigned int getStartPosition() const { return(iStartPosition); }

            inline bool operator==(const TreeNode& n) const
            {
                return ((iSplitLocation == n.iSplitLocation) &&
                    (iChilds[0] == n.iChilds[0]) && (iChilds[1] == n.iChilds[1]) &&
                    (iStartPosition == n.iStartPosition) &&
                    (iSplitAxis == n.iSplitAxis) &&
                    (iBounds == n.iBounds) &&
                    (iNumberOfValues == n.iNumberOfValues));
            }

            inline bool operator!=(const TreeNode& n) const
            {
                return !((iSplitLocation == n.iSplitLocation) &&
                    (iChilds[0] == n.iChilds[0]) && (iChilds[1] == n.iChilds[1]) &&
                    (iStartPosition == n.iStartPosition) &&
                    (iSplitAxis == n.iSplitAxis) &&
                    (iBounds == n.iBounds) &&
                    (iNumberOfValues == n.iNumberOfValues));
            }

    };
}
#endif
