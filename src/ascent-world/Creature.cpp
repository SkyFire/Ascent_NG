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

#define M_PI	   3.14159265358979323846

Creature::Creature(uint64 guid)
{
	proto=0;
	m_valuesCount = UNIT_END;
	m_objectTypeId = TYPEID_UNIT;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0,(UNIT_END)*sizeof(uint32));
	m_updateMask.SetCount(UNIT_END);
	SetUInt32Value( OBJECT_FIELD_TYPE,TYPE_UNIT|TYPE_OBJECT);
	SetUInt64Value( OBJECT_FIELD_GUID,guid);
	m_wowGuid.Init(GetGUID());


	m_quests = NULL;
	proto = NULL;
 
	creature_info=NULL;
	m_H_regenTimer=0;
	m_P_regenTimer=0;
	m_useAI = true;
	mTaxiNode = 0;

	Skinned = false;
	
	m_enslaveCount = 0;
	m_enslaveSpell = 0;
	
	for(uint32 x=0;x<7;x++)
	{
		FlatResistanceMod[x]=0;
		BaseResistanceModPct[x]=0;
		ResistanceModPct[x]=0;
		ModDamageDone[x]=0;
		ModDamageDonePct[x]=1.0;
	}

	for(uint32 x=0;x<5;x++)
	{
		TotalStatModPct[x]=0;
		StatModPct[x]=0;
		FlatStatMod[x]=0;
	}

	totemOwner = NULL;
	totemSlot = -1;

	m_PickPocketed = false;
	m_SellItems = NULL;
	_myScriptClass = NULL;
	m_TaxiNode = 0;
	myFamily = 0;

	haslinkupevent = false;
	original_emotestate = 0;
	mTrainer = 0;
	m_spawn = 0;
	auctionHouse = 0;
	has_waypoint_text = has_combat_text = false;
	SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER,1);
	SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER,1);
	m_custom_waypoint_map = 0;
	m_escorter = 0;
	m_limbostate = false;
	m_corpseEvent=false;
	m_respawnCell=NULL;
	m_walkSpeed = 2.5f;
	m_runSpeed = MONSTER_NORMAL_RUN_SPEED;
	m_base_runSpeed = m_runSpeed;
	m_base_walkSpeed = m_walkSpeed;
	m_noRespawn=false;
    m_canRegenerateHP = true;
	m_transportGuid = 0;
	m_transportPosition = NULL;
	BaseAttackType = SCHOOL_NORMAL;

	m_taggingPlayer = m_taggingGroup = 0;
	m_lootMethod = -1;
	m_noDeleteAfterDespawn = false;
}


Creature::~Creature()
{
	sEventMgr.RemoveEvents(this);

	if(IsTotem())
		totemOwner->m_TotemSlots[totemSlot] = 0;

	if(_myScriptClass != 0)
		_myScriptClass->Destroy();

	if(m_custom_waypoint_map != 0)
	{
		for(WayPointMap::iterator itr = m_custom_waypoint_map->begin(); itr != m_custom_waypoint_map->end(); ++itr)
			delete (*itr);
		delete m_custom_waypoint_map;
	}
	if(m_respawnCell!=NULL)
		m_respawnCell->_respawnObjects.erase(this);
}

