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

#include "common.h"
#include "log.h"
#include "linkedlist.h"
#include "voice_channel.h"

// this is only 512KB of memory on a 32bit system, so its np
#define MAX_CHANNEL 65535
voice_channel * g_voiceChannels[MAX_CHANNEL];

int voice_channel_generate_id()
{
	int n;
	for( n = 1; n < MAX_CHANNEL; ++n )
	{
		if( g_voiceChannels[n] == NULL )
			return n;
	}

	return -1;
}

/*int voice_get_channel_count()
{
	int n;
	int count = 0;
	for( n = 1; n < MAX_CHANNEL; ++n )
	{
		if( g_voiceChannels[n] != NULL )
			++count;
	}

	return count;
}*/

void voice_channel_init()
{
	memset(g_voiceChannels, 0, sizeof(voice_channel*)*MAX_CHANNEL);
}

voice_channel * voice_channel_create(int channeltype, void* server_owner)
{
	int cid;
	int n;
	voice_channel * chn;
	static int channelslots[VOICE_CHANNEL_TYPE_COUNT] = { 250, 0, 40,     40 };
	//                                                    channel party   raid
	//             we use 40 slots for a party here because it can be expanded

	cid = voice_channel_generate_id();
	if( cid < 0 )
	{
		log_write(ERROR, "We are out of channel id's. Maybe you need to run more voice servers?");
		return NULL;
	}

	// apply
	chn = (voice_channel*)vc_malloc(sizeof(voice_channel));
	g_voiceChannels[cid] = chn;

	// initialize
	chn->member_slots = channelslots[channeltype] + 1;
	chn->member_count = 0;
	chn->channel_id = cid;
	chn->members = (voice_channel_member*)vc_malloc(sizeof(voice_channel_member) * chn->member_slots);
	chn->server_owner = server_owner;

	for( n = 0; n < chn->member_slots; ++n )
	{
		chn->members[n].used = 0;
		chn->members[n].active = 0;
	}

	log_write(DEBUG, "channel %u is being created for type %u", (int)cid, (int)channeltype);

	return chn;
}

int voice_channel_remove(int channelid)
{
	voice_channel * ch;
	if( channelid >= MAX_CHANNEL )
		return -1;

	if( (ch = g_voiceChannels[channelid]) == NULL )
		return -1;

	free(ch->members);
	free(ch);
	g_voiceChannels[channelid] = NULL;
	return 0;
}

voice_channel * voice_channel_get(int channelid)
{
	if( channelid >= MAX_CHANNEL )
		return NULL;

	return g_voiceChannels[channelid];
}

void voice_remove_channels(void* socket_ptr)
{
	int n;
	for( n = 0; n < MAX_CHANNEL; ++n )
	{
		if( g_voiceChannels[n] != NULL && g_voiceChannels[n]->server_owner == socket_ptr )
			voice_channel_remove(n);
	}
}