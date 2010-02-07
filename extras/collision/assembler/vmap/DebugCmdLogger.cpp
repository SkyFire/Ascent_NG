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

#include "DebugCmdLogger.h"

namespace VMAP
{

    bool CommandFileRW::appendCmd(const Command& pCommand)
    {
        #ifdef _DEBUG
        bool result = false;
        if(iWritingEnabled || pCommand.isCoreCmd())
        {
            FILE* f = fopen(iFileName.c_str(), "ab");
            if(f)
            {
                result = true;
                if(fwrite(&pCommand, sizeof(Command), 1, f) != 1) { result = false; }
                fclose(f);
            }
        }
        else
        {
            result = true;
        }
        return result;
        #else
        return true;
        #endif
    }

    //=========================================================

    bool CommandFileRW::appendCmds(const Array<Command>& pCmdArray)
    {
        #ifdef _DEBUG
        bool result = false;
        if(iWritingEnabled)
        {
            FILE* f;
            if(resetfile)
                f = fopen(iFileName.c_str(), "wb");
            else
                f = fopen(iFileName.c_str(), "ab");
            resetfile = false;

            if(f)
            {
                result = true;
                for(int i=0; i<pCmdArray.size(); ++i)
                {
                    if(fwrite(&pCmdArray[i], sizeof(Command), 1, f) != 1) { result = false; break; }
                }
                fclose(f);
            }
        }
        else
        {
            result = true;
        }
        return result;
        #else
        return true;
        #endif
    }

    //=========================================================

    bool CommandFileRW::getNewCommands(Array<Command>& pCmdArray)
    {
        bool result = false;
        FILE* f = fopen(iFileName.c_str(), "rb");
        if(f)
        {
            Command cmd;
            if(fseek(f, iLastPos, SEEK_SET) == 0) { result = true; }
            while(result)
            {
                if(fread(&cmd, sizeof(Command), 1, f) != 1)
                {
                    result = false;
                }
                iLastPos = ftell(f);
                if(cmd.getType() == STOP)
                {
                    break;
                }
                pCmdArray.append(cmd);
            }
            fclose(f);
        }
        if(result)
        {
            iCommandArray.append(pCmdArray);
        }
        return(result);
    }
    //========================================================
}
