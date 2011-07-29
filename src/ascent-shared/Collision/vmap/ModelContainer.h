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


#ifndef _MODELCONTAINER_H
#define _MODELCONTAINER_H

// load our modified version first !!
#include "AABSPTree.h"

#include <Collision/g3dlite/G3D/AABox.h>
#include <Collision/g3dlite/G3D/Vector3.h>
#include <Collision/g3dlite/G3D/Ray.h>

#include "ShortBox.h"
#include "TreeNode.h"
#include "RayIntersectionIterator.h"
#include "SubModel.h"
#include "BaseModel.h"

namespace VMAP
{
    /**
    The ModelContainer is a balanced BSP-Tree of SubModels.
    We store a map tile or an instance in one ModelContainer.
    The ModelContainer manages the memory used for the tree nodes, the SubModels and its triangles in static arrays.
    The tree nodes are used for the BSP-Tree of SubModels as well as for the BSP-Tree of triangles within one SubModel.
    The references are done by indexes within these static arrays.
    Therefore we are able to just load a binary block and do not need to mess around with memory allocation and pointers.
    */

    //=====================================================

    class ModelContainer : public BaseModel
    {
        private:
            unsigned int iNSubModel;
            SubModel *iSubModel;
            //Vector3 iDirection;
            AABox iBox;

        public:
            ModelContainer() : BaseModel()
			{
				iNSubModel = 0;
				iSubModel = NULL;
				iTreeNodes = NULL;
				iTriangles = NULL;
				iNTriangles = 0;
				iNNodes = 0;
			}

            // for the mainnode
            ModelContainer(unsigned int pNTriangles, unsigned int pNNodes, unsigned int pNSubModel);

            ModelContainer(AABSPTree<SubModel *> *pTree);

            ~ModelContainer(void);

            RayIntersectionIterator<TreeNode, SubModel> beginRayIntersection(const Ray& ray, double pMaxTime, bool skipAABoxTests = false) const;

            RayIntersectionIterator<TreeNode, SubModel> endRayIntersection() const;

            inline const void setSubModel(const SubModel& pSubModel, int pPos) { iSubModel[pPos] = pSubModel; }

            inline const SubModel& getSubModel(int pPos) const { return iSubModel[pPos]; }

            inline unsigned int getNSubModel() const { return(iNSubModel); }

            RealTime getIntersectionTime(const Ray&, bool pExitAtFirst, float pMaxDist) const;

            void countSubModelsAndNodesAndTriangles(AABSPTree<SubModel *>::Node& pNode, int& nSubModels, int& nNodes, int& nTriangles);

            void fillContainer(const AABSPTree<SubModel *>::Node& pNode, int &pSubModelPos, int &pTreeNodePos, int &pTrianglePos, Vector3& pLo, Vector3& pHi, Vector3& pFinalLo, Vector3& pFinalHi);

            bool readRawFile(const char *name);

            inline const AABox& getAABoxBounds() const { return(iBox); }

            inline void setBounds(const Vector3& lo, const Vector3& hi) { iBox.set(lo,hi); }

            bool writeFile(const char *filename);

            bool readFile(const char *filename);

            size_t getMemUsage();
    };

    //=====================================================

    //=====================================================

    size_t hashCode(const ModelContainer& pMc);
    bool operator==(const ModelContainer& pMc1, const ModelContainer& pMc2);
    void getBounds(const ModelContainer& pMc, G3D::AABox& pAABox);
    void getBounds(const ModelContainer* pMc, G3D::AABox& pAABox);

}
#endif
