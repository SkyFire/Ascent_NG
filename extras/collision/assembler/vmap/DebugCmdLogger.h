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

#ifndef _DEBUGCMDLOGGER_H
#define _DEBUGCMDLOGGER_H

#include <G3D/Vector3.h>
#include <G3D/Array.h>

using namespace G3D;

/**
Class is used for debugging. We log activities into a file.
With an external Class we read that log and display the activity in a graphical view.
*/

namespace VMAP
{

    //==========================================
    enum C_TYPES
    {
        STOP,
        START,
        LOAD_TILE,
        UNLOAD_TILE,
        SET_POS,
        TEST_VIS,
        LOAD_INSTANCE,
        UNLOAD_INSTANCE,
        TEST_HEIGHT,
        TEST_OBJECT_HIT,
    };

    class Command
    {
        int iType;
        float floats[9];
        int ints[4];
        char buffer[100];
        public:

            Command() { iType = STOP; }

            inline int getType() { return iType; }
            inline Vector3 getVector(int pos) { return(Vector3(floats[pos*3+0], floats[pos*3+1], floats[pos*3+2])); }
            inline int getInt(int pos) { return(ints[pos]); }
            inline char* getBuffer() { return(buffer); }

            void fillStopCmd() { iType = STOP; }
            void fillStartCmd() { iType = START; }
            void fillLoadTileCmd(int x, int y, uint32 pMapId) { iType = LOAD_TILE; ints[0] = x; ints[1] = y; ints[2] = pMapId; }
            //void fillLoadTileCmd(int x,int y) { iType = LOAD_TILE; ints[0] = x; ints[1] = y; }
            void fillUnloadTileCmd(uint32 pMapId) { iType = UNLOAD_INSTANCE; ints[0] = pMapId; }
            void fillUnloadTileCmd(unsigned int pMapId, int x,int y) { iType = UNLOAD_TILE; ints[0] = x; ints[1] = y; ints[0]=pMapId; }
            void fillSetPosCmd(Vector3 pPos) { iType = SET_POS; floats[0] = pPos.x; floats[1]=pPos.y; floats[2]=pPos.z; }
            void fillTestVisCmd(int pMapId, Vector3 pPos1, Vector3 pPos2, bool result)
            {
                iType = TEST_VIS; floats[0] = pPos1.x; floats[1]=pPos1.y; floats[2]=pPos1.z;
                floats[3] = pPos2.x; floats[4]=pPos2.y; floats[5]=pPos2.z;
                ints[0] = result; ints[1] = pMapId;
            }
            void fillTestHeightCmd(int pMapId, Vector3 pPos, float result)
            {
                iType = TEST_HEIGHT; floats[0] = pPos.x; floats[1]=pPos.y; floats[2]=pPos.z;
                floats[3] = result; ints[0] = pMapId;
            }
            void fillTestObjectHitCmd(int pMapId, Vector3 pPos1, Vector3 pPos2, Vector3 pResultPos, bool result)
            {
                iType = TEST_OBJECT_HIT; floats[0] = pPos1.x; floats[1]=pPos1.y; floats[2]=pPos1.z;
                floats[3] = pPos2.x; floats[4]=pPos2.y; floats[5]=pPos2.z;
                floats[6] = pResultPos.x; floats[7]=pResultPos.y; floats[8]=pResultPos.z;
                ints[0] = result; ints[1] = pMapId;
            }

            bool isCoreCmd() const { return(iType != TEST_VIS); }
    };

    //==========================================

    class CommandFileRW
    {
        private:
            std::string iFileName;
            long iLastPos;
            Array<Array<Command> > iCommandArray;
            bool resetfile;
            bool iWritingEnabled;
        public:
            CommandFileRW() { iLastPos=0; iWritingEnabled = true; resetfile = true;}
            CommandFileRW(const std::string& pFileName) { iLastPos = 0; iFileName = pFileName; iWritingEnabled = true; resetfile = true; }
            void setResetFile() { resetfile = true; }
            void enableWriting(bool pValue) { iWritingEnabled = pValue; }
            void setFileName(const std::string& pName) { iFileName = pName; }
            bool getNewCommands(Array<Command>& commandArray);
            const Array<Array<Command> >& getFullCommandArray() { return iCommandArray; }

            bool appendCmd(const Command& pCommand);
            bool appendCmds(const Array<Command>& pCmdArray);
    };

}
#endif
