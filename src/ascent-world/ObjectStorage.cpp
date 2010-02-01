/*
* Ascent MMORPG Server
* Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
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

/** Table formats converted to strings
b = bool
c = uint8
h = uint16
u = uint32
i = int32
f = float
s = string
x = skip
*/
const char * gAchievementRewardFormat					= "uuuu";
const char * gAreaTriggerFormat							= "ucuusffffuu";
const char * gCreatureNameFormat						= "usssuuuuuuuuuuffcc";
const char * gCreatureProtoFormat						= "uuuuuuufuuuffuffuuuuuuuuuuuffsuuuufffuuic";
const char * gFishingFormat								= "uuu";
const char * gGameObjectNameFormat						= "uuusuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gGraveyardFormat							= "uffffuuuux";
const char * gItemPageFormat							= "usu";
const char * gItemPrototypeFormat						= "uuuussssuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuffuffuffuffuffuuuuuuuuuufuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gNpcTextFormat								= "ufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuu";
const char * gQuestFormat								= "uuuuuuuuuuuuuuuuuuuussssssssssuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuc";
const char * gTeleportCoordFormat						= "uxuffff";
const char * gWorldMapInfoFormat						= "uuuuufffusuuuuuuufub";
const char * gRandomItemCreationFormat					= "uuuu";
const char * gRandomCardCreationFormat					= "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gScrollCreationFormat						= "uu";
const char * gZoneGuardsFormat							= "uuu";

/** SQLStorage symbols
 */
SERVER_DECL SQLStorage<AchievementReward, HashMapStorageContainer<AchievementReward> >		AchievementRewardStorage;
SERVER_DECL SQLStorage<AreaTrigger, HashMapStorageContainer<AreaTrigger> >					AreaTriggerStorage;
SERVER_DECL SQLStorage<CreatureInfo, HashMapStorageContainer<CreatureInfo> >				CreatureNameStorage;
SERVER_DECL SQLStorage<CreatureProto, HashMapStorageContainer<CreatureProto> >				CreatureProtoStorage;
SERVER_DECL SQLStorage<FishingZoneEntry, HashMapStorageContainer<FishingZoneEntry> >		FishingZoneStorage;
SERVER_DECL SQLStorage<GameObjectInfo, HashMapStorageContainer<GameObjectInfo> >			GameObjectNameStorage;
SERVER_DECL SQLStorage<GraveyardTeleport, HashMapStorageContainer<GraveyardTeleport> >		GraveyardStorage;
SERVER_DECL SQLStorage<ItemPage, HashMapStorageContainer<ItemPage> >						ItemPageStorage;
SERVER_DECL SQLStorage<ItemPrototype, ArrayStorageContainer<ItemPrototype> >				ItemPrototypeStorage;
SERVER_DECL SQLStorage<GossipText, HashMapStorageContainer<GossipText> >					NpcTextStorage;
SERVER_DECL SQLStorage<Quest, HashMapStorageContainer<Quest> >								QuestStorage;
SERVER_DECL SQLStorage<TeleportCoords, HashMapStorageContainer<TeleportCoords> >			TeleportCoordStorage;
SERVER_DECL SQLStorage<MapInfo, ArrayStorageContainer<MapInfo> >							WorldMapInfoStorage;
SERVER_DECL SQLStorage<ZoneGuardEntry, HashMapStorageContainer<ZoneGuardEntry> >			ZoneGuardStorage;
SERVER_DECL SQLStorage<RandomItemCreation, HashMapStorageContainer<RandomItemCreation> >	RandomItemCreationStorage;
SERVER_DECL SQLStorage<RandomCardCreation, HashMapStorageContainer<RandomCardCreation> >	RandomCardCreationStorage;
SERVER_DECL SQLStorage<ScrollCreation, HashMapStorageContainer<ScrollCreation> >			ScrollCreationStorage;


SERVER_DECL set<string> ExtraMapCreatureTables;
SERVER_DECL set<string> ExtraMapGameObjectTables;

