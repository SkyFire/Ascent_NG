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

#include "RStdAfx.h"

initialiseSingleton(ClientMgr);

ClientMgr::ClientMgr()
{
	Session::InitHandlers();
	m_maxSessionId = 0;
	Log.Success("ClientMgr", "Interface Created");
}

ClientMgr::~ClientMgr()
{
	for (SessionMap::iterator itr=m_sessions.begin(); itr!=m_sessions.end(); ++itr)
		delete itr->second;

	for (ClientMap::iterator itr=m_clients.begin(); itr!=m_clients.end(); ++itr)
		delete itr->second;
};

void ClientMgr::SendPackedClientInfo(WServer * server)
{
	if(!m_clients.size())
		return;

	WorldPacket data(ISMSG_PACKED_PLAYER_INFO, sizeof(RPlayerInfo) * m_clients.size() + 4);
	data << uint32(m_clients.size());

	/* pack them all togther, w000t! */
	ClientMap::iterator itr = m_clients.begin();
	RPlayerInfo * pi;
	for(; itr != m_clients.end(); ++itr)
	{
		pi = itr->second;
		pi->Pack(data);
	}

    /* TODO: compress the packet */
	server->SendPacket(&data);
}

Session * ClientMgr::CreateSession(uint32 AccountId)
{
	m_lock.AcquireWriteLock();

	//lets generate a session id
	//get from reusable
	uint32 sessionid = 0;
	if (m_reusablesessions.size() > 0)
	{
		sessionid = m_reusablesessions[m_reusablesessions.size() - 1];
		m_reusablesessions.pop_back();
	}
	else
	{
		sessionid = ++m_maxSessionId;
		Log.Debug("Session", "New max session id: %u", sessionid);
	}

	//ok, if we have a session with this account, add it to delete queue
	for (SessionMap::iterator itr=m_sessions.begin(); itr!=m_sessions.end(); ++itr)
		if (itr->second->GetAccountId() == AccountId)
			m_pendingdeletesessionids.push_back(itr->first);

	//we couldn't generate an id for some reason
	if(sessionid == 0)
		return NULL;

	Log.Debug("ClientMgr", "Allocating session %u for account id %u", sessionid, AccountId);
	Session* s = new Session(sessionid);

	m_sessions.insert(SessionMap::value_type(sessionid, s));

	m_lock.ReleaseWriteLock();
	return s;
}

void ClientMgr::Update()
{
	//get teh write lock for this so we don't fuck up other things
	m_lock.AcquireWriteLock();
	for (std::vector<uint32>::iterator itr=m_pendingdeletesessionids.begin(); itr!=m_pendingdeletesessionids.end(); ++itr)
	{
		SessionMap::iterator itr2 = m_sessions.find(*itr);
		if (itr2 == m_sessions.end()) //uh oh
			continue;
		Session* s=itr2->second;

		//use a reference int incase a player times out and logs in again :), were under a write lock here, were synced
		if (s->GetPlayer() != NULL && --s->GetPlayer()->references == 0)
		{
			m_sessionsbyinfo.erase(s->GetPlayer());
			m_clients.erase(s->GetPlayer()->Guid);
			m_stringclients.erase(s->GetPlayer()->Name);
			delete s->GetPlayer();
		}

		m_reusablesessions.push_back(*itr);
		m_sessions.erase(itr2);
		delete s;
	}
	m_lock.ReleaseWriteLock();

	for (SessionMap::iterator itr=m_sessions.begin(); itr!=m_sessions.end(); ++itr)
		if (!itr->second->deleted)
			itr->second->Update();
}

void ClientMgr::DestroySession(uint32 sessionid)
{
	m_lock.AcquireWriteLock();
	//session doesn't exist
	Session* s = GetSession(sessionid);
	if (s == NULL)
		return;

	s->deleted = true;
	m_pendingdeletesessionids.push_back(sessionid);
	m_lock.ReleaseWriteLock();
}

RPlayerInfo * ClientMgr::CreateRPlayer(uint32 guid)
{
	m_lock.AcquireWriteLock();
	ClientMap::iterator itr = m_clients.find(guid);
	if (itr == m_clients.end())
	{
		RPlayerInfo * rp = new RPlayerInfo;
		rp->references = 1;
		rp->Guid = guid;
		m_clients.insert(ClientMap::value_type(guid, rp));
		m_lock.ReleaseWriteLock();
		return rp;
	}
	else
	{
		//no need for sync here, were under a write lock
		++itr->second->references;
		//Sync_Add(&itr->second->references);
		m_lock.ReleaseWriteLock();
		return itr->second;
	}
}

void ClientMgr::DestroyRPlayerInfo(uint32 guid)
{
	RPlayerInfo * rp;
	m_lock.AcquireWriteLock();
	ClientMap::iterator itr = m_clients.find(guid);
	//no need for sync here, were under a write lock
	if(itr != m_clients.end() && --itr->second->references == 0)
	{
		rp = itr->second;
		m_clients.erase(itr);
		m_stringclients.erase(rp->Name);
		delete rp;
	}
	m_lock.ReleaseWriteLock();
}
