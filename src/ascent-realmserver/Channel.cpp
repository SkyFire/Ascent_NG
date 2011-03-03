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
#include "ChannelMgr.h"
#include "Channel.h"
#include "Chat.h"

Mutex m_confSettingLock;
vector<string> m_bannedChannels;

void Channel::LoadConfSettings()
{
	string BannedChannels = Config.MainConfig.GetStringDefault("Channels", "BannedChannels", "");
	m_confSettingLock.Acquire();
	m_bannedChannels = StrSplit(BannedChannels, ";");
	m_confSettingLock.Release();
}

bool Channel::HasMember(RPlayerInfo* pRPlayerInfo)
{
	m_lock.Acquire();
	if( m_members.find(pRPlayerInfo) == m_members.end() )
	{
		m_lock.Release();
		return false;
	}
	else
	{
		m_lock.Release();
		return true;
	}
}

Channel::Channel(const char * name, uint32 team, uint32 type_id, uint32 id)
{
	m_flags = 0;
	m_announce = true;
	m_muted = false;
	m_general = false;
	m_name = string(name);
	m_team = team;
	m_typeId = type_id;
	m_channelId = id;
	m_minimumLevel = 1;
	m_deleted = false;

	pDBC = dbcChatChannels.LookupEntryForced(type_id);
	if( pDBC != NULL )
	{
		m_general = true;
		m_announce = false;

		m_flags |= 0x10;			// general flag
		// flags (0x01 = custom?, 0x04 = trade?, 0x20 = city?, 0x40 = lfg?, , 0x80 = voice?,		

		if( pDBC->flags & 0x08 )
			m_flags |= 0x08;		// trade

		if( pDBC->flags & 0x10 || pDBC->flags & 0x20 )
			m_flags |= 0x20;		// city flag

		if( pDBC->flags & 0x40000 )
			m_flags |= 0x40;		// lfg flag
	}
	else
		m_flags = 0x01;

	// scape stuff
	if( !stricmp(name, "global") || !stricmp(name, "mall") || !stricmp(name, "lfg") )
	{
		m_minimumLevel = 10;
		m_general = true;
		m_announce = false;
	}
}

void Channel::AttemptJoin(RPlayerInfo* plr, const char * password)
{
	if(plr == NULL || plr->GetSession() == NULL)
		return;
	Guard mGuard(m_lock);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	uint32 flags = CHANNEL_FLAG_NONE;

	if( !m_general && plr->GetSession()->CanUseCommand("c") )
		flags |= CHANNEL_FLAG_MODERATOR;

	if(!m_password.empty() && strcmp(m_password.c_str(), password) != 0)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_WRONGPASS) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(m_bannedMembers.find(plr->Guid) != m_bannedMembers.end())
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_YOURBANNED) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(m_members.find(plr) != m_members.end())
	{
		if( !m_general )
		{
			data << uint8(CHANNEL_NOTIFY_FLAG_ALREADY_ON) << m_name;
			plr->GetSession()->SendPacket(&data);
		}

		return;
	}

	if(m_members.empty() && !m_general)
		flags |= CHANNEL_FLAG_OWNER;

	WorldPacket data2(ISMSG_CHANNEL_ACTION, 9);
	data2 << uint8(CHANNEL_JOIN); //joined channel
	data2 << uint32(plr->Guid);
	data2 << uint32(m_channelId);
	plr->GetSession()->GetServer()->SendPacket(&data2);
	m_members.insert(make_pair(plr, flags));

	if(m_announce)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_JOINED) << m_name << uint64(plr->Guid);
		SendToAll(&data, NULL);
	}

	data.clear();
	if( m_flags & 0x40 && !plr->GetSession()->HasFlag( 0x2 /*ACCOUNT_FLAG_NO_AUTOJOIN*/ ) )
		data << uint8(CHANNEL_NOTIFY_FLAG_YOUJOINED) << m_name << uint8(0x1A) << uint32(0) << uint32(0);
	else
		data << uint8(CHANNEL_NOTIFY_FLAG_YOUJOINED) << m_name << m_flags << m_typeId << uint32(0);

	plr->GetSession()->SendPacket(&data);
}