void Creature::Update( uint32 p_time )
{
	Unit::Update( p_time );
	if(IsTotem() && isDead())
	{
		RemoveFromWorld(false, true);
		delete this;
		return;
	}

	if(m_corpseEvent)
	{
		sEventMgr.RemoveEvents(this);
		if(this->proto==NULL)
			sEventMgr.AddEvent(this, &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else if (this->creature_info->Rank == ELITE_WORLDBOSS)
			sEventMgr.AddEvent(this, &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_BOSSCORPSE, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else if ( this->creature_info->Rank == ELITE_RAREELITE || this->creature_info->Rank == ELITE_RARE)
			sEventMgr.AddEvent(this, &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_RARECORPSE, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else
			sEventMgr.AddEvent(this, &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_CORPSE, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		m_corpseEvent=false;
	}
}

void Creature::SafeDelete()
{
	sEventMgr.RemoveEvents(this);
	//sEventMgr.AddEvent(World::getSingletonPtr(), &World::DeleteObject, ((Object*)this), EVENT_CREATURE_SAFE_DELETE, 1000, 1);
	sEventMgr.AddEvent(this, &Creature::DeleteMe, EVENT_CREATURE_SAFE_DELETE, 1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Creature::DeleteMe()
{
	if(IsInWorld())
		RemoveFromWorld(false, true);

	delete this;
}

void Creature::OnRemoveCorpse()
{
	// time to respawn!
	if (IsInWorld() && (int32)m_mapMgr->GetInstanceID() == m_instanceId)
	{

		DEBUG_LOG("Removing corpse of "I64FMT"...", GetGUID());
	   
			if((GetMapMgr()->GetMapInfo() && GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID && this->proto && this->proto->boss) || m_noRespawn)
			{
				RemoveFromWorld(false, true);
			}
			else
			{
				if(proto && proto->RespawnTime)
					RemoveFromWorld(true, false);
				else
					RemoveFromWorld(false, true);
			}
		
	   
		setDeathState(DEAD);

		m_position = m_spawnLocation;
	}
	else
	{
		// if we got here it's pretty bad
	}
}

void Creature::OnRespawn(MapMgr * m)
{
	DEBUG_LOG("Respawning "I64FMT"...", GetGUID());
	SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0); // not tagging shiat
	if(proto && m_spawn)
	{
		SetUInt32Value(UNIT_NPC_FLAGS, proto->NPCFLags);
		SetUInt32Value(UNIT_NPC_EMOTESTATE, m_spawn->emote_state);
	}

	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
	Skinned = false;
	//ClearTag();
	m_taggingGroup = m_taggingPlayer = 0;
	m_lootMethod = -1;

	/* creature death state */
	if(proto && proto->death_state == 1)
	{
		uint32 newhealth = m_uint32Values[UNIT_FIELD_HEALTH] / 100;
		if(!newhealth)
			newhealth = 1;
		SetUInt32Value(UNIT_FIELD_HEALTH, 1);
		m_limbostate = true;
		bInvincible = true;
		SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DEAD);
	}

	setDeathState(ALIVE);
	GetAIInterface()->StopMovement(0); // after respawn monster can move
	m_PickPocketed = false;
	PushToWorld(m);
}

void Creature::Create (const char* name, uint32 mapid, float x, float y, float z, float ang)
{
	Object::_Create( mapid, x, y, z, ang );
}

void Creature::CreateWayPoint (uint32 WayPointID, uint32 mapid, float x, float y, float z, float ang)
{
	Object::_Create( mapid, x, y, z, ang);
}

///////////
/// Looting

void Creature::GenerateLoot()
{
	if( IsPet() )
		return;

	lootmgr.FillCreatureLoot(&m_loot,GetEntry(), m_mapMgr ? (m_mapMgr->iInstanceMode > 0 ? true : false) : false);
	
	m_loot.gold = proto ? proto->money : 0;

	//For now let fill according to entry
	if(!m_loot.gold)
	{
		CreatureInfo *info=GetCreatureName();
		if (info && info->Type != BEAST)
		{
			if(m_uint32Values[UNIT_FIELD_MAXHEALTH] <= 1667)
				m_loot.gold = (uint32)((info->Rank+1)*getLevel()*(rand()%5 + 1)); //generate copper
			else
				m_loot.gold = (uint32)((info->Rank+1)*getLevel()*(rand()%5 + 1)*(this->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.0006)); //generate copper
		}
	}
	
	if(m_loot.gold)
		m_loot.gold = int32(float(m_loot.gold) * sWorld.getRate(RATE_MONEY));
}

void Creature::SaveToDB()
{
	if( m_spawn == NULL )
		return;
	 
	std::stringstream ss;
	ss << "REPLACE INTO creature_spawns VALUES("
		<< m_spawn->id << ","
		<< GetEntry() << ","
		<< GetMapId() << ","
		<< m_position.x << ","
		<< m_position.y << ","
		<< m_position.z << ","
		<< m_position.o << ","
		<< m_aiInterface->getMoveType() << ","
		<< m_uint32Values[UNIT_FIELD_DISPLAYID] << ","
		<< m_uint32Values[UNIT_FIELD_FACTIONTEMPLATE] << ","
		<< m_uint32Values[UNIT_FIELD_FLAGS] << ","
		<< m_uint32Values[UNIT_FIELD_BYTES_0] << ","
		<< m_uint32Values[UNIT_FIELD_BYTES_2] << ","
		<< m_uint32Values[UNIT_NPC_EMOTESTATE] << ",0,";
		/*<< ((this->m_spawn ? m_spawn->respawnNpcLink : uint32(0))) << ",";*/

	if(m_spawn)
		ss << m_spawn->channel_spell << "," << m_spawn->channel_target_go << "," << m_spawn->channel_target_creature << ",";
	else
		ss << "0,0,0,";

	ss << uint32(GetStandState()) << ")";
	WorldDatabase.Execute(ss.str().c_str());
}

void Creature::SaveToFile(std::stringstream & name)
{
/*	FILE * OutFile;

	OutFile = fopen(name.str().c_str(), "wb");
	if (!OutFile) return;
	
	uint32 creatureEntry = GetUInt32Value(OBJECT_FIELD_ENTRY);
	if (!m_sqlid)
		m_sqlid = objmgr.GenerateLowGuid(HIGHGUID_UNIT);

	std::stringstream ss;
	ss << "DELETE FROM creatures WHERE id=" << m_sqlid;
	fwrite(ss.str().c_str(),1,ss.str().size(),OutFile);

	ss.rdbuf()->str("");
	ss << "\nINSERT INTO creatures (id, mapId, zoneId, name_id, positionX, positionY, positionZ, orientation, moverandom, running, data) VALUES ( "
		<< m_sqlid << ", "
		<< GetMapId() << ", "
		<< GetZoneId() << ", "
		<< GetUInt32Value(OBJECT_FIELD_ENTRY) << ", "
		<< m_position.x << ", "
		<< m_position.y << ", "
		<< m_position.z << ", "
		<< m_position.o << ", "
		<< GetAIInterface()->getMoveType() << ", "
		<< GetAIInterface()->getMoveRunFlag() << ", '";
	for( uint16 index = 0; index < m_valuesCount; index ++ )
		ss << GetUInt32Value(index) << " ";

	ss << "' )";
	fwrite(ss.str().c_str(),1,ss.str().size(),OutFile);
	fclose(OutFile);*/
}


void Creature::LoadScript()
{
	_myScriptClass = sScriptMgr.CreateAIScriptClassForEntry(this);
}

void Creature::DeleteFromDB()
{
	if( m_spawn == NULL )
		return;

	WorldDatabase.Execute("DELETE FROM creature_spawns WHERE id=%u", m_spawn->id);
	WorldDatabase.Execute("DELETE FROM creature_waypoints WHERE spawnid=%u", m_spawn->id);
}


/////////////
/// Quests

void Creature::AddQuest(QuestRelation *Q)
{
	m_quests->push_back(Q);
}

void Creature::DeleteQuest(QuestRelation *Q)
{
	list<QuestRelation *>::iterator it;
	for ( it = m_quests->begin(); it != m_quests->end(); ++it )
	{
		if (((*it)->type == Q->type) && ((*it)->qst == Q->qst ))
		{
			delete (*it);
			m_quests->erase(it);
			break;
		}
	}
}

Quest* Creature::FindQuest(uint32 quest_id, uint8 quest_relation)
{   
	list<QuestRelation *>::iterator it;
	for (it = m_quests->begin(); it != m_quests->end(); ++it)
	{
		QuestRelation *ptr = (*it);

		if ((ptr->qst->id == quest_id) && (ptr->type & quest_relation))
		{
			return ptr->qst;
		}
	}
	return NULL;
}

uint16 Creature::GetQuestRelation(uint32 quest_id)
{
	uint16 quest_relation = 0;
	list<QuestRelation *>::iterator it;

	for (it = m_quests->begin(); it != m_quests->end(); ++it)
	{
		if ((*it)->qst->id == quest_id)
		{
			quest_relation |= (*it)->type;
		}
	}
	return quest_relation;
}

uint32 Creature::NumOfQuests()
{
	return (uint32)m_quests->size();
}

void Creature::_LoadQuests()
{
	sQuestMgr.LoadNPCQuests(this);
}

void Creature::setDeathState(DeathState s) 
{
	if(s == JUST_DIED) 
	{

		GetAIInterface()->SetUnitToFollow(NULL);
		m_deathState = CORPSE;
		m_corpseEvent=true;
		
		/*sEventMgr.AddEvent(this, &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, 180000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);*/
		if(m_enslaveSpell)
			RemoveEnslave();

		if(m_currentSpell)
			m_currentSpell->cancel();

	}else m_deathState = s;
}

void Creature::AddToWorld()
{
	// force set faction
	if(m_faction == 0 || m_factionDBC == 0)
		_setFaction();

	if(creature_info == 0)
		creature_info = CreatureNameStorage.LookupEntry(GetEntry());

	if(creature_info == 0) return;
	
	if(m_faction == 0 || m_factionDBC == 0)
		return;

	Object::AddToWorld();
}

void Creature::AddToWorld(MapMgr * pMapMgr)
{
	// force set faction
	if(m_faction == 0 || m_factionDBC == 0)
		_setFaction();

	if(creature_info == 0)
		creature_info = CreatureNameStorage.LookupEntry(GetEntry());

	if(creature_info == 0) return;

	if(m_faction == 0 || m_factionDBC == 0)
		return;

	Object::AddToWorld(pMapMgr);
}

bool Creature::CanAddToWorld()
{
	if(m_factionDBC == 0 || m_faction == 0)
		_setFaction();

	if(creature_info == 0 || m_faction == 0 || m_factionDBC == 0 || proto == 0)
		return false;
	
	return true;
}

void Creature::RemoveFromWorld(bool addrespawnevent, bool free_guid)
{
	m_taggingPlayer = m_taggingGroup = 0;
	m_lootMethod = 1;

	RemoveAllAuras();
	
	if(IsPet()) /* Is a pet: IsPet() actually returns false on a pet? o_X */
	{
		if(IsInWorld())
			Unit::RemoveFromWorld(true);

		SafeDelete();
		return;
	}

	if(IsInWorld())
	{
		uint32 delay = 0;
		if(addrespawnevent && proto && proto->RespawnTime > 0)
			delay = proto->RespawnTime;
		Despawn(0, delay);
	}
}

void Creature::EnslaveExpire()
{
	
	m_enslaveCount++;
	Player *caster = objmgr.GetPlayer(GetUInt32Value(UNIT_FIELD_CHARMEDBY));
	if(caster)
	{
		caster->SetUInt64Value(UNIT_FIELD_CHARM, 0);
		caster->SetUInt64Value(UNIT_FIELD_SUMMON, 0);
		WorldPacket data(8);
		data.Initialize(SMSG_PET_SPELLS);
		data << uint64(0);
		caster->GetSession()->SendPacket(&data);
	}
	SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);
	SetUInt64Value(UNIT_FIELD_SUMMONEDBY, 0);
	SetIsPet(false);

	m_walkSpeed = m_base_walkSpeed;
	m_runSpeed = m_base_runSpeed;

	switch(GetCreatureName()->Type)
	{
	case DEMON:
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 90);
		break;
	default:
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 954);
		break;
	};
	_setFaction();

	GetAIInterface()->Init(((Unit *)this), AITYPE_AGRO, MOVEMENTTYPE_NONE);

	// Update InRangeSet
	UpdateOppFactionSet();
}

bool Creature::RemoveEnslave()
{
	return RemoveAura(m_enslaveSpell);
}

void Creature::AddInRangeObject(Object* pObj)
{
	Unit::AddInRangeObject(pObj);
}

void Creature::OnRemoveInRangeObject(Object* pObj)
{
	if(totemOwner == pObj)		// player gone out of range of the totem
	{
		// Expire next loop.
		event_ModifyTimeLeft(EVENT_TOTEM_EXPIRE, 1);
	}

	if(m_escorter == pObj)
	{
		// we lost our escorter, return to the spawn.
		m_aiInterface->StopMovement(10000);
		DestroyCustomWaypointMap();
		Despawn(1000, 1000);
	}

	Unit::OnRemoveInRangeObject(pObj);
}

void Creature::ClearInRangeSet()
{
	Unit::ClearInRangeSet();
}

void Creature::CalcResistance(uint32 type)
{
	int32 res = (BaseResistance[type] * (100 + BaseResistanceModPct[type])) / 100;
	res += FlatResistanceMod[type];
	res += (res * ResistanceModPct[type]) / 100;
	if(type==0)res+=GetUInt32Value(UNIT_FIELD_STAT1)*2;//fix armor from agi
	SetUInt32Value(UNIT_FIELD_RESISTANCES + type, res > 0 ? res : 0);
}

void Creature::CalcStat(uint32 type)
{
	int32 res=(BaseStats[type]*(100+StatModPct[type]))/100;
		
	res+=FlatStatMod[type];
	if(res<0)res=0;
		
	res+=(res*(TotalStatModPct[type]))/100;
	SetUInt32Value(UNIT_FIELD_STAT0+type,res>0?res:0);
}


void Creature::RegenerateHealth()
{
	if(m_limbostate || !m_canRegenerateHP)
		return;

	uint32 cur=GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 mh=GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if(cur>=mh)return;
	
	//though creatures have their stats we use some wierd formula for amt
	float amt = 0.0f;
	float lvl = float(getLevel());

	if(PctRegenModifier == 0.0f)
		amt = lvl*2.0f;
	else if(PctRegenModifier > 0)
		amt = (lvl*2.0f)*(1.0f+PctRegenModifier);
	else
		amt = (lvl*2.0f)*(-1.0f+PctRegenModifier);
		
	//Apply shit from conf file
	amt*=sWorld.getRate(RATE_HEALTH);
	
	if(amt<=1.0f)//this fixes regen like 0.98
		cur++;
	else
		cur+=(uint32)amt;
	SetUInt32Value(UNIT_FIELD_HEALTH,(cur>=mh)?mh:cur);
}

void Creature::RegenerateMana()
{
	float amt;
	if (m_interruptRegen)
		return;
   
	uint32 cur=GetUInt32Value(UNIT_FIELD_POWER1);
	uint32 mm=GetUInt32Value(UNIT_FIELD_MAXPOWER1);
	if(cur>=mm)return;
	amt=(getLevel()+10)*PctPowerRegenModifier[POWER_TYPE_MANA];
	
	//Apply shit from conf file
	amt*=sWorld.getRate(RATE_POWER1);
	if(amt<=1.0)//this fixes regen like 0.98
		cur++;
	else
		cur+=(uint32)amt;
	SetUInt32Value(UNIT_FIELD_POWER1,(cur>=mm)?mm:cur);
}

void Creature::RegenerateFocus()
{
	if (m_interruptRegen)
		return;

	uint32 cur=GetUInt32Value(UNIT_FIELD_POWER3);
	uint32 mm=GetUInt32Value(UNIT_FIELD_MAXPOWER3);
	if(cur>=mm)return;
	float amt = 10.0f * PctPowerRegenModifier[POWER_TYPE_FOCUS];
	cur+=(uint32)amt;
	SetUInt32Value(UNIT_FIELD_POWER3,(cur>=mm)?mm:cur);
}

void Creature::CallScriptUpdate()
{
	ASSERT(_myScriptClass);
	if(!IsInWorld())
		return;

	_myScriptClass->AIUpdate();
}

void Creature::AddVendorItem(uint32 itemid, uint32 amount)
{
	CreatureItem ci;
	ci.amount = amount;
	ci.itemid = itemid;
	ci.available_amount = 0;
	ci.max_amount = 0;
	ci.incrtime = 0;
	if(!m_SellItems)
	{
		m_SellItems = new vector<CreatureItem>;
		objmgr.SetVendorList(GetEntry(), m_SellItems);
	}
	m_SellItems->push_back(ci);
}
void Creature::ModAvItemAmount(uint32 itemid, uint32 value)
{
	for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
	{
		if(itr->itemid == itemid)
		{
			if(itr->available_amount)
			{
				if(value > itr->available_amount)	// shouldnt happen
				{
					itr->available_amount=0;
					return;
				}
				else
					itr->available_amount -= value;
                
				if(!event_HasEvent(EVENT_ITEM_UPDATE))
					sEventMgr.AddEvent(this, &Creature::UpdateItemAmount, itr->itemid, EVENT_ITEM_UPDATE, itr->incrtime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			}
			return;
		}
	}
}
void Creature::UpdateItemAmount(uint32 itemid)
{
	for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
	{
		if(itr->itemid == itemid)
		{
			if (itr->max_amount==0)		// shouldnt happen
				itr->available_amount=0;
			else
			{
				itr->available_amount = itr->max_amount;
			}
			return;
		}
	}
}

void Creature::TotemExpire()
{
	if( totemOwner != NULL )
	{
		totemOwner->m_TotemSlots[totemSlot] = 0;
	}
	
	totemSlot = -1;
	totemOwner = NULL;

	if( IsInWorld() )
		RemoveFromWorld(false, true);

	SafeDelete();
}

void Creature::FormationLinkUp(uint32 SqlId)
{
	if(!m_mapMgr)		// shouldnt happen
		return;

	Creature * creature = m_mapMgr->GetSqlIdCreature(SqlId);
	if(creature != 0)
	{
		m_aiInterface->m_formationLinkTarget = creature;
		haslinkupevent = false;
		event_RemoveEvents(EVENT_CREATURE_FORMATION_LINKUP);
	}
}

void Creature::ChannelLinkUpGO(uint32 SqlId)
{
	if(!m_mapMgr)		// shouldnt happen
		return;

	GameObject * go = m_mapMgr->GetSqlIdGameObject(SqlId);
	if(go != 0)
	{
		event_RemoveEvents(EVENT_CREATURE_CHANNEL_LINKUP);
		SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, go->GetGUID());
		SetUInt32Value(UNIT_CHANNEL_SPELL, m_spawn->channel_spell);
	}
}

void Creature::ChannelLinkUpCreature(uint32 SqlId)
{
	if(!m_mapMgr)		// shouldnt happen
		return;

	Creature * go = m_mapMgr->GetSqlIdCreature(SqlId);
	if(go != 0)
	{
		event_RemoveEvents(EVENT_CREATURE_CHANNEL_LINKUP);
		SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, go->GetGUID());
		SetUInt32Value(UNIT_CHANNEL_SPELL, m_spawn->channel_spell);
	}
}

