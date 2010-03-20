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

#define MAX_SESSIONS 3000

class ClientMgr : public Singleton<ClientMgr>
{
public:
	typedef std::tr1::unordered_map<string, RPlayerInfo*> ClientStringMap;
	typedef HM_NAMESPACE::hash_map<uint32, RPlayerInfo*> ClientMap;
	typedef HM_NAMESPACE::hash_map<uint32, Session*> SessionMap;
	ClientMap m_clients;
	RWLock m_lock;

protected:
	ClientStringMap m_stringclients;
	uint32 m_maxSessionId;
public:
	SessionMap m_sessions;
	HM_NAMESPACE::hash_map<RPlayerInfo*, Session*> m_sessionsbyinfo;
	std::vector<uint32> m_reusablesessions;
	std::vector<uint32> m_pendingdeletesessionids;

	ClientMgr();
	~ClientMgr();

	/* create rplayerinfo struct */
	RPlayerInfo * CreateRPlayer(uint32 guid);

	/* destroy rplayerinfo struct */
	void DestroyRPlayerInfo(uint32 guid);

	ASCENT_INLINE Session* GetSessionByRPInfo(RPlayerInfo* p)
	{
		m_lock.AcquireReadLock();
		HM_NAMESPACE::hash_map<RPlayerInfo*, Session*>::iterator itr = m_sessionsbyinfo.find(p);
		if (itr == m_sessionsbyinfo.end())
		{
			m_lock.ReleaseReadLock();
			return NULL;
		}
		Session* s = itr->second;
		m_lock.ReleaseReadLock();
		return s;
	}

	ASCENT_INLINE void AddSessionRPInfo(Session* s, RPlayerInfo* p)
	{
		m_lock.AcquireWriteLock();
		m_sessionsbyinfo.insert(std::make_pair<RPlayerInfo*, Session*>(p, s));
		m_lock.ReleaseWriteLock();
	}

	ASCENT_INLINE void AddStringPlayerInfo(RPlayerInfo* p)
	{
		m_lock.AcquireWriteLock();
		m_stringclients.insert(std::make_pair<std::string, RPlayerInfo*>(p->Name, p));
		m_lock.ReleaseWriteLock();
	}

	/* get rplayer */
	ASCENT_INLINE RPlayerInfo * GetRPlayer(uint32 guid)
	{
		m_lock.AcquireReadLock();
		ClientMap::iterator itr = m_clients.find(guid);
		if (itr == m_clients.end())
		{
			m_lock.ReleaseReadLock();
			return NULL;
		}
		RPlayerInfo* r = itr->second;
		m_lock.ReleaseReadLock();
		return r;
	}

	/* get rplayer */
	ASCENT_INLINE RPlayerInfo * GetRPlayer(std::string name)
	{
		m_lock.AcquireReadLock();
		ClientStringMap::iterator itr = m_stringclients.find(name);
		if (itr == m_stringclients.end())
		{
			m_lock.ReleaseReadLock();
			return NULL;
		}
		RPlayerInfo* r = itr->second;
		m_lock.ReleaseReadLock();
		return r;
	}

	/* send "mini" client data to all servers */
	void SendPackedClientInfo(WServer * server);

	/* get session by id */
	ASCENT_INLINE Session * GetSession(uint32 Id)
	{
		SessionMap::iterator itr=m_sessions.find(Id);
		if (itr == m_sessions.end() || itr->second->deleted)
			return NULL;
		return itr->second;
	}

	ASCENT_INLINE Session* GetSessionByAccountId(uint32 Id)
	{
		m_lock.AcquireReadLock();
		for (SessionMap::iterator itr=m_sessions.begin(); itr!=m_sessions.end(); ++itr)
		{
			if (!itr->second->deleted && itr->second->GetAccountId() == Id)
			{
				Session* s=itr->second;
				m_lock.ReleaseReadLock();
				return s;
			}
		}
		m_lock.ReleaseReadLock();
		return NULL;
	}

	/* create a new session, returns null if the player is already logged in */
	Session * CreateSession(uint32 AccountId);

	void DestroySession(uint32 sessionid);

	/* updates sessions */
	void Update();
};

#define sClientMgr ClientMgr::getSingleton()