void Channel::Part(RPlayerInfo* plr, bool silent)
{
	if(plr == NULL || plr->GetSession() == NULL)
		return;

	m_lock.Acquire();
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	uint32 flags;
	MemberMap::iterator itr = m_members.find(plr);
	if(itr == m_members.end())
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		m_lock.Release();
		return;
	}

	flags = itr->second;
	m_members.erase(itr);

	WorldPacket data2(ISMSG_CHANNEL_ACTION, 9);
	data2 << uint8(CHANNEL_PART); //left channel
	data2 << uint32(plr->Guid);
	data2 << uint32(m_channelId);
	plr->GetSession()->GetServer()->SendPacket(&data2);

	if(flags & CHANNEL_FLAG_OWNER)
	{
		// we need to find a new owner
		SetOwner(NULL, NULL);
	}

	if(plr->GetSession())
	{
		if( !silent )
		{
			data << uint8(CHANNEL_NOTIFY_FLAG_YOULEFT) << m_name << m_typeId << uint32(0) << uint8(0);
			plr->GetSession()->SendPacket(&data);
		}
	}

	if(m_announce)
	{
		data.clear();
		data << uint8(CHANNEL_NOTIFY_FLAG_LEFT) << m_name << uint64(plr->Guid);
		SendToAll(&data);

		/*		data.Initialize(SMSG_PLAYER_LEFT_CHANNEL);
		data << uint64(plr->Guid) << m_flags << m_id << m_name;
		SendToAll(&data);*/
	}

#ifndef WIN32
	if(m_members.size() == 0 )
	{
		m_lock.Release();
		channelmgr.RemoveChannel(this);
	}
	else
		m_lock.Release();
#else
	m_lock.Release();
#endif
}

void Channel::SetOwner(RPlayerInfo* oldpl, RPlayerInfo* plr)
{
	Guard mGuard(m_lock);
	RPlayerInfo* pOwner = NULL;
	uint32 oldflags, oldflags2;
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(oldpl != NULL)
	{
		MemberMap::iterator itr = m_members.find(oldpl);
		if(m_members.end() == itr)
		{
			data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
			plr->GetSession()->SendPacket(&data);
			return;
		}

		if(!(itr->second & CHANNEL_FLAG_OWNER))
		{
			data << uint8(CHANNEL_NOTIFY_FLAG_NOT_OWNER) << m_name;
			plr->GetSession()->SendPacket(&data);
			return;
		}
	}

	if(plr == NULL)
	{
		for(MemberMap::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
		{
			if(itr->second & CHANNEL_FLAG_OWNER)
			{
				// remove the old owner
				oldflags2 = itr->second;
				itr->second &= ~CHANNEL_FLAG_OWNER;
				data << uint8(CHANNEL_NOTIFY_FLAG_MODE_CHG) << m_name << uint64(itr->first->Guid) << uint8(oldflags2) << uint8(itr->second);				
				SendToAll(&data);
			}
			else
			{
				if(pOwner == NULL)
				{
					pOwner = itr->first;
					oldflags = itr->second;
					itr->second |= CHANNEL_FLAG_OWNER;
				}
			}				
		}
	}
	else
	{
		for(MemberMap::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
		{
			if(itr->second & CHANNEL_FLAG_OWNER)
			{
				// remove the old owner
				oldflags2 = itr->second;
				itr->second &= ~CHANNEL_FLAG_OWNER;
				data << uint8(CHANNEL_NOTIFY_FLAG_MODE_CHG) << m_name << uint64(itr->first->Guid) << uint8(oldflags2) << uint8(itr->second);	
				SendToAll(&data);
			}
			else
			{
				if(plr == itr->first)
				{
					pOwner = itr->first;
					oldflags = itr->second;
					itr->second |= CHANNEL_FLAG_OWNER;
				}
			}				
		}
	}

	if(pOwner == NULL)
		return;		// obviously no members

	data.clear();
	data << uint8(CHANNEL_NOTIFY_FLAG_CHGOWNER) << m_name << uint64(pOwner->Guid);
	SendToAll(&data);

	// send the mode changes
	data.clear();
	data << uint8(CHANNEL_NOTIFY_FLAG_MODE_CHG) << m_name << uint64(pOwner->Guid) << uint8(oldflags) << uint8(oldflags | CHANNEL_FLAG_OWNER);
	SendToAll(&data);
}

void Channel::Invite(RPlayerInfo* plr, RPlayerInfo* new_player)
{
	Guard mGuard(m_lock);
	if(m_members.find(plr) == m_members.end())
	{
		SendNotOn(plr);
		return;
	}

	if(m_members.find(new_player) != m_members.end())
	{
		SendAlreadyOn(plr, new_player);
		return;
	}

	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	data << uint8(CHANNEL_NOTIFY_FLAG_INVITED) << m_name << uint64(plr->Guid);
	new_player->GetSession()->SendPacket(&data);

	data.clear();
	data << uint8(CHANNEL_NOTIFY_FLAG_YOU_INVITED) << m_name << uint64(new_player->Guid);
	plr->GetSession()->SendPacket(&data);
}

void Channel::Moderate(RPlayerInfo* plr)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(m_members.end() == itr)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(itr->second & CHANNEL_FLAG_OWNER || itr->second & CHANNEL_FLAG_MODERATOR) && !plr->GetSession()->CanUseCommand("c"))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	m_muted = !m_muted;
	data << uint8(m_muted ? CHANNEL_NOTIFY_FLAG_MODERATED : CHANNEL_NOTIFY_FLAG_UNMODERATED) << m_name << uint64(plr->Guid);
	SendToAll(&data);
}

