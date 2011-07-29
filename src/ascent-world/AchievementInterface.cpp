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

#include "StdAfx.h"

set<uint32> m_completedRealmFirstAchievements;

AchievementInterface::AchievementInterface(Player* plr)
{
	m_player = plr;
	m_achievementInspectPacket = NULL;
}

AchievementInterface::~AchievementInterface()
{
	m_player = NULL;

	if( m_achivementDataMap.size() > 0 )
	{
		std::map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
		for(; itr != m_achivementDataMap.end(); ++itr)
		{
			delete itr->second;
		}
	}

	if( m_achievementInspectPacket )
		delete m_achievementInspectPacket;
}

void AchievementInterface::LoadFromDB( QueryResult * pResult )
{
	if( !pResult ) // We have no achievements yet. :)
		return;

	do 
	{
		Field * fields = pResult->Fetch();
		uint32 achievementid = fields[1].GetUInt32();
		string criteriaprogress = fields[2].GetString();
		bool completed = (fields[3].GetUInt32() > 0);

		AchievementEntry * ae = dbcAchievement.LookupEntry( achievementid );
		AchievementData * ad = new AchievementData;
		memset(ad, 0, sizeof(AchievementData));

		ad->id = achievementid;
		ad->num_criterias = ae->AssociatedCriteriaCount;
		ad->completed = completed;
		ad->date = fields[3].GetUInt32();
		
		if( ad->completed && string(ae->name).find("Realm First!") != string::npos )
			m_completedRealmFirstAchievements.insert( ae->ID );

		vector<string> Delim = StrSplit( criteriaprogress, "," );
		for( uint32 i = 0; i < 32; ++i)
		{
			if( i >= Delim.size() )
				continue;

			string posValue = Delim[i];
			if( !posValue.size() )
				continue;

			uint32 r = atoi(posValue.c_str());
			ad->counter[i] = r;

			//printf("Loaded achievement: %u, %s\n", ae->ID, ad->completed ? "completed" : "incomplete" );
		}

		m_achivementDataMap.insert( make_pair( achievementid, ad) );
	} while ( pResult->NextRow() );
}

void AchievementInterface::SaveToDB(QueryBuffer * buffer)
{
	bool NewBuffer = false;
	if( !buffer )
	{
		buffer = new QueryBuffer;
		NewBuffer = true;
	}

	map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		AchievementData * ad = itr->second;
		if( !ad->m_isDirty )
			continue;

		std::stringstream ss;
		ss << "REPLACE INTO achievements (player,achievementid,progress,completed) VALUES (";
		ss << m_player->GetLowGUID() << ",";
		ss << ad->id << ",";
		ss << "'";
		for(uint32 i = 0; i < ad->num_criterias; ++i)
		{
			ss << ad->counter[i] << ",";
		}
		ss << "',";
		ss << ad->date << ")";
		buffer->AddQueryStr( ss.str().c_str() );

		ad->m_isDirty = false;
	}

	if( NewBuffer )
		CharacterDatabase.AddQueryBuffer( buffer );
}

WorldPacket* AchievementInterface::BuildAchievementData(bool forInspect)
{
	if(forInspect && m_achievementInspectPacket)
		return m_achievementInspectPacket;

	WorldPacket * data = new WorldPacket(forInspect ? SMSG_RESPOND_INSPECT_ACHIEVEMENTS : SMSG_ALL_ACHIEVEMENT_DATA, 400);
	if(forInspect)
		*data << m_player->GetNewGUID();

	std::map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		if( itr->second->completed )
		{
			*data << uint32(itr->first);
			*data << uint32( unixTimeToTimeBitfields(itr->second->date) );
		}
	}

	*data << int32(-1);
	itr = m_achivementDataMap.begin(); // Re-loop, luls
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		if( !itr->second->completed )
		{
			AchievementEntry * ae = dbcAchievement.LookupEntry( itr->second->id );
			// Loop over the associated criteria
			for(uint32 i = 0; i < ae->AssociatedCriteriaCount; ++i)
			{
				*data << uint32( ae->AssociatedCriteria[i] );
				uint32 counterVar = itr->second->counter[i];
				FastGUIDPack( *data, counterVar );
				*data << m_player->GetNewGUID();
				*data << uint32(0);
				*data << uint32( unixTimeToTimeBitfields( time(NULL) ) );
				*data << uint32(0);
				*data << uint32(0);
			}
		}
	}
	*data << int32(-1);

	if(forInspect)
		m_achievementInspectPacket = data;

	return data;
}

void AchievementInterface::GiveRewardsForAchievement(AchievementEntry * ae)
{
	AchievementReward * ar = AchievementRewardStorage.LookupEntry( ae->ID );
	if(!ar) return;

	// Reward: Item
	if( ar->ItemID )
	{
		// Just use the in-game mail system and mail it to him.
		MailMessage msg;
		memset(&msg, 0, sizeof(MailMessage));
		
		Item* pItem = objmgr.CreateItem( ar->ItemID, NULL );
		if(!pItem) return;

		pItem->SaveToDB( INVENTORY_SLOT_NOT_SET, 0, true, NULL );
		msg.items.push_back(pItem->GetUInt32Value(OBJECT_FIELD_GUID));

		msg.body = "Your reward for completing this achievement is attached below.";
		msg.subject = string(ae->name);

		msg.sender_guid = m_player->GetGUID();
		msg.player_guid = m_player->m_playerInfo->guid;
		msg.delivery_time = (uint32)UNIXTIME;
		msg.expire_time = 0; // This message NEVER expires.
		
		DEBUG_LOG("AchievementInterface","GiveRewardsForAchievement disabled until properly fixed");
//		sMailSystem.DeliverMessage(&msg);

		pItem->Destructor();
	}

	// Reward: Title. We don't yet support titles due to a lack of uint128.
}

void AchievementInterface::EventAchievementEarned(AchievementData * pData)
{
	pData->completed = true;
	pData->date = (uint32)time(NULL);

	AchievementEntry * ae = dbcAchievement.LookupEntry(pData->id);

	GiveRewardsForAchievement(ae);

	WorldPacket * data = BuildAchievementEarned(pData);

	if( m_player->IsInWorld() )
		m_player->GetSession()->SendPacket( data );
	else
		m_player->CopyAndSendDelayedPacket( data );

	delete data;

	HandleAchievementCriteriaRequiresAchievement(pData->id);

	// Realm First Achievements
	if( string(ae->name).find("Realm First") != string::npos  ) // flags are wrong lol
	{
		m_completedRealmFirstAchievements.insert( ae->ID );

		// Send to my team
		WorldPacket data(SMSG_SERVER_FIRST_ACHIEVEMENT, 60);
		data << m_player->GetName();
		data << m_player->GetGUID();
		data << ae->ID;
		data << uint32(1);
		sWorld.SendFactionMessage(&data, m_player->GetTeam());

		// Send to the other team (no clickable link)
		WorldPacket data2(SMSG_SERVER_FIRST_ACHIEVEMENT, 60);
		data2 << m_player->GetName();
		data2 << m_player->GetGUID();
		data2 << ae->ID;
		data2 << uint32(0);
		sWorld.SendFactionMessage(&data2, m_player->GetTeam() ? 0 : 1);
	}
}



WorldPacket* AchievementInterface::BuildAchievementEarned(AchievementData * pData)
{
	pData->m_isDirty = true;
	WorldPacket * data = new WorldPacket(SMSG_ACHIEVEMENT_EARNED, 40);
	*data << m_player->GetNewGUID();
	*data << pData->id;
	*data << uint32( unixTimeToTimeBitfields(time(NULL)) );
	*data << uint32(0);

	if( m_achievementInspectPacket )
	{
		delete m_achievementInspectPacket;
		m_achievementInspectPacket = NULL;
	}

	return data;
}

AchievementData* AchievementInterface::CreateAchievementDataEntryForAchievement(AchievementEntry * ae)
{
	AchievementData * ad = new AchievementData;
	memset(ad, 0, sizeof(AchievementData));
	ad->id = ae->ID;
	ad->num_criterias = ae->AssociatedCriteriaCount;
	m_achivementDataMap.insert( make_pair( ad->id, ad ) );
	return ad;
}

