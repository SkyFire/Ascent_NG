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

#ifndef REALMSERVER_CHAT_H
#define REALMSERVER_CHAT_H

//#include "SkillNameMgr.h"

class ChatHandler;
class Session;

enum ChatMsg
{
	CHAT_MSG_ADDON									= -1,
	CHAT_MSG_SYSTEM									= 0,
	CHAT_MSG_SAY									= 1,
	CHAT_MSG_PARTY									= 2,
	CHAT_MSG_RAID									= 3,
	CHAT_MSG_GUILD									= 4,
	CHAT_MSG_OFFICER								= 5,
	CHAT_MSG_YELL									= 6,
	CHAT_MSG_WHISPER								= 7,
	CHAT_MSG_WHISPER_MOB							= 8,
	CHAT_MSG_WHISPER_INFORM							= 9,
	CHAT_MSG_EMOTE									= 10,
	CHAT_MSG_TEXT_EMOTE								= 11,
	CHAT_MSG_MONSTER_SAY							= 12,
	CHAT_MSG_MONSTER_PARTY							= 13,
	CHAT_MSG_MONSTER_YELL							= 14,
	CHAT_MSG_MONSTER_WHISPER						= 15,
	CHAT_MSG_MONSTER_EMOTE							= 16,
	CHAT_MSG_CHANNEL								= 17,
	CHAT_MSG_CHANNEL_JOIN							= 18,
	CHAT_MSG_CHANNEL_LEAVE							= 19,
	CHAT_MSG_CHANNEL_LIST							= 20,
	CHAT_MSG_CHANNEL_NOTICE							= 21,
	CHAT_MSG_CHANNEL_NOTICE_USER					= 22,
	CHAT_MSG_AFK									= 23,
	CHAT_MSG_DND									= 24,
	CHAT_MSG_IGNORED								= 25,
	CHAT_MSG_SKILL									= 26,
	CHAT_MSG_LOOT									= 27,
	CHAT_MSG_MONEY									= 28,
	CHAT_MSG_OPENING								= 29,
	CHAT_MSG_TRADESKILLS							= 30,
	CHAT_MSG_PET_INFO								= 31,
	CHAT_MSG_COMBAT_MISC_INFO						= 32,
	CHAT_MSG_XP_GAIN								= 33,
	CHAT_MSG_HONOR_GAIN								= 34,
	CHAT_MSG_COMBAT_FACTION_CHANGE					= 35,
	CHAT_MSG_BG_SYSTEM_NEUTRAL						= 36,
	CHAT_MSG_BG_SYSTEM_ALLIANCE						= 37,
	CHAT_MSG_BG_SYSTEM_HORDE						= 38,
	CHAT_MSG_RAID_LEADER							= 39,
	CHAT_MSG_RAID_WARNING							= 40,
	CHAT_MSG_RAID_BOSS_EMOTE						= 41,
	CHAT_MSG_RAID_BOSS_WHISPER						= 42,
	CHAT_MSG_FILTERED								= 43,
	CHAT_MSG_BATTLEGROUND							= 44,
	CHAT_MSG_BATTLEGROUND_LEADER					= 45,
	CHAT_MSG_RESTRICTED								= 46,
	CHAT_MSG_ACHIEVEMENT							= 47,
	CHAT_MSG_GUILD_ACHIEVEMENT						= 48,
};

enum Languages
{
	LANG_ADDON									= -1,
	LANG_UNIVERSAL                              = 0x00,
	LANG_ORCISH                                 = 0x01,
	LANG_DARNASSIAN                             = 0x02,
	LANG_TAURAHE                                = 0x03,
	LANG_DWARVISH                               = 0x06,
	LANG_COMMON                                 = 0x07,
	LANG_DEMONIC                                = 0x08,
	LANG_TITAN                                  = 0x09,
	LANG_THELASSIAN                             = 0x0A,
	LANG_DRACONIC                               = 0x0B,
	LANG_KALIMAG                                = 0x0C,
	LANG_GNOMISH                                = 0x0D,
	LANG_TROLL                                  = 0x0E,
	LANG_GUTTERSPEAK                            = 0x21,
	LANG_DRAENEI                                = 0x23,
	LANG_ZOMBIE									= 0x24,
	LANG_GNOMISHBINARY							= 0x25,
	LANG_GOBLINBINARY							= 0x26,
	NUM_LANGUAGES                               = 0x27
};

#define MSG_COLOR_LIGHTRED	  "|cffff6060"
#define MSG_COLOR_LIGHTBLUE	 "|cff00ccff"
#define MSG_COLOR_BLUE		  "|cff0000ff"
#define MSG_COLOR_GREEN		 "|cff00ff00"
#define MSG_COLOR_RED		   "|cffff0000"
#define MSG_COLOR_GOLD		  "|cffffcc00"
#define MSG_COLOR_GREY		  "|cff888888"
#define MSG_COLOR_WHITE		 "|cffffffff"
#define MSG_COLOR_SUBWHITE	  "|cffbbbbbb"
#define MSG_COLOR_MAGENTA	   "|cffff00ff"
#define MSG_COLOR_YELLOW		"|cffffff00"
#define MSG_COLOR_CYAN		  "|cff00ffff"

#define CHECKSESSION if(m_session == NULL) return NULL;

class SERVER_DECL ChatHandler : public Singleton<ChatHandler>
{
public:
	ChatHandler();
	~ChatHandler();

	WorldPacket * FillMessageData( uint32 type, int32 language,  const char* message,uint64 guid, uint8 flag = 0) const;
	WorldPacket * FillSystemMessageData( const char* message ) const;

	int ParseCommands(const char* text, Session *session);

	void SystemMessage(Session *m_session, const char *message, ...);
	void ColorSystemMessage(Session *m_session, const char *colorcode, const char *message, ...);
	void RedSystemMessage(Session *m_session, const char *message, ...);
	void GreenSystemMessage(Session *m_session, const char *message, ...);
	void BlueSystemMessage(Session *m_session, const char *message, ...);
	void RedSystemMessageToPlr(RPlayerInfo* plr, const char *message, ...);
	void GreenSystemMessageToPlr(RPlayerInfo* plr, const char *message, ...);
	void BlueSystemMessageToPlr(RPlayerInfo* plr, const char *message, ...);
	void SystemMessageToPlr(RPlayerInfo* plr, const char *message, ...);

protected:

	void SendMultilineMessage(Session *m_session, const char *str);
};


#define sChatHandler ChatHandler::getSingleton()
#endif