void Creature::LoadAIAgents()
{
	/*std::stringstream ss;
	ss << "SELECT * FROM ai_agents where entry=" << GetUInt32Value(OBJECT_FIELD_ENTRY);
	QueryResult *result = sDatabase.Query( ss.str().c_str() );

	if( !result )
		return;

	AI_Spell *sp;

	do
	{
		Field *fields = result->Fetch();

		sp = new AI_Spell;
		sp->entryId = fields[0].GetUInt32();
		sp->agent = fields[1].GetUInt16();
		sp->procChance = fields[3].GetUInt32();
		sp->spellId = fields[5].GetUInt32();
		sp->spellType = fields[6].GetUInt32();;
		sp->spelltargetType = fields[7].GetUInt32();
		sp->floatMisc1 = fields[9].GetFloat();
		sp->Misc2 = fields[10].GetUInt32();
		sp->minrange = GetMinRange(sSpellRange.LookupEntry(sSpellStore.LookupEntry(sp->spellId)->rangeIndex));
		sp->maxrange = GetMaxRange(sSpellRange.LookupEntry(sSpellStore.LookupEntry(sp->spellId)->rangeIndex));

		if(sp->agent == AGENT_RANGED)
		{
			GetAIInterface()->m_canRangedAttack = true;
		}
		else if(sp->agent == AGENT_FLEE)
		{
			GetAIInterface()->m_canFlee = true;
			if(sp->floatMisc1)
			{
				GetAIInterface()->m_FleeHealth = sp->floatMisc1;
			}
			else
			{
				GetAIInterface()->m_FleeHealth = 0.2f;
			}
			if(sp->Misc2)
			{
				GetAIInterface()->m_FleeDuration = sp->Misc2;
			}
			else
			{
				GetAIInterface()->m_FleeDuration = 10000;
			}
		}
		else if(sp->agent == AGENT_CALLFORHELP)
		{
			GetAIInterface()->m_canCallForHelp = true;
			if(sp->floatMisc1)
				GetAIInterface()->m_CallForHelpHealth = sp->floatMisc1;
			else
				GetAIInterface()->m_CallForHelpHealth = 0.2f;
		}
		else
		{
			GetAIInterface()->addSpellToList(sp);
		}
	} while( result->NextRow() );

	delete result;*/
}