void ObjectMgr::LoadProfessionDiscoveries()
{
	QueryResult * result = WorldDatabase.Query("SELECT * from professiondiscoveries");
	if ( result != NULL )
	{
		do
		{
			Field *f = result->Fetch();
			ProfessionDiscovery * pf = new ProfessionDiscovery;
			pf->SpellId = f[0].GetUInt32();
			pf->SpellToDiscover = f[1].GetUInt32();
			pf->SkillValue = f[2].GetUInt32();
			pf->Chance = f[3].GetFloat();
			ProfessionDiscoveryTable.insert( pf );
		}
		while( result->NextRow() );
	}
	delete result;
}

void ObjectMgr::LoadExtraCreatureProtoStuff()
{
	{
		StorageContainerIterator<CreatureProto> * itr = CreatureProtoStorage.MakeIterator();
		CreatureProto * cn;
		while(!itr->AtEnd())
		{
			cn = itr->Get();
			if(itr->Get()->aura_string)
			{
				string auras = string(itr->Get()->aura_string);
				vector<string> aurs = StrSplit(auras, " ");
				for(vector<string>::iterator it = aurs.begin(); it != aurs.end(); ++it)
				{
					uint32 id = atol((*it).c_str());
					if(id)
						itr->Get()->start_auras.insert( id );
				}
			}

			if(!itr->Get()->MinHealth)
				itr->Get()->MinHealth = 1;
			if(!itr->Get()->MaxHealth)
				itr->Get()->MaxHealth = 1;
			if (itr->Get()->AttackType > SCHOOL_ARCANE)
				itr->Get()->AttackType = SCHOOL_NORMAL;

			cn->m_canFlee = cn->m_canRangedAttack = cn->m_canCallForHelp = false;
			cn->m_fleeHealth = 0.0f;
			// please.... m_fleeDuration is a uint32...
			//cn->m_fleeDuration = 0.0f;
			cn->m_fleeDuration = 0;

			if(!itr->Inc())
				break;
		}

		itr->Destruct();
	}

	{
		StorageContainerIterator<CreatureInfo> * itr = CreatureNameStorage.MakeIterator();
		CreatureInfo * ci;
		while(!itr->AtEnd())
		{
			ci = itr->Get();

			ci->lowercase_name = string(ci->Name);
			for(uint32 j = 0; j < ci->lowercase_name.length(); ++j)
				ci->lowercase_name[j] = tolower(ci->lowercase_name[j]); // Darvaleo 2008/08/15 - Copied lowercase conversion logic from ItemPrototype task

			ci->gossip_script = sScriptMgr.GetDefaultGossipScript();

			if(!itr->Inc())
				break;
		}
		itr->Destruct();
	}

	{
		StorageContainerIterator<Quest> * itr = QuestStorage.MakeIterator();
		Quest * qst;
		while(!itr->AtEnd())
		{
			qst = itr->Get();
			qst->pQuestScript = NULL;

			if( !itr->Inc() )
				break;
		}
		itr->Destruct();
	}

	// Load AI Agents
	if(!Config.MainConfig.GetBoolDefault("Server", "LoadAIAgents", true))
		return;

	QueryResult * result = WorldDatabase.Query( "SELECT Entry,Type+0,Chance,MaxCount,Spell,SpellType+0,TargetType+0,CoolDown,floatMisc1,Misc2 FROM ai_agents" );
	CreatureProto * cn = NULL;

	if( result != NULL )
	{
		AI_Spell *sp = NULL;
		SpellEntry * spe = NULL;
		uint32 entry = 0;
		uint32 spellID = 0;
		uint16 agent = 0;
		uint32 counter = 0;
		{
			do
			{
				sp = NULL;
				spe = NULL;
				entry = 0;
				spellID = 0;
				agent = 0;
				Field *fields = result->Fetch();
				entry = fields[0].GetUInt32();
				agent = fields[1].GetUInt16();
				spellID = fields[4].GetUInt32();
				int32 tcd = fields[7].GetInt32();

				cn = CreatureProtoStorage.LookupEntry(entry);
				if(  cn == NULL )
				{
					Log.Warning("AIAgent", "Agent skipped, NPC %u does not exist.", fields[0].GetUInt32());
					continue;
				}

				spe = dbcSpell.LookupEntryForced(spellID);
				if( spe == NULL )
				{
					Log.Warning("AIAgent", "Agent skipped, NPC %u tried to add non-existing Spell %u.", fields[0].GetUInt32(), fields[4].GetUInt32());
					continue;
				}

				sp = new AI_Spell;
				sp->entryId = entry;
				sp->agent = agent;
				sp->procChance = fields[2].GetUInt32();
				sp->procCount = fields[3].GetUInt32();
				sp->spell = spe;
				sp->spellType = fields[5].GetUInt8();
				sp->spelltargetType = fields[6].GetUInt8();
				if( spe->c_is_flags & SPELL_FLAG_CASTED_ON_FRIENDS && !(sp->spelltargetType == TTYPE_OWNER))//just to make sure ;)
					sp->spelltargetType = TTYPE_CASTER;
				sp->cooldown = (tcd <0 ? 0 : tcd);
				sp->floatMisc1 = fields[8].GetFloat();
				sp->Misc2 = fields[9].GetUInt32();
				sp->autocast_type=(uint32)-1;
				sp->custom_pointer=false;
				sp->procCounter=0;

				//Set cooldowntimer
				sp->cooldowntime=getMSTime();

				switch(sp->agent)
				{
					case AGENT_SPELL:
					{
						if(sp->spell->Effect[0] == SPELL_EFFECT_LEARN_SPELL || sp->spell->Effect[1] == SPELL_EFFECT_LEARN_SPELL ||
							sp->spell->Effect[2] == SPELL_EFFECT_LEARN_SPELL)
						{
							Log.Warning("AIAgent","SpellId %u skipped in ai_agent for NPC %u, it is a teaching spell", spellID, sp->entryId);
							delete sp;
							sp = NULL;
							continue;
						}
						if(sp->spell->Effect[0] == SPELL_EFFECT_SCRIPT_EFFECT || sp->spell->Effect[1] == SPELL_EFFECT_SCRIPT_EFFECT || sp->spell->Effect[2] == SPELL_EFFECT_SCRIPT_EFFECT)
							DEBUG_LOG("AIAgent","Spell %u for NPC %u is a scripted_effect, can result in unwanted side effects", spellID, sp->entryId);

						sp->minrange = GetMinRange(dbcSpellRange.LookupEntry(sp->spell->rangeIndex));
						sp->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(sp->spell->rangeIndex));

						if( tcd < 0 ) // -1 will force dbc lookup
						{
							//now this will not be exact cooldown but maybe a bigger one to not make him spam spells to often
							uint32 cooldown = 0;

							if (sp->spell->Attributes & ATTRIBUTES_PASSIVE) //passive skills
							{
								cooldown = 1000*60*60*4; //once per 4 hours :P
							}else{
								if( sp->spell->CastingTimeIndex )
									cooldown = GetCastTime( dbcSpellCastTime.LookupEntry( sp->spell->CastingTimeIndex ));

								uint32 maxRT = std::max(sp->spell->RecoveryTime, sp->spell->CategoryRecoveryTime);
								if (maxRT>cooldown)
									cooldown = maxRT;
							}

							if(cooldown <= 0)
							{
								Log.Warning("AIAgent","SpellId %u has no CoolDownTime in DBC. Forced to GCD.", spellID );
								sp->cooldown=1500;
							}else{
								sp->cooldown=cooldown;
							}
						}
						counter += 1;
					}break;
	
					case AGENT_RANGED:
					{
						cn->m_canRangedAttack = true;
						cn->m_RangedAttackSpell = (spellID?spellID:15620);
						cn->m_SpellSoundid = sp->Misc2;
						delete sp;
						sp = NULL;
						counter += 1;
					}break;

					case AGENT_FLEE:
					{
						// % health 
						cn->m_canFlee = true;
						if(sp->floatMisc1)
							cn->m_fleeHealth = sp->floatMisc1;
						else //if left to zero, start running inmeadetely
							cn->m_fleeHealth = 100.0f;

						if(sp->Misc2)
							cn->m_fleeDuration = sp->Misc2;
						else
							cn->m_fleeDuration = 10000;

						delete sp;
						sp = NULL;
						counter += 1;
					}break;

					case AGENT_CALLFORHELP:
					{
						cn->m_canCallForHelp = true;
						if(sp->floatMisc1)
							cn->m_callForHelpHealth = sp->floatMisc1;
						else
							cn->m_callForHelpHealth = 0.2f;

						delete sp;
						sp = NULL;
						counter += 1;
					}break;

					//Unsupported Agent type, don't add to list
					default:
					{
						Log.Warning("AIAgent","Skipping in-valid  entry %u for ai_type %u.", sp->entryId, sp->agent );
						delete sp;
						sp = NULL;
					}break;
				}
				//Valid; add to list
				if(sp != NULL)
					cn->spells.push_back(sp);
				sp = NULL;
			}while( result->NextRow() );
		}
		if(counter)
			Log.Notice("AIAgent","Loaded %u ai_agents from database",counter);
		else
			Log.Warning("AIAgent","No ai_agents found in database");
	}
	if(result)
		delete result;
}

