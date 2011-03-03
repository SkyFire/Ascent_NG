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

#include "RStdAfx.h"

initialiseSingleton(ClusterMgr);
ClusterMgr::ClusterMgr()
{
	memset(SingleInstanceMaps, 0, sizeof(WServer*) * MAX_SINGLE_MAPID);
	memset(WorkerServers, 0, sizeof(WServer*) * MAX_WORKER_SERVERS);
	m_maxInstanceId = 0;
	m_maxWorkerServer = 0;
	Log.Success("ClusterMgr", "Interface Created");

	WServer::InitHandlers();
}

WServer * ClusterMgr::GetServerByInstanceId(uint32 InstanceId)
{
	InstanceMap::iterator itr = Instances.find(InstanceId);
	return (itr == Instances.end()) ? 0 : itr->second->Server;
}

WServer * ClusterMgr::GetServerByMapId(uint32 MapId)
{
	ASSERT(IS_MAIN_MAP(MapId));
	return SingleInstanceMaps[MapId]->Server;
}

Instance * ClusterMgr::GetInstanceByInstanceId(uint32 InstanceId)
{
	InstanceMap::iterator itr = Instances.find(InstanceId);
	return (itr == Instances.end()) ? 0 : itr->second;
}

Instance * ClusterMgr::GetInstanceByMapId(uint32 MapId)
{
	m_lock.AcquireReadLock();
	Instance* s = SingleInstanceMaps[MapId];
	m_lock.ReleaseReadLock();
	return s;
}

Instance* ClusterMgr::GetAnyInstance()
{
	//
	m_lock.AcquireReadLock();
	for (uint32 i=0; i<MAX_SINGLE_MAPID; ++i)
	{
		if (SingleInstanceMaps[i] != NULL)
		{
			m_lock.ReleaseReadLock();
			return SingleInstanceMaps[i];
		}
	}
	m_lock.ReleaseReadLock();
	return NULL;

}

Instance * ClusterMgr::GetPrototypeInstanceByMapId(uint32 MapId)
{
	m_lock.AcquireReadLock();
	//lets go through all the instances of this map and find the one with the least instances :P
	std::multimap<uint32, Instance*>::iterator itr = InstancedMaps.find(MapId);

	if (itr == InstancedMaps.end())
	{
		m_lock.ReleaseReadLock();
		return NULL;
	}

	Instance* i = NULL;
	uint32 min = 500000;
	for (; itr != InstancedMaps.upper_bound(MapId); ++itr)
	{
		if (itr->second->MapCount < min)
		{
			min = itr->second->MapCount;
			i = itr->second;
		}
	}

	m_lock.ReleaseReadLock();
	return i;
}


WServer * ClusterMgr::CreateWorkerServer(WSSocket * s)
{
	/* find an id */
	m_lock.AcquireWriteLock();
	uint32 i;
	for(i = 1; i < MAX_WORKER_SERVERS; ++i)
	{
		if(WorkerServers[i] == 0)
			break;
	}

	if(i == MAX_WORKER_SERVERS)
	{
		m_lock.ReleaseWriteLock();
		return 0;		// No spaces
	}

	DEBUG_LOG("ClusterMgr", "Allocating worker server %u to %s:%u", i, s->GetRemoteIP().c_str(), s->GetRemotePort());
	WorkerServers[i] = new WServer(i, s);
	if(m_maxWorkerServer < i)
		m_maxWorkerServer = i;
	m_lock.ReleaseWriteLock();
	return WorkerServers[i];
}

void ClusterMgr::AllocateInitialInstances(WServer * server, vector<uint32>& preferred)
{
	m_lock.AcquireReadLock();
	vector<uint32> result;
	result.reserve(10);

	for(vector<uint32>::iterator itr = preferred.begin(); itr != preferred.end(); ++itr)
	{
		if(SingleInstanceMaps[*itr] == 0)
		{
			result.push_back(*itr);
		}
	}
	m_lock.ReleaseReadLock();

	for(vector<uint32>::iterator itr = result.begin(); itr != result.end(); ++itr)
	{
		CreateInstance(*itr, server);
	}
}

Instance * ClusterMgr::CreateInstance(uint32 MapId, WServer * server)
{
	MapInfo * info = WorldMapInfoStorage.LookupEntry(MapId);
	if(!info)
		return NULL;

	Instance * pInstance = new Instance;
	pInstance->InstanceId = ++m_maxInstanceId;
	pInstance->MapId = MapId;
	pInstance->Server = server;

	m_lock.AcquireWriteLock();
	Instances.insert( make_pair( pInstance->InstanceId, pInstance ) );

	if(info->type == 0 /*INSTANCE_NULL*/)
		SingleInstanceMaps[MapId] = pInstance;
	m_lock.ReleaseWriteLock();

	/* tell the actual server to create the instance */
	WorldPacket data(ISMSG_CREATE_INSTANCE, 8);
	data << MapId << pInstance->InstanceId;
	server->SendPacket(&data);
	server->AddInstance(pInstance);
	DEBUG_LOG("ClusterMgr", "Allocating instance %u on map %u to server %u", pInstance->InstanceId, pInstance->MapId, server->GetID());
	return pInstance;
}

