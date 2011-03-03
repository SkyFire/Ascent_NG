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

#define GROWL_RANK_1 2649
#define GROWL_RANK_2 14916
#define WATER_ELEMENTAL 510

uint32 GetAutoCastTypeForSpell(SpellEntry * ent)
{
	switch(ent->NameHash)
	{

	/************************************************************************/
	/* Warlock Pet Spells													*/
	/************************************************************************/

	case SPELL_HASH_FIREBOLT:			// Firebolt
	case SPELL_HASH_LASH_OF_PAIN:		// Lash of Pain
	case SPELL_HASH_TORMENT:			// Torment
	case SPELL_HASH_SUFFERING:
	case SPELL_HASH_SOOTHING_KISS:
	case SPELL_HASH_SEDUCTION:
	case SPELL_HASH_CLEAVE:
	case SPELL_HASH_INTERCEPT:
	case SPELL_HASH_DEVOUR_MAGIC:
	case SPELL_HASH_SPELL_LOCK:

		return AUTOCAST_EVENT_ATTACK;
		break;

	case SPELL_HASH_BLOOD_PACT:			// Blood Pact
	case SPELL_HASH_AVOIDANCE:
	case SPELL_HASH_PARANOIA:
		return AUTOCAST_EVENT_ON_SPAWN;
		break;

	case SPELL_HASH_FIRE_SHIELD:		// Fire Shield
		return AUTOCAST_EVENT_OWNER_ATTACKED;
		break;
		
	case SPELL_HASH_PHASE_SHIFT:		// Phase Shift
	case SPELL_HASH_CONSUME_SHADOWS:
	case SPELL_HASH_LESSER_INVISIBILITY:
		return AUTOCAST_EVENT_LEAVE_COMBAT;
		break;

	/************************************************************************/
	/* Hunter Pet Spells													*/
	/************************************************************************/

	case SPELL_HASH_BITE:				// Bite
	case SPELL_HASH_CHARGE:				// Charge
	case SPELL_HASH_CLAW:				// Claw
	case SPELL_HASH_COWER:				// Cower
	case SPELL_HASH_DASH:				// Dash
	case SPELL_HASH_DIVE:				// Dive 
	case SPELL_HASH_FIRE_BREATH:		// Fire Breath
	case SPELL_HASH_FURIOUS_HOWL:		// Furious Howl
	case SPELL_HASH_GORE:				// Gore
	case SPELL_HASH_GROWL:				// Growl
	case SPELL_HASH_LIGHTNING_BREATH:	// Lightning Breath
	case SPELL_HASH_POISON_SPIT:		// Poison Spit 
	case SPELL_HASH_PROWL:				// Prowl
	case SPELL_HASH_SCORPID_POISON:		// Scorpid Poison
	case SPELL_HASH_SCREECH:			// Screech
	case SPELL_HASH_SHELL_SHIELD:		// Shell Shield
	case SPELL_HASH_THUNDERSTOMP:		// Thunderstomp
	case SPELL_HASH_WARP:				// Warp 
		return AUTOCAST_EVENT_ATTACK;
		break;

	/************************************************************************/
	/* Mage Pet Spells														*/
	/************************************************************************/

	case SPELL_HASH_WATERBOLT:			// Waterbolt
		return AUTOCAST_EVENT_ATTACK;
		break;
	}

	return AUTOCAST_EVENT_NONE;
}

void Pet::CreateAsSummon(uint32 entry, CreatureInfo *ci, Creature* created_from_creature, Unit* owner, SpellEntry* created_by_spell, uint32 type, uint32 expiretime)
{
	if(ci == NULL)
		return;
	SetIsPet(true);

	m_OwnerGuid = owner->GetGUID();
	m_Owner = TO_PLAYER(owner);
	m_OwnerGuid = m_Owner->GetGUID();
	creature_info = ci;
	myFamily = dbcCreatureFamily.LookupEntry(creature_info->Family);
	//m_name = objmgr.GetCreatureFamilyName(myFamily->ID);
	if( myFamily->name == NULL )
		m_name = "Pet";
	else
		m_name.assign( myFamily->name );

	// Create ourself	
	Create(m_name.c_str(), owner->GetMapId(), owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), owner->GetOrientation());
	SetUInt32Value(OBJECT_FIELD_ENTRY, entry);
	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);	// better set this one

	// Fields common to both lock summons and pets
	uint32 level = (m_Owner->GetUInt32Value( UNIT_FIELD_LEVEL ) - 5);
	if( type & 0x2 && created_from_creature != NULL && created_from_creature->getLevel() > level)
	{
		level = created_from_creature->getLevel();
	}

	SetUInt32Value( UNIT_FIELD_LEVEL, level);

	SetUInt32Value(UNIT_FIELD_DISPLAYID,  ci->Male_DisplayID);
	SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, ci->Male_DisplayID);
	SetUInt64Value(UNIT_FIELD_SUMMONEDBY, owner->GetGUID());
	SetUInt64Value(UNIT_FIELD_CREATEDBY, owner->GetGUID());
	
	if(type & 0x1 && created_by_spell != NULL)
		SetUInt64Value(UNIT_CREATED_BY_SPELL, created_by_spell->Id);

	if(type & 0x1 || created_from_creature == NULL)
	{
		Summon = true;
		SetUInt32Value(UNIT_FIELD_BYTES_0, 2048 | (0 << 24));
		SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
		SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 2000);
		SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME, 2000);
		SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.5f);
		SetFloatValue(UNIT_FIELD_COMBATREACH, 0.75f);
		SetUInt32Value(UNIT_FIELD_BYTES_2, (0x01 | (0x2 << 24)));
		SetUInt32Value(UNIT_FIELD_PETNUMBER, GetUIdFromGUID());
		SetPowerType(POWER_TYPE_MANA);
		if(entry == WATER_ELEMENTAL)
			m_name = "Water Elemental";
		else if( entry == 19668)
			m_name = "Shadowfiend";
		else
			m_name = sWorld.GenerateName();

	} else {
		SetUInt32Value(UNIT_FIELD_BYTES_0, 2048 | (0 << 24));
		SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 2000);
		SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME, 2000); // Supalosa: 2.00 normalized attack speed
		
		// hacks D: allow correct creation of hunter pets via gm command
		if(created_from_creature == this)
		{
			SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.5f);
			SetFloatValue(UNIT_FIELD_COMBATREACH, 0.75f);
		}
		else
		{
		SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, created_from_creature->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS));
		SetFloatValue(UNIT_FIELD_COMBATREACH, created_from_creature->GetFloatValue(UNIT_FIELD_COMBATREACH));
		}

		// These need to be checked.
		SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_COMBAT); // why combat ??
		SetUInt32Value(UNIT_FIELD_POWER5, PET_HAPPINESS_UPDATE_VALUE >> 1);//happiness
		SetUInt32Value(UNIT_FIELD_MAXPOWER5, 1000000);
		SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
		SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, GetNextLevelXP(getLevel()));
		SetUInt32Value(UNIT_FIELD_BYTES_1, 0);

		// Focus
		SetUInt32Value(UNIT_FIELD_POWER3, 100);
		SetUInt32Value(UNIT_FIELD_MAXPOWER3, 100);

		// 0x3 -> Enable pet rename.
		SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x3 << 16));

		// Change the power type to FOCUS
		SetPowerType(POWER_TYPE_FOCUS);

		// create our spells
		SetDefaultSpells();

		InitTalentsForLevel(true);
	}

	// Apply stats.
	ApplyStatsForLevel();

	BaseDamage[0]=GetFloatValue(UNIT_FIELD_MINDAMAGE);
	BaseDamage[1]=GetFloatValue(UNIT_FIELD_MAXDAMAGE);
	BaseOffhandDamage[0]=GetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE);
	BaseOffhandDamage[1]=GetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE);
	BaseRangedDamage[0]=GetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE);
	BaseRangedDamage[1]=GetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE);

	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	m_PartySpellsUpdateTimer = 0;

	m_PetNumber = TO_PLAYER(owner)->GeneratePetNumber();
	SetUInt32Value(UNIT_FIELD_PETNUMBER, GetUIdFromGUID());

	m_ExpireTime = expiretime;
	bExpires = m_ExpireTime > 0 ? true : false;

	if(!bExpires)
	{
		// Create PlayerPet struct (Rest done by UpdatePetInfo)
		PlayerPet *pp = new PlayerPet;
		pp->number = m_PetNumber;
		pp->stablestate = STABLE_STATE_ACTIVE;
		TO_PLAYER(owner)->AddPlayerPet(pp, pp->number);
	}	

	//maybe we should use speed from the template we created the creature ?
	m_base_runSpeed = m_runSpeed = owner->m_base_runSpeed; //should we be able to keep up with master ?
	m_base_walkSpeed = m_walkSpeed = owner->m_base_walkSpeed; //should we be able to keep up with master ?

	InitializeMe(true);
}