void ObjectMgr::LoadExtraItemStuff()
{
	map<uint32,uint32> foodItems;
	QueryResult * result = WorldDatabase.Query("SELECT * FROM itempetfood ORDER BY entry");
	if(result)
	{
		Field *f = result->Fetch();
		do
		{		
			foodItems.insert( make_pair( f[0].GetUInt32(), f[1].GetUInt32() ) );
		}
		while(result->NextRow());
	}
	delete result;

	StorageContainerIterator<GameObjectInfo> *gtr = GameObjectNameStorage.MakeIterator();
	while(!gtr->AtEnd())
	{
		gtr->Get()->InvolvedQuestCount =0;
		gtr->Get()->InvolvedQuestIds = NULL;
		gtr->Inc();
	}
	gtr->Destruct();
	

	StorageContainerIterator<ItemPrototype> * itr = ItemPrototypeStorage.MakeIterator();
	ItemPrototype * pItemPrototype;
	while(!itr->AtEnd())
	{
		pItemPrototype = itr->Get();
		if(pItemPrototype->ItemSet > 0)
		{
			ItemSetContentMap::iterator itr = mItemSets.find(pItemPrototype->ItemSet);
			std::list<ItemPrototype*>* l;
			if(itr == mItemSets.end())
			{
				l = new std::list<ItemPrototype*>;				
				mItemSets.insert( ItemSetContentMap::value_type( pItemPrototype->ItemSet, l) );
			} else {
				l = itr->second;
			}
			l->push_back(pItemPrototype);
		}


		// lowercase name, used for searches
		pItemPrototype->lowercase_name = itr->Get()->Name1;
		for(uint32 j = 0; j < pItemPrototype->lowercase_name.length(); ++j)
			pItemPrototype->lowercase_name[j] = tolower(pItemPrototype->lowercase_name[j]);

		//load item_pet_food_type from extra table
		uint32 ft = 0;
		map<uint32,uint32>::iterator iter = foodItems.find(pItemPrototype->ItemId);
		if(iter != foodItems.end())
			ft = iter->second;
		pItemPrototype->FoodType = ft ;
	
		pItemPrototype->gossip_script=NULL;

		// forced pet entries
		switch( pItemPrototype->ItemId )
		{
		case 28071: //Grimoire of Anguish (Rank 1)
		case 28072: //Grimoire of Anguish (Rank 2)
		case 28073: //Grimoire of Anguish (Rank 3)
		case 25469: //Grimoire of Avoidance
		case 23734: //Grimoire of Cleave (Rank 1)
		case 23745: //Grimoire of Cleave (Rank 2)
		case 23755: //Grimoire of Cleave (Rank 3)
		case 25900: //Grimoire of Demonic Frenzy
		case 23711: //Grimoire of Intercept (Rank 1)
		case 23730: //Grimoire of Intercept (Rank 2)
		case 23731: //Grimoire of Intercept (Rank 3)
			// Felguard
			pItemPrototype->ForcedPetId = 17252;
			break;

		case 16321: //Grimoire of Blood Pact (Rank 1)
		case 16322: //Grimoire of Blood Pact (Rank 2)
		case 16323: //Grimoire of Blood Pact (Rank 3)
		case 16324: //Grimoire of Blood Pact (Rank 4)
		case 16325: //Grimoire of Blood Pact (Rank 5)
		case 22180: //Grimoire of Blood Pact (Rank 6)
		case 16326: //Grimoire of Fire Shield (Rank 1)
		case 16327: //Grimoire of Fire Shield (Rank 2)
		case 16328: //Grimoire of Fire Shield (Rank 3)
		case 16329: //Grimoire of Fire Shield (Rank 4)
		case 16330: //Grimoire of Fire Shield (Rank 5)
		case 22181: //Grimoire of Fire Shield (Rank 6)
		case 16302: //Grimoire of Firebolt (Rank 2)
		case 16316: //Grimoire of Firebolt (Rank 3)
		case 16317: //Grimoire of Firebolt (Rank 4)
		case 16318: //Grimoire of Firebolt (Rank 5)
		case 16319: //Grimoire of Firebolt (Rank 6)
		case 16320: //Grimoire of Firebolt (Rank 7)
		case 22179: //Grimoire of Firebolt (Rank 8)
		case 16331: //Grimoire of Phase Shift
			// Imp
			pItemPrototype->ForcedPetId = 416;
			break;

		case 16357: //Grimoire of Consume Shadows (Rank 1)
		case 16358: //Grimoire of Consume Shadows (Rank 2)
		case 16359: //Grimoire of Consume Shadows (Rank 3)
		case 16360: //Grimoire of Consume Shadows (Rank 4)
		case 16361: //Grimoire of Consume Shadows (Rank 5)
		case 16362: //Grimoire of Consume Shadows (Rank 6)
		case 22184: //Grimoire of Consume Shadows (Rank 7)
		case 16351: //Grimoire of Sacrifice (Rank 1)
		case 16352: //Grimoire of Sacrifice (Rank 2)
		case 16353: //Grimoire of Sacrifice (Rank 3)
		case 16354: //Grimoire of Sacrifice (Rank 4)
		case 16355: //Grimoire of Sacrifice (Rank 5)
		case 16356: //Grimoire of Sacrifice (Rank 6)
		case 22185: //Grimoire of Sacrifice (Rank 7)
		case 16363: //Grimoire of Suffering (Rank 1)
		case 16364: //Grimoire of Suffering (Rank 2)
		case 16365: //Grimoire of Suffering (Rank 3)
		case 16366: //Grimoire of Suffering (Rank 4)
		case 22183: //Grimoire of Suffering (Rank 5)
		case 28068: //Grimoire of Suffering (Rank 6)
		case 16346: //Grimoire of Torment (Rank 2)
		case 16347: //Grimoire of Torment (Rank 3)
		case 16348: //Grimoire of Torment (Rank 4)
		case 16349: //Grimoire of Torment (Rank 5)
		case 16350: //Grimoire of Torment (Rank 6)
		case 22182: //Grimoire of Torment (Rank 7)
			// Voidwalker
			pItemPrototype->ForcedPetId = 1860;
			break;

		case 16368: //Grimoire of Lash of Pain (Rank 2)
		case 16371: //Grimoire of Lash of Pain (Rank 3)
		case 16372: //Grimoire of Lash of Pain (Rank 4)
		case 16373: //Grimoire of Lash of Pain (Rank 5)
		case 16374: //Grimoire of Lash of Pain (Rank 6)
		case 22186: //Grimoire of Lash of Pain (Rank 7)
		case 16380: //Grimoire of Lesser Invisibility
		case 16379: //Grimoire of Seduction
		case 16375: //Grimoire of Soothing Kiss (Rank 1)
		case 16376: //Grimoire of Soothing Kiss (Rank 2)
		case 16377: //Grimoire of Soothing Kiss (Rank 3)
		case 16378: //Grimoire of Soothing Kiss (Rank 4)
		case 22187: //Grimoire of Soothing Kiss (Rank 5)
			// Succubus
			pItemPrototype->ForcedPetId = 1863;
			break;

		case 16381: //Grimoire of Devour Magic (Rank 2)
		case 16382: //Grimoire of Devour Magic (Rank 3)
		case 16383: //Grimoire of Devour Magic (Rank 4)
		case 22188: //Grimoire of Devour Magic (Rank 5)
		case 22189: //Grimoire of Devour Magic (Rank 6)
		case 16390: //Grimoire of Paranoia
		case 16388: //Grimoire of Spell Lock (Rank 1)
		case 16389: //Grimoire of Spell Lock (Rank 2)
		case 16384: //Grimoire of Tainted Blood (Rank 1)
		case 16385: //Grimoire of Tainted Blood (Rank 2)
		case 16386: //Grimoire of Tainted Blood (Rank 3)
		case 16387: //Grimoire of Tainted Blood (Rank 4)
		case 22190: //Grimoire of Tainted Blood (Rank 5)
			//Felhunter
			pItemPrototype->ForcedPetId = 417;
			break;

		case 21283:
		case 3144:
		case 21282:
		case 9214:
		case 21281:
		case 22891:
			// Player
			pItemPrototype->ForcedPetId = 0;
			break;

		default:
			pItemPrototype->ForcedPetId = -1;
			break;
		}

        if(!itr->Inc())
			break;
	}

	itr->Destruct();
	foodItems.clear();
}

