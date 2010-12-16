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

#ifndef _VMAPMANAGER_H
#define _VMAPMANAGER_H

// load our modified version first !!
#include "AABSPTree.h"
#include "ManagedModelContainer.h"
#include "IVMapManager.h"
#include "DebugCmdLogger.h"

#include <G3D/Table.h>

//===========================================================

#define DIR_FILENAME_EXTENSION ".vmdir"

#define FILENAMEBUFFER_SIZE 500

/**
This is the main Class to manage loading and unloading of maps, line of sight, height calculation and so on.
For each map or map tile to load it reads a directory file that contains the ModelContainer files used by this map or map tile.
Each global map or instance has its own dynamic BSP-Tree.
The loaded ModelContainers are included in one of these BSP-Trees.
Additionally a table to match map ids and map names is used.
*/

// Create a value describing the map tile
#define MAP_TILE_IDENT(x,y) ((x<<8) + y)
//===========================================================

namespace VMAP
{
    //===========================================================

    class FilesInDir
    {
        private:
            int iRefCount;
            Array<std::string> iFiles;
        public:

            FilesInDir() { iRefCount = 0; }
            void append(const std::string& pName) { iFiles.append(pName); }
            void incRefCount() { iRefCount++; }
            void decRefCount() { if(iRefCount > 0) iRefCount--; }
            int getRefCount() { return iRefCount; }
            const Array<std::string>& getFiles() const { return iFiles; }
    };

    //===========================================================
    //===========================================================
    //===========================================================
    //===========================================================

    class MapTree
    {
        private:
            AABSPTree<ModelContainer *> *iTree;

            // Key: filename, value ModelContainer
            Table<std::string, ManagedModelContainer *> iLoadedModelContainer;

            // Key: dir file name, value FilesInDir
            Table<std::string, FilesInDir> iLoadedDirFiles;

            // Store all the map tile idents that are loaded for that map
            // some maps are not splitted into tiles and we have to make sure, not removing the map before all tiles are removed
            Table<unsigned int, bool> iLoadedMapTiles;
            std::string iBasePath;

        private:
            float getIntersectionTime(const Ray& pRay, float pMaxDist, bool pStopAtFirstHit);
            bool isAlreadyLoaded(const std::string& pName) { return(iLoadedModelContainer.containsKey(pName)); }
            RayIntersectionIterator<AABSPTree<ModelContainer*>::Node, ModelContainer*> beginRayIntersection(const Ray& ray, bool skipAABoxTests = false) const;
            RayIntersectionIterator<AABSPTree<ModelContainer*>::Node, ModelContainer*> endRayIntersection() const;
            void setLoadedMapTile(unsigned int pTileIdent) { iLoadedMapTiles.set(pTileIdent, true); }
            void removeLoadedMapTile(unsigned int pTileIdent) { iLoadedMapTiles.remove(pTileIdent); }
            bool hasLoadedMapTiles() { return(iLoadedMapTiles.size() > 0); }
            bool containsLoadedMapTile(unsigned int pTileIdent) { return(iLoadedMapTiles.containsKey(pTileIdent)); }
        public:
            ManagedModelContainer *getModelContainer(const std::string& pName) { return(iLoadedModelContainer.get(pName)); }
            const bool hasDirFile(const std::string& pDirName) const { return(iLoadedDirFiles.containsKey(pDirName)); }
            FilesInDir& getDirFiles(const std::string& pDirName) const { return(iLoadedDirFiles.get(pDirName)); }
        public:
            MapTree(const char *pBasePath);
            ~MapTree();

            bool isInLineOfSight(const Vector3& pos1, const Vector3& pos2);
            bool getObjectHitPos(const Vector3& pos1, const Vector3& pos2, Vector3& pResultHitPos, float pModifyDist);
            float getHeight(const Vector3& pPos);

            bool loadMap(const std::string& pDirFileName, unsigned int pMapTileIdent);
            void addModelConatiner(const std::string& pName, ManagedModelContainer *pMc);
            void unloadMap(const std::string& dirFileName, unsigned int pMapTileIdent, bool pForce=false);

            void getModelContainer(Array<ModelContainer *>& pArray ) { iTree->getMembers(pArray); }
            const void addDirFile(const std::string& pDirName, const FilesInDir& pFilesInDir) { iLoadedDirFiles.set(pDirName, pFilesInDir); }
            int size() { return(iTree->size()); }
    };

    //===========================================================
    class MapIdNames
    {
        public:
            std::string iDirName;
            std::string iMapGroupName;
    };

    //===========================================================
    class VMapManager : public IVMapManager
    {
        private:
            // Tree to check collision
            Table<unsigned int , MapTree *> iInstanceMapTrees;
            Table<unsigned int , bool> iMapsSplitIntoTiles;
            Table<unsigned int , bool> iIgnoreMapIds;

            CommandFileRW iCommandLogger;
        private:
            bool _loadMap(const char* pBasePath, unsigned int pMapId, int x, int y, bool pForceTileLoad=false);
            void _unloadMap(unsigned int  pMapId, int x, int y);
            bool _existsMap(const std::string& pBasePath, unsigned int pMapId, int x, int y, bool pForceTileLoad);

        public:
            // public for debug
            Vector3 convertPositionToInternalRep(float x, float y, float z) const;
            Vector3 convertPositionToMangosRep(float x, float y, float z) const;
            std::string getDirFileName(unsigned int pMapId) const;
            std::string getDirFileName(unsigned int pMapId, int x, int y) const;
            MapTree* getInstanceMapTree(int pMapId) { return(iInstanceMapTrees.get(pMapId)); }
        public:
            VMapManager();
            ~VMapManager(void);

            int loadMap(const char* pBasePath, unsigned int pMapId, int x, int y);

            bool existsMap(const char* pBasePath, unsigned int pMapId, int x, int y);

            void unloadMap(unsigned int pMapId, int x, int y);
            void unloadMap(unsigned int pMapId);

            bool isInLineOfSight(unsigned int pMapId, float x1, float y1, float z1, float x2, float y2, float z2) ;
            /**
            fill the hit pos and return true, if an object was hit
            */
            bool getObjectHitPos(unsigned int pMapId, float x1, float y1, float z1, float x2, float y2, float z2, float& rx, float &ry, float& rz, float pModifyDist);
            float getHeight(unsigned int pMapId, float x, float y, float z);

            bool processCommand(char *pCommand);            // for debug and extensions

            void preventMapsFromBeingUsed(const char* pMapIdString);
    };
}
#endif