WServer * ClusterMgr::GetWorkerServerForNewInstance()
{
    WServer * lowest = 0;
	int32 lowest_load = -1;

	/* for now we'll just work with the instance count. in the future we might want to change this to
	   use cpu load instead. */

	m_lock.AcquireReadLock();
	for(uint32 i = 0; i < MAX_WORKER_SERVERS; ++i) {
		if(WorkerServers[i] != 0) {
			if((int32)WorkerServers[i]->GetInstanceCount() < lowest_load)
			{
				lowest = WorkerServers[i];
				lowest_load = int32(WorkerServers[i]->GetInstanceCount());
			}
		}
	}
	m_lock.ReleaseReadLock();

	return lowest;
}

/* create new instance based on template, or a saved instance */
Instance * ClusterMgr::CreateInstance(uint32 InstanceId, uint32 MapId)
{

	MapInfo * info = WorldMapInfoStorage.LookupEntry(MapId);
	if(!info)
		return NULL;

	/* pick a server for us :) */
	WServer * server = GetWorkerServerForNewInstance();
	if(!server)
		return NULL;

	ASSERT(GetInstance(InstanceId) == NULL);

	/* bump up the max id if necessary */
	if(m_maxInstanceId <= InstanceId)
		m_maxInstanceId = InstanceId + 1;

    Instance * pInstance = new Instance;
	pInstance->InstanceId = InstanceId;
	pInstance->MapId = MapId;
	pInstance->Server = server;

	m_lock.AcquireWriteLock();
	Instances.insert( make_pair( InstanceId, pInstance ) );

	if(info->type == 0 /*INSTANCE_NULL*/)
		SingleInstanceMaps[MapId] = pInstance;
	m_lock.ReleaseWriteLock();

	/* tell the actual server to create the instance */
	WorldPacket data(ISMSG_CREATE_INSTANCE, 8);
	data << MapId << InstanceId;
	server->SendPacket(&data);
	server->AddInstance(pInstance);
	DEBUG_LOG("ClusterMgr", "Allocating instance %u on map %u to server %u", pInstance->InstanceId, pInstance->MapId, server->GetID());
	return pInstance;
}

void ClusterMgr::Update()
{
	for(uint32 i = 1; i <= m_maxWorkerServer; ++i)
		if(WorkerServers[i])
			WorkerServers[i]->Update();
}

void ClusterMgr::DistributePacketToAll(WorldPacket * data, WServer * exclude)
{
	for(uint32 i = 0; i <= m_maxWorkerServer; ++i)
		if(WorkerServers[i] && WorkerServers[i] != exclude)
			WorkerServers[i]->SendPacket(data);
}

void ClusterMgr::OnServerDisconnect(WServer* s)
{
	m_lock.AcquireWriteLock();

	for(uint32 i = 0; i <= m_maxWorkerServer; ++i)
	{
		if (WorkerServers[i] == s)
		{
			Log.Warning("ClusterMgr", "Removing worker server %u due to socket disconnection", i);
			WorkerServers[i] = NULL;
		}
	}


	for (uint32 i=0; i<MAX_SINGLE_MAPID; ++i)
	{
		if (SingleInstanceMaps[i] != NULL && SingleInstanceMaps[i]->Server == s)
		{
			Log.Warning("ClusterMgr", "Removing single map %u due to worker server disconnection", i);
			delete SingleInstanceMaps[i];
			SingleInstanceMaps[i] = NULL;
		}
	}

	for (InstanceMap::iterator itr=Instances.begin(); itr!=Instances.end(); ++itr)
	{
		if (itr->second->Server == s)
		{
			Log.Warning("ClusterMgr", "Removing instance %u on map %u due to worker server disconnection", itr->first, itr->second->MapId);
			delete itr->second;
			Instances.erase(itr);
			itr=Instances.begin();

			//don't get out of range :P
			if (Instances.size() == 0)
				break;
		}
	}

	for (std::multimap<uint32, Instance*>::iterator itr=InstancedMaps.begin(); itr!=InstancedMaps.end(); ++itr)
	{
		if (itr->second->Server == s)
		{
			Log.Warning("ClusterMgr", "Removing instance prototype map %u due to worker server disconnection", itr->first);
			delete itr->second;
			InstancedMaps.erase(itr);
			itr=InstancedMaps.begin();

			//don't get out of range :P
			if (InstancedMaps.size() == 0)
				break;
		}
	}

	delete s;

	m_lock.ReleaseWriteLock();
}