void Channel::Say(RPlayerInfo* plr, const char * message, RPlayerInfo* for_gm_client, bool forced)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, strlen(message)+100);
	if(!forced)
	{
		if(m_members.end() == itr)
		{
			data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
			plr->GetSession()->SendPacket(&data);
			return;
		}

		if(itr->second & CHANNEL_FLAG_MUTED)
		{
			data << uint8(CHANNEL_NOTIFY_FLAG_YOUCANTSPEAK) << m_name;
			plr->GetSession()->SendPacket(&data);
			return;
		}

		if(m_muted && !(itr->second & CHANNEL_FLAG_VOICED) && !(itr->second & CHANNEL_FLAG_MODERATOR) && !(itr->second & CHANNEL_FLAG_OWNER))
		{
			data << uint8(CHANNEL_NOTIFY_FLAG_YOUCANTSPEAK) << m_name;
			plr->GetSession()->SendPacket(&data);
			return;	
		}
	}

	// not blizzlike but meh
	if( plr->Level < m_minimumLevel )
	{
		sChatHandler.SystemMessage(plr->GetSession(), "You must be level %u to speak in the channel, '%s'.", m_minimumLevel, m_name.c_str());
		return;
	}

	data.SetOpcode(SMSG_MESSAGECHAT);
	data << uint8(CHAT_MSG_CHANNEL);
	data << uint32(0);		// language
	data << uint64(plr->Guid);	// guid
	data << uint32(0);		// rank?
	data << m_name;			// channel name
	data << uint64(plr->Guid);	// guid again?
	data << uint32(strlen(message)+1);
	data << message;
	data << (uint8)(plr->GMPermissions.size() ? 4 : 0);
	if(for_gm_client != NULL)
		for_gm_client->GetSession()->SendPacket(&data);
	else
		SendToAll(&data);
}

void Channel::SendNotOn(RPlayerInfo* plr)
{
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
	plr->GetSession()->SendPacket(&data);
}

void Channel::SendAlreadyOn(RPlayerInfo* plr, RPlayerInfo* plr2)
{
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	data << uint8(CHANNEL_NOTIFY_FLAG_ALREADY_ON) << m_name << uint64(plr2->Guid);
	plr->GetSession()->SendPacket(&data);
}

void Channel::Kick(RPlayerInfo* plr, RPlayerInfo* die_player, bool ban)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(die_player);
	MemberMap::iterator me_itr = m_members.find(plr);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	uint32 flags;

	if(me_itr == m_members.end())
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(itr == m_members.end())
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOT_ON_2) << m_name << uint64(die_player->Guid);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(me_itr->second & CHANNEL_FLAG_OWNER || me_itr->second & CHANNEL_FLAG_MODERATOR))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	flags = itr->second;
	data << uint8(CHANNEL_NOTIFY_FLAG_KICKED) << m_name << uint64(die_player->Guid);
	SendToAll(&data);

	if(ban)
	{
		data.clear();
		data << uint8(CHANNEL_NOTIFY_FLAG_BANNED) << m_name << uint64(die_player->Guid);
		SendToAll(&data);
	}

	m_members.erase(itr);

	if(flags & CHANNEL_FLAG_OWNER)
		SetOwner(NULL, NULL);

	if(ban)
		m_bannedMembers.insert(die_player->Guid);

	data.clear();
	data << uint8(CHANNEL_NOTIFY_FLAG_YOULEFT) << m_name << m_typeId << uint32(0) << uint8(0);
	die_player->GetSession()->SendPacket(&data);
}

