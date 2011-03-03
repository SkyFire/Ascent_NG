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

static const uint32 LanguageSkills[NUM_LANGUAGES] = {
	0,				// UNIVERSAL		0x00
	109,			// ORCISH			0x01
	113,			// DARNASSIAN		0x02
	115,			// TAURAHE			0x03
	0,				// -				0x04
	0,				// -				0x05
	111,			// DWARVISH			0x06
	98,				// COMMON			0x07
	139,			// DEMON TONGUE		0x08
	140,			// TITAN			0x09
	137,			// THALSSIAN		0x0A
	138,			// DRACONIC			0x0B
	0,				// KALIMAG			0x0C
	313,			// GNOMISH			0x0D
	315,			// TROLL			0x0E
	0,				// -				0x0F
	0,				// -				0x10
	0,				// -				0x11
	0,				// -				0x12
	0,				// -				0x13
	0,				// -				0x14
	0,				// -				0x15
	0,				// -				0x16
	0,				// -				0x17
	0,				// -				0x18
	0,				// -				0x19
	0,				// -				0x1A
	0,				// -				0x1B
	0,				// -				0x1C
	0,				// -				0x1D
	0,				// -				0x1E
	0,				// -				0x1F
	0,				// -				0x20
	673,			// -				0x21
	0,				// -				0x22
	759,			// -				0x23
};

void Session::HandleMessagechatOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 9);
	WorldPacket *data;
	//CHECK_INWORLD_RETURN;

	uint32 type;
	int32 lang;

	recv_data >> type;
	// were only going to handle these types here, otherwise send it to the world server for processing
	if( type != CHAT_MSG_CHANNEL && type != CHAT_MSG_WHISPER )
	{
		GetServer()->SendWoWPacket(this, &recv_data);
	}
	recv_data >> lang;

	if( lang >= NUM_LANGUAGES )
		return;

	/*
	if(GetPlayer()->IsBanned())
		{
			SystemMessage("You cannot do that when banned.");
			return;
		}*/
	

	string msg, misc;

	// special misc
	if( type == CHAT_MSG_CHANNEL || type == CHAT_MSG_WHISPER )
	{
		recv_data >> misc;
		recv_data >> msg;
	}
	else
		recv_data >> msg;

	/*
	if(!sHookInterface.OnChat(TO_PLAYER(_player), type, lang, msg, misc))
			return;*/
	

	// Idiots spamming giant pictures through the chat system
	if( msg.find("|TInterface") != string::npos || msg.find("\n") != string::npos)
	{
		sChatHandler.SystemMessage(this, "Your message has been blocked.");
		return;
	}

	if(  msg.find("|c") != string::npos && msg.find("|H") == string::npos )
	{
		return;
	}