WayPoint * Creature::CreateWaypointStruct()
{
	return new WayPoint();
}
//#define SAFE_FACTIONS

bool Creature::Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info)
{
	m_spawn = spawn;
	proto = CreatureProtoStorage.LookupEntry(spawn->entry);
	if(!proto)
		return false;
	creature_info = CreatureNameStorage.LookupEntry(spawn->entry);
	if(!creature_info)
		return false;
	
	m_walkSpeed = m_base_walkSpeed = proto->walk_speed; //set speeds
	m_runSpeed = m_base_runSpeed = proto->run_speed; //set speeds
	m_flySpeed = proto->fly_speed;

	//Set fields
	SetUInt32Value(OBJECT_FIELD_ENTRY,proto->Id);
	SetFloatValue(OBJECT_FIELD_SCALE_X,proto->Scale);
	
	//SetUInt32Value(UNIT_FIELD_HEALTH, (mode ? long2int32(proto->Health * 1.5)  : proto->Health));
	//SetUInt32Value(UNIT_FIELD_BASE_HEALTH, (mode ? long2int32(proto->Health * 1.5)  : proto->Health));
	//SetUInt32Value(UNIT_FIELD_MAXHEALTH, (mode ? long2int32(proto->Health * 1.5)  : proto->Health));
	uint32 health = proto->MinHealth + RandomUInt(proto->MaxHealth - proto->MinHealth);
	if(mode)
		health = long2int32(double(health) * 1.5);
	SetUInt32Value(UNIT_FIELD_HEALTH, health);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, health);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, health);

	SetUInt32Value(UNIT_FIELD_POWER1,proto->Mana);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1,proto->Mana);
	SetUInt32Value(UNIT_FIELD_BASE_MANA,proto->Mana);
	
	// Whee, thank you blizz, I love patch 2.2! Later on, we can randomize male/female mobs! xD
	// Determine gender (for voices)
	//if(spawn->displayid != creature_info->Male_DisplayID)
	//	setGender(1);   // Female
	
	uint32 model;
	uint32 gender = creature_info->GenerateModelId(&model);
	setGender(gender);

	SetUInt32Value(UNIT_FIELD_DISPLAYID,model);
	SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID,model);
	SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID,proto->MountedDisplayID);

    //SetUInt32Value(UNIT_FIELD_LEVEL, (mode ? proto->Level + (info ? info->lvl_mod_a : 0) : proto->Level));
	SetUInt32Value(UNIT_FIELD_LEVEL, proto->MinLevel + (RandomUInt(proto->MaxLevel - proto->MinLevel)));
	if(mode && info)
		ModUnsigned32Value(UNIT_FIELD_LEVEL, info->lvl_mod_a);

	for(uint32 i = 0; i < 7; ++i)
		SetUInt32Value(UNIT_FIELD_RESISTANCES+i,proto->Resistances[i]);

	SetUInt32Value(UNIT_FIELD_BASEATTACKTIME,proto->AttackTime);
	SetFloatValue(UNIT_FIELD_MINDAMAGE, (mode ? proto->MinDamage * 1.5f  : proto->MinDamage));
	SetFloatValue(UNIT_FIELD_MAXDAMAGE, (mode ? proto->MaxDamage * 1.5f  : proto->MaxDamage));

	SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME,proto->RangedAttackTime);
	SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,proto->RangedMinDamage);
	SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE,proto->RangedMaxDamage);

	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, proto->Item1SlotDisplay);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, proto->Item2SlotDisplay);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_02, proto->Item3SlotDisplay);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, proto->Item1Info1);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, proto->Item1Info2);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, proto->Item2Info1);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, proto->Item2Info2);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_04, proto->Item3Info1);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_05, proto->Item3Info2);

	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, spawn->factionid);
	SetUInt32Value(UNIT_FIELD_FLAGS, spawn->flags);
	SetUInt32Value(UNIT_NPC_EMOTESTATE, spawn->emote_state);
	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, proto->BoundingRadius);
	SetFloatValue(UNIT_FIELD_COMBATREACH, proto->CombatReach);
	original_emotestate = spawn->emote_state;
	// set position
	m_position.ChangeCoords( spawn->x, spawn->y, spawn->z, spawn->o );
	m_spawnLocation.ChangeCoords(spawn->x, spawn->y, spawn->z, spawn->o);
	m_aiInterface->setMoveType(spawn->movetype);	
	m_aiInterface->m_waypoints = objmgr.GetWayPointMap(spawn->id);

	m_faction = dbcFactionTemplate.LookupEntry(spawn->factionid);
	if(m_faction)
	{
		m_factionDBC = dbcFaction.LookupEntry(m_faction->Faction);
		// not a neutral creature
		if(!(m_factionDBC->RepListId == -1 && m_faction->HostileMask == 0 && m_faction->FriendlyMask == 0))
		{
			GetAIInterface()->m_canCallForHelp = true;
		}
	}
	else
	{
		sLog.outString("Warning: Creature %u is missing a valid faction template.\n", spawn->entry);
	}