Pet::Pet(uint64 guid) : Creature(guid)
{
	m_PetXP = 0;
	Summon = false;
	std::fill(&ActionBar[0], &ActionBar[10], 0);
	//memset(ActionBar, 0, sizeof(uint32)*10);

	m_AutoCombatSpell = 0;

	m_PartySpellsUpdateTimer = 0;
	m_HappinessTimer = PET_HAPPINESS_UPDATE_TIMER;
	m_PetNumber = 0;

	m_State = PET_STATE_DEFENSIVE;
	m_Action = PET_ACTION_FOLLOW;
	m_ExpireTime = 0;
	bExpires = false;
	m_Diet = 0;
	m_Action = PET_ACTION_FOLLOW;
	m_State = PET_STATE_DEFENSIVE;
	m_dismissed = false;
	m_AISpellStore.clear();
	mSpells.clear();
	m_talents.clear();
}

Pet::~Pet()
{

	mSpells.clear();

	for(std::map<uint32, AI_Spell*>::iterator itr = m_AISpellStore.begin(); itr != m_AISpellStore.end(); ++itr)
		delete itr->second;
	m_AISpellStore.clear();

	if(IsInWorld())
		Remove(false, true, true);

	if( m_Owner )
	{
		m_Owner->SetSummon(NULL);
		ClearPetOwner();
	}

	//TODO: clean this up in _SavePet ->  m_talents.clear();
	//add spent talents to player pet struct
	//drop unused fields from the db
	//m_talents.clear();
}

void Pet::Init()
{
	SetUInt32Value(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER + POWER_TYPE_FOCUS, 20);
	Creature::Init();
}

void Pet::Destructor()
{
	delete this;
}

void Pet::Update(uint32 time)
{
	if(m_Owner == NULL)
		return;

	Creature::Update(time); // passthrough

	if(!bExpires)
	{
		//Happiness
		if(m_HappinessTimer == 0)
		{	
			int32 val = GetUInt32Value(UNIT_FIELD_POWER5);
			//amount of burned happiness is loyalty_lvl depended
			int32 burn = 1042;
			if( CombatStatus.IsInCombat() )
				burn = burn >> 1; //in combat reduce burn by half (guessed) 
			if((val - burn)<0)
				val = 0;
			else
				val -= burn;
			SetUInt32Value(UNIT_FIELD_POWER5, val);// Set the value
			m_HappinessTimer = PET_HAPPINESS_UPDATE_TIMER;// reset timer
		} 
		else 
		{
			if( time > m_HappinessTimer )
				m_HappinessTimer = 0;
			else
				m_HappinessTimer -= time;
		}
	}

	if(bExpires)
	{
		if(m_ExpireTime == 0)
		{
			// remove
			Dismiss(false);
			return;
		} else {
			if(time > m_ExpireTime)
			{
				m_ExpireTime = 0;
			} else {
				m_ExpireTime -= time;
			}
		}
	}
}

void Pet::SendSpellsToOwner()
{
	if(m_Owner == NULL)
		return;

	int packetsize = (m_uint32Values[OBJECT_FIELD_ENTRY] != WATER_ELEMENTAL) ? ((int)mSpells.size() * 4 + 68) : 68;
	WorldPacket * data = new WorldPacket(SMSG_PET_SPELLS, packetsize);
	*data << GetGUID();
	*data << uint16(GetCreatureInfo()->Family);// Pet family to display talent tree
	*data << uint32(0x00000101);//unk2
	*data << uint8(m_State);
	*data << uint8(m_Action);
	*data << uint16(0x0000);//unk3

	// Send the actionbar
	for(uint32 i = 0; i < 10; ++i)
	{
		if(ActionBar[i] & 0x4000000)		// Command
			*data << uint32(ActionBar[i]);
		else
		{
			if(uint16(ActionBar[i]))
				*data << uint16(ActionBar[i]) << GetSpellState(ActionBar[i]);
			else
				*data << uint16(0) << uint8(0) << uint8(i+5);
		}
	}

	// we don't send spells for the water elemental so it doesn't show up in the spellbook
	if(m_uint32Values[OBJECT_FIELD_ENTRY] != WATER_ELEMENTAL)	
	{
		// Send the rest of the spells.
		*data << uint8(mSpells.size());
		for(PetSpellMap::iterator itr = mSpells.begin(); itr != mSpells.end(); ++itr)
			*data << uint16(itr->first->Id) << uint16(itr->second);
	}
	*data << uint8(0);	// count
	/*
	for(uint32 i = 0; i < count)
	{
		*data << uint32(0);
		*data << uint16(0);
		*data << uint32(0);
		*data << uint32(0);
	}
	*/

	m_Owner->delayedPackets.add(data);
}

void Pet::SendNullSpellsToOwner()
{
	if(m_Owner == NULL)
		return;

	WorldPacket data(8);
	data.SetOpcode(SMSG_PET_SPELLS);
	data << uint64(0);
	m_Owner->GetSession()->SendPacket(&data);
}

void Pet::InitializeSpells()
{
	for(PetSpellMap::iterator itr = mSpells.begin(); itr != mSpells.end(); ++itr)
	{
		SpellEntry *info = itr->first;
		if(!info)
			continue;

		// Check that the spell isn't passive
		if( info->Attributes & ATTRIBUTES_PASSIVE )
		{
			// Cast on self..
			Spell* sp(new Spell(TO_OBJECT(this), info, true, NULL));
			SpellCastTargets targets(GetGUID());
			sp->prepare(&targets);

			continue;
		}

		AI_Spell*sp = CreateAISpell(info);
		if(itr->second == AUTOCAST_SPELL_STATE)
			SetAutoCast(sp, true);
		else
			SetAutoCast(sp,false);
	}
}

AI_Spell*Pet::CreateAISpell(SpellEntry * info)
{
	// Create an AI_Spell
	map<uint32,AI_Spell*>::iterator itr = m_AISpellStore.find(info->Id);
	if(itr != m_AISpellStore.end())
		return itr->second;

	AI_Spell *sp = new AI_Spell;
	sp->agent = AGENT_SPELL;
	sp->entryId = GetEntry();
	sp->floatMisc1 = 0;
	sp->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(info->rangeIndex));
	sp->minrange = GetMinRange(dbcSpellRange.LookupEntry(info->rangeIndex));
	sp->Misc2 = 0;
	sp->procChance = 0;
	sp->spell = info;
	sp->spellType = STYPE_DAMAGE;
	sp->spelltargetType = TTYPE_SINGLETARGET;
	sp->cooldown = objmgr.GetPetSpellCooldown(info->Id);
	sp->cooldowntime = 0;
	if(info->Effect[0] == SPELL_EFFECT_APPLY_AURA || info->Effect[0] == SPELL_EFFECT_APPLY_AREA_AURA)
		sp->spellType = STYPE_BUFF;

	if(info->EffectImplicitTargetA[0] == EFF_TARGET_IN_FRONT_OF_CASTER)
	{
		float radius = ::GetRadius(dbcSpellRadius.LookupEntry(info->EffectRadiusIndex[0]));
		sp->maxrange = radius;
		sp->spelltargetType = TTYPE_SOURCE;
	}

	sp->autocast_type = GetAutoCastTypeForSpell(info);
	sp->custom_pointer = false;
	sp->procCount=0;
	m_AISpellStore[info->Id] = sp;
	return sp;
}

