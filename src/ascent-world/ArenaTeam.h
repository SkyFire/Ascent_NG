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

#ifndef ASCENT_ARENATEAMS_H
#define ASCENT_ARENATEAMS_H

struct ArenaTeamMember
{
	PlayerInfo * Info;
	uint32 Played_ThisWeek;
	uint32 Won_ThisWeek;
	uint32 Played_ThisSeason;
	uint32 Won_ThisSeason;
	uint32 PersonalRating;
};

class SERVER_DECL ArenaTeam
{
	void AllocateSlots(uint32 Type)
	{
		uint32 Slots = 0;
		if(Type == ARENA_TEAM_TYPE_2V2)
			Slots=4;
		else if(Type == ARENA_TEAM_TYPE_3V3)
			Slots=6;
		else if(Type == ARENA_TEAM_TYPE_5V5)
			Slots=10;
		ASSERT(Slots);
		m_members = new ArenaTeamMember[Slots];
		memset(m_members,0,sizeof(ArenaTeamMember)*Slots);
		m_slots = Slots;
		m_memberCount=0;
	}

public:

	uint32 m_id;
	uint32 m_type;
	uint32 m_leader;
	uint32 m_slots;
	string m_name;
	uint32 m_memberCount;
	ArenaTeamMember * m_members;

	uint32 m_emblemStyle;
	uint32 m_emblemColour;
	uint32 m_borderStyle;
	uint32 m_borderColour;
	uint32 m_backgroundColour;

	uint32 m_stat_rating;
	uint32 m_stat_gamesplayedweek;
	uint32 m_stat_gameswonweek;
	uint32 m_stat_gamesplayedseason;
	uint32 m_stat_gameswonseason;
	uint32 m_stat_ranking;

	ArenaTeam(uint32 Type, uint32 Id);
	ArenaTeam(Field * f);
	~ArenaTeam()
	{
		delete [] m_members;
	}

	void SendPacket(WorldPacket * data);
	void Query(WorldPacket & data);
	void Stat(WorldPacket & data);
	void Roster(WorldPacket & data);
	void Inspect(WorldPacket & data);
	void Destroy();
	void SaveToDB();

	bool AddMember(PlayerInfo * info);
	bool RemoveMember(PlayerInfo * info);
	bool HasMember(uint32 guid);
	void SetLeader(PlayerInfo * info);
	ArenaTeamMember * GetMember(PlayerInfo * info);
	ArenaTeamMember * GetMemberByGuid(uint32 guid);

	uint32 GetPlayersPerTeam()
	{
		switch(m_type)
		{
		case ARENA_TEAM_TYPE_2V2:
			return 2;

		case ARENA_TEAM_TYPE_3V3:
			return 3;

		case ARENA_TEAM_TYPE_5V5:
			return 5;
		}

		// never reached
		return 2;
	}
};

#endif		// ASCENT_ARENATEAMS_H
