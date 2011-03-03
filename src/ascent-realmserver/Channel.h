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

#ifndef __CHANNEL_H
#define __CHANNEL_H

using namespace std;

enum CHANNEL_FLAGS
{
	CHANNEL_FLAG_NONE				= 0x00,
	CHANNEL_FLAG_OWNER				= 0x01,
	CHANNEL_FLAG_MODERATOR			= 0x02,
	CHANNEL_FLAG_VOICED				= 0x04,
	CHANNEL_FLAG_MUTED				= 0x08,
	CHANNEL_FLAG_CUSTOM				= 0x10,
	CHANNEL_FLAG_MICROPHONE_MUTE	= 0x20,
};

enum CHANNEL_NOTIFY_FLAGS
{
	CHANNEL_NOTIFY_FLAG_JOINED		= 0x00,
	CHANNEL_NOTIFY_FLAG_LEFT		= 0x01,
	CHANNEL_NOTIFY_FLAG_YOUJOINED	= 0x02,
	CHANNEL_NOTIFY_FLAG_YOULEFT		= 0x03,
	CHANNEL_NOTIFY_FLAG_WRONGPASS	= 0x04,
	CHANNEL_NOTIFY_FLAG_NOTON		= 0x05,
	CHANNEL_NOTIFY_FLAG_NOTMOD		= 0x06,
	CHANNEL_NOTIFY_FLAG_SETPASS		= 0x07,
	CHANNEL_NOTIFY_FLAG_CHGOWNER	= 0x08,
	CHANNEL_NOTIFY_FLAG_NOT_ON_2	= 0x09,
	CHANNEL_NOTIFY_FLAG_NOT_OWNER	= 0x0A,
	CHANNEL_NOTIFY_FLAG_WHO_OWNER	= 0x0B,
	CHANNEL_NOTIFY_FLAG_MODE_CHG	= 0x0C,
	CHANNEL_NOTIFY_FLAG_ENABLE_ANN	= 0x0D,
	CHANNEL_NOTIFY_FLAG_DISABLE_ANN	= 0x0E,
	CHANNEL_NOTIFY_FLAG_MODERATED	= 0x0F,
	CHANNEL_NOTIFY_FLAG_UNMODERATED	= 0x10,
	CHANNEL_NOTIFY_FLAG_YOUCANTSPEAK= 0x11,
	CHANNEL_NOTIFY_FLAG_KICKED		= 0x12,
	CHANNEL_NOTIFY_FLAG_YOURBANNED	= 0x13,
	CHANNEL_NOTIFY_FLAG_BANNED		= 0x14,
	CHANNEL_NOTIFY_FLAG_UNBANNED	= 0x15,
	CHANNEL_NOTIFY_FLAG_UNK_1		= 0x16,
	CHANNEL_NOTIFY_FLAG_ALREADY_ON	= 0x17,
	CHANNEL_NOTIFY_FLAG_INVITED		= 0x18,
	CHANNEL_NOTIFY_FLAG_WRONG_FACT	= 0x19,
	CHANNEL_NOTIFY_FLAG_UNK_2		= 0x1A,
	CHANNEL_NOTIFY_FLAG_UNK_3		= 0x1B,
	CHANNEL_NOTIFY_FLAG_UNK_4		= 0x1C,
	CHANNEL_NOTIFY_FLAG_YOU_INVITED	= 0x1D,
	CHANNEL_NOTIFY_FLAG_UNK_5		= 0x1E,
	CHANNEL_NOTIFY_FLAG_UNK_6		= 0x1F,
	CHANNEL_NOTIFY_FLAG_UNK_7		= 0x20,
	CHANNEL_NOTIFY_FLAG_NOT_IN_LFG	= 0x21,
	CHANNEL_NOTIFY_FLAG_VOICE_ON	= 0x22,
	CHANNEL_NOTIFY_FLAG_VOICE_OFF	= 0x23,
};

