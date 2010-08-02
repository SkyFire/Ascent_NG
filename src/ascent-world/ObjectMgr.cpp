/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"
initialiseSingleton( ObjectMgr );

const char * NormalTalkMessage = "What can I teach you, $N?";

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

	Log.Notice("ObjectMgr", "Deleting Spell Override...");
	for(OverrideIdMap::iterator i = mOverrideIdMap.begin(); i != mOverrideIdMap.end(); ++i)
	{
		delete i->second;
	}

	Log.Notice("ObjectMgr", "Deleting Trainers...");
	for( TrainerMap::iterator i = mTrainers.begin( ); i != mTrainers.end( ); ++ i) {
		Trainer * t = i->second;
		if(t->UIMessage && t->UIMessage != (char*)NormalTalkMessage)
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
		pPlayerCreateInfo->attackpower = fields[20].GetUInt32();
		pPlayerCreateInfo->mindmg = fields[21].GetFloat();
		pPlayerCreateInfo->maxdmg = fields[22].GetFloat();

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
	Corpse *pCorpse;
	QueryResult *result = CharacterDatabase.Query("SELECT * FROM Corpses WHERE guid =%u ", guid );

	if( !result )
		return NULL;
	
	do
	{
		Field *fields = result->Fetch();
		pCorpse = new Corpse(HIGHGUID_TYPE_CORPSE,fields[0].GetUInt32());
		pCorpse->SetPosition(fields[1].GetFloat(), fields[2].GetFloat(), fields[3].GetFloat(), fields[4].GetFloat());
		pCorpse->SetZoneId(fields[5].GetUInt32());
		pCorpse->SetMapId(fields[6].GetUInt32());
		pCorpse->LoadValues( fields[7].GetString());
		if(pCorpse->GetUInt32Value(CORPSE_FIELD_DISPLAY_ID) == 0)
		{
			delete pCorpse;
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
Corpse *ObjectMgr::GetCorpseByOwner(uint32 ownerguid)
{
	CorpseMap::const_iterator itr;
	Corpse *rv = NULL;
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

void ObjectMgr::DelinkPlayerCorpses(Player *pOwner)
{
	//dupe protection agaisnt crashs
	Corpse * c;
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
	Player * rv = NULL;
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
	Player * rv;
	
	_playerslock.AcquireReadLock();	
	PlayerStorageMap::const_iterator itr = _players.find(guid);
	rv = (itr != _players.end()) ? itr->second : 0;
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
					printf("Item %u at vendor %u has extended cost %u which is invalid. Skipping.\n", itm.itemid, fields[0].GetUInt32(), ec);
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

	SpellEntry * sp;

	do
	{
		Field *fields = result->Fetch();
		sp = dbcSpell.LookupEntryForced( fields[0].GetUInt32() );
		if( sp != NULL )
			sp->ThreatForSpell = fields[1].GetUInt32();
		else
			Log.Warning("AIThreatSpell", "Cannot apply to spell %u; spell is nonexistant.", fields[0].GetUInt32());

	} while( result->NextRow() );

	delete result;
}

Item * ObjectMgr::CreateItem(uint32 entry,Player * owner)
{
	ItemPrototype * proto = ItemPrototypeStorage.LookupEntry(entry);
	if(proto == 0) return 0;

	if(proto->InventoryType == INVTYPE_BAG)
	{
		Container * pContainer = new Container(HIGHGUID_TYPE_CONTAINER,GenerateLowGuid(HIGHGUID_TYPE_CONTAINER));
		pContainer->Create( entry, owner);
		pContainer->SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
		return pContainer;
	}
	else
	{
		Item * pItem = new Item(HIGHGUID_TYPE_ITEM,GenerateLowGuid(HIGHGUID_TYPE_ITEM));
		pItem->Create(entry, owner);
		pItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
		return pItem;
	}
}

Item * ObjectMgr::LoadItem(uint64 guid)
{
	QueryResult * result = CharacterDatabase.Query("SELECT * FROM playeritems WHERE guid = %u", GUID_LOPART(guid));
	Item * pReturn = 0;

	if(result)
	{
		ItemPrototype * pProto = ItemPrototypeStorage.LookupEntry(result->Fetch()[2].GetUInt32());
		if(!pProto)
			return NULL;

		if(pProto->InventoryType == INVTYPE_BAG)
		{
			Container * pContainer = new Container(HIGHGUID_TYPE_CONTAINER,(uint32)guid);
			pContainer->LoadFromDB(result->Fetch());
			pReturn = pContainer;
		}
		else
		{
			Item * pItem = new Item(HIGHGUID_TYPE_ITEM,(uint32)guid);
			pItem->LoadFromDB(result->Fetch(), 0, false);
			pReturn = pItem;
		}
		delete result;
	}
	
	return pReturn;
}

void ObjectMgr::LoadCorpses(MapMgr * mgr)
{
	Corpse *pCorpse = NULL;

	QueryResult *result = CharacterDatabase.Query("SELECT * FROM corpses WHERE instanceId = %u", mgr->GetInstanceID());

	if(result)
	{
		do
		{
			Field *fields = result->Fetch();
			pCorpse = new Corpse(HIGHGUID_TYPE_CORPSE,fields[0].GetUInt32());
			pCorpse->SetPosition(fields[1].GetFloat(), fields[2].GetFloat(), fields[3].GetFloat(), fields[4].GetFloat());
			pCorpse->SetZoneId(fields[5].GetUInt32());
			pCorpse->SetMapId(fields[6].GetUInt32());
			pCorpse->SetInstanceID(fields[7].GetUInt32());
			pCorpse->LoadValues( fields[8].GetString());
			if(pCorpse->GetUInt32Value(CORPSE_FIELD_DISPLAY_ID) == 0)
			{
				delete pCorpse;
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

void ObjectMgr::CorpseAddEventDespawn(Corpse *pCorpse)
{
	if(!pCorpse->IsInWorld())
		delete pCorpse;
	else
		sEventMgr.AddEvent(pCorpse->GetMapMgr(), &MapMgr::EventCorpseDespawn, pCorpse->GetGUID(), EVENT_CORPSE_DESPAWN, 600000, 1,0);
}

void ObjectMgr::DespawnCorpse(uint64 Guid)
{
	Corpse * pCorpse = objmgr.GetCorpse((uint32)Guid);
	if(pCorpse == 0)	// Already Deleted
		return;
	
	pCorpse->Despawn();
	delete pCorpse;
}

void ObjectMgr::CorpseCollectorUnload()
{
	CorpseMap::const_iterator itr;
	_corpseslock.Acquire();
	for (itr = m_corpses.begin(); itr != m_corpses.end();)
	{
		Corpse * c =itr->second;
		++itr;
		if(c->IsInWorld())
			c->RemoveFromWorld(false);
		delete c;
	}
	m_corpses.clear();
	_corpseslock.Release();
}

GossipMenu::GossipMenu(uint64 Creature_Guid, uint32 Text_Id) : TextId(Text_Id), CreatureGuid(Creature_Guid)
{

}

void GossipMenu::AddItem(uint8 Icon, const char* Text, int32 Id /* = -1 */, int8 Extra /* = 0 */)
{
	GossipMenuItem Item;
	Item.Icon = Icon;
	Item.Extra = Extra;
	Item.Text = Text;
	Item.Id = (uint32)Menu.size();
	if(Id > 0)
		Item.IntId = Id;
	else
		Item.IntId = Item.Id;		

	Menu.push_back(Item);
}

void GossipMenu::AddItem(GossipMenuItem* GossipItem)
{
	Menu.push_back(*GossipItem);
}

void GossipMenu::BuildPacket(WorldPacket& Packet)
{
	Packet << CreatureGuid;
	Packet << uint32(0);			// some new menu type in 2.4?
	Packet << TextId;
	Packet << uint32(Menu.size());

	for(std::vector<GossipMenuItem>::iterator iter = Menu.begin();
		iter != Menu.end(); ++iter)
	{
		Packet << iter->Id;
		Packet << iter->Icon;
		Packet << iter->Extra;
		Packet << uint32(0);	// something new in tbc. maybe gold requirement or smth?
		Packet << iter->Text;
		Packet << uint8(0); // ?
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
		Trainer * tr = new Trainer;
		tr->RequiredSkill = fields[1].GetUInt32();
		tr->RequiredSkillLine = fields[2].GetUInt32();
		tr->RequiredClass = fields[3].GetUInt32();
		tr->TrainerType = fields[4].GetUInt32();
		tr->Can_Train_Gossip_TextId = fields[6].GetUInt32();
		tr->Cannot_Train_GossipTextId = fields[7].GetUInt32();
		tr->UIMessage = (char*)NormalTalkMessage;
		if(!tr->Can_Train_Gossip_TextId)
			tr->Can_Train_Gossip_TextId=1;
		if(!tr->Cannot_Train_GossipTextId)
			tr->Cannot_Train_GossipTextId=1;

		temp = fields[5].GetString();
		len=strlen(temp);
		if(len)
		{
			tr->UIMessage = new char[len+1];
			strcpy(tr->UIMessage, temp);
			tr->UIMessage[len] = 0;
		}

		//now load the spells
		result2 = WorldDatabase.Query("SELECT * FROM trainer_spells where entry='%u'",entry);
		if(!result2)
		{
			Log.Error("LoadTrainers", "Trainer with no spells, entry %u.", entry);
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
					Log.Warning("LoadTrainers", "Trainer %u with no invalid spells (%u/%u).", entry, CastSpellID, LearnSpellID);
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
		if(Class == 10 || Class == 6)
			continue;

		// Search for a playercreateinfo.
		for(uint32 Race = RACE_HUMAN; Race <= RACE_DRAENEI; ++Race )
		{
			PCI = GetPlayerCreateInfo(Race, Class);

			if(PCI == 0)
				continue;   // Class not valid for this race.

			// Generate each level's information
			uint32 MaxLevel = sWorld.m_genLevelCap + 1;
			LevelInfo* lvl=0, lastlvl;
			lastlvl.HP = PCI->health;
			lastlvl.Mana = PCI->mana;
			lastlvl.Stat[0] = PCI->strength;
			lastlvl.Stat[1] = PCI->ability;
			lastlvl.Stat[2] = PCI->stamina;
			lastlvl.Stat[3] = PCI->intellect;
			lastlvl.Stat[4] = PCI->spirit;
			lastlvl.XPToNextLevel = 400;
			LevelMap * lMap = new LevelMap;

			// Create first level.
			lvl = new LevelInfo;
			*lvl = lastlvl;

			// Insert into map
			lMap->insert( LevelMap::value_type( 1, lvl ) );

			uint32 val;
			for(uint32 Level = 2; Level < MaxLevel; ++Level)
			{
				lvl = new LevelInfo;

				// Calculate Stats
				for(uint32 s = 0; s < 5; ++s)
				{
					val = GainStat(Level, Class, s);
					lvl->Stat[s] = lastlvl.Stat[s] + val;
				}

				// Calculate HP/Mana
				uint32 TotalHealthGain = 0;
				uint32 TotalManaGain = 0;

				switch(Class)
				{
				case WARRIOR:
					if(Level<13)TotalHealthGain+=19;
					else if(Level <36) TotalHealthGain+=Level+6;
//					else if(Level >60) TotalHealthGain+=Level+100;
					else if(Level >60) TotalHealthGain+=Level+206;
					else TotalHealthGain+=2*Level-30;
					break;
				case HUNTER:
					if(Level<13)TotalHealthGain+=17;
//					else if(Level >60) TotalHealthGain+=Level+45;
					else if(Level >60) TotalHealthGain+=Level+161;
					else TotalHealthGain+=Level+4;

					if(Level<11)TotalManaGain+=29;
					else if(Level<27)TotalManaGain+=Level+18;
//					else if(Level>60)TotalManaGain+=Level+20;
					else if(Level>60)TotalManaGain+=Level+150;
					else TotalManaGain+=45;
					break;
				case ROGUE:
					if(Level <15)TotalHealthGain+=17;
//					else if(Level >60) TotalHealthGain+=Level+110;
					else if(Level >60) TotalHealthGain+=Level+191;
					else TotalHealthGain+=Level+2;
					break;
				case DRUID:
					if(Level < 17)TotalHealthGain+=17;
//					else if(Level >60) TotalHealthGain+=Level+55;
					else if(Level >60) TotalHealthGain+=Level+176;
					else TotalHealthGain+=Level;

					if(Level < 26)TotalManaGain+=Level+20;
//					else if(Level>60)TotalManaGain+=Level+25;
					else if(Level>60)TotalManaGain+=Level+150;
					else TotalManaGain+=45;
					break;
				case MAGE:
					if(Level < 23)TotalHealthGain+=15;
//					else if(Level >60) TotalHealthGain+=Level+40;
					else if(Level >60) TotalHealthGain+=Level+190;
					else TotalHealthGain+=Level-8;

					if(Level <28)TotalManaGain+=Level+23;
//					else if(Level>60)TotalManaGain+=Level+26;
					else if(Level>60)TotalManaGain+=Level+115;
					else TotalManaGain+=51;
					break;
				case SHAMAN:
					if(Level <16)TotalHealthGain+=17;
//					else if(Level >60) TotalHealthGain+=Level+75;
					else if(Level >60) TotalHealthGain+=Level+157;
					else TotalHealthGain+=Level+1;

					if(Level<22)TotalManaGain+=Level+19;
//					else if(Level>60)TotalManaGain+=Level+70;
					else if(Level>60)TotalManaGain+=Level+175;
					else TotalManaGain+=49;
					break;
				case WARLOCK:
					if(Level <17)TotalHealthGain+=17;
//					else if(Level >60) TotalHealthGain+=Level+50;
					else if(Level >60) TotalHealthGain+=Level+192;
					else TotalHealthGain+=Level-2;

					if(Level< 30)TotalManaGain+=Level+21;
//					else if(Level>60)TotalManaGain+=Level+25;
					else if(Level>60)TotalManaGain+=Level+121;
					else TotalManaGain+=51;
					break;
				case PALADIN:
					if(Level < 14)TotalHealthGain+=18;
//					else if(Level >60) TotalHealthGain+=Level+55;
					else if(Level >60) TotalHealthGain+=Level+167;
					else TotalHealthGain+=Level+4;

					if(Level<30)TotalManaGain+=Level+17;
//					else if(Level>60)TotalManaGain+=Level+100;
					else if(Level>60)TotalManaGain+=Level+131;
					else TotalManaGain+=42;
					break;
				case PRIEST:
					if(Level <21)TotalHealthGain+=15;
//					else if(Level >60) TotalHealthGain+=Level+40;
					else if(Level >60) TotalHealthGain+=Level+157;
					else TotalHealthGain+=Level-6;

					if(Level <22)TotalManaGain+=Level+22;
					else if(Level <32)TotalManaGain+=Level+37;
//					else if(Level>60)TotalManaGain+=Level+35;
					else if(Level>60)TotalManaGain+=Level+207;
					else TotalManaGain+=54;
					break;
				}

				// Apply HP/Mana
				lvl->HP = lastlvl.HP + TotalHealthGain;
				lvl->Mana = lastlvl.Mana + TotalManaGain;

				// Calculate next level XP
				uint32 nextLvlXP = 0;
/*				if( Level > 0 && Level <= 30 )
				{
					nextLvlXP = ((int)((((double)(8 * Level * ((Level * 5) + 45)))/100)+0.5))*100;
				}
				else if( Level == 31 )
				{
					nextLvlXP = ((int)((((double)(((8 * Level) + 3) * ((Level * 5) + 45)))/100)+0.5))*100;
				}
				else if( Level == 32 )
				{
					nextLvlXP = ((int)((((double)(((8 * Level) + 6) * ((Level * 5) + 45)))/100)+0.5))*100;
				}
				else
				{
					nextLvlXP = ((int)((((double)(((8 * Level) + ((Level - 30) * 5)) * ((Level * 5) + 45)))/100)+0.5))*100;
				}*/

				//this is a fixed table taken from 2.3.0 wow. This can;t get more blizzlike with the "if" cases ;)
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
				lastlvl = *lvl;
				lastlvl.HP = lastlvl.HP;

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
			if( Level > sWorld.m_genLevelCap ) // too far.
				Level = sWorld.m_genLevelCap;

			// Pull the level information from the second map.
			LevelMap::iterator it2 = itr->second->find( Level );
			ASSERT( it2 != itr->second->end() );

			return it2->second;
		}
	}

	return NULL;
}

void ObjectMgr::LoadDefaultPetSpells()
{
	QueryResult * result = WorldDatabase.Query("SELECT * FROM petdefaultspells");
	if(result)
	{
		do 
		{
			Field * f = result->Fetch();
			uint32 Entry = f[0].GetUInt32();
			uint32 spell = f[1].GetUInt32();
			SpellEntry * sp = dbcSpell.LookupEntry(spell);

			if(spell && Entry && sp)
			{
				PetDefaultSpellMap::iterator itr = mDefaultPetSpells.find(Entry);
				if(itr != mDefaultPetSpells.end())
					itr->second.insert(sp);
				else
				{
					set<SpellEntry*> s;
					s.insert(sp);
					mDefaultPetSpells[Entry] = s;
				}
			}
		} while(result->NextRow());

		delete result;
	}
}

set<SpellEntry*>* ObjectMgr::GetDefaultPetSpells(uint32 Entry)
{
	PetDefaultSpellMap::iterator itr = mDefaultPetSpells.find(Entry);
	if(itr == mDefaultPetSpells.end())
		return 0;

	return &(itr->second);
}

void ObjectMgr::LoadPetSpellCooldowns()
{
	DBCFile dbc;

	if( !dbc.open( "DBC/CreatureSpellData.dbc" ) )
	{
		Log.Error( "ObjectMgr", "Cannot find file ./DBC/CreatureSpellData.dbc" );
		return;
	}

	uint32 SpellId;
	uint32 Cooldown;
	for(uint32 i = 0; i < dbc.getRecordCount(); ++i)
	{
		for(uint32 j = 0; j < 3; ++j)
		{
			SpellId = dbc.getRecord(i).getUInt(1 + j);
			Cooldown = dbc.getRecord(i).getUInt(5 + j);
			Cooldown *= 10;
			if(SpellId)
			{
				PetSpellCooldownMap::iterator itr = mPetSpellCooldowns.find(SpellId);
				if(itr == mPetSpellCooldowns.end())
				{
					if( Cooldown )
						mPetSpellCooldowns.insert( make_pair(SpellId, Cooldown) );
				}
				else
				{
					uint32 SP2 = mPetSpellCooldowns[SpellId];
					ASSERT(Cooldown == SP2);
				}
			}
		}
	}
}

uint32 ObjectMgr::GetPetSpellCooldown(uint32 SpellId)
{
	PetSpellCooldownMap::iterator itr = mPetSpellCooldowns.find( SpellId );
	if( itr != mPetSpellCooldowns.end() )
		return itr->second;

	SpellEntry* sp = dbcSpell.LookupEntry( SpellId );
	return sp->CategoryRecoveryTime + sp->StartRecoveryTime;
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
	QueryResult *result = WorldDatabase.Query("SELECT * FROM creature_waypoints");
	if(!result)return;

	do
	{
		Field *fields = result->Fetch();
		WayPoint* wp = new WayPoint;
		wp->id = fields[1].GetUInt32();
		wp->x = fields[2].GetFloat();
		wp->y = fields[3].GetFloat();
		wp->z = fields[4].GetFloat();
		wp->waittime = fields[5].GetUInt32();
		wp->flags = fields[6].GetUInt32();
		wp->forwardemoteoneshot = fields[7].GetBool();
		wp->forwardemoteid = fields[8].GetUInt32();
		wp->backwardemoteoneshot = fields[9].GetBool();
		wp->backwardemoteid = fields[10].GetUInt32();
		wp->forwardskinid = fields[11].GetUInt32();
		wp->backwardskinid = fields[12].GetUInt32();

		HM_NAMESPACE::hash_map<uint32,WayPointMap*>::const_iterator i;
		uint32 spawnid=fields[0].GetUInt32();
		i=m_waypoints.find(spawnid);
		if(i==m_waypoints.end())
		{
			WayPointMap* m=new WayPointMap;
			if(m->size() <= wp->id)
				m->resize(wp->id+1);
			(*m)[wp->id]=wp;
			m_waypoints[spawnid]=m;		
		}else
		{
			if(i->second->size() <= wp->id)
				i->second->resize(wp->id+1);

			(*(i->second))[wp->id]=wp;
		}
	}while( result->NextRow() );

	Log.Notice("ObjectMgr", "%u waypoints cached.", result->GetRowCount());
	delete result;
}

WayPointMap*ObjectMgr::GetWayPointMap(uint32 spawnid)
{
	HM_NAMESPACE::hash_map<uint32,WayPointMap*>::const_iterator i;
	i=m_waypoints.find(spawnid);
	if(i!=m_waypoints.end())
	{
		WayPointMap * m=i->second;
		// we don't wanna erase from the map, becuase some are used more
		// than once (for instances)

		//m_waypoints.erase(i);
		return m;
	}
	else return NULL;
}

Pet * ObjectMgr::CreatePet()
{
	uint32 guid;
	m_petlock.Acquire();
	guid =++m_hiPetGuid;
	m_petlock.Release();

	uint64 fullguid = ((uint64)HIGHGUID_TYPE_PET << 32) | ((uint64)guid << 24) | guid;
	return new Pet(fullguid);
}

Player * ObjectMgr::CreatePlayer()
{
	uint32 guid;
	m_playerguidlock.Acquire();
	guid =++m_hiPlayerGuid;
	m_playerguidlock.Release();
	return new Player(guid);
}

void ObjectMgr::AddPlayer(Player * p)//add it to global storage
{
	_playerslock.AcquireWriteLock();
	_players[p->GetLowGUID()] = p;
	_playerslock.ReleaseWriteLock();
}

void ObjectMgr::RemovePlayer(Player * p)
{
	_playerslock.AcquireWriteLock();
	_players.erase(p->GetLowGUID());
	_playerslock.ReleaseWriteLock();

}

Corpse * ObjectMgr::CreateCorpse()
{
	uint32 guid;
	m_corpseguidlock.Acquire();
	guid =++m_hiCorpseGuid;
	m_corpseguidlock.Release();
	return new Corpse(HIGHGUID_TYPE_CORPSE,guid);
}

void ObjectMgr::AddCorpse(Corpse * p)//add it to global storage
{
	_corpseslock.Acquire();
	m_corpses[p->GetLowGUID()]=p;
	_corpseslock.Release();
}

void ObjectMgr::RemoveCorpse(Corpse * p)
{
	_corpseslock.Acquire();
	m_corpses.erase(p->GetLowGUID());
	_corpseslock.Release();
}

Corpse * ObjectMgr::GetCorpse(uint32 corpseguid)
{
	Corpse * rv;
	_corpseslock.Acquire();
	CorpseMap::const_iterator itr = m_corpses.find(corpseguid);
	rv = (itr != m_corpses.end()) ? itr->second : 0;
	_corpseslock.Release();
	return rv;
}

Transporter * ObjectMgr::GetTransporter(uint32 guid)
{
	Transporter * rv;
	_TransportLock.Acquire();
	HM_NAMESPACE::hash_map<uint32, Transporter*>::const_iterator itr = mTransports.find(guid);
	rv = (itr != mTransports.end()) ? itr->second : 0;
	_TransportLock.Release();
	return rv;
}

void ObjectMgr::AddTransport(Transporter *pTransporter)
{
	_TransportLock.Acquire();
	mTransports[pTransporter->GetUIdFromGUID()]=pTransporter;
 	_TransportLock.Release();
}

Transporter * ObjectMgr::GetTransporterByEntry(uint32 entry)
{
	Transporter * rv = 0;
	_TransportLock.Acquire();
	HM_NAMESPACE::hash_map<uint32, Transporter*>::iterator itr = mTransports.begin();
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
			p->charterId[CharterType] = NULL;
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

void ObjectMgr::HandleMonsterSayEvent(Creature * pCreature, MONSTER_SAY_EVENTS Event)
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
		mod.mob_rep_limit = fields[2].GetUInt32();
		mod.boss_rep_reward = fields[3].GetInt32();
		mod.boss_rep_limit = fields[4].GetUInt32();
		mod.faction[0] = fields[5].GetUInt32();
		mod.faction[1] = fields[6].GetUInt32();

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

bool ObjectMgr::HandleInstanceReputationModifiers(Player * pPlayer, Unit * pVictim)
{
	uint32 team = pPlayer->GetTeam();
	bool is_boss;
	if(pVictim->GetTypeId() != TYPEID_UNIT)
		return false;

	HM_NAMESPACE::hash_map<uint32, InstanceReputationModifier*>::iterator itr = m_reputation_instance.find(pVictim->GetMapId());
	if(itr == m_reputation_instance.end())
		return false;

	is_boss = 0;//static_cast< Creature* >( pVictim )->GetCreatureName() ? ((Creature*)pVictim)->GetCreatureName()->Rank : 0;
	if( !is_boss && static_cast< Creature* >( pVictim )->proto && static_cast< Creature* >( pVictim )->proto->boss )
		is_boss = 1;

	// Apply the bonuses as normal.
	int32 replimit;
	int32 value;

	for(vector<InstanceReputationMod>::iterator i = itr->second->mods.begin(); i !=  itr->second->mods.end(); ++i)
	{
		if(!(*i).faction[team])
			continue;

		if(is_boss)
		{
			value = i->boss_rep_reward;
			replimit = i->boss_rep_limit;
		}
		else
		{
			value = i->mob_rep_reward;
			replimit = i->mob_rep_reward;
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

#ifdef VOICE_CHAT
void ObjectMgr::GroupVoiceReconnected()
{
	m_groupLock.AcquireReadLock();
	for(GroupMap::iterator itr = m_groups.begin(); itr != m_groups.end(); ++itr)
		itr->second->VoiceSessionReconnected();
	m_groupLock.ReleaseReadLock();
}
#endif
