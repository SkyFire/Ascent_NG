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
        iTriangles = 0;
        iTreeNodes = 0;
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
