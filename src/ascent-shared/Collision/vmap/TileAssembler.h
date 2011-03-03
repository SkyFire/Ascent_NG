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

#ifndef _TILEASSEMBLER_H_
#define _TILEASSEMBLER_H_

// load our modified version first !!
#include "AABSPTree.h"

#include <Collision/g3dlite/G3D/Vector3.h>

#include "CoordModelMapping.h"
#include "SubModel.h"
#include "ModelContainer.h"

namespace VMAP
{
    /**
    This Class is used to convert raw vector data into balanced BSP-Trees.
    To start the conversion call convertWorld().
    */
    //===============================================

    class ModelPosition
    {
        private:
            Matrix3 ixMatrix;
            Matrix3 iyMatrix;
            Matrix3 izMatrix;
        public:
            Vector3 iPos;
            Vector3 iDir;
            float iScale;
            void init()
            {

                // Swap x and y the raw data uses the axis differently
                ixMatrix = Matrix3::fromAxisAngle(Vector3::UNIT_Y,-(G3D_PI*iDir.x/180.0));
                iyMatrix = Matrix3::fromAxisAngle(Vector3::UNIT_X,-(G3D_PI*iDir.y/180.0));
                izMatrix = Matrix3::fromAxisAngle(Vector3::UNIT_Z,-(G3D_PI*iDir.z/180.0));

            }
            Vector3 transform(const Vector3& pIn) const;
            void moveToBasePos(const Vector3& pBasePos) { iPos -= pBasePos; }
    };
    //===============================================

    class TileAssembler
    {
        private:
            CoordModelMapping *iCoordModelMapping;
            std::string iDestDir;
            std::string iSrcDir;
            bool (*iFilterMethod)(char *pName);
            Table<std::string, unsigned int > iUniqueNameIds;
            unsigned int iCurrentUniqueNameId;

        public:
            TileAssembler(const std::string& pSrcDirName, const std::string& pDestDirName);
            virtual ~TileAssembler();

            bool fillModelContainerArray(const std::string& pDirFileName, unsigned int pMapId, int pXPos, int pYPos, Array<ModelContainer*>& pMC);
            ModelContainer* processNames(const Array<std::string>& pPosFileNames, const char* pDestFileName);

            void init();
            bool convertWorld();

            bool fillModelIntoTree(AABSPTree<SubModel *> *pMainTree, const Vector3& pBasePos, std::string& pPosFilename, std::string& pModelFilename);
            void getModelPosition(std::string& pPosString, ModelPosition& pModelPosition);
            bool readRawFile(std::string& pModelFilename,  ModelPosition& pModelPosition, AABSPTree<SubModel *> *pMainTree);
            void addWorldAreaMapId(unsigned int pMapId) { iCoordModelMapping->addWorldAreaMap(pMapId); }
            void setModelNameFilterMethod(bool (*pFilterMethod)(char *pName)) { iFilterMethod = pFilterMethod; }
            std::string getDirEntryNameFromModName(unsigned int pMapId, const std::string& pModPosName);
            unsigned int getUniqueNameId(const std::string pName);
    };
    //===============================================
}                                                           // VMAP
#endif                                                      /*_TILEASSEMBLER_H_*/