void Channel::Unban(RPlayerInfo* plr, RPlayerInfo * bplr)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(m_members.end() == itr)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(itr->second & CHANNEL_FLAG_OWNER || itr->second & CHANNEL_FLAG_MODERATOR))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	set<uint32>::iterator it2 = m_bannedMembers.find(bplr->Guid);
	if(it2 == m_bannedMembers.end())
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOT_ON_2) << m_name << uint64(bplr->Guid);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	data << uint8(CHANNEL_NOTIFY_FLAG_UNBANNED) << m_name << uint64(bplr->Guid);
	SendToAll(&data);
	m_bannedMembers.erase(it2);
}

void Channel::Voice(RPlayerInfo* plr, RPlayerInfo* v_player)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	MemberMap::iterator itr2 = m_members.find(v_player);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(m_members.end() == itr)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(m_members.end() == itr2)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOT_ON_2) << m_name << uint64(v_player->Guid);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(itr->second & CHANNEL_FLAG_OWNER || itr->second & CHANNEL_FLAG_MODERATOR))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	uint32 oldflags = itr2->second;
	itr2->second |= CHANNEL_FLAG_VOICED;
	data << uint8(CHANNEL_NOTIFY_FLAG_MODE_CHG) << m_name << uint64(v_player->Guid) << uint8(oldflags) << uint8(itr2->second);
	SendToAll(&data);
}

void Channel::Devoice(RPlayerInfo* plr, RPlayerInfo* v_player)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	MemberMap::iterator itr2 = m_members.find(v_player);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(m_members.end() == itr)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(m_members.end() == itr2)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOT_ON_2) << m_name << uint64(v_player->Guid);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(itr->second & CHANNEL_FLAG_OWNER || itr->second & CHANNEL_FLAG_MODERATOR))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	uint32 oldflags = itr2->second;
	itr2->second &= ~CHANNEL_FLAG_VOICED;
	data << uint8(CHANNEL_NOTIFY_FLAG_MODE_CHG) << m_name << uint64(v_player->Guid) << uint8(oldflags) << uint8(itr2->second);
	SendToAll(&data);
}

void Channel::Mute(RPlayerInfo* plr, RPlayerInfo* die_player)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	MemberMap::iterator itr2 = m_members.find(die_player);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(m_members.end() == itr)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(m_members.end() == itr2)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOT_ON_2) << m_name << uint64(die_player->Guid);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(itr->second & CHANNEL_FLAG_OWNER || itr->second & CHANNEL_FLAG_MODERATOR))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	uint32 oldflags = itr2->second;
	itr2->second |= CHANNEL_FLAG_MUTED;
	data << uint8(CHANNEL_NOTIFY_FLAG_MODE_CHG) << m_name << uint64(die_player->Guid) << uint8(oldflags) << uint8(itr2->second);
	SendToAll(&data);
}

void Channel::Unmute(RPlayerInfo* plr, RPlayerInfo* die_player)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	MemberMap::iterator itr2 = m_members.find(die_player);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(m_members.end() == itr)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(m_members.end() == itr2)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOT_ON_2) << m_name << uint64(die_player->Guid);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(itr->second & CHANNEL_FLAG_OWNER || itr->second & CHANNEL_FLAG_MODERATOR))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	uint32 oldflags = itr2->second;
	itr2->second &= ~CHANNEL_FLAG_MUTED;
	data << uint8(CHANNEL_NOTIFY_FLAG_MODE_CHG) << m_name << uint64(die_player->Guid) << uint8(oldflags) << uint8(itr2->second);
	SendToAll(&data);
}