#define make_task(storage, itype, storagetype, tablename, format) tl.AddTask( new Task( \
	new NoSharedCallbackP2< SQLStorage< itype, storagetype< itype > >, const char *, const char *> \
    (&storage, &SQLStorage< itype, storagetype< itype > >::Load, tablename, format) ) )

void Storage_FillTaskList(TaskList & tl)
{
	make_task(ItemPrototypeStorage, ItemPrototype, ArrayStorageContainer, "items", gItemPrototypeFormat);
	make_task(CreatureNameStorage, CreatureInfo, HashMapStorageContainer, "creature_names", gCreatureNameFormat);
	make_task(GameObjectNameStorage, GameObjectInfo, HashMapStorageContainer, "gameobject_names", gGameObjectNameFormat);
	make_task(CreatureProtoStorage, CreatureProto, HashMapStorageContainer, "creature_proto", gCreatureProtoFormat);
	make_task(AreaTriggerStorage, AreaTrigger, HashMapStorageContainer, "areatriggers", gAreaTriggerFormat);
	make_task(ItemPageStorage, ItemPage, HashMapStorageContainer, "itempages", gItemPageFormat);
	make_task(QuestStorage, Quest, HashMapStorageContainer, "quests", gQuestFormat);
	make_task(GraveyardStorage, GraveyardTeleport, HashMapStorageContainer, "graveyards", gGraveyardFormat);
	make_task(TeleportCoordStorage, TeleportCoords, HashMapStorageContainer, "teleport_coords", gTeleportCoordFormat);
	make_task(FishingZoneStorage, FishingZoneEntry, HashMapStorageContainer, "fishing", gFishingFormat);
	make_task(NpcTextStorage, GossipText, HashMapStorageContainer, "npc_text", gNpcTextFormat);
	make_task(WorldMapInfoStorage, MapInfo, ArrayStorageContainer, "worldmap_info", gWorldMapInfoFormat);
	make_task(ZoneGuardStorage, ZoneGuardEntry, HashMapStorageContainer, "zoneguards", gZoneGuardsFormat);
	make_task(AchievementRewardStorage, AchievementReward, HashMapStorageContainer, "achievement_rewards", gAchievementRewardFormat);
	make_task(RandomItemCreationStorage, RandomItemCreation, HashMapStorageContainer, "randomitemcreation", gRandomItemCreationFormat);
	make_task(RandomCardCreationStorage, RandomCardCreation, HashMapStorageContainer, "randomcardcreation", gRandomCardCreationFormat);
	make_task(ScrollCreationStorage, ScrollCreation, HashMapStorageContainer, "scrollcreation", gScrollCreationFormat);
}

