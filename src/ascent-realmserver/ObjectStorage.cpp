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

#include "RStdAfx.h"

/** Table formats converted to strings
 */
const char * gAreaTriggerFormat							= "ucuusffffuu";
const char * gCreatureNameFormat						= "usssuuuuuuuuuuuffcc";
const char * gCreatureProtoFormat						= "uuuuuuufuuuffuffuuuuuuuuuuuffsuibuufffuuic";
const char * gCreatureProtoHeroicFormat                 = "uuuuuffuuuuuuuusu";
const char * gFishingFormat								= "uuu";
const char * gGameObjectNameFormat						= "uuusuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gGraveyardFormat							= "uffffuuuux";
const char * gItemPageFormat							= "usu";
const char * gItemPrototypeFormat						= "uuuussssuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuffuffuffuffuffuuuuuuuuuufuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gNpcTextFormat								= "ufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuu";
const char * gQuestFormat								= "uuuuuuuuuuuuuuuuuuuussssssssssuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuc";
const char * gTeleportCoordFormat						= "uxuffff";
const char * gRealmMapInfoFormat						= "uuuuufffusuuuuuufub";
const char * gZoneGuardsFormat							= "uuu";

/** SQLStorage symbols
 */
SQLStorage<ItemPrototype, ArrayStorageContainer<ItemPrototype> >				ItemPrototypeStorage;
SQLStorage<CreatureInfo, HashMapStorageContainer<CreatureInfo> >				CreatureNameStorage;
SQLStorage<GameObjectInfo, HashMapStorageContainer<GameObjectInfo> >			GameObjectNameStorage;
//SQLStorage<CreatureProto, HashMapStorageContainer<CreatureProto> >				CreatureProtoStorage;
//SQLStorage<AreaTrigger, HashMapStorageContainer<AreaTrigger> >					AreaTriggerStorage;
SQLStorage<ItemPage, HashMapStorageContainer<ItemPage> >						ItemPageStorage;
SQLStorage<Quest, HashMapStorageContainer<Quest> >								QuestStorage;
SQLStorage<GossipText, HashMapStorageContainer<GossipText> >					NpcTextStorage;
//SQLStorage<SpellExtraInfo, HashMapStorageContainer<SpellExtraInfo> >			SpellExtraStorage;
//SQLStorage<GraveyardTeleport, HashMapStorageContainer<GraveyardTeleport> >		GraveyardStorage;
//SQLStorage<TeleportCoords, HashMapStorageContainer<TeleportCoords> >			TeleportCoordStorage;
//SQLStorage<FishingZoneEntry, HashMapStorageContainer<FishingZoneEntry> >		FishingZoneStorage;
SQLStorage<MapInfo, HashMapStorageContainer<MapInfo> >							WorldMapInfoStorage;

void Storage_Load()
{
	ItemPrototypeStorage.Load("items", gItemPrototypeFormat);
	CreatureNameStorage.Load("creature_names", gCreatureNameFormat);
	GameObjectNameStorage.Load("gameobject_names", gGameObjectNameFormat);
	ItemPageStorage.Load("itempages", gItemPageFormat);
	QuestStorage.Load("quests", gQuestFormat);
	WorldMapInfoStorage.Load("realmmap_info", gRealmMapInfoFormat);
}

void Storage_Cleanup()
{
	ItemPrototypeStorage.Cleanup();
	CreatureNameStorage.Cleanup();
	GameObjectNameStorage.Cleanup();
	ItemPageStorage.Cleanup();
	QuestStorage.Cleanup();
	WorldMapInfoStorage.Cleanup();
}

bool Storage_ReloadTable(const char * TableName)
{
	// bur: mah god this is ugly :P
	if(!stricmp(TableName, "items"))					// Items
		ItemPrototypeStorage.Reload();
/*	else if(!stricmp(TableName, "creature_proto"))		// Creature Proto
		CreatureProtoStorage.Reload();*/
	else if(!stricmp(TableName, "creature_names"))		// Creature Names
		CreatureNameStorage.Reload();
	else if(!stricmp(TableName, "gameobject_names"))	// GO Names
		GameObjectNameStorage.Reload();
/*	else if(!stricmp(TableName, "areatriggers"))		// Areatriggers
		AreaTriggerStorage.Reload();*/
	else if(!stricmp(TableName, "itempages"))			// Item Pages
		ItemPageStorage.Reload();
/*	else if(!stricmp(TableName, "spellextra"))			// Spell Extra Info
		SpellExtraStorage.Reload();*/
	else if(!stricmp(TableName, "quests"))				// Quests
		QuestStorage.Reload();
	else if(!stricmp(TableName, "npc_text"))			// NPC Text Storage
		NpcTextStorage.Reload();
/*	else if(!stricmp(TableName, "fishing"))				// Fishing Zones
		FishingZoneStorage.Reload();
	else if(!stricmp(TableName, "teleport_coords"))		// Teleport coords
		TeleportCoordStorage.Reload();
	else if(!stricmp(TableName, "graveyards"))			// Graveyards
		TeleportCoordStorage.Reload();*/
	else if(!stricmp(TableName, "realmmap_info"))		// WorldMapInfo
		WorldMapInfoStorage.Reload();
	else
		return false;
	return true;
}
