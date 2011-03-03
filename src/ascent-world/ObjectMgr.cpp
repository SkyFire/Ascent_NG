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
initialiseSingleton( ObjectMgr );

ObjectMgr::ObjectMgr()
{
	m_hiPetGuid = 0;
	m_hiContainerGuid = 0;
	m_hiItemGuid = 0;
	m_hiGroupId = 1;
	m_mailid = 0;
	m_hiPlayerGuid = 0;
	m_hiCorpseGuid = 0;
	m_hiArenaTeamId=0;
	m_hiGuildId=0;
}


ObjectMgr::~ObjectMgr()
{
	Log.Notice("ObjectMgr", "Deleting Corpses...");
	CorpseCollectorUnload();

	Log.Notice("ObjectMgr", "Deleting Itemsets...");
	for(ItemSetContentMap::iterator i = mItemSets.begin(); i != mItemSets.end(); ++i)
	{
		delete i->second;
	}
	mItemSets.clear();

	Log.Notice("ObjectMgr", "Deleting PlayerCreateInfo...");
	for( PlayerCreateInfoMap::iterator i = mPlayerCreateInfo.begin( ); i != mPlayerCreateInfo.end( ); ++ i ) {
		delete i->second;
	}
	mPlayerCreateInfo.clear( );

	Log.Notice("ObjectMgr", "Deleting Guilds...");
	for ( GuildMap::iterator i = mGuild.begin(); i != mGuild.end(); ++i ) {
		delete i->second;
	}

	Log.Notice("ObjectMgr", "Deleting Vendors...");
	for( VendorMap::iterator i = mVendors.begin( ); i != mVendors.end( ); ++ i ) 
	{
		delete i->second;
	}

	Log.Notice("ObjectMgr", "Deleting Trainers...");
	for( TrainerMap::iterator i = mTrainers.begin( ); i != mTrainers.end( ); ++ i) {
		Trainer * t = i->second;
		if(t->UIMessage)
			delete [] t->UIMessage;
		delete t;
	}

	Log.Notice("ObjectMgr", "Deleting Level Information...");
	for( LevelInfoMap::iterator i = mLevelInfo.begin(); i != mLevelInfo.end(); ++i)
	{
		LevelMap * l = i->second;
		for(LevelMap::iterator i2 = l->begin(); i2 != l->end(); ++i2)
		{
			delete i2->second;
		}
		l->clear();
		delete l;
	}

	Log.Notice("ObjectMgr", "Deleting Waypoint Cache...");
	for(HM_NAMESPACE::hash_map<uint32, WayPointMap*>::iterator i = m_waypoints.begin(); i != m_waypoints.end(); ++i)
	{
		for(WayPointMap::iterator i2 = i->second->begin(); i2 != i->second->end(); ++i2)
			if((*i2))
				delete (*i2);

		delete i->second;
	}

	Log.Notice("ObjectMgr", "Deleting NPC Say Texts...");
	for(uint32 i = 0 ; i < NUM_MONSTER_SAY_EVENTS ; ++i)
	{
		NpcMonsterSay * p;
		for(MonsterSayMap::iterator itr = mMonsterSays[i].begin(); itr != mMonsterSays[i].end(); ++itr)
		{
			p = itr->second;
			for(uint32 j = 0; j < p->TextCount; ++j)
				free((char*)p->Texts[j]);
			delete [] p->Texts;
			free((char*)p->MonsterName);
			delete p;
		}

		mMonsterSays[i].clear();
	}

	Log.Notice("ObjectMgr", "Deleting Charters...");
	for(int i = 0; i < NUM_CHARTER_TYPES; ++i)
	{
		for(HM_NAMESPACE::hash_map<uint32, Charter*>::iterator itr =  m_charters[i].begin(); itr != m_charters[i].end(); ++itr)
		{
			delete itr->second;
		}
	}

	Log.Notice("ObjectMgr", "Deleting Reputation Tables...");
	for(ReputationModMap::iterator itr = this->m_reputation_creature.begin(); itr != m_reputation_creature.end(); ++itr)
	{
		ReputationModifier * mod = itr->second;
		mod->mods.clear();
		delete mod;
	}
	for(ReputationModMap::iterator itr = this->m_reputation_faction.begin(); itr != m_reputation_faction.end(); ++itr)
	{
		ReputationModifier * mod = itr->second;
		mod->mods.clear();
		delete mod;
	}

	for(HM_NAMESPACE::hash_map<uint32,InstanceReputationModifier*>::iterator itr = this->m_reputation_instance.begin(); itr != this->m_reputation_instance.end(); ++itr)
	{
		InstanceReputationModifier * mod = itr->second;
		mod->mods.clear();
		delete mod;
	}

	Log.Notice("ObjectMgr", "Deleting Groups...");
	for(GroupMap::iterator itr = m_groups.begin(); itr != m_groups.end();)
	{
		Group* pGroup = itr->second;
		++itr;

		if( pGroup != NULL )
		{
			for( uint32 i = 0; i < pGroup->GetSubGroupCount(); ++i )
			{
				SubGroup* pSubGroup = pGroup->GetSubGroup( i );
				if( pSubGroup != NULL )
				{
					pSubGroup->Disband();
				}
			}
			delete pGroup;
		}
	}

	Log.Notice("ObjectMgr", "Deleting Player Information...");
	for(HM_NAMESPACE::hash_map<uint32, PlayerInfo*>::iterator itr = m_playersinfo.begin(); itr != m_playersinfo.end(); ++itr)
	{
		itr->second->m_Group=NULL;
		free(itr->second->name);
		delete itr->second;
	}

	Log.Notice("ObjectMgr", "Deleting GM Tickets...");
	for(GmTicketList::iterator itr = GM_TicketList.begin(); itr != GM_TicketList.end(); ++itr)
		delete (*itr);

	Log.Notice("ObjectMgr", "Deleting Arena Teams..."); 
	for(HM_NAMESPACE::hash_map<uint32, ArenaTeam*>::iterator itr = m_arenaTeams.begin(); itr != m_arenaTeams.end(); ++itr) 
		delete itr->second;

	Log.Notice("ObjectMgr", "Deleting Profession Discoveries...");
	std::set<ProfessionDiscovery*>::iterator itr = ProfessionDiscoveryTable.begin();
	for ( ; itr != ProfessionDiscoveryTable.end(); itr++ )
		delete (*itr);	

	Log.Notice("ObjectMgr", "Deleting Achievement Cache...");
	for(AchievementCriteriaMap::iterator itr = m_achievementCriteriaMap.begin(); itr != m_achievementCriteriaMap.end(); ++itr)
		delete (itr->second);

	Log.Notice("ObjectMgr", "Deleting Achievement Quest Map...");
	for(map<uint32,set<Quest*>*>::iterator qitr = ZoneToQuestMap.begin(); qitr != ZoneToQuestMap.end(); ++qitr)
		delete qitr->second;

	Log.Notice("ObjectMgr", "Deleting Pet Levelup Spells...");
	for(PetLevelupSpellMap::iterator itr = mPetLevelupSpellMap.begin(); itr != mPetLevelupSpellMap.end(); ++itr)
	{
		itr->second.clear();
	}
	mPetLevelupSpellMap.clear();
}

void ObjectMgr::LoadAchievements()
{
	for(uint32 i = 0; i < dbcAchievement.GetNumRows(); ++i)
	{
		AchievementEntry * ae = dbcAchievement.LookupRow(i);
		if(ae)
		{
			ae->AssociatedCriteriaCount = 0;
		}
	}

	for(uint32 i = 0; i < dbcAchivementCriteria.GetNumRows(); ++i)
	{
		AchievementCriteriaEntry * ace = dbcAchivementCriteria.LookupRow( i );
		if( ace )
		{
			AchievementCriteriaMap::iterator itr = m_achievementCriteriaMap.find( ace->requiredType );
			if( itr == m_achievementCriteriaMap.end() )
			{
				// We need to make a new set, and insert this.
				AchievementCriteriaSet * acs = new AchievementCriteriaSet;
				acs->insert( ace );
				m_achievementCriteriaMap.insert( make_pair( ace->requiredType, acs ) );
			}
			else
			{
				// Push into old set
				AchievementCriteriaSet * acs = itr->second;
				acs->insert( ace );
			}

			AchievementEntry * ae = dbcAchievement.LookupEntryForced( ace->referredAchievement );
			if( ae )
			{
				if(ae->AssociatedCriteriaCount >= 32) continue;
				ae->AssociatedCriteria[ ae->AssociatedCriteriaCount ] = ace->ID;
				ae->AssociatedCriteriaCount++;
			}
		}
	}

	// For Achievements: Complete Quest by Zone Id
	StorageContainerIterator<Quest> * itr = QuestStorage.MakeIterator();
	for(; !itr->AtEnd(); itr->Inc() )
	{
		Quest * pQuest = itr->Get();
		if(!pQuest) continue;

		uint32 QuestZone = pQuest->zone_id;
		map<uint32,set<Quest*>*>::iterator qitr = ZoneToQuestMap.find( QuestZone );
		if( qitr == ZoneToQuestMap.end() )
		{
			// We don't have any entries for this zone yet.
			set<Quest*>* mySet = new set<Quest*>;
			mySet->insert( pQuest );
			ZoneToQuestMap.insert( make_pair( QuestZone, mySet ) );
		}
		else
		{
			set<Quest*>* thisSet = qitr->second;
			thisSet->insert( pQuest );
		}
	}
	itr->Destruct();
}

//
// Groups
//

Group * ObjectMgr::GetGroupByLeader(Player* pPlayer)
{
	GroupMap::iterator itr;
	Group * ret=NULL;
	m_groupLock.AcquireReadLock();
	for(itr = m_groups.begin(); itr != m_groups.end(); ++itr)
	{
		if(itr->second->GetLeader()==pPlayer->m_playerInfo)
		{
			ret = itr->second;
			break;
		}
	}

	m_groupLock.ReleaseReadLock();
	return ret;
}

Group * ObjectMgr::GetGroupById(uint32 id)
{
	GroupMap::iterator itr;
	Group * ret=NULL;
	m_groupLock.AcquireReadLock();
	itr = m_groups.find(id);
	if(itr!=m_groups.end())
		ret=itr->second;

	m_groupLock.ReleaseReadLock();
	return ret;
}

//
// Player names
//
void ObjectMgr::DeletePlayerInfo( uint32 guid )
{
	PlayerInfo * pl;
	HM_NAMESPACE::hash_map<uint32,PlayerInfo*>::iterator i;
	PlayerNameStringIndexMap::iterator i2;
	playernamelock.AcquireWriteLock();
	i=m_playersinfo.find(guid);
	if(i==m_playersinfo.end())
	{
		playernamelock.ReleaseWriteLock();
		return;
	}
	
	pl=i->second;
	if(pl->m_Group)
	{
		pl->m_Group->RemovePlayer(pl);
		pl->m_Group = NULL;
	}

	if(pl->guild)
	{
		if(pl->guild->GetGuildLeader()==pl->guid)
			pl->guild->Disband();
		else
			pl->guild->RemoveGuildMember(pl,NULL);
	}

	string pnam = string(pl->name);
	ASCENT_TOLOWER(pnam);
	i2 = m_playersInfoByName.find(pnam);
	if( i2 != m_playersInfoByName.end() && i2->second == pl )
		m_playersInfoByName.erase( i2 );

	free(pl->name);
	delete i->second;
	m_playersinfo.erase(i);

	playernamelock.ReleaseWriteLock();
}

PlayerInfo *ObjectMgr::GetPlayerInfo( uint32 guid )
{
	HM_NAMESPACE::hash_map<uint32,PlayerInfo*>::iterator i;
	PlayerInfo * rv;
	playernamelock.AcquireReadLock();
	i=m_playersinfo.find(guid);
	if(i!=m_playersinfo.end())
		rv = i->second;
	else
		rv = NULL;
	playernamelock.ReleaseReadLock();
	return rv;
}

void ObjectMgr::AddPlayerInfo(PlayerInfo *pn)
{
	playernamelock.AcquireWriteLock();
	m_playersinfo[pn->guid] =  pn ;
	string pnam = string(pn->name);
	ASCENT_TOLOWER(pnam);
	m_playersInfoByName[pnam] = pn;
	playernamelock.ReleaseWriteLock();
}