void Storage_Cleanup()
{
	{
		StorageContainerIterator<CreatureProto> * itr = CreatureProtoStorage.MakeIterator();
		CreatureProto * p;
		while(!itr->AtEnd())
		{
			p = itr->Get();

			if (p->aura_string)
			{
				free(p->aura_string);
				p->aura_string = NULL;
			}

			for(list<AI_Spell*>::iterator it = p->spells.begin(); it != p->spells.end(); ++it)
				delete (*it);
			p->spells.clear();
			p->start_auras.clear();
			if(!itr->Inc())
				break;
		}
		itr->Destruct();
	}
	ItemPrototypeStorage.Cleanup();
	CreatureNameStorage.Cleanup();
	GameObjectNameStorage.Cleanup();
	CreatureProtoStorage.Cleanup();
	{
		StorageContainerIterator<AreaTrigger> * itr = AreaTriggerStorage.MakeIterator();
		AreaTrigger * a;
		while(!itr->AtEnd())
		{
			a = itr->Get();

			if (a->Name)
			{
				free(a->Name);
				a->Name = NULL;
			}

			if(!itr->Inc())
				break;
		}
		itr->Destruct();
	}
	AreaTriggerStorage.Cleanup();
	ItemPageStorage.Cleanup();
	RandomItemCreationStorage.Cleanup();
	RandomCardCreationStorage.Cleanup();
	ScrollCreationStorage.Cleanup();

	{
		StorageContainerIterator<Quest> * itr = QuestStorage.MakeIterator();
		Quest * q;
		while(!itr->AtEnd())
		{
			q = itr->Get();

			if (q->title)
			{
				free(q->title);
				q->title = NULL;
			}
			if (q->details)
			{
				free(q->details);
				q->details = NULL;
			}
			if (q->objectives)
			{
				free(q->objectives);
				q->objectives = NULL;
			}
			if (q->completiontext)
			{
				free(q->completiontext);
				q->completiontext = NULL;
			}
			if (q->incompletetext)
			{
				free(q->incompletetext);
				q->incompletetext = NULL;
			}
			if (q->endtext)
			{
				free(q->endtext);
				q->endtext = NULL;
			}
			for(uint8 x = 0; x < 4; x++)
			{
				if (q->objectivetexts[x])
				{
					free(q->objectivetexts[x]);
					q->objectivetexts[x] = NULL;
				}
			}

			if(!itr->Inc())
				break;
		}
		itr->Destruct();
	}


	QuestStorage.Cleanup();
	GraveyardStorage.Cleanup();
	TeleportCoordStorage.Cleanup();
	FishingZoneStorage.Cleanup();
	NpcTextStorage.Cleanup();
		{
		StorageContainerIterator<MapInfo> * itr = WorldMapInfoStorage.MakeIterator();
		MapInfo * m;
		while(!itr->AtEnd())
		{
			m = itr->Get();

			if (m->name)
			{
				free(m->name);
				m->name = NULL;
			}

			if(!itr->Inc())
				break;
		}
		itr->Destruct();
	}
	WorldMapInfoStorage.Cleanup();
	ZoneGuardStorage.Cleanup();
}