//SETUP NPC FLAGS
	SetUInt32Value(UNIT_NPC_FLAGS,proto->NPCFLags);

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR ) )
		m_SellItems = objmgr.GetVendorList(GetEntry());

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER ) )
		_LoadQuests();

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TAXIVENDOR) )
		m_TaxiNode = sTaxiMgr.GetNearestTaxiNode( m_position.x, m_position.y, m_position.z, GetMapId() );

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER) || HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_TRAINER_PROF))
		mTrainer = objmgr.GetTrainer(GetEntry());

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_AUCTIONEER ) )
		auctionHouse = sAuctionMgr.GetAuctionHouse(GetEntry());

//NPC FLAGS
	 m_aiInterface->m_waypoints=objmgr.GetWayPointMap(spawn->id);

	//load resistances
	for(uint32 x=0;x<7;x++)
		BaseResistance[x]=GetUInt32Value(UNIT_FIELD_RESISTANCES+x);
	for(uint32 x=0;x<5;x++)
		BaseStats[x]=GetUInt32Value(UNIT_FIELD_STAT0+x);

	BaseDamage[0]=GetFloatValue(UNIT_FIELD_MINDAMAGE);
	BaseDamage[1]=GetFloatValue(UNIT_FIELD_MAXDAMAGE);
	BaseOffhandDamage[0]=GetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE);
	BaseOffhandDamage[1]=GetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE);
	BaseRangedDamage[0]=GetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE);
	BaseRangedDamage[1]=GetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE);
	BaseAttackType=proto->AttackType;

	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);   // better set this one
	SetUInt32Value(UNIT_FIELD_BYTES_0, spawn->bytes);
	SetUInt32Value(UNIT_FIELD_BYTES_2, spawn->bytes2);

