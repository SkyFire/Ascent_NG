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

/** Table formats converted to strings
 */
const char * gItemPrototypeFormat						= "uuuussssuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuffuffuffuffuffuuuuuuuuuufuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gCreatureNameFormat						= "usssuuuuuuuuuuffcc";
const char * gGameObjectNameFormat						= "uuusuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gCreatureProtoFormat						= "uuuuuuufuuuffuffuuuuuuuuuuuuuuuuuuffsuuuufffu";
const char * gAreaTriggerFormat							= "ucuusffffuu";
const char * gItemPageFormat							= "usu";
const char * gNpcTextFormat								= "ufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuu";
const char * gQuestFormat								= "uuuuuuuuuuuuuuuuuuussssssssssuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gSpellExtraFormat							= "uuuu";
const char * gGraveyardFormat							= "uffffuuuux";
const char * gTeleportCoordFormat						= "uxufffx";
const char * gPvPAreaFormat								= "ush";
const char * gFishingFormat								= "uuu";
const char * gWorldMapInfoFormat						= "uuuuufffusuuuuuuufu";
const char * gZoneGuardsFormat							= "uuu";

/** SQLStorage symbols
 */
SERVER_DECL SQLStorage<ItemPrototype, ArrayStorageContainer<ItemPrototype> >				ItemPrototypeStorage;
SERVER_DECL SQLStorage<CreatureInfo, HashMapStorageContainer<CreatureInfo> >				CreatureNameStorage;
SERVER_DECL SQLStorage<GameObjectInfo, HashMapStorageContainer<GameObjectInfo> >			GameObjectNameStorage;
SERVER_DECL SQLStorage<CreatureProto, HashMapStorageContainer<CreatureProto> >				CreatureProtoStorage;
SERVER_DECL SQLStorage<AreaTrigger, HashMapStorageContainer<AreaTrigger> >					AreaTriggerStorage;
SERVER_DECL SQLStorage<ItemPage, HashMapStorageContainer<ItemPage> >						ItemPageStorage;
SERVER_DECL SQLStorage<Quest, HashMapStorageContainer<Quest> >								QuestStorage;
SERVER_DECL SQLStorage<GossipText, HashMapStorageContainer<GossipText> >					NpcTextStorage;
SERVER_DECL SQLStorage<GraveyardTeleport, HashMapStorageContainer<GraveyardTeleport> >		GraveyardStorage;
SERVER_DECL SQLStorage<TeleportCoords, HashMapStorageContainer<TeleportCoords> >			TeleportCoordStorage;
SERVER_DECL SQLStorage<FishingZoneEntry, HashMapStorageContainer<FishingZoneEntry> >		FishingZoneStorage;
SERVER_DECL SQLStorage<MapInfo, ArrayStorageContainer<MapInfo> >							WorldMapInfoStorage;
SERVER_DECL SQLStorage<ZoneGuardEntry, HashMapStorageContainer<ZoneGuardEntry> >			ZoneGuardStorage;