vector<pair<string,string> > additionalTables;

bool LoadAdditionalTable(const char * TableName, const char * SecondName)
{
	if(!stricmp(TableName, "creature_spawns"))
	{
		ExtraMapCreatureTables.insert(string(SecondName));
		return false;
	}
	else if(!stricmp(TableName, "gameobject_spawns"))
	{
		ExtraMapGameObjectTables.insert(string(SecondName));
		return false;
	}
	else if(!stricmp(TableName, "items"))					// Items
		ItemPrototypeStorage.LoadAdditionalData(SecondName, gItemPrototypeFormat);
	else if(!stricmp(TableName, "creature_proto"))		// Creature Proto
		CreatureProtoStorage.LoadAdditionalData(SecondName, gCreatureProtoFormat);
	else if(!stricmp(TableName, "creature_names"))		// Creature Names
		CreatureNameStorage.LoadAdditionalData(SecondName, gCreatureNameFormat);
	else if(!stricmp(TableName, "gameobject_names"))	// GO Names
		GameObjectNameStorage.LoadAdditionalData(SecondName, gGameObjectNameFormat);
	else if(!stricmp(TableName, "areatriggers"))		// Areatriggers
		AreaTriggerStorage.LoadAdditionalData(SecondName, gAreaTriggerFormat);
	else if(!stricmp(TableName, "itempages"))			// Item Pages
		ItemPrototypeStorage.LoadAdditionalData(SecondName, gItemPageFormat);
	else if(!stricmp(TableName, "quests"))				// Quests
		QuestStorage.LoadAdditionalData(SecondName, gQuestFormat);
	else if(!stricmp(TableName, "npc_text"))			// NPC Text Storage
		NpcTextStorage.LoadAdditionalData(SecondName, gNpcTextFormat);
	else if(!stricmp(TableName, "fishing"))				// Fishing Zones
		FishingZoneStorage.LoadAdditionalData(SecondName, gFishingFormat);
	else if(!stricmp(TableName, "teleport_coords"))		// Teleport coords
		TeleportCoordStorage.LoadAdditionalData(SecondName, gTeleportCoordFormat);
	else if(!stricmp(TableName, "graveyards"))			// Graveyards
		GraveyardStorage.LoadAdditionalData(SecondName, gGraveyardFormat);
	else if(!stricmp(TableName, "worldmap_info"))		// WorldMapInfo
		WorldMapInfoStorage.LoadAdditionalData(SecondName, gWorldMapInfoFormat);
	else if(!stricmp(TableName, "zoneguards"))
		ZoneGuardStorage.LoadAdditionalData(SecondName, gZoneGuardsFormat);
	else
		return false;

	return true;
}