class Channel
{
	Mutex m_lock;
	typedef map<RPlayerInfo* , uint32> MemberMap;
	MemberMap m_members;
	set<uint32> m_bannedMembers;
public:
	friend class ChannelIterator;
	static void LoadConfSettings();
	string m_name;
	string m_password;
	uint8 m_flags;
	uint32 m_typeId;
	bool m_general;
	bool m_muted;
	bool m_announce;
	uint32 m_team;
	ASCENT_INLINE size_t GetNumMembers() { return m_members.size(); }
	uint32 m_minimumLevel;
	ChatChannelDBC * pDBC;
	uint32 m_channelId;
	bool m_deleted;
public:
	friend class ChannelMgr;
	Channel(const char * name, uint32 team, uint32 type_id, uint32 id);
	~Channel();

	void AttemptJoin(RPlayerInfo* plr, const char * password);
	void Part(RPlayerInfo* plr, bool silent);
	void Kick(RPlayerInfo* plr, RPlayerInfo* die_player, bool ban);
	void Invite(RPlayerInfo* plr, RPlayerInfo* new_player);
	void Moderate(RPlayerInfo* plr);
	void Mute(RPlayerInfo* plr, RPlayerInfo* die_player);
	void Voice(RPlayerInfo* plr, RPlayerInfo* v_player);
	void Unmute(RPlayerInfo* plr, RPlayerInfo* die_player);
	void Devoice(RPlayerInfo* plr, RPlayerInfo* v_player);
	void Say(RPlayerInfo* plr, const char * message, RPlayerInfo* for_gm_client, bool forced);
	void Unban(RPlayerInfo* plr, RPlayerInfo * bplr);
	void GiveModerator(RPlayerInfo* plr, RPlayerInfo* new_player);
	void TakeModerator(RPlayerInfo* plr, RPlayerInfo* new_player);
	void Announce(RPlayerInfo* plr);
	void Password(RPlayerInfo* plr, const char * pass);
	void List(RPlayerInfo* plr);
	void GetOwner(RPlayerInfo* plr);

	void SetOwner(RPlayerInfo* oldpl, RPlayerInfo* plr);

	// Packet Forging
	void SendAlreadyOn(RPlayerInfo* plr, RPlayerInfo* plr2);
	void SendYouAreBanned(RPlayerInfo* plr);
	void SendNotOn(RPlayerInfo* plr);
	void SendNotOwner(RPlayerInfo* plr);
	void SendYouCantSpeak(RPlayerInfo* plr);
	void SendModeChange(RPlayerInfo* plr, uint8 old_flags, uint8 new_flags);

	void SendToAll(WorldPacket * data);
	void SendToAll(WorldPacket * data, RPlayerInfo* plr);

	bool HasMember(RPlayerInfo* pRPlayerInfo);
};

class ChannelIterator
{
	Channel::MemberMap::iterator m_itr;
	Channel::MemberMap::iterator m_endItr;
	bool m_searchInProgress;
	Channel * m_target;
public:
	ChannelIterator(Channel* target) : m_searchInProgress(false),m_target(target) {}
	~ChannelIterator() { if(m_searchInProgress) { EndSearch(); } }

	void BeginSearch()
	{
		// iteminterface doesn't use mutexes, maybe it should :P
		ASSERT(!m_searchInProgress);
		m_target->m_lock.Acquire();
		m_itr = m_target->m_members.begin();
		m_endItr = m_target->m_members.end();
		m_searchInProgress=true;
	}

	void EndSearch()
	{
		// nothing here either
		ASSERT(m_searchInProgress);
		m_target->m_lock.Release();
		m_searchInProgress=false;
	}

	RPlayerInfo* operator*() const
	{
		return m_itr->first;
	}

	RPlayerInfo* operator->() const
	{
		return m_itr->first;
	}

	void Increment()
	{
		if(!m_searchInProgress)
			BeginSearch();

		if(m_itr==m_endItr)
			return;

		++m_itr;
	}

	ASCENT_INLINE RPlayerInfo* Grab() { return m_itr->first; }
	ASCENT_INLINE bool End() { return (m_itr==m_endItr)?true:false; }
};

#endif