bool AchievementInterface::CanCompleteAchievement(AchievementData * ad)
{
	// don't allow GMs to complete achievements
	if( m_player->GetSession()->HasGMPermissions() )
		return false;

	if( ad->completed ) return false;

	bool hasCompleted = false;
	AchievementEntry * ach = dbcAchievement.LookupEntry(ad->id);
	if( ach->categoryId == 1 ) // We cannot complete statistics
		return false;

	// realm first achievements
	if( m_completedRealmFirstAchievements.find(ad->id) != m_completedRealmFirstAchievements.end() )
		return false;

	bool failedOne = false;
	for(uint32 i = 0; i < ad->num_criterias; ++i)
	{
		bool thisFail = false;
		AchievementCriteriaEntry * ace = dbcAchivementCriteria.LookupEntry(ach->AssociatedCriteria[i]);
		uint32 ReqCount = ace->raw.field4 ? ace->raw.field4 : 1;

		if( ace->groupFlag & ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP && m_player->GetGroup() )
			return false;

		if( ace->timeLimit && ace->timeLimit < ad->completionTimeLast )
			thisFail = true;

		if( ad->counter[i] < ReqCount )
			thisFail = true;

		if( ach->factionFlag == ACHIEVEMENT_FACTION_ALLIANCE && m_player->GetTeam() == 1 )
			thisFail = true;

		if( ach->factionFlag == ACHIEVEMENT_FACTION_HORDE && m_player->GetTeam() == 0 )
			thisFail = true;

		if( thisFail && ace->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_ONE_FLAG )
			failedOne = true;
		else if(thisFail)
			return false;

		if( !thisFail && ace->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_ONE_FLAG )
			hasCompleted = true;
	}

	if( failedOne && !hasCompleted )
		return false;

	return true;
}

AchievementData* AchievementInterface::GetAchievementDataByAchievementID(uint32 ID)
{
	map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.find( ID );
	if( itr != m_achivementDataMap.end() )
		return itr->second;
	else
	{
		AchievementEntry * ae = dbcAchievement.LookupEntryForced(ID);
		if(!ae)
		{
			printf("No achievement for %u!\n", ID);
		}
		return CreateAchievementDataEntryForAchievement(ae);
	}
}

void AchievementInterface::SendCriteriaUpdate(AchievementData * ad, uint32 idx)
{
	ad->m_isDirty = true;
	ad->date = (uint32)time(NULL);
	WorldPacket data(SMSG_CRITERIA_UPDATE, 50);
	AchievementEntry * ae = dbcAchievement.LookupEntry(ad->id);
	data << uint32(ae->AssociatedCriteria[idx]);
	FastGUIDPack( data, (uint64)ad->counter[idx] );
	data << m_player->GetNewGUID();   
	data << uint32(0);
	data << uint32(unixTimeToTimeBitfields(time(NULL)));
	data << uint32(0);
	data << uint32(0); 

	if( !m_player->IsInWorld() )
		m_player->CopyAndSendDelayedPacket(&data);
	else
		m_player->GetSession()->SendPacket(&data);

	if( m_achievementInspectPacket )
	{
		delete m_achievementInspectPacket;
		m_achievementInspectPacket = NULL;
	}
}

void AchievementInterface::HandleAchievementCriteriaConditionDeath()
{
	// We died, so reset all our achievements that have ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH
	if( !m_achivementDataMap.size() )
		return;

	map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		AchievementData * ad = itr->second;
		if(ad->completed) continue;
		AchievementEntry * ae = dbcAchievement.LookupEntry( ad->id );
		for(uint32 i = 0; i < ad->num_criterias; ++i)
		{
			uint32 CriteriaID = ae->AssociatedCriteria[i];
			AchievementCriteriaEntry * ace = dbcAchivementCriteria.LookupEntry( CriteriaID );
			if( ad->counter[i] && ace->raw.additionalRequirement1_type & ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH )
			{
				ad->counter[i] = 0;
				SendCriteriaUpdate(ad, i); break;
			}
		}
	}
}