bool Storage_ReloadTable(const char * TableName)
{
	// bur: mah god this is ugly :P
	if(!stricmp(TableName, "items"))					// Items
		ItemPrototypeStorage.Reload();
	else if(!stricmp(TableName, "creature_proto"))		// Creature Proto
		CreatureProtoStorage.Reload();
	else if(!stricmp(TableName, "creature_names"))		// Creature Names
		CreatureNameStorage.Reload();
	else if(!stricmp(TableName, "gameobject_names"))	// GO Names
		GameObjectNameStorage.Reload();
	else if(!stricmp(TableName, "areatriggers"))		// Areatriggers
		AreaTriggerStorage.Reload();
	else if(!stricmp(TableName, "itempages"))			// Item Pages
		ItemPageStorage.Reload();
	else if(!stricmp(TableName, "quests"))				// Quests
		QuestStorage.Reload();
	else if(!stricmp(TableName, "npc_text"))			// NPC Text Storage
		NpcTextStorage.Reload();
	else if(!stricmp(TableName, "fishing"))				// Fishing Zones
		FishingZoneStorage.Reload();
	else if(!stricmp(TableName, "teleport_coords"))		// Teleport coords
		TeleportCoordStorage.Reload();
	else if(!stricmp(TableName, "graveyards"))			// Graveyards
		GraveyardStorage.Reload();
	else if(!stricmp(TableName, "worldmap_info"))		// WorldMapInfo
		WorldMapInfoStorage.Reload();
	else if(!stricmp(TableName, "zoneguards"))
		ZoneGuardStorage.Reload();
	else if(!stricmp(TableName, "command_overrides"))	// Command Overrides
	{
		CommandTableStorage::getSingleton().Dealloc();
		CommandTableStorage::getSingleton().Init();
		CommandTableStorage::getSingleton().Load();
	}
	else
		return false;
	
	uint32 len = (uint32)strlen(TableName);
	uint32 len2;
	for(vector<pair<string,string> >::iterator itr = additionalTables.begin(); itr != additionalTables.end(); ++itr)
	{
		len2=(uint32)itr->second.length();
		if(!strnicmp(TableName, itr->second.c_str(), min(len,len2)))
			LoadAdditionalTable(TableName, itr->first.c_str());
	}
	return true;
}

void Storage_LoadAdditionalTables()
{
	ExtraMapCreatureTables.insert(string("creature_spawns"));
	ExtraMapGameObjectTables.insert(string("gameobject_spawns"));

	string strData = Config.MainConfig.GetStringDefault("Startup", "LoadAdditionalTables", "");
	if(strData.empty())
		return;

	vector<string> strs = StrSplit(strData, ",");
	if(strs.empty())
		return;

	for(vector<string>::iterator itr = strs.begin(); itr != strs.end(); ++itr)
	{
		char s1[200];
		char s2[200];
		if(sscanf((*itr).c_str(), "%s %s", s1, s2) != 2)
			continue;

		if(LoadAdditionalTable(s2, s1)) {
			pair<string,string> tmppair;
			tmppair.first = string(s1);
			tmppair.second = string(s2);
			additionalTables.push_back(tmppair);
		}
	}
}