void ObjectMgr::RenamePlayerInfo(PlayerInfo * pn, const char * oldname, const char * newname)
{
	playernamelock.AcquireWriteLock();
	string oldn = string(oldname);
	ASCENT_TOLOWER(oldn);

	PlayerNameStringIndexMap::iterator itr = m_playersInfoByName.find( oldn );
	if( itr != m_playersInfoByName.end() && itr->second == pn )
	{
		string newn = string(newname);
		ASCENT_TOLOWER(newn);
		m_playersInfoByName.erase( itr );
		m_playersInfoByName[newn] = pn;
	}

	playernamelock.ReleaseWriteLock();
}

void ObjectMgr::LoadSpellSkills()
{
	uint32 i;
//	int total = sSkillStore.GetNumRows();

	for(i = 0; i < dbcSkillLineSpell.GetNumRows(); i++)
	{
		skilllinespell *sp = dbcSkillLineSpell.LookupRow(i);
		if (sp)
		{
			mSpellSkills[sp->spell] = sp;
		}
	}
	Log.Notice("ObjectMgr", "%u spell skills loaded.", mSpellSkills.size());
}

skilllinespell* ObjectMgr::GetSpellSkill(uint32 id)
{
	return mSpellSkills[id];
}

void ObjectMgr::LoadPlayersInfo()
{
	PlayerInfo * pn;
	QueryResult *result = CharacterDatabase.Query("SELECT guid,name,race,class,level,gender,zoneId,timestamp,acct FROM characters");
	uint32 period, c;
	if(result)
	{
		period = (result->GetRowCount() / 20) + 1;
		c = 0;
		
		do
		{
			Field *fields = result->Fetch();
			pn=new PlayerInfo;
			memset(pn, 0, sizeof(PlayerInfo));
			pn->guid = fields[0].GetUInt32();
			pn->name = strdup(fields[1].GetString());
			pn->race = fields[2].GetUInt8();
			pn->cl = fields[3].GetUInt8();
			pn->lastLevel = fields[4].GetUInt32();
			pn->gender = fields[5].GetUInt8();
			pn->lastZone=fields[6].GetUInt32();
			pn->lastOnline=fields[7].GetUInt32();
			pn->acct = fields[8].GetUInt32();
#ifdef VOICE_CHAT
			pn->groupVoiceId = -1;
#endif

			if(pn->race==RACE_HUMAN||pn->race==RACE_DWARF||pn->race==RACE_GNOME||pn->race==RACE_NIGHTELF||pn->race==RACE_DRAENEI)
				pn->team = 0;
			else 
				pn->team = 1;
		  
			if( GetPlayerInfoByName(pn->name) != NULL )
			{
				// gotta rename him
				char temp[300];
				snprintf(temp, 300, "%s__%X__", pn->name, pn->guid);
				Log.Notice("ObjectMgr", "Renaming duplicate player %s to %s. (%u)", pn->name,temp,pn->guid);
				CharacterDatabase.WaitExecute("UPDATE characters SET name = \"%s\", forced_rename_pending = 1 WHERE guid = %u",
					CharacterDatabase.EscapeString(string(temp)).c_str(), pn->guid);

				free(pn->name);
				pn->name = strdup(temp);
			}

			string lpn=string(pn->name);
			ASCENT_TOLOWER(lpn);
			m_playersInfoByName[lpn] = pn;

			//this is startup -> no need in lock -> don't use addplayerinfo
			 m_playersinfo[(uint32)pn->guid]=pn;

			 if( !((++c) % period) )
				 Log.Notice("PlayerInfo", "Done %u/%u, %u%% complete.", c, result->GetRowCount(), float2int32( (float(c) / float(result->GetRowCount()))*100.0f ));
		} while( result->NextRow() );

		delete result;
	}
	Log.Notice("ObjectMgr", "%u players loaded.", m_playersinfo.size());
	LoadGuilds();
}

PlayerInfo* ObjectMgr::GetPlayerInfoByName(const char * name)
{
	string lpn=string(name);
	ASCENT_TOLOWER(lpn);
	PlayerNameStringIndexMap::iterator i;
	PlayerInfo *rv = NULL;
	playernamelock.AcquireReadLock();

	i=m_playersInfoByName.find(lpn);
	if( i != m_playersInfoByName.end() )
		rv = i->second;

	playernamelock.ReleaseReadLock();
	return rv;
}

void ObjectMgr::LoadPlayerCreateInfo()
{
	QueryResult *result = WorldDatabase.Query( "SELECT * FROM playercreateinfo" );

	if( !result )
	{
		Log.Error("MySQL","Query failed: SELECT * FROM playercreateinfo");
		return;
	}

	 if( result->GetFieldCount() < 24 )
	 {
		 Log.Error("PlayerCreateInfo", "Incorrect number of columns in playercreateinfo found %u, should be 24. check for sql updates", result->GetFieldCount());
		 return;
	 }

	PlayerCreateInfo *pPlayerCreateInfo;

	do
	{
		Field *fields = result->Fetch();

		pPlayerCreateInfo = new PlayerCreateInfo;

		pPlayerCreateInfo->index = fields[0].GetUInt8();
		pPlayerCreateInfo->race = fields[1].GetUInt8();
		pPlayerCreateInfo->factiontemplate = fields[2].GetUInt32();
		pPlayerCreateInfo->class_ = fields[3].GetUInt8();
		pPlayerCreateInfo->mapId = fields[4].GetUInt32();
		pPlayerCreateInfo->zoneId = fields[5].GetUInt32();
		pPlayerCreateInfo->positionX = fields[6].GetFloat();
		pPlayerCreateInfo->positionY = fields[7].GetFloat();
		pPlayerCreateInfo->positionZ = fields[8].GetFloat();
		pPlayerCreateInfo->displayId = fields[9].GetUInt16();
		pPlayerCreateInfo->strength = fields[10].GetUInt8();
		pPlayerCreateInfo->ability = fields[11].GetUInt8();
		pPlayerCreateInfo->stamina = fields[12].GetUInt8();
		pPlayerCreateInfo->intellect = fields[13].GetUInt8();
		pPlayerCreateInfo->spirit = fields[14].GetUInt8();
		pPlayerCreateInfo->health = fields[15].GetUInt32();
		pPlayerCreateInfo->mana = fields[16].GetUInt32();
		pPlayerCreateInfo->rage = fields[17].GetUInt32();
		pPlayerCreateInfo->focus = fields[18].GetUInt32();
		pPlayerCreateInfo->energy = fields[19].GetUInt32();
		pPlayerCreateInfo->runic = fields[20].GetUInt32();
		pPlayerCreateInfo->attackpower = fields[21].GetUInt32();
		pPlayerCreateInfo->mindmg = fields[22].GetFloat();
		pPlayerCreateInfo->maxdmg = fields[23].GetFloat();

		QueryResult *sk_sql = WorldDatabase.Query(
			"SELECT * FROM playercreateinfo_skills WHERE indexid=%u",pPlayerCreateInfo->index);

		if(sk_sql)
		{
			do 
			{
				Field *fields = sk_sql->Fetch();
				CreateInfo_SkillStruct tsk;
				tsk.skillid = fields[1].GetUInt32();
				tsk.currentval = fields[2].GetUInt32();
				tsk.maxval = fields[3].GetUInt32();
				pPlayerCreateInfo->skills.push_back(tsk);
			} while(sk_sql->NextRow());
			delete sk_sql;
		}
		QueryResult *sp_sql = WorldDatabase.Query(
			"SELECT * FROM playercreateinfo_spells WHERE indexid=%u",pPlayerCreateInfo->index);

		if(sp_sql)
		{
			do 
			{
				pPlayerCreateInfo->spell_list.insert(sp_sql->Fetch()[1].GetUInt32());
			} while(sp_sql->NextRow());
			delete sp_sql;
		}
	  
		QueryResult *items_sql = WorldDatabase.Query(
			"SELECT * FROM playercreateinfo_items WHERE indexid=%u",pPlayerCreateInfo->index);
		
		if(items_sql)
		{
			do 
			{
				Field *fields = items_sql->Fetch();
				CreateInfo_ItemStruct itm;
				itm.protoid = fields[1].GetUInt32();
				itm.slot = fields[2].GetUInt8();
				itm.amount = fields[3].GetUInt32();
				pPlayerCreateInfo->items.push_back(itm);
			} while(items_sql->NextRow());
		   delete items_sql;
		}

		QueryResult *bars_sql = WorldDatabase.Query(
			"SELECT * FROM playercreateinfo_bars WHERE class=%u",pPlayerCreateInfo->class_ );

		if(bars_sql)
		{
			do 
			{
				Field *fields = bars_sql->Fetch();
				CreateInfo_ActionBarStruct bar;
				bar.button = fields[2].GetUInt32();
				bar.action = fields[3].GetUInt32();
				bar.type = fields[4].GetUInt32();
				bar.misc = fields[5].GetUInt32();
				pPlayerCreateInfo->actionbars.push_back(bar);
			} while(bars_sql->NextRow());			
			delete bars_sql;
		}
	
		mPlayerCreateInfo[pPlayerCreateInfo->index] = pPlayerCreateInfo;
	} while( result->NextRow() );

	delete result;

	Log.Notice("ObjectMgr", "%u player create infos loaded.", mPlayerCreateInfo.size());
	GenerateLevelUpInfo();
}

// DK:LoadGuilds()
void ObjectMgr::LoadGuilds()
{
	QueryResult *result = CharacterDatabase.Query( "SELECT * FROM guilds" );
	if(result)
	{
		uint32 period = (result->GetRowCount() / 20) + 1;
		uint32 c = 0;
		do 
		{
			Guild * pGuild = Guild::Create();
			if(!pGuild->LoadFromDB(result->Fetch()))
			{
				delete pGuild;
			}
			else
				mGuild.insert(make_pair(pGuild->GetGuildId(), pGuild));

			if( !((++c) % period) )
				Log.Notice("Guilds", "Done %u/%u, %u%% complete.", c, result->GetRowCount(), float2int32( (float(c) / float(result->GetRowCount()))*100.0f ));

		} while(result->NextRow());
		delete result;
	}
	Log.Notice("ObjectMgr", "%u guilds loaded.", mGuild.size());
}

Corpse* ObjectMgr::LoadCorpse(uint32 guid)
{
	Corpse* pCorpse;
	QueryResult *result = CharacterDatabase.Query("SELECT * FROM Corpses WHERE guid =%u ", guid );

	if( !result )
		return NULL;
	
	do
	{
		Field *fields = result->Fetch();
		pCorpse = new Corpse(HIGHGUID_TYPE_CORPSE,fields[0].GetUInt32());
		pCorpse->Init();
		pCorpse->SetPosition(fields[1].GetFloat(), fields[2].GetFloat(), fields[3].GetFloat(), fields[4].GetFloat());
		pCorpse->SetZoneId(fields[5].GetUInt32());
		pCorpse->SetMapId(fields[6].GetUInt32());
		pCorpse->LoadValues( fields[7].GetString());
		if(pCorpse->GetUInt32Value(CORPSE_FIELD_DISPLAY_ID) == 0)
		{
			pCorpse->Destructor();
			continue;
		}

		pCorpse->SetLoadedFromDB(true);
		pCorpse->SetInstanceID(fields[8].GetUInt32());
		pCorpse->AddToWorld();
	 } while( result->NextRow() );

	delete result;

	return pCorpse;
}


//------------------------------------------------------
// Live corpse retreival.
// comments: I use the same tricky method to start from the last corpse instead of the first
//------------------------------------------------------
Corpse* ObjectMgr::GetCorpseByOwner(uint32 ownerguid)
{
	CorpseMap::const_iterator itr;
	Corpse* rv = NULL;
	_corpseslock.Acquire();
	for (itr = m_corpses.begin();itr != m_corpses.end(); ++itr)
	{
		if(itr->second->GetUInt32Value(CORPSE_FIELD_OWNER) == ownerguid)
		{
			rv = itr->second;
			break;
		}
	}
	_corpseslock.Release();


	return rv;
}

void ObjectMgr::DelinkPlayerCorpses(Player* pOwner)
{
	//dupe protection agaisnt crashs
	Corpse* c;
	c=this->GetCorpseByOwner(pOwner->GetLowGUID());
	if(!c)return;
	sEventMgr.AddEvent(c, &Corpse::Delink, EVENT_CORPSE_SPAWN_BONES, 1, 1, 0);
	CorpseAddEventDespawn(c);
}

