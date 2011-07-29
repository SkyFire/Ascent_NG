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

#ifndef __CHANNELMGR_H
#define __CHANNELMGR_H

class Channel;

class ChannelMgr :  public Singleton < ChannelMgr >
{

public:
	ChannelMgr();
	~ChannelMgr();
	
	Channel *GetCreateChannel(const char *name, RPlayerInfo* p, uint32 type_id);
	Channel *GetChannel(const char *name, RPlayerInfo* p);
	Channel * GetChannel(const char * name, uint32 team);
	Channel *GetChannel(uint32 id);
#ifdef VOICE_CHAT
	void VoiceDied();
#endif
	void RemoveChannel(Channel * chn);
	bool seperatechannels;

private:
	//team 0: alliance, team 1 horde
	typedef map<string,Channel *> ChannelList;
	typedef HM_NAMESPACE::hash_map<uint32, Channel*> ChannelMap;
	ChannelMap m_idToChannel;
	uint32 m_idHigh;
	ChannelList Channels[2];
	Mutex lock;
};

#define channelmgr ChannelMgr::getSingleton()

#endif