void Pet::LoadFromDB(Player* owner, PlayerPet * playerPetInfo)
{
	m_Owner = owner;
	m_OwnerGuid = m_Owner->GetGUID();
	m_PlayerPetInfo = playerPetInfo;
	creature_info = CreatureNameStorage.LookupEntry(m_PlayerPetInfo->entry);
	if( creature_info == NULL )
		return;
	myFamily = dbcCreatureFamily.LookupEntry(creature_info->Family);

	Create(playerPetInfo->name.c_str(), owner->GetMapId(), owner->GetPositionX() + 2 , owner->GetPositionY() +2, owner->GetPositionZ(), owner->GetOrientation());

	LoadValues(m_PlayerPetInfo->fields.c_str());
	if(getLevel() == 0)
	{
		m_PlayerPetInfo->level = m_Owner->getLevel();
		SetUInt32Value(UNIT_FIELD_LEVEL, m_PlayerPetInfo->level);
	}
	/*
	SetUInt32Value(OBJECT_FIELD_ENTRY, m_PlayerPetInfo->entry);
	if (m_PlayerPetInfo->level == 0)
		m_PlayerPetInfo->level = m_Owner->getLevel();
	SetUInt32Value(UNIT_FIELD_LEVEL, m_PlayerPetInfo->level);
	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);	// better set this one

	SetUInt32Value(UNIT_FIELD_DISPLAYID, creature_info->Male_DisplayID);
	SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, creature_info->Male_DisplayID);
	SetUInt64Value(UNIT_FIELD_SUMMONEDBY, owner->GetGUID());
	SetUInt64Value(UNIT_FIELD_CREATEDBY, owner->GetGUID());

	SetUInt32Value(UNIT_FIELD_BYTES_0, 2048 | (0 << 24));
	SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 2000);
	SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME, 2000); // Supalosa: 2.00 normalized attack speed
	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 1.0f);//created_from_creature->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS));
	SetFloatValue(UNIT_FIELD_COMBATREACH, 1.0f);//created_from_creature->GetFloatValue(UNIT_FIELD_COMBATREACH));

	// These need to be checked.
	SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_COMBAT); // why combat ??
	SetUInt32Value(UNIT_FIELD_POWER5, m_PlayerPetInfo->happiness );
	SetUInt32Value(UNIT_FIELD_MAXPOWER5, 1000000);
	SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
	SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, GetNextLevelXP(getLevel()));

	// Focus
	SetUInt32Value(UNIT_FIELD_POWER3, 100);
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, 100);

	// 0x3 -> Enable pet rename.
	SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x3 << 16));

	SetUnspentPetTalentPoints(m_PlayerPetInfo->availableTalentPoints);

	// Change the power type to FOCUS
	SetPowerType(POWER_TYPE_FOCUS);
	*/

	ApplyStatsForLevel();

	BaseDamage[0]=GetFloatValue(UNIT_FIELD_MINDAMAGE);
	BaseDamage[1]=GetFloatValue(UNIT_FIELD_MAXDAMAGE);
	BaseOffhandDamage[0]=GetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE);
	BaseOffhandDamage[1]=GetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE);
	BaseRangedDamage[0]=GetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE);
	BaseRangedDamage[1]=GetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE);
	//SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));

	m_PetNumber = m_PlayerPetInfo->number;
	m_PetXP = m_PlayerPetInfo->xp;
	m_name = m_PlayerPetInfo->name;
	Summon = m_PlayerPetInfo->summon;
	SetIsPet(true);
	//SetUInt32Value(UNIT_FIELD_PETNUMBER, GetUIdFromGUID());

	m_HappinessTimer = m_PlayerPetInfo->happinessupdate;
	
	bExpires = false;

	if(m_Owner && getLevel() > m_Owner->getLevel())
	{
		SetUInt32Value(UNIT_FIELD_LEVEL, m_Owner->getLevel());
		SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
		SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, GetNextLevelXP(m_Owner->getLevel()));
	}

	for(uint8 i = 0; i < 10; ++i)
	{
		ActionBar[i] = m_PlayerPetInfo->actionbarspell[i];
	}
	for(uint8 i = 0; i < 10; ++i)
	{
		if(!(ActionBar[i] & 0x4000000) && m_PlayerPetInfo->actionbarspell[i])
		{
			SpellEntry * sp = dbcSpell.LookupEntry(m_PlayerPetInfo->actionbarspell[i]);
			if(!sp)
			{
				ActionBar[i] = m_PlayerPetInfo->actionbarspell[i] = 0;
				continue;
			}
			mSpells[sp] = m_PlayerPetInfo->actionbarspellstate[i];
		}
	}

	InitializeMe(false);
}

void Pet::InitializeMe(bool first)
{
	if( m_Owner->GetSummon() != NULL )
	{
		// 2 pets???!
		m_Owner->GetSummon()->Remove(true, true, true);
		m_Owner->SetSummon( TO_PET(this) );
	}
	else
		m_Owner->SetSummon( TO_PET(this) );

	// set up ai and shit
	GetAIInterface()->Init(TO_UNIT(this) ,AITYPE_PET,MOVEMENTTYPE_NONE,m_Owner);
	GetAIInterface()->SetUnitToFollow(m_Owner);
	GetAIInterface()->SetFollowDistance(3.0f);

	SetCreatureName(CreatureNameStorage.LookupEntry(GetEntry()));
	proto=CreatureProtoStorage.LookupEntry(GetEntry());
	m_Owner->SetUInt64Value(UNIT_FIELD_SUMMON, GetGUID());
	SetUInt32Value(UNIT_FIELD_PETNUMBER, GetUIdFromGUID());
	SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, (uint32)UNIXTIME);
	myFamily = dbcCreatureFamily.LookupEntry(creature_info->Family);
	SetPetDiet();
	_setFaction();
	m_State = 1;		// dont set agro on spawn

	if(GetEntry() == 416)
		m_aiInterface->disable_melee = true;

	ApplyStatsForLevel();
	// Load our spells
	SetDefaultSpells();

	if(Summon)
	{
		// Adds parent +frost spell damage
		if(m_uint32Values[OBJECT_FIELD_ENTRY] == WATER_ELEMENTAL)
		{
			float parentfrost = (float)m_Owner->GetDamageDoneMod(SCHOOL_FROST);
			parentfrost *= 0.40f;
			ModDamageDone[SCHOOL_FROST] = (uint32)parentfrost;
		}
	}
	else
	{
		// Pull from database... :/
		QueryResult * query = CharacterDatabase.Query("SELECT * FROM playerpetspells WHERE ownerguid=%u AND petnumber=%u",
			m_Owner->GetLowGUID(), m_PetNumber);
		if(query)
		{
			do 
			{
				Field * f = query->Fetch();
				SpellEntry* spell = dbcSpell.LookupEntry(f[2].GetUInt32());
				uint16 flags = f[3].GetUInt16();
				if(spell != NULL && mSpells.find(spell) == mSpells.end())
					mSpells.insert ( make_pair( spell, flags ) );

			} while(query->NextRow());
			delete query;
		}
		// Pull from database... :/
		uint8 spentPoints = 0;
		QueryResult * query2 = CharacterDatabase.Query("SELECT * FROM playerpettalents WHERE ownerguid=%u AND petnumber=%u",
			m_Owner->GetLowGUID(), m_PetNumber);
		if(query2)
		{
			do 
			{
				Field * f = query2->Fetch();
				uint32 talentid = f[2].GetUInt32();
				uint8 rank = f[3].GetUInt8();
				if(rank)
				{
					spentPoints += rank;
				}
				else
					spentPoints++;
				if(m_talents.find(talentid) == m_talents.end())
				{
					m_talents.insert( make_pair( talentid, rank ) );
				}

			} while(query2->NextRow());
			delete query2;
		}
		if(spentPoints > GetSpentPetTalentPoints())
			ResetTalents(false);
	}

	InitializeSpells(); 
	PushToWorld(m_Owner->GetMapMgr());

	// Set up default actionbar
	SetDefaultActionbar();

	SendSpellsToOwner();

	m_Owner->smsg_TalentsInfo(true);

	// set to active
	if(!bExpires)
		UpdatePetInfo(false);

	sEventMgr.AddEvent(TO_PET(this), &Pet::HandleAutoCastEvent, uint32(AUTOCAST_EVENT_ON_SPAWN), EVENT_UNK, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	sEventMgr.AddEvent(TO_PET(this), &Pet::HandleAutoCastEvent, uint32(AUTOCAST_EVENT_LEAVE_COMBAT), EVENT_UNK, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Pet::UpdatePetInfo(bool bSetToOffline)
{
	if(bExpires || m_Owner==NULL)
		return;	// don't update expiring pets

	PlayerPet *pi = m_Owner->GetPlayerPet(m_PetNumber);
	pi->active = !bSetToOffline;
	pi->entry = GetEntry();
	std::stringstream ss;
	for( uint32 index = 0; index < UNIT_END; index ++ )
	{
		ss << GetUInt32Value(index) << " ";
	}
	pi->fields = ss.str();
	pi->name = GetName();
	pi->number = m_PetNumber;
	pi->xp = m_PetXP;
	pi->level = GetUInt32Value(UNIT_FIELD_LEVEL);
	pi->happiness = GetUInt32Value( UNIT_FIELD_POWER5 );
	pi->happinessupdate = m_HappinessTimer;

	// save action bar
	for(uint32 i = 0; i < 10; ++i)
	{
		if(ActionBar[i] & 0x4000000) //command
		{
			pi->actionbarspell[i] = ActionBar[i];
			pi->actionbarspellstate[i] = 0;
		}
		else if(ActionBar[i]) //spell
		{
			pi->actionbarspell[i] = ActionBar[i];
			pi->actionbarspellstate[i] = uint32(GetSpellState(ActionBar[i]));
		}
		else //empty
			pi->actionbarspell[i] = 0;
			pi->actionbarspellstate[i] = DEFAULT_SPELL_STATE;
	}

	pi->summon = Summon;
}

void Pet::Dismiss(bool bSafeDelete)//Abandon pet
{
	// already deleted
	if( m_dismissed )
		return;

	// Delete any petspells for us.
	if( !bExpires )
	{
		if(!Summon && m_Owner)
		{
			CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE ownerguid=%u AND petnumber=%u",
				m_Owner->GetLowGUID(), m_PetNumber);
			CharacterDatabase.Execute("DELETE FROM playerpettalents WHERE ownerguid=%u AND petnumber=%u",
				m_Owner->GetLowGUID(), m_PetNumber);
		}

		if(m_Owner != NULL)
		{
			m_Owner->RemovePlayerPet( m_PetNumber );
		}
	}

	// find out playerpet entry, delete it
	Remove(bSafeDelete, false, true);
}

void Pet::Remove(bool bSafeDelete, bool bUpdate, bool bSetOffline)
{
	if( m_dismissed )
		return;

	RemoveAllAuras(); // Prevent pet overbuffing
	if(m_Owner)
	{
		// remove association with player
		m_Owner->SetUInt64Value(UNIT_FIELD_SUMMON, 0);

		if(bUpdate) 
		{
			if(!bExpires) 
				UpdatePetInfo(bSetOffline);
			if(!IsSummonedPet() && !bExpires)
				m_Owner->_SavePet(NULL);//not perfect but working
		}
		m_Owner->SetSummon(NULL);
		SendNullSpellsToOwner();
	}
	ClearPetOwner();
/*	if(bSafeDelete)
		
	else*/
/*		PetSafeDelete();*/

	// has to be next loop - reason because of RemoveFromWorld, iterator gets broke.
	/*if(IsInWorld() && Active) Deactivate(m_mapMgr);*/
	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent(TO_PET(this), &Pet::PetSafeDelete, EVENT_CREATURE_SAFE_DELETE, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	m_dismissed = true;
}

void Pet::PetSafeDelete()
{
	if(IsInWorld())
	{
		// remove from world, and delete
		RemoveFromWorld(false, false);
	}

	//sEventMgr.AddEvent(World::getSingletonPtr(), &World::DeleteObject, TO_OBJECT(this), EVENT_CREATURE_SAFE_DELETE, 1000, 1);
	Creature::SafeDelete();
}

void Pet::DelayedRemove(bool bTime, bool bDeath)
{
	if(GetMapMgr()!= NULL)
		m_Owner = GetMapMgr()->GetPlayer((uint32)m_OwnerGuid);
	if(bTime)
	{
		if(GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0 || bDeath)
			Dismiss(true);  // remove us..
		else
			Remove(true, true, true);
	}
	else
		sEventMgr.AddEvent(TO_PET(this), &Pet::DelayedRemove, true, bDeath, EVENT_PET_DELAYED_REMOVE, PET_DELAYED_REMOVAL_TIME, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Pet::GiveXP( uint32 xp )
{
	if( m_Owner == NULL || Summon )
		return;	
	
	if( getLevel() >= m_Owner->getLevel() )		//pet do not get xp if its level >= owners level
		return;

	xp += m_uint32Values[UNIT_FIELD_PETEXPERIENCE];
	uint32 nxp = m_uint32Values[UNIT_FIELD_PETNEXTLEVELEXP];
	bool changed = false;

	while(xp >= nxp)
	{
		ModUnsigned32Value(UNIT_FIELD_LEVEL, 1);
		xp -= nxp;
		nxp = GetNextLevelXP(m_uint32Values[UNIT_FIELD_LEVEL]);
		changed = true;
	}

	SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, xp);
	SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, nxp);
	if(changed)
	{
		ApplyStatsForLevel();
		InitTalentsForLevel();
	}
}

uint32 Pet::GetNextLevelXP(uint32 currentlevel)
{
	// Calculate next level XP
	uint32 nextLvlXP = 0;
	uint32 Level = currentlevel + 1;

	// copy and paste :D taken from objectmgr.cpp
	// we'll just grab the xp from this table and divide it, see note below about
	// pet xp calculation
	// this is a fixed table taken from 2.3.0 wow. This can't get more blizzlike with the "if" cases ;)
	if( ( currentlevel ) < MAX_PREDEFINED_NEXTLEVELXP )
	{
		nextLvlXP = NextLevelXp[( currentlevel )];
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
	// Source: http://www.wow-petopia.com/ Previously, pets needed 1/6th (or about 16%) of the experience
	// that a character of the same level would need.
	// Now they only need 1/10th (or about 10%) -- which is a 66% improvement! 
	double xp = double(nextLvlXP) / 10.0;
	return FL2UINT(xp);
}

void Pet::LevelUpTo(uint32 level)
{
	uint32 currentlevel = getLevel();
	uint32 XPtoGive = 0;
	while(level > currentlevel)
	{
		XPtoGive += NextLevelXp[currentlevel];
		currentlevel++;
	}
	GiveXP(XPtoGive);

}

void Pet::SetDefaultSpells()
{
	if(Summon)
	{
		// this one's easy :p we just pull em from the owner.
		map<uint32, set<uint32> >::iterator it1;
		set<uint32>::iterator it2;
		it1 = m_Owner->SummonSpells.find(GetEntry());
		if(it1 != m_Owner->SummonSpells.end())
		{
			it2 = it1->second.begin();
			for(; it2 != it1->second.end(); ++it2)
			{
				AddSpell(dbcSpell.LookupEntry(*it2), false, false);
			}
		}
	}
	else
	{

		uint32 Line = GetCreatureInfo()->SpellDataID;
		if(Line)
		{
			CreatureSpellDataEntry * SpellData = dbcCreatureSpellData.LookupEntry(Line);
			if(SpellData)
				for(uint32 i = 0; i < 3; ++i)
					if(SpellData->Spells[i] != 0)
						AddSpell(dbcSpell.LookupEntry(SpellData->Spells[i]), false, false); //add spell to pet
		}
	}
}

void Pet::AddSpell(SpellEntry * sp, bool learning, bool sendspells)
{
	// Cast on self if we're a passive spell
	if( sp->Attributes & ATTRIBUTES_PASSIVE )
	{
		if(IsInWorld())
		{
			Spell* spell(new Spell(TO_OBJECT(this), sp, true, NULL));
			SpellCastTargets targets(GetGUID());
			spell->prepare(&targets);
			mSpells[sp] = 0x0100;
		}	
	}
	else
	{
	   // Active spell add to the actionbar.
		bool has=false;
		for(int i = 0; i < 10; ++i)
		{
			if(ActionBar[i] == sp->Id)
			{
				has=true;
				break;
			}
		}

		if(!has)
		{
			for(int i = 0; i < 10; ++i)
			{
				if(ActionBar[i] == 0)
				{
					ActionBar[i] = sp->Id;
					break;
				}
			}
		}

		bool done=false;
		if(learning)
		{
			for(PetSpellMap::iterator itr = mSpells.begin(); itr != mSpells.end(); ++itr)
			{
				if(sp->NameHash == itr->first->NameHash)
				{
					// replace the spell in the action bar
					for(int i = 0; i < 10; ++i)
					{
						if(ActionBar[i] == itr->first->Id)
						{
							ActionBar[i] = sp->Id;
							break;
						}
					}

					// Create the AI_Spell
					AI_Spell*asp = CreateAISpell(sp);

					// apply the spell state
					uint16 ss = GetSpellState(itr->first);
					mSpells[sp] = ss;
					if(ss==AUTOCAST_SPELL_STATE)
						SetAutoCast(asp, true);

					if(asp->autocast_type==AUTOCAST_EVENT_ON_SPAWN)
						CastSpell(TO_UNIT(this), sp, false);

					RemoveSpell(itr->first);
					done=true;
					break;
				}
			}
		}

		if(done==false)
		{
			if(mSpells.find(sp) != mSpells.end())
				return;

			if(learning)
			{
				AI_Spell*asp = CreateAISpell(sp);
				uint16 ss = (asp->autocast_type > 0) ? AUTOCAST_SPELL_STATE : DEFAULT_SPELL_STATE;
				mSpells[sp] = ss;
				if(ss==AUTOCAST_SPELL_STATE)
					SetAutoCast(asp,true);

				if(asp->autocast_type==AUTOCAST_EVENT_ON_SPAWN)
					CastSpell(TO_UNIT(this), sp, false);
			}
			else
				mSpells[sp] = DEFAULT_SPELL_STATE;
		}
	}
	if(IsInWorld() && sendspells)
		SendSpellsToOwner();
}

void Pet::SetSpellState(SpellEntry* sp, uint16 State)
{
	PetSpellMap::iterator itr = mSpells.find(sp);
	if(itr == mSpells.end())
		return;

	uint16 oldstate = itr->second;
	itr->second = State;

	if(State == AUTOCAST_SPELL_STATE || oldstate == AUTOCAST_SPELL_STATE)
	{
		AI_Spell*sp2 = GetAISpellForSpellId(sp->Id);
		if(sp2)
		{
			if(State == AUTOCAST_SPELL_STATE)
				SetAutoCast(sp2, true);
			else
				SetAutoCast(sp2,false);
		}
	}		
}

uint16 Pet::GetSpellState(SpellEntry* sp)
{
	PetSpellMap::iterator itr = mSpells.find(sp);
	if(itr == mSpells.end())
		return DEFAULT_SPELL_STATE;

	return itr->second;
}

void Pet::SetDefaultActionbar()
{
	// Set up the default actionbar.
	ActionBar[0] = PET_SPELL_ATTACK;
	ActionBar[1] = PET_SPELL_FOLLOW;
	ActionBar[2] = PET_SPELL_STAY;

	// Fill up 4 slots with our spells
	if(mSpells.size() > 0)
	{
		PetSpellMap::iterator itr = mSpells.begin();
		uint32 pos = 0;
		for(; itr != mSpells.end() && pos < 4; ++itr, ++pos)
			ActionBar[3+pos] = itr->first->Id;
	}

	ActionBar[7] = PET_SPELL_AGRESSIVE;
	ActionBar[8] = PET_SPELL_DEFENSIVE;
	ActionBar[9] = PET_SPELL_PASSIVE;
}

void Pet::WipeSpells()
{
	while( mSpells.size() > 0 )
	{
		RemoveSpell( mSpells.begin()->first );
	}
	SendSpellsToOwner();
}

void Pet::LearnSpell(uint32 spellid)
{
	SpellEntry * sp = dbcSpell.LookupEntry(spellid);
	if(!sp)
		return;

	AddSpell(sp, true);

	if(m_Owner && m_Owner->GetTypeId() == TYPEID_PLAYER)
	{
		WorldPacket data(2);
		data.SetOpcode(SMSG_PET_LEARNED_SPELL);
		data << uint16(spellid);
		m_Owner->GetSession()->SendPacket(&data);
	}
}

void Pet::LearnLevelupSpells()
{
	uint32 family_id = Summon ? m_uint32Values[OBJECT_FIELD_ENTRY] : myFamily->ID;

	PetLevelupSpellSet const *levelupSpells = objmgr.GetPetLevelupSpellList(family_id);
	if(levelupSpells)
	{
		uint32 petlevel = getLevel();
		std::map<uint32, uint32>::const_iterator itr = levelupSpells->find(petlevel);
		if(itr != levelupSpells->end())
			LearnSpell(itr->second);
	}
}

void Pet::RemoveSpell(SpellEntry * sp)
{
	mSpells.erase(sp);
	map<uint32, AI_Spell*>::iterator itr = m_AISpellStore.find(sp->Id);
	if(itr != m_AISpellStore.end())
	{
		for(list<AI_Spell*>::iterator it = m_aiInterface->m_spells.begin(); it != m_aiInterface->m_spells.end(); ++it)
		{
			if((*it) == itr->second)
			{
				/*if((*it)->autocast_type > 0)
					m_autoCastSpells[(*it)->autocast_type].remove((*it));*/
				if((*it)->autocast_type > 0)
				{
					for(list<AI_Spell*>::iterator i3 = m_autoCastSpells[(*it)->autocast_type].begin();
						i3 != m_autoCastSpells[(*it)->autocast_type].end(); ++i3)
					{
						if( (*i3) == itr->second )
						{
							m_autoCastSpells[(*it)->autocast_type].erase(i3);
							break;
						}
					}
				}

				m_aiInterface->m_spells.erase(it);
				m_aiInterface->CheckNextSpell(itr->second);
				break;
			}
		}

		delete itr->second;
		m_AISpellStore.erase(itr);
	}
	else
	{
		for(list<AI_Spell*>::iterator it = m_aiInterface->m_spells.begin(); it != m_aiInterface->m_spells.end(); ++it)
		{
			if((*it)->spell == sp)
			{
				// woot?
				AI_Spell*spe = *it;
				m_aiInterface->m_spells.erase(it);
				delete spe;
				break;
			}
		}
	}
	//Remove spell from action bar as well
	for( uint32 pos = 0; pos < 10; pos++ )
	{
		if( ActionBar[pos] == sp->Id )
			ActionBar[pos] = 0;
	}
}

void Pet::Rename(string NewName)
{
	m_name = NewName;
	// update petinfo
	UpdatePetInfo(false);

	// update timestamp to force a re-query
	SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, (uint32)UNIXTIME);
}

void Pet::ApplySummonLevelAbilities()
{
	uint32 level = m_uint32Values[UNIT_FIELD_LEVEL];
	double pet_level = (double)level;

	// Determine our stat index.
	int stat_index = -1;
	//float scale = 1;
	bool has_mana = true;

	switch(m_uint32Values[OBJECT_FIELD_ENTRY])
	{
	case 416: //Imp
		stat_index = 0;
		m_aiInterface->disable_melee = true;
		break;
	case 1860: //VoidWalker
		stat_index = 1;
		break;
	case 1863: //Succubus
		stat_index = 2;
		break;
	case 417: //Felhunter
		stat_index = 3;
		break;
	case 11859: // Doomguard
	case 89:	// Infernal
	case 17252: // Felguard
		stat_index = 4;
		break;
	case 510:	// Mage's water elemental
		stat_index = 5;
		m_aiInterface->disable_melee = true;
		break;
	}
	if(m_uint32Values[OBJECT_FIELD_ENTRY] == 89)
		has_mana = false;

	if(stat_index < 0)
	{
		OUT_DEBUG("PETSTAT: No stat index found for entry %u, `%s`!", GetEntry(), creature_info->Name);
		return;
	}

	static double R_base_str[6] = {18.1884058, -15, -15, -15, -15, -15};
	static double R_mod_str[6] = {1.811594203, 2.4, 2.4, 2.4, 2.4, 2.4};
	static double R_base_agi[6] = {19.72463768, -1.25, -1.25, -1.25, -1.25, -1.25};
	static double R_mod_agi[6] = {0.275362319, 1.575, 1.575, 1.575, 1.575, 1.575};
	static double R_base_sta[6] = {17.23188406, -17.75, -17.75, -17.75, -17.75, 0};
	static double R_mod_sta[6] = {2.768115942, 4.525, 4.525, 4.525, 4.525, 4.044};
	static double R_base_int[6] = {19.44927536, 12.75, 12.75, 12.75, 12.75, 20};
	static double R_mod_int[6] = {4.550724638, 1.875, 1.875, 1.875, 1.875, 2.8276};
	static double R_base_spr[6] = {19.52173913, -2.25, -2.25, -2.25, -2.25, 20.5};
	static double R_mod_spr[6] = {3.47826087, 1.775, 1.775, 1.775, 1.775, 3.5};
	static double R_base_pwr[6] = {7.202898551, -101, -101, -101, -101, -101};
	static double R_mod_pwr[6] = {2.797101449, 6.5, 6.5, 6.5, 6.5, 6.5};
	static double R_base_armor[6] = {-11.69565217, -702, -929.4, -1841.25, -1157.55, -1000};
	static double R_mod_armor[6] = {31.69565217, 139.6, 74.62, 89.175, 101.1316667, 100};
	static double R_pet_sta_to_hp[6] = {6.405982906, 15.91304348, 7.956521739, 10.79813665, 11.55590062, 10.0};
	static double R_base_min_dmg[6] = {0.550724638, 4.566666667, 26.82, 29.15, 20.17888889, 20};
	static double R_mod_min_dmg[6] = {1.449275362, 1.433333333, 2.18, 1.85, 1.821111111, 1};
	static double R_base_max_dmg[6] = {1.028985507, 7.133333333, 36.16, 39.6, 27.63111111, 20};
	static double R_mod_max_dmg[6] = {1.971014493, 1.866666667, 2.84, 2.4, 2.368888889, 1.1};

	double base_str = R_base_str[stat_index];
	double mod_str = R_mod_str[stat_index];
	double base_agi = R_base_agi[stat_index];
	double mod_agi = R_mod_agi[stat_index];
	double base_sta = R_base_sta[stat_index];
	double mod_sta = R_mod_sta[stat_index];
	double base_int = R_base_int[stat_index];
	double mod_int = R_mod_int[stat_index];
	double base_spr = R_base_spr[stat_index];
	double mod_spr = R_mod_spr[stat_index];
	double base_pwr = R_base_pwr[stat_index];
	double mod_pwr = R_mod_pwr[stat_index];
	double base_armor = R_base_armor[stat_index];
	double mod_armor = R_mod_armor[stat_index];
	double base_min_dmg = R_base_min_dmg[stat_index];
	double mod_min_dmg = R_mod_min_dmg[stat_index];
	double base_max_dmg = R_base_max_dmg[stat_index];
	double mod_max_dmg = R_mod_max_dmg[stat_index];
	double pet_sta_to_hp = R_pet_sta_to_hp[stat_index];

	// Calculate bonuses
	double pet_sta_bonus = 0.3 * (double)m_Owner->BaseStats[STAT_STAMINA];	  // + sta_buffs
	double pet_int_bonus = 0.3 * (double)m_Owner->BaseStats[STAT_INTELLECT];	// + int_buffs
	double pet_arm_bonus = 0.35 * (double)m_Owner->BaseResistance[RESISTANCE_ARMOR];// + arm_buffs

	double pet_str = base_str + pet_level * mod_str;
	double pet_agi = base_agi + pet_level * mod_agi;
	double pet_sta = base_sta + pet_level * mod_sta + pet_sta_bonus;
	double pet_int = base_int + pet_level * mod_int + pet_int_bonus;
	double pet_spr = base_spr + pet_level * mod_spr;
	double pet_pwr = base_pwr + pet_level * mod_pwr;
	double pet_arm = base_armor + pet_level * mod_armor + pet_arm_bonus;

	// Calculate values
	BaseStats[STAT_STRENGTH] = FL2UINT(pet_str);
	BaseStats[STAT_AGILITY] = FL2UINT(pet_agi);
	BaseStats[STAT_STAMINA] = FL2UINT(pet_sta);
	BaseStats[STAT_INTELLECT] = FL2UINT(pet_int);
	BaseStats[STAT_SPIRIT] = FL2UINT(pet_spr);

	double pet_min_dmg = base_min_dmg + pet_level * mod_min_dmg;
	double pet_max_dmg = base_max_dmg + pet_level * mod_max_dmg;
	BaseDamage[0] = float(pet_min_dmg);
	BaseDamage[1] = float(pet_max_dmg);

	for(uint32 x = 0; x < 5; ++x)
		CalcStat(x);

	// Apply armor and attack power.
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, FL2UINT(pet_pwr));
	BaseResistance[RESISTANCE_ARMOR] = FL2UINT(pet_arm);
	CalcResistance(RESISTANCE_ARMOR);
	CalcDamage();

	// Calculate health / mana
	double health = pet_sta * pet_sta_to_hp;
	double mana = has_mana ? (pet_int * 15) : 0.0;
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, FL2UINT(health));
	SetUInt32Value(UNIT_FIELD_BASE_MANA, FL2UINT(mana));
}