/*

	//arghhh STFU. I'm not giving you gold or items NOOB
	switch(type)
	{
	case CHAT_MSG_WHISPER:
	case CHAT_MSG_CHANNEL:
		{
			if( m_muted && m_muted >= (uint32)UNIXTIME )
			{
				SystemMessage("Your voice is currently muted by a moderator. This will expire in %s.", ConvertTimeStampToString(m_muted - (uint32)UNIXTIME).c_str());
				return;
			}
		}break;
	}

	if( lang != -1 && !GetPlayer->GMPermissions.size() )
	{
		if(sRMaster.flood_lines)
		{
			/ * flood detection, wheeee! * /
			if(UNIXTIME >= floodTime)
			{
				floodLines = 0;
				floodTime = UNIXTIME + sRMaster.flood_seconds;
			}

			if((++floodLines) > sRMaster.flood_lines)
			{
				if( sRMaster.flood_mute_after_flood )
				{
					SystemMessage("You have been muted for %u seconds for spamming.", sRMaster.flood_mute_after_flood );
					m_muted = (uint32)UNIXTIME + sRMaster.flood_mute_after_flood;
					return;
				}

				if(sRMaster.flood_message)
					SystemMessage("Your message has triggered serverside flood protection. You can speak again in %u seconds.", floodTime - UNIXTIME);

				return;
			}
		}

		if( sRMaster.flood_message_time )
		{
			if( !stricmp(m_repeatMessage.c_str(), msg.c_str()) )
			{
				if( ((uint32)UNIXTIME - m_repeatTime) < sRMaster.flood_message_time )
				{
					SystemMessage("Your message has triggered serverside flood protection. Please don't repeat yourself.");
					return;
				}
			}

			m_repeatMessage = msg;
			m_repeatTime = (uint32)UNIXTIME;
		}

		if( sRMaster.flood_caps_pct > 0.0f )
		{
			if( msg.length() >= sRMaster.flood_caps_min_len )
			{
				uint32 fc = 0;
				uint32 slen = uint32(msg.length());
				uint32 clen = 0;
				for(; fc < slen; ++fc)
				{
					if( msg[fc] >= 'A' && msg[fc] <= 'Z' )
						++clen;
				}

				float pct = (float(clen) / float(slen)) * 100.0f;
				if( pct >= sRMaster.flood_caps_pct )
				{
					SystemMessage("Message blocked. Please don't speak in caps. You are are allowed %.2f%% caps in a message of %u characters.", sWorld.flood_caps_pct, sWorld.flood_caps_min_len);
					return;
				}
			}
		}

		// filter
		if(g_chatFilter->Parse(msg) == true)
		{
			SystemMessage("Your chat message was blocked by a server-side filter.");
			return;
		}
	}
 */

	switch(type)
	{
	case CHAT_MSG_WHISPER:
		{
			RPlayerInfo* player = sClientMgr.GetRPlayer(misc.c_str());
			if(!player)
			{
				ASCENT_TOLOWER(misc);
				if( misc == "console" )
				{
					Log.Notice("Whisper","%s to %s: %s", GetPlayer()->Name, misc.c_str(), msg.c_str());
					return;
				}
				else
				{
					data = new WorldPacket(SMSG_CHAT_PLAYER_NOT_FOUND, misc.length() + 1);
					*data << misc;
					SendPacket(data);
					delete data;
				}
				break;
			}

			// Check that the player isn't a gm
			if(!GetPlayer()->GMPermissions.size() && player->GMPermissions.size())
			{
				// Build automated reply
				string Reply = "This Game Master does not currently have an open ticket from you and did not receive your whisper. Please submit a new GM Ticket request if you need to speak to a GM. This is an automatic message.";
				data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.c_str(), uint64(player->Guid), 3);
				SendPacket(data);
				delete data;
				break;
			}

			if(lang > 0 && LanguageSkills[lang] /* && _player->_HasSkillLine(LanguageSkills[lang]) == false*/)
				return;

			if(lang==0 && !CanUseCommand("c"))
				return;

			/*
			if( player->Social_IsIgnoring( GetPlayer->Guid  )
						{
							data = sChatHandler.FillMessageData( CHAT_MSG_IGNORED, LANG_UNIVERSAL,  msg.c_str(), uint64(GetPlayer()->Guid), GetPlayer->GMPermissions.size() ? 4 : 0 );
							SendPacket(data);
							delete data;
						}
						else*/
			
			{
				/*
				if(GetPlayer()->m_modlanguage >=0)
					data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, GetPlayer()->m_modlanguage,  msg.c_str(), uint64(GetPlayer()->Guid), GetPlayer->GMPermissions.size() ? 4 : 0 );
				else
				*/
					data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, ((CanUseCommand("c") || player->GetSession()->CanUseCommand("c")) && lang != -1) ? LANG_UNIVERSAL : lang,  msg.c_str(), uint64(GetPlayer()->Guid), GetPlayer()->GMPermissions.size() ? 4 : 0 );

				player->GetSession()->SendPacket(data);
				delete data;
			}

			//Sent the to Users id as the channel, this should be fine as it's not used for whisper

			data = sChatHandler.FillMessageData(CHAT_MSG_WHISPER_INFORM, LANG_UNIVERSAL,msg.c_str(), uint64(player->Guid), player->GMPermissions.size() ? 4 : 0  );
			SendPacket(data);
			delete data;

			/*
			if(player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_AFK))
			{
				// Has AFK flag, autorespond.
				data = sChatHandler.FillMessageData(CHAT_MSG_AFK, LANG_UNIVERSAL,  player->m_afk_reason.c_str(),uint64(player->Guid), GetPlayer->GMPermissions.size() ? 4 : 0);
				SendPacket(data);
				delete data;
			}
			else if(player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_DND))
			{
				// Has AFK flag, autorespond.
				data = sChatHandler.FillMessageData(CHAT_MSG_DND, LANG_UNIVERSAL, player->m_afk_reason.c_str(),uint64(player->Guid), GetPlayer->GMPermissions.size() ? 4 : 0);
				SendPacket(data);
				delete data;
			}
			*/
			

		} break;
	case CHAT_MSG_CHANNEL:
		{
			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			Channel *chn = channelmgr.GetChannel(misc.c_str(),GetPlayer()); 
			if(chn != NULL)
				chn->Say(GetPlayer(),msg.c_str(), NULL, false);
		} break;

	default:
		OUT_DEBUG("CHAT: unknown msg type %u, lang: %u", type, lang);
	}
}

/*
void Session::HandleReportSpamOpcode(WorldPacket & recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 29);

	// the 0 in the out packet is unknown
	GetPlayer()->GetSession()->OutPacket(SMSG_COMPLAIN_RESULT, 1, 0 );
}
*/
