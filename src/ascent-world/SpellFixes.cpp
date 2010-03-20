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

/// externals for spell system
extern pSpellEffect SpellEffectsHandler[TOTAL_SPELL_EFFECTS];
extern pSpellAura SpellAuraHandler[TOTAL_SPELL_AURAS];

//ooh look how c++ this is :P
uint32 GetTriggerSpellFromDescription(std::string delimiter, std::string desc)
{
	std::string token;

	// find the delimiter.
	size_t i = desc.find(delimiter);
	if (i == string::npos)
		return 0;

	// find the first character of the spell id.
	size_t j = desc.find_first_not_of(delimiter, i);
	if (j == string::npos)
		return 0;

	// find the end of the spell id.
	size_t k = desc.find("s1", j);
	if (k == string::npos)
		return 0;

	// get our token
	token = desc.substr(j, k - j);

	// convert to int
	uint32 id = 0;
	std::istringstream iss(token);
	iss >> id;

	// and return our spell id
	return id;
}

void CreateDummySpell(uint32 id)
{
	std::string name = "Dummy Trigger";
	SpellEntry * sp = new SpellEntry;
	memset(sp, 0, sizeof(SpellEntry));
	sp->Id = id;
	sp->Attributes = 384;
	sp->AttributesEx = 268435456;
	sp->Flags3 = 4;
	sp->CastingTimeIndex=1;
	sp->procChance=75;
	sp->rangeIndex=13;
	sp->EquippedItemClass=uint32(-1);
	sp->Effect[0]=SPELL_EFFECT_DUMMY;
	sp->EffectImplicitTargetA[0]=25;
	sp->NameHash=crc32((const unsigned char*)name.c_str(), (unsigned int)name.length());
	sp->dmg_multiplier[0]=1.0f;
	sp->StanceBarOrder=-1;
	dbcSpell.SetRow(id,sp);
	sWorld.dummyspells.push_back(sp);
}



uint32 GetSpellClass(SpellEntry *sp)
{
	switch(sp->skilline)
	{
		case SKILL_ARMS:
		case SKILL_FURY:
		case SKILL_PROTECTION:
			return WARRIOR;
		case SKILL_HOLY2:
		case SKILL_PROTECTION2:
		case SKILL_RETRIBUTION:
			return PALADIN;
		case SKILL_BEAST_MASTERY:
		case SKILL_SURVIVAL:
		case SKILL_MARKSMANSHIP:
			return HUNTER;
		case SKILL_ASSASSINATION:
		case SKILL_COMBAT:
		case SKILL_SUBTLETY:
			return ROGUE;
		case SKILL_DISCIPLINE:
		case SKILL_HOLY:
		case SKILL_SHADOW:
			return PRIEST;
		case SKILL_ENHANCEMENT:
		case SKILL_RESTORATION:
		case SKILL_ELEMENTAL_COMBAT:
			return SHAMAN;
		case SKILL_FROST:
		case SKILL_FIRE:
		case SKILL_ARCANE:
			return MAGE;
		case SKILL_AFFLICTION:
		case SKILL_DEMONOLOGY:
		case SKILL_DESTRUCTION:
			return WARLOCK;
		case SKILL_RESTORATION2:
		case SKILL_BALANCE:
		case SKILL_FERAL_COMBAT:
			return DRUID;
		case SKILL_DK_FROST:
		case SKILL_UNHOLY:
		case SKILL_BLOOD:
			return DEATHKNIGHT;
	}

	return -1;
}

void _DumpClassSpells(FILE *f, uint32 _class)
{
	SpellEntry *sp;

	fprintf(f, "\t{ ");
	for(uint32 x=0; x < (uint32)dbcSpell.GetNumRows(); x++)
	{
		sp = dbcSpell.LookupRow(x);
		if (sp->talent_tree == 0 && GetSpellClass(sp) == _class)
			fprintf(f, "%d, ", sp->Id);
	}
	fprintf(f, " }, \n");
}

void DumpClassSpells()
{
	FILE *f;

	f = fopen("ClassSpells.h", "w");
	if (f == NULL)
	{
		sLog.outString("Error while opening ClassSpells.h: %s", strerror(errno));
		return;
	}

	fprintf(f, "/* ** Automatically Generated File ** */\n\n");
	fprintf(f, "static uint32 spellarray[DRUID+1][9999] = {\n");
	fprintf(f, "\t{ 0 }, \n");
	_DumpClassSpells(f, WARRIOR);
	_DumpClassSpells(f, PALADIN);
	_DumpClassSpells(f, HUNTER);
	_DumpClassSpells(f, ROGUE);
	_DumpClassSpells(f, PRIEST);
	_DumpClassSpells(f, DEATHKNIGHT);
	_DumpClassSpells(f, SHAMAN);
	_DumpClassSpells(f, MAGE);
	_DumpClassSpells(f, WARLOCK);
	_DumpClassSpells(f, DRUID);
	fprintf(f, "\t};\n");

	fclose(f);
}

void DumpTalentSpells()
{
	FILE *f;

	f = fopen("TalentSpells.csv", "w");
	if (f == NULL)
	{
		sLog.outString("Error while opening TalentSpells.csv: %s", strerror(errno));
		return;
	}

	for(uint32 i = 0 ;i < dbcTalent.GetNumRows(); i++)
	{
		TalentEntry *talent = dbcTalent.LookupRow(i);
		if(!talent)
			continue;
		for(uint32 rank = 0; rank < 5; rank++)
		{
			if(talent->RankID[rank] != 0)
				fprintf(f, "%d\n", talent->RankID[rank]); 
		}
		SpellEntry * sp = dbcSpell.LookupEntryForced(talent->RankID[0]);
		if(!sp)
			continue;
		for(uint32 k = 0; k < 3; ++k)
		{
			if(sp->Effect[k] == SPELL_EFFECT_LEARN_SPELL)
			{
				SpellEntry * sp2 = dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[k]);
				if(!sp2) continue;
				for(uint32 j = 0; j < dbcSpell.GetNumRows(); j++)
				{
					SpellEntry * sp3 = dbcSpell.LookupRow(j);
					if(!sp3 || sp3->NameHash != sp2->NameHash)
						continue;
					fprintf(f, "%d\n", sp3->Id); 
				}
			}
		}
	}

	fclose(f);
}


// fills array 'arr' with integers in arguments and returns its new size. Last argument must be 0!
uint32 fill( uint32* arr, ... ){
	va_list vl;
	uint32 i;
	va_start( vl, arr );
	for( i = 0; i < 100; ++i ){
		arr[i] = va_arg( vl, uint32 );
		if(arr[i] == 0)
			break;
	}
	va_end( vl );
	return i;
}

// Generates SpellNameHashes.h
void GenerateNameHashesFile()
{
	const uint32 fieldSize = 81;
	const char* prefix = "SPELL_HASH_";
	uint32 prefixLen = uint32(strlen(prefix));
	DBCFile dbc;

	if( !dbc.open( "DBC/Spell.dbc" ) )
	{
		Log.Error("World", "Cannot find file ./DBC/Spell.dbc" );
		return;
	}
	uint32 cnt = (uint32)dbc.getRecordCount();
	uint32 namehash = 0;
	FILE * f = fopen("SpellNameHashes.h", "w");
	char spaces[fieldSize], namearray[fieldSize];
	strcpy(namearray, prefix);
	char* name = &namearray[prefixLen];
	for(int i=0;i<fieldSize-1;i++)
		spaces[i] = ' ';
	spaces[fieldSize-1] = 0;
	uint32 nameTextLen = 0, nameLen = 0;
	for(uint32 x=0; x < cnt; x++)
	{
		const char* nametext = dbc.getRecord(x).getString(139);
		nameTextLen = (unsigned int)strlen(nametext);
		strncpy(name, nametext, fieldSize-prefixLen-2);	// Cut it to fit in field size
		name[fieldSize-prefixLen-2] = 0; // in case nametext is too long and strncpy didn't copy the null
		nameLen = (unsigned int)strlen(name);
		for(uint32 i = 0;i<nameLen;i++)
		{
			if(name[i] >= 'a' && name[i] <= 'z')
				name[i] = toupper(name[i]);
			else if(!(name[i] >= '0' && name[i] <= '9') &&
				!(name[i] >= 'A' && name[i] <= 'Z'))
				name[i] = '_';
		}
		namehash = crc32((const unsigned char*)nametext, nameTextLen);
		int32 numSpaces = fieldSize-prefixLen-nameLen-1;
		if(numSpaces < 0)
			fprintf(f, "WTF");
		spaces[numSpaces] = 0;
		fprintf(f, "#define %s%s0x%08X\n", namearray, spaces, namehash);
		spaces[numSpaces] = ' ';
	}
	fclose(f);
}

// Copies effect number 'fromEffect' in 'fromSpell' to effect number 'toEffect' in 'toSpell'
void CopyEffect(SpellEntry *fromSpell, uint8 fromEffect, SpellEntry *toSpell, uint8 toEffect)
{
	if(!fromSpell || !toSpell || fromEffect > 2 || toEffect > 2)
		return;

	uint32 *from = fromSpell->Effect;
	uint32 *to = toSpell->Effect;
	// Copy 20 values starting at Effect
	for(uint8 index = 0;index < 20;index++)
	{
		to[index * 3 + toEffect] = from[index * 3 + fromEffect];
	}
}

