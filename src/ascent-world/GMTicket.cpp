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

#include "StdAfx.h"

enum GMticketType
{
	GM_TICKET_TYPE_STUCK				= 1,
	GM_TICKET_TYPE_BEHAVIOR_HARASSMENT	= 2,
	GM_TICKET_TYPE_GUILD				= 3,
	GM_TICKET_TYPE_ITEM					= 4,
	GM_TICKET_TYPE_ENVIRONMENTAL		= 5,
	GM_TICKET_TYPE_NON_QUEST_CREEP		= 6,
	GM_TICKET_TYPE_QUEST_QUEST_NPC		= 7,
	GM_TICKET_TYPE_TECHNICAL			= 8,
	GM_TICKET_TYPE_ACCOUNT_BILLING		= 9,
	GM_TICKET_TYPE_CHARACTER			= 10,
};

void WorldSession::HandleGMTicketCreateOpcode(WorldPacket & recv_data)
{
	uint32 type;
	float x, y, z;
	std::string message = "";
	std::string message2 = "";
	GM_Ticket *ticket = new GM_Ticket;
	WorldPacket data(SMSG_GMTICKET_CREATE, 4);

	// recv Data
	recv_data >> type;
	recv_data >> x;
	recv_data >> y;
	recv_data >> z;
	recv_data >> message;
	recv_data >> message2;

	/*if (type == GM_TICKET_TYPE_BEHAVIOR_HARASSMENT)
	{
		// more magic
		uint32 unk1;
		uint32 unk2;
		uint32 unk3;
		string unk4;
		recv_data >> unk1; // count of something
		for (int i = 0; i < unk1; i++)
		{
			recv_data >> unk2
			// do something with unk2
		}
		recv_data >> unk3;
		if (unk3)
			recv_data >> unk4;
	}*/

	// Create new Ticket and store it
	objmgr.remGMTicket(GetPlayer()->GetGUID());

	ticket->guid = GetPlayer()->GetGUID();
	ticket->type = type;
	ticket->posX = x;
	ticket->posY = y;
	ticket->posZ = z;
	ticket->message = message;
	ticket->timestamp = (uint32)UNIXTIME;
	ticket->name = GetPlayer()->GetName();
	ticket->level = GetPlayer()->getLevel();

	objmgr.remGMTicket(GetPlayer()->GetGUID());
	objmgr.AddGMTicket(ticket,false);

	// Response - no errors
	data << uint32(2);

	SendPacket(&data);

#ifdef CLUSTERING 
	std::stringstream str;
	str << "GmTicket 5," << GetPlayer()->GetName();
	string strmessage = str.str();
	WorldPacket data2(ICMSG_CHANNEL_ACTION, 1 + sWorld.getGmClientChannel().size() + 4 + strmessage.size() + 4 + 1);
	data2 << uint8(CHANNEL_SAY);
	data2 << sWorld.getGmClientChannel();
	data2 << GetPlayer()->GetLowGUID();
	data2 << strmessage;
	data2 << uint32(NULL);
	data2 << bool(true);
	sClusterInterface.SendPacket(&data2);
#else

	// send message indicating new ticket
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(),GetPlayer());
	if(chn)
	{
		char msg[100];
		snprintf(msg, 100, "GmTicket 5,%s", GetPlayer()->GetName());
		chn->Say(_player, msg, NULL, true);
	}
#endif
}

void WorldSession::HandleGMTicketUpdateOpcode(WorldPacket & recv_data)
{
	std::string message = "";
	WorldPacket data(SMSG_GMTICKET_UPDATETEXT, 4);

	// recv Data
	recv_data >> message;

	// Update Ticket
	GM_Ticket *ticket = objmgr.GetGMTicket(GetPlayer()->GetGUID());
	if(!ticket) // Player doesnt have a GM Ticket yet
	{
		// Response - error couldnt find existing Ticket
		data << uint32(1);

		SendPacket(&data);
		return;
	}
	ticket->message = message;
	ticket->timestamp = (uint32)UNIXTIME;

	objmgr.remGMTicket(GetPlayer()->GetGUID());
	objmgr.AddGMTicket(ticket,false);

	// Response - no errors
	data << uint32(2);

	SendPacket(&data);
}

void WorldSession::HandleGMTicketDeleteOpcode(WorldPacket & recv_data)
{
	WorldPacket data(SMSG_GMTICKET_DELETETICKET, 4);
	// no data

	// remove Ticket
	objmgr.remGMTicket(GetPlayer()->GetGUID());

	// Response - no errors
	data << uint32(9);

	SendPacket(&data);
}

void WorldSession::HandleGMTicketGetTicketOpcode(WorldPacket & recv_data)
{
	WorldPacket data(SMSG_GMTICKET_GETTICKET, 400);
	// no data

	// get Current Ticket
	GM_Ticket *ticket = objmgr.GetGMTicket(GetPlayer()->GetGUID());

	if(!ticket) // no Current Ticket
	{
		data << uint32(10);
		SendPacket(&data);
		return;
	}

	// Send current Ticket
	data << uint32(6); // unk
	data << ticket->message.c_str();
	data << (uint8)ticket->type;

	SendPacket(&data);
}


void WorldSession::HandleGMTicketSystemStatusOpcode(WorldPacket & recv_data)
{
	WorldPacket data(SMSG_GMTICKET_SYSTEMSTATUS, 4);

	// no data

	// Response - System is working Fine
	if(sWorld.getGMTicketStatus())
		data << uint32(1);
	else
		data << uint32(0);

	SendPacket(&data);
}

void WorldSession::HandleGMTicketToggleSystemStatusOpcode(WorldPacket & recv_data)
{
	if(!HasGMPermissions())
		return;

	sWorld.toggleGMTicketStatus();
}