void ObjectMgr::LoadGMTickets()
{
	QueryResult *result = CharacterDatabase.Query( "SELECT * FROM gm_tickets" );

	GM_Ticket *ticket;
	if(result == 0)
		return;

	do
	{
		Field *fields = result->Fetch();

		ticket = new GM_Ticket;
		ticket->guid = fields[0].GetUInt32();
		ticket->name = fields[1].GetString();
		ticket->level = fields[2].GetUInt32();
		ticket->type = fields[3].GetUInt32();
		ticket->posX = fields[4].GetFloat();
		ticket->posY = fields[5].GetFloat();
		ticket->posZ = fields[6].GetFloat();
		ticket->message = fields[7].GetString();
		ticket->timestamp = fields[8].GetUInt32();

		AddGMTicket(ticket,true);

	} while( result->NextRow() );

	Log.Notice("ObjectMgr", "%u GM Tickets loaded.", result->GetRowCount());
	delete result;
}

void ObjectMgr::SaveGMTicket(uint64 guid, QueryBuffer * buf)
{
	GM_Ticket* ticket = GetGMTicket(guid);
	if(!ticket)
	{
		return;
	}

	std::stringstream ss1;
	std::stringstream ss2;
	ss1 << "DELETE FROM gm_tickets WHERE guid = " << guid << ";";
	if( buf == NULL )
		CharacterDatabase.Execute(ss1.str( ).c_str( ));
	else
		buf->AddQueryStr(ss1.str());

	ss2 << "INSERT INTO gm_tickets (guid, name, level, type, posX, posY, posZ, message, timestamp) VALUES(";
	ss2 << ticket->guid << ", '";
	ss2 << CharacterDatabase.EscapeString(ticket->name) << "', ";
	ss2 << ticket->level << ", ";
	ss2 << ticket->type << ", ";
	ss2 << ticket->posX << ", ";
	ss2 << ticket->posY << ", ";
	ss2 << ticket->posZ << ", '";
	ss2 << CharacterDatabase.EscapeString(ticket->message) << "', ";
	ss2 << ticket->timestamp << ");";

	if(buf == NULL)
		CharacterDatabase.ExecuteNA(ss2.str( ).c_str( ));
	else
		buf->AddQueryStr(ss2.str());
}