////////////AI
	
	// kek
	for(list<AI_Spell*>::iterator itr = proto->spells.begin(); itr != proto->spells.end(); ++itr)
	{
		m_aiInterface->addSpellToList(*itr);
	}
	//m_aiInterface->m_canCallForHelp = proto->m_canCallForHelp;
	//m_aiInterface->m_CallForHelpHealth = proto->m_callForHelpHealth;
	m_aiInterface->m_canFlee = proto->m_canFlee;
	m_aiInterface->m_FleeHealth = proto->m_fleeHealth;
	m_aiInterface->m_FleeDuration = proto->m_fleeDuration;

	//these fields are always 0 in db
	GetAIInterface()->setMoveType(0);
	GetAIInterface()->setMoveRunFlag(0);
	
	// load formation data
	if( spawn->form != NULL )
	{
		m_aiInterface->m_formationLinkSqlId = spawn->form->fol;
		m_aiInterface->m_formationFollowDistance = spawn->form->dist;
		m_aiInterface->m_formationFollowAngle = spawn->form->ang;
	}
	else
	{
		m_aiInterface->m_formationLinkSqlId = 0;
		m_aiInterface->m_formationFollowDistance = 0;
		m_aiInterface->m_formationFollowAngle = 0;
	}

//////////////AI

	myFamily = dbcCreatureFamily.LookupEntry(creature_info->Family);

	
// PLACE FOR DIRTY FIX BASTARDS
	// HACK! set call for help on civ health @ 100%
	if(creature_info->Civilian >= 1)
		m_aiInterface->m_CallForHelpHealth = 100;
 
 //HACK!
	if(m_uint32Values[UNIT_FIELD_DISPLAYID] == 17743 ||
		m_uint32Values[UNIT_FIELD_DISPLAYID] == 20242 ||
		m_uint32Values[UNIT_FIELD_DISPLAYID] == 15435 ||
		(creature_info->Family == UNIT_TYPE_MISC))
	{
		m_useAI = false;
	}

	/* more hacks! */
	if(proto->Mana != 0)
		SetPowerType(POWER_TYPE_MANA);
	else
		SetPowerType(0);

	has_combat_text = objmgr.HasMonsterSay(GetEntry(), MONSTER_SAY_EVENT_ENTER_COMBAT);
	has_waypoint_text = objmgr.HasMonsterSay(GetEntry(), MONSTER_SAY_EVENT_RANDOM_WAYPOINT);
	m_aiInterface->m_isGuard = isGuard(GetEntry());

	m_aiInterface->getMoveFlags();

	/* creature death state */
	if(proto->death_state == 1)
	{
		uint32 newhealth = m_uint32Values[UNIT_FIELD_HEALTH] / 100;
		if(!newhealth)
			newhealth = 1;
		SetUInt32Value(UNIT_FIELD_HEALTH, 1);
		m_limbostate = true;
		bInvincible = true;
		SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DEAD);
	}
	m_invisFlag = proto->invisibility_type;
	if( spawn->stand_state )
		SetStandState( (uint8)spawn->stand_state );

	return true;
}


