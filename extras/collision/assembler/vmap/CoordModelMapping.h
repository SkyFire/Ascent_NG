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

#ifndef _COORDMODELMAPPING_H_
#define _COORDMODELMAPPING_H_

#include <G3D/Table.h>
#include <G3D/Array.h>

using namespace G3D;

/**
This Class is a helper Class to convert the raw vector data into BSP-Trees.
We read the directory file of the raw data output and build logical groups.
Models with a lot of vectors are not merged into a resulting model, but separated into an additional file.
*/

namespace VMAP
{

    #define MIN_VERTICES_FOR_OWN_CONTAINER_FILE 65000

    // if we are in an instance
    #define MIN_INST_VERTICES_FOR_OWN_CONTAINER_FILE 40000

    //=====================================================
    class NameCollection
    {
        public:
            Array<std::string> iMainFiles;
            Array<std::string> iSingeFiles;

            void appendToMain(std::string pStr) { iMainFiles.append(pStr); }
            void appendToSingle(std::string pStr) { iSingeFiles.append(pStr); }

            size_t size() { return (iMainFiles.size() + iSingeFiles.size()); }
    };

    //=====================================================

    class CMappingEntry
    {
        private:
            int xPos;
            int yPos;
            unsigned int iMapId;
            Array<std::string> iFilenames;

        public:
            CMappingEntry() { };
            CMappingEntry(unsigned int pMapId, const int pXPos, const int pYPos)
            {
                iMapId = pMapId;
                xPos = pXPos; yPos = pYPos;
            };
            ~CMappingEntry() {};

            void addFilename(char *pName);
            const std::string getKeyString() const;
            inline const Array<std::string>& getFilenames() const { return(iFilenames); }

            static const std::string getKeyString(unsigned int pMapId, int pXPos, int pYPos)
            {
                char b[100];
                sprintf(b,"%03u_%d_%d", pMapId, pXPos, pYPos);
                return(std::string(b));
            }

    };

    //=====================================================

    class CoordModelMapping
    {
        private:
            Table<std::string, CMappingEntry *> iMapObjectFiles;
            Table<std::string, std::string> iProcesseSingleFiles;
            Array<unsigned int> iMapIds;
            Array<unsigned int> iWorldAreaGroups;
            bool (*iFilterMethod)(char *pName);

            inline void addCMappingEntry(CMappingEntry* pCMappingEntry)
            {
                iMapObjectFiles.set(pCMappingEntry->getKeyString(), pCMappingEntry);
            }

            inline CMappingEntry* getCMappingEntry(const std::string& pKey)
            {
                if(iMapObjectFiles.containsKey(pKey))
                    return(iMapObjectFiles.get(pKey));
                else
                    return 0;
            }

        public:
            CoordModelMapping() { iFilterMethod = NULL; }
            virtual ~CoordModelMapping();

            bool readCoordinateMapping(const std::string& pDirectoryFileName);

            const NameCollection getFilenamesForCoordinate(unsigned int pMapId, int xPos, int yPos);

            static unsigned int getMapIdFromFilename(std::string pName)
            {
                size_t spos;

                spos = pName.find_last_of('/');
                std::string basename = pName.substr(0, spos);
                spos = basename.find_last_of('/');
                std::string groupname = basename.substr(spos+1, basename.length());
                unsigned int mapId = atoi(groupname.c_str());
                return(mapId);
            }

            const Array<unsigned int>& getMaps() const { return iMapIds; }
            inline bool isAlreadyProcessedSingleFile(std::string pName) { return(iProcesseSingleFiles.containsKey(pName)); }
            inline void addAlreadyProcessedSingleFile(std::string pName) { iProcesseSingleFiles.set(pName,pName); }

            inline void addWorldAreaMap(unsigned int pMapId)
            {
                if(!iWorldAreaGroups.contains(pMapId))
                {
                    iWorldAreaGroups.append(pMapId);
                }
            }
            inline bool isWorldAreaMap(unsigned int pMapId) { return(iWorldAreaGroups.contains(pMapId)); }
            void setModelNameFilterMethod(bool (*pFilterMethod)(char *pName)) { iFilterMethod = pFilterMethod; }

    };
}
#endif                                                      /*_COORDMODELMAPPING_H_*/