void ApplySingleSpellFixes(SpellEntry *sp)
{
	if(sp != NULL)
	{
		switch(sp->Id)
		{
			// Spell 1455 Proc Chance (Life Tap Rank 2:
			case  1455:
				sp->procChance = 100;
				break;

			// Spell 1456 Proc Chance (Life Tap Rank 3:
			case  1456:
				sp->procChance = 100;
				break;
		
			// Spell 3391 Proc Chance (Thrash :
			case  3391:
				sp->procChance = 10;
				break;
			
			// Spell 8182 Proc Chance (Frost Resistance Rank 1:
			case  8182:
				sp->procChance = 100;
				break;

				// Spell 10476 Proc Chance (Frost Resistance Rank 2:
			case  10476:
				sp->procChance = 100;
				break;

				// Spell 10477 Proc Chance (Frost Resistance Rank 3:
			case  10477:
				sp->procChance = 100;
				break;

				// Spell 25559 Proc Chance (Frost Resistance Rank 4:
			case  25559:
				sp->procChance = 100;
				break;

				// Spell 58742 Proc Chance (Frost Resistance Rank 5:
			case  58742:
				sp->procChance = 100;
				break;

				// Spell 58744 Proc Chance (Frost Resistance Rank 6:
			case  58744:
				sp->procChance = 100;
				break;
		
			// Spell 8185 Proc Chance (Fire Resistance Rank 1:
			case  8185:
				sp->procChance = 100;
				break;

				// Spell 10534 Proc Chance (Fire Resistance Rank 2:
			case  10534:
				sp->procChance = 100;
				break;

				// Spell 10535 Proc Chance (Fire Resistance Rank 3:
			case  10535:
				sp->procChance = 100;
				break;

				// Spell 25562 Proc Chance (Fire Resistance Rank 4:
			case 25562:
				sp->procChance = 100;
				break;

				// Spell 58738 Proc Chance (Fire Resistance Rank 5:
			case 58738:
				sp->procChance = 100;
				break;

				// Spell 58740 Proc Chance (Fire Resistance Rank 6:
			case 58740:
				sp->procChance = 100;
				break;
		
			// Spell 11687 Proc Chance (Life Tap Rank 4:
			case  11687:
				sp->procChance = 100;
				break;
		
			// Spell 11688 Proc Chance (Life Tap Rank 5:
			case  11688:
				sp->procChance = 100;
				break;
		
			// Spell 11689 Proc Chance (Life Tap Rank 6:
			case  11689:
				sp->procChance = 100;
				break;
		
			// Spell 12284 Proc Chance (Mace Specialization Rank 1:
			case  12284:
				sp->procChance = 1;
				break;
		
			// Spell 12292 Proc Chance (Death Wish :
			case  12292:
				sp->procChance = 100;
				break;
		
			// Spell 12322 Proc Chance (Unbridled Wrath Rank 1:
			case  12322:
				sp->procChance = 8;
				break;
		
			// Spell 12701 Proc Chance (Mace Specialization Rank 2:
			case  12701:
				sp->procChance = 2;
				break;
		
			// Spell 12702 Proc Chance (Mace Specialization Rank 3:
			case  12702:
				sp->procChance = 3;
				break;
		
			// Spell 12703 Proc Chance (Mace Specialization Rank 4:
			case  12703:
				sp->procChance = 4;
				break;
		
			// Spell 12704 Proc Chance (Mace Specialization Rank 5:
			case  12704:
				sp->procChance = 6;
				break;
		
			// Spell 12999 Proc Chance (Unbridled Wrath Rank 2:
			case  12999:
				sp->procChance = 16;
				break;
		
			// Spell 13000 Proc Chance (Unbridled Wrath Rank 3:
			case  13000:
				sp->procChance = 24;
				break;
		
			// Spell 13001 Proc Chance (Unbridled Wrath Rank 4:
			case  13001:
				sp->procChance = 32;
				break;
		
			// Spell 13002 Proc Chance (Unbridled Wrath Rank 5:
			case  13002:
				sp->procChance = 40;
				break;
		
			// Spell 14076 Proc Chance (Dirty Tricks Rank 1:
			case  14076:
				sp->procChance = 30;
				break;
		
			// Spell 14094 Proc Chance (Dirty Tricks Rank 2:
			case  14094:
				sp->procChance = 60;
				break;
		
			// Spell 15494 Proc Chance (Fury of Forgewright :
			case  15494:
				sp->procChance = 10;
				break;
		
			// Spell 15600 Proc Chance (Hand of Justice :
			case  15600:
				sp->procChance = 2;
				break;
		
			// Spell 15601 Proc Chance (Hand of Justice :
			case  15601:
				sp->procChance = 10;
				break;
		
			// Spell 15642 Proc Chance (Ironfoe :
			case  15642:
				sp->procChance = 10;
				break;
				
			// Spell 16843 Proc Chance (Crimson Fury :
			case  16843:
				sp->procChance = 10;
				break;
		
			// Spell 18797 Proc Chance (Flurry Axe :
			case  18797:
				sp->procChance = 10;
				break;
		
			// Spell 18803 Proc Chance (Focus :
			case  18803:
				sp->procChance = 100;
				break;
		
			// Spell 19105 Proc Chance (MHTest01 Effect :
			case  19105:
				sp->procChance = 10;
				break;
		
			// Spell 19109 Proc Chance (MHTest02 Effect :
			case  19109:
				sp->procChance = 10;
				break;
		
			// Improved Wing Clip
			case  19228:
				{
				sp->procChance = 7;
				sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
				}break;
		
			// Improved Wing Clip
			case  19232:
				{
				sp->procChance = 14;
				sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
				}break;
		
			// Improved Wing Clip 
			case  19233:
				{
				sp->procChance = 20;
				sp->procFlags = PROC_ON_CAST_SPECIFIC_SPELL;
				}break;
		
			// Spell 21919 Proc Chance (Thrash :
			case  21919:
				sp->procChance = 10;
				break;
		
			// Spell 23158 Proc Chance (Concussive Shot Cooldown Reduction :
			case  23158:
				sp->procChance = 4;
				break;
				
			// Spell 27035 Proc Chance (Sword Specialization (OLD: :
			case  27035:
				sp->procChance = 10;
				break;
		
			// Spell 27521 Proc Chance (Mana Restore :
			case  27521:
				sp->procChance = 2;
				break;
		
			// Spell 27867 Proc Chance (Freeze :
			case  27867:
				sp->procChance = 2;
				break;

			//Vehicle: Scarlet cannon
			
			// Elemental Focus
			case  16164:
				{
					sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;
			case  16246:
				{
					sp->procFlags	=	PROC_ON_CAST_SPELL;
					sp->procCharges++; //	first	charge gets	lost when	it gets	procced
				}break;
			
			//shaman - Stormstrike
			case  17364:
				{
					sp->procFlags=PROC_ON_SPELL_HIT_VICTIM;
				}break;

			//shaman - Improved Stormstrike
			case 51521:
			case 51522:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
			
			//shaman - Hex
			case  51514:
				{
					// Damage	caused may interrupt the effect.
					sp->AuraInterruptFlags |=	AURA_INTERRUPT_ON_UNUSED2;
				}break;

			//shaman - Frozen Power
			case 63374:
			case 63373:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->EffectSpellClassMask[1][0] = 0x80000000;	// frost shock
					sp->EffectSpellClassMask[1][1] = 0;
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 63685;
				}break;

			//shaman - Ancestral Awakening
			case 51556:
			case 51557:
			case 51558:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
					sp->EffectTriggerSpell[0] = 52752;
				}break;
			case 52752:
				{
					sp->spell_can_crit = false;
					sp->logsId = sp->Id;
				}break;
			case 974:
			case 32593:
			case 32594:
			case 49283:
			case 49284: //Earth Shield
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_REMOVEONUSE;
					sp->EffectTriggerSpell[0] = sp->Id;
				}break;

			// shaman	-	Maelstrom	Weapon
			case 51528:
				{
					sp->procChance = 0;
					sp->ProcsPerMinute = 1.3f;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
				}break;
			case 51529:
				{
					sp->procChance = 0;
					sp->ProcsPerMinute = 2.6f;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
				}break;
			case 51530:
				{
					sp->procChance = 0;
					sp->ProcsPerMinute = 3.9f;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
				}break;
			case 51531:
				{
					sp->procChance = 0;
					sp->ProcsPerMinute = 5.2f;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
				}break;
			case 51532:
				{
					sp->procChance = 0;
					sp->ProcsPerMinute = 6.5f;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
				}break;

			// Maelstorm proc	charge removal
			case  53817:	
				{
					sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
			
			// Astral	Shift
			case  52179:
				{
					sp->EffectAmplitude[0] = 0;
					sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
					sp->EffectMiscValue[0] = 127;
				}break;
			
			// Shamanistic Rage
			case 30823:
				{
					sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
				}break;
			
			case 51479:
			case 51478:
			case 51474: // Astral Shift
				{
					sp->procChance = 100;
					sp->EffectImplicitTargetA[0] = 1;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags = PROC_ON_SPELL_LAND_VICTIM;
					sp->EffectTriggerSpell[0] = 52179;
					sp->Effect[1] = 0;
				}break;
			
			case 51975:
				{
					// shaman	-	Poison Cleansing Totem
					SpellEntry*  sp2 = dbcSpell.LookupEntryForced( 58780 );
					if( sp2 != NULL)
					{
						sp->EffectRadiusIndex[0] = sp2->EffectRadiusIndex[0];	//30 yards
						sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY_IN_AREA;
					}
				}break;
			
			case 52025:
				{
					// shaman	-	Disease	Cleansing	Totem
					SpellEntry*  sp2 = dbcSpell.LookupEntryForced( 58780 );
					if( sp2 != NULL)
					{
						sp->EffectRadiusIndex[0] = sp2->EffectRadiusIndex[0];	//30 yards
						sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY_IN_AREA;
					}
				}break;
			
			// Fire	Nova Totem visual	fix
			case 8349:
			case 8502:
			case 8503:
			case 11306:
			case 11307:
			case 25535:
			case 25537:
			case 61650:
			case 61654:
				{
					sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1]	=	19823;
				}break;
			
			// Flurry
			case 12319:
			case 12971:
			case 12972:
			case 12973:
			case 12974:
			case 16256:
			case 16281:
			case 16282:
			case 16283:
			case 16284:
				{
					sp->procFlags	=	PROC_ON_CRIT_ATTACK;
				}break;
			
			// Flurry	proc
			case 12966:
			case 12967:
			case 12968:
			case 12969:
			case 12970:
			case 16257:
			case 16277:
			case 16278:
			case 16279:
			case 16280:
				{
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->procCharges++; //	first	charge gets	lost when	it gets	procced
				}break;
			//Minor Glyph Research + Northrend Inscription Research
			case 61177:
			case 61288:
			case 60893:
				{
					sp->Effect[1]	=	0;
					sp->EffectBaseDice[1]	=	0;
					sp->EffectBasePoints[1]	=	0;
					sp->EffectImplicitTargetA[1]	=	0;
					sp->EffectDieSides[1]	=	0;
				}break;
			// Shaman	Totem	items	fixes
			// Totem of	Survival,	Totem	of the Tundra
			case 46097:
			case 43860:
			case 43861:
			case 43862:
			case 60564:
			case 60571:
			case 60572:
			case 37575:
				{
					sp->EffectSpellClassMask[0][0] = 0x00100000	|	0x10000000 | 0x80000000; //Earth + Flame + Frost Shocks
					sp->EffectSpellClassMask[0][1] = 0x08000000;	// Wind	Shock
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
				}break;
			case 60567:
				{
					sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
				}break;
			
			// Totem of	Third	Wind
			case 46098:
			case 34138:
			case 42370:
			case 43728:
				{
					sp->procFlags	=	PROC_ON_CAST_SPELL;
					sp->EffectSpellClassMask[0][0] = 0x00000080;	// Lesser	Healing	Wave
				}break;
			
			// Totem of	the	Elemental	Plane
			case  60770:
				{
					sp->procFlags	=	PROC_ON_CAST_SPELL;
					sp->EffectSpellClassMask[0][0] = 0x00000001;	// Lightning Bolt
				}break;
			
			// Fathom-Brooch of	the	Tidewalker
			case  37247:	
				{
					sp->procFlags	=	PROC_ON_CAST_SPELL;
					sp->proc_interval	=	45000;
				}break;
			//Warlock: Backlash
			case  34935:
			case  34938:
			case  34939:
				{
					sp->proc_interval	=	8000;
					sp->procFlags	|= PROC_ON_MELEE_ATTACK_VICTIM | PROC_TARGET_SELF;
				}break;
			
			case  34936:
				{
						sp->AuraInterruptFlags |=	AURA_INTERRUPT_ON_CAST_SPELL;
				}break;
						
				//Priest - Holy	Nova
			case 15237:
				{
					sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1]	=	23455;
				}break;
			case 15430:
				{
					sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1]	=	23458;
				}break;
			case 15431:
				{
					sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1]	=	23459;
				}break;
			case 27799:
				{
					sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1]	=	27803;
				}break;
			case 27800:
				{
					sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1]	=	27804;
				}break;
			case 27801:
				{
					sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1]	=	27805;
				}break;
			case 25331:
					{
						sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[1]	=	25329;
					}break;
			case 48077:
					{
						sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[1]	=	48075;
					}break;
			case 48078:
				{
					sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1]	=	48076;
				}break;
			
			// Moroes' garrote targets a single	enemy	instead	of us
			case 37066:
				{
					sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
				}break;
			
			// Penance
			case 47540:
			case 53005:
			case 53006:
			case 53007:
				{
						sp->DurationIndex = 566; // Change to instant cast as script will cast the real channeled spell.
						sp->ChannelInterruptFlags = 0; // Remove channeling behaviour.
				}break;

			// Penance triggered healing spells have wrong targets.
			case 47750:
			case 52983:
			case 52984:
			case 52985:
				{
						sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_FRIEND;
				}break;

			// Hymn of Hope
			case 60931:
			case 60933:
				{
						sp->EffectImplicitTargetA[0] = 37;
				}break;

			//Paladin - Seal of Righteousness
			case 21084:
				{
					
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 20187;
					sp->procFlags = PROC_ON_MELEE_ATTACK;
					
				}break;

			//Paladin - Holy Shock rank 1-rank 7 Spell DMG fix
			case 20473:
				{
					sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
					sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 25912;
					sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[2] = 25914;
					sp->EffectImplicitTargetA[2] = EFF_TARGET_SINGLE_FRIEND;
				}break;

			case 20929:
				{
					sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
					sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 25911;
					sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[2] = 25913;
					sp->EffectImplicitTargetA[2] = EFF_TARGET_SINGLE_FRIEND;
				}break;

			case 20930:
				{
					sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
					sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 25902;
					sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[2] = 25903;
					sp->EffectImplicitTargetA[2] = EFF_TARGET_SINGLE_FRIEND;
				}break;

			case 27174:
				{
					sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
					sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 27176;
					sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[2] = 27175;
					sp->EffectImplicitTargetA[2] = EFF_TARGET_SINGLE_FRIEND;
				}break;

			case 33072:
				{
					sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
					sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 33073;
					sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[2] = 33074;
					sp->EffectImplicitTargetA[2] = EFF_TARGET_SINGLE_FRIEND;
				}break;

			case 48824:
				{
					sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
					sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 48822;
					sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[2] = 48820;
					sp->EffectImplicitTargetA[2] =  EFF_TARGET_SINGLE_FRIEND;
				}break;

			case 48825:
				{
					sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
					sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 48823;
					sp->Effect[2] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[2] = 48821;
					sp->EffectImplicitTargetA[2] = EFF_TARGET_SINGLE_FRIEND;
				}break;

			//paladin	-	Reckoning
			case  20177:
			case  20179:
			case  20180:
			case  20181:
			case  20182:
				{
						sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM	|	PROC_TARGET_SELF;
				}break;

		
			//paladin	-	Reckoning	Effect = proc	on proc
			case  20178:
				{
						sp->procChance = 100;
						sp->procFlags	=	PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
				}break;

		
			//paladin	-	Judgement	of Wisdom
			case  20186:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 20268;
						sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
						sp->procChance = 35;
						sp->Effect[1] = 0;
						sp->EffectApplyAuraName[1] = 0;
				}break;

		
			//paladin	-	Judgement	of Light
			case  20185:
				{
						sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 20267;
						sp->procChance = 35;
						sp->Effect[1] = 0;
						sp->EffectApplyAuraName[1] = 0;
				}break;

			case  20267:
			case  20268:
			case  20341:
			case  20342:
			case  20343:
			case  27163:
				{
						sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
				}break;
			//paladin	-	Eye	for	an Eye
			case  9799:
			case  25988:
				{
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_CRIT_HIT_VICTIM;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	25997;
				}break;
						
			//paladin	-	Blessed	Life
			case  31828:
			case  31829:
			case  31830:
				{
						sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;
						sp->EffectTriggerSpell[0]	=	31828;
				}break;

			//paladin	-	Light's	Grace
			case  31833:
			case  31835:
			case  31836:
				{
						sp->EffectSpellClassMask[0][0]=0x80000000;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;

			//Paladin - Sacred Cleansing
			case  53551:
			case  53552:
			case  53553:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->EffectSpellClassMask[0][0] = 0x00001000;
						sp->EffectSpellClassMask[0][1] = 0x00001000;
						sp->EffectSpellClassMask[0][2] = 0x00001000;
				}break;
			// Touched by the Light 
			case 53590:
			case 53591:
			case 53592:
				{
					sp->EffectMiscValue[0] = 2; // stamina
				}break;
			//Shield of the Templar
			case 53709:
			case 53710:
			case 53711:
				{
					sp->EffectSpellClassMask[0][0] = 64;
					sp->EffectSpellClassMask[0][1] = 16384;
					sp->EffectSpellClassMask[0][2] = 1048576;
					sp->EffectSpellClassMask[1][0] = 64;
					sp->EffectSpellClassMask[1][1] = 16384;
					sp->EffectSpellClassMask[1][2] = 1048576;
					sp->EffectSpellClassMask[2][0] = 64;
					sp->EffectSpellClassMask[2][1] = 16384;
					sp->EffectSpellClassMask[2][2] = 1048576;
				}break;
			//Paladin - Infusion of light
			case  53569:
			case  53576:
				{
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;
			case 53672:
			case 54149:
				{
					sp->AuraInterruptFlags	=	AURA_INTERRUPT_ON_CAST_SPELL;
				}break;
			//Paladin - Vindication
			case 9452:
			case 26016:
				{
						sp->procFlags	=	PROC_ON_MELEE_ATTACK | PROC_ON_ANY_HOSTILE_ACTION;
						sp->procChance	=	25;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA; // hoax
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
						sp->maxstack = 1;
				}break;
			//Paladin - Art of War
			case 53486:
				{
						sp->procFlags	=	PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT;
						sp->EffectApplyAuraName[2]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[2]	=	53489;
				}break;
			case 53488:
				{
						sp->procFlags	=	PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT;
						sp->EffectApplyAuraName[2]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[2]	=	59578;
				}break;
			case 53489:
			case 59578:
				{
						 sp->AuraInterruptFlags	|= AURA_INTERRUPT_ON_CAST_SPELL;
				}break;

			//shaman - Lightning Overload	
			case  30675:	// rank	1
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	39805;//proc something (we will	owerride this)
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 4;
				}break;

			case  30678:	 //	rank 2
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	39805;//proc something (we will	owerride this)
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 8;
				}break;

			case  30679:	// rank	3
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	39805;//proc something (we will	owerride this)
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 12;
				}break;

			case  30680:	// rank	4
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	39805;//proc something (we will	owerride this)
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 16;
				}break;

			case  30681:	// rank	5
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	39805;//proc something (we will	owerride this)
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 20;
				}break;

			//shaman - Purge 
			case  370:
			case  8012:
			case  27626:
			case  33625:	
				{
						sp->DispelType = DISPEL_MAGIC;
				}break;

		
			//Shaman - Eye of	the	Storm
			case  29062:
			case  29064:
			case  29065:
				{
						sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
				}break;
		
			//shaman - Elemental Devastation
			case  29179:
			case  29180:
			case  30160:
			//shaman - Ancestral healing
			case  16176:
			case  16235:
			case  16240:	
				{
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;

			//shaman - Ancestral healing proc	spell
			case  16177:
			case  16236:
			case  16237:
				{
						sp->rangeIndex = 4;
				}break;

			//rogue - overkill stealth break
			case 58427:
				{
						sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;
				}break;
		
			//rogue	-	Find Weakness.
			case  31233:
			case  31239:
			case  31240:
			case  31241:
			case  31242:	
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;

			//rogue	-	Cheap	Shot
			case  1833: 
				{
						sp->Flags3 &=	~FLAGS3_REQ_BEHIND_TARGET;	//Doesn't	require	to be	behind target
				}break;

			//rogue	-	Camouflage.
			case  13975:
			case  14062:
			case  14063:
			case  14064:
			case  14065:	
				{
						sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
				}break;
			//rogue	-	Mace Specialization.
			case  13709:
			case  13800:
			case  13801:	
			case  13802:
			case  13803:	
				{
						sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		
				}break;

			//rogue	- Shadowstep
			case 36563:	
				{
						sp->EffectMiscValue[1] = SMT_MISC_EFFECT;
						sp->procFlags	=	0;
						sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
				}break;
		
			case 44373:
				{
						sp->procFlags	=	0;
						sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
				}break;

			//Rogue  - Ruthlessness
			case 14156:
			case 14160:
			case 14161:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;//proc	spell
						sp->EffectTriggerSpell[0] = 14157;	
						sp->procFlags = PROC_ON_CAST_SPELL | PROC_TARGET_SELF;					
				}break;
		
			//priest	-	Holy Concentration
			case 34753:
			case 34859:
			case 34860:
				 {
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;
			case 34754://Make it similar to Mage Clearcasting
				{
						sp->procCharges	=	2;
						sp->Flags3	=	4;
						sp->Flags4	=	1073741824;
						sp->procFlags	=	87376;
				}break;

			//guardian spirit
			case 47788:
				{
						sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
						sp->AuraInterruptFlags = 0;
				}break;

			case 48153:
				{
						sp->EffectBasePoints[0] = 50;
				}break;

			//Priest:	Blessed	Recovery
			case 27811:
				{
						sp->EffectTriggerSpell[0]	=	27813;
						sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
				}break;

			case 27815:
				{
						sp->EffectTriggerSpell[0]	=	27817;
						sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
				}break;

			case 27816:
			{
						sp->EffectTriggerSpell[0]	=	27818;
						sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
				}break;

			//priest-	Blessed	Resilience
			case  33142:
			case  33145:
			case  33146:
			//priest-	Focused	Will
			case  45234:
			case  45243:
			case  45244:
				{
						sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
				}break;
			//Priest:	Improved Vampiric Embrace (Rank1)
			case 27839:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;

				}break;
			//Paladin: Seal	of Wisdom
			case  20166:
				{
						sp->procChance = 25;
						sp->procFlags = PROC_ON_MELEE_ATTACK;
						sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
				}break;

			//druids enrage
			case 50334:
				{
						sp->AdditionalAura = 58923;
				}break;

			//Druid: Feral Swiftness
			case 17002:
				{
					sp->AdditionalAura = 24867;
				}break;
			case 24866:
				{
					sp->AdditionalAura = 24864;
				}break;

			// Feral Swiftness dodge
			case 24867:
			case 24864:
				{
					sp->apply_on_shapeshift_change = true;
				}break;

			// Druid: Moonkin Form (passive)
			case 24905: 
				{
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;

			//Druid: Natural Perfection
			case  33881:
			case  33882:
			case  33883:
				{
						sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
				}break;

			//Druid: Frenzied	Regeneration
			case  22842:
			case  22895:
			case  22896:
			case  26999:
				{
						sp->Effect[0]	=	6;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	22845;
				}break;

			// Spell 16850 Proc Chance (Celestial Focus Rank 1:
			case  16850:
				{
						sp->procChance = 3;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
		
			case  16923:	
				{
						sp->procChance = 6;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;

			case  16924:	
				{
						sp->procChance = 9;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;

		
			//paladin	-	seal of	blood
			case  31892:
			case  38008:
				{
						sp->procFlags	=	PROC_ON_MELEE_ATTACK;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	31893;
				}break;
			case 53720:
				{
						sp->procFlags = PROC_ON_MELEE_ATTACK;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 53719;
				}break;

			// Seal of Blood/Martyr holy damage
			case 31893:
			case 53719:
				{
					sp->School = SCHOOL_HOLY; 
					sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC; 
				}break;

			//paladin - seal of command
			case 20375:
				{
					sp->procFlags = PROC_ON_MELEE_ATTACK;
					sp->ProcsPerMinute = 7;
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 20424;
				}break;

			//paladin - hammer of the righteous
			case 53595:
				{
						sp->speed = 0;
						sp->is_melee_spell = true;
				}break;

			//paladin	-	Spiritual	Attunement 
			case  31785:
			case  33776:
				{
						sp->procFlags	=	PROC_ON_SPELL_LAND_VICTIM | PROC_TARGET_SELF;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	31786;
				}break;


			//Druid: Leader	of the Pack
			case  24932:
					{
						sp->Effect[1]	=	0;
						sp->Effect[2]	=	0; //removing	strange	effects.
					}break;
		
			//Druid: Improved	Leader of	the	Pack
			case  34299:
					{
						sp->proc_interval	=	6000;//6 secs
					}break;
		
		
			//fix	for	the	right	Enchant	ID for Enchant Cloak - Major Resistance
			case  27962:
			case  36285:
					{
						sp->EffectMiscValue[0] = 2998;
					}break;
		
			//muhaha,	rewriting	Retaliation	spell	as old one :D
			case  20230:
					{
						sp->Effect[0]	=	6; //aura
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	22858; //evil	,	but	this is	good for us	:D
						sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM; //add procflag	here since this	was	not	processed	with the others	!
					}break;
		
		
			//"bloodthirst"	new	version	is ok	but	old	version	is wrong from	now	on :(
			case  23881:
					{
						sp->Effect[1]	=	64;	//cast on	us,	it is	good
						sp->EffectTriggerSpell[1]	=	23885; //evil	,	but	this is	good for us	:D
					}break;
		
			case  23892:
				{
						sp->Effect[1]	=	64;
						sp->EffectTriggerSpell[1]	=	23886; //evil	,	but	this is	good for us	:D
					}break;
		
			case  23893:
				{
						sp->Effect[1]	=	64;	//
						sp->EffectTriggerSpell[1]	=	23887; //evil	,	but	this is	good for us	:D
					}break;
		
			case  23894:
				{
						sp->Effect[1]	=	64;	//
						sp->EffectTriggerSpell[1]	=	23888; //evil	,	but	this is	good for us	:D
					}break;
		
			case  25251:
				{
						sp->Effect[1]	=	64;	//aura
						sp->EffectTriggerSpell[1]	=	25252; //evil	,	but	this is	good for us	:D
					}break;
		
			case  30335:
				{
						sp->Effect[1]	=	64;	//aura
						sp->EffectTriggerSpell[1]	=	30339; //evil	,	but	this is	good for us	:D
					}break;
		
				//hunter - Beast lore
			case 1462:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_EMPATHY;
				}break;
			// Hunter	-	Master Tactician
			case  34506:
			case  34507:
			case  34508:
			case  34838:
			case  34839:
					{
						sp->procFlags	=	PROC_ON_RANGED_ATTACK	|	PROC_TARGET_SELF;
					}break;
		
			// Hunter	-	T.N.T
			case 56333:
			case 56336:
			case 56337:
				{
						sp->procFlags = PROC_ON_TRAP_TRIGGER | PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CRIT_ATTACK | PROC_ON_PHYSICAL_ATTACK | PROC_ON_RANGED_ATTACK;
						sp->EffectSpellClassMask[0][0] = 0x00000004;
						sp->EffectSpellClassMask[0][1] = sp->EffectSpellClassMask[1][1];						
						sp->EffectSpellClassMask[1][0] = 0x0;
				}break;

			// Hunter	-	Spirit Bond
			case  19578:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	19579;
					}break;
		
			case  20895:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	24529;
					}break;
		
			case  19579:
			case  24529:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA; //we	should do	the	same for player	too	as we	did	for	pet
						sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
						sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
						sp->EffectBasePoints[1]	=	sp->EffectBasePoints[0];
						sp->EffectAmplitude[1] = sp->EffectAmplitude[0];
						sp->EffectDieSides[1]	=	sp->EffectDieSides[0];
					}break;		
		
			// Hunter	-	Animal Handler
			case  34453:
			case  34454:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
						sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HIT_CHANCE;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
					}break;
		
		
			// Hunter	-	Catlike	Reflexes
			case  34462:
			case  34464:
			case  34465:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
						sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
						sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
					}break;
		
		
			// Hunter	-	Serpent's	Swiftness
			case  34466:
			case  34467:
			case  34468:
			case  34469:
			case  34470:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
						sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HASTE;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
				}break;
		
			// Hunter	-	Ferocious	Inspiration
			case  34455:
			case  34459:
			case  34460:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
						sp->EffectTriggerSpell[0]	=	34456;
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF;
						sp->Effect[1]	=	0; //remove	this
				}break;
		
			// Hunter	-	Focused	Fire
			case  35029:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	35060;
				}break;
			case  35030:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	35061;
				}break;
			case  35060:
			case  35061:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_WITH_PET;
				}break;
		
			// Hunter	-	Thick	Hide
			case  19609:
			case  19610:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
						sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_RESISTANCE;	//we do	not	support	armor	rating for pets	yet	!
						sp->EffectBasePoints[0]	*= 10; //just	give it	a	little juice :P
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				}break;
		
			// Hunter	-	Ferocity
			case  19612:
			case  19599:
			case  19600:
			case  19601:
			case  19602:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
						sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_CRIT_PERCENT;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				}break;
		
			// Hunter	-	Bestial	Swiftness
			case  19596:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
						sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_SPEED;	
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				}break;
		
			// Hunter	-	Endurance	Training
			case  19583:
			case  19584:
			case  19585:
			case  19586:
			case  19587:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
						sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				}break;
			// Hunter - Thrill of the Hunt
			case 34497:
			case 34498:
			case 34499:
				{
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
						sp->procChance = sp->EffectBasePoints[0]+1;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 34720;
				}break;
		
			// Hunter	-	Expose Weakness
			case  34500:
			case  34502:
			case  34503:
						{
						sp->procFlags	=	PROC_ON_RANGED_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT;
				}break;
			// hunter - Wild Quiver
			case 53215:
			case 53216:
			case 53217:
				{
						sp->procFlags = PROC_ON_RANGED_ATTACK;

				}break;
			//Hunter - Frenzy
			case  19621:
			case  19622:
			case  19623:
			case  19624:
			case  19625:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	19615;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
						sp->procChance = sp->EffectBasePoints[0];
						sp->procFlags	=	PROC_ON_CRIT_ATTACK;
						sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET | PROC_TARGET_SELF;
				}break;
					
			case 56641:
			case 34120:
			case 49051:
			case 49052: //Steady Shot cast time fix
				{
						sp->CastingTimeIndex = 5; // Set 2 sec cast time
				}break;

			case 61846:
			case 61847: // Aspect of the Dragonhawk
				{	// need to copy Mod Dodge Percent aura from a separate spell
					CopyEffect(dbcSpell.LookupEntryForced(61848), 0, sp, 2);
				}break;

			//Hunter - Unleashed Fury
			case 19616:
			case 19617:
			case 19618:
			case 19619:
			case 19620:

				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
						sp->EffectMiscValue[0] = 1;	//tweekign melee dmg
						sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
				}break;
		
			//Hunter : Pathfinding
			case  19559:
			case 19560:
				{
						sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
				}break;
		
			//Hunter : Rapid Killing - might need	to add honor trigger too here. I'm guessing	you	receive	Xp too so	i'm	avoiding double	proc
			case  34948:
			case 34949:
				{
						sp->procFlags	=	PROC_ON_GAIN_EXPIERIENCE | PROC_TARGET_SELF;
				}break;
		
			// Winter's	chill
			case 12579:
				{
						sp->c_is_flags |=	SPELL_FLAG_CANNOT_PROC_ON_SELF;
				}break;	

			//Mage - Icy Veins
			case  12472:
				{
						sp->EffectMiscValue[1] = SMT_PROC_CHANCE;
				}break;

			//Mage - Wand	Specialization.	Not	the	forst	thing	we messed	up.	Blizz	uses attack	as magic and wandds	as weapons :S
			case 6057:
			case 6085:
			//Priest - Wand	Specialization
			case 14524:
			case 14525:
			case 14526:
			case 14527:
			case 14528:

				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
						sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
				}break;
		
			//Mage - Precision
			case 29438:
			case 29439:
			case 29440:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
						sp->EffectMiscValue[0] = SMT_COST;
				}break;
		
			//Mage - Arcane	Concentration
			case  11213:
			case  12574:
			case  12575:
			case  12576:
			case  12577:
				{
					sp->procFlags	=	PROC_ON_CAST_SPELL | PROC_TARGET_SELF;
				}break;
		
			//Mage - ClearCasting	Effect 
			//Druid - Clearcasting Effect
			//Druid - Nature's Grace effect
			case  16870:
			case  12536:
			case  16886:
				{
						sp->procCharges	=	2;
				}break;	
		
			//Mage - Arcane	Blast
			case  36032:
				{
						sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
						sp->procFlags |= PROC_TARGET_SELF;
						sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
				}break;
		
			//rogue - Prey on the weak
			case 51685:
			case 51686:
			case 51687:
			case 51688:
			case 51689:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
				}break;

			//rogue	-	Seal Fate
			case  14186:
				{
						sp->procFlags	=	PROC_ON_CRIT_ATTACK;
						sp->procChance = 20;
				}break;
			case  14190:
				{
						sp->procFlags	=	PROC_ON_CRIT_ATTACK;
						sp->procChance = 40;
				}break;
			case  14193:
				{
						sp->procFlags	=	PROC_ON_CRIT_ATTACK;
						sp->procChance = 60;
				}break;
			case  14194:
				{
						sp->procFlags	=	PROC_ON_CRIT_ATTACK;
						sp->procChance = 80;
				}break;
			case  14195:
				{
						sp->procFlags	=	PROC_ON_CRIT_ATTACK;
						sp->procChance = 100;
				}break;

			// druid - Nature's	Grace
			case  16880:
			case  61345:
			case  61346:
				{
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;
		
			//druid	-	Blood	Frenzy
			case  16954:
				{
						sp->procFlags = PROC_ON_CRIT_ATTACK;
						sp->procChance = 100;
				}break;
			case  16952:
				{
						sp->procFlags = PROC_ON_CRIT_ATTACK;
						sp->procChance = 50;
				}break;
		
			//druid	-	Primal Fury
			case  16961:
				{
						sp->procFlags = PROC_ON_CRIT_ATTACK;
						sp->procChance = 100;
				}break;
			case  16958:
				{
						sp->procFlags = PROC_ON_CRIT_ATTACK;
						sp->procChance = 50;
				}break;
		
			//druid	-	Intensity
			case 17106:
			case 17107:
			case 17108:
				{
						sp->EffectApplyAuraName[1]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
		
			//Improved Sprint
			case  13743:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 50;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				}break;
			case  13875:
				{
						sp->procChance = 100;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
				}break;

			//Warlock - healthstones
			case 6201:
			case 6202:
			case 5699:
			case 11729:
			case 11730:
			case 27230:
			case 47871:
			case 47878:
				{
						sp->Reagent[1] = 0;
						sp->ReagentCount[1] = 0;
				}break;	
			//warlock	-	 Seed	of Corruption
			case  27243:
				{
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[1]	=	27285;
						sp->procFlags	=	PROC_ON_SPELL_HIT_VICTIM | PROC_ON_DIE;
						sp->procChance = 100;
				}break;
			//warlock - soul link
			case  19028:
				{
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_SPLIT_DAMAGE;
						sp->EffectBasePoints[0] = 20;
						sp->EffectMiscValue[0] = 127;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
						sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
						
						sp->Effect[1] = 6;
						sp->EffectApplyAuraName[1] = 79;
						sp->EffectBasePoints[1]	= 4;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
						sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
						sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
				}break;
			
			//warlock: Nightfall
			case  18094:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 17941;
						sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
						sp->procChance = 2;
						sp->EffectSpellClassMask[0][0] = 0x0;
				}break;
			case 18095:
			{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 17941;
						sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
						sp->procChance = 4;
						sp->EffectSpellClassMask[0][0] = 0x0;
				}break;

			//Warlock - Backdraft
			case 47258:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	54274;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 100;
						sp->procCharges	= 4;
						sp->EffectSpellClassMask[0][0]=0x00800000;
						sp->EffectSpellClassMask[0][1]=0x00800000;
				}break;
			case 47269:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	54276;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 100;
						sp->procCharges	= 4;
						sp->EffectSpellClassMask[0][0]=0x00800000;
						sp->EffectSpellClassMask[0][1]=0x00800000;
				}break;
			case 47260:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	54277;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 100;
						sp->procCharges	= 4;
						sp->EffectSpellClassMask[0][0]=0x00800000;
						sp->EffectSpellClassMask[0][1]=0x00800000;
				}break;
			//Warlock - Eradication
			case 47195:
			case 47196:
			case 47197:
				{
						sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
						sp->EffectSpellClassMask[0][0] = 2;
						sp->proc_interval = 30000;
				}break;
					
			//warlock: Improved	Enslave	Demon
			case  18821:
				{
						sp->EffectMiscValue[0]=SMT_SPELL_VALUE_PCT;
						sp->EffectBasePoints[0]	=	-(sp->EffectBasePoints[0]+2);
				}break;	
			//Mage - Improved	Blizzard
			case  11185:
				{		 
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	12484;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
			case  12487:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	12485;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
			case  12488:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	12486;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
		
			//mage - Combustion
			case  11129:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL | PROC_ON_SPELL_CRIT_HIT	|	PROC_TARGET_SELF;
						sp->procCharges	=	0;
						sp->c_is_flags |=	SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
				}break;
		
			//mage - Master	of Elements
			case  29074:
			case  29075:
			case  29076:
				{
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[1] = 29077;
						sp->EffectImplicitTargetA[1] = 1;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
						sp->procChance = 100;
				}break;
		
			//mage:	Blazing	Speed
			case 31641:
			case 31642:
					{
						sp->EffectTriggerSpell[0]	=	31643;
				}break;

			//mage talent	"frostbyte". we	make it	to be	dummy
			case  11071:
			case  12496:
			case  12497:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 12494;
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->procChance = sp->EffectBasePoints[0];
				}break;

			//rogue-shiv ->	add	1	combo	point
			case  5938:
				{
						sp->Effect[1]=80;
						sp->Flags3 &=	~FLAGS3_REQ_BEHIND_TARGET;	//Doesn't	require	to be	behind target
						sp->is_melee_spell = true;
				}break;
		
			//warlock	-	Demonic	Sacrifice
			case  18789:
			case  18790:
			case  18791:
			case  18792:
			case  35701:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_ON_PET;
				}break;

		
			//warlock	-	Demonic	Tactics
			case  30242:
				{
						sp->Effect[0]	=	0; //disble	this.	This is	just blizz crap. Pure	proove that	they suck	:P
						sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
						sp->EffectApplyAuraName[2] = SPELL_AURA_MOD_SPELL_CRIT_CHANCE; //lvl 1 has it	fucked up	:O
						sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
				}break;
			case  30245:
			case  30246:
			case  30247:
			case  30248:
				{
						sp->Effect[0]	=	0; //disble	this.	This is	just blizz crap. Pure	proove that	they suck	:P
						sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
						sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
				}break;
		
			//warlock	-	Demonic	Resilience
			case  30319:
			case  30320:
			case  30321:
				{
						sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
				}break;
		
			//warlock	-	Improved Imp
			case  18694:
			case  18695:
			case  18696:
			case  18705:
			case  18706:
			case  18707:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				}break;
		
			//warlock	-	Improved Succubus
			case  18754:
			case  18755:
			case  18756:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
				}break;
		
			//warlock	-	Fel	Intellect
			case  18731:
			case  18743:
			case  18744:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
						sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
						sp->EffectMiscValue[0] = 3;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				}break;
		
			//warlock	-	Fel	Stamina
			case  18748:
			case  18749:
			case  18750:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
						sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
						sp->EffectMiscValue[0] = 2;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				}break;
		
			//warlock	-	Unholy Power
			case  18769:
			case  18770:
			case  18771:
			case  18772:
			case  18773:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
						sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				//this is	required since blizz uses	spells for melee attacks while we	use	fixed	functions
						sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
						sp->EffectMiscValue[1] = SCHOOL_NORMAL;
						sp->EffectBasePoints[1]	=	sp->EffectBasePoints[0]	;
				}break;
		
			//warlock	-	Master Demonologist	-	25 spells	here
			case  23785:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	|	SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	23784;
				}break;
			case  23822:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	|	SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	23830;
				}break;
			case  23823:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	|	SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	23831;
				}break;
			case  23824:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	|	SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	23832;
				}break;
			case  23825:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	|	SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	35708;
				}break;
			//and	the	rest
			case  23784:
			case  23830:
			case  23831:
			case  23832:
			case  35708:
				{
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				}break;

			case  23759:
			case  23760:
			case  23761:
			case  23762:
			case  23826:
			case  23827:
			case  23828:
			case  23829:
			case  23833:
			case  23834:
			case  23835:
			case  23836:
			case  23837:
			case  23838:
			case  23839:
			case  23840:
			case  23841:
			case  23842:
			case  23843:
			case  23844:
				{
					sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_WITH_PET;
					sp->Effect[0]	=	SPELL_EFFECT_APPLY_AURA;
				}break;

			case  35702:
			case  35703:
			case  35704:
			case  35705:
			case  35706:
				{
						sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
				}break;
			//warlock	-	Improved Healthstone
			case  18692:
			case  18693:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
						sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
				}break;
			//warlock	-	Improved Drain Soul
			case  18213:
			case  18372:
				{
						sp->procFlags	=	PROC_ON_TARGET_DIE | PROC_TARGET_SELF;
						sp->procChance = 100;
						sp->Effect[0]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	18371;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
						sp->Effect[2]	=	0	;	//remove this	effect
				}break;
		
			//warlock	-	Shadow Embrace
			case  32385:
			case  32387:
			case  32392:
			case  32393:
			case  32394:
				{
						sp->procChance = 100;
						sp->procFlags = PROC_ON_SPELL_LAND;
						sp->Effect[1] = 0;	//remove this	effect ? Maybe remove	the	other	one	:P xD
				}break;
			//warlock - soul leech
			case 30293:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 30294;
					sp->procChance = 10;
					sp->procFlags = PROC_ON_SPELL_LAND;
				}break;
			case 30295:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 30294;
					sp->procChance = 20;
					sp->procFlags = PROC_ON_SPELL_LAND;
				}break;
			case 30296:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 30294;
					sp->procChance = 30;
					sp->procFlags = PROC_ON_SPELL_LAND;
				}break;
			//warlock	-	Pyroclasm
			case 18073:
			case 18096:
			case 63245:
				{
						sp->Effect[0]	=	0; //delete	this owerride	effect :P
						sp->EffectTriggerSpell[1]	=	18093; //trigger spell was wrong :P
						sp->procFlags	=	PROC_ON_ANY_HOSTILE_ACTION;
						sp->procChance = 26; //god,	save us	from fixed values	!
				}break;
		
			//Mage - Improved Scorch
			case 11095:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 33;
						sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
			case 12872:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;	
						sp->procChance =66;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
			case 12873:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance =100;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
		
			//Nature's Grasp
			case 16689:
				{
						sp->Effect[0]	=	6; 
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	339; 
						sp->Effect[1]	=	0; 
						sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM	|	PROC_REMOVEONUSE;
						sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
						sp->procChance = 100;
				}break;
			case 16810:
				{
						sp->Effect[0]	=	6; 
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	1062;	
						sp->Effect[1]	=	0; 
						sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM	|	PROC_REMOVEONUSE;
						sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
						sp->procChance = 100;
				}break;
			case 16811:
				{
						sp->Effect[0]	=	6; 
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	5195;	
						sp->Effect[1]	=	0; 
						sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM	|	PROC_REMOVEONUSE;
						sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
						sp->procChance = 100;
				}break;
			case 16812:
				{
						sp->Effect[0]	=	6; 
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	5196;	
						sp->Effect[1]	=	0; 
						sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM	|	PROC_REMOVEONUSE;
						sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
						sp->procChance = 100;
				}break;
			case 16813:
				{
						sp->Effect[0]	=	6; 
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	9852;	
						sp->Effect[1]	=	0; 
						sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM	|	PROC_REMOVEONUSE;
						sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
						sp->procChance = 100;
				}break;
			case 17329:
				{
						sp->Effect[0]	=	6; 
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	9853;	
						sp->Effect[1]	=	0; 
						sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM	|	PROC_REMOVEONUSE;
						sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
						sp->procChance = 100;
				}break;
			case 27009:
				{
						sp->Effect[0]	=	6; 
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	26989; 
						sp->Effect[1]	=	0; 
						sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM	|	PROC_REMOVEONUSE;
						sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
						sp->procChance = 100;
				}break;
		
			//wrath	of air totem targets sorounding	creatures	instead	of us
			case 2895:
				{
						sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
						sp->EffectImplicitTargetA[2] = 0;
						sp->EffectImplicitTargetB[0] = 0;
						sp->EffectImplicitTargetB[1] = 0;
						sp->EffectImplicitTargetB[2] = 0;
				}break;
			//Druid: Natural Shapeshifter
			case 16833:
			case 16834:
			case 16835:
				{
						sp->DurationIndex	=	0;
				}break;

		
			//Priest - Inspiration proc	spell
			case 14893:
			case 15357:
			case 15359:
				{
						sp->rangeIndex = 4;
				}break;
		
			//Relentless Strikes
			case 14179:
			case 58422:
			case 58423:
			case 58424:
			case 58425:
				{
						sp->EffectApplyAuraName[0]	=	SPELL_AURA_PROC_TRIGGER_SPELL;//proc	spell
						sp->EffectTriggerSpell[0]	=	14181;	
						sp->procFlags	=	PROC_ON_CAST_SPELL | PROC_TARGET_SELF;
						sp->procChance = 100;
				}break;
		
			//priest - surge of	light
			case 33150:
			case 33154:
				{
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;

			case 33151:
				{
						sp->procCharges	=	2;
				}break;
		
			//Seal of	Justice	-lowered proc	chance (experimental values	!)
			case 20164:
			case 31895:
				{
						sp->procChance = 20;
						sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
				}break;

			// Seal of Wisdom - dummy effect
			case 20165:
				{
						sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
				}break;		
			// judgement of command shit
			case 20425:
				{
						sp->Effect[1] = 2;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_SINGLE_ENEMY;
				}break;
			//Berserking
			case 26635:
				{
					sp->Attributes	=	327680;
					sp->CasterAuraState	=	0;
				}break;
			//Escape Artist
			case 20589:
				{
					sp->Attributes = 65552;
					sp->Flags3 = 0;
					sp->Effect[0] = 108;
					sp->Effect[1] = 108;
					sp->EffectDieSides[0] = 1;
					sp->EffectDieSides[1] = 1;
					sp->EffectBaseDice[0] = 1;
					sp->EffectBaseDice[1] = 1;
					sp->EffectBasePoints[0] = 9;
					sp->EffectBasePoints[1] = 9;
					sp->EffectMiscValue[0] = 7;
					sp->EffectMiscValue[1] = 11;
				}break;
			//Zapthrottle Mote Extractor
			case 30427:
				{
					sp->Effect[0] = SPELL_EFFECT_DUMMY;
				}break;
			//Goblin Jumper Cable
			case 22999:
			case 8342:
			case 54732:
				{
					sp->Effect[0] = 113;
					sp->EffectMiscValue[0] = 120;
				}break;
			//rogue	-	intiative
			case  13976:
			case  13979:
			case  13980:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procFlags	=	uint32(PROC_ON_CAST_SPELL|PROC_TARGET_SELF);
				}break;
		
			//this an	on equip item	spell(2824)	:	 ice arrow(29501)
			case  29501:
				{
						sp->procChance = 30;//some say it	is triggered every now and then
						sp->procFlags	=	PROC_ON_RANGED_ATTACK;
				}break;
		
			//warrior	-	second wind	should trigger on	self
			case  29841:
			case  29842:
				{
						sp->procFlags	|= PROC_TARGET_SELF;
				}break;

			// Improved Revenge
			case 12797:
			case 12799:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
				}break;

			//warrior	-	berserker	rage is	missing	1	effect = regenerate	rage
			case  18499:
				{
						sp->Effect[2]	=	6;
						sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[2]	=	37521; //not sure	if this	is the one.	In my	time this	adds 30	rage
						sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM	|	PROC_TARGET_SELF;
				}break;
		
			//warrior	-	improved berserker rage
			case  20500:
			case  20501:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL | PROC_TARGET_SELF;
				}break;
		
			// warrior - Spell Reflection
			case  23920:
				{
						sp->procFlags	=	PROC_NULL; //	No need	to proc
				}break;

		
			// warrior - Titan's Grip
			case  46917:	// main	spell
				{
						SpellEntry * sp2 = dbcSpell.LookupEntryForced(	49152	);	// move	this aura	into main	spell
						if(sp != NULL)
						{
							sp->Effect[1] = sp2->Effect[0];
							sp->EffectApplyAuraName[1] = sp2->EffectApplyAuraName[0];
							sp->EffectMiscValue[1] = sp2->EffectMiscValue[0];
							sp->EffectSpellClassMask[1][0] = sp2->EffectSpellClassMask[0][0];
							sp->EffectSpellClassMask[1][1] = sp2->EffectSpellClassMask[0][1];
							sp->EffectSpellClassMask[1][2] = sp2->EffectSpellClassMask[0][2];
							sp->EffectBasePoints[1] = sp2->EffectBasePoints[0];
							sp->EffectDieSides[1]= sp2->EffectDieSides[0];
							sp->EffectBaseDice[1]= sp2->EffectBaseDice[0] ;
						}
				}break;
		
			// Charge	-	Changing from	dummy	for	the	power	regen
			case 100:
			case 6178:
			case 11578:
				{
					sp->Effect[1]	=	SPELL_EFFECT_ENERGIZE;
					sp->EffectMiscValue[1] = 1;
				}break;
		
			//warrior	-	Rampage
			case  30030:
			case 30033:
				{
						sp->procFlags	=	PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
						sp->EffectTriggerSpell[0]	=	sp->EffectTriggerSpell[1];
				}break;

			// warrior - overpower 
			case 7384:
			case 7887:
			case 11584:
			case 11585:
				{
						sp->Attributes |=	ATTRIBUTES_CANT_BE_DPB;
				}break;
		
			// warrior - heroic	fury
			case 60970:
				{
						sp->Effect[1]	=	SPELL_EFFECT_DUMMY;
				}break;
		
			case  20608:	//Reincarnation
					{
						for(uint32 i=0;i<8;i++)
						{
								if(sp->Reagent[i])
								{
									sp->Reagent[i] = 0;
									sp->ReagentCount[i]	=	0;
								}
						}
					}break;


			//Warrior - Juggernaut
			case 64976:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 65156;
					sp->procFlags = PROC_ON_CAST_SPELL | PROC_TARGET_SELF;;
					sp->procChance = 100;
				}break;
			// druid - travel	form
			case 5419:
					{
						sp->Attributes |=	ATTRIBUTES_MOUNT_CASTABLE;
					}break;

		
			// druid - Naturalist
			case  17069:
			case  17070:
			case  17071:
			case  17072:
			case  17073:
				{
						sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
						sp->EffectMiscValue[1] = 1;	
				}break;
		
			// Druid: Omen of Clarity
			case  16864:
				{
						sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_CAST_SPELL;
						sp->procChance = 6;
						sp->proc_interval = 10000;
				}break;
			case 52610:
				{
						sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_ATTACK_POWER_PCT;
				}break;
		
			//Serpent-Coil Braid
			case 37447:
			case 61062://Heroes' Frostfire Garb AND Valorous Frostfire Garb
				{
						sp->Effect[0] = 6;
						sp->Effect[1] = 6;
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 100;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->EffectTriggerSpell[1]	=	37445;
						sp->maxstack = 1;
				}break;

			// Mark of Conquest
			case 33510:
				{
						sp->procChance = 7;
						sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;


		
			//Paladin	-	Improved Lay on	Hands
			case  20234:
			case  20235:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
		
			// Sudden	Death
			case 29724:
			case 29725:
			case 29723:
				{
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
				}break;
		
			// Taste for Blood
			case 56638:
			case 56637:
			case 56636:
				{
					sp->procFlags	=	PROC_ON_ANY_HOSTILE_ACTION;
					sp->EffectSpellClassMask[0][0]	= 0x0;
					sp->EffectSpellClassMask[1][0]	= 0x0;
				}break;
				
			// Flametongue weapon
			case 58792:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->EffectTriggerSpell[0]	=	58788;
				}break;
			case 58791:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->EffectTriggerSpell[0]	=	58787;
				}break;
			case 58784:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->EffectTriggerSpell[0]	=	58786;
				}break;
			case 16313:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->EffectTriggerSpell[0]	=	25488;
				}break;
			case 16312:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->EffectTriggerSpell[0]	=	16344;
				}break;
			case 16311:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->EffectTriggerSpell[0]	=	16343;
				}break;
			case 15569:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->EffectTriggerSpell[0]	=	10445;
				}break;
			case 15568:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->EffectTriggerSpell[0]	=	8029;
				}break;
			case 15567:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->EffectTriggerSpell[0]	=	8028;
				}break;
			case 10400:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->EffectTriggerSpell[0]	=	8026;
				}break;

			//windfury weapon
			case  33757:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procFlags	=	PROC_ON_MELEE_ATTACK;
						sp->EffectTriggerSpell[0]	=	25504;
						sp->procChance = 20;
						sp->proc_interval	=	3000;
						sp->maxstack = 1;
						// Current proc system doesn't support common proc_interval for 2
						// procs making dual wielding WF overpowered. So it is disabled for now
						//sp->always_apply = true; //	so that	we can apply 2 wf	auras	while	dual-wielding
				}break;
		
			//for	test only
			//case  32796:
			//	{
			//{
				//printf("!!!!!!hash %u	\n",sp->NameHash);
				//sp->procChance=100;
				//SpellDuration	*sd=sSpellDuration.LookupEntryForced(sp->DurationIndex);
				//printf("iterruptflag %u, duration	%u",sp->AuraInterruptFlags,GetDuration(sd));
			//}
		
			//improoved	berserker	stance should	be triggered on	berserker	stance use
			//sp = sSpellStore.LookupEntryForced(12704);
			//	{	sp->procFlags	=	PROC_ON_CAST_SPECIFIC_SPELL;
		
			//sp = sSpellStore.LookupEntryForced(16280);
			//	{	printf("!!Interrupt	flags	%u interval	%u charges %u\n",sp->AuraInterruptFlags,sp->proc_interval,sp->procCharges);
			//sp = sSpellStore.LookupEntryForced(16284);
			//	{	printf("Interrupt	flags	%u\n",sp->AuraInterruptFlags);
		
			// dot heals
			case  38394:
				{
						sp->procFlags	=	1024;
					//sp->SpellGroupType = 6;
				}break;
				
			case  16972:
			case  16974:
			case  16975:
				{
					//fix	for	Predatory	Strikes
					uint32 mm	=	(1<<(FORM_BEAR-1))|(1<<(FORM_DIREBEAR-1))|(1<<(FORM_MOONKIN-1))|(1<<(FORM_CAT-1));
					sp->RequiredShapeShift = mm;
				}break;
		
			case  20134:
				{
						sp->procChance = 50;
				}break;
		
			/* aspect	of the pack	-	change to	AA */
			case  13159:
				{
						sp->Effect[1]	=	SPELL_EFFECT_APPLY_AREA_AURA;
						sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;
				}break;

			/* aspect	of the cheetah - add proc	flags	*/
			case  5118:
					{
						sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;;
				}break;

			
			case SPELL_RANGED_GENERAL:
			case SPELL_RANGED_THROW:
			case 26679:
			case 29436:
			case 37074:
			case 41182:
			case 41346:
				{
					if(	sp->RecoveryTime==0	&& sp->StartRecoveryTime ==	0	)
						sp->RecoveryTime = 1600;
				}break;

			case  SPELL_RANGED_WAND:
				{
						sp->Spell_Dmg_Type = SPELL_DMG_TYPE_RANGED;
						if(	sp->RecoveryTime==0	&& sp->StartRecoveryTime ==	0	)
							sp->RecoveryTime = 1600;
				}break;

			/**********************************************************
			*	Misc stuff (NPC SPELLS)
			**********************************************************/

			case 5106:
				{
					sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_UNUSED2;
				}break;
		
			/**********************************************************
			*	Misc stuff (QUEST)
			**********************************************************/
			// Bat Net
			case 52151:
				{
						sp->EffectImplicitTargetA[0] = 6;
				}break;

			// queststuff
			case  30877:
				{
						sp->EffectImplicitTargetB[0]=0;
				}break;

			// quest stuff
			case 23179:
				{
						sp->EffectMiscValue[0] = 1434;
				}break;

			// DK RUNEFORGING
			case 53341:
			case 53343:
				{
					sp->Effect[1] = SPELL_EFFECT_QUEST_COMPLETE;
					sp->EffectMiscValue[1] = 12842;
				}

				// arcane charges
			case 45072:
				{
					sp->Attributes |= ATTRIBUTES_MOUNT_CASTABLE;
				}break;

			case 48252: //Prototype Neural Needle
			case 45634: //Neural Needle
				{
					sp->Effect[1] = 0;
				}break;
				//Tuber whistle
			case  36652:
				{
					sp->Effect[1] = 3;
				}break;
				//Cure Caribou Hide
			case 46387:
				{
					sp->ReagentCount[0] = 1;
					sp->ReagentCount[1] = 0;
					sp->Reagent[1] = 0;
				}break;
				//Create Totem of Issliruk
			case 46816:
				{
					sp->Reagent[0] = 0;
					sp->ReagentCount[0] = 0;
				}break;

			/**********************************************************
			*	Misc stuff (ITEMS)
			**********************************************************/

			// Nitro Boosts
			case 55004:
				{
						CopyEffect(dbcSpell.LookupEntryForced(54861), 0, sp, 2);
						sp->DurationIndex = 39;
				}break;

			// Gnomish Lightning Generator
			case 55039:
				{
						sp->InterruptFlags = 0;
						sp->AuraInterruptFlags = 0;
						sp->ChannelInterruptFlags = 0;
				}break;

			// Libram of Divinity
			case 28853:
			// Libram of Light
			case 28851:
			// Blessed Book of Nagrand
			case 32403:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_FLAT_MODIFIER;
						sp->EffectMiscValue[0] = 0;
						sp->EffectSpellClassMask[0][0] = 1073741824;
				}break;
			// Libram of Mending
			case 43741:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->procChance = 100;
					sp->EffectSpellClassMask[0][0] = 0x80000000;
					sp->EffectSpellClassMask[0][1] = 0x0;
					sp->EffectSpellClassMask[0][2] = 0x0;
					sp->EffectSpellClassMask[1][0] = 0x0;
					sp->EffectSpellClassMask[1][1] = 0x0;
					sp->EffectSpellClassMask[1][2] = 0x0;
					sp->EffectSpellClassMask[2][0] = 0x0;
					sp->EffectSpellClassMask[2][1] = 0x0;
					sp->EffectSpellClassMask[2][2] = 0x0;
				}break;

			// Druid Trinket Living	Root of	the	Wildheart
			case 37336:
				{
						sp->Effect[0] = SPELL_EFFECT_DUMMY;// oh noez, we	make it	DUMMY, no	other	way	to fix it	atm
				}break;

			// Palla trinket
			case 43745:
			case 34258:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->EffectSpellClassMask[0][0] = 8388608 | 520;
						sp->procChance = 100;
				}break;

			// Lesser Rune of Warding / Grater Rune of Warding
			case 42135:
			case 42136:
				{
						sp->EffectImplicitTargetA[1] = 6;
						sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
				}break;

			//Idol of terror
			case 43737:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
			// Lesser	Heroism	(Tenacious Earthstorm	Diamond)
			case  32844:
				{
						sp->procChance = 5;
						sp->procFlags	 = PROC_ON_MELEE_ATTACK;
				}break;
		
			// Darkmoon	Card:	Heroism
			case  23689:
				{
						sp->ProcsPerMinute = 2.5f;
						sp->procFlags	 = PROC_ON_MELEE_ATTACK;
				}break;

			// Darkmoon	Card:	Maelstrom
			case  23686:
				{
						sp->ProcsPerMinute = 2.5f;
						sp->procFlags	 = PROC_ON_MELEE_ATTACK;
				}break;

			// dragonspine trophy
			case 34774:
				{
						sp->ProcsPerMinute = 1.5f;
						sp->proc_interval	=	20000;
						sp->procFlags	=	PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;
		
			// Romulo's	Poison Vial
			case 34586:
				{
						sp->ProcsPerMinute = 1.6f;
						sp->procFlags	=	PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;
		
			// madness of	the	betrayer
			case 40475:
				{
						sp->procChance = 50;
						sp->ProcsPerMinute = 1.0f;
						sp->procFlags	=	PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;
			
			// Band	of the Eternal Defender
			case 35077:
				{
						sp->proc_interval	=	55000;
						sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;
				}break;
			// Band	of the Eternal Champion
			case 35080:
				{
						sp->proc_interval	=	55000;
						sp->procFlags	=	PROC_ON_MELEE_ATTACK;
				}break;
			// Band	of the Eternal Restorer
			case 35086:
			// Band	of the Eternal Sage
			case 35083:
				{
						sp->proc_interval	=	55000;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
			// Sonic Booster
			case 54707:
				{
						sp->ProcsPerMinute = 1.0f;
						sp->proc_interval	=	60000;
						sp->procFlags = PROC_ON_MELEE_ATTACK;
				}break;	
			case 60301: // Meteorite Whetstone
			case 60317: // Signet of Edward the Odd
				{
						sp->proc_interval	=	45000;
						sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;
			// Bandit's Insignia
			case 60442:
				{
						sp->proc_interval	=	45000;
						sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;
			// Fury of the Five Flights
			case 60313:
				{
						sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;
			// Grim Toll
			case 60436:
				{
						sp->proc_interval	=	45000;
						sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;
			// Illustration of the Dragon Soul
			case 60485:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
			// Soul of the Dead
			case 60537:
				{
						sp->proc_interval	=	45000;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;
			// Anvil of Titans
			case 62115:
				{
						sp->proc_interval	=	45000;
						sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;
			// Embrace of the Spider
			case 60490:
			// Flow of Knowledge
			case 62114:
			// Forge Ember
			case 60473:
			// Mystical Skyfire Diamond
			case 32837:
			// Sundial of the Exiled
			case 60063:
				{
						sp->proc_interval	=	45000;
						sp->procFlags = PROC_ON_CAST_SPELL;
				}break;

			// Majestic Dragon Figurine
			case 60524:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
			// Mirror of Truth
			case 33648:
				{
						sp->proc_interval	=	45000;
						sp->procFlags = PROC_ON_CRIT_ATTACK;
				}break;

			// Vestige of Haldor
			case 60306:
				{
						sp->proc_interval	=	45000;
						sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;

			//Energized	
			case  43750:	
					{
						sp->procFlags = PROC_ON_CAST_SPELL;
					}break;

		
			//Spell	Haste	Trinket
			case  33297:	
					{
						sp->procFlags	=	PROC_ON_CAST_SPELL | PROC_TARGET_SELF;
					}break;
			case 57345: // Darkmoon Card: Greatness
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->proc_interval = 45000;
					sp->EffectTriggerSpell[0] = 60229;
				}break;

			// Swordguard Embroidery
			case 55776:
				{
						sp->proc_interval	=	60000;
						sp->procFlags = PROC_ON_MELEE_ATTACK;
				}break;
			// Chuchu's Tiny Box of Horrors
			case 61618:
				{
						sp->proc_interval	=	45000;
						sp->procFlags = PROC_ON_MELEE_ATTACK;
				}break;		

			case 57351: // Darkmoon Card: Berserker!
				{
					sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK | PROC_ON_ANY_DAMAGE_VICTIM;;	// when you strike, or are struck in combat
				}break;
			case 60196:
				{
					sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
				}break;
			// Darkmoon Card: Death
			case 57352:
				{
						sp->proc_interval	=	45000;
						sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK | PROC_ON_CAST_SPELL;
				}break;
			case 60493: //Dying Curse
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->proc_interval = 45000;
				}break;
		
			// Ashtongue Talisman	of Shadows 
			case  40478:
				{
						sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
				}break;

			// Ashtongue Talisman	of Swiftness
			case  40485:
			// Ashtongue Talisman	of Valor
			case  40458:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
			//Ashtongue Talisman of Zeal
			case 40470:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 50;
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->EffectTriggerSpell[0] = 40472;
						sp->maxstack = 1;
				}break;
		
			// Memento of	Tyrande
			case  37655:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 10;
						sp->proc_interval	=	45000;
				}break;
		
			// Ashtongue Talisman	of Insight
			case  40482:
				{
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
					}break;

		
			//Ashtongue	Talisman of	Equilibrium
			case  40442:
				{
						sp->Effect[0]	=	6;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 40;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->EffectTriggerSpell[0]	=	40452;
						sp->maxstack = 1;
						sp->Effect[1]	=	6;
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 25;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->EffectTriggerSpell[1]	=	40445;
						sp->maxstack = 1;
						sp->Effect[2]	=	6;
						sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 25;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->EffectTriggerSpell[2]	=	40446;
						sp->maxstack = 1;
				}break;
		
			//Ashtongue	Talisman of	Acumen
			case  40438:
				{
						sp->Effect[0]	=	6;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 10;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->EffectTriggerSpell[0]	=	40441;
						sp->maxstack = 1;
						sp->Effect[1]	=	6;
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 10;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->EffectTriggerSpell[1]	=	40440;
						sp->maxstack = 1;
				}break;
		
			//Ashtongue	Talisman of	Lethality
			case  40460:
				{
						sp->Effect[0]	=	6;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 20;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->EffectTriggerSpell[0]	=	40461;
						sp->maxstack = 1;
				}break;
			//Leatherworking Drums
			case 35475://Drums of War
			case 35476://Drums of Battle
			case 35478://Drums of Restoration
			case 35477://Drums of Speed
				{
					sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
					sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
					sp->EffectImplicitTargetA[2] = EFF_TARGET_NONE;
					sp->EffectImplicitTargetB[0] = EFF_TARGET_NONE;
					sp->EffectImplicitTargetB[1] = EFF_TARGET_NONE;
					sp->EffectImplicitTargetB[2] = EFF_TARGET_NONE;
				}break;

			/**********************************************************
			*	Misc stuff (ITEM SETS)
			**********************************************************/

			//Item Set:	Malorne	Harness
			case  37306:
			case  37311:
			//Item Set:	Deathmantle
			case  37170:
				{
						sp->procChance = 4;
						sp->procFlags	=	PROC_ON_MELEE_ATTACK;
				}break;
		
			//Item Set:	Netherblade
			case  37168:
				{
						sp->procChance = 15;
				}break;
		
			//Item Set:	Tirisfal Regalia
			case  37443:
				{
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;
		
			//Item Set:	Avatar Regalia
			case  37600:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 6;
				}break;
		
			//Item Set:	Incarnate	Raiment
			case  37568:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
		
			//Item Set:	Voidheart	Raiment
			case  37377:
				{
						sp->Effect[0]	=	6;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 5;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->proc_interval	=	20;
						sp->EffectTriggerSpell[0]	=	37379;
				}break;
			case  39437:
				{
						sp->Effect[0]	=	6;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 5;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->proc_interval	=	20;
						sp->EffectTriggerSpell[0]	=	37378;
				}break;
		
			//Item Set:	Cataclysm	Raiment
			case  37227:
				{
						sp->proc_interval	=	60000;
						sp->procChance = 100;
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;
		
			//Item Set:	Cataclysm	Regalia
			case  37228:
				{
						sp->procChance = 7;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
			case  37237:
				{
						sp->procChance = 25;
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;
		
			//Item Set:	Cataclysm	Harness
			case  37239:
				{
						sp->procChance = 2;
						sp->procFlags	=	PROC_ON_MELEE_ATTACK;
				}break;
		
			//Item Set:	Cyclone	Regalia
			case  37213:
				{
						sp->procChance = 11;
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;
		
			//Item Set:	Lightbringer Battlegear
			case  38427:
				{
						sp->procFlags	=	PROC_ON_MELEE_ATTACK;
						sp->procChance = 20;
				}break;
		
			//Item Set:	Crystalforge Battlegear
			case  37195:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 6;
				}break;
		
			//Item Set:	Crystalforge Raiment
			case  37189:
				{
						sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
						sp->proc_interval	=	60000;
				}break;

			case  37188:
			//Item Set:	Crystalforge Armor
			case  37191:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
				}break;
		
			//Item Set:	Destroyer	Armor
			case  37525:
				{
						sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM;
						sp->procChance = 7;
				}break;
		
			//Item Set:	Destroyer	Battlegear
			case  37528:
			//Item Set:	Warbringer Armor
			case  37516:
				{
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 100;
				}break;

			/**********************************************************
			*	Misc stuff (GLYPH)
			**********************************************************/
			
			case 55680:// Glyph of Prayer of Healing
			case 56176:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]   =   56161;
						sp->procFlags   =   PROC_ON_CAST_SPELL;
				}break; 
			case 58631: // Glyph of Icy Touch
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
				}break;

			// Glyph of reneved life
			case 58059: 
				{
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;	

			// Glyph of Vigor
			case 56805:
				{
					CopyEffect(dbcSpell.LookupEntryForced(21975), 0, sp, 2);
				}break;
		
			// Glyph of Lesser Healing Wave
			case 55438:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
				}break;

			// Glyph of Frostbolt
			case 56370:
				{
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;

			// Glyph of Revenge
			case 58364:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->EffectSpellClassMask[0][0]=0x0;
					sp->EffectSpellClassMask[0][1]=0x0;
				}break;
			// Glyph of Revenge Proc
			case 58363:
				{
					sp->AuraInterruptFlags  =   AURA_INTERRUPT_ON_CAST_SPELL;
				}break;
			case 56218://Glyph of Corruption
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0]	=	17941;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance = 4;
						sp->EffectSpellClassMask[0][0]=0x0;
				}break;

			//////////////////////////////////////////////////////
			// CLASS-SPECIFIC	SPELL	FIXES						//
			//////////////////////////////////////////////////////
		
			/* Note: when	applying spell hackfixes,	please follow	a	template */
		
			//////////////////////////////////////////
			// WARRIOR								//
			//////////////////////////////////////////
			case 7405: //	Sunder Armor
			case 7386: //	Sunder Armor
			case 8380: //	Sunder Armor
			case 11596:	// Sunder	Armor
			case 11597:	// Sunder	Armor
			case 25225:	// Sunder	Armor
			case 30330:	// Mortal	Strike Ranks 
			case 25248:	// Mortal	Strike Ranks 
			case 21553:	// Mortal	Strike Ranks 
			case 21552:	// Mortal	Strike Ranks 
			case 21551:	// Mortal	Strike Ranks 
			case 12294:	// Mortal	Strike Ranks 
			case 16511:	// Hemo	Rank 1
			case 17347:	// Hemo	Rank 2
			case 17348:	// Hemo	Rank 3
			case 26864:	// Hemo	Rank 4
				{
						sp->Unique = true;
				}break;
			// Wrecking Crew
			case 46867:
			case 56611:
			case 56612:
			case 56613:
			case 56614:
				{
						sp->procFlags	=	 PROC_ON_CRIT_ATTACK;
				}break;
			// Enrage
			case 12317:
			case 13045:
			case 13046:
			case 13047:
			case 13048:
				{
						sp->procFlags	=	 PROC_ON_ANY_DAMAGE_VICTIM;
				}break;
			// Improved Defensive Stance (Missing Parry Flag)
			case 29593:
			case 29594:
				{
						sp->procFlags	=	 PROC_ON_BLOCK_VICTIM | PROC_ON_DODGE_VICTIM;
				}break;
			// Sword and Board
			case 46951:
			case 46952:
			case 46953:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
			// Sword and Board Refresh
			case 50227:
				{
						sp->Effect[1]	=	3;
				}break;
			// Sword specialization
			case 12281:
			case 13960:
				{
						sp->procFlags = PROC_ON_MELEE_ATTACK;
						sp->procChance = 1;
				}break;
			case 12812:
			case 13961:
				{
						sp->procFlags = PROC_ON_MELEE_ATTACK;
						sp->procChance = 2;
				}break;
			case 12813:
			case 13962:
				{
						sp->procFlags = PROC_ON_MELEE_ATTACK;
						sp->procChance = 3;
				}break;
			case 12814:
			case 13963:
				{
						sp->procFlags = PROC_ON_MELEE_ATTACK;
						sp->procChance = 4;
				}break;
			case 12815:
			case 13964:
				{
						sp->procFlags = PROC_ON_MELEE_ATTACK;
						sp->procChance = 5;
				}break;
			// vigilance
			case 50720:
				{
						sp->MaxTargets = 1;
						sp->Unique = true;
						sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[2] = SPELL_AURA_REDIRECT_THREAT;
						sp->EffectImplicitTargetA[2] = 57;
						sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
				}break;

			// Damage Shield
			case 58872:
			case 58874 :
				{
						sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_BLOCK_VICTIM;
						sp->procChance = 100;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectImplicitTargetA[1] = 1;
						sp->EffectTriggerSpell[1] = 59653;
				}break;

			// Improved Hamstring
			case 12289:
			case 12668:
			case 23695:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->EffectSpellClassMask[0][0] = 2;
				}break;

			// Whirlwind
			case 1680:
				{
						sp->AllowBackAttack = true;
				}break;

			// Shockwave
			case 46968:
				{
					sp->EffectApplyAuraName[2] = SPELL_AURA_MOD_STUN;
					sp->EffectImplicitTargetA[0] = EFF_TARGET_IN_FRONT_OF_CASTER;
					sp->EffectImplicitTargetA[1] = EFF_TARGET_IN_FRONT_OF_CASTER;
					sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
					sp->School = SCHOOL_NORMAL;
					sp->Effect[1] = SPELL_EFFECT_SCHOOL_DAMAGE;
					sp->spell_can_crit = true;
				}break;

			// Blood Craze
			case 16487:
			case 16489:
			case 16492:
				{
					sp->procFlags = PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_CRIT_HIT_VICTIM;
				}break;

			// Gag Order
			case 12311:
			case 12958:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
				}break;

			//////////////////////////////////////////
			// PALADIN								//
			//////////////////////////////////////////
		
			// Insert	paladin	spell	fixes	here
			// Light's Grace PROC
			case 31834:
				{
						sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
				}break;

			//blessing of sanctuary / greater blessing of sanctuary
			case 20911:
			case 25899:
				{
						sp->procFlags = PROC_ON_DODGE_VICTIM | PROC_ON_BLOCK_VICTIM;
						sp->procChance = 100;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[1] = 57319;
						sp->EffectImplicitTargetA[1] = 6;
						sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
				}break;

			//Seal of Command - Holy	damage,	but	melee	mechanics	(crit	damage,	chance,	etc)
			case  20424:
				{
						sp->is_melee_spell = true;
						sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
						sp->School = SCHOOL_HOLY;
						sp->rangeIndex = 3;
				}break;
			
			//Illumination
			case 20210:
				{
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 20272;
						sp->procChance = 20;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;
			case 20212:
				{
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 20272;
						sp->procChance = 40;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;
			case 20213:
				{
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 20272;
						sp->procChance = 60;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;
			case 20214:
				{
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 20272;
						sp->procChance = 80;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;
			case 20215:
				{
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 20272;
						sp->procChance = 100;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;

			// Heart of the Crusader rank 1
			case 20335:
				{
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 21183;
						sp->procChance = 100;
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->EffectSpellClassMask[0][0] = 8388608;
				}break;

			// Heart of the Crusader rank 2
			case 20336:
				{
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 54498;
						sp->procChance = 100;
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->EffectSpellClassMask[0][0] = 8388608;
				}break;
	
			// Heart of the Crusader rank 3
			case 20337:
				{
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 54499;
						sp->procChance = 100;
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->EffectSpellClassMask[0][0] = 8388608;
				}break;	

			// Pursuit of Justice rank 1
			case 26022:
				{
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
						sp->EffectImplicitTargetA[1] = 1;
						sp->EffectBasePoints[1] = 8;
				}break;

			// Pursuit of Justice rank 2
			case 26023:
				{
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
						sp->EffectImplicitTargetA[1] = 1;
						sp->EffectBasePoints[1] = 15;
				}break;
			
			// Righteous Vengeance
			case 53380:
			case 53381:
			case 53382:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 61840;
					sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT;
				}break;

			// Sheat of Light (Hot Effect)
			case 53501:
			case 53502:
			case 53503:
				{
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1]	=	54203;
					sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT;
				}break;
			case 54203:
				{
					sp->logsId = sp->Id;
				}break;		
		
		
			// Sacred Shield
			case 53601:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->procChance = 100;
					sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM | PROC_TARGET_SELF;
					sp->EffectTriggerSpell[0] = 58597;
					sp->proc_interval = 6000;
				}break;
			case 31801: // Seal of Vengeance
				{
					sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 31803;
					sp->procFlags = PROC_ON_MELEE_ATTACK;
					sp->procChance = 100;
					sp->Effect[1] = 0;
					sp->EffectApplyAuraName[1] = 0;
					sp->EffectBasePoints[1] = 0;
				}break;

			case 53736:	// Seal of Corruption
				{
					sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 53742;
					sp->procFlags = PROC_ON_MELEE_ATTACK;
					sp->procChance = 100;
					sp->Effect[1] = 0;
					sp->EffectApplyAuraName[1] = 0;
					sp->EffectBasePoints[1] = 0;
				}break;	

			// SoC/SoV Dot's
			case 31803:
			case 53742:
				{
					sp->School = SCHOOL_HOLY; 
					sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
					sp->Unique = true;
				}break;

			// Judgement of righteousness
			case 20187:
			// Judgement of command
			case 20467:
			// Judgement of Justice/Wisdom/Light
			case 53733:
			// Judgement of Corruption/Vengeance      NOTE: Judgement of blood & Martyr was removed 3.2.0
			case 31804:
				{
					sp->School = SCHOOL_HOLY; 
					sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
				}break;

			// Divine Storm
			case 53385:
				{
					sp->AllowBackAttack = true;
				}break;


			//////////////////////////////////////////
			// HUNTER								//
			//////////////////////////////////////////

			//Hunter - Go for the Throat
			case 34950:
			case 34954:
				{
					sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;
				}break;
			case 34952:
			case 34953:
				{
					sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				}break;

			//Ranged Weapon Specialization
			case 19507: 
			case 19508:
			case 19509:
			case 19510:
			case 19511:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
					sp->EffectMiscValue[0] = 1;
				}break;
			//Two Handed Weapon Specialization and One Handed Weapon Specializacion
			//Warrior and Paladin and Death Knight - this will change on 3.1.0
			case 20111:
			case 20112:
			case 20113:
			case 12163:
			case 12711:
			case 12712:
			case 16538:
			case 16539:
			case 16540:
			case 16541:
			case 16542:
			case 20196:
			case 20197:
			case 20198:
			case 20199:
			case 20200:
			case 55107:
			case 55108:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
					sp->EffectMiscValue[0] = 2;
				}break;
			//Frost Trap
			case 13809:
				{
					sp->procFlags = PROC_ON_TRAP_TRIGGER;
					sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
					sp->Effect[2] = 0;  //stop strange effects
				}break;

			//Snakes from Snake Trap cast this, shouldn't stack
			case 25810:
			case 25809:
				{
					sp->maxstack = 1;
				}break;

			case  27065:
			case  20904:
			case  20903:
			case  20902:
			case  20901:
			case  20900:
			case  19434:
				{
						sp->Unique = true;
				}break;

			//Hunter : Entrapment
			case 19184:
			case 19387:
			case 19388:
				{
					sp->procFlags = PROC_ON_TRAP_TRIGGER;
				}break;
			// Hunter - Arcane Shot  - Rank 6 to 11
			case 14285:
			case 14286:
			case 14287:
			case 27019:
			case 49044:
			case 49045:
				{
					sp->Effect[0] = 2;
					sp->Effect[1] = 0;
					sp->EffectDieSides[1] = 0;
					sp->EffectBaseDice[1] = 0;
					sp->EffectBasePoints[0] = sp->EffectBasePoints[1];
					sp->EffectBasePoints[1] = 0;                  
					sp->EffectImplicitTargetA[0] = 6;
					sp->EffectImplicitTargetA[1] = 0;
				}break;

			// Misdirection
			case 34477:
				{
					sp->MaxTargets = 1;
					sp->Unique = true;
					sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[2] = SPELL_AURA_REDIRECT_THREAT;
				}break;

			// Misdirection proc
			case 35079:
				{
					sp->MaxTargets = 1;
					sp->Unique = true;
				}break;
			// Aspect of the Viper
			case 34074:
				{
					sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[2] = 34075;
					sp->procChance = 100;
					sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
				}break;
			// Viper String
			case 3034:
				{
					sp->EffectMultipleValue[0] = 3;
				}break;
			// Improved Steady Shot
			case 53221:
			case 53222:
			case 53224:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->EffectSpellClassMask[0][0] = 0x0;
					sp->EffectSpellClassMask[0][1] = 0x0;
				}break;


			// Lock and Load
			case 56342:
			case 56343:
			case 56344:
				{
					sp->procFlags = PROC_ON_TRAP_TRIGGER;
					sp->procChance = sp->EffectBasePoints[0] + 1;
				}break;

			// Lock and load proc
			case 56453:
				{
					sp->DurationIndex = 9;
				}break;

			// Master's Call
			case 53271:
				{
					sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 54216;
					sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 54216;
				}break;

			// Haunting party
			case 53290:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 57669;
					sp->EffectImplicitTargetA[0] = 1;
					sp->procChance = 33;
					sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;
			case 53291:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 57669;
					sp->EffectImplicitTargetA[0] = 1;
					sp->procChance = 66;
					sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;
			case 53292:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 57669;
					sp->EffectImplicitTargetA[0] = 1;
					sp->procChance = 100;
					sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;

			// Flare
			case 1543:
				{
					sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF | SPELL_FLAG_IS_TARGETINGSTEALTHED;
				}break;

			//////////////////////////////////////////
			// ROGUE								//
			//////////////////////////////////////////

			// Cheat Death
			case 31228:
			case 31229:
			case 31230:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
				}break;

			// Cloack of shadows
			// Cloack of shadows PROC
			case 31224:
			case 35729:
				{
						sp->DispelType = DISPEL_MAGIC;
						sp->AttributesEx |= 1024;
				}break;

			// Honor Among Thieves PROC
			case 52916:
				{
						sp->proc_interval = 4000; //workaround
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT | PROC_ON_CRIT_ATTACK | PROC_ON_RANGED_CRIT_ATTACK;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectImplicitTargetA[1] = 38;
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[1] = 51699;
				}break;

			// Shadow Dance
			case 51713:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
				}break;

			// Let's hack	the	entire cheating	death	effect to	damage perc	resist ;)
			case 45182:	// Cheating	Death	buff
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
						sp->EffectMiscValue[0] = -91;
				}break;
		
			// Wound Poison	Stuff
			case 27189:	
			case 13224:	
			case 13223:	
			case 13222:	
			case 13218:	
				{
						sp->Unique = true;
				}break;

			// Killing Spree
			case 51690:
				{
						sp->Effect[0]	=	SPELL_EFFECT_APPLY_AURA;
						sp->Effect[1]	=	0;
						sp->Effect[2]	=	0;
						sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
				}break;
			// Focused Attacks
			case 51634:
			case 51635:
			case 51636:
				{
						sp->procFlags = PROC_ON_CRIT_ATTACK;
				}break;

			// Setup
			case 13983:
				{
						sp->proc_interval = 1000;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->procFlags = PROC_ON_FULL_RESIST;
						sp->procChance = 33;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;
			case 14070:
				{
						sp->proc_interval = 1000;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->procFlags = PROC_ON_FULL_RESIST;
						sp->procChance = 66;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;
			case 14071:
				{
						sp->proc_interval = 1000;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->procFlags = PROC_ON_FULL_RESIST;
						sp->procChance = 100;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;

			// Setup PROC
			case 15250:
				{
						sp->proc_interval = 1000;
				}break;

			// Mutilate
			case 1329:
			case 34411:
			case 34412:
			case 34413:
			case 48663:
			case 48666:
				{
						sp->Effect[1] = 0;
						sp->procChance = 0;
						sp->Flags3 &=   ~FLAGS3_REQ_BEHIND_TARGET;
				}break;

			case 35541:
			case 35550:
			case 35551:
			case 35552:
			case 35553:
				{
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->procChance	=	20;
				}break;

			// Vanish - remove imparing movement effects
			case 11327:
			case 11329:
			case 26888:
				{
					sp->Effect[0] = SPELL_EFFECT_DUMMY;
					sp->EffectApplyAuraName[1] = 154;
				}
			//////////////////////////////////////////
			// PRIEST								//
			//////////////////////////////////////////
			
			// Dispersion (org spell)
			case 47585:
				{
						sp->AdditionalAura = 47218;
				}break;

			// Dispersion (remove im effects, in 3.1 there is a spell for this)
			case 47218:
				{
						sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_MECHANIC_IMMUNITY;
						sp->EffectApplyAuraName[1] = SPELL_AURA_MECHANIC_IMMUNITY;
						sp->EffectMiscValue[0] = 7;
						sp->EffectMiscValue[1] = 11;
						sp->EffectImplicitTargetA[0] = 1;
						sp->EffectImplicitTargetA[1] = 1;
				}break;

			// Mass dispel
			case 32375:
			case 32592:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->procChance = 100;
						sp->EffectImplicitTargetA[1] = 1;
				}break;

				// Power Infusion
			case 10060:
				{
					sp->c_is_flags = SPELL_FLAG_IS_FORCEDBUFF;
				}break;

			// Prayer of mending (tricky one :() rank 1
			case 33076:
				{
						sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 41635;
						sp->Effect[1] = SPELL_EFFECT_DUMMY;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PARTY_MEMBER;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_PARTY_MEMBER;
						sp->EffectImplicitTargetB[0] = 0;
						sp->EffectImplicitTargetB[1] = 0;
				}break;

			// Prayer of mending (tricky one :() rank 2
			case 48112:
				{
						sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 48110;
						sp->Effect[1] = SPELL_EFFECT_DUMMY;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PARTY_MEMBER;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_PARTY_MEMBER;
						sp->EffectImplicitTargetB[0] = 0;
						sp->EffectImplicitTargetB[1] = 0;
				}break;

			// Prayer of mending (tricky one :() rank 3
			case 48113:
				{
						sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 48111;
						sp->Effect[1] = SPELL_EFFECT_DUMMY;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_PARTY_MEMBER;
						sp->EffectImplicitTargetA[1] = EFF_TARGET_PARTY_MEMBER;
						sp->EffectImplicitTargetB[0] = 0;
						sp->EffectImplicitTargetB[1] = 0;
				}break;

			// triggered spell, rank 1
			case 41635:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 41637;
						sp->EffectBasePoints[0] = 800;
						sp->procCharges = 0;
						sp->procChance = 100;
						sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
				}break;

			// triggered spell, rank 2
			case 48110:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 41637;
						sp->EffectBasePoints[0] = 905;
						sp->procCharges = 0;
						sp->procChance = 100;
						sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
				}break;

			// triggered spell, rank 3
			case 48111:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 41637;
						sp->EffectBasePoints[0] = 1043;
						sp->procCharges = 0;
						sp->procChance = 100;
						sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
				}break;

			// triggered on	hit, this	is the spell that	does the healing/jump
			case 41637:
				{
						sp->Effect[0]	=	SPELL_EFFECT_DUMMY;
						sp->EffectBasePoints[0]	=	5;
						sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
						sp->EffectImplicitTargetB[0] = 0;
				}break;	

			// Inner Focus
			case 14751:
				{
						sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
				}break;

			// Divine Aegis
			case 47509:
			case 47511:
			case 47515:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectImplicitTargetA[0] = 21;
					sp->EffectTriggerSpell[0] = 47753;
				}break;

			// Insert	priest spell fixes here
		
			//////////////////////////////////////////
			// SHAMAN								//
			//////////////////////////////////////////
			case 51466:
			case 51470: //Elemental Oath
				{
					sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[1] = SPELL_AURA_ADD_FLAT_MODIFIER;
					sp->EffectMiscValue[1] = SMT_LAST_EFFECT_BONUS;
					sp->EffectSpellClassMask[1][0] = 0;
					sp->EffectSpellClassMask[1][1] = 0x00004000; // Clearcasting
				}break;
			case 51562:
			case 51563:
			case 51564:
			case 51565:
			case 51566: // Tidal Waves
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->EffectSpellClassMask[0][0] = 0x00000100;	// Chain heal
					sp->EffectSpellClassMask[0][1] = 0x00000000;
					sp->EffectSpellClassMask[0][2] = 0x00000010;	// Riptide
				}break;
			case 53390:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
			case 51940:
			case 51989:
			case 52004:
			case 52005:
			case 52007:
			case 52008: // Earthliving Weapon
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->procChance = 20;
				}break;
			case 51945:
			case 51990:
			case 51997:
			case 51998:
			case 51999:
			case 52000:
				{
					sp->logsId = sp->Id;
				}break; 
			case 55198:	// Tidal Force
				{
					sp->Effect[0] = SPELL_EFFECT_DUMMY;
					sp->EffectApplyAuraName[0] = 0;
					sp->EffectTriggerSpell[0] = 55166;
				}break;
			case 55166: 
				{
					sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;
			case 51525:
			case 51526:
			case 51527:	// Static Shock
				{
					sp->procFlags = PROC_ON_MELEE_ATTACK;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = sp->Id;
					sp->EffectSpellClassMask[0][0] = 0;
				}break;
			case 16180:
			case 16196:
			case 16198: // Improved Water Shield
				{
					sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectSpellClassMask[0][0] = 0x000000C0; // Healing Wave and Lesser Healing Wave
					sp->EffectSpellClassMask[0][2] = 0x00000010; //Riptide
					sp->EffectTriggerSpell[0] = sp->Id;
				}break;
			case 16187:
			case 16205:
			case 16206:
			case 16207:
			case 16208: // Restorative Totems
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
					sp->EffectMiscValue[0] = SMT_DAMAGE_DONE;
					sp->EffectApplyAuraName[1] = SPELL_AURA_ADD_PCT_MODIFIER;
				}break;
			case 31616: // Nature's Guardian
				{
					sp->logsId = sp->Id;
					sp->spell_can_crit = false;
				}break;

			// Improved firenova totem
			case 16086:
			case 16544:
				{
					sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;		
		
			//////////////////////////////////////////
			// MAGE	spell						//
			//////////////////////////////////////////
		
			// Insert	mage spell fixes here
			
			// Invisibility
			case 66:
				{
					sp->EffectTriggerSpell[1] = 32612;
					sp->EffectAmplitude[1]	=	3000;
				}break;

			// Invisibility part	2
			case  32612:
				{
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
						sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL | AURA_INTERRUPT_ON_START_ATTACK | AURA_INTERRUPT_ON_HOSTILE_SPELL_INFLICTED;
				}break;
			//mirror image
			case 55342:
				{
					sp->EffectTriggerSpell[0] = 31216;
				}
			//improved blink
			case 31569:
			case 31570:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
				}break;

			// Magic Absorption
			case 29441:
			case 29444:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 29442;
				}break;

			//Missile Barrage
			case 44404:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->procChance = 4;
				}break;
			case 54486:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->procChance = 8;
				}break;
			case 54488:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->procChance = 12;
				}break;
			case 54489:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->procChance = 16;
				}break;
			case 54490:
				{
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->procChance = 20;
				}break;
			//Fiery Payback 
			case 44440:
			case 44441:
				{
					sp->procChance	=	100;
					sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_ANY_DAMAGE_VICTIM;
				}break;

			//Fingers of Frost
			case 44543:
				{
					sp->procFlags	=	PROC_ON_CAST_SPELL;
					sp->procChance	=	5;
				}break;
			case 44545:
				{
					sp->procFlags	=	PROC_ON_CAST_SPELL;
					sp->procChance	=	10;
				}break;

			//Conjure Refreshment table
			case 43985:
			case 58661:
				{
					sp->EffectImplicitTargetA[0]	=	EFF_TARGET_DYNAMIC_OBJECT;
				}break;

			// Ritual of Refreshment
			case 43987:
			case 58659:
				{
					sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
				}break;

			// Living bomb
			case 44457:
			case 55359:
			case 55360:
				{
					sp->c_is_flags |= SPELL_FLAG_ON_ONLY_ONE_TARGET;
				}break;

			// Arcane Potency proc
			case 57529:
			case 57531:
				{
						sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
				}break;

			// Burnout
			case 44449:
			case 44469:
			case 44470:
			case 44471:
			case 44472:
				{
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[1] = 44450;
						sp->EffectImplicitTargetA[1] = 1;
						sp->procChance = 100;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
				}break;

			//////////////////////////////////////////
			// WARLOCK								//
			//////////////////////////////////////////
		
			// Insert	warlock	spell	fixes	here
			// Demonic Knowledge
			case 35691:
			case 35692:
			case 35693:
				{
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->procChance = 100;
						sp->procFlags = PROC_ON_CAST_SPELL;
						sp->EffectImplicitTargetA[1] = 5;
						sp->EffectTriggerSpell[1] = 35696;
				}break;

			// Demonic Knowledge PROC
			case 35696:
				{
						sp->EffectImplicitTargetA[0] = 1;
						sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
				}break;

			// Demonic Pact AA
			case 48090:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AREA_AURA;
					sp->Effect[1] = SPELL_EFFECT_APPLY_AREA_AURA;
				}break;

			// Demonic Pact
			case 53646:
				{
						sp->procChance = 100;
						sp->procFlags = PROC_ON_MELEE_ATTACK;
				}break;
			// Unstable Affliction
			case 30108:
			case 30404:
			case 30405:
			case 47841:
			case 47843:
				{
						sp->procFlags = PROC_ON_DISPEL_AURA_VICTIM;
						sp->procChance = 100;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectImplicitTargetA[1] = 6;
						sp->EffectTriggerSpell[1] = 31117;
				}break;
		// Death's Embrace
			case 47198:
			case 47199:
			case 47200:
				{
						sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
						sp->EffectImplicitTargetA[2] = 1;
				}break;

			// Everlasting affliction
			case 47201:
			case 47202:
			case 47203:
			case 47204:
			case 47205:
				{
						sp->procFlags = PROC_ON_SPELL_LAND;
				}break;

			//warlock - Molten Core
			case 47245:
			case 47246:
			case 47247:
				{
					sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
				}break;

			// Pandemic
			case 58435:
			case 58436:
			case 58437:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 58691;
					sp->EffectImplicitTargetA[0] = 6;
				}break;

			// Mana Feed
			case 30326:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 32554;
					sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
				}break;

			// Fel Synergy
			case 47230:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 54181;
				}break;
			case 47231:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 54181;
				}break;

			//////////////////////////////////////////
			// DRUID								//
			//////////////////////////////////////////
		
			// Insert	druid	spell	fixes	here
			case 22570:
			case 49802:	// Maim
				{
						sp->AuraInterruptFlags |=	AURA_INTERRUPT_ON_UNUSED2;
						sp->Attributes |=	ATTRIBUTES_STOP_ATTACK;
				}break;
		
			// Feral Charge - cat
			case 49376:
				{
						sp->Effect[2] = 0;
						sp->EffectTriggerSpell[2] = 0;
						sp->Effect[1] = SPELL_EFFECT_TELEPORT_UNITS;
				}break;

			// RAVAGE
			case 6785:
				{
						sp->EffectBasePoints[0] = 162;			//r1
				}break;
			case 6787:
				{
						sp->EffectBasePoints[0] = 239;			//r2
				}break;
			case 9866:
				{
						sp->EffectBasePoints[0] = 300;			//r3
				}break;
			case 9867:
				{
						sp->EffectBasePoints[0] = 377;			//r4
				}break;
			case 27005:
				{
						sp->EffectBasePoints[0] = 566;			//r5
				}break;
			case 48578:
				{
						sp->EffectBasePoints[0] = 1405;			//r6
				}break;
			case 48579:
				{
						sp->EffectBasePoints[0] = 1770;			//r7
				}break;
			
			//SHRED
			case 5221:
				{
						sp->EffectBasePoints[0] = 54;			//r1
				}break;
			case 6800:
				{
						sp->EffectBasePoints[0] = 72;			//r2
				}break;
			case 8992:
				{
						sp->EffectBasePoints[0] = 99;			//r3
				}break;
			case 9829:
				{
						sp->EffectBasePoints[0] = 144;			//r4
				}break;
			case 9830:
				{
						sp->EffectBasePoints[0] = 180;			//r5
				}break;
			case 27001:
				{
						sp->EffectBasePoints[0] = 236;			//r6
				}break;
			case 27002:
				{
						sp->EffectBasePoints[0] = 405;			//r7
				}break;
			case 48571:
				{
						sp->EffectBasePoints[0] = 630;			//r8
				}break;
			case 48572:
				{
						sp->EffectBasePoints[0] = 742;			//r9
				}break;

			// Natural reaction
			case 57878:
			case 57880:
			case 57881:
				{
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->procFlags = 0;
						sp->procChance = 0;
						sp->EffectTriggerSpell[1] = 0;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;

			// Dash
			case 1850:
			case 9821:
			case 33357:
				{
						sp->RequiredShapeShift = 1;
				}break;

			// Totem of Wrath
			case 30708:
				{
						sp->Effect[0] = 6;
						sp->EffectImplicitTargetA[0] = 22;
						sp->EffectImplicitTargetB[0] = 15;
						sp->EffectRadiusIndex[0] = 10; //30 yard
				}break;
			/*
			//Totem of Wrath rank 2
			case 57720:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectTriggerSpell[0] = 30652;
					sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;
			//Totem of wrath rank 3
			case 57721:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectTriggerSpell[0] = 30653;
					sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;
			//Totem of wrath rank 4
			case 57722:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectTriggerSpell[0] = 30654;
					sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;*/

			// Mangle - Cat 
			case 33876:
				{
						sp->EffectBasePoints[0] = 198; //rank 1
				}break;
			case 33982:
				{
						sp->EffectBasePoints[0] = 256; //rank 2
				}break;
			case 33983:
				{
						sp->EffectBasePoints[0] = 330; //rank 3
				}break;
			case 48565:
				{
						sp->EffectBasePoints[0] = 536; //rank 4
				}break;
			case 48566:
				{
						sp->EffectBasePoints[0] = 634; //rank 5
				}break;
			// Mangle - Bear 
			case 33878:
				{
						sp->EffectBasePoints[0] = 86; //rank 1
				}break;
			case 33986:
				{
						sp->EffectBasePoints[0] = 120; //rank 2
				}break;
			case 33987:
				{
						sp->EffectBasePoints[0] = 155; //rank 3
				}break;
			case 48563:
				{
						sp->EffectBasePoints[0] = 251; //rank 4
				}break;
			case 48564:
				{
						sp->EffectBasePoints[0] = 299; //rank 5
				}break;

			//Druid - Master Shapeshifter
			case 48411:
			case 48412:
				{
						sp->Effect[0]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0]	=	SPELL_AURA_DUMMY;
						sp->Effect[1]	=	0;
						sp->Effect[2]	=	0;
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->procChance	=	100;
				}break;
			case 48418:
				{
					sp->RequiredShapeShift	=	(uint32)(1<<(FORM_BEAR-1))|(1<<(FORM_DIREBEAR-1));
				}break;
			case 48420:
				{
					sp->RequiredShapeShift	=	(uint32)(1<<(FORM_CAT-1));
				}break;
			case 48421:
				{
					sp->RequiredShapeShift	=	(uint32)1 << (FORM_MOONKIN-1);
				}break;
			case 48422:
				{
					sp->RequiredShapeShift	=	(uint32)1 << (FORM_TREE-1);
				}break;
			//Owlkin Frenzy
			case 48389:
			case 48392:
			case 48393:
				{
					sp->procFlags	=	PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
					sp->EffectSpellClassMask[0][0]	=	0x0;
				}break;
			// Infected Wounds
			case 48483:
			case 48484:
			case 48485:
				{
					sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_MELEE_ATTACK;
				}break;
			// Swipe (cat) max targets, fixed in 3.1
			case 62078:
				{
					sp->MaxTargets = 10;
				}break;
			// faerie fire (feral dmg)
			case 16857:
				{
					sp->Effect[1] = SPELL_EFFECT_SCHOOL_DAMAGE;
					sp->EffectBasePoints[1] = 1;
					sp->EffectImplicitTargetA[1] = 6;
				}break;

			// King of the jungle dmg buff
			case 51185:
				{
					sp->DurationIndex = 1;
					sp->RequiredShapeShift = (uint32)(1<<(FORM_BEAR-1))|(1<<(FORM_DIREBEAR-1));
				}break;
			case 60200:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
				}break;

			// Eclipse
			case 48516:
			case 48521:
			case 48525:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 48517;
					sp->EffectTriggerSpell[1] = 48518;
				}break;

			// Living Seed
			case 48496:
			case 48499:
			case 48500:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectImplicitTargetA[0] = 21;
					sp->EffectTriggerSpell[0] = 48503;
				}break;

			// Healing way
			case 29203:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = 108; //ADD PCT MOD
					sp->EffectMiscValue[0] = 0;
					sp->EffectSpellClassMask[0][0] = 64;
					sp->EffectSpellClassMask[0][1] = 0;
					sp->EffectSpellClassMask[0][2] = 0;
				}break;

			//////////////////////////////////////////
			// DEATH KHINGH							//
			//////////////////////////////////////////

			// Merciless Combat
			case 49024:
			case 49538:
				{
					sp->EffectMiscValue[0] = 7278;
				}break;

			//////////////////////////////////////////
			// BOSSES								//
			//////////////////////////////////////////
		
			// Insert	boss spell fixes here

			// War Stomp
			case 20549:
				{
					sp->RequiredShapeShift = 0;
				}break;
			
			// Dark	Glare
			case  26029:
				{
						sp->cone_width = 15.0f;	// 15	degree cone
				}break;
		
			// Commendation	of Kael'thas
			case  45057:
				{
						sp->proc_interval	=	30000;
				}break;
		
			// Recently	Dropped	Flag
			case  42792:
				{
						sp->c_is_flags |=	SPELL_FLAG_IS_FORCEDDEBUFF;
				}break;
		
			case  43958:
				{
						sp->Effect[0]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
						sp->DurationIndex	=	6; //	10 minutes.
						sp->c_is_flags |=	SPELL_FLAG_IS_FORCEDDEBUFF;
		
						sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_ADD_PCT_MODIFIER;
						sp->EffectMiscValue[1] = SMT_RESIST_DISPEL;
						sp->EffectBasePoints[1]	=	90;
				}break;
		
			// Recently	Dropped	Flag
			case 43869:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
						sp->c_is_flags |=	SPELL_FLAG_IS_FORCEDDEBUFF;
				}break;
		
			case 48978:
			case 61216:
				{
						sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
						sp->EffectMiscValue[1] = 1;
				}break;
		
			case 49390:
			case 61221:
				{
						sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
						sp->EffectMiscValue[1] = 2;
				}break;
		
			case 49391:
			case 61222:
				{
						sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
						sp->EffectMiscValue[1] = 3;
				}break;
		
			case 49392:
				{
						sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
						sp->EffectMiscValue[1] = 4;
				}break;
		
			case 49393:
				{
						sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
						sp->EffectMiscValue[1] = 5;
				}break;
		
			// Furious Attacks
			case 46910:
				{
						sp->EffectTriggerSpell[0]	=	56112;
						sp->procChance = 50;
						sp->procFlags	|= PROC_ON_MELEE_ATTACK;
				}break;
		
			case 46911:
				{
						sp->EffectTriggerSpell[0]	=	56112;
						sp->procChance = 100;
						sp->procFlags	|= PROC_ON_MELEE_ATTACK;
				}break;
		
			// Rogue:	Hunger for Blood!
			case 51662:
				{
						sp->Effect[1]	=	SPELL_EFFECT_DUMMY;
				}break;
		
			// Mage: Focus Magic
			case 54646:
				{
						sp->c_is_flags = SPELL_FLAG_ON_ONLY_ONE_TARGET;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
						sp->procChance = 100;
						sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[1] = 54648;
				}break;
		
			// Mage: Hot Streak
			case 44445:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 48108;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
						sp->procChance = 33;
						sp->EffectSpellClassMask[0][0] = 0x0;
				}break;
			case 44446:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 48108;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
						sp->procChance = 66;
						sp->EffectSpellClassMask[0][0] = 0x0;
				}break;
			case 44448:
				{
						sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
						sp->EffectTriggerSpell[0] = 48108;
						sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
						sp->procChance = 100;
						sp->EffectSpellClassMask[0][0] = 0x0;
				}break;

			case 1122:
				{
						sp->EffectBasePoints[0]	=	0;
				}break;

			case 1860:
			case 20719:
				{
						sp->Effect[0]	=	SPELL_EFFECT_DUMMY;
				}break;
				//Warrior Bloodsurge
			case 46915:
			case 46914:
			case 46913:
				{
					sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_CAST_SPELL;
				}break;
			//Rogue - Waylay			 
			case  51692:
			case  51696:
				{	 
						sp->procFlags	=	PROC_ON_CRIT_ATTACK;
				}break;

			// Rogue - Cold	Blood
			case  14177:
				{	 
						sp->procFlags	=	PROC_ON_CAST_SPELL;
						sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
				}break;

			// priest	-	mind flay
			case 15407:
			case 17311:
			case 17312:
			case 17313:
			case 17314:
			case 18807:
			case 25387:
			case 48155:
			case 48156:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
					sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
					sp->logsId = 58381;
				}break;
			// Death and Decay
			case 43265:
			case 49936:
			case 49937:
			case 49938:
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
				}break;

		
			//Warlock	Chaos	bolt
			case 50796:
			case 59170:
			case 59171:
			case 59172:
				{
					sp->Attributes = ATTRIBUTES_IGNORE_INVULNERABILITY;
				}break;

			//Force	debuff's	 
			// Hypothermia
			case  41425:	 
			// Forbearance
			case  25771:	 
			// Weakened	Soul
			case  6788:
				{
						sp->c_is_flags = SPELL_FLAG_IS_FORCEDDEBUFF;	 
				}break;

			// Death Knight	spell	fixes 

			
			//pestilence
			case 50842:
				{
					sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 51429;
					sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
					sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
				}break;

			//Outbreak
			case 49013:
			case 55236:
			case 55237:
				{
					sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[1] = SPELL_AURA_ADD_PCT_MODIFIER;
					sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[2] = SPELL_AURA_ADD_PCT_MODIFIER;
					sp->EffectMiscValue[1] = SMT_DAMAGE_DONE;
					sp->EffectMiscValue[2] = SMT_DAMAGE_DONE;
				}break;

			//Vicious Strikes
			case 51745:
			case 51746:
				{
					sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_CRIT_PERCENT;
					sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[2] = SPELL_AURA_ADD_PCT_MODIFIER;
					sp->EffectMiscValue[2] = SMT_CRITICAL_DAMAGE;
				}break;

			//Butchery - Gives you runic power after you kill and enemy and while you are combat it generates 1-2 runic power and gives you 10-20 rune powy after npc or playa death
			case 48979:
			case 49483:
				{
					sp->Effect[0] = 6;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 50163;
					sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
				}break;
			//necrosis
			case 51459:
			case 51462:
			case 51463:
			case 51464:
			case 51465:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 51460;
				}break;
			//Desecration
			case 55666:
			case 55667:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 55741;
					sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
			//Dirge
			case 49223:
			case 49599:
				{
					sp->Effect[0] = 6;
					sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0] = 51206;
				}break;
			//Rune Strike
			case 56815:
			case 56816:
			case 56817:
			case 62036:
				{
						sp->Attributes |=	ATTRIBUTES_CANT_BE_DPB; //Note: This makes Rune strike undodge-parry-blockable
				}break;
					
			//Chilblains
			case 50040:
			case 50041:
			case 50043:
				{
					sp->EffectApplyAuraName[0] = 109;
					sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
			//Raise Dead
		
			//Blade	Barrier
			case 49182:
			case 49500:
			case 49501:
			case 55225:
			case 55226:
				{
					sp->procFlags	=	PROC_ON_CAST_SPELL;
					sp->procChance = 100;
				}break;

			// Killing Machine
			case 51123:
				{
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->ProcsPerMinute = 1;
				}break;
			case 51127:
				{
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->ProcsPerMinute = 2;
				}break;
			case 51128:
				{
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->ProcsPerMinute = 3;
				}break;
			case 51129:
				{
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->ProcsPerMinute = 4;
				}break;
			case 51130:
				{
					sp->procFlags	=	PROC_ON_MELEE_ATTACK;
					sp->ProcsPerMinute = 5;
				}break;
			case 49175:
			case 50031:
			case 51456:	// Improved Icy Touch
				{
					sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
				}break;
			case 48743:	// Death Pact
				{
					sp->Effect[1] = SPELL_EFFECT_NULL;	// Incorrect targetting makes it kill everyone around
				}break;

			case 49143:
			case 51416:
			case 51417:
			case 51418:
			case 51419:
			case 55268: // Frost Strike
				{
					sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
				}break;
			case 55090:
			case 55265:
			case 55270:
			case 55271: // Scourge Strike
				{
					sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
				}break;

			case 1843:	// Disarm
				{
					sp->Effect[0] = 0;	// to prevent abuse at Arathi
				}break;
		
			// Bone	Shield
			case  49222:
				{
						sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;
						sp->proc_interval	=	3000;
				}break;
			// Shadow	of Death
			case  49157:
				{
						sp->Effect[0]	=	0;	// don't want	DKs	to be	always invincible
				}break;
		
			// Death Grip
			case  49576:
				{
						sp->FacingCasterFlags	=	0;
				}break;
		
			// shadow	of death
			case 54223:
				{
						sp->Effect[2]	=	SPELL_EFFECT_APPLY_AURA;
						sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
						sp->Flags4 |=	CAN_PERSIST_AND_CASTED_WHILE_DEAD;
				}break;
			case 54749://Burning Determination
			case 54747:
				{
					sp->procFlags = PROC_ON_SPELL_LAND_VICTIM;
				}break;
			case 48266://blood presence
				{
					sp->EffectTriggerSpell[1] = 50475;
					sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_MELEE_ATTACK;
				}break;
			case 50475:
				{
					sp->Effect[0] = SPELL_EFFECT_NULL;
				}break;
			case 48263://Frost Presence
				{
					sp->AdditionalAura = 61261;
				}break;
			case 48265://Unholy Presence
				{
					sp->AdditionalAura = 49772;
				}break;
			case 56364:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
				}break;
			case 44443://Firestarter
			case 44442:
				{
					sp->procFlags = PROC_ON_CAST_SPELL;
					sp->EffectSpellClassMask[0][0] = 0x0;
					sp->EffectSpellClassMask[0][1] = 0x0;
				}break;
			//Mage - Brain Freeze
			case 44546:
			case 44584:
			case 44549:
				{
					sp->procFlags	=	PROC_ON_CAST_SPELL;
					sp->EffectSpellClassMask[0][0] = 0x0;
					sp->EffectSpellClassMask[0][1] = 0x0;
				}break;
			case 54741:
				{
					sp->AuraInterruptFlags	=	AURA_INTERRUPT_ON_CAST_SPELL;
				}break;

			case 56368:
				{
					sp->Effect[1] = sp->Effect[0];
					sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
				}break;
			case 48020:
				{
					sp->Effect[1] = SPELL_EFFECT_DUMMY;
				}break;
			case 48018:
				{
					sp->AdditionalAura = 62388;
					sp->EffectImplicitTargetA[0] = 1;
				}break;

			//Noise Machine - Sonic Shield
			case 54808:
				{
					sp->EffectApplyAuraName[0]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0]	=	55019;
					sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM;
					sp->proc_interval	=	60000;
					sp->procChance	=	50;
				}break;
			//Pendulum of Telluric Currents
			case 60482:
				{
					sp->EffectApplyAuraName[0]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0]	=	60483;
					sp->procFlags	=	PROC_ON_CAST_SPELL;
					sp->procChance	=	15;
				}break;
			//Tears of Bitter Anguish
			case 58901:
				{
					sp->EffectApplyAuraName[0]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
					sp->EffectTriggerSpell[0]	=	58904;
					sp->procFlags	=	PROC_ON_CRIT_ATTACK;
					sp->procChance	=	10;
				}break;
			case 20578:
				sp->AuraInterruptFlags = AURA_INTERRUPT_ON_MOVEMENT|AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN;
				break;
			// Learn Dual Talent Specialization
			case 63624:
				{
					sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
					sp->EffectTriggerSpell[1] = 63680;
				}break;
			case 51729:
				{
					sp->buffIndexType = 0;
					sp->buffType = 0;
					sp->AreaGroupId = 0;
				}break;
			case 58691://Pandemic
				{
					sp->spell_can_crit = false;
				}break;
			case 54197:
				{
					sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
					sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
				}break;
			//Warlock - Nether Protection
			case 30299:
			case 30301:
			case 30302:
				{
					sp->procFlags = PROC_ON_SPELL_HIT_VICTIM;
				}break;
		}

		//////////////////////////////////////////////////////////////////
		//AREA AURA TARGETS - START
		//////////////////////////////////////////////////////////////////
		switch( sp->NameHash )
		{
		//Pal
		case SPELL_HASH_CONCENTRATION_AURA:
		case SPELL_HASH_RETRIBUTION_AURA:
		case SPELL_HASH_DIVINE_GUARDIAN:
		//Hunter
		case SPELL_HASH_ASPECT_OF_THE_WILD:
		case SPELL_HASH_TRUESHOT_AURA:
		//Death knight
		case SPELL_HASH_PATH_OF_FROST:
		//Druid
		case SPELL_HASH_MOONKIN_AURA:
		case SPELL_HASH_IMPROVED_MOONKIN_FORM:
		//Warlock
		case SPELL_HASH_BLOOD_PACT:
		case SPELL_HASH_FEL_INTELLIGENCE:
			{
				sp->AreaAuraTarget = AA_TARGET_RAID;
			}break;

		//Pal
		case SPELL_HASH_DEVOTION_AURA:
		case SPELL_HASH_CRUSADER_AURA:
		case SPELL_HASH_SHADOW_RESISTANCE_AURA:
		case SPELL_HASH_FROST_RESISTANCE_AURA:
		case SPELL_HASH_FIRE_RESISTANCE_AURA:
		//Hunter
		case SPELL_HASH_ASPECT_OF_THE_PACK:
		//Shaman
		case SPELL_HASH_FIRE_RESISTANCE:
		case SPELL_HASH_FROST_RESISTANCE:
		case SPELL_HASH_FLAMETONGUE_TOTEM:
		case SPELL_HASH_NATURE_RESISTANCE:
		case SPELL_HASH_STONESKIN:
		case SPELL_HASH_STRENGTH_OF_EARTH:
		case SPELL_HASH_WINDFURY_TOTEM:
		case SPELL_HASH_WRATH_OF_AIR_TOTEM:
        //Priest
        case SPELL_HASH_PRAYER_OF_FORTITUDE:
        case SPELL_HASH_PRAYER_OF_SHADOW_PROTECTION:
        case SPELL_HASH_PRAYER_OF_SPIRIT:
        //Warrior
        case SPELL_HASH_BATTLE_SHOUT:
        case SPELL_HASH_COMMANDING_SHOUT:
			{
				sp->AreaAuraTarget = AA_TARGET_PARTY;
			}break;

		//Hunter
		case SPELL_HASH_ASPECT_OF_THE_BEAST:
			{
				sp->AreaAuraTarget = AA_TARGET_PET;
			}break;
		//Rogue
		case SPELL_HASH_HONOR_AMONG_THIEVES:
			{
				sp->AreaAuraTarget = AA_TARGET_NOTSELF | AA_TARGET_PARTY;
			}break;
		}

		switch( sp->Id )
		{
		//Druid - tree of life
		case 34123:
		//Warlock - Demonic Pact
		case 48090:
			{
				sp->AreaAuraTarget = AA_TARGET_RAID;
			}break;

		case 30708:
			{
				sp->AreaAuraTarget = AA_TARGET_ALLENEMIES;
			}break;

		//Druid - leader of the pack
		case 24932:
			{
				sp->AreaAuraTarget = AA_TARGET_PARTY;
			}break;
		}

		//////////////////////////////////////////////////////////////////
		//AREA AURA TARGETS - END
		//////////////////////////////////////////////////////////////////

		if( IsDamagingSpell( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_DAMAGING;
		if( IsHealingSpell( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_HEALING;
		if( IsTargetingStealthed( sp ) )
			sp->c_is_flags |= SPELL_FLAG_IS_TARGETINGSTEALTHED;
		if( IsFlyingSpell(sp) )
			sp->c_is_flags |= SPELL_FLAG_IS_FLYING;
		if( IsCastedOnFriends(sp) )
			sp->c_is_flags |= SPELL_FLAG_CASTED_ON_FRIENDS;
		if( IsCastedOnEnemies(sp) )
			sp->c_is_flags |= SPELL_FLAG_CASTED_ON_ENEMIES;
	}
}
void ApplyNormalFixes()
{
	//Updating spell.dbc
	SpellEntry *sp;

	Log.Notice("World", "Processing %u spells...", dbcSpell.GetNumRows());
	uint32 cnt = (uint32)dbcSpell.GetNumRows();
	uint32 effect;

	map<uint32, uint32> talentSpells;
	map<uint32,uint32>::iterator talentSpellIterator;
	unsigned int i,j;
	for(i = 0; i < dbcTalent.GetNumRows(); ++i)
	{
		TalentEntry * tal = dbcTalent.LookupRow(i);
		for(j = 0; j < 5; ++j)
			if(tal->RankID[j] != 0)
				talentSpells.insert(make_pair(tal->RankID[j], tal->TalentTree));
	}


	for(uint32 x=0; x < cnt; x++)
	{
		sp = dbcSpell.LookupRow(x);

		uint32 result = 0;

		uint32 type = 0;
		uint32 namehash = 0;

		sp->forced_creature_target = 0;
		sp->AdditionalAura = 0;
		sp->poison_type = 0;
		sp->self_cast_only = false;
		sp->Unique = false;
		sp->apply_on_shapeshift_change = false;
		sp->always_apply = false;

		// hash the name
		//!!!!!!! representing all strings on 32 bits is dangerous. There is a chance to get same hash for a lot of strings ;)
		namehash = crc32((const unsigned char*)sp->Name, (unsigned int)strlen(sp->Name));
		sp->NameHash   = namehash; //need these set before we start processing spells

		float radius = 0.0f;
		if(sp->EffectRadiusIndex[0] != 0)
			radius=::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[0]));
		
		if( sp->EffectRadiusIndex[1] != 0 )
			radius = std::max(radius,::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[1])));

		if( sp->EffectRadiusIndex[2] != 0 )
			radius=std::max(::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[2])),radius);

		radius=std::max(GetMaxRange(dbcSpellRange.LookupEntry(sp->rangeIndex)),radius);
		sp->base_range_or_radius = radius;
		sp->base_range_or_radius_sqr = radius*radius;
		sp->cone_width = 0.0f;
		sp->area_aura_update_interval = 2000;

		// NEW SCHOOLS AS OF 2.4.0:
		/* (bitwise)
		SCHOOL_NORMAL = 1,
		SCHOOL_HOLY   = 2,
		SCHOOL_FIRE   = 4,
		SCHOOL_NATURE = 8,
		SCHOOL_FROST  = 16,
		SCHOOL_SHADOW = 32,
		SCHOOL_ARCANE = 64
		*/

		for (i=0; i<8; i++)
		{
			if (sp->School & (1<<i))
			{
				sp->School = i;
				break;
			}
		}

		/*
		AURASTATE_FLAG_DODGE_BLOCK			= 1,		//1
		AURASTATE_FLAG_HEALTH20		     = 2,		//2
		AURASTATE_FLAG_BERSERK		      = 4,		//3
		AURASTATE_FLAG_JUDGEMENT		    = 16,       //5
		AURASTATE_FLAG_PARRY				= 64,       //7
		AURASTATE_FLAG_LASTKILLWITHHONOR    = 512,      //10
		AURASTATE_FLAG_CRITICAL		     = 1024,     //11
		AURASTATE_FLAG_HEALTH35		     = 4096,     //13
		AURASTATE_FLAG_IMMOLATE		     = 8192,     //14
		AURASTATE_FLAG_REJUVENATE		   = 16384,    //15 //where do i use this ?
		AURASTATE_FLAG_POISON		       = 32768,    //16
		*/

		// apply on shapeshift change
		if( sp->NameHash == SPELL_HASH_TRACK_HUMANOIDS )
			sp->apply_on_shapeshift_change = true;

		if( sp->NameHash == SPELL_HASH_BLOOD_FURY || sp->NameHash == SPELL_HASH_SHADOWSTEP )
			sp->always_apply = true;

		//there are some spells that change the "damage" value of 1 effect to another : devastate = bonus first then damage
		//this is a total bullshit so remove it when spell system supports effect overwriting
		for( uint32 col1_swap = 0; col1_swap < 2 ; col1_swap++ )
		{
			for( uint32 col2_swap = col1_swap ; col2_swap < 3 ; col2_swap++ )
			{
				if( sp->Effect[col1_swap] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE && sp->Effect[col2_swap] == SPELL_EFFECT_DUMMYMELEE )
				{
					uint32 temp;
					float ftemp;
					temp = sp->Effect[col1_swap];			sp->Effect[col1_swap] = sp->Effect[col2_swap] ;						sp->Effect[col2_swap] = temp;
					temp = sp->EffectDieSides[col1_swap];	sp->EffectDieSides[col1_swap] = sp->EffectDieSides[col2_swap] ;		sp->EffectDieSides[col2_swap] = temp;
					temp = sp->EffectBaseDice[col1_swap];	sp->EffectBaseDice[col1_swap] = sp->EffectBaseDice[col2_swap] ;		sp->EffectBaseDice[col2_swap] = temp;
					ftemp = sp->EffectDicePerLevel[col1_swap];			sp->EffectDicePerLevel[col1_swap] = sp->EffectDicePerLevel[col2_swap] ;				sp->EffectDicePerLevel[col2_swap] = ftemp;
					ftemp = sp->EffectRealPointsPerLevel[col1_swap];	sp->EffectRealPointsPerLevel[col1_swap] = sp->EffectRealPointsPerLevel[col2_swap] ;	sp->EffectRealPointsPerLevel[col2_swap] = ftemp;
					temp = sp->EffectBasePoints[col1_swap];	sp->EffectBasePoints[col1_swap] = sp->EffectBasePoints[col2_swap] ;	sp->EffectBasePoints[col2_swap] = temp;
					temp = sp->EffectMechanic[col1_swap];	sp->EffectMechanic[col1_swap] = sp->EffectMechanic[col2_swap] ;	sp->EffectMechanic[col2_swap] = temp;
					temp = sp->EffectImplicitTargetA[col1_swap];	sp->EffectImplicitTargetA[col1_swap] = sp->EffectImplicitTargetA[col2_swap] ;	sp->EffectImplicitTargetA[col2_swap] = temp;
					temp = sp->EffectImplicitTargetB[col1_swap];	sp->EffectImplicitTargetB[col1_swap] = sp->EffectImplicitTargetB[col2_swap] ;	sp->EffectImplicitTargetB[col2_swap] = temp;
					temp = sp->EffectRadiusIndex[col1_swap];	sp->EffectRadiusIndex[col1_swap] = sp->EffectRadiusIndex[col2_swap] ;	sp->EffectRadiusIndex[col2_swap] = temp;
					temp = sp->EffectApplyAuraName[col1_swap];	sp->EffectApplyAuraName[col1_swap] = sp->EffectApplyAuraName[col2_swap] ;	sp->EffectApplyAuraName[col2_swap] = temp;
					temp = sp->EffectAmplitude[col1_swap];		sp->EffectAmplitude[col1_swap] = sp->EffectAmplitude[col2_swap] ;	sp->EffectAmplitude[col2_swap] = temp;
					ftemp = sp->EffectMultipleValue[col1_swap];		sp->EffectMultipleValue[col1_swap] = sp->EffectMultipleValue[col2_swap] ;	sp->EffectMultipleValue[col2_swap] = ftemp;
					temp = sp->EffectChainTarget[col1_swap];	sp->EffectChainTarget[col1_swap] = sp->EffectChainTarget[col2_swap] ;	sp->EffectChainTarget[col2_swap] = temp;
					temp = sp->EffectSpellClassMask[col1_swap][0];	sp->EffectSpellClassMask[col1_swap][0] = sp->EffectSpellClassMask[col2_swap][0] ;	sp->EffectSpellClassMask[col2_swap][0] = temp;
					temp = sp->EffectSpellClassMask[col1_swap][1];	sp->EffectSpellClassMask[col1_swap][1] = sp->EffectSpellClassMask[col2_swap][1] ;	sp->EffectSpellClassMask[col2_swap][1] = temp;
					temp = sp->EffectSpellClassMask[col1_swap][2];	sp->EffectSpellClassMask[col1_swap][2] = sp->EffectSpellClassMask[col2_swap][2] ;	sp->EffectSpellClassMask[col2_swap][2] = temp;
					temp = sp->EffectMiscValue[col1_swap];		sp->EffectMiscValue[col1_swap] = sp->EffectMiscValue[col2_swap] ;	sp->EffectMiscValue[col2_swap] = temp;
					temp = sp->EffectTriggerSpell[col1_swap];	sp->EffectTriggerSpell[col1_swap] = sp->EffectTriggerSpell[col2_swap] ;	sp->EffectTriggerSpell[col2_swap] = temp;
					ftemp = sp->EffectPointsPerComboPoint[col1_swap];	sp->EffectPointsPerComboPoint[col1_swap] = sp->EffectPointsPerComboPoint[col2_swap] ;	sp->EffectPointsPerComboPoint[col2_swap] = ftemp;
				}
			}
		}
		for(uint32 b=0;b<3;++b)
		{
			if(sp->EffectTriggerSpell[b] != 0 && dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[b]) == NULL)
			{
				/* proc spell referencing non-existant spell. create a dummy spell for use w/ it. */
				CreateDummySpell(sp->EffectTriggerSpell[b]);
			}
			/** Load teaching spells (used for hunters when learning pets wild abilities) */
			if(sp->Effect[b]==SPELL_EFFECT_LEARN_SPELL && sp->EffectImplicitTargetA[b]==EFF_TARGET_PET)
			{
				map<uint32,uint32>::iterator itr = sWorld.TeachingSpellMap.find(sp->EffectTriggerSpell[b]);
				if(itr == sWorld.TeachingSpellMap.end())
					sWorld.TeachingSpellMap.insert(make_pair(sp->EffectTriggerSpell[b],sp->Id));
			}

			if( sp->Attributes & ATTRIBUTES_ONLY_OUTDOORS && sp->EffectApplyAuraName[b] == SPELL_AURA_MOUNTED )
			{
				sp->Attributes &= ~ATTRIBUTES_ONLY_OUTDOORS;
			}
		}

		/*if(sp->self_cast_only && !(sp->Attributes&64))
			printf("SPELL SELF CAST ONLY: %s %u\n", sp->Name, sp->Id);*/

		if(!strcmp(sp->Name, "Hearthstone") || !strcmp(sp->Name, "Stuck") || !strcmp(sp->Name, "Astral Recall"))
			sp->self_cast_only = true;

		sp->proc_interval = 0;//trigger at each event
		sp->ProcsPerMinute = 0;
		sp->c_is_flags = 0;
		sp->spell_coef_flags = 0;
		sp->Dspell_coef_override = 0;
		sp->OTspell_coef_override = 0;
		sp->AP_coef_override = 0;
		sp->RAP_coef_override = 0;
		sp->auraimmune_flag = 0;
		sp->AllowBackAttack = false;

		talentSpellIterator = talentSpells.find(sp->Id);
		if(talentSpellIterator == talentSpells.end())
			sp->talent_tree = 0;
		else
			sp->talent_tree = talentSpellIterator->second;

		skilllinespell *sk = objmgr.GetSpellSkill(sp->Id);
		sp->skilline = sk ? sk->skilline : 0;


		//judgement of command
		if( namehash == SPELL_HASH_JUDGEMENT_OF_COMMAND )		
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;

		//Rogue: Posion time fix for 2.3
		if( strstr( sp->Name, "Crippling Poison") && sp->Effect[0] == 54 ) //I, II
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Mind-numbing Poison") && sp->Effect[0] == 54 ) //I,II,III
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Instant Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V,VI,VII    
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Deadly Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V,VI,VII
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Wound Poison") && sp->Effect[0] == 54 ) //I,II,III,IV,V
			sp->EffectBasePoints[0] = 3599;
		if( strstr( sp->Name, "Anesthetic Poison") && sp->Effect[0] == 54 ) //I
			sp->EffectBasePoints[0] = 3599;

		if( strstr( sp->Name, "Sharpen Blade") && sp->Effect[0] == 54 ) //All BS stones
		    sp->EffectBasePoints[0] = 3599;

		//these mostly do not mix so we can use else 
		// look for seal, etc in name
		if( strstr( sp->Name, "Seal"))
		    type |= SPELL_TYPE_SEAL;
		else if( strstr( sp->Name, "Blessing"))
		    type |= SPELL_TYPE_BLESSING;
		else if( strstr( sp->Name, "Curse"))
		    type |= SPELL_TYPE_CURSE;
		else if( strstr( sp->Name, "Aspect"))
		    type |= SPELL_TYPE_ASPECT;
		else if( strstr( sp->Name, "Sting") || strstr( sp->Name, "sting"))
		    type |= SPELL_TYPE_STING;
        else if( strstr( sp->Name, "Judgement"))
            type |= SPELL_TYPE_JUDGEMENT;
		// don't break armor items!
		else if(strcmp(sp->Name, "Armor") && strstr( sp->Name, "Armor") || strstr( sp->Name, "Demon Skin"))
		    type |= SPELL_TYPE_ARMOR;
        else if( strstr( sp->Name, "Concentration Aura") || strstr( sp->Name, "Crusader Aura") || strstr( sp->Name, "Devotion Aura") || strstr( sp->Name, "Fire Resistance Aura") || strstr( sp->Name, "Frost Resistance Aura") || strstr( sp->Name, "Retribution Aura") || strstr( sp->Name, "Shadow Resistance Aura") )
            type |= SPELL_TYPE_AURA;
		else if( strstr( sp->Name, "Track")==sp->Name)
		    type |= SPELL_TYPE_TRACK;
		else if( namehash == SPELL_HASH_GIFT_OF_THE_WILD || namehash == SPELL_HASH_MARK_OF_THE_WILD )
		    type |= SPELL_TYPE_MARK_GIFT;
		else if( namehash == SPELL_HASH_IMMOLATION_TRAP || namehash == SPELL_HASH_FREEZING_TRAP || namehash == SPELL_HASH_FROST_TRAP || namehash == SPELL_HASH_EXPLOSIVE_TRAP || namehash == SPELL_HASH_SNAKE_TRAP )
		    type |= SPELL_TYPE_HUNTER_TRAP;
		else if( sp->EffectApplyAuraName[0] == SPELL_AURA_MOD_STAT && sp->EffectMiscValue[0] == 3 )
		    type |= SPELL_TYPE_MAGE_INTEL;
		else if( namehash == SPELL_HASH_AMPLIFY_MAGIC || namehash == SPELL_HASH_DAMPEN_MAGIC )
		    type |= SPELL_TYPE_MAGE_MAGI;
		else if( namehash == SPELL_HASH_FIRE_WARD || namehash == SPELL_HASH_FROST_WARD )
		    type |= SPELL_TYPE_MAGE_WARDS;
		else if( namehash == SPELL_HASH_SHADOW_PROTECTION || namehash == SPELL_HASH_PRAYER_OF_SHADOW_PROTECTION )
		    type |= SPELL_TYPE_PRIEST_SH_PPROT;
		else if( namehash == SPELL_HASH_WATER_SHIELD || namehash == SPELL_HASH_EARTH_SHIELD || namehash == SPELL_HASH_LIGHTNING_SHIELD )
		    type |= SPELL_TYPE_SHIELD;
		else if( namehash == SPELL_HASH_POWER_WORD__FORTITUDE || namehash == SPELL_HASH_PRAYER_OF_FORTITUDE )
		    type |= SPELL_TYPE_FORTITUDE;
		else if( namehash == SPELL_HASH_DIVINE_SPIRIT || namehash == SPELL_HASH_PRAYER_OF_SPIRIT )
		    type |= SPELL_TYPE_SPIRIT;