void Creature::Load(CreatureProto * proto_, float x, float y, float z, float o)
{
	proto = proto_;

	creature_info = CreatureNameStorage.LookupEntry(proto->Id);
	if(!creature_info)
		return;

	m_walkSpeed = m_base_walkSpeed = proto->walk_speed; //set speeds
	m_runSpeed = m_base_runSpeed = proto->run_speed; //set speeds

	//Set fields
	SetUInt32Value(OBJECT_FIELD_ENTRY,proto->Id);
	SetFloatValue(OBJECT_FIELD_SCALE_X,proto->Scale);

	//SetUInt32Value(UNIT_FIELD_HEALTH, (mode ? long2int32(proto->Health * 1.5)  : proto->Health));
	//SetUInt32Value(UNIT_FIELD_BASE_HEALTH, (mode ? long2int32(proto->Health * 1.5)  : proto->Health));
	//SetUInt32Value(UNIT_FIELD_MAXHEALTH, (mode ? long2int32(proto->Health * 1.5)  : proto->Health));
	uint32 health = proto->MinHealth + RandomUInt(proto->MaxHealth - proto->MinHealth);

	SetUInt32Value(UNIT_FIELD_HEALTH, health);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, health);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, health);

	SetUInt32Value(UNIT_FIELD_POWER1,proto->Mana);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1,proto->Mana);
	SetUInt32Value(UNIT_FIELD_BASE_MANA,proto->Mana);

	uint32 model;
	uint32 gender = creature_info->GenerateModelId(&model);
	setGender(gender);

	SetUInt32Value(UNIT_FIELD_DISPLAYID,model);
	SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID,model);
	SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID,proto->MountedDisplayID);

	//SetUInt32Value(UNIT_FIELD_LEVEL, (mode ? proto->Level + (info ? info->lvl_mod_a : 0) : proto->Level));
	SetUInt32Value(UNIT_FIELD_LEVEL, proto->MinLevel + (RandomUInt(proto->MaxLevel - proto->MinLevel)));

	for(uint32 i = 0; i < 7; ++i)
		SetUInt32Value(UNIT_FIELD_RESISTANCES+i,proto->Resistances[i]);

	SetUInt32Value(UNIT_FIELD_BASEATTACKTIME,proto->AttackTime);
	SetFloatValue(UNIT_FIELD_MINDAMAGE, proto->MinDamage);
	SetFloatValue(UNIT_FIELD_MAXDAMAGE, proto->MaxDamage);

	SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME,proto->RangedAttackTime);
	SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,proto->RangedMinDamage);
	SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE,proto->RangedMaxDamage);

	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, proto->Item1SlotDisplay);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, proto->Item2SlotDisplay);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_02, proto->Item3SlotDisplay);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, proto->Item1Info1);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, proto->Item1Info2);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, proto->Item2Info1);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, proto->Item2Info2);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_04, proto->Item3Info1);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_05, proto->Item3Info2);

	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, proto->Faction);
	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, proto->BoundingRadius);
	SetFloatValue(UNIT_FIELD_COMBATREACH, proto->CombatReach);
	original_emotestate = 0;
	// set position

	m_position.ChangeCoords( x, y, z, o );
	m_spawnLocation.ChangeCoords(x, y, z, o);
	m_faction = dbcFactionTemplate.LookupEntry(proto->Faction);

	if(m_faction)
	{
		m_factionDBC = dbcFaction.LookupEntry(m_faction->Faction);
		// not a neutral creature
		if(!(m_factionDBC->RepListId == -1 && m_faction->HostileMask == 0 && m_faction->FriendlyMask == 0))
		{
			GetAIInterface()->m_canCallForHelp = true;
		}
	}

	//SETUP NPC FLAGS
	SetUInt32Value(UNIT_NPC_FLAGS,proto->NPCFLags);

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR ) )
		m_SellItems = objmgr.GetVendorList(GetEntry());

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER ) )
		_LoadQuests();

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TAXIVENDOR) )
		m_TaxiNode = sTaxiMgr.GetNearestTaxiNode( m_position.x, m_position.y, m_position.z, GetMapId() );

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER) || HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_TRAINER_PROF))
		mTrainer = objmgr.GetTrainer(GetEntry());

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_AUCTIONEER ) )
		auctionHouse = sAuctionMgr.GetAuctionHouse(GetEntry());

	//load resistances
	for(uint32 x=0;x<7;x++)
		BaseResistance[x]=GetUInt32Value(UNIT_FIELD_RESISTANCES+x);
	for(uint32 x=0;x<5;x++)
		BaseStats[x]=GetUInt32Value(UNIT_FIELD_STAT0+x);

	BaseDamage[0]=GetFloatValue(UNIT_FIELD_MINDAMAGE);
	BaseDamage[1]=GetFloatValue(UNIT_FIELD_MAXDAMAGE);
	BaseOffhandDamage[0]=GetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE);
	BaseOffhandDamage[1]=GetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE);
	BaseRangedDamage[0]=GetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE);
	BaseRangedDamage[1]=GetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE);
	BaseAttackType=proto->AttackType;

	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);   // better set this one

	////////////AI

	// kek
	for(list<AI_Spell*>::iterator itr = proto->spells.begin(); itr != proto->spells.end(); ++itr)
	{
		m_aiInterface->addSpellToList(*itr);
	}
	m_aiInterface->m_canCallForHelp = proto->m_canCallForHelp;
	m_aiInterface->m_CallForHelpHealth = proto->m_callForHelpHealth;
	m_aiInterface->m_canFlee = proto->m_canFlee;
	m_aiInterface->m_FleeHealth = proto->m_fleeHealth;
	m_aiInterface->m_FleeDuration = proto->m_fleeDuration;

	//these fields are always 0 in db
	GetAIInterface()->setMoveType(0);
	GetAIInterface()->setMoveRunFlag(0);

	// load formation data
	m_aiInterface->m_formationLinkSqlId = 0;
	m_aiInterface->m_formationFollowDistance = 0;
	m_aiInterface->m_formationFollowAngle = 0;

	//////////////AI

	myFamily = dbcCreatureFamily.LookupEntry(creature_info->Family);


	// PLACE FOR DIRTY FIX BASTARDS
	// HACK! set call for help on civ health @ 100%
	if(creature_info->Civilian >= 1)
		m_aiInterface->m_CallForHelpHealth = 100;

	//HACK!
	if(m_uint32Values[UNIT_FIELD_DISPLAYID] == 17743 ||
		m_uint32Values[UNIT_FIELD_DISPLAYID] == 20242 ||
		m_uint32Values[UNIT_FIELD_DISPLAYID] == 15435 ||
		(creature_info->Family == UNIT_TYPE_MISC))
	{
		m_useAI = false;
	}

	/* more hacks! */
	if(proto->Mana != 0)
		SetPowerType(POWER_TYPE_MANA);
	else
		SetPowerType(0);

	has_combat_text = objmgr.HasMonsterSay(GetEntry(), MONSTER_SAY_EVENT_ENTER_COMBAT);
	has_waypoint_text = objmgr.HasMonsterSay(GetEntry(), MONSTER_SAY_EVENT_RANDOM_WAYPOINT);
	m_aiInterface->m_isGuard = isGuard(GetEntry());

	m_aiInterface->getMoveFlags();

	/* creature death state */
	if(proto->death_state == 1)
	{
		uint32 newhealth = m_uint32Values[UNIT_FIELD_HEALTH] / 100;
		if(!newhealth)
			newhealth = 1;
		SetUInt32Value(UNIT_FIELD_HEALTH, 1);
		m_limbostate = true;
		bInvincible = true;
		SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DEAD);
	}
	m_invisFlag = proto->invisibility_type;
}