void Channel::GiveModerator(RPlayerInfo* plr, RPlayerInfo* new_player)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	MemberMap::iterator itr2 = m_members.find(new_player);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(m_members.end() == itr)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(m_members.end() == itr2)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOT_ON_2) << m_name << uint64(new_player->Guid);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(itr->second & CHANNEL_FLAG_OWNER || itr->second & CHANNEL_FLAG_MODERATOR))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	uint32 oldflags = itr2->second;
	itr2->second |= CHANNEL_FLAG_MODERATOR;
	data << uint8(CHANNEL_NOTIFY_FLAG_MODE_CHG) << m_name << uint64(new_player->Guid) << uint8(oldflags) << uint8(itr2->second);
	SendToAll(&data);
}

void Channel::TakeModerator(RPlayerInfo* plr, RPlayerInfo* new_player)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	MemberMap::iterator itr2 = m_members.find(new_player);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(m_members.end() == itr)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(m_members.end() == itr2)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOT_ON_2) << m_name << uint64(new_player->Guid);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(itr->second & CHANNEL_FLAG_OWNER || itr->second & CHANNEL_FLAG_MODERATOR))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	uint32 oldflags = itr2->second;
	itr2->second &= ~CHANNEL_FLAG_MODERATOR;
	data << uint8(CHANNEL_NOTIFY_FLAG_MODE_CHG) << m_name << uint64(new_player->Guid) << uint8(oldflags) << uint8(itr2->second);
	SendToAll(&data);
}

void Channel::Announce(RPlayerInfo* plr)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(m_members.end() == itr)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(itr->second & CHANNEL_FLAG_OWNER || itr->second & CHANNEL_FLAG_MODERATOR))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	m_announce = !m_announce;
	data << uint8(m_announce ? CHANNEL_NOTIFY_FLAG_ENABLE_ANN : CHANNEL_NOTIFY_FLAG_DISABLE_ANN) << m_name << uint64(plr->Guid);
	SendToAll(&data);
}

void Channel::Password(RPlayerInfo* plr, const char * pass)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(m_members.end() == itr)
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	if(!(itr->second & CHANNEL_FLAG_OWNER || itr->second & CHANNEL_FLAG_MODERATOR))
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTMOD) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	m_password = string(pass);
	data << uint8(CHANNEL_NOTIFY_FLAG_SETPASS) << m_name << uint64(plr->Guid);
	SendToAll(&data);	
}