//		else if( strstr( sp->Name, "Curse of Weakness") || strstr( sp->Name, "Curse of Agony") || strstr( sp->Name, "Curse of Recklessness") || strstr( sp->Name, "Curse of Tongues") || strstr( sp->Name, "Curse of the Elements") || strstr( sp->Name, "Curse of Idiocy") || strstr( sp->Name, "Curse of Shadow") || strstr( sp->Name, "Curse of Doom"))
//		else if(namehash==4129426293 || namehash==885131426 || namehash==626036062 || namehash==3551228837 || namehash==2784647472 || namehash==776142553 || namehash==3407058720 || namehash==202747424)
//		else if( strstr( sp->Name, "Curse of "))
//		    type |= SPELL_TYPE_WARLOCK_CURSES;
		else if( strstr( sp->Name, "Immolate") || strstr( sp->Name, "Conflagrate"))
			type |= SPELL_TYPE_WARLOCK_IMMOLATE;
		else if( strstr( sp->Name, "Amplify Magic") || strstr( sp->Name, "Dampen Magic"))
			type |= SPELL_TYPE_MAGE_AMPL_DUMP;
		else if( strstr( sp->Description, "Battle Elixir"))
		    type |= SPELL_TYPE_ELIXIR_BATTLE;
		else if( strstr( sp->Description, "Guardian Elixir"))
		    type |= SPELL_TYPE_ELIXIR_GUARDIAN;
		else if( strstr( sp->Description, "Battle and Guardian elixir"))
		    type |= SPELL_TYPE_ELIXIR_FLASK;
		else if( namehash == SPELL_HASH_HUNTER_S_MARK )		// hunter's mark
			type |= SPELL_TYPE_HUNTER_MARK;
		else if( namehash == SPELL_HASH_COMMANDING_SHOUT || namehash == SPELL_HASH_BATTLE_SHOUT )
		    type |= SPELL_TYPE_WARRIOR_SHOUT;
		else if( ( namehash == SPELL_HASH_FROST_PRESENCE || namehash == SPELL_HASH_BLOOD_PRESENCE || namehash == SPELL_HASH_UNHOLY_PRESENCE ) &&
				 ( sp->Id != 61261 ) && ( sp->Id != 49772 ))
			type |= SPELL_TYPE_DK_PRESENCE;
		else if( strstr( sp->Description, "Finishing move")==sp->Description)
			sp->c_is_flags |= SPELL_FLAG_IS_FINISHING_MOVE;

		for(uint32 b = 0; b < 3; ++b)
		{
			if( sp->Effect[b] == SPELL_EFFECT_PERSISTENT_AREA_AURA )
			{
				/*if( sp->EffectImplicitTargetB[b] != 0 )
				printf("%s %s - A: %u, B: %u\n", sp->Name, sp->Rank, sp->EffectImplicitTargetA[b], sp->EffectImplicitTargetB[b]);*/

				sp->EffectImplicitTargetA[b] = EFF_TARGET_SELF;
				sp->EffectImplicitTargetB[b] = 0;
			}

			// 3.0.3 totemzzz
			if( sp->Effect[b] == SPELL_EFFECT_HEALTH_FUNNEL )
			{
				sp->Effect[b] = SPELL_EFFECT_APPLY_AREA_AURA;
			}
		}

		// parse rank text
		if( !sscanf( sp->Rank, "Rank %d", (unsigned int*)&sp->RankNumber) )
		{
			const char* ranktext = sp->Rank;
			uint32 rank = 0;

			//stupid spell ranking problem
			if( strstr( ranktext, "Apprentice"))
				rank = 1;
			else if( strstr( ranktext, "Journeyman"))
				rank = 2;
			else if( strstr( ranktext, "Expert"))
				rank = 3;
			else if( strstr( ranktext, "Artisan"))
				rank = 4;
			else if( strstr( ranktext, "Master"))
				rank = 5;
			else if( strstr( ranktext, "Grandmaster"))
				rank = 6;
			sp->RankNumber = rank;
		}

		if(sp->spellLevel==0)
		{
			uint32 new_level=sp->RankNumber;
			if(new_level!=0)
			{
				uint32 teachspell=0;
				if(sp->Effect[0]==SPELL_EFFECT_LEARN_SPELL)
					teachspell = sp->EffectTriggerSpell[0];
				else if(sp->Effect[1]==SPELL_EFFECT_LEARN_SPELL)
					teachspell = sp->EffectTriggerSpell[1];
				else if(sp->Effect[2]==SPELL_EFFECT_LEARN_SPELL)
					teachspell = sp->EffectTriggerSpell[2];
				if(teachspell)
				{
					SpellEntry *spellInfo;
					spellInfo = dbcSpell.LookupEntryForced(teachspell);
					spellInfo->spellLevel = new_level;
					sp->spellLevel = new_level;
				}
			}
		}

		/*FILE * f = fopen("C:\\spells.txt", "a");
		fprintf(f, "case 0x%08X:		// %s\n", namehash, sp->Name);
		fclose(f);*/

		// find diminishing status
		sp->DiminishStatus = GetDiminishingGroup(namehash);
		sp->buffIndexType=0;
		switch(namehash)
		{
		case SPELL_HASH_HUNTER_S_MARK:		// Hunter's mark
			sp->buffIndexType = SPELL_TYPE_INDEX_MARK;
			break;

		case SPELL_HASH_POLYMORPH:			// Polymorph
		case SPELL_HASH_POLYMORPH__CHICKEN:	// Polymorph: Chicken
		case SPELL_HASH_POLYMORPH__SHEEP:	// Polymorph: Sheep
			sp->buffIndexType = SPELL_TYPE_INDEX_POLYMORPH;
			break;

		case SPELL_HASH_FEAR:				// Fear
			sp->buffIndexType = SPELL_TYPE_INDEX_FEAR;
			break;

		case SPELL_HASH_SAP:				// Sap
			sp->buffIndexType = SPELL_TYPE_INDEX_SAP;
			break;

		case SPELL_HASH_SCARE_BEAST:		// Scare Beast
			sp->buffIndexType = SPELL_TYPE_INDEX_SCARE_BEAST;
			break;

		case SPELL_HASH_HIBERNATE:			// Hibernate
			sp->buffIndexType = SPELL_TYPE_INDEX_HIBERNATE;
			break;

		case SPELL_HASH_EARTH_SHIELD:		// Earth Shield
			sp->buffIndexType = SPELL_TYPE_INDEX_EARTH_SHIELD;
			break;

		case SPELL_HASH_CYCLONE:			// Cyclone
			sp->buffIndexType = SPELL_TYPE_INDEX_CYCLONE;
			break;

		case SPELL_HASH_BANISH:				// Banish
			sp->buffIndexType = SPELL_TYPE_INDEX_BANISH;
			break;

		//case SPELL_HASH_JUDGEMENT_OF_VENGEANCE:
		case SPELL_HASH_JUDGEMENT_OF_LIGHT:
		case SPELL_HASH_JUDGEMENT_OF_WISDOM:
		case SPELL_HASH_JUDGEMENT_OF_JUSTICE:
			sp->buffIndexType = SPELL_TYPE_INDEX_JUDGEMENT;
			break;

		case SPELL_HASH_REPENTANCE:
			sp->buffIndexType = SPELL_TYPE_INDEX_REPENTANCE;
			break;
		case SPELL_HASH_SLOW:
			sp->buffIndexType = SPELL_TYPE_INDEX_SLOW;
			break;
		}

		// HACK FIX: Break roots/fear on damage.. this needs to be fixed properly!
		uint32 z;
		if(!(sp->AuraInterruptFlags & AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN))
		{
			for(z = 0; z < 3; ++z) {
				if(sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_FEAR ||
					sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_ROOT)
				{
					sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_UNUSED2;
					break;
				}
			}
		}

		for(z = 0;z < 3; ++z)
		{
			if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_MELEE ) || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE || sp->Effect[z] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE || sp->Effect[z] == SPELL_EFFECT_DUMMYMELEE )
				sp->is_melee_spell = true;
			if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->Spell_Dmg_Type == SPELL_DMG_TYPE_RANGED ) )
			{
				//sLog.outString( "Ranged Spell: %u [%s]" , sp->Id , sp->Name );
				sp->is_ranged_spell = true;
			}

			switch(sp->EffectImplicitTargetA[z])
			{
				case 6:
				case 22:
				case 24:
				case 25:
					sp->c_is_flags |= SPELL_FLAG_CAN_BE_REFLECTED;
					break;
			}

			if( sp->Effect[z] == SPELL_EFFECT_DISPEL )
				sp->c_is_flags |= SPELL_FLAG_IS_DISPEL_SPELL;

			if( sp->Effect[z] == SPELL_EFFECT_HEAL )
				sp->c_is_flags |= SPELL_FLAG_IS_HEALING_SPELL;

			if( sp->Effect[z] == SPELL_EFFECT_ENERGIZE )
				sp->c_is_flags |= SPELL_FLAG_IS_HEALING_MANA_SPELL;

		}

		// set extra properties
		sp->buffType   = type;

		uint32 pr=sp->procFlags;
		for(uint32 y=0;y < 3; y++)
		{
			// get the effect number from the spell
			effect = sp->Effect[y];

			if(effect==SPELL_EFFECT_APPLY_AURA)
			{
				uint32 aura = sp->EffectApplyAuraName[y];
				if( aura == SPELL_AURA_PROC_TRIGGER_SPELL ||
					aura == SPELL_AURA_PROC_TRIGGER_DAMAGE
					)//search for spellid in description
				{
					const char *p=sp->Description;
					while((p=strstr(p,"$")))
					{
						p++;
						//got $  -> check if spell
						if(*p>='0' && *p <='9')
						{//woot this is spell id
						
							result=atoi(p);
						}					
					}
					pr=0;

					char desc[4096];
					strcpy(desc,sp->Description);
					uint32 len = (uint32)strlen(desc);
					for(i = 0; i < len; ++i)
						desc[i] = tolower(desc[i]);
					//dirty code for procs, if any1 got any better idea-> u are welcome
					//139944 --- some magic number, it will trigger on all hits etc
						//for seems to be smth like custom check
					if( strstr( desc,"your ranged criticals"))
						pr|=PROC_ON_RANGED_CRIT_ATTACK;
					if( strstr( desc,"chance on hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"takes damage"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"attackers when hit"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"character strikes an enemy"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"strike you with a melee attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"target casts a spell"))
						pr|=PROC_ON_CAST_SPELL;
				    if( strstr( desc,"your harmful spells land"))
						pr|=PROC_ON_CAST_SPELL;
				    if( strstr( desc,"on spell critical hit"))
						pr|=PROC_ON_SPELL_CRIT_HIT;
				    if( strstr( desc,"spell critical strikes"))
						pr|=PROC_ON_SPELL_CRIT_HIT;
				    if( strstr( desc,"being able to resurrect"))
						pr|=PROC_ON_DIE;
					if( strstr( desc,"any damage caused"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"the next melee attack against the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"when successfully hit"))
						pr|=PROC_ON_MELEE_ATTACK ;
					if( strstr( desc,"an enemy on hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"when it hits"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"when successfully hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"on a successful hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"damage to attacker on hit"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"on a hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"strikes you with a melee attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"when caster takes damage"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"when the caster is using melee attacks"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"when struck in combat") || strstr(desc,"When struck in combat"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"successful melee attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"chance per attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"chance per hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"that strikes a party member"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"when hit by a melee attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"landing a melee critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( desc,"your critical strikes"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( desc,"whenever you deal ranged damage"))
						pr|=PROC_ON_RANGED_ATTACK;
//					if( strstr( desc,"whenever you deal melee damage"))
					if( strstr( desc,"you deal melee damage"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"your melee attacks"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"damage with your Sword"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"when struck in melee combat"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"any successful spell cast against the priest"))
						pr|=PROC_ON_SPELL_HIT_VICTIM;
					if( strstr( desc,"the next melee attack on the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"striking melee or ranged attackers"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM|PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( desc,"when damaging an enemy in melee"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"victim of a critical strike"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
					if( strstr( desc,"on successful melee or ranged attack"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"enemy that strikes you in melee"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"after getting a critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( desc,"whenever damage is dealt to you"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"when ranged or melee damage is dealt"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"damaging melee attacks"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"on melee or ranged attack"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"on a melee swing"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"Chance on melee"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"spell criticals against you"))
						pr|=PROC_ON_SPELL_CRIT_HIT_VICTIM;
					if( strstr( desc,"after being struck by a melee or ranged critical hit"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
//					if( strstr( desc,"on a critical hit"))
					if( strstr( desc,"critical hit"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( desc,"strikes the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"a spell, melee or ranged attack hits the caster"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"after dealing a critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( desc,"each melee or ranged damage hit against the priest"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM|PROC_ON_RANGED_ATTACK_VICTIM;				
					if( strstr( desc, "a chance to deal additional"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc, "chance to get an extra attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc, "melee attacks has"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc, "any damage spell hits a target"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc, "giving each melee attack a chance"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc, "damage when hit"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM; //myabe melee damage ?
					if( strstr( desc, "gives your"))
					{
						if( strstr( desc, "melee"))
							pr|=PROC_ON_MELEE_ATTACK;
						else if( strstr( desc,"sinister strike, backstab, gouge and shiv"))
							pr|=PROC_ON_CAST_SPELL;
						else if( strstr( desc,"chance to daze the target"))
							pr|=PROC_ON_CAST_SPELL;
						else if( strstr( desc,"finishing moves"))
							pr|=PROC_ON_CAST_SPELL;
//						else if( strstr( desc,"shadow bolt, shadowburn, soul fire, incinerate, searing pain and conflagrate"))
//							pr|=PROC_ON_CAST_SPELL|PROC_TARGET_SELF;
						//we should find that specific spell (or group) on what we will trigger
						else pr|=PROC_ON_CAST_SPECIFIC_SPELL;
					}
					if( strstr( desc, "chance to add an additional combo") && strstr(desc, "critical") )
						pr|=PROC_ON_CRIT_ATTACK;
					else if( strstr( desc, "chance to add an additional combo"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc, "victim of a melee or ranged critical strike"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
					if( strstr( desc, "getting a critical effect from"))
						pr|=PROC_ON_SPELL_CRIT_HIT_VICTIM;
					if( strstr( desc, "damaging attack is taken"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc, "struck by a Stun or Immobilize"))
						pr|=PROC_ON_SPELL_HIT_VICTIM;
					if( strstr( desc, "melee critical strike"))
						pr|=PROC_ON_CRIT_ATTACK;
					if( strstr( sp->Name, "Bloodthirst"))
						pr|=PROC_ON_MELEE_ATTACK | PROC_TARGET_SELF;
					if( strstr( desc, "experience or honor"))
						pr|=PROC_ON_GAIN_EXPIERIENCE;
					if( strstr( desc,"your next offensive ability"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"hit by a melee or ranged attack"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( desc,"enemy strikes the caster"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"melee and ranged attacks against you"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( desc,"when a block occurs"))
						pr|=PROC_ON_BLOCK_VICTIM;
					if( strstr( desc,"dealing a critical strike from a weapon swing, spell, or ability"))
						pr|=PROC_ON_CRIT_ATTACK|PROC_ON_SPELL_CRIT_HIT;
					if( strstr( desc,"dealing a critical strike from a weapon swing, spell, or ability"))
						pr|=PROC_ON_CRIT_ATTACK|PROC_ON_SPELL_CRIT_HIT;
					if( strstr( desc,"shadow bolt critical strikes increase shadow damage"))
						pr|=PROC_ON_SPELL_CRIT_HIT;
					if( strstr( desc,"next offensive ability"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"after being hit with a shadow or fire spell"))
						pr|=PROC_ON_SPELL_LAND_VICTIM;
					if( strstr( desc,"giving each melee attack"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"each strike has"))
						pr|=PROC_ON_MELEE_ATTACK;		
					if( strstr( desc,"your Fire damage spell hits"))
						pr|=PROC_ON_CAST_SPELL;		//this happens only on hit ;)
					if( strstr( desc,"corruption, curse of agony, siphon life and seed of corruption spells also cause"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"pain, mind flay and vampiric touch spells also cause"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"shadow damage spells have"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"on successful spellcast"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"your spell criticals have"))
						pr|=PROC_ON_SPELL_CRIT_HIT | PROC_ON_SPELL_CRIT_HIT_VICTIM;
					if( strstr( desc,"after dodging their attack"))
					{
						pr|=PROC_ON_DODGE_VICTIM;
						if( strstr( desc,"add a combo point"))
							pr|=PROC_TARGET_SELF;
					}
					if( strstr( desc,"fully resisting"))
						pr|=PROC_ON_FULL_RESIST;
					if( strstr( desc,"fully resist"))
						pr|=PROC_ON_FULL_RESIST;
					if( strstr( desc,"Your Shadow Word: Pain, Mind Flay and Vampiric Touch spells also cause the target"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"chance on spell hit"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"your spell casts have"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"chance on spell cast"))
						pr|=PROC_ON_CAST_SPELL;
					if( strstr( desc,"your melee and ranged attacks"))
						pr|=PROC_ON_MELEE_ATTACK|PROC_ON_RANGED_ATTACK;
//					if( strstr( desc,"chill effect to your Blizzard"))
//						pr|=PROC_ON_CAST_SPELL;	
					//////////////////////////////////////////////////
					//proc dmg flags
					//////////////////////////////////////////////////
					if( strstr( desc,"each attack blocked"))
						pr|=PROC_ON_BLOCK_VICTIM;
					if( strstr( desc,"into flame, causing an additional"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"victim of a critical melee strike"))
						pr|=PROC_ON_CRIT_HIT_VICTIM;
					if( strstr( desc,"damage to melee attackers"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"target blocks a melee attack"))
						pr|=PROC_ON_BLOCK_VICTIM;
					if( strstr( desc,"ranged and melee attacks to deal"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( desc,"damage on hit"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"chance on hit"))
						pr|=PROC_ON_MELEE_ATTACK;
					if( strstr( desc,"after being hit by any damaging attack"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"striking melee or ranged attackers"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;
					if( strstr( desc,"damage to attackers when hit"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"striking melee attackers"))
						pr|=PROC_ON_MELEE_ATTACK_VICTIM;
					if( strstr( desc,"whenever the caster takes damage"))
						pr|=PROC_ON_ANY_DAMAGE_VICTIM;
					if( strstr( desc,"damage on every attack"))
						pr|=PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"chance to reflect Fire spells"))
						pr|=PROC_ON_SPELL_HIT_VICTIM;
					if( strstr( desc,"hunter takes on the aspects of a hawk"))
						pr|=PROC_TARGET_SELF | PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"hunter takes on the aspects of a dragonhawk"))
						pr|=PROC_TARGET_SELF | PROC_ON_RANGED_ATTACK;
					if( strstr( desc,"successful auto shot attacks"))
						pr|=PROC_ON_AUTO_SHOT_HIT;
					if( strstr( desc,"after getting a critical effect from your"))
						pr=PROC_ON_SPELL_CRIT_HIT;
					if( strstr( desc, "with a melee weapon"))
						pr|=PROC_ON_MELEE_ATTACK;
//					if( strstr( desc,"Your critical strikes from Fire damage"))
//						pr|=PROC_ON_SPELL_CRIT_HIT;
				}//end "if procspellaura"
				//dirty fix to remove auras that should expire on event and they are not
//				else if(sp->procCharges>0)
//				{
					//there are at least 185 spells that should loose charge uppon some event.Be prepared to add more here !
					// ! watch it cause this might conflict with our custom modified spells like : lighning shield !

					//spells like : Presence of Mind,Nature's Swiftness, Inner Focus,Amplify Curse,Coup de Grace
					//SELECT * FROM dbc_spell where proc_charges!=0 and (effect_aura_1=108 or effect_aura_2=108 and effect_aura_3=108) and description!=""
//					if(aura == SPELL_AURA_ADD_PCT_MODIFIER)
//						sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_CAST_SPELL;
					//most of them probably already have these flags...not sure if we should add to all of them without checking
/*					if( strstr( desc, "melee"))
						sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_START_ATTACK;
					if( strstr( desc, "ranged"))
						sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_START_ATTACK;*/
//				}

				// Aura 109 fix
				if(sp->EffectApplyAuraName[y] == SPELL_AURA_ADD_TARGET_TRIGGER)
				{
					sp->EffectApplyAuraName[y] = SPELL_AURA_PROC_TRIGGER_SPELL;
					pr = PROC_ON_CAST_SPELL;
				}
			}//end "if aura"
		}//end "for each effect"
		sp->procFlags = pr;

		if( strstr( sp->Description, "Must remain seated"))
		{
			sp->RecoveryTime = 1000;
			sp->CategoryRecoveryTime = 1000;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// procintervals
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//lightning shield trigger spell id's are all wrong ?
		//if you are bored you could make these by hand but i guess we might find other spells with this problem..and this way it's safe
		if( strstr( sp->Name, "Lightning Shield" ) && sp->EffectTriggerSpell[0] )
		{
			//check if we can find in the description
			if( strstr(sp->Description, "for $") )
			{
				sp->EffectTriggerSpell[0] = GetTriggerSpellFromDescription("for $", sp->Description);

				SpellEntry * triggerSpell = dbcSpell.LookupEntryForced( sp->EffectTriggerSpell[0] );
				if(triggerSpell)
					triggerSpell->Dspell_coef_override = 0.3f;
			}
			sp->proc_interval = 3000; //few seconds
		}
		//mage ignite talent should proc only on some chances
		else if( strstr( sp->Name, "Ignite") && sp->Id>=11119 && sp->Id<=12848 && sp->EffectApplyAuraName[0] == 4 )
		{
			//check if we can find in the description
			char *startofid=strstr(sp->Description, "an additional ");
			if(startofid)
			{
				startofid += strlen("an additional ");
				sp->EffectBasePoints[0]=atoi(startofid); //get new value. This is actually level*8 ;)
				if( strstr(sp->Description, "over $") )
					sp->EffectTriggerSpell[0] = GetTriggerSpellFromDescription("over $", sp->Description);
			}
			sp->Effect[0] = 6; //aura
			sp->EffectApplyAuraName[0] = 42; //force him to use procspell effect
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT; //add procflag here since this was not processed with the others !
		}
		// Winter's Chill handled by frost school
		else if( strstr( sp->Name, "Winter's Chill"))
		{
			sp->School = 4;
		}

		// Blackout handled by Shadow school
		else if( strstr( sp->Name, "Blackout"))
		{
			sp->School = 5;
		}

		//this starts to be an issue for trigger spell id : Deep Wounds
		else if( strstr( sp->Name, "Deep Wounds") && sp->EffectTriggerSpell[0])
		{
			if( strstr(sp->Description, "over $") )
				sp->EffectTriggerSpell[0] = GetTriggerSpellFromDescription("over $", sp->Description);
		}
		else if( strstr( sp->Name, "Holy Shock"))
		{
			//check if we can find in the description
			if( strstr(sp->Description, "causing $") )
				sp->EffectTriggerSpell[0] = GetTriggerSpellFromDescription("causing $", sp->Description);

			//check if we can find in the description
			if( strstr(sp->Description, " or $") )
				sp->EffectTriggerSpell[1] = GetTriggerSpellFromDescription(" or $", sp->Description);
		}
		else if( strstr( sp->Name, "Touch of Weakness"))
		{
			//check if we can find in the description
			if( strstr(sp->Description, "cause $") )
			{
				sp->EffectTriggerSpell[0] = GetTriggerSpellFromDescription("cause $", sp->Description);
				sp->EffectTriggerSpell[1]=sp->EffectTriggerSpell[0]; //later versions of this spell changed to eff[1] the aura
				sp->procFlags = uint32(PROC_ON_MELEE_ATTACK_VICTIM);
			}
		}
		else if( strstr( sp->Name, "Firestone Passive"))
		{
			//Enchants the main hand weapon with fire, granting each attack a chance to deal $17809s1 additional fire damage.
			//check if we can find in the description
			if( strstr(sp->Description, "to deal $") )
			{
				sp->EffectTriggerSpell[0] = GetTriggerSpellFromDescription("to deal $", sp->Description);
				sp->EffectApplyAuraName[0] = 42;
				sp->procFlags = PROC_ON_MELEE_ATTACK;
				sp->procChance = 50;
			}
		}

		//some procs trigger at intervals
		else if(sp->NameHash == SPELL_HASH_WATER_SHIELD)
		{
			sp->proc_interval = 3000; //few seconds
			sp->procFlags |= PROC_TARGET_SELF;
		}
		else if(sp->NameHash == SPELL_HASH_EARTH_SHIELD)
			sp->proc_interval = 3000; //few seconds
		else if( strstr( sp->Name, "Shadowguard"))
			sp->proc_interval = 3000; //few seconds
		else if( strstr( sp->Name, "Poison Shield"))
			sp->proc_interval = 3000; //few seconds
		else if( strstr( sp->Name, "Infused Mushroom"))
			sp->proc_interval = 10000; //10 seconds
		else if( strstr( sp->Name, "Aviana's Purpose"))
			sp->proc_interval = 10000; //10 seconds
		//don't change to namehash since we are searching only a protion of the name
 		else if( strstr( sp->Name, "Crippling Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
			sp->poison_type = POISON_TYPE_CRIPPLING;
		}
		else if( strstr( sp->Name, "Mind-Numbing Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
			sp->poison_type = POISON_TYPE_MIND_NUMBING;
		}
		else if( strstr( sp->Name, "Instant Poison"))
		{
			sp->poison_type = POISON_TYPE_INSTANT;
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
		}
		else if( strstr( sp->Name, "Scorpid Poison") )
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
			sp->poison_type = POISON_TYPE_SCORPID;
		}
		else if( strstr( sp->Name, "Deadly Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
			sp->poison_type = POISON_TYPE_DEADLY;
		}
		else if( strstr( sp->Name, "Wound Poison"))
		{
			sp->c_is_flags |= SPELL_FLAG_IS_POISON;
			sp->poison_type = POISON_TYPE_WOUND;
		}

		if( strstr( sp->Description, "cannot be resisted" ) )
			sp->c_is_flags |= SPELL_FLAG_IS_NOT_RESISTABLE;

		if( sp->School == SCHOOL_HOLY && !(sp->c_is_flags & SPELL_FLAG_IS_NOT_RESISTABLE) )
			sp->c_is_flags |= SPELL_FLAG_IS_NOT_RESISTABLE;

		if( strstr( sp->Description, "pierces through all absorption effects" ) )
			sp->c_is_flags |= SPELL_FLAG_PIERCES_ABSORBTION_EFF;

		//Set Silencing spells mech.
				// Set default mechanics if we don't already have one
		if( !sp->MechanicsType )
		{
		    //Set Silencing spells mechanic.
		    if( sp->EffectApplyAuraName[0] == 27 ||
				sp->EffectApplyAuraName[1] == 27 ||
				sp->EffectApplyAuraName[2] == 27 )
				sp->MechanicsType = MECHANIC_SILENCED;

		    //Set Stunning spells mechanic.
		    if( sp->EffectApplyAuraName[0] == 12 ||
				sp->EffectApplyAuraName[1] == 12 ||
				sp->EffectApplyAuraName[2] == 12 )
				sp->MechanicsType = MECHANIC_STUNNED;

		    //Set Fearing spells mechanic
		    if( sp->EffectApplyAuraName[0] == 7 ||
				sp->EffectApplyAuraName[1] == 7 ||
				sp->EffectApplyAuraName[2] == 7 )
				sp->MechanicsType = MECHANIC_FLEEING;
		}

		// Sap, Gouge, Blehhhh
		if( sp->NameHash == SPELL_HASH_GOUGE ||
			sp->NameHash == SPELL_HASH_SAP ||
			sp->NameHash == SPELL_HASH_REPENTANCE ||
			sp->NameHash == SPELL_HASH_MAIM ||
			sp->NameHash == SPELL_HASH_GOBLIN_ROCKET_HELMET ||
			sp->NameHash == SPELL_HASH_RECKLESS_CHARGE)
			sp->MechanicsType = MECHANIC_INCAPACIPATED;

		if( sp->proc_interval != 0 )
			sp->procFlags |= PROC_REMOVEONUSE;

		//Seal of Jusice - Proc Chance
		if( sp->NameHash == SPELL_HASH_SEAL_OF_JUSTICE )
			sp->procChance = 25;

		/* Decapitate */
		if( sp->NameHash == SPELL_HASH_DECAPITATE )
			sp->procChance = 30;

		if( sp->NameHash == SPELL_HASH_DIVINE_SHIELD || sp->NameHash == SPELL_HASH_DIVINE_PROTECTION || sp->NameHash == SPELL_HASH_BLESSING_OF_PROTECTION || sp->NameHash == SPELL_HASH_HAND_OF_PROTECTION )
			sp->MechanicsType = 25;

		if(sp->NameHash == SPELL_HASH_DRINK && sp->EffectBasePoints[0] == -1 &&
			sp->EffectApplyAuraName[1] == 226 && sp->EffectBasePoints[1] > 0)
		{
			sp->EffectBasePoints[0] = sp->EffectBasePoints[1];
			sp->Effect[1] = SPELL_EFFECT_NULL;
		}

		if(
			((sp->Attributes & ATTRIBUTES_TRIGGER_COOLDOWN) && (sp->AttributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH)) //rogue cold blood
			|| ((sp->Attributes & ATTRIBUTES_TRIGGER_COOLDOWN) && (!sp->AttributesEx || sp->AttributesEx & ATTRIBUTESEX_REMAIN_OOC))
			)
		{
			sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
		}

		/////////////////////////////////////////////////////////////////
		//SPELL COEFFICIENT SETTINGS START
		//////////////////////////////////////////////////////////////////

		//Flag for DoT and HoT
		bool spcheck = false;
		SpellEntry *spz;

		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if (sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_DAMAGE ||
				sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_HEAL ||
				sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_LEECH )
			{
				sp->spell_coef_flags |= SPELL_FLAG_IS_DOT_OR_HOT_SPELL;
				break;
			}
		}

		//Flag for DD or DH
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if ( sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_TRIGGER_SPELL && sp->EffectTriggerSpell[i] )
			{
				spz = dbcSpell.LookupEntry( sp->EffectTriggerSpell[i] );
				if( spz &&
					spz->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE ||
					spz->Effect[i] == SPELL_EFFECT_HEAL
					) 
					spcheck = true;
			}
			if (sp->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE ||
				sp->Effect[i] == SPELL_EFFECT_HEAL ||
				spcheck
				)
			{
				sp->spell_coef_flags |= SPELL_FLAG_IS_DD_OR_DH_SPELL;
				break;
			}
		}

		for(uint8 i = 0 ; i < 3; i++)
		{
			switch (sp->EffectImplicitTargetA[i])
			{
				//AoE
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS:
			case EFF_TARGET_ALL_ENEMY_IN_AREA:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT:
			case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
			case EFF_TARGET_IN_FRONT_OF_CASTER:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_FRIENDLY_IN_AREA:
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME:
			case EFF_TARGET_ALL_PARTY:
			case EFF_TARGET_LOCATION_INFRONT_CASTER:
			case EFF_TARGET_BEHIND_TARGET_LOCATION:
			case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE:
				{
					sp->spell_coef_flags |= SPELL_FLAG_AOE_SPELL;
					break;
				}
			}	
		}

		for(uint8 i = 0 ; i < 3 ; i++)
		{
			switch (sp->EffectImplicitTargetB[i])
			{
				//AoE
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS:
			case EFF_TARGET_ALL_ENEMY_IN_AREA:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT:
			case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
			case EFF_TARGET_IN_FRONT_OF_CASTER:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_FRIENDLY_IN_AREA:
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME:
			case EFF_TARGET_ALL_PARTY:
			case EFF_TARGET_LOCATION_INFRONT_CASTER:
			case EFF_TARGET_BEHIND_TARGET_LOCATION:
			case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE:
				{
					sp->spell_coef_flags |= SPELL_FLAG_AOE_SPELL;
					break;
				}
			}	
			if(sp->Effect[i] == 0)
				continue;

			//Apply aura fixes
			if( sp->Effect[i] == SPELL_EFFECT_APPLY_AURA )
			{
				switch( sp->EffectApplyAuraName[i] )
				{
					case SPELL_AURA_MOD_CONFUSE: 
						sp->auraimmune_flag |= AURAIMMUNE_CONFUSE;break;
					case SPELL_AURA_MOD_CHARM: 
						sp->auraimmune_flag |= AURAIMMUNE_CHARM;break;
					case SPELL_AURA_MOD_FEAR: 
						sp->auraimmune_flag |= AURAIMMUNE_FEAR;break;
					case SPELL_AURA_MOD_STUN: 
						sp->auraimmune_flag |= AURAIMMUNE_STUN;break;
					case SPELL_AURA_MOD_PACIFY: 
						sp->auraimmune_flag |= AURAIMMUNE_PACIFY;break;
					case SPELL_AURA_MOD_ROOT: 
						sp->auraimmune_flag |= AURAIMMUNE_ROOT;break;
					case SPELL_AURA_MOD_SILENCE: 
						sp->auraimmune_flag |= AURAIMMUNE_SILENCE;break;
					case SPELL_AURA_MOD_INCREASE_SPEED: 
						sp->auraimmune_flag |= AURAIMMUNE_INCSPEED;break;
					case SPELL_AURA_MOD_DECREASE_SPEED: 
						sp->auraimmune_flag |= AURAIMMUNE_DECSPEED;break;
					case SPELL_AURA_TRANSFORM: 
						sp->auraimmune_flag |= AURAIMMUNE_TRANSFORM;break;
					case SPELL_AURA_MOD_TAUNT: 
						sp->auraimmune_flag |= AURAIMMUNE_TAUNT;break;
				}
			}
		}

		//Special Cases
		//Holy Light & Flash of Light
		if(sp->NameHash == SPELL_HASH_HOLY_LIGHT ||
			sp->NameHash == SPELL_HASH_FLASH_OF_LIGHT)
			sp->spell_coef_flags |= SPELL_FLAG_IS_DD_OR_DH_SPELL;

		if(sp->NameHash == SPELL_HASH_FIREBALL)
			sp->spell_coef_flags = SPELL_FLAG_IS_DD_OR_DH_SPELL;

		//Additional Effect (not healing or damaging)
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if(sp->Effect[i] == 0)
				continue;

			switch (sp->Effect[i])
			{
			case SPELL_EFFECT_SCHOOL_DAMAGE:
			case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
			case SPELL_EFFECT_HEALTH_LEECH:
			case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
			case SPELL_EFFECT_ADD_EXTRA_ATTACKS:
			case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
			case SPELL_EFFECT_POWER_BURN:
			case SPELL_EFFECT_ATTACK:
			case SPELL_EFFECT_HEAL:
			case SPELL_EFFECT_HEALTH_FUNNEL:
			case SPELL_EFFECT_HEAL_MAX_HEALTH:
			case SPELL_EFFECT_DUMMY:
				continue;
			}

			switch (sp->EffectApplyAuraName[i])
			{
			case SPELL_AURA_PERIODIC_DAMAGE:
			case SPELL_AURA_PROC_TRIGGER_DAMAGE:
			case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
			case SPELL_AURA_POWER_BURN:
			case SPELL_AURA_PERIODIC_HEAL:
			case SPELL_AURA_MOD_INCREASE_HEALTH:
			case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
			case SPELL_AURA_DUMMY:
				continue;
			}

			sp->spell_coef_flags |= SPELL_FLAG_ADITIONAL_EFFECT;
			break;

		}

		///	SPELLS CAN CRIT ///
		sp->spell_can_crit = true; // - except in special cases noted in this section

		//////////////////////////////////////////////////////
		// CLASS-SPECIFIC SPELL FIXES						//
		//////////////////////////////////////////////////////

		/* Note: when applying spell hackfixes, please follow a template */

		//////////////////////////////////////////
		// WARRIOR								//
		//////////////////////////////////////////

		// Insert warrior spell fixes here
		if( sp->NameHash == SPELL_HASH_MORTAL_STRIKE)
			sp->maxstack = 1; // Healing reduction shouldn't stack

		if( sp->NameHash == SPELL_HASH_TRAUMA )
			sp->procFlags = PROC_ON_CRIT_ATTACK;

		if( sp->NameHash == SPELL_HASH_HOLY_SHIELD )
			sp->procFlags = PROC_ON_BLOCK_VICTIM;

		if( sp->NameHash == SPELL_HASH_SLAM )
			sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;

		//////////////////////////////////////////
		// PALADIN								//
		//////////////////////////////////////////

		// Insert paladin spell fixes here
		if( sp->NameHash == SPELL_HASH_JUDGEMENT_OF_WISDOM )
			sp->maxstack = 1;
		if( sp->NameHash == SPELL_HASH_JUDGEMENT_OF_LIGHT )
			sp->maxstack = 1;
		if( sp->NameHash == SPELL_HASH_JUDGEMENT_OF_JUSTICE )
			sp->maxstack = 1;
		if( sp->NameHash == SPELL_HASH_HEART_OF_THE_CRUSADER )
			sp->maxstack = 1;
			
		// Seal of Righteousness - can crit
		if( sp->NameHash == SPELL_HASH_SEAL_OF_RIGHTEOUSNESS )
			sp->spell_can_crit = true;

		//Seal of Light
		if( sp->NameHash == SPELL_HASH_SEAL_OF_LIGHT)
		{
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
			sp->ProcsPerMinute = 10;	/* this will do */
		}

		if( sp->NameHash == SPELL_HASH_SEALS_OF_THE_PURE )
		{
			sp->EffectSpellClassMask[0][0] = 8389632 | 4194312 | 512;
			sp->EffectSpellClassMask[0][1] = 134217728 | 536870912;
			sp->EffectSpellClassMask[0][2] = 0;
			sp->EffectSpellClassMask[1][0] = 2048;
			sp->EffectSpellClassMask[1][1] = 0;
			sp->EffectSpellClassMask[1][2] = 0;
		}

		if( sp->NameHash == SPELL_HASH_JUDGEMENTS_OF_THE_PURE && sp->Id != 54153 && sp->Id != 53655 && sp->Id != 53656 && sp->Id != 53657 && sp->Id != 54152 )
		{
			sp->EffectSpellClassMask[0][0] = 8388608;
			sp->EffectSpellClassMask[0][1] = 0;
			sp->EffectSpellClassMask[0][2] = 0;
			sp->EffectSpellClassMask[1][0] = 8389632 | 4194312 | 512;
			sp->EffectSpellClassMask[1][1] = 134217728 | 536870912 | 33554432;
			sp->EffectSpellClassMask[1][2] = 0;
			sp->EffectSpellClassMask[2][0] = 2048;
			sp->EffectSpellClassMask[2][1] = 0;
			sp->EffectSpellClassMask[2][2] = 0;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}

		if( sp->NameHash == SPELL_HASH_IMPROVED_DEVOTION_AURA )
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HEALING_PCT;
			sp->EffectBasePoints[1] = 6;
			sp->EffectMiscValue[1] = 127;
		}

		if( sp->NameHash == SPELL_HASH_AVENGER_S_SHIELD )
		{
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
		}

		if( sp->NameHash == SPELL_HASH_SHIELD_OF_RIGHTEOUSNESS )
		{
			sp->EffectChainTarget[0] = 0;
		}

		//////////////////////////////////////////
		// HUNTER								//
		//////////////////////////////////////////

		// Insert hunter spell fixes here
				
		if( sp->EquippedItemClass == 2 && sp->EquippedItemSubClass & 262156 ) // 4 + 8 + 262144 ( becomes item classes 2, 3 and 18 which correspond to bow, gun and crossbow respectively)
			sp->is_ranged_spell = true;

		if( sp->NameHash == SPELL_HASH_AIMED_SHOT)
			sp->maxstack = 1; // Healing reduction shouldn't stack

		if( sp->NameHash == SPELL_HASH_EXPLOSIVE_SHOT )
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;

		if( sp->NameHash == SPELL_HASH_MORTAL_SHOTS )
			sp->EffectSpellClassMask[0][0] += 1;

		if( sp->NameHash == SPELL_HASH_MEND_PET )
			sp->ChannelInterruptFlags = 0;

		if( sp->NameHash == SPELL_HASH_EAGLE_EYE )
			sp->Effect[1] = 0;

		if( sp->NameHash == SPELL_HASH_ENTRAPMENT && sp->EffectApplyAuraName[0] == SPELL_AURA_MOD_ROOT )
		{
			sp->EffectImplicitTargetA[0] = 15;
			sp->EffectRadiusIndex[0] = 13;
		}

		//////////////////////////////////////////
		// ROGUE								//
		//////////////////////////////////////////

		// Insert rogue spell fixes here
		if( sp->NameHash == SPELL_HASH_REMORSELESS_ATTACKS )
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE;

		if( sp->NameHash == SPELL_HASH_UNFAIR_ADVANTAGE )
			sp->procFlags = PROC_ON_DODGE_VICTIM;

		if( sp->NameHash == SPELL_HASH_COMBAT_POTENCY )
			sp->procFlags = PROC_ON_MELEE_ATTACK;

		if( sp->NameHash == SPELL_HASH_PAIN_AND_SUFFERING )
			sp->procFlags = PROC_ON_CAST_SPELL;

		if( sp->NameHash == SPELL_HASH_FOCUSED_ATTACKS )
			sp->procFlags = PROC_ON_CRIT_ATTACK;

		if( sp->NameHash == SPELL_HASH_SEAL_FATE )
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}

		if( sp->NameHash == SPELL_HASH_VILE_POISONS )
		{
			sp->EffectSpellClassMask[0][0] = 8388608; // envenom
			sp->EffectSpellClassMask[0][1] = 8;
			sp->EffectSpellClassMask[1][0] = 8192 | 268435456 | 65536; //poisons
			sp->EffectSpellClassMask[1][1] = 524288;
		}

		if( sp->NameHash == SPELL_HASH_STEALTH )
		{
			if( !(sp->AuraInterruptFlags & AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN ) )
				sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN;
			
			// fuck this
			sp->EffectBasePoints[1] = 0;
		}

		if( sp->NameHash == SPELL_HASH_NERVES_OF_STEEL )
		{
			sp->CasterAuraState = 6;
			sp->EffectBasePoints[0] = -31;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
		}

		if( sp->NameHash == SPELL_HASH_DISARM_TRAP )
			sp->Effect[0] = SPELL_EFFECT_DUMMY;

		//////////////////////////////////////////
		// PRIEST								//
		//////////////////////////////////////////

		// Insert priest spell fixes here
		if( sp->NameHash == SPELL_HASH_BORROWED_TIME )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[1] = SPELL_AURA_ADD_PCT_MODIFIER;
		}

		if( sp->NameHash == SPELL_HASH_IMPROVED_SPIRIT_TAP )
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;

		if( sp->NameHash == SPELL_HASH_MISERY )
			sp->procFlags = PROC_ON_CAST_SPELL; 

		if( sp->NameHash == SPELL_HASH_POWER_INFUSION )
			sp->buffType = SPELL_TYPE_HASTE;

		// Pure hax
		if( sp->NameHash == SPELL_HASH_LIGHTWELL )
		{
			sp->Effect[0] = 50; //Spawn GO
			switch(sp->Id)
			{
			case 724:
				sp->EffectMiscValue[0] = 300000;
				break;
			case 27870:
				sp->EffectMiscValue[0] = 300001;
				break;
			case 27871:
				sp->EffectMiscValue[0] = 300002;
				break;
			case 28275:
				sp->EffectMiscValue[0] = 300003;
				break;
			case 48086:
				sp->EffectMiscValue[0] = 300004;
				break;
			case 48087:
				sp->EffectMiscValue[0] = 300005;
				break;
			}
		}


		//////////////////////////////////////////
		// SHAMAN								//
		//////////////////////////////////////////

		// Insert shaman spell fixes here
		
		//Heroism/bloodlust/power infusion stack
		if( sp->NameHash == SPELL_HASH_HEROISM || sp->NameHash == SPELL_HASH_BLOODLUST )
			sp->buffType = SPELL_TYPE_HASTE;

		//Hex hax
		if( sp->NameHash == SPELL_HASH_HEX )
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_UNUSED2;

		// Glyph of hex
		if( sp->Id == 63291 )
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;

		// Lightning Shield - cannot crit
		if( sp->NameHash == SPELL_HASH_LIGHTNING_SHIELD ) // not a mistake, the correct proc spell for lightning shield is also called lightning shield
			sp->spell_can_crit = false;

		if( sp->NameHash == SPELL_HASH_FROSTBRAND_WEAPON )
			sp->ProcsPerMinute = 9.0f;

		// Nature's Guardian
		if( sp->NameHash == SPELL_HASH_NATURE_S_GUARDIAN )
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_ANY_DAMAGE_VICTIM;
			sp->proc_interval = 8000;
			sp->EffectTriggerSpell[0] = 31616;
		}

		if(sp->NameHash == SPELL_HASH_WINDFURY_WEAPON || sp->NameHash == SPELL_HASH_FLAMETONGUE_WEAPON || sp->NameHash == SPELL_HASH_ROCKBITER_WEAPON ||
				sp->NameHash == SPELL_HASH_FROSTBRAND_WEAPON ||	sp->NameHash == SPELL_HASH_EARTHLIVING_WEAPON)
			sp->Flags3 |= FLAGS3_ENCHANT_OWN_ONLY;

		// Stoneclaw Totem
		if(sp->NameHash == SPELL_HASH_STONECLAW_TOTEM_PASSIVE)
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;

		// Flametongue Totem passive target fix
		if(sp->NameHash == SPELL_HASH_FLAMETONGUE_TOTEM && sp->Attributes & ATTRIBUTES_PASSIVE)
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[1] = 0;
		}

		// Unleashed Rage
		if(sp->NameHash == SPELL_HASH_UNLEASHED_RAGE)
			sp->procFlags = PROC_ON_CRIT_ATTACK;

		// Healing Stream
		if(sp->NameHash == SPELL_HASH_HEALING_STREAM_TOTEM && sp->Effect[0] == SPELL_EFFECT_DUMMY)
		{
			sp->EffectRadiusIndex[0] = 10; // 30 yards
			sp->Effect[0] = SPELL_EFFECT_HEAL;
			sp->logsId = 5394;
		}

		// Mana Spring
		if(sp->NameHash == SPELL_HASH_MANA_SPRING_TOTEM && sp->Effect[0] == SPELL_EFFECT_DUMMY)
		{
			sp->Effect[0] = SPELL_EFFECT_ENERGIZE;
			sp->EffectMiscValue[0] = POWER_TYPE_MANA;
			sp->logsId = 5675;
		}

		if( sp->NameHash == SPELL_HASH_FAR_SIGHT )
			sp->Effect[1] = 0;

		//////////////////////////////////////////
		// MAGE								//
		//////////////////////////////////////////

		// Insert mage spell fixes here

		// Hypothermia: undispellable
		if( sp->NameHash == SPELL_HASH_HYPOTHERMIA )
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;

		if( sp->NameHash == SPELL_HASH_IMPROVED_COUNTERSPELL )
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 0x00004000;	// Proc on counterspell only
		}

		//////////////////////////////////////////
		// WARLOCK								//
		//////////////////////////////////////////

		// Insert warlock spell fixes here
		if( sp->NameHash == SPELL_HASH_SHADOW_WEAVING )
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = sp->EffectBasePoints[0] + 1;
		}
		if( sp->NameHash == SPELL_HASH_ERADICATION )
			sp->procFlags = PROC_ON_SPELL_LAND;

		if( sp->NameHash == SPELL_HASH_SHADOW_TRANCE )
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;

		if( sp->NameHash == SPELL_HASH_NETHER_PROTECTION )
		{
			sp->EffectTriggerSpell[0] = 54370;
			sp->procFlags = PROC_ON_SPELL_LAND;
		}

		//////////////////////////////////////////
		// DRUID								//
		//////////////////////////////////////////

		// Insert druid spell fixes here

		//infected wounds
		if( sp->NameHash == SPELL_HASH_INFECTED_WOUNDS )
			sp->procFlags = PROC_ON_CAST_SPELL;

		if( sp->NameHash == SPELL_HASH_POUNCE )
			sp->always_apply = true;

		if( sp->NameHash == SPELL_HASH_OWLKIN_FRENZY )
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;

		if( sp->NameHash == SPELL_HASH_EARTH_AND_MOON )
			sp->procFlags = PROC_ON_CAST_SPELL;

		if( sp->NameHash == SPELL_HASH_STARFALL && sp->Effect[1] == SPELL_EFFECT_TRIGGER_SPELL )
		{//we can only attack one target with main star
			sp->MaxTargets = 1;
		}

		if( sp->NameHash == SPELL_HASH_SHRED )
			sp->MechanicsType = MECHANIC_BLEEDING;

		if( sp->NameHash == SPELL_HASH_NURTURING_INSTINCT )
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}

		if( sp->NameHash == SPELL_HASH_PRIMAL_TENACITY )
		{
			sp->DurationIndex = 21;
			sp->EffectBasePoints[1] = 0;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}

		// fuck you too
		if( sp->NameHash == SPELL_HASH_PROWL )
			sp->EffectBasePoints[0] = 0;

		//////////////////////////////////////////
		// OTHER								//
		//////////////////////////////////////////
		// fix for relics procs - pure hax
		if( sp->EffectSpellClassMask[0][1] == 16 && 
			sp->EffectSpellClassMask[1][0] == 995691415 &&
			sp->EffectSpellClassMask[1][1] == 64 )
		{
			sp->EffectSpellClassMask[0][1] = 0; 
			sp->EffectSpellClassMask[1][0] = 0;
			sp->EffectSpellClassMask[1][1] = 0;
		}
	}

	//Fully loaded coefficients, we must share channeled coefficient to its triggered spells
	for(uint32 x=0; x < cnt; x++)
	{
		sp = dbcSpell.LookupRow(x);
		ApplySingleSpellFixes(sp);
	}
	for(list<SpellEntry*>::iterator itr = sWorld.dummyspells.begin(); itr != sWorld.dummyspells.end(); ++itr)
		ApplySingleSpellFixes(*itr);



	/////////////////////////////////////////////////////////////////
	//FORCER CREATURE SPELL TARGETING
	//////////////////////////////////////////////////////////////////
	QueryResult * resultfcst = WorldDatabase.Query("SELECT * FROM spell_forced_targets");
	if( resultfcst != NULL )
	{
		do 
		{
			Field * f;
			f = resultfcst->Fetch();
			sp = dbcSpell.LookupEntryForced( f[0].GetUInt32() );
			if( sp )
				sp->forced_creature_target = f[1].GetUInt32();
			} while( resultfcst->NextRow() );
		delete resultfcst;
	}

	//Settings for special cases
	QueryResult * resultx = WorldDatabase.Query("SELECT * FROM spell_coef_override");
	if( resultx != NULL )
	{
		uint32 spellid;
		do 
		{
			Field * f;
			f = resultx->Fetch();
			spellid = f[0].GetUInt32();
			SpellEntry * sp = dbcSpell.LookupEntryForced( spellid );
			if( sp != NULL )
			{
				sp->Dspell_coef_override = f[1].GetFloat();
				sp->OTspell_coef_override = f[2].GetFloat();
				sp->AP_coef_override = f[3].GetFloat();
				sp->RAP_coef_override = f[4].GetFloat();
			}
			else
			{
				if(Config.MainConfig.GetBoolDefault("Server", "CleanDatabase", false))
				{
					WorldDatabase.Query("DELETE FROM spell_coef_override where id = '%u'", spellid);
				}
				Log.Warning("SpellCoefOverride", "Has nonexistant spell %u.", spellid);
			}
			spellid = 0;
		} while( resultx->NextRow() );
		delete resultx;
	}

	sp = dbcSpell.LookupEntryForced( 26659 );
	SpellEntry * sp2 = sp;
	sp2->Id = 62388;
	sp2->Name = "Dummy Shit";
	sp2->DurationIndex = 41;
	sp2->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
	dbcSpell.SetRow(62388,sp2);


}