void Pet::ApplyPetLevelAbilities()
{
	uint32 level = m_uint32Values[UNIT_FIELD_LEVEL];
	double dlevel = (double)level;

	// All pets have identical 5% bonus. http://www.wowwiki.com/Hunter_pet
	double pet_mod_sta = 1.05, pet_mod_arm = 1.05, pet_mod_dps = 1.05;

	// As of patch 3.0 the pet gains 45% of the hunters stamina
	double pet_sta_bonus = 0.45 * (double)m_Owner->GetUInt32Value(UNIT_FIELD_STAT2);
	double pet_arm_bonus = 0.35 * (double)m_Owner->BaseResistance[RESISTANCE_ARMOR];	   // Armor
	double pet_ap_bonus = 0.22 * (double)m_Owner->GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER);

	//Base attributes from http://petopia.brashendeavors.net/html/art...ttributes.shtml
	
	static uint32 R_pet_base_armor[80] = { 
		15, 16, 41, 76, 120, 174, 239, 316, 406, 512,
		538, 573, 608, 642, 677, 713, 748, 782, 817, 852,
		888, 922, 957, 992, 1026, 1061, 1097, 1130, 1165, 1200,
		1234, 1270, 1304, 1340, 1373, 1480, 1593, 1709, 1834, 1964,
		2101, 2246, 2397, 2557,	2725, 2780, 2835, 2888, 2944, 2999,
		3052, 3108, 3163, 3216, 3271, 3327, 3380, 3435, 3489, 3791,
		4091, 4391, 4691, 4991, 5291, 5591, 5892, 6192, 6492, 6792,
		7092, 7392, 7692, 7992,	8292, 8592, 8892, 9192, 9492, 9792 }; 

		static double R_pet_base_hp[80] = { 
			42, 55, 71, 86, 102, 120, 137, 156, 176, 198,
			222, 247, 273, 300, 328, 356, 386, 417, 449, 484,
			521, 562, 605, 651, 699, 750, 800, 853, 905, 955,
			1006, 1057, 1110, 1163, 1220, 1277, 1336, 1395, 1459, 1524,
			1585, 1651, 1716, 1782, 1848, 1919, 1990, 2062, 2138, 2215,
			2292, 2371, 2453, 2533, 2614, 2699, 2784, 2871, 2961, 3052,
			3144, 3237, 3331, 3425, 3524, 3624, 3728, 3834, 3941, 4049,
			5221, 5416, 5875, 6139, 6426, 6735, 7069, 8425, 8804, 9202}; // this row is guessed :o

	// Calculate HP
	//patch from darken
	double pet_hp;
	double pet_armor;
	if(level-1<80)
	{
		pet_hp= ( ( ( R_pet_base_hp[level-1]) + ( pet_sta_bonus * 10 ) ) * pet_mod_sta);
		pet_armor= ( (R_pet_base_armor[level-1] ) * pet_mod_arm + pet_arm_bonus );
	}
	else
	{
		pet_hp	= ( ( ( 0.6 * dlevel * dlevel + 10.6 * dlevel + 33 ) + ( pet_sta_bonus * 10 ) ) * pet_mod_sta);
		pet_armor = ( ( -75 + 50 * dlevel ) * pet_mod_arm + pet_arm_bonus );
	}
	double pet_attack_power = ( ( 7.9 * ( ( dlevel * dlevel ) / ( dlevel * 3 ) ) ) + ( pet_ap_bonus ) ) * pet_mod_dps;

	if(pet_attack_power <= 0.0f) pet_attack_power = 1;
	if(pet_armor <= 0.0f) pet_armor = 1;

	// Set base values.
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, FL2UINT(pet_hp));
	BaseResistance[RESISTANCE_ARMOR] = FL2UINT(pet_armor);
	// source www.wow-petopia.com
	// Pets now get a base resistance to all elements equal to their level.
	// For example, a level 11 pet will have a base of 11 resistance to Arcane,
	// Fire, Frost, Nature and Shadow damage.
	BaseResistance[RESISTANCE_ARCANE] = getLevel();
	BaseResistance[RESISTANCE_FIRE] = getLevel();
	BaseResistance[RESISTANCE_FROST] = getLevel();
	BaseResistance[RESISTANCE_NATURE] = getLevel();
	BaseResistance[RESISTANCE_SHADOW] = getLevel();
	for(uint8 x = 0; x < 7; x++)
	{
		CalcResistance(x);
	}

	// Calculate damage.
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, FL2UINT(pet_attack_power));
	CalcDamage();

	// These are just for visuals, no other actual purpose.
	BaseStats[STAT_STRENGTH] = uint32(20+getLevel()*1.55);
	BaseStats[STAT_AGILITY] = uint32(20+getLevel()*0.64);
	// Reverse the health value to calculate stamina
	BaseStats[STAT_STAMINA] = FL2UINT(pet_hp / 10);
	BaseStats[STAT_INTELLECT] = uint32(20+getLevel()*0.18);
	BaseStats[STAT_SPIRIT] = uint32(20+getLevel()*0.36);

	for(uint32 x = 0; x < 5; ++x)
		CalcStat(x);

	LearnLevelupSpells();
}

