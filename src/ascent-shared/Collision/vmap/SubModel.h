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

#ifndef _SUBMODEL_H
#define _SUBMODEL_H

// load our modified version first !!
#include "AABSPTree.h"

#include "ShortVector.h"
#include "ShortBox.h"
#include "TreeNode.h"
#include "RayIntersectionIterator.h"
#include "BaseModel.h"

namespace VMAP
{
    /**
    This is a balanced static BSP-Tree of triangles.
    The memory for the tree nodes and the triangles are managed by the ModelContainer.
    The exception to this is during the conversion of raw data info balanced BSP-Trees.
    During this conversion the memory management is done internally.
    */
    class SubModel : public BaseModel
    {
        private:
            unsigned int iNodesPos;
            unsigned int iTrianglesPos;
            bool iHasInternalMemAlloc;
			unsigned int iIsIndoor;
            ShortBox iBox;
        #ifdef _DEBUG_VIEW
            Array<TriangleBox *> iDrawBox;
        #endif
        public:
            SubModel() : BaseModel(){ };

            SubModel(unsigned int pNTriangles, TriangleBox *pTriangles, unsigned int pTrianglesPos, unsigned int pNNodes, TreeNode *pTreeNodes, unsigned int pNodesPos);
            SubModel(AABSPTree<Triangle> *pTree);
            ~SubModel(void);
            //Gets a 50 byte binary block
            void initFromBinBlock(void *pBinBlock);

            RayIntersectionIterator<TreeNode, TriangleBox> beginRayIntersection(const Ray& ray, double pMaxTime, bool skipAABoxTests = false) const;

            RayIntersectionIterator<TreeNode, TriangleBox> endRayIntersection() const;

            void fillRenderArray(Array<TriangleBox> &pArray, const TreeNode* pTreeNode);

            RealTime getIntersectionTime(const Ray& pRay, bool pExitAtFirst, float pMaxDist) const;

            void countNodesAndTriangles(AABSPTree<Triangle>::Node& pNode, int &pNNodes, int &pNTriabgles);

            void fillContainer(const AABSPTree<Triangle>::Node& pNode, int &pTreeNodePos, int &pTrianglePos, Vector3& pLo, Vector3& pHi);

            inline const ShortBox& getReletiveBounds() const { return(iBox); }

            inline void setReletiveBounds(const ShortVector& lo, const ShortVector& hi) { iBox.setLo(lo); iBox.setHi(hi); }

            inline const AABox getAABoxBounds() const { return(AABox(iBox.getLo().getVector3() + getBasePosition(), iBox.getHi().getVector3()+ getBasePosition())); }

            // get start pos bases on the global array
            inline TriangleBox const* getTriangles() const { return &BaseModel::getTriangle(iTrianglesPos); }
            inline TriangleBox      * getTriangles()       { return &BaseModel::getTriangle(iTrianglesPos); }

            // get start pos bases on the global array
            inline TreeNode const* getTreeNodes() const { return &BaseModel::getTreeNode(iNodesPos); }
            inline TreeNode      * getTreeNodes()       { return &BaseModel::getTreeNode(iNodesPos); }

            // internal method usign internal offset
            inline const TreeNode& getTreeNode(int pPos) const { return(SubModel::getTreeNodes()[pPos]); }

            // internal method usign internal offset
            inline const TriangleBox& getTriangle(int pPos) const { return(SubModel::getTriangles()[pPos]); }

            inline unsigned int getNodesPos() const { return(iNodesPos); }
            inline unsigned int getTrianglesPos() const { return(iTrianglesPos); }

			inline void setIndoorFlag(unsigned int val) { iIsIndoor = val; }
			inline const unsigned int getIndoorFlag() const { return iIsIndoor; }
    };

    size_t hashCode(const SubModel& pSm);
    bool operator==(const SubModel& pSm1, const SubModel& pSm2);
    void getBounds(const SubModel& pSm, G3D::AABox& pAABox);
    void getBounds(const SubModel* pSm, G3D::AABox& pAABox);
    //====================================
}                                                           // VMAP
#endif