void Creature::OnPushToWorld()
{
	if(proto)
	{
		set<uint32>::iterator itr = proto->start_auras.begin();
		SpellEntry * sp;
		for(; itr != proto->start_auras.end(); ++itr)
		{
			sp = dbcSpell.LookupEntry((*itr));
			if(sp == 0) continue;

			CastSpell(this, sp, 0);
		}
	}
	LoadScript();
	Unit::OnPushToWorld();

	if(_myScriptClass)
		_myScriptClass->OnLoad();

	if(m_spawn)
	{
		if(m_aiInterface->m_formationLinkSqlId)
		{
			// add event
			sEventMgr.AddEvent(this, &Creature::FormationLinkUp, m_aiInterface->m_formationLinkSqlId,
				EVENT_CREATURE_FORMATION_LINKUP, 1000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			haslinkupevent = true;
		}

		if(m_spawn->channel_target_creature)
		{
			sEventMgr.AddEvent(this, &Creature::ChannelLinkUpCreature, m_spawn->channel_target_creature, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0);	// only 5 attempts
		}
		
		if(m_spawn->channel_target_go)
		{
			sEventMgr.AddEvent(this, &Creature::ChannelLinkUpGO, m_spawn->channel_target_go, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0);	// only 5 attempts
		}
	}

	m_aiInterface->m_is_in_instance = (m_mapMgr->GetMapInfo()->type!=INSTANCE_NULL) ? true : false;
	if (this->HasItems())
	{
		for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
		{
				if (itr->max_amount == 0)
					itr->available_amount=0;
				else if (itr->available_amount<itr->max_amount)				
					sEventMgr.AddEvent(this, &Creature::UpdateItemAmount, itr->itemid, EVENT_ITEM_UPDATE, VENDOR_ITEMS_UPDATE_TIME, 1,0);
		}

	}
}

// this is used for guardians. They are non respawnable creatures linked to a player
void Creature::SummonExpire()
{
	RemoveFromWorld(false, true);
	SafeDelete();//delete creature totaly.
}

void Creature::Despawn(uint32 delay, uint32 respawntime)
{
	if(delay)
	{
		sEventMgr.AddEvent(this, &Creature::Despawn, (uint32)0, respawntime, EVENT_CREATURE_RESPAWN, delay, 1,0);
		return;
	}

	if(!IsInWorld())
		return;

	if(respawntime)
	{
		/* get the cell with our SPAWN location. if we've moved cell this might break :P */
		MapCell * pCell = m_mapMgr->GetCellByCoords(m_spawnLocation.x, m_spawnLocation.y);
		if(!pCell)
			pCell = m_mapCell;
	
		ASSERT(pCell);
		pCell->_respawnObjects.insert(((Object*)this));
		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnCreature, this, pCell, EVENT_CREATURE_RESPAWN, respawntime, 1, 0);
		Unit::RemoveFromWorld(false);
		m_position = m_spawnLocation;
		m_respawnCell=pCell;
	}
	else
	{
		Unit::RemoveFromWorld(true);
		SafeDelete();
	}
}

void Creature::TriggerScriptEvent(string func)
{
	if( _myScriptClass )
		_myScriptClass->StringFunctionCall( func.c_str() );
}

void Creature::DestroyCustomWaypointMap()
{
	if(m_custom_waypoint_map)
	{
		for(WayPointMap::iterator itr = m_custom_waypoint_map->begin(); itr != m_custom_waypoint_map->end(); ++itr)
		{
			delete (*itr);
		}
		delete m_custom_waypoint_map;
		m_custom_waypoint_map = 0;
		m_aiInterface->SetWaypointMap(0);
	}
}

void Creature::RemoveLimboState(Unit * healer)
{
	if(!m_limbostate != true)
		return;

	m_limbostate = false;
	SetUInt32Value(UNIT_NPC_EMOTESTATE, m_spawn ? m_spawn->emote_state : 0);
	SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	bInvincible = false;
}

// Generates 3 random waypoints around the NPC
void Creature::SetGuardWaypoints()
{
	if(!GetMapMgr()) return;
	if(!GetCreatureName()) return;

	GetAIInterface()->setMoveType(1);
	for(int i = 1; i <= 4; i++)
	{
		float ang = rand()/100.0f;
		float ran = (rand()%(100))/10.0f;
		while(ran < 1)
			ran = (rand()%(100))/10.0f;

		WayPoint * wp = new WayPoint;
		wp->id = i;
		wp->flags = 0;
		wp->waittime = 800;  /* these guards are antsy :P */
		wp->x = GetSpawnX()+ran*sin(ang);
		wp->y = GetSpawnY()+ran*cos(ang);
#ifdef COLLISION
		wp->z = CollideInterface.GetHeight(m_mapId, wp->x, wp->y, m_spawnLocation.z + 2.0f);
		if( wp->z == NO_WMO_HEIGHT )
			wp->z = m_mapMgr->GetLandHeight(wp->x, wp->y);

		if( fabs( wp->z - m_spawnLocation.z ) > 10.0f )
			wp->z = m_spawnLocation.z;
#else
		wp->z = GetMapMgr()->GetLandHeight(wp->x, wp->y);
#endif		// COLLISION
		wp->o = 0;
		wp->backwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->forwardemoteoneshot = 0;
		wp->backwardskinid = m_uint32Values[UNIT_FIELD_NATIVEDISPLAYID];
		wp->forwardskinid = m_uint32Values[UNIT_FIELD_NATIVEDISPLAYID];
		GetAIInterface()->addWayPoint(wp);
	}
}