void Pet::ApplyStatsForLevel()
{
	if(m_uint32Values[UNIT_CREATED_BY_SPELL])	   // Summon
		ApplySummonLevelAbilities();
	else
		ApplyPetLevelAbilities();

	// Apply common stuff
	float pet_level = float(m_uint32Values[UNIT_FIELD_LEVEL]);

	// Apply scale for this family.
	float level_diff = float(myFamily->maxlevel - myFamily->minlevel);
	float scale_diff = float(myFamily->maxsize - myFamily->minsize);
	float factor = scale_diff / level_diff;
	float scale = factor * pet_level + myFamily->minsize;
	SetFloatValue(OBJECT_FIELD_SCALE_X, scale);

	// Apply health fields.
	SetUInt32Value(UNIT_FIELD_HEALTH, m_uint32Values[UNIT_FIELD_BASE_HEALTH]);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, m_uint32Values[UNIT_FIELD_BASE_HEALTH]);
	SetUInt32Value(UNIT_FIELD_POWER1, m_uint32Values[UNIT_FIELD_BASE_MANA]);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, m_uint32Values[UNIT_FIELD_BASE_MANA]);
}

HappinessState Pet::GetHappinessState() 
{
	//gets happiness state from happiness points
	uint32 pts = GetUInt32Value( UNIT_FIELD_POWER5 );
	if( pts < PET_HAPPINESS_UPDATE_VALUE )
		return UNHAPPY;
	else if( pts >= PET_HAPPINESS_UPDATE_VALUE << 1 )
		return HAPPY;
	else
		return CONTENT;
}