void AchievementInterface::HandleAchievementCriteriaKillCreature(uint32 killedMonster)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
	{
		return;
	}
	
	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqKill = ace->kill_creature.creatureID;
		uint32 ReqCount = ace->kill_creature.creatureCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;


		// Wrong monster, continue on, kids.
		if( ReqKill != killedMonster )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaWinBattleground(uint32 bgMapId, uint32 scoreMargin, uint32 time, CBattleground* bg)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_WIN_BG );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqBGMap = ace->win_bg.bgMapID;
		uint32 ReqCount = ace->win_bg.winCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;
		// Wrong BG, continue on, kids.
		if( ReqBGMap != bgMapId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		ad->completionTimeLast = time;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				if( compareCriteria->raw.additionalRequirement1_type && scoreMargin < compareCriteria->raw.additionalRequirement1_type ) // BG Score Requirement.
					continue;

				// AV stuff :P
				if( bg->GetType() == BATTLEGROUND_ALTERAC_VALLEY )
				{
					AlteracValley* pAV(TO_ALTERACVALLEY(bg));
					if( pAchievementEntry->ID == 225 ||  pAchievementEntry->ID == 1164) // AV: Everything Counts
					{
						continue; // We do not support mines yet in AV
					}

					if( pAchievementEntry->ID == 220 ) // AV: Stormpike Perfection
					{
						bool failure = false;
						// We must control all Alliance nodes and Horde nodes (towers only)
						for(uint32 i = 0; i < AV_NUM_CONTROL_POINTS; ++i)
						{
							if( pAV->GetNode(i)->IsGraveyard() )
								continue;

							if( pAV->GetNode(i)->GetState() != AV_NODE_STATE_ALLIANCE_CONTROLLED )
								failure = true;
						}
						if( failure ) continue;
					}

					if( pAchievementEntry->ID == 873 ) // AV: Frostwolf Perfection
					{
						bool failure = false;
						// We must control all Alliance nodes and Horde nodes (towers only)
						for(uint32 i = 0; i < AV_NUM_CONTROL_POINTS; ++i)
						{
							if( pAV->GetNode(i)->IsGraveyard() )
								continue;

							if( pAV->GetNode(i)->GetState() != AV_NODE_STATE_HORDE_CONTROLLED )
								failure = true;
						}
						if( failure ) continue;
					}
				}

				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaRequiresAchievement(uint32 achievementId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqAchievement = ace->complete_achievement.linkedAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( ReqAchievement != achievementId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaLevelUp(uint32 level)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqLevel = ace->reach_level.level;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( level < ReqLevel )
			continue;

		// Realm first to 80 stuff has race and class requirements. Let the hacking begin.
		if( string(pAchievementEntry->name).find("Realm First!") != string::npos )
		{
			static const char* classNames[] = { "", "Warrior", "Paladin", "Hunter", "Rogue", "Priest", "Death Knight", "Shaman", "Mage", "Warlock", "", "Druid" };
			static const char* raceNames[] = { "", "Human", "Orc", "Dwarf", "Night Elf", "Forsaken", "Tauren", "Gnome", "Troll", "", "Blood Elf", "Draenei" };

			uint32 ReqClass = 0;
			uint32 ReqRace = 0;
			for(uint32 i = 0; i < 12; ++i)
			{
				if(strlen(classNames[i]) > 0 && string(pAchievementEntry->name).find(classNames[i]) != string::npos )
				{
					// We require this class
					ReqClass = i;
					break;
				}
			}
			for(uint32 i = 0; i < 12; ++i)
			{
				if(strlen(raceNames[i]) > 0 && string(pAchievementEntry->name).find(raceNames[i]) != string::npos )
				{
					// We require this race
					ReqRace = i;
					break;
				}
			}

			if( ReqClass && m_player->getClass() != ReqClass )
				continue;

			if( ReqRace && m_player->getRace() != ReqRace )
				continue;
		}

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = m_player->getLevel() > ReqLevel ? ReqLevel : m_player->getLevel();
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaOwnItem(uint32 itemId, uint32 stack)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqItemId = ace->own_item.itemID;
		uint32 ReqItemCount = ace->own_item.itemCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( itemId != ReqItemId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if( ad->completed )
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + stack;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}

	HandleAchievementCriteriaLootItem(itemId, stack);
}

void AchievementInterface::HandleAchievementCriteriaLootItem(uint32 itemId, uint32 stack)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqItemId = ace->loot_item.itemID;
		uint32 ReqItemCount = ace->loot_item.itemCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( itemId != ReqItemId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + stack;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaQuestCount(uint32 questCount)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqQuestCount = ace->complete_quest_count.totalQuestCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = questCount;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaHonorableKillClass(uint32 classId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqClass = ace->hk_class.classID;
		uint32 ReqCount = ace->hk_class.count;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( ReqClass != classId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaHonorableKillRace(uint32 raceId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_HK_RACE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqRace = ace->hk_race.raceID;
		uint32 ReqCount = ace->hk_race.count;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( ReqRace != raceId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaTalentResetCostTotal(uint32 cost)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + cost;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaTalentResetCount()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaBuyBankSlot(bool retroactive)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqSlots = ace->buy_bank_slot.numberOfSlots; // We don't actually use this. :P

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				if( retroactive )
				{
					uint32 bytes = m_player->GetUInt32Value(PLAYER_BYTES_2);
					uint32 slots = (uint8)(bytes >> 16);
					ad->counter[i] = slots > ReqSlots ? ReqSlots : slots;
				}
				else
					ad->counter[i] = ad->counter[i] + 1;

				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaFlightPathsTaken()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaExploreArea(uint32 areaId, uint32 explorationFlags)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqFlags = ace->explore_area.areaReference;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		WorldMapOverlayEntry * wmoe = dbcWorldMapOverlay.LookupEntryForced( ReqFlags );
		if(!wmoe) continue;

		AreaTable * at = dbcArea.LookupEntryForced(wmoe->AreaTableID);
		if(!at || !(ReqFlags & at->explorationFlag) )
			continue;

		uint32 offset = at->explorationFlag / 32;
		offset += PLAYER_EXPLORED_ZONES_1;

		uint32 val = (uint32)(1 << (at->explorationFlag % 32));
		uint32 currFields = m_player->GetUInt32Value(offset);

		// Not explored /sadface
		if( !(currFields & val) )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaHonorableKill()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqKills = ace->honorable_kill.killCount;

		if( m_player->m_killsLifetime < ReqKills )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

#define SCRIPTOK_FALSE { scriptOk = false; break; }
void AchievementInterface::HandleAchievementCriteriaDoEmote(uint32 emoteId, Unit* pTarget)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqEmote = ace->do_emote.emoteID;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if(ReqEmote != emoteId)
			continue; // Wrong emote, newb!

		// Target information is not stored, so we'll have to do this one by one...
		// --unless the target's name is the description of the criteria! Bahahaha
		bool scriptOk = false;
		if( pTarget && pTarget->IsCreature() )
		{
			Creature* pCreature = TO_CREATURE(pTarget);
			if( !(!ace->name || strlen(ace->name) == 0 || !pCreature->GetCreatureInfo() || stricmp(pCreature->GetCreatureInfo()->Name, ace->name) != 0) )
			{
				scriptOk = true;
			}
		}

		string name = string(pAchievementEntry->name);
		if( name.find("Total") != string::npos )
		{
			// It's a statistic, like: "Total Times /Lol'd"
			scriptOk = true;
		}

		// Script individual ones here...
		if( ace->ID == 2379 ) // Make Love, Not Warcraft
		{
			if( !pTarget || !pTarget->IsPlayer() || !pTarget->isDead() || !isHostile(pTarget, m_player) )
				SCRIPTOK_FALSE

			scriptOk = true;
		}
		else if( ace->ID == 6261 ) // Winter Veil: A Frosty Shake 
		{
			if( m_player->GetZoneId() != 4395 ) // Not in Dalaran
				SCRIPTOK_FALSE
			
			if( !pTarget || !pTarget->HasAura(21848) ) // Not a Snowman
				SCRIPTOK_FALSE

			scriptOk = true;
		}

		if( !scriptOk ) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaCompleteQuestsInZone(uint32 zoneId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqZone = ace->complete_quests_in_zone.zoneID;
		uint32 ReqCount = ace->complete_quests_in_zone.questCount;

		if( ReqZone != zoneId )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) 
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); 
				break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaReachSkillLevel(uint32 skillId, uint32 skillLevel)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqSkill = ace->reach_skill_level.skillID;
		uint32 ReqLevel = ace->reach_skill_level.skillLevel;

		if( ReqSkill != skillId )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = skillLevel;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaWinDuel()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		//uint32 ReqDuels = ace->win_duel.duelCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaLoseDuel()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		//uint32 ReqDuels = ace->win_duel.duelCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaKilledByCreature(uint32 killedMonster)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE );
	if( itr == objmgr.m_achievementCriteriaMap.end() )
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqCreature = ace->killed_by_creature.creatureEntry;

		if( ReqCreature != killedMonster )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaKilledByPlayer()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaDeath()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_DEATH );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}

	HandleAchievementCriteriaDeathAtMap(m_player->GetMapId());
}

void AchievementInterface::HandleAchievementCriteriaDeathAtMap(uint32 mapId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 MapID = ace->death_at_map.mapID;

		if( mapId != MapID )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}
