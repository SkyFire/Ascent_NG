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

#include "StdAfx.h"

bool ChatHandler::HandleSetBGScoreCommand(const char* args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandleStartBGCommand(const char *args, WorldSession *m_session)
{
	if(m_session->GetPlayer()->m_bg && sEventMgr.HasEvent(m_session->GetPlayer()->m_bg, EVENT_BATTLEGROUND_COUNTDOWN))
	{
		sEventMgr.RemoveEvents(m_session->GetPlayer()->m_bg, EVENT_BATTLEGROUND_COUNTDOWN);
		m_session->GetPlayer()->m_bg->Start();
	}
	return true;
}

bool ChatHandler::HandlePauseBGCommand(const char *args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandleBGInfoCommnad(const char *args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandleBattlegroundCommand(const char* args, WorldSession *m_session)
{
	uint32 type = atoi(args);
	if(type != 1 && type != 2 && type != 3 && type != 4 && type != 5 && type != 7)
		return false;

	PlayerPointer plr = getSelectedChar(m_session, true);
	if(!plr) return true;
	BattlegroundManager.HandleBattlegroundListPacket(plr->GetSession(), atoi(args), true);
	return true;
}

bool ChatHandler::HandleSetWorldStateCommand(const char* args, WorldSession *m_session)
{
	uint32 id,val;
	if(sscanf(args,"%u %u", &id,&val)!=2)
		return false;

	//if(m_session->GetPlayer()->m_bg)
	//{
		m_session->GetPlayer()->GetMapMgr()->GetStateManager().CreateWorldState(id, val);
		m_session->GetPlayer()->GetMapMgr()->GetStateManager().UpdateWorldState(id, val);
	//}

	return true;
}

bool ChatHandler::HandlePlaySoundCommand(const char* args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandleSetBattlefieldStatusCommand(const char* args, WorldSession *m_session)
{
	uint32 type = atoi(args);
	BattlegroundManager.SendBattlegroundQueueStatus(m_session->GetPlayer(), type);
	return true;
}

bool ChatHandler::HandleBattlegroundExitCommand(const char* args, WorldSession* m_session)
{
	return true;
}

bool ChatHandler::HandleBattlegroundForcestartCommand(const char* args, WorldSession* m_session)
{
	BattlegroundManager.EventQueueUpdate(true);
	return true;
}
