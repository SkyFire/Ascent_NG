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

#define MAX_SESSIONS_PER_SERVER 1000

struct Instance;
typedef void(WServer::*WServerHandler)(WorldPacket &);

class WServer
{
	static WServerHandler PHandlers[IMSG_NUM_TYPES];
	uint32 m_id;
	WSSocket * m_socket;
	FastQueue<WorldPacket*, Mutex> m_recvQueue;
	list<Instance*> m_instances;

public:
	static void InitHandlers();
	WServer(uint32 id, WSSocket * s);
	~WServer() { sSocketGarbageCollector.QueueSocket(m_socket); };

	ASCENT_INLINE size_t GetInstanceCount() { return m_instances.size(); }
	ASCENT_INLINE void SendPacket(WorldPacket * data) { if(m_socket) m_socket->SendPacket(data); }
	ASCENT_INLINE void SendWoWPacket(Session * from, WorldPacket * data) { if(m_socket) m_socket->SendWoWPacket(from, data); }
	ASCENT_INLINE void AddInstance(Instance * pInstance) { m_instances.push_back(pInstance); }
	ASCENT_INLINE void QueuePacket(WorldPacket * data) { m_recvQueue.Push(data); }
	ASCENT_INLINE uint32 GetID() { return m_id; }

	void Update();

protected:
	/* packet handlers */
	void HandleRegisterWorker(WorldPacket & pck);
	void HandleWoWPacket(WorldPacket & pck);
	void HandlePlayerLoginResult(WorldPacket & pck);
	void HandlePlayerLogout(WorldPacket & pck);
	void HandleTeleportRequest(WorldPacket & pck);
	void HandleError(WorldPacket & pck);
	void HandleSwitchServer(WorldPacket & pck);
	void HandleCreatePlayerResult(WorldPacket & pck);
	void HandleTransporterMapChange(WorldPacket & pck);
	void HandleSaveAllPlayers(WorldPacket & pck);
	void HandlePlayerTeleport(WorldPacket & pck);
	void HandlePlayerInfo(WorldPacket & pck);
	void HandleChannelAction(WorldPacket & pck);
	void HandleChannelUpdate(WorldPacket & pck);
	void HandleChannelLFGDungeonStatusReply(WorldPacket& pck);
};