SERVER_DECL set<string> ExtraMapCreatureTables;
SERVER_DECL set<string> ExtraMapGameObjectTables;

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
	QueryResult * result = WorldDatabase.Query( "SELECT * FROM ai_agents" );
	CreatureProto * cn;

	if( result != NULL )
	{
		AI_Spell *sp;
		SpellEntry * spe;
		uint32 entry;

		if(Config.MainConfig.GetBoolDefault("Server", "LoadAIAgents", true))
		{
			do
			{
				Field *fields = result->Fetch();
				entry = fields[0].GetUInt32();
				cn = CreatureProtoStorage.LookupEntry(entry);
				spe = dbcSpell.LookupEntryForced(fields[5].GetUInt32());
				if( spe == NULL )
				{
					Log.Warning("AIAgent", "For %u has nonexistant spell %u.", fields[0].GetUInt32(), fields[5].GetUInt32());
				}
				if(!cn)
					continue;

				sp = new AI_Spell;
				sp->entryId = fields[0].GetUInt32();
				sp->agent = fields[1].GetUInt16();
				sp->procChance = fields[3].GetUInt32();
				sp->procCount = fields[4].GetUInt32();
				sp->spell = spe;
				sp->spellType = fields[6].GetUInt32();
				sp->spelltargetType = fields[7].GetUInt32();
				sp->cooldown = fields[8].GetUInt32();
				sp->floatMisc1 = fields[9].GetFloat();
				sp->autocast_type=(uint32)-1;
				sp->custom_pointer=false;
				sp->cooldowntime=getMSTime();
				sp->procCounter=0;

		/*		if (!sp->procCountDB) 
					sp->procCount = uint32(-1);
				else sp->procCount = sp->procCountDB;*/
				sp->Misc2 = fields[10].GetUInt32();
				if(sp->agent == AGENT_SPELL)
				{
					if(!sp->spell)
					{
						//printf("SpellId %u in ai_agent for %u is invalid.\n", (unsigned int)fields[5].GetUInt32(), (unsigned int)sp->entryId);
						delete sp;
						continue;
					}
					
					if(sp->spell->Effect[0] == SPELL_EFFECT_LEARN_SPELL || sp->spell->Effect[1] == SPELL_EFFECT_LEARN_SPELL ||
						sp->spell->Effect[2] == SPELL_EFFECT_LEARN_SPELL)
					{
						//printf("Teaching spell %u in ai_agent for %u\n", (unsigned int)fields[5].GetUInt32(), (unsigned int)sp->entryId);
						delete sp;
						continue;
					}

					sp->minrange = GetMinRange(dbcSpellRange.LookupEntry(sp->spell->rangeIndex));
					sp->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(sp->spell->rangeIndex));

					//omg the poor darling has no clue about making ai_agents
					if(sp->cooldown==0xffffffff)
					{
						//now this will not be exact cooldown but maybe a bigger one to not make him spam spells to often
						int cooldown;
						SpellDuration *sd=dbcSpellDuration.LookupEntry(sp->spell->DurationIndex);
						int Dur=0;
						int Casttime=0;//most of the time 0
						int RecoveryTime=sp->spell->RecoveryTime;
						if(sp->spell->DurationIndex)
							Dur =::GetDuration(sd);
						Casttime=GetCastTime(dbcSpellCastTime.LookupEntry(sp->spell->CastingTimeIndex));
						cooldown=Dur+Casttime+RecoveryTime;
						if(cooldown<0)
							sp->cooldown=0;//huge value that should not loop while adding some timestamp to it
						else sp->cooldown=cooldown;
					}

					/*
					//now apply the morron filter
					if(sp->procChance==0)
					{
						//printf("SpellId %u in ai_agent for %u is invalid.\n", (unsigned int)fields[5].GetUInt32(), (unsigned int)sp->entryId);
						delete sp;
						continue;
					}
					if(sp->spellType==0)
					{
						//right now only these 2 are used
						if(IsBeneficSpell(sp->spell))
							sp->spellType==STYPE_HEAL;
						else sp->spellType==STYPE_BUFF;
					}
					if(sp->spelltargetType==0)
						sp->spelltargetType = RecommandAISpellTargetType(sp->spell);
						*/
				}

				if(sp->agent == AGENT_RANGED)
				{
					cn->m_canRangedAttack = true;
					delete sp;
				}
				else if(sp->agent == AGENT_FLEE)
				{
					cn->m_canFlee = true;
					if(sp->floatMisc1)
						cn->m_canFlee = (sp->floatMisc1>0.0f ? true : false);
					else
						cn->m_fleeHealth = 0.2f;

					if(sp->Misc2)
						cn->m_fleeDuration = sp->Misc2;
					else
						cn->m_fleeDuration = 10000;

					delete sp;
				}
				else if(sp->agent == AGENT_CALLFORHELP)
				{
					cn->m_canCallForHelp = true;
					if(sp->floatMisc1)
						cn->m_callForHelpHealth = 0.2f;
					delete sp;
				}
				else
				{
					cn->spells.push_back(sp);
				}
			} while( result->NextRow() );
		}

		delete result;
	}
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
		pItemPrototype->lowercase_name = pItemPrototype->Name1;
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
		case 28071:
		case 28072:
		case 28073:
		case 25469:
		case 23734:
		case 23735:
		case 23755:
		case 16381:
		case 16382:
		case 16383:
		case 22188:
		case 22189:
		case 25900:
		case 23711:
		case 23730:
		case 23731:
		case 16390:
		case 16388:
		case 16389:
		case 16384:
		case 16385:
		case 16386:
		case 16387:
		case 22190:
			// Felguard
			pItemPrototype->ForcedPetId = 17252;
			break;

		case 16321:
		case 16322:
		case 16323:
		case 16324:
		case 16325:
		case 16326:
		case 16327:
		case 16328:
		case 16329:
		case 16330:
		case 22181:
		case 16302:
		case 16316:
		case 16317:
		case 16318:
		case 16319:
		case 16320:
		case 22179:
		case 16331:
			// Imp
			pItemPrototype->ForcedPetId = 416;
			break;

		case 16357:
		case 16358:
		case 16359:
		case 16360:
		case 16361:
		case 16362:
		case 22184:
		case 16351:
		case 16352:
		case 16353:
		case 16354:
		case 16355:
		case 16356:
		case 22185:
		case 16346:
		case 16347:
		case 16348:
		case 16349:
		case 16350:
		case 22182:
			// Voidwalker
			pItemPrototype->ForcedPetId = 1860;
			break;

		case 16380:
		case 16379:
		case 16375:
		case 16376:
		case 16377:
		case 16378:
		case 22187:
		case 16363:
		case 16364:
		case 16365:
		case 16366:
		case 22183:
		case 28068:
			// Succubus
			pItemPrototype->ForcedPetId = 1863;
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
	new CallbackP2< SQLStorage< itype, storagetype< itype > >, const char *, const char *> \
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
}

void Storage_Cleanup()
{
	{
		StorageContainerIterator<CreatureProto> * itr = CreatureProtoStorage.MakeIterator();
		CreatureProto * p;
		while(!itr->AtEnd())
		{
			p = itr->Get();
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
	AreaTriggerStorage.Cleanup();
	ItemPageStorage.Cleanup();
	QuestStorage.Cleanup();
	GraveyardStorage.Cleanup();
	TeleportCoordStorage.Cleanup();
	FishingZoneStorage.Cleanup();
	NpcTextStorage.Cleanup();
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


