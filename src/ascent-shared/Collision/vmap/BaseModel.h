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

#ifndef _BASEMODEL_H_
#define _BASEMODEL_H_

#include <Collision/g3dlite/G3D/AABox.h>
#include <Collision/g3dlite/G3D/Vector3.h>

#include "ShortVector.h"
#include "ShortBox.h"
#include "TreeNode.h"

/**
A model is based on triangles. To be able to check intersection we need a BSP-Tree.
This Class holds the array of triangles as well as the management information for the BSP-Tree.
Both are stored in static array and index information is used instead of pointers.
Therefore we can load the whole object as a binary block.

The vectors are relative to a base position.
*/

namespace VMAP
{

    class BaseModel
    {
        protected:
            TriangleBox *iTriangles;
            TreeNode *iTreeNodes;
            unsigned int iNTriangles;
            unsigned int iNNodes;
            Vector3 iBasePosition;
        public:
            BaseModel() { iNTriangles = 0; iNNodes = 0; iTriangles = 0; iTreeNodes = 0;};
            BaseModel(unsigned int pNNodes  , TreeNode* pTreeNode, unsigned int  pNTriangles, TriangleBox* pTriangleBox)
            {
                iNNodes = pNNodes; iNTriangles = pNTriangles; iTriangles = pTriangleBox; iTreeNodes = pTreeNode;
            };
            BaseModel(unsigned int pNNodes, unsigned int  pNTriangles);

            // destructor does nothing ! The subclass controles the array memory and knows when to free it
            ~BaseModel() {}

            void free();
            void init(unsigned int pNNodes, unsigned int  pNTriangles);

            void getMember(Array<TriangleBox>& pMembers);

            inline const TriangleBox& getTriangle(int pPos) const { return(iTriangles[pPos]); }
            inline       TriangleBox& getTriangle(int pPos)       { return(iTriangles[pPos]); }

            inline void setTriangle(const TriangleBox& pTriangleBox, int pPos) { iTriangles[pPos] = pTriangleBox; }

            inline const TreeNode& getTreeNode(int pPos) const { return(getTreeNodes()[pPos]); }
            inline       TreeNode& getTreeNode(int pPos)       { return(getTreeNodes()[pPos]); }

            inline const void setTreeNode(const TreeNode& pTreeNode, int pPos) { getTreeNodes()[pPos] = pTreeNode; }

            inline const void setBasePosition(const Vector3& pBasePosition) { iBasePosition = pBasePosition; }

            inline const Vector3& getBasePosition() const { return(iBasePosition); }

            inline unsigned int getNNodes() const { return(iNNodes); }
            inline unsigned int getNTriangles() const { return(iNTriangles); }

            inline void setNNodes(unsigned int pNNodes)  { iNNodes = pNNodes; }
            inline void setNTriangles(unsigned int  pNTriangles)  { iNTriangles = pNTriangles; }

            inline void setTriangleArray(TriangleBox *pGlobalTriangleArray ) { iTriangles = pGlobalTriangleArray ; }
            inline void setTreeNodeArray(TreeNode *pGlobalTreeNodeArray ) { iTreeNodes = pGlobalTreeNodeArray ; }

            inline TriangleBox* getTriangles() const { return(iTriangles); }

            inline TreeNode* getTreeNodes() const{ return(iTreeNodes); }

            inline size_t getMemUsage() { return(iNTriangles * sizeof(TriangleBox) + iNNodes * sizeof(TreeNode) + sizeof(BaseModel)); }
    };

}
#endif                                                      /*BASEMODEL_H_*/
