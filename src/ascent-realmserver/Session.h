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

#ifndef _R_SESSION_H
#define _R_SESSION_H

typedef void(Session::*SessionPacketHandler)(WorldPacket&);

class Session
{
public:
	friend class WorldSocket;
	friend class CharacterStorage;
	Session(uint32 id);
	~Session();

protected:
	FastQueue<WorldPacket*, Mutex> m_readQueue;
	WorldSocket * m_socket;
	WServer * m_server;
	WServer * m_nextServer;
	uint32 m_sessionId;
	uint32 m_accountId;
	uint32 m_ClientBuild;
	RPlayerInfo * m_currentPlayer;
	uint32 m_latency;
	uint32 m_accountFlags;
	string m_GMPermissions;
	string m_accountName;
	uint32 m_build;
	uint32 language;
	static SessionPacketHandler Handlers[NUM_MSG_TYPES];
	bool m_loadedPlayerData;

public:
	bool deleted;
	static void InitHandlers();
	void Update();
	ASCENT_INLINE RPlayerInfo * GetPlayer() { return m_currentPlayer; }

	ASCENT_INLINE void ClearCurrentPlayer() { m_currentPlayer = 0; }
	ASCENT_INLINE void ClearServers() { m_nextServer = m_server = 0; }
	ASCENT_INLINE void SetNextServer() { m_server = m_nextServer; }
	ASCENT_INLINE void SetNextServer(WServer* s) { m_nextServer = s; };
	ASCENT_INLINE void SetServer(WServer * s) { m_server = s; }
	ASCENT_INLINE WServer * GetServer() { return m_server; }
	ASCENT_INLINE WorldSocket * GetSocket() { return m_socket; }
	ASCENT_INLINE uint32 GetAccountId() { return m_accountId; }
	ASCENT_INLINE uint32 GetSessionId() { return m_sessionId; }
	ASCENT_INLINE std::string GetAccountPermissions() { return m_GMPermissions; }
	ASCENT_INLINE std::string GetAccountName() { return m_accountName; }
	ASCENT_INLINE uint32 GetAccountFlags() { return m_accountFlags; }
	ASCENT_INLINE uint32 GetClientBuild() { return m_ClientBuild; }

	bool CanUseCommand(std::string cmdstr)
	{
		if(m_GMPermissions.find("a") != string::npos)
			return true;
		
		if(cmdstr.length() <= 1)
			return (m_GMPermissions.find(cmdstr) != string::npos);
		else
		{
			for(size_t i = 0; i < cmdstr.length(); i++)
			{
				if(m_GMPermissions.find(cmdstr[i]) == string::npos)
					return false;
			}
		}
		return true;
	}

	bool HasFlag(uint32 flag) { return (m_accountFlags & flag) != 0; }

	void SendPacket(WorldPacket * data)
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->SendPacket(data);
	}

	ASCENT_INLINE void SendPacket(StackPacket * packet)
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->SendPacket(packet);
	}

	void OutPacket(uint16 opcode, uint16 len, const void* data)
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->OutPacket(opcode, len, data);
	}

	void Disconnect()
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->Disconnect();
	}

	void HandlePlayerLogin(WorldPacket & pck);

	void HandleCharacterEnum(WorldPacket & pck);
	void HandleCharacterEnumProc();

	void HandleCharacterCreate(WorldPacket & pck);
	void HandleCharacterDelete(WorldPacket & pck);
	void HandleCharacterRename(WorldPacket & pck);
	
	void HandleItemQuerySingleOpcode(WorldPacket & pck);
	void HandleCreatureQueryOpcode(WorldPacket & pck);
	void HandleGameObjectQueryOpcode(WorldPacket & pck);
	void HandleItemPageQueryOpcode(WorldPacket & pck);
	void HandleNpcTextQueryOpcode(WorldPacket & pck);

	void HandleRealmSplitQuery(WorldPacket & pck);
	void HandleNameQueryOpcode( WorldPacket & recv_data );
	void HandleQueryTimeOpcode( WorldPacket & recv_data );
	void HandlePageTextQueryOpcode( WorldPacket & recv_data );
	void HandleItemNameQueryOpcode( WorldPacket & recv_data );

	/// Channel Opcodes (ChannelHandler.cpp)
	void HandleChannelJoin(WorldPacket& recvPacket);
	void HandleChannelLeave(WorldPacket& recvPacket);
	void HandleChannelList(WorldPacket& recvPacket);
	void HandleChannelPassword(WorldPacket& recvPacket);
	void HandleChannelSetOwner(WorldPacket& recvPacket);
	void HandleChannelOwner(WorldPacket& recvPacket);
	void HandleChannelModerator(WorldPacket& recvPacket);
	void HandleChannelUnmoderator(WorldPacket& recvPacket);
	void HandleChannelMute(WorldPacket& recvPacket);
	void HandleChannelUnmute(WorldPacket& recvPacket);
	void HandleChannelInvite(WorldPacket& recvPacket);
	void HandleChannelKick(WorldPacket& recvPacket);
	void HandleChannelBan(WorldPacket& recvPacket);
	void HandleChannelUnban(WorldPacket& recvPacket);
	void HandleChannelAnnounce(WorldPacket& recvPacket);
	void HandleChannelModerate(WorldPacket& recvPacket);
	void HandleChannelNumMembersQuery(WorldPacket & recvPacket);
	void HandleChannelRosterQuery(WorldPacket & recvPacket);
	void HandleMessagechatOpcode( WorldPacket & recv_data );
};

#endif