void Pet::AddPetSpellToOwner(uint32 spellId)
{
	//exit if owner hasn't Beast training ability (id 5149)
	if(!m_Owner || !m_Owner->HasSpell(5149))
		return;
	//find appropriate teaching spell...
	uint32 TeachingSpellID = 0;
	TeachingSpellID = sWorld.GetTeachingSpell(spellId);
    if(TeachingSpellID)
	{
		if(m_Owner->HasSpell(TeachingSpellID))
			return;
		else
		{
			//...and add it to pet owner to be able teach other pets
			m_Owner->addSpell(TeachingSpellID);
			return;
		}
	
	}
	else
		OUT_DEBUG("WORLD: Could not find teaching spell for spell %u", spellId);
}
uint32 Pet::GetHighestRankSpell(uint32 spellId)
{	
	//get the highest rank of spell from known spells
	SpellEntry *sp = dbcSpell.LookupEntry(spellId);
	SpellEntry *tmp = 0;
	if(sp && mSpells.size() > 0)
	{
		PetSpellMap::iterator itr = mSpells.begin();
		for(; itr != mSpells.end(); ++itr)
			if(sp->NameHash == itr->first->NameHash)
				if((!tmp || tmp->RankNumber < itr->first->RankNumber))
					 tmp = itr->first;
	}
	return tmp ? tmp->Id : 0;
}

AI_Spell * Pet::HandleAutoCastEvent()
{
	AI_Spell * sp = NULL;

	if(m_autoCastSpells[AUTOCAST_EVENT_ATTACK].size() > 1)
	{
		// more than one autocast spell. pick a random one.
		uint32 c = RandomUInt((uint32)m_autoCastSpells[AUTOCAST_EVENT_ATTACK].size());
		uint32 j = 0;
		list<AI_Spell*>::iterator itr = m_autoCastSpells[AUTOCAST_EVENT_ATTACK].begin();

		for(; itr != m_autoCastSpells[AUTOCAST_EVENT_ATTACK].end(), j < c; ++j, ++itr);
		if(itr == m_autoCastSpells[AUTOCAST_EVENT_ATTACK].end())
		{
			AI_Spell * tsp = *m_autoCastSpells[AUTOCAST_EVENT_ATTACK].begin();
			if( tsp->autocast_type == AUTOCAST_EVENT_ATTACK )
				sp = tsp;
			else
			{
				// bad pointers somehow end up here :S
				m_autoCastSpells[AUTOCAST_EVENT_ATTACK].erase(m_autoCastSpells[AUTOCAST_EVENT_ATTACK].begin());
				return HandleAutoCastEvent(); //try again
			}
		}
		else
		{
			AI_Spell * tsp = (*itr);
			if( tsp->autocast_type == AUTOCAST_EVENT_ATTACK )
				sp = tsp;
			else
			{
				m_autoCastSpells[AUTOCAST_EVENT_ATTACK].erase(itr);
				return HandleAutoCastEvent();//try again
			}
		}
	}
	else if(m_autoCastSpells[AUTOCAST_EVENT_ATTACK].size())
	{
		AI_Spell * tsp = *m_autoCastSpells[AUTOCAST_EVENT_ATTACK].begin();
		if( tsp->autocast_type == AUTOCAST_EVENT_ATTACK )
				sp = tsp;
		else
		{
			// bad pointers somehow end up here :S
			m_autoCastSpells[AUTOCAST_EVENT_ATTACK].erase(m_autoCastSpells[AUTOCAST_EVENT_ATTACK].begin());
			return NULL; // we have no spell anymore, give up.
		}
	}
	if (sp != NULL && sp->cooldown && getMSTime() >= sp->cooldowntime )
		return sp;
	else
		return NULL;
}