void Channel::List(RPlayerInfo* plr)
{
	Guard mGuard(m_lock);
	WorldPacket data(SMSG_CHANNEL_LIST, 50 + (m_members.size()*9));
	MemberMap::iterator itr = m_members.find(plr);
	if(itr == m_members.end())
	{
		data.Initialize(SMSG_CHANNEL_NOTIFY);
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	uint8 flags;
	data << uint8(1) << m_name;
	data << uint8(m_flags);
	data << uint32(m_members.size());
	for(MemberMap::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
	{
		data << uint64(itr->first->Guid);
		flags = 0;
		if(!(itr->second & CHANNEL_FLAG_MUTED))
			flags |= 0x04;		// voice flag

		if(itr->second & CHANNEL_FLAG_OWNER)
			flags |= 0x01;		// owner flag

		if(itr->second & CHANNEL_FLAG_MODERATOR)
			flags |= 0x02;		// moderator flag

		if(!m_general)
			flags |= 0x10;

		data << flags;
	}

	plr->GetSession()->SendPacket(&data);
}

void Channel::GetOwner(RPlayerInfo* plr)
{
	Guard mGuard(m_lock);
	MemberMap::iterator itr = m_members.find(plr);
	WorldPacket data(SMSG_CHANNEL_NOTIFY, 100);
	if(itr == m_members.end())
	{
		data << uint8(CHANNEL_NOTIFY_FLAG_NOTON) << m_name;
		plr->GetSession()->SendPacket(&data);
		return;
	}

	for(itr = m_members.begin(); itr != m_members.end(); ++itr)
	{
		if(itr->second & CHANNEL_FLAG_OWNER)
		{
			data << uint8(CHANNEL_NOTIFY_FLAG_WHO_OWNER) << m_name << uint64(itr->first->Guid);
			plr->GetSession()->SendPacket(&data);
			return;
		}
	}
}

Channel::~Channel()
{
	WorldPacket data(ISMSG_CHANNEL_ACTION, 9);

	m_lock.Acquire();
	for(MemberMap::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
	{
		data << uint8(CHANNEL_PART); //joined channel
		data << uint32(itr->first->Guid);
		data << uint32(m_channelId);
		if(itr->first->GetSession() && itr->first->GetSession()->GetServer())
		itr->first->GetSession()->GetServer()->SendPacket(&data);
		data.clear();
	}

	m_lock.Release();
	m_deleted = true;
}

void Channel::SendToAll(WorldPacket * data)
{
	Guard guard(m_lock);
	for(MemberMap::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
	{
		if( itr->first->GetSession() )
			itr->first->GetSession()->SendPacket(data);
	}
}

void Channel::SendToAll(WorldPacket * data, RPlayerInfo* plr)
{
	Guard guard(m_lock);
	for(MemberMap::iterator itr = m_members.begin(); itr != m_members.end(); ++itr) 
	{
		if ( itr->first != plr && itr->first->GetSession() )
			itr->first->GetSession()->SendPacket(data);
	}
}

Channel * ChannelMgr::GetCreateChannel(const char *name, RPlayerInfo* p, uint32 type_id)
{
	ChannelList::iterator itr;
	ChannelList * cl = &Channels[0];
	Channel * chn;
	uint32 cid;
	if( seperatechannels && p != NULL )
		cl = &Channels[p->Team];

	lock.Acquire();
	for(itr = cl->begin(); itr != cl->end(); ++itr)
	{
		if(!stricmp(name, itr->first.c_str()))
		{
			lock.Release();
			return itr->second;
		}
	}

	// make sure the name isn't banned
	m_confSettingLock.Acquire();
	for(vector<string>::iterator itr = m_bannedChannels.begin(); itr != m_bannedChannels.end(); ++itr)
	{
		if(!strnicmp( name, itr->c_str(), itr->size() ) )
		{
			lock.Release();
			m_confSettingLock.Release();
			return NULL;
		}
	}
	m_confSettingLock.Release();

	cid = ++m_idHigh;
	chn = new Channel(name, ( seperatechannels && p != NULL ) ? p->Team : 0, type_id, cid);
	cl->insert(make_pair(chn->m_name, chn));
	m_idToChannel.insert(make_pair(cid, chn));
	lock.Release();
	return chn;
}

Channel * ChannelMgr::GetChannel(const char *name, RPlayerInfo* p)
{
	ChannelList::iterator itr;
	ChannelList * cl = &Channels[0];
	if(seperatechannels)
		cl = &Channels[p->Team];

	lock.Acquire();
	for(itr = cl->begin(); itr != cl->end(); ++itr)
	{
		if(!stricmp(name, itr->first.c_str()))
		{
			lock.Release();
			return itr->second;
		}
	}

	lock.Release();
	return NULL;
}

Channel * ChannelMgr::GetChannel(uint32 id)
{
	ChannelMap::iterator itr;
	Channel *ret = NULL;

	lock.Acquire();
	itr = m_idToChannel.find(id);
	if( itr != m_idToChannel.end() )
		ret = itr->second;

	lock.Release();
	return ret;
}

Channel * ChannelMgr::GetChannel(const char *name, uint32 team)
{
	ChannelList::iterator itr;
	ChannelList * cl = &Channels[0];
	if(seperatechannels)
		cl = &Channels[team];

	lock.Acquire();
	for(itr = cl->begin(); itr != cl->end(); ++itr)
	{
		if(!stricmp(name, itr->first.c_str()))
		{
			lock.Release();
			return itr->second;
		}
	}

	lock.Release();
	return NULL;
}

void ChannelMgr::RemoveChannel(Channel * chn)
{
	ChannelList::iterator itr;
	ChannelList * cl = &Channels[0];
	if(seperatechannels)
		cl = &Channels[chn->m_team];

	lock.Acquire();
	m_idToChannel.erase(chn->m_channelId);
	for(itr = cl->begin(); itr != cl->end(); ++itr)
	{
		if(itr->second == chn)
		{
			cl->erase(itr);
			chn->m_lock.Release();
			delete chn;
			lock.Release();
			return;
		}
	}

	lock.Release();
}

ChannelMgr::ChannelMgr()
{
	m_idHigh = 0;
}

ChannelMgr::~ChannelMgr()
{
	for(int i = 0; i < 2; ++i)
	{
		ChannelList::iterator itr = this->Channels[i].begin();
		for(; itr != this->Channels[i].end(); ++itr)
		{
			delete itr->second;
		}
		Channels[i].clear();
	}
}