void ObjectMgr::SetHighestGuids()
{
	QueryResult *result;

	result = CharacterDatabase.Query( "SELECT MAX(guid) FROM characters" );
	if( result )
	{
		m_hiPlayerGuid = result->Fetch()[0].GetUInt32();
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX(guid) FROM playeritems");
	if( result )
	{
		m_hiItemGuid = (uint32)result->Fetch()[0].GetUInt32();
		delete result;
	}

	result = CharacterDatabase.Query( "SELECT MAX(guid) FROM corpses" );
	if( result )
	{
		m_hiCorpseGuid = result->Fetch()[0].GetUInt32();
		delete result;
	}

	result = WorldDatabase.Query("SELECT MAX(id) FROM creature_spawns");
	if(result)
	{
		m_hiCreatureSpawnId = result->Fetch()[0].GetUInt32();
		delete result;
	}

	result = WorldDatabase.Query("SELECT MAX(id) FROM gameobject_spawns");
	if(result)
	{
		m_hiGameObjectSpawnId = result->Fetch()[0].GetUInt32();
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX(group_id) FROM groups");
	if(result)
	{
		m_hiGroupId = result->Fetch()[0].GetUInt32();
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX(charterId) FROM charters");
	if(result)
	{
		m_hiCharterId = result->Fetch()[0].GetUInt32();
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX(guildId) FROM guilds");
	if(result)
	{
		m_hiGuildId = result->Fetch()[0].GetUInt32();
		delete result;
	}

	Log.Notice("ObjectMgr", "HighGuid(CORPSE) = %u", m_hiCorpseGuid);
	Log.Notice("ObjectMgr", "HighGuid(PLAYER) = %u", m_hiPlayerGuid);
	Log.Notice("ObjectMgr", "HighGuid(GAMEOBJ) = %u", m_hiGameObjectSpawnId);
	Log.Notice("ObjectMgr", "HighGuid(UNIT) = %u", m_hiCreatureSpawnId);
	Log.Notice("ObjectMgr", "HighGuid(ITEM) = %u", m_hiItemGuid);
	Log.Notice("ObjectMgr", "HighGuid(CONTAINER) = %u", m_hiContainerGuid);
	Log.Notice("ObjectMgr", "HighGuid(GROUP) = %u", m_hiGroupId);
	Log.Notice("ObjectMgr", "HighGuid(CHARTER) = %u", m_hiCharterId);
	Log.Notice("ObjectMgr", "HighGuid(GUILD) = %u", m_hiGuildId);
}


uint32 ObjectMgr::GenerateMailID()
{
	return m_mailid++;
}
uint32 ObjectMgr::GenerateLowGuid(uint32 guidhigh)
{
	ASSERT(guidhigh == HIGHGUID_TYPE_ITEM || guidhigh == HIGHGUID_TYPE_CONTAINER || guidhigh == HIGHGUID_TYPE_PLAYER);

	uint32 ret;
	if(guidhigh == HIGHGUID_TYPE_ITEM)
	{
		m_guidGenMutex.Acquire();
		ret = ++m_hiItemGuid;
		m_guidGenMutex.Release();
	}else if(guidhigh==HIGHGUID_TYPE_PLAYER)
	{
		m_playerguidlock.Acquire();
		ret = ++m_hiPlayerGuid;
		m_playerguidlock.Release();
	}else{
		m_guidGenMutex.Acquire();
		ret = ++m_hiContainerGuid;
		m_guidGenMutex.Release();
	}
	return ret;
}

void ObjectMgr::ProcessGameobjectQuests()
{
	QueryResult *result = WorldDatabase.Query("SELECT * FROM npc_gossip_textid");
	if(result)
	{
		uint32 entry, text;
		do 
		{
			entry = result->Fetch()[0].GetUInt32();
			text  = result->Fetch()[1].GetUInt32();

			mNpcToGossipText[entry] = text;

		} while(result->NextRow());
		delete result;
	}
	Log.Notice("ObjectMgr", "%u NPC Gossip TextIds loaded.", mNpcToGossipText.size());
}

Player* ObjectMgr::GetPlayer(const char* name, bool caseSensitive)
{
	Player* rv = NULL;
	PlayerStorageMap::const_iterator itr;
	_playerslock.AcquireReadLock();	

	if(!caseSensitive)
	{
		std::string strName = name;
		ASCENT_TOLOWER(strName);
		for (itr = _players.begin(); itr != _players.end(); ++itr)
		{
			if(!stricmp(itr->second->GetNameString()->c_str(), strName.c_str()))
			{
				rv = itr->second;
				break;
			}
		}
	}
	else
	{
		for (itr = _players.begin(); itr != _players.end(); ++itr)
		{
			if(!strcmp(itr->second->GetName(), name))
			{
				rv = itr->second;
				break;
			}
		}
	}
		
	_playerslock.ReleaseReadLock();

	return rv;
}

Player* ObjectMgr::GetPlayer(uint32 guid)
{
	Player* rv = NULL;
	_playerslock.AcquireReadLock();	
	PlayerStorageMap::const_iterator itr = _players.find(guid);
	rv = (itr != _players.end()) ? itr->second : NULL;
	_playerslock.ReleaseReadLock();
	return rv;
}

PlayerCreateInfo* ObjectMgr::GetPlayerCreateInfo(uint8 race, uint8 class_) const
{
	PlayerCreateInfoMap::const_iterator itr;
	for (itr = mPlayerCreateInfo.begin(); itr != mPlayerCreateInfo.end(); itr++)
	{
		if( (itr->second->race == race) && (itr->second->class_ == class_) )
			return itr->second;
	}
	return NULL;
}

void ObjectMgr::AddGuild(Guild *pGuild)
{
	ASSERT( pGuild );
	mGuild[pGuild->GetGuildId()] = pGuild;
}

uint32 ObjectMgr::GetTotalGuildCount()
{
	return (uint32)mGuild.size();
}

bool ObjectMgr::RemoveGuild(uint32 guildId)
{
	GuildMap::iterator i = mGuild.find(guildId);
	if (i == mGuild.end())
	{
		return false;
	}


	mGuild.erase(i);
	return true;
}

Guild* ObjectMgr::GetGuild(uint32 guildId)
{
	GuildMap::const_iterator itr = mGuild.find(guildId);
	if(itr == mGuild.end())
		return NULL;
	return itr->second;
}

Guild* ObjectMgr::GetGuildByLeaderGuid(uint64 leaderGuid)
{
	GuildMap::const_iterator itr;
	for (itr = mGuild.begin();itr != mGuild.end(); itr++)
	{
		if( itr->second->GetGuildLeader() == leaderGuid )
			return itr->second;
	}
	return NULL;
}

Guild* ObjectMgr::GetGuildByGuildName(std::string guildName)
{
	GuildMap::const_iterator itr;
	for (itr = mGuild.begin();itr != mGuild.end(); itr++)
	{
		if( itr->second->GetGuildName() == guildName )
			return itr->second;
	}
	return NULL;
}


void ObjectMgr::AddGMTicket(GM_Ticket *ticket,bool startup)
{
	ASSERT( ticket );
	GM_TicketList.push_back(ticket);

	// save
	if(!startup)
		SaveGMTicket(ticket->guid, NULL);
}

//modified for vs2005 compatibility
void ObjectMgr::remGMTicket(uint64 guid)
{
	for(GmTicketList::iterator i = GM_TicketList.begin(); i != GM_TicketList.end();)
	{
		if((*i)->guid == guid)
		{
			i = GM_TicketList.erase(i);
		}
		else
		{
			++i;
		}
	}

	// kill from db
	CharacterDatabase.Execute("DELETE FROM gm_tickets WHERE guid=%u", guid);
}

GM_Ticket* ObjectMgr::GetGMTicket(uint64 guid)
{
	for(GmTicketList::iterator i = GM_TicketList.begin(); i != GM_TicketList.end(); i++)
	{
		if((*i)->guid == guid)
		{
			return (*i);
		}
	}
	return NULL;
}

void ObjectMgr::LoadVendors()
{
	HM_NAMESPACE::hash_map<uint32, std::vector<CreatureItem>*>::const_iterator itr;
	std::vector<CreatureItem> *items;
	CreatureItem itm;
  
	QueryResult *result = WorldDatabase.Query("SELECT * FROM vendors");
	if( result != NULL )
	{
		if( result->GetFieldCount() < 6 )
		{
			Log.Notice("ObjectMgr", "Invalid format in vendors (%u/5) columns, not enough data to proceed.\n", result->GetFieldCount() );
			return;
		}
		else if( result->GetFieldCount() > 6 )
		{
			Log.Notice("ObjectMgr", "Invalid format in vendors (%u/5) columns, loading anyway because we have enough data\n", result->GetFieldCount() );
		}

		do
		{
			Field* fields = result->Fetch();

			itr = mVendors.find( fields[0].GetUInt32() );

			if( itr == mVendors.end() )
			{
				items = new std::vector<CreatureItem>;
				mVendors[fields[0].GetUInt32()] = items;
			}
			else
			{
				items = itr->second;
			}

			itm.itemid           = fields[1].GetUInt32();
			itm.amount           = fields[2].GetUInt32();
			itm.available_amount = fields[3].GetUInt32();
			itm.max_amount       = fields[3].GetUInt32();
			itm.incrtime         = fields[4].GetUInt32();
			itm.extended_cost    = NULL;

			uint32 ec = fields[5].GetUInt32();
			if( ec != 0 )
			{
				itm.extended_cost = dbcItemExtendedCost.LookupEntryForced(ec);
				if( itm.extended_cost == NULL )
				{
					if(Config.MainConfig.GetBoolDefault("Server", "CleanDatabase", false))
					{
						WorldDatabase.Query("UPDATE vendors set extendedcost = '0' where item = '%u' AND entry = '%u'", itm.itemid, fields[0].GetUInt32());
					}
					Log.Warning("ObjectMgr","Item %u at vendor %u has extended cost %u which is invalid. Skipping.", itm.itemid, fields[0].GetUInt32(), ec);
					continue;
				}
			}

			items->push_back( itm );
		}
		while( result->NextRow() );

		delete result;
	}
	Log.Notice("ObjectMgr", "%u vendors loaded.", mVendors.size());
}

void ObjectMgr::ReloadVendors()
{
	mVendors.clear();
	LoadVendors();
}

std::vector<CreatureItem>* ObjectMgr::GetVendorList(uint32 entry)
{
	return mVendors[entry];
}

void ObjectMgr::LoadTotemSpells()
{
	std::stringstream query;
	QueryResult *result = WorldDatabase.Query( "SELECT * FROM totemspells" );

	if(!result)
	{
		return;
	}

	//TotemSpells *ts = NULL;
	SpellEntry * sp;
	uint32 spellid;

	do
	{
		Field *fields = result->Fetch();
		spellid = fields[1].GetUInt32();
		sp = dbcSpell.LookupEntry(spellid);
		if(!spellid || !sp) continue;

		m_totemSpells.insert( TotemSpellMap::value_type( fields[0].GetUInt32(), sp ));
	} while( result->NextRow() );

	delete result;
	Log.Notice("ObjectMgr", "%u totem spells loaded.", m_totemSpells.size());
}

SpellEntry* ObjectMgr::GetTotemSpell(uint32 spellId)
{
	return m_totemSpells[spellId];
}

void ObjectMgr::LoadAIThreatToSpellId()
{
	QueryResult *result = WorldDatabase.Query( "SELECT * FROM ai_threattospellid" );

	if(!result)
	{
		return;
	}

	uint32 spellid;
	SpellEntry * sp;

	do
	{
		Field *fields = result->Fetch();
		spellid = fields[0].GetUInt32();
		sp = dbcSpell.LookupEntryForced( spellid );
		if( sp != NULL )
			sp->ThreatForSpell = fields[1].GetUInt32();
		else
		{
			if(Config.MainConfig.GetBoolDefault("Server", "CleanDatabase", false))
			{
				WorldDatabase.Query( "DELETE FROM ai_threattospellid where spell = '%u'", spellid);
			}
			Log.Warning("AIThreatSpell", "Cannot apply to spell %u; spell is nonexistant.", spellid);
		}
		spellid = 0;

	} while( result->NextRow() );

	delete result;
}

Item* ObjectMgr::CreateItem(uint32 entry,Player* owner)
{
	ItemPrototype * proto = ItemPrototypeStorage.LookupEntry(entry);
	if(proto == 0) return NULL;

	if(proto->InventoryType == INVTYPE_BAG)
	{
		Container* pContainer(new Container(HIGHGUID_TYPE_CONTAINER,GenerateLowGuid(HIGHGUID_TYPE_CONTAINER)));
		pContainer->Create( entry, owner);
		pContainer->SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
		return pContainer;
	}
	else
	{
		Item* pItem(new Item(HIGHGUID_TYPE_ITEM,GenerateLowGuid(HIGHGUID_TYPE_ITEM)));
		pItem->Create(entry, owner);
		pItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
		return pItem;
	}
}

Item* ObjectMgr::LoadItem(uint64 guid)
{
	QueryResult * result = CharacterDatabase.Query("SELECT * FROM playeritems WHERE guid = %u", GUID_LOPART(guid));
	Item* pReturn = NULL;

	if(result)
	{
		ItemPrototype * pProto = ItemPrototypeStorage.LookupEntry(result->Fetch()[2].GetUInt32());
		if(!pProto)
			return NULL;

		if(pProto->InventoryType == INVTYPE_BAG)
		{
			Container* pContainer(new Container(HIGHGUID_TYPE_CONTAINER,(uint32)guid));
			pContainer->LoadFromDB(result->Fetch());
			pReturn = pContainer;
		}
		else
		{
			Item* pItem(new Item(HIGHGUID_TYPE_ITEM,(uint32)guid));
			pItem->LoadFromDB(result->Fetch(), NULL, false);
			pReturn = pItem;
		}
		delete result;
	}
	
	return pReturn;
}

void ObjectMgr::LoadCorpses(MapMgr* mgr)
{
	Corpse* pCorpse = NULL;

	QueryResult *result = CharacterDatabase.Query("SELECT * FROM corpses WHERE instanceId = %u", mgr->GetInstanceID());

	if(result)
	{
		do
		{
			Field *fields = result->Fetch();
			pCorpse = new Corpse(HIGHGUID_TYPE_CORPSE,fields[0].GetUInt32());
			pCorpse->Init();
			pCorpse->SetPosition(fields[1].GetFloat(), fields[2].GetFloat(), fields[3].GetFloat(), fields[4].GetFloat());
			pCorpse->SetZoneId(fields[5].GetUInt32());
			pCorpse->SetMapId(fields[6].GetUInt32());
			pCorpse->SetInstanceID(fields[7].GetUInt32());
			pCorpse->LoadValues( fields[8].GetString());
			if(pCorpse->GetUInt32Value(CORPSE_FIELD_DISPLAY_ID) == 0)
			{
				pCorpse->Destructor();
				continue;
			}

			pCorpse->PushToWorld(mgr);
		} while( result->NextRow() );

		delete result;
	}
}

std::list<ItemPrototype*>* ObjectMgr::GetListForItemSet(uint32 setid)
{
	return mItemSets[setid];
}

void ObjectMgr::CorpseAddEventDespawn(Corpse* pCorpse)
{
	if(!pCorpse->IsInWorld())
	{
		pCorpse->Destructor();
	}
	else
		sEventMgr.AddEvent(pCorpse->GetMapMgr(), &MapMgr::EventCorpseDespawn, pCorpse->GetGUID(), EVENT_CORPSE_DESPAWN, 600000, 1,0);
}

void ObjectMgr::DespawnCorpse(uint64 Guid)
{
	Corpse* pCorpse = objmgr.GetCorpse((uint32)Guid);
	if(pCorpse == 0)	// Already Deleted
		return;
	
	pCorpse->Despawn();
	pCorpse->Destructor();
}

void ObjectMgr::CorpseCollectorUnload()
{
	CorpseMap::const_iterator itr;
	_corpseslock.Acquire();
	for (itr = m_corpses.begin(); itr != m_corpses.end();)
	{
		Corpse* c =itr->second;
		++itr;
		if(c->IsInWorld())
			c->RemoveFromWorld(false);

		c->Destructor();
	}
	m_corpses.clear();
	_corpseslock.Release();
}

GossipMenu::GossipMenu(uint64 Creature_Guid, uint32 Text_Id) : TextId(Text_Id), CreatureGuid(Creature_Guid)
{

}

void GossipMenu::AddItem(uint8 Icon, const char* Text, int32 Id, bool Coded, uint32 BoxMoney, const char* BoxMessage)
{
	GossipMenuItem GossipItem;
	GossipItem.Id = (uint32)Menu.size();
	if(Id > 0)
		GossipItem.IntId = Id;
	else
		GossipItem.IntId = GossipItem.Id;		
	GossipItem.Icon = Icon;
	GossipItem.Coded = Coded;
	GossipItem.BoxMoney = BoxMoney;
	GossipItem.Text = Text;
	GossipItem.BoxMessage = BoxMessage;

	Menu.push_back(GossipItem);
}

void GossipMenu::AddItem(GossipMenuItem* GossipItem)
{
	Menu.push_back(*GossipItem);
}

void GossipMenu::BuildPacket(WorldPacket& Packet)
{
	Packet << uint64(CreatureGuid);
	Packet << uint32(0);			// some new menu type in 2.4?
	Packet << uint32(TextId);
	Packet << uint32(Menu.size());

	for(std::vector<GossipMenuItem>::iterator iter = Menu.begin();
		iter != Menu.end(); ++iter)
	{
		Packet << iter->Id;
		Packet << iter->Icon;
		Packet << iter->Coded;		// makes pop up box password
		Packet << iter->BoxMoney;	// money required to open menu, 2.0.3
		Packet << iter->Text;		// text for gossip item
		Packet << iter->BoxMessage;	// accept text (related to money) pop up box, 2.0.3
	}
}

void GossipMenu::SendTo(Player* Plr)
{
	WorldPacket data(SMSG_GOSSIP_MESSAGE, Menu.size() * 50 + 12);
	BuildPacket(data);
	data << uint32(0);  // 0 quests obviously
	Plr->GetSession()->SendPacket(&data);
}

void ObjectMgr::CreateGossipMenuForPlayer(GossipMenu** Location, uint64 Guid, uint32 TextID, Player* Plr)
{
	GossipMenu *Menu = new GossipMenu(Guid, TextID);
	ASSERT(Menu);

	if(Plr->CurrentGossipMenu != NULL)
		delete Plr->CurrentGossipMenu;

	Plr->CurrentGossipMenu = Menu;
	*Location = Menu;
}

GossipMenuItem GossipMenu::GetItem(uint32 Id)
{
	if(Id >= Menu.size())
	{
		GossipMenuItem k;
		k.IntId = 1;
		return k;
	} else {
		return Menu[Id];
	}
}

uint32 ObjectMgr::GetGossipTextForNpc(uint32 ID)
{
	return mNpcToGossipText[ID];
}

void ObjectMgr::LoadTrainers()
{
	QueryResult * result = WorldDatabase.Query("SELECT * FROM trainer_defs");
	QueryResult * result2;
	Field * fields2;
	const char * temp;
	size_t len;

	LoadDisabledSpells();

	if(!result)
		return;


	do 
	{
		Field * fields = result->Fetch();
		uint32 entry = fields[0].GetUInt32();
		CreatureInfo*trainer_info = CreatureNameStorage.LookupEntry(entry);
		if( trainer_info == NULL )
		{
			Log.Warning("Trainers", "NPC id for Trainer %u does not exist, skipping.", entry);
			continue;
		}

		Trainer * tr = new Trainer;
		tr->RequiredSkill = fields[1].GetUInt32();
		tr->RequiredSkillLine = fields[2].GetUInt32();
		tr->RequiredClass = fields[3].GetUInt32();
		tr->TrainerType = fields[4].GetUInt32();

		temp = fields[5].GetString();
		len=strlen(temp);
		if(!len)
		{
			temp = "What can I teach you $N?";
			len = strlen(temp);
		}
		tr->UIMessage = new char[len+1];
		strncpy(tr->UIMessage, temp, len);
		tr->UIMessage[len] = 0;

		uint32 tmptxtid[2];
		for( uint32 i = 0; i < 2; ++i)
		{
			tmptxtid[i] = fields[6+i].GetUInt32();
			if( tmptxtid[i] ) 
			{
				GossipText * text;
				text = NpcTextStorage.LookupEntry(tmptxtid[i]);
				if(text == 0)
				{
					if(Config.MainConfig.GetBoolDefault("Server", "CleanDatabase", false))
					{
						std::string columnname = (i == 0 ? "can_train_gossip_textid" : "cannot_train_gossip_textid");
						WorldDatabase.Query("UPDATE trainer_defs SET %s = '0' where entry = '%u'", columnname.c_str(), entry);
					}
					Log.Warning("Trainers", "Trainer %u contains an invalid npc_gossip_id %d.", entry, tmptxtid[i] );
					tmptxtid[i] = 0;
				}
			}
		}
		tr->Can_Train_Gossip_TextId = tmptxtid[0];
		tr->Cannot_Train_GossipTextId = tmptxtid[1];

		//now load the spells
		result2 = WorldDatabase.Query("SELECT * FROM trainer_spells where entry='%u'",entry);
		if(!result2)
		{
			Log.Error("LoadTrainers", "Trainer without spells, entry %u.", entry);
			if(tr->UIMessage)
				delete [] tr->UIMessage;
			delete tr;
			continue;
		}
		if(result2->GetFieldCount() != 10)
		{
			Log.LargeErrorMessage(LARGERRORMESSAGE_WARNING, "Trainers table format is invalid. Please update your database.");
			return;
		}
		else
		{
			do
			{
				fields2 = result2->Fetch();
				TrainerSpell ts;
				bool abrt=false;
				uint32 CastSpellID=fields2[1].GetUInt32();
				uint32 LearnSpellID=fields2[2].GetUInt32();

				ts.pCastSpell = NULL;
				ts.pLearnSpell = NULL;
				ts.pCastRealSpell = NULL;

				if( CastSpellID != 0 )
				{
					ts.pCastSpell = dbcSpell.LookupEntryForced( CastSpellID );
					if( ts.pCastSpell )
					{
						for(int k=0;k<3;k++)
						{
							if(ts.pCastSpell->Effect[k]==SPELL_EFFECT_LEARN_SPELL)
							{
								ts.pCastRealSpell = dbcSpell.LookupEntryForced(ts.pCastSpell->EffectTriggerSpell[k]);
								if( ts.pCastRealSpell == NULL )
								{
									Log.Warning("Trainers", "Trainer %u contains cast spell %u that is non-teaching", entry, CastSpellID);
									abrt=true;
								}
								break;
							}
						}
					}

					if(abrt)
						continue;
				}

				if( LearnSpellID != 0 )
				{
					ts.pLearnSpell = dbcSpell.LookupEntryForced( LearnSpellID );
				}

				if( ts.pCastSpell == NULL && ts.pLearnSpell == NULL )
				{
					if(Config.MainConfig.GetBoolDefault("Server", "CleanDatabase", false))
					{
						if(ts.pCastSpell == NULL)
							WorldDatabase.Query("DELETE FROM trainer_spells where entry='%u' AND learn_spell='%u'",entry, LearnSpellID);
						else
							WorldDatabase.Query("DELETE FROM trainer_spells where entry='%u' AND cast_spell='%u'",entry, CastSpellID);
					}
					Log.Warning("ObjectMgr", "Trainer %u skipped invalid spell (%u/%u).", entry, CastSpellID, LearnSpellID);
					continue; //omg a bad spell !
				}

				if( ts.pCastSpell && !ts.pCastRealSpell)
					continue;

				ts.Cost = fields2[3].GetUInt32();
				ts.RequiredSpell = fields2[4].GetUInt32();
				ts.RequiredSkillLine = fields2[5].GetUInt32();
				ts.RequiredSkillLineValue = fields2[6].GetUInt32();
				ts.RequiredLevel = fields2[7].GetUInt32();
				ts.DeleteSpell = fields2[8].GetUInt32();
				ts.IsProfession = (fields2[9].GetUInt32() != 0) ? true : false;
				tr->Spells.push_back(ts);
			}
			while(result2->NextRow());
			delete result2;

			tr->SpellCount = (uint32)tr->Spells.size();

			//and now we insert it to our lookup table
			if(!tr->SpellCount)
			{
				if(tr->UIMessage)
					delete [] tr->UIMessage;
				delete tr;
				continue;
			}

			mTrainers.insert( TrainerMap::value_type( entry, tr ) );
		}
		
	} while(result->NextRow());
	delete result;
	Log.Notice("ObjectMgr", "%u trainers loaded.", mTrainers.size());
}

Trainer* ObjectMgr::GetTrainer(uint32 Entry)
{
	TrainerMap::iterator iter = mTrainers.find(Entry);
	if(iter == mTrainers.end())
		return NULL;

	return iter->second;
}

void ObjectMgr::GenerateLevelUpInfo()
{
	// Generate levelup information for each class.
	PlayerCreateInfo * PCI;
	for(uint32 Class = WARRIOR; Class <= DRUID; ++Class)
	{
		// These are empty.
		if(Class == 10)
			continue;

		// Search for a playercreateinfo.
		for(uint32 Race = RACE_HUMAN; Race <= RACE_DRAENEI; ++Race )
		{
			PCI = GetPlayerCreateInfo(Race, Class);

			if(PCI == 0)
				continue;   // Class not valid for this race.

			// Generate each level's information
			uint32 MaxLevel = MAXIMUM_ATTAINABLE_LEVEL + 1;
			LevelInfo* lvl=new LevelInfo;
			lvl->HP = PCI->health;
			lvl->Mana = PCI->mana;
			lvl->Stat[0] = PCI->strength;
			lvl->Stat[1] = PCI->ability;
			lvl->Stat[2] = PCI->stamina;
			lvl->Stat[3] = PCI->intellect;
			lvl->Stat[4] = PCI->spirit;
			lvl->XPToNextLevel = 400;
			lvl->BaseHP = PCI->health;
			lvl->BaseMana = PCI->mana;
			LevelMap * lMap = new LevelMap;

			// Insert into map
			lMap->insert( LevelMap::value_type( 1, lvl ) );

			for(uint32 Level = 2; Level < MaxLevel; ++Level)
			{
				lvl = new LevelInfo;
				memset(lvl, 0, sizeof(LevelInfo));

				// Calculate Stats
				for(uint32 s = 0; s < 5; ++s)
				{
					lvl->Stat[s] = CalcStatForLevel( Level, Class, s );
					if( s == STAT_STRENGTH )
						lvl->Stat[s] += PCI->strength;
					if( s == STAT_INTELLECT )
						lvl->Stat[s] += PCI->intellect;
					if( s == STAT_STAMINA )
						lvl->Stat[s] += PCI->stamina;
					if( s == STAT_SPIRIT )
						lvl->Stat[s] += PCI->spirit;
					if( s == STAT_AGILITY )
						lvl->Stat[s] += PCI->ability;
				}

				// Apply HP/Mana
				uint32 HP = 0;
				if( lvl->Stat[STAT_STAMINA] > 20 )
				{
					HP = 20;
					HP += ( (lvl->Stat[STAT_STAMINA]-20) * 10);
				}
				else
					HP = lvl->Stat[STAT_STAMINA];

				uint32 Mana = 0;
				if( lvl->Stat[STAT_INTELLECT] > 20 )
				{
					Mana = 20;
					Mana += ((lvl->Stat[STAT_INTELLECT]-20) * 10);
				}
				else
					Mana = lvl->Stat[STAT_INTELLECT];

				uint32 BaseHP = 0;
				uint32 BaseMana = 0;

				// Description: We're calculating the Base Mana and HP that we get per level. These are based off of
				// the total value at level 80 and are probably incorrect at lower levels.
				// At a future date, we should attempt to correct them for those levels by decreasing the amount at
				// lower levels.
				// The first attempt at doing so is below.
				
				switch(Class)
				{
				case PRIEST:
					{
						BaseHP = uint32(87 * Level);
						BaseMana = uint32(48.2875f * Level);
						break;
					}
				case WARRIOR:
					{
						BaseHP = uint32(101.5125f * Level);
						break;
					}
				case DEATHKNIGHT:
					{
						BaseHP = uint32(101.5125f * Level);
						break;
					}
				case HUNTER:
					{
						BaseHP = uint32(91.55f * Level);
						BaseMana = uint32(63.075f * Level);
						break;
					}
				case ROGUE:
					{
						BaseHP = uint32(95.05f * Level);
						break;
					}
				case SHAMAN:
					{
						BaseHP = uint32(81.0625f * Level);
						BaseMana = uint32(54.95f * Level);
						break;
					}
				case DRUID:
					{
						BaseHP = uint32(92.7125f * Level);
						BaseMana = uint32(43.7f * Level);
						break;
					}
				case PALADIN:
					{
						BaseHP = uint32(86.675f * Level);
						BaseMana = uint32(54.925f * Level);
						break;
					}
				case MAGE:
					{
						BaseHP = uint32(87.0375f * Level);
						BaseMana = uint32(40.85f * Level);
						break;
					}
				case WARLOCK:
					{
						BaseHP = uint32(89.55f * Level);
						BaseMana = uint32(48.2f * Level);
						break;
					}
				}

				float perlevmod = 4.0f + (Level / 16.0f);

				BaseMana = uint32(BaseMana / (perlevmod - (Level / 10.0f)));
				BaseHP = uint32(BaseHP / (perlevmod - (Level / 10.0f)));

				lvl->HP = HP + BaseHP;
				lvl->Mana = Mana + BaseMana;
				lvl->BaseHP = BaseHP;
				lvl->BaseMana = BaseMana;

				// Calculate next level XP
				uint32 nextLvlXP = 0;

				//this is a fixed table taken from 2.3.0 wow. This can't get more blizzlike with the "if" cases ;)
				if( ( Level - 1 ) < MAX_PREDEFINED_NEXTLEVELXP )
				{
					nextLvlXP = NextLevelXp[( Level - 1 )];
				}
				else
				{
					// 2.2
					//double MXP = 45 + ( 5 * level );
					// 2.3
					double MXP = 235 + ( 5 * Level );
					double DIFF = Level < 29 ? 0.0 : Level < 30 ? 1.0 : Level < 31 ? 3.0 : Level < 32 ? 6.0 : 5.0 * ( double( Level ) - 30.0 );
					double XP = ( ( 8.0 * double( Level ) ) + DIFF ) * MXP;
					nextLvlXP = (int)( ( XP / 100.0 ) + 0.5 ) * 100;
				}

				lvl->XPToNextLevel = nextLvlXP;

				// Apply to map.
				lMap->insert( LevelMap::value_type( Level, lvl ) );
			}

			// Now that our level map is full, let's create the class/race pair
			pair<uint32, uint32> p;
			p.first = Race;
			p.second = Class;

			// Insert back into the main map.
			mLevelInfo.insert( LevelInfoMap::value_type( p, lMap ) );
		}
	}
	Log.Notice("ObjectMgr", "%u level up informations generated.", mLevelInfo.size());
}

LevelInfo* ObjectMgr::GetLevelInfo(uint32 Race, uint32 Class, uint32 Level)
{
	// Iterate levelinfo map until we find the right class+race.
	LevelInfoMap::iterator itr = mLevelInfo.begin();
	for(; itr != mLevelInfo.end(); ++itr )
	{
		if( itr->first.first == Race && itr->first.second == Class)
		{
			// We got a match.
			// Let's check that our level is valid first.
			if( Level > MAXIMUM_ATTAINABLE_LEVEL ) // too far.
				Level = MAXIMUM_ATTAINABLE_LEVEL;

			// Pull the level information from the second map.
			LevelMap::iterator it2 = itr->second->find( Level );
			ASSERT( it2 != itr->second->end() );

			return it2->second;
		}
	}

	return NULL;
}

uint32 ObjectMgr::GetPetSpellCooldown(uint32 SpellId)
{
	SpellEntry* sp = dbcSpell.LookupEntry( SpellId );
	if(sp)
	{
		uint32 pscd = ( sp->CategoryRecoveryTime == 0 ? sp->RecoveryTime : sp->CategoryRecoveryTime) +  (sp->StartRecoveryCategory == 0 ? sp->StartRecoveryTime : sp->StartRecoveryCategory);
		return pscd > PET_SPELL_SPAM_COOLDOWN ? pscd : PET_SPELL_SPAM_COOLDOWN;
	}
	Log.Error("ObjectMgr","GetPetSpellCooldown tried to add a non existing spell %d",SpellId);
	return 600000;//
}

void ObjectMgr::LoadSpellOverride()
{
//	int i = 0;
	std::stringstream query;
	QueryResult *result = WorldDatabase.Query( "SELECT DISTINCT overrideId FROM spelloverride" );

	if(!result)
	{
		return;
	}

//	int num = 0;
//	uint32 total =(uint32) result->GetRowCount();
	SpellEntry * sp;
	uint32 spellid;

	do
	{
		Field *fields = result->Fetch();
		query.rdbuf()->str("");
		query << "SELECT spellId FROM spelloverride WHERE overrideId = ";
		query << fields[0].GetUInt32();
		QueryResult *resultIn = WorldDatabase.Query(query.str().c_str());
		std::list<SpellEntry*>* list = new std::list<SpellEntry*>;
		if(resultIn)
		{
			do
			{
				Field *fieldsIn = resultIn->Fetch();
				spellid = fieldsIn[0].GetUInt32();
				sp = dbcSpell.LookupEntry(spellid);
				if(!spellid || !sp) 
					continue;
				list->push_back(sp);
			}while(resultIn->NextRow());
		}
		delete resultIn;
		if(list->size() == 0)
			delete list;
		else
			mOverrideIdMap.insert( OverrideIdMap::value_type( fields[0].GetUInt32(), list ));
	} while( result->NextRow() );
	delete result;
	Log.Notice("ObjectMgr", "%u spell overrides loaded.", mOverrideIdMap.size());
}

void ObjectMgr::SetVendorList(uint32 Entry, std::vector<CreatureItem>* list_)
{
	mVendors[Entry] = list_;
}


void ObjectMgr::LoadCreatureWaypoints()
{
	QueryResult *result = WorldDatabase.Query("SELECT * FROM creature_waypoints ORDER BY spawnid,waypointid");
	if(!result)
		return;
	QueryResult *result2 = WorldDatabase.Query("SELECT spawnid,COUNT(waypointid) FROM creature_waypoints GROUP BY spawnid ORDER BY spawnid,waypointid");
	if(!result2)
		return;
	uint32 lastspawnid = 0;

	Field * fields;
	Field * fields2;
	uint16 count = 0;
	uint32 skipid = 0;
	uint32 wpid = 0;


	do
	{
		fields = result->Fetch();
		uint32 spawnid=fields[0].GetUInt32();
		if(!spawnid || spawnid == skipid)
			continue;

		wpid = 0;
		wpid = fields[1].GetUInt32();
		if(!wpid)
		{
			Log.Warning("Waypoints","Waypoint can't be 0, counting starts at 1 (spawn %u)", spawnid);
			skipid = spawnid;
			continue;
		}
		WayPoint* wp = new WayPoint;
		if(spawnid != lastspawnid)
		{
			fields2 = result2->Fetch();
			count = fields2[1].GetUInt16();
			result2->NextRow();
		}
		lastspawnid = spawnid;

		wp->id = wpid;
		wp->x = fields[2].GetFloat();
		wp->y = fields[3].GetFloat();
		wp->z = fields[4].GetFloat();
		wp->o = fields[5].GetFloat();
		wp->waittime = fields[6].GetUInt32();
		wp->flags = fields[7].GetUInt32();
		wp->forwardemoteoneshot = fields[8].GetBool();
		wp->forwardemoteid = fields[9].GetUInt32();
		wp->backwardemoteoneshot = fields[10].GetBool();
		wp->backwardemoteid = fields[11].GetUInt32();
		wp->forwardskinid = fields[12].GetUInt32();
		wp->backwardskinid = fields[13].GetUInt32();
		wp->forwardStandState = fields[14].GetUInt32();
		wp->backwardStandState = fields[15].GetUInt32();
		wp->forwardSpellToCast = fields[16].GetUInt32();
		wp->backwardSpellToCast = fields[17].GetUInt32();
		wp->forwardSayText = fields[18].GetString();
		wp->backwardSayText = fields[19].GetString();
		wp->count = count;

		HM_NAMESPACE::hash_map<uint32,WayPointMap*>::const_iterator i;
		i=m_waypoints.find(spawnid);
		if(i==m_waypoints.end())
		{
			if(wp->id > 1)
			{
				Log.Warning("Waypoints","Waypoint id's can't start above 1 (spawn %u)", spawnid);
				if(spawnid)
					skipid = spawnid;
				delete wp;
				continue;
			}

			WayPointMap* m=new WayPointMap;
			if(m->size() < wp->id)
				m->resize(wp->count);

			(*m)[wp->id-1]=wp;
			//m->push_back(wp);
			m_waypoints[spawnid]=m;		
		}else
		{
			if(i->second->size() < wp->id)
			{
				Log.Warning("Waypoints","Waypoint id's are not continuous, skipping spawn (spawn %u)", spawnid);
				delete i->second;
				m_waypoints.erase(spawnid);
				skipid = spawnid;
				continue;
			}
			(*(i->second))[wp->id-1]=wp;
			//i->second->push_back(wp);
		}
	}while( result->NextRow() );

	Log.Notice("ObjectMgr", "%u waypoints cached.", result->GetRowCount());
	delete result;
	delete result2;
}

WayPointMap*ObjectMgr::GetWayPointMap(uint32 spawnid)
{
	HM_NAMESPACE::hash_map<uint32,WayPointMap*>::const_iterator i;
	i=m_waypoints.find(spawnid);
	if(i!=m_waypoints.end())
	{
		WayPointMap * m=i->second;
		// we don't wanna erase from the map, because some are used more
		// than once (for instances)

		//m_waypoints.erase(i);
		return m;
	}
	else return NULL;
}

Pet* ObjectMgr::CreatePet()
{
	uint32 guid;
	m_petlock.Acquire();
	guid =++m_hiPetGuid;
	m_petlock.Release();

	uint64 fullguid = ((uint64)HIGHGUID_TYPE_PET << 32) | ((uint64)guid << 24) | guid;
	Pet* pet = NULL;
	pet = new Pet(fullguid);
	pet->Init();
	return pet;
}

Player* ObjectMgr::CreatePlayer()
{
	uint32 guid;
	m_playerguidlock.Acquire();
	guid =++m_hiPlayerGuid;
	m_playerguidlock.Release();
	Player* p(new Player(guid));
	p->Init();
	return p;
}

void ObjectMgr::AddPlayer(Player* p)//add it to global storage
{
	_playerslock.AcquireWriteLock();
	_players[p->GetLowGUID()] = p;
	_playerslock.ReleaseWriteLock();
}

void ObjectMgr::RemovePlayer(Player* p)
{
	_playerslock.AcquireWriteLock();
	_players.erase(p->GetLowGUID());
	_playerslock.ReleaseWriteLock();

}

Corpse* ObjectMgr::CreateCorpse()
{
	uint32 guid;
	m_corpseguidlock.Acquire();
	guid =++m_hiCorpseGuid;
	m_corpseguidlock.Release();
	Corpse* pCorpse = new Corpse(HIGHGUID_TYPE_CORPSE,guid);
	pCorpse->Init();
	return pCorpse;
}

void ObjectMgr::AddCorpse(Corpse* p)//add it to global storage
{
	_corpseslock.Acquire();
	m_corpses[p->GetLowGUID()]=p;
	_corpseslock.Release();
}

void ObjectMgr::RemoveCorpse(Corpse* p)
{
	_corpseslock.Acquire();
	m_corpses.erase(p->GetLowGUID());
	_corpseslock.Release();
}

Corpse* ObjectMgr::GetCorpse(uint32 corpseguid)
{
	Corpse* rv;
	_corpseslock.Acquire();
	CorpseMap::const_iterator itr = m_corpses.find(corpseguid);
	rv = (itr != m_corpses.end()) ? itr->second : NULL;
	_corpseslock.Release();
	return rv;
}

Transporter* ObjectMgr::GetTransporter(uint32 guid)
{
	Transporter* rv;
	_TransportLock.Acquire();
	HM_NAMESPACE::hash_map<uint32, Transporter* >::const_iterator itr = mTransports.find(guid);
	rv = (itr != mTransports.end()) ? itr->second : NULL;
	_TransportLock.Release();
	return rv;
}

void ObjectMgr::AddTransport(Transporter* pTransporter)
{
	_TransportLock.Acquire();
	mTransports[pTransporter->GetUIdFromGUID()]=pTransporter;
 	_TransportLock.Release();
}

Transporter* ObjectMgr::GetTransporterByEntry(uint32 entry)
{
	Transporter* rv = NULL;
	_TransportLock.Acquire();
	HM_NAMESPACE::hash_map<uint32, Transporter* >::iterator itr = mTransports.begin();
	for(; itr != mTransports.end(); ++itr)
	{
		if(itr->second->GetEntry() == entry)
		{
			rv = itr->second;
			break;
		}
	}
	_TransportLock.Release();
	return rv;
}

void ObjectMgr::LoadGuildCharters()
{
	m_hiCharterId = 0;
	QueryResult * result = CharacterDatabase.Query("SELECT * FROM charters");
	if(!result) return;
	do 
	{
		Charter * c = new Charter(result->Fetch());
		m_charters[c->CharterType].insert(make_pair(c->GetID(), c));
		if(c->GetID() > m_hiCharterId)
			m_hiCharterId = c->GetID();
	} while(result->NextRow());
	delete result;
	Log.Notice("ObjectMgr", "%u charters loaded.", m_charters[0].size());
}

Charter * ObjectMgr::GetCharter(uint32 CharterId, CharterTypes Type)
{
	Charter * rv;
	HM_NAMESPACE::hash_map<uint32,Charter*>::iterator itr;
	m_charterLock.AcquireReadLock();
	itr = m_charters[Type].find(CharterId);
	rv = (itr == m_charters[Type].end()) ? 0 : itr->second;
	m_charterLock.ReleaseReadLock();
	return rv;
}

Charter * ObjectMgr::CreateCharter(uint32 LeaderGuid, CharterTypes Type)
{
	m_charterLock.AcquireWriteLock();
	Charter * c = new Charter(++m_hiCharterId, LeaderGuid, Type);
	m_charters[c->CharterType].insert(make_pair(c->GetID(), c));
	m_charterLock.ReleaseWriteLock();
	return c;
}

Charter::Charter(Field * fields)
{
	uint32 f = 0;
	CharterId = fields[f++].GetUInt32();
	CharterType = fields[f++].GetUInt32();
	LeaderGuid = fields[f++].GetUInt32();
	GuildName = fields[f++].GetString();
	ItemGuid = fields[f++].GetUInt64();
	SignatureCount = 0;
	Slots = GetNumberOfSlotsByType();
	Signatures = new uint32[Slots];

	for(uint32 i = 0; i < Slots; ++i)
	{
		Signatures[i] = fields[f++].GetUInt32();
		if(Signatures[i])
		{
			PlayerInfo * inf = objmgr.GetPlayerInfo(Signatures[i]);
			if( inf == NULL )
			{
				Signatures[i] = 0;
				continue;
			}

			inf->charterId[CharterType] = CharterId;
			++SignatureCount;
		}
	}
}

void Charter::AddSignature(uint32 PlayerGuid)
{
	if(SignatureCount >= Slots)
		return;

	SignatureCount++;
	uint32 i;
	for(i = 0; i < Slots; ++i)
	{
		if(Signatures[i] == 0)
		{
			Signatures[i] = PlayerGuid;
			break;
		}
	}

	assert(i != Slots);
}

void Charter::RemoveSignature(uint32 PlayerGuid)
{
	for(uint32 i = 0; i < Slots; ++i)
	{
		if(Signatures[i] == PlayerGuid)
		{
			Signatures[i] = 0;
			SignatureCount--;
			SaveToDB();
			break;
		}
	}
}


void Charter::Destroy()
{
	if( Slots == 0 )			// ugly hack because of f*cked memory
		return;

	//meh remove from objmgr
	objmgr.RemoveCharter(this);
	// Kill the players with this (in db/offline)
	CharacterDatabase.Execute( "DELETE FROM charters WHERE charterId = %u", CharterId );
	PlayerInfo * p;
	for( uint32 i = 0; i < Slots; ++i )
	{
		if(!Signatures[i])
			continue;

		p = objmgr.GetPlayerInfo(Signatures[i]);
		if( p != NULL )
			p->charterId[CharterType] = 0;
	}

	// click, click, boom!
	delete this;
}

void Charter::SaveToDB()
{
	std::stringstream ss;
	uint32 i;
	ss << "REPLACE INTO charters VALUES(" << CharterId << "," << CharterType << "," << LeaderGuid << ",'" << GuildName << "'," << ItemGuid;

	for(i = 0; i < Slots; ++i)
		ss << "," << Signatures[i];

	for(; i < 9; ++i)
		ss << ",0";

	ss << ")";
	CharacterDatabase.Execute(ss.str().c_str());
}

Charter * ObjectMgr::GetCharterByItemGuid(uint64 guid)
{
	m_charterLock.AcquireReadLock();
	for(int i = 0; i < NUM_CHARTER_TYPES; ++i)
	{
		HM_NAMESPACE::hash_map<uint32, Charter*>::iterator itr = m_charters[i].begin();
		for(; itr != m_charters[i].end(); ++itr)
		{
			if(itr->second->ItemGuid == guid)
			{
				m_charterLock.ReleaseReadLock();
				return itr->second;
			}
		}
	}
	m_charterLock.ReleaseReadLock();
	return NULL;
}

Charter * ObjectMgr::GetCharterByGuid(uint64 playerguid, CharterTypes type)
{
	m_charterLock.AcquireReadLock();
	for(int i = 0; i < NUM_CHARTER_TYPES; ++i)
	{
		HM_NAMESPACE::hash_map<uint32, Charter*>::iterator itr = m_charters[i].begin();
		for(; itr != m_charters[i].end(); ++itr)
		{
			if(playerguid == itr->second->LeaderGuid)
			{
				m_charterLock.ReleaseReadLock();
				return itr->second;
			}

			for(uint32 j = 0; j < itr->second->SignatureCount; ++j)
			{
				if(itr->second->Signatures[j] == playerguid)
				{
					m_charterLock.ReleaseReadLock();
					return itr->second;
				}
			}
		}
	}
	m_charterLock.ReleaseReadLock();
	return NULL;
}

Charter * ObjectMgr::GetCharterByName(string &charter_name, CharterTypes Type)
{
	Charter * rv = 0;
	m_charterLock.AcquireReadLock();
	HM_NAMESPACE::hash_map<uint32, Charter*>::iterator itr = m_charters[Type].begin();
	for(; itr != m_charters[Type].end(); ++itr)
	{
		if(itr->second->GuildName == charter_name)
		{
			rv = itr->second;
			break;
		}
	}

	m_charterLock.ReleaseReadLock();
	return rv;
}

void ObjectMgr::RemoveCharter(Charter * c)
{
	if( c == NULL )
		return;
	if( c->CharterType > NUM_CHARTER_TYPES )
	{
		Log.Notice("ObjectMgr", "Charter %u cannot be destroyed as type %u is not a sane type value.", c->CharterId, c->CharterType );
		return;
	}
	m_charterLock.AcquireWriteLock();
	m_charters[c->CharterType].erase(c->CharterId);
	m_charterLock.ReleaseWriteLock();
}

void ObjectMgr::LoadReputationModifierTable(const char * tablename, ReputationModMap * dmap)
{
	QueryResult * result = WorldDatabase.Query("SELECT * FROM %s", tablename);
	ReputationModMap::iterator itr;
	ReputationModifier * modifier;
	ReputationMod mod;

	if(result)
	{
		do 
		{
			mod.faction[0] = result->Fetch()[1].GetUInt32();
			mod.faction[1] = result->Fetch()[2].GetUInt32();
			mod.value = result->Fetch()[3].GetInt32();
			mod.replimit = result->Fetch()[4].GetUInt32();

			itr = dmap->find(result->Fetch()[0].GetUInt32());
			if(itr == dmap->end())
			{
				modifier = new ReputationModifier;
				modifier->entry = result->Fetch()[0].GetUInt32();
				modifier->mods.push_back(mod);
				dmap->insert( ReputationModMap::value_type( result->Fetch()[0].GetUInt32(), modifier ) );
			}
			else
			{
				itr->second->mods.push_back(mod);
			}
		} while(result->NextRow());
		delete result;
	}
	Log.Notice("ObjectMgr", "%u reputation modifiers on %s.", dmap->size(), tablename);
}

void ObjectMgr::LoadReputationModifiers()
{
	LoadReputationModifierTable("reputation_creature_onkill", &m_reputation_creature);
	LoadReputationModifierTable("reputation_faction_onkill", &m_reputation_faction);
	LoadInstanceReputationModifiers();
}

ReputationModifier * ObjectMgr::GetReputationModifier(uint32 entry_id, uint32 faction_id)
{
	// first, try fetching from the creature table (by faction is a fallback)
	ReputationModMap::iterator itr = m_reputation_creature.find(entry_id);
	if(itr != m_reputation_creature.end())
		return itr->second;

	// fetch from the faction table
	itr = m_reputation_faction.find(faction_id);
	if(itr != m_reputation_faction.end())
		return itr->second;

	// no data. fallback to default -5 value.
	return 0;
}

void ObjectMgr::LoadMonsterSay()
{
	QueryResult * result = WorldDatabase.Query("SELECT * FROM npc_monstersay");
	if(!result) return;

	uint32 Entry, Event;
	Field * fields = result->Fetch();
	do 
	{
		Entry = fields[0].GetUInt32();
		Event = fields[1].GetUInt32();

		if(Event >= NUM_MONSTER_SAY_EVENTS)
			continue;

		if( mMonsterSays[Event].find( Entry ) != mMonsterSays[Event].end() )
		{
			sLog.outError("Duplicate monstersay event %u for entry %u, skipping", Event, Entry	);
			continue;
		}	

		NpcMonsterSay * ms = new NpcMonsterSay;
		ms->Chance = fields[2].GetFloat();
		ms->Language = fields[3].GetUInt32();
		ms->Type = fields[4].GetUInt32();
		ms->MonsterName = fields[5].GetString() ? strdup(fields[5].GetString()) : strdup("None");

		char * texts[5];
		char * text;
		uint32 textcount = 0;

		for(uint32 i = 0; i < 5; ++i)
		{
			text = (char*)fields[6+i].GetString();
			if(!text) continue;
			if(strlen(fields[6+i].GetString()) < 5)
				continue;

			texts[textcount] = strdup(fields[6+i].GetString());

			// check for ;
			if(texts[textcount][strlen(texts[textcount])-1] == ';')
				texts[textcount][strlen(texts[textcount])-1] = 0;

			++textcount;
		}

		if(!textcount)
		{
			free(((char*)ms->MonsterName));
			delete ms;
			continue;
		}

		ms->Texts = new const char*[textcount];
		memcpy(ms->Texts, texts, sizeof(char*) * textcount);
		ms->TextCount = textcount;

		mMonsterSays[Event].insert( make_pair( Entry, ms ) );

	} while(result->NextRow());
	Log.Notice("ObjectMgr", "%u monster say events loaded.", result->GetRowCount());
	delete result;
}

void ObjectMgr::HandleMonsterSayEvent(Creature* pCreature, MONSTER_SAY_EVENTS Event)
{
	MonsterSayMap::iterator itr = mMonsterSays[Event].find(pCreature->GetEntry());
	if(itr == mMonsterSays[Event].end())
		return;

	NpcMonsterSay * ms = itr->second;
	if(Rand(ms->Chance))
	{
		// chance successful.
		int choice = (ms->TextCount == 1) ? 0 : RandomUInt(ms->TextCount - 1);
		const char * text = ms->Texts[choice];
		pCreature->SendChatMessage(ms->Type, ms->Language, text);
	}
}

bool ObjectMgr::HasMonsterSay(uint32 Entry, MONSTER_SAY_EVENTS Event)
{
	MonsterSayMap::iterator itr = mMonsterSays[Event].find(Entry);
	if(itr == mMonsterSays[Event].end())
		return false;

	return true;
}

void ObjectMgr::LoadInstanceReputationModifiers()
{
	QueryResult * result = WorldDatabase.Query("SELECT * FROM reputation_instance_onkill");
	if(!result) return;

	do 
	{
		Field * fields = result->Fetch();
		InstanceReputationMod mod;
		mod.mapid = fields[0].GetUInt32();
		mod.mob_rep_reward = fields[1].GetInt32();
		mod.mob_rep_reward_heroic = fields[2].GetInt32();
		mod.mob_rep_limit = fields[3].GetUInt32();
		mod.mob_rep_limit_heroic = fields[4].GetUInt32();
		mod.boss_rep_reward = fields[5].GetInt32();
		mod.boss_rep_reward_heroic = fields[6].GetInt32();
		mod.boss_rep_limit = fields[7].GetUInt32();
		mod.boss_rep_limit_heroic = fields[8].GetUInt32();
		mod.faction[0] = fields[9].GetUInt32();
		mod.faction[1] = fields[10].GetUInt32();
		HM_NAMESPACE::hash_map<uint32, InstanceReputationModifier*>::iterator itr = m_reputation_instance.find(mod.mapid);
		if(itr == m_reputation_instance.end())
		{
			InstanceReputationModifier * m = new InstanceReputationModifier;
			m->mapid = mod.mapid;
			m->mods.push_back(mod);
			m_reputation_instance.insert( make_pair( m->mapid, m ) );
		}
		else
			itr->second->mods.push_back(mod);

	} while(result->NextRow());
	delete result;
	Log.Notice("ObjectMgr", "%u instance reputation modifiers loaded.", m_reputation_instance.size());
}

bool ObjectMgr::HandleInstanceReputationModifiers(Player* pPlayer, Unit* pVictim)
{
	uint32 team = pPlayer->GetTeam();
	bool is_boss, is_heroic;
	if(pVictim->GetTypeId() != TYPEID_UNIT)
		return false;

	HM_NAMESPACE::hash_map<uint32, InstanceReputationModifier*>::iterator itr = m_reputation_instance.find(pVictim->GetMapId());
	if(itr == m_reputation_instance.end())
		return false;

	is_boss = (TO_CREATURE( pVictim )->proto && TO_CREATURE( pVictim )->proto->boss) ? true : false;
	is_heroic = (pPlayer->IsInWorld() && pPlayer->iInstanceType == MODE_HEROIC && pPlayer->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL) ? true : false;

	// Apply the bonuses as normal.
	int32 replimit;
	int32 value;

	for(vector<InstanceReputationMod>::iterator i = itr->second->mods.begin(); i !=  itr->second->mods.end(); ++i)
	{
		if(!(*i).faction[team])
			continue;

		if(is_boss)
		{
			value = is_heroic ? i->boss_rep_reward_heroic : i->boss_rep_reward;
			replimit = is_heroic ? i->boss_rep_limit_heroic : i->boss_rep_limit;
		}
		else
		{
			value =  is_heroic ? i->mob_rep_reward_heroic : i->mob_rep_reward;
			replimit = is_heroic ? i->mob_rep_limit_heroic : i->mob_rep_limit;
		}

		if(!value || (replimit && pPlayer->GetStanding(i->faction[team]) >= replimit))
			continue;

		//value *= sWorld.getRate(RATE_KILLREPUTATION);
		value = float2int32(float(value) * sWorld.getRate(RATE_KILLREPUTATION));
		pPlayer->ModStanding(i->faction[team], value);
	}

	return true;
}

void ObjectMgr::LoadDisabledSpells()
{
	QueryResult * result = WorldDatabase.Query("SELECT * FROM spell_disable");
	if(result)
	{
		do 
		{
			m_disabled_spells.insert( result->Fetch()[0].GetUInt32() );
		} while(result->NextRow());
		delete result;
	}

	Log.Notice("ObjectMgr", "%u disabled spells.", m_disabled_spells.size());
}

void ObjectMgr::ReloadDisabledSpells()
{
	m_disabled_spells.clear();
	LoadDisabledSpells();
}

void ObjectMgr::LoadGroups()
{
	QueryResult * result = CharacterDatabase.Query("SELECT * FROM groups");
	if(result)
	{
		do 
		{
			Group * g = new Group(false);
			g->LoadFromDB(result->Fetch());
		} while(result->NextRow());
		delete result;
	}

	Log.Notice("ObjectMgr", "%u groups loaded.", this->m_groups.size());
}

void ObjectMgr::LoadArenaTeams()
{
	QueryResult * result = CharacterDatabase.Query("SELECT * FROM arenateams");
	if(result != NULL)
	{
		do 
		{
			ArenaTeam * team = new ArenaTeam(result->Fetch());
			AddArenaTeam(team);
			if(team->m_id > m_hiArenaTeamId)
				m_hiArenaTeamId=team->m_id;

		} while(result->NextRow());
		delete result;
	}

	/* update the ranking */
	UpdateArenaTeamRankings();
}

ArenaTeam * ObjectMgr::GetArenaTeamById(uint32 id)
{
	HM_NAMESPACE::hash_map<uint32, ArenaTeam*>::iterator itr;
	m_arenaTeamLock.Acquire();
	itr = m_arenaTeams.find(id);
	m_arenaTeamLock.Release();
	return (itr == m_arenaTeams.end()) ? NULL : itr->second;
}

ArenaTeam * ObjectMgr::GetArenaTeamByName(string & name, uint32 Type)
{
	m_arenaTeamLock.Acquire();
	for(HM_NAMESPACE::hash_map<uint32, ArenaTeam*>::iterator itr = m_arenaTeams.begin(); itr != m_arenaTeams.end(); ++itr)
	{
		if(!strnicmp(itr->second->m_name.c_str(), name.c_str(), name.size()))
		{
			m_arenaTeamLock.Release();
			return itr->second;
		}
	}
	m_arenaTeamLock.Release();
	return NULL;
}

void ObjectMgr::RemoveArenaTeam(ArenaTeam * team)
{
	m_arenaTeamLock.Acquire();
	m_arenaTeams.erase(team->m_id);
	m_arenaTeamMap[team->m_type].erase(team->m_id);
	m_arenaTeamLock.Release();
}

void ObjectMgr::AddArenaTeam(ArenaTeam * team)
{
	m_arenaTeamLock.Acquire();
	m_arenaTeams[team->m_id] = team;
	m_arenaTeamMap[team->m_type].insert(make_pair(team->m_id,team));
	m_arenaTeamLock.Release();
}

class ArenaSorter
{
public:
	bool operator()(ArenaTeam* const & a,ArenaTeam* const & b)
	{
		return (a->m_stat_rating > b->m_stat_rating);
	}
		bool operator()(ArenaTeam*& a, ArenaTeam*& b)
		{
				return (a->m_stat_rating > b->m_stat_rating);
		}
};

void ObjectMgr::UpdateArenaTeamRankings()
{
	m_arenaTeamLock.Acquire();
	for(uint32 i = 0; i < NUM_ARENA_TEAM_TYPES; ++i)
	{
		vector<ArenaTeam*> ranking;
		
		for(HM_NAMESPACE::hash_map<uint32,ArenaTeam*>::iterator itr = m_arenaTeamMap[i].begin(); itr != m_arenaTeamMap[i].end(); ++itr)
			ranking.push_back(itr->second);

		std::sort(ranking.begin(), ranking.end(), ArenaSorter());
		uint32 rank = 1;
		for(vector<ArenaTeam*>::iterator itr = ranking.begin(); itr != ranking.end(); ++itr)
		{
			if((*itr)->m_stat_ranking != rank)
			{
				(*itr)->m_stat_ranking = rank;
				(*itr)->SaveToDB();
			}
			++rank;
		}
	}
	m_arenaTeamLock.Release();
}

void ObjectMgr::UpdateArenaTeamWeekly()
{	// reset weekly matches count for all teams and all members
	m_arenaTeamLock.Acquire();
	for(uint32 i = 0; i < NUM_ARENA_TEAM_TYPES; ++i)
	{
		for(HM_NAMESPACE::hash_map<uint32,ArenaTeam*>::iterator itr = m_arenaTeamMap[i].begin(); itr != m_arenaTeamMap[i].end(); ++itr)
		{
			ArenaTeam *team = itr->second;
			if(team)
			{
				team->m_stat_gamesplayedweek = 0;
				team->m_stat_gameswonweek = 0;
				for(uint32 j = 0; j < team->m_memberCount; ++j)
				{
					team->m_members[j].Played_ThisWeek = 0;
					team->m_members[j].Won_ThisWeek = 0;
				}
				team->SaveToDB();
			}
		}
	}
	m_arenaTeamLock.Release();
}

void ObjectMgr::ResetDailies()
{
	_playerslock.AcquireReadLock();
	PlayerStorageMap::iterator itr = _players.begin();
	for(; itr != _players.end(); itr++)
	{
		Player* pPlayer = itr->second;
		uint8 eflags = 0;
		if( pPlayer->IsInWorld() )
			eflags = EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT;
		sEventMgr.AddEvent(pPlayer, &Player::ResetDailyQuests, EVENT_PLAYER_UPDATE, 100, 0, eflags);
	}
	_playerslock.ReleaseReadLock();
}

#ifdef VOICE_CHAT
void ObjectMgr::GroupVoiceReconnected()
{
	m_groupLock.AcquireReadLock();
	for(GroupMap::iterator itr = m_groups.begin(); itr != m_groups.end(); ++itr)
		itr->second->VoiceSessionReconnected();
	m_groupLock.ReleaseReadLock();
}
#endif

void ObjectMgr::LoadPetLevelupSpellMap()
{
	CreatureFamilyEntry	* creatureFamily;
	SpellEntry * sp;
	skilllinespell * sk;
	uint32 count = 0;

//	static uint32 FamilySkillArray[]	=	{[FAMILY_BAT,SKILL_PET_BAT],[FAMILY_BEAR,SKILL_PET_BEAR],[FAMILY_BIRD_OF_PREY,KILL_PET_OWL],[FAMILY_BOAR,SKILL_PET_BOAR],[FAMILY_CARRION_BIRD,SKILL_PET_CARRION_BIRD],[FAMILY_CAT,SKILL_PET_CAT],[FAMILY_CHIMAERA,SKILL_PET_EXOTIC_CHIMAERA],[FAMILY_CORE_HOUND.SKILL_PET_EXOTIC_CORE_HOUND],[FAMILY_CRAB,SKILL_PET_CRAB],[FAMILY_CROCILISK,SKILL_PET_CROCILISK],[FAMILY_DEVILSAUR,SKILL_PET_EXOTIC_DEVILSAUR],[FAMILY_DRAGONHAWK,SKILL_PET_DRAGONHAWK],[FAMILY_GORILLA,SKILL_PET_GORILLA],[FAMILY_HYENA,SKILL_PET_HYENA],[FAMILY_MOTH,SKILL_PET_MOTH],[FAMILY_NETHER_RAY,SKILL_PET_NETHER_RAY],[FAMILY_RAPTOR,SKILL_PET_RAPTOR],[FAMILY_RAVAGER,SKILL_PET_RAVAGER],[FAMILY_RHINO,SKILL_PET_EXOTIC_RHINO],[FAMILY_SCORPID,SKILL_PET_SCORPID],[FAMILY_SERPENT,SKILL_PET_SERPENT],[FAMILY_SILITHID,SKILL_PET_EXOTIC_SILITHID],[FAMILY_SPIDER,SKILL_PET_SPIDER],[FAMILY_SPIRIT_BEAST,SKILL_PET_EXOTIC_SPIRIT_BEAST],[FAMILY_SPOREBAT,SKILL_PET_SPOREBAT],[FAMILY_TALLSTRIDER,SKILL_PET_TALLSTRIDER],[FAMILY_TURTLE,SKILL_PET_TURTLE],[FAMILY_WARP_STALKER,SKILL_PET_WARP_STALKER],[FAMILY_WASP,SKILL_PET_WASP],[FAMILY_WIND_SERPENT,SKILL_PET_WIND_SERPENT],[FAMILY_WOLF,SKILL_PET_WOLF],[FAMILY_WORM,SKILL_PET_EXOTIC_WORM]};

	for	(uint32	i =	0; i < dbcCreatureFamily.GetNumRows(); ++i)
	{
		//Valid hunter pet family?
		creatureFamily = dbcCreatureFamily.LookupEntry(i);
		if(!creatureFamily || creatureFamily->pettalenttype < 0)
			continue;

		for(uint32 j = 0; j < dbcSkillLineSpell.GetNumRows(); ++j)
		{
			//Valid skill line?
			sk = dbcSkillLineSpell.LookupEntry(j);
			if(!sk || sk->racemask != 0 || sk->classmask != 0)
				continue;

			//Vaild pet-family spell?
			sp = dbcSpell.LookupEntry(sk->spell);
			if(!sp || sp->SpellFamilyName != SPELLFAMILY_HUNTER)
				continue;

			switch(sk->skilline)
			{
				case SKILL_PET_IMP:
				{
					mPetLevelupSpellMap[FAMILY_FAKE_IMP][sp->spellLevel] = sk->spell;
				}break;
				case SKILL_PET_VOIDWALKER:
				{
					mPetLevelupSpellMap[FAMILY_FAKE_VOIDWALKER][sp->spellLevel] = sk->spell;
				}break;
				case SKILL_PET_SUCCUBUS:
				{
					mPetLevelupSpellMap[FAMILY_FAKE_SUCCUBUS][sp->spellLevel] = sk->spell;
				}break;
				case SKILL_PET_FELHUNTER:
				{
					mPetLevelupSpellMap[FAMILY_FAKE_FELHUNTER][sp->spellLevel] = sk->spell;
				}break;
				case SKILL_PET_FELGUARD:
				{
					mPetLevelupSpellMap[FAMILY_FAKE_FELGUARD][sp->spellLevel] = sk->spell;
				}break;
				case SPELL_HASH_GROWL:
				{
					mPetLevelupSpellMap[creatureFamily->ID][sp->spellLevel] = sk->spell;
				}break;
				case SPELL_HASH_COWER:
				{
					mPetLevelupSpellMap[creatureFamily->ID][sp->spellLevel] = sk->spell;
				}break;
			}
			switch(creatureFamily->ID)
			{
				case FAMILY_BAT:
				{
					if(sk->skilline !=	SKILL_PET_BAT)
						continue;
				}break;
				case FAMILY_BEAR:
				{
					if(sk->skilline !=	SKILL_PET_BEAR)
						continue;
				}break;
				case FAMILY_BIRD_OF_PREY:
				{
					if(sk->skilline !=	SKILL_PET_OWL)
						continue;
				}break;
				case FAMILY_BOAR:	
				{
					if(sk->skilline	!= SKILL_PET_BOAR)
						continue;
				}break;
				case FAMILY_CARRION_BIRD:
				{
					if(sk->skilline !=	SKILL_PET_CARRION_BIRD)
						continue;
				}break;
				case FAMILY_CAT:
				{
					if(sk->skilline !=	SKILL_PET_CAT)
						continue;
				}break;
				case FAMILY_CHIMAERA:
				{
					if(sk->skilline !=	SKILL_PET_EXOTIC_CHIMAERA)
						continue;
				}break;
				case FAMILY_CORE_HOUND:
				{
					if(sk->skilline !=	SKILL_PET_EXOTIC_CORE_HOUND)
						continue;
				}break;
				case FAMILY_CRAB:
				{
					if(sk->skilline !=	SKILL_PET_CRAB)
						continue;
				}break;
				case FAMILY_CROCILISK:
				{
					if(sk->skilline !=	SKILL_PET_CROCILISK)
						continue;
				}break;
				case FAMILY_DEVILSAUR:
				{
					if(sk->skilline !=	SKILL_PET_EXOTIC_DEVILSAUR)
						continue;
				}break;
				case FAMILY_DRAGONHAWK:
				{
					if(sk->skilline !=	SKILL_PET_DRAGONHAWK)
						continue;
				}break;
				case FAMILY_GORILLA:
				{
					if(sk->skilline !=	SKILL_PET_GORILLA)
						continue;
				}break;
				case FAMILY_HYENA:
				{
					if(sk->skilline !=	SKILL_PET_HYENA)
						continue;
				}break;
				case FAMILY_MOTH:
				{
					if(sk->skilline !=	SKILL_PET_MOTH)
						continue;
				}break;
				case FAMILY_NETHER_RAY:
				{
					if(sk->skilline !=	SKILL_PET_NETHER_RAY)
						continue;
				}break;
				case FAMILY_RAPTOR:
				{
					if(sk->skilline !=	SKILL_PET_RAPTOR)
						continue;
				}break;
				case FAMILY_RAVAGER:
				{
					if(sk->skilline !=	SKILL_PET_RAVAGER)
						continue;
				}break;
				case FAMILY_RHINO:
				{
					if(sk->skilline !=	SKILL_PET_EXOTIC_RHINO)
						continue;
				}break;
				case FAMILY_SCORPID:
				{
					if(sk->skilline !=	SKILL_PET_SCORPID)
						continue;
				}break;
				case FAMILY_SERPENT:
				{
					if(sk->skilline !=	SKILL_PET_SERPENT)
						continue;
				}break;
				case FAMILY_SILITHID:
				{
					if(sk->skilline !=	SKILL_PET_EXOTIC_SILITHID)
						continue;
				}break;
				case FAMILY_SPIDER:
				{
					if(sk->skilline !=	SKILL_PET_SPIDER)
						continue;
				}break;
				case FAMILY_SPIRIT_BEAST:
				{
					if(sk->skilline !=	SKILL_PET_EXOTIC_SPIRIT_BEAST)
						continue;
				}break;
				case FAMILY_SPOREBAT:
				{
					if(sk->skilline !=	SKILL_PET_SPOREBAT)
						continue;
				}break;
				case FAMILY_TALLSTRIDER:
				{
					if(sk->skilline !=	SKILL_PET_TALLSTRIDER)
						continue;
				}break;
				case FAMILY_TURTLE:
				{
					if(sk->skilline !=	SKILL_PET_TURTLE)
						continue;
				}break;
				case FAMILY_WARP_STALKER:
				{
					if(sk->skilline !=	SKILL_PET_WARP_STALKER)
						continue;
				}break;
				case FAMILY_WASP:
				{
					if(sk->skilline !=	SKILL_PET_WASP)
						continue;
				}break;
				case FAMILY_WIND_SERPENT:
				{
					if(sk->skilline !=	SKILL_PET_WIND_SERPENT)
						continue;
				}break;
				case FAMILY_WOLF:
				{
					if(sk->skilline !=	SKILL_PET_WOLF)
						continue;
				}break;
				case FAMILY_WORM:
				{
					if(sk->skilline !=	SKILL_PET_EXOTIC_WORM)
						continue;
				}break;
				default:
				{
					Log.Error("ObjectMgr",	"Unhandled creature	family %u",	creatureFamily->ID);
				}break;
			}
			mPetLevelupSpellMap[creatureFamily->ID][sp->spellLevel]	=	sk->spell;
			count++;
		}
	}
	Log.Notice("ObjectMgr", "%u Pet LevelUp Spells loaded.",	count);
}

PetLevelupSpellSet const* ObjectMgr::GetPetLevelupSpellList(uint32 petFamily)	const
{
	PetLevelupSpellMap::const_iterator itr = mPetLevelupSpellMap.find(petFamily);
	if(itr != mPetLevelupSpellMap.end())
		return &itr->second;
	return NULL;
}