void Pet::HandleAutoCastEvent(uint32 Type)
{
	list<AI_Spell*>::iterator itr, it2;
	AI_Spell*sp;
	if(!m_Owner)
		return;

	if(Type == AUTOCAST_EVENT_ATTACK)
	{
		if(m_autoCastSpells[AUTOCAST_EVENT_ATTACK].size() > 1)
		{
			// more than one autocast spell. pick a random one.
			uint32 i;
			uint32 ms = getMSTime();
			for(i=0;i<m_autoCastSpells[AUTOCAST_EVENT_ATTACK].size();++i)
			{
				uint32 c = RandomUInt((uint32)m_autoCastSpells[AUTOCAST_EVENT_ATTACK].size());
				uint32 j = 0;
				itr = m_autoCastSpells[AUTOCAST_EVENT_ATTACK].begin();

				for(; itr != m_autoCastSpells[AUTOCAST_EVENT_ATTACK].end(), j < c; ++j, ++itr);
				if(itr == m_autoCastSpells[AUTOCAST_EVENT_ATTACK].end())
				{
					m_aiInterface->SetNextSpell(*m_autoCastSpells[AUTOCAST_EVENT_ATTACK].begin());
					break;
				}
				else
				{
					if((*itr)->cooldowntime && (*itr)->cooldowntime > ms)
						continue;

					m_aiInterface->SetNextSpell(*itr);
				}
			}
		}
		else if(m_autoCastSpells[AUTOCAST_EVENT_ATTACK].size())
		{
			sp =*m_autoCastSpells[AUTOCAST_EVENT_ATTACK].begin();
			if(sp->cooldown && getMSTime() < sp->cooldowntime)
				return;

			m_aiInterface->SetNextSpell(*m_autoCastSpells[AUTOCAST_EVENT_ATTACK].begin());
		}
		return;
	}

	for(  itr = m_autoCastSpells[Type].begin(); itr != m_autoCastSpells[Type].end(); )
	{
		it2 = itr++;
		sp = *it2;

		if( sp->spelltargetType == TTYPE_OWNER )
			CastSpell( m_Owner, sp->spell, false );
		else
		{
			//modified by Zack: Spell targetting will be generated in the castspell function now.You cannot force to target self all the time
			CastSpell( TO_UNIT(NULL), sp->spell, false);
		}
	}
}

void Pet::SetAutoCast(AI_Spell*sp, bool on)
{
	if(sp->autocast_type > 0)
	{
		if(!on)
		{
			for(list<AI_Spell*>::iterator itr = m_autoCastSpells[sp->autocast_type].begin();
				itr != m_autoCastSpells[sp->autocast_type].end(); ++itr)
			{
				if( (*itr) == sp )
				{
					m_autoCastSpells[sp->autocast_type].erase(itr);
					break;
				}
			}
		}
		else
		{
			for(list<AI_Spell*>::iterator itr = m_autoCastSpells[sp->autocast_type].begin();
				itr != m_autoCastSpells[sp->autocast_type].end(); ++itr)
			{
				if((*itr) == sp)
					return;
			}

			m_autoCastSpells[sp->autocast_type].push_back(sp);
		}
	}
}
uint32 Pet::GetUntrainCost()
{
	// TODO: implement penalty
	// costs are: 10s, 50s, 1g, 2g, ..(+1g).. 10g cap
	return 1000;
}

bool Pet::ResetTalents(bool costs)
{
	if (!m_Owner || !m_Owner->IsPlayer())
		return false;

	uint32 level = getLevel();
	uint32 talentPointsForLevel = GetPetTalentPointsAtLevel();

	if (GetSpentPetTalentPoints() == 0)
	{
		SetUnspentPetTalentPoints(talentPointsForLevel);
		m_Owner->smsg_TalentsInfo(true);
		return false;
	}

	uint32 money = 0;

	if(costs)
	{
		money = GetUntrainCost();

		if (m_Owner->GetUInt32Value( PLAYER_FIELD_COINAGE ) < money)
		{
			WorldPacket data(SMSG_BUY_FAILED, 12);
			data << uint64( m_Owner->GetGUID() );
			data << uint32( 0 );
			data << uint8( 2 );		//not enough money
			return false;
		}
	}

	// check if we have any talents in our map
	// if not we don't need to do anything here
	if(m_talents.size())
	{
		PetTalentMap::iterator itr = m_talents.begin();

		for ( ; itr != m_talents.end(); itr++)
		{
			// find our talent
			TalentEntry *talentEntry = dbcTalent.LookupEntry( itr->first );
			if( talentEntry == NULL )
				continue;

			uint32 spellid = talentEntry->RankID[itr->second];

			SpellEntry * sp = dbcSpell.LookupEntry( spellid );
			if( sp )
			{
				if(mSpells.find( sp ) != mSpells.end())
					RemoveSpell( spellid );

				for(uint32 k = 0; k < 3; ++k)
				{
					if(sp->Effect[k] == SPELL_EFFECT_LEARN_SPELL)
					{
						SpellEntry * sp2 = dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[k]);
						if(!sp2) continue;
						RemoveSpell(sp2);
					}
				}
			}
		}
		m_talents.clear();
	}


	SetUnspentPetTalentPoints(talentPointsForLevel);
	m_Owner->smsg_TalentsInfo(true);

	if(costs)
		m_Owner->ModUnsigned32Value( PLAYER_FIELD_COINAGE , -(int32)money);

	SendSpellsToOwner();
	return true;
}

void Pet::InitTalentsForLevel(bool creating)
{
	if(!m_Owner)
		return;

	uint32 level = getLevel();
	uint8 talentPointsForLevel = GetPetTalentPointsAtLevel();

	if(creating)
	{
		SetUnspentPetTalentPoints(talentPointsForLevel);
		return;
	}

	uint8 spentPoints = GetSpentPetTalentPoints();
	// check  for cheats, 
	if(talentPointsForLevel == 0 || spentPoints > talentPointsForLevel)
	{
		// Remove all talent points
		ResetTalents(false);
		return;
	}
	uint8 newPoints = talentPointsForLevel - spentPoints;

	SetUnspentPetTalentPoints(newPoints);
}

uint8 Pet::GetPetTalentPointsAtLevel()
{
	uint16 level = getLevel();
	uint8 points = 0;
	if (level > 19)
	{
		points = uint8(level - 16) / 4;
	}
	else
		return 0;

	// take into account any points we have gained
	// from SPELL_AURA_MOD_PET_TALENT_POINTS
	points += m_Owner->m_PetTalentPointModifier;

	// calculations are done return total points
	return points;
}
void Pet::InitializeTalents()
{
	uint32 talentid = 0;
	uint32 rank = 0;
	uint32 spellId = 0;
	for(PetTalentMap::iterator itr = m_talents.begin(); itr != m_talents.end(); ++itr)
	{
		talentid = itr->first;
		rank = itr->second;
		sLog.outString("Pet Talent: Attempting to load talent %u", talentid);

		// find our talent
		TalentEntry * talentEntry = dbcTalent.LookupEntry(talentid);
		if(!talentEntry)
		{
			spellId = 0;
			Log.Warning("Pet Talent", "Tried to load non-existent talent %u", talentid);
			continue;
		}

		// get the spell id for the talent rank we need to add
		spellId = talentEntry->RankID[rank];

		// add the talent spell to our pet
		LearnSpell( spellId );
	}
	if(	m_Owner)
		m_Owner->smsg_TalentsInfo(true);
}
