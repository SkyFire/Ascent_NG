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

#ifdef CLUSTERING
#ifndef _CLUSTERINTERFACE_H
#define _CLUSTERINTERFACE_H

#define MAX_SESSIONS 3000
#include "../ascent-realmserver/Structures.h"

class ClusterInterface;
typedef void(ClusterInterface::*ClusterInterfaceHandler)(WorldPacket&);

class ClusterInterface : public Singleton<ClusterInterface>
{
	
	WSClient * _clientSocket;
	FastQueue<WorldPacket*, Mutex> _pckQueue;
	time_t _lastConnectTime;
	WorldSession * _sessions[MAX_SESSIONS];
	bool m_connected;
	uint8 key[20];
	uint32 m_latency;
	Mutex m_mapMutex;

public:

	Mutex m_onlinePlayerMapMutex;
	typedef HM_NAMESPACE::hash_map<uint32,RPlayerInfo*> OnlinePlayerStorageMap;
	OnlinePlayerStorageMap _onlinePlayers;
	string GenerateVersionString();

	static ClusterInterfaceHandler PHandlers[IMSG_NUM_TYPES];
	static void InitHandlers();

	ClusterInterface();
	~ClusterInterface();

	void ForwardWoWPacket(uint16 opcode, uint32 size, const void * data, uint32 sessionid);
	void ConnectToRealmServer();
	
	RPlayerInfo * GetPlayer(uint32 guid)
	{
		//RPlayerInfo * inf;
		OnlinePlayerStorageMap::iterator itr;
		m_onlinePlayerMapMutex.Acquire();
		itr = _onlinePlayers.find(guid);
		m_onlinePlayerMapMutex.Release();
		return (itr == _onlinePlayers.end()) ? 0 : itr->second;
	}

	ASCENT_INLINE WorldSession * GetSession(uint32 sid) { return _sessions[sid]; }

	void HandleSessionRemoved(WorldPacket & pck);
	void HandleTeleportResult(WorldPacket & pck);
	void HandleAuthRequest(WorldPacket & pck);
	void HandleAuthResult(WorldPacket & pck);
	void HandleRegisterResult(WorldPacket & pck);
	void HandleCreateInstance(WorldPacket & pck);
	void HandleDestroyInstance(WorldPacket & pck);
	void HandlePlayerLogin(WorldPacket & pck);
	void HandlePackedPlayerInfo(WorldPacket & pck);
	void HandlePlayerInfo(WorldPacket & pck);
	void HandleWoWPacket(WorldPacket & pck);
	void HandlePlayerChangedServers(WorldPacket & pck);
	void HandleSaveAllPlayers(WorldPacket & pck);
	void HandleTransporterMapChange(WorldPacket & pck);
	void HandlePlayerTeleport(WorldPacket & pck);
	void HandleCreatePlayer(WorldPacket & pck);
	void HandleDestroyPlayerInfo(WorldPacket & pck);
	void HandleChannelAction(WorldPacket & pck);
	void HandleChannelLFGDungeonStatusRequest(WorldPacket & pck);

	ASCENT_INLINE void QueuePacket(WorldPacket * pck) { _pckQueue.Push(pck); }

	void Update();
	void DestroySession(uint32 sid);
	void ConnectionDropped();

	ASCENT_INLINE void SendPacket(WorldPacket * data) { if(_clientSocket) _clientSocket->SendPacket(data); }
	ASCENT_INLINE void SetSocket(WSClient * s) { _clientSocket = s; }

	void RequestTransfer(Player* plr, uint32 MapId, uint32 InstanceId, LocationVector & vec);
};

#define sClusterInterface ClusterInterface::getSingleton()

#endif
#endif
