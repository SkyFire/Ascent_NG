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

#include "BaseModel.h"

namespace VMAP
{
    //==========================================================

    void BaseModel::getMember(Array<TriangleBox>& pMembers)
    {
        for(unsigned int i=0; i<iNTriangles; i++)
        {
            pMembers.append(iTriangles[i]);
        }
    }

    //==========================================================
    BaseModel::BaseModel(unsigned int pNNodes, unsigned int  pNTriangles)
    {
        init(pNNodes, pNTriangles);
    };

    //==========================================================

    void BaseModel::init(unsigned int pNNodes, unsigned int  pNTriangles)
    {
        iNNodes = pNNodes;
        iNTriangles = pNTriangles;
        iTriangles = NULL;
        iTreeNodes = NULL;
        if(iNNodes >0) iTreeNodes = new TreeNode[iNNodes];
        if(iNTriangles >0) iTriangles = new TriangleBox[iNTriangles];
    }

    //==========================================================

    void BaseModel::free()
    {
        if(getTriangles() != 0) delete [] getTriangles(); setNTriangles(0);
        if(getTreeNodes() != 0) delete [] getTreeNodes(); setNNodes(0);
    }

}                                                           // VMAP
