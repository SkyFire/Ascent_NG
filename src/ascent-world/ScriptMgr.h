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

#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

#define SCRIPT_MODULE void*
#define ADD_CREATURE_FACTORY_FUNCTION(cl) static CreatureAIScript * Create(CreaturePointer c) { return new cl(c); }
#define ADD_INSTANCE_FACTORY_FUNCTION( ClassName ) static InstanceScript* Create( MapMgrPointer pMapMgr ) { return new ClassName( pMapMgr ); };

class Channel;
class Guild;
struct Quest;
enum ServerHookEvents
{
	SERVER_HOOK_EVENT_ON_NEW_CHARACTER		= 1,
	SERVER_HOOK_EVENT_ON_KILL_PLAYER		= 2,
	SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD	= 3,
	SERVER_HOOK_EVENT_ON_ENTER_WORLD		= 4,
	SERVER_HOOK_EVENT_ON_GUILD_JOIN			= 5,
	SERVER_HOOK_EVENT_ON_DEATH				= 6,
	SERVER_HOOK_EVENT_ON_REPOP				= 7,
	SERVER_HOOK_EVENT_ON_EMOTE				= 8,
	SERVER_HOOK_EVENT_ON_ENTER_COMBAT		= 9,
	SERVER_HOOK_EVENT_ON_CAST_SPELL			= 10,
	SERVER_HOOK_EVENT_ON_TICK				= 11,
	SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST		= 12,
	SERVER_HOOK_EVENT_ON_LOGOUT				= 13,
	SERVER_HOOK_EVENT_ON_QUEST_ACCEPT		= 14,
	SERVER_HOOK_EVENT_ON_ZONE				= 15,
	SERVER_HOOK_EVENT_ON_CHAT				= 16,
	SERVER_HOOK_EVENT_ON_GUILD_CREATE		= 17,
	SERVER_HOOK_EVENT_ON_ENTER_WORLD_2		= 18,
	SERVER_HOOK_EVENT_ON_CHARACTER_CREATE	= 19,
	SERVER_HOOK_EVENT_ON_QUEST_CANCELLED	= 20,
	SERVER_HOOK_EVENT_ON_QUEST_FINISHED		= 21,
	SERVER_HOOK_EVENT_ON_HONORABLE_KILL		= 22,
	SERVER_HOOK_EVENT_ON_ARENA_FINISH		= 23,
	SERVER_HOOK_EVENT_ON_CONTINENT_CREATE	= 25,
	SERVER_HOOK_EVENT_ON_POST_SPELL_CAST	= 26,
	SERVER_HOOK_EVENT_ON_AREATRIGGER		= 27,
	SERVER_HOOK_EVENT_ON_PLAYER_SAVE_TO_DB	= 28,
	SERVER_HOOK_EVENT_ON_AURA_REMOVE		= 29,

	NUM_SERVER_HOOKS,
};

enum ScriptTypes
{
	SCRIPT_TYPE_MISC						= 0x01,
	SCRIPT_TYPE_INFODUMPER					= 0x02,
	SCRIPT_TYPE_SCRIPT_ENGINE				= 0x20,
	SCRIPT_TYPE_SCRIPT_ENGINE_AS			= 0x22,
};


/* Hook typedefs */
typedef bool(*tOnNewCharacter)(uint32 Race, uint32 Class, WorldSession * Session, const char * Name);
typedef void(*tOnKillPlayer)(PlayerPointer pPlayer, PlayerPointer pVictim);
typedef void(*tOCharacterCreate)(PlayerPointer pPlayer);
typedef void(*tOnFirstEnterWorld)(PlayerPointer pPlayer);
typedef void(*tOnEnterWorld)(PlayerPointer pPlayer);
typedef void(*tOnGuildCreate)(PlayerPointer pLeader, Guild * pGuild);
typedef void(*tOnGuildJoin)(PlayerPointer pPlayer, Guild * pGuild);
typedef void(*tOnDeath)(PlayerPointer pPlayer);
typedef bool(*tOnRepop)(PlayerPointer pPlayer);
typedef void(*tOnEmote)(PlayerPointer pPlayer, uint32 Emote, UnitPointer pUnit);
typedef void(*tOnEnterCombat)(PlayerPointer pPlayer, UnitPointer pTarget);
typedef bool(*tOnCastSpell)(PlayerPointer pPlayer, SpellEntry * pSpell);
typedef void(*tOnTick)();
typedef bool(*tOnLogoutRequest)(PlayerPointer pPlayer);
typedef void(*tOnLogout)(PlayerPointer pPlayer);
typedef void(*tOnQuestAccept)(PlayerPointer pPlayer, Quest * pQuest, ObjectPointer pObject);
typedef void(*tOnZone)(PlayerPointer pPlayer, uint32 Zone, uint32 OldZone);
typedef bool(*tOnChat)(PlayerPointer pPlayer, uint32 Type, uint32 Lang, string Message, string Misc);
typedef bool(*ItemScript)(ItemPointer pItem, PlayerPointer pPlayer);
typedef void(*tOnQuestCancel)(PlayerPointer pPlayer, Quest * pQuest);
typedef void(*tOnQuestFinished)(PlayerPointer pPlayer, Quest * pQuest, ObjectPointer pObject);
typedef void(*tOnHonorableKill)(PlayerPointer pPlayer, PlayerPointer pKilled);
typedef void(*tOnArenaFinish)(PlayerPointer pPlayer, uint32 type, ArenaTeam * pTeam, bool victory, bool rated);
typedef void(*tOnContinentCreate)(MapMgrPointer mgr);
typedef void(*tOnPostSpellCast)(PlayerPointer pPlayer, SpellEntry * pSpell, UnitPointer pTarget);
typedef void(*tOnAreaTrigger)(PlayerPointer plr, uint32 areatrigger);
typedef void(*tOnPlayerSaveToDB)(PlayerPointer pPlayer, QueryBuffer* buf);
typedef void(*tOnAuraRemove)(PlayerPointer pPlayer, uint32 spellID);

class Spell;
class Aura;
class Creature;
class CreatureAIScript;
class GossipScript;
class GameObjectAIScript;
class InstanceScript;
class ScriptMgr;
struct ItemPrototype;
class QuestLogEntry;

/* Factory Imports (from script lib) */
typedef CreatureAIScript*(*exp_create_creature_ai)(CreaturePointer pCreature);
typedef GameObjectAIScript*(*exp_create_gameobject_ai)(GameObjectPointer pGameObject);
typedef InstanceScript* ( *exp_create_instance_ai )( MapMgrPointer pMapMgr );
typedef bool(*exp_handle_dummy_spell)(uint32 i, SpellPointer pSpell);
typedef bool(*exp_handle_dummy_aura)(uint32 i, AuraPointer pAura, bool apply);
typedef void(*exp_script_register)(ScriptMgr * mgr);
typedef uint32(*exp_get_script_type)();

typedef uint32(*exp_get_version)();

/* Hashmap typedefs */
typedef HM_NAMESPACE::hash_map<uint32, exp_create_creature_ai> CreatureCreateMap;
typedef HM_NAMESPACE::hash_map<uint32, exp_create_gameobject_ai> GameObjectCreateMap;
typedef HM_NAMESPACE::hash_map<uint32, exp_handle_dummy_aura> HandleDummyAuraMap;
typedef HM_NAMESPACE::hash_map<uint32, exp_handle_dummy_spell> HandleDummySpellMap;
typedef HM_NAMESPACE::hash_map< uint32, exp_create_instance_ai > InstanceCreateMap;
typedef set<GossipScript*> CustomGossipScripts;
typedef set<QuestScript*> QuestScripts;
typedef list<void*> ServerHookList;
typedef list<SCRIPT_MODULE> LibraryHandleMap;

class SERVER_DECL ScriptMgr : public Singleton<ScriptMgr>
{
public:

	ScriptMgr();
	~ScriptMgr();

	friend class HookInterface;

	void LoadScripts();
	void UnloadScripts();

	CreatureAIScript * CreateAIScriptClassForEntry(CreaturePointer pCreature);
	GameObjectAIScript * CreateAIScriptClassForGameObject(uint32 uEntryId, GameObjectPointer pGameObject);
	InstanceScript* CreateScriptClassForInstance( uint32 pMapId, MapMgrPointer pMapMgr );

	bool CallScriptedDummySpell(uint32 uSpellId, uint32 i, SpellPointer pSpell);
	bool CallScriptedDummyAura( uint32 uSpellId, uint32 i, AuraPointer  pAura, bool apply);
	bool CallScriptedItem(ItemPointer pItem, PlayerPointer pPlayer);

	void register_creature_script(uint32 entry, exp_create_creature_ai callback);
	void register_gameobject_script(uint32 entry, exp_create_gameobject_ai callback);
	void register_gossip_script(uint32 entry, GossipScript * gs);
	void register_go_gossip_script(uint32 entry, GossipScript * gs);
	void register_dummy_aura(uint32 entry, exp_handle_dummy_aura callback);
	void register_dummy_spell(uint32 entry, exp_handle_dummy_spell callback);
	void register_hook(ServerHookEvents event, void * function_pointer);
	void register_item_gossip_script(uint32 entry, GossipScript * gs);
	void register_quest_script(uint32 entry, QuestScript * qs);
	void register_instance_script( uint32 pMapId, exp_create_instance_ai pCallback );

	ASCENT_INLINE GossipScript * GetDefaultGossipScript() { return DefaultGossipScript; }

protected:

	InstanceCreateMap mInstances;
	CreatureCreateMap _creatures;
	GameObjectCreateMap _gameobjects;
	HandleDummyAuraMap _auras;
	HandleDummySpellMap _spells;
	LibraryHandleMap _handles;
	ServerHookList _hooks[NUM_SERVER_HOOKS];
	GossipScript * DefaultGossipScript;
	CustomGossipScripts _customgossipscripts;
	QuestScripts _questscripts;
};

class SERVER_DECL CreatureAIScript
{
public:
	CreatureAIScript(CreaturePointer creature);
	virtual ~CreatureAIScript() {};

	virtual void OnCombatStart(UnitPointer mTarget) {}
	virtual void OnCombatStop(UnitPointer mTarget) {}
	virtual void OnDamageTaken(UnitPointer mAttacker, float fAmount) {}
	virtual void OnCastSpell(uint32 iSpellId) {}
	virtual void OnTargetParried(UnitPointer mTarget) {}
	virtual void OnTargetDodged(UnitPointer mTarget) {}
	virtual void OnTargetBlocked(UnitPointer mTarget, int32 iAmount) {}
	virtual void OnTargetCritHit(UnitPointer mTarget, float fAmount) {}
	virtual void OnTargetDied(UnitPointer mTarget) {}
	virtual void OnParried(UnitPointer mTarget) {}
	virtual void OnDodged(UnitPointer mTarget) {}
	virtual void OnBlocked(UnitPointer mTarget, int32 iAmount) {}
	virtual void OnCritHit(UnitPointer mTarget, float fAmount) {}
	virtual void OnHit(UnitPointer mTarget, float fAmount) {}
	virtual void OnDied(UnitPointer mKiller) {}
	virtual void OnAssistTargetDied(UnitPointer mAssistTarget) {}
	virtual void OnFear(UnitPointer mFeared, uint32 iSpellId) {}
	virtual void OnFlee(UnitPointer mFlee) {}
	virtual void OnCallForHelp() {}
	virtual void OnLoad() {}
	virtual void OnReachWP(uint32 iWaypointId, bool bForwards) {}
	virtual void AIUpdate() {}
	virtual void OnEmote(PlayerPointer pPlayer, EmoteType Emote) {}
	virtual void StringFunctionCall(const char * pFunc) {}

	void RegisterAIUpdateEvent(uint32 frequency);
	void ModifyAIUpdateEvent(uint32 newfrequency);
	void RemoveAIUpdateEvent();

	virtual void Destroy() { delete this; }
	CreaturePointer GetUnit() { return _unit; }

protected:
	CreaturePointer _unit;
};

class SERVER_DECL GameObjectAIScript
{
public:
	GameObjectAIScript(GameObjectPointer goinstance);
	virtual ~GameObjectAIScript() {}

	virtual void OnCreate() {}
	virtual void OnSpawn() {}
	virtual void OnDespawn() {}
	virtual void OnActivate(PlayerPointer pPlayer) {}
	virtual void AIUpdate() {}
	virtual void Destroy() { delete this; }

	void RegisterAIUpdateEvent(uint32 frequency);
	void RemoveAIUpdateEvent();

protected:

	GameObjectPointer _gameobject;
};

class SERVER_DECL GossipScript
{
public:
	GossipScript();
	virtual ~GossipScript() {} 

	virtual void GossipHello(ObjectPointer pObject, PlayerPointer Plr, bool AutoSend);
	virtual void GossipSelectOption(ObjectPointer pObject, PlayerPointer Plr, uint32 Id, uint32 IntId, const char * EnteredCode);
	virtual void GossipEnd(ObjectPointer pObject, PlayerPointer Plr);
	virtual void Destroy();
};

class SERVER_DECL QuestScript
{
public:
	QuestScript() {};
	virtual ~QuestScript() {};

	virtual void OnQuestStart(PlayerPointer mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnQuestComplete(PlayerPointer mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnQuestCancel(PlayerPointer mTarget) {}
	virtual void OnGameObjectActivate(uint32 entry, PlayerPointer mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnCreatureKill(uint32 entry, PlayerPointer mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnExploreArea(uint32 areaId, PlayerPointer mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnPlayerItemPickup(uint32 itemId, uint32 totalCount, PlayerPointer mTarget, QuestLogEntry *qLogEntry) {}
};

/* * Class InstanceScript
   * Instanced class created for each instance of the map, holds all 
   * scriptable exports
*/

class SERVER_DECL InstanceScript
{
public:
	InstanceScript( MapMgrPointer pMapMgr );
	virtual ~InstanceScript() {};

	// Procedures that had been here before
	virtual GameObjectPointer	GetObjectForOpenLock( PlayerPointer pCaster, SpellPointer pSpell, SpellEntry* pSpellEntry ) { return NULLGOB; };
	virtual void				SetLockOptions( uint32 pEntryId, GameObjectPointer pGameObject ) {};
	virtual uint32				GetRespawnTimeForCreature( uint32 pEntryId, CreaturePointer pCreature) { return 240000; };

	// Player
	virtual void				OnPlayerDeath( PlayerPointer pVictim, UnitPointer pKiller ) {};

	// Area and AreaTrigger
	virtual void				OnPlayerEnter( PlayerPointer pPlayer ) {};
	virtual void				OnAreaTrigger( PlayerPointer pPlayer, uint32 pAreaId ) {};
	virtual void				OnZoneChange( PlayerPointer pPlayer, uint32 pNewZone, uint32 pOldZone ) {};

	// Data get / set - idea taken from ScriptDev2
	virtual void				SetInstanceData( uint32 pType, uint32 pIndex, uint32 pData ) {};
	virtual uint32				GetInstanceData( uint32 pType, uint32 pIndex ) { return 0; };

	// Creature / GameObject - part of it is simple reimplementation for easier use Creature / GO < --- > Script
	virtual void				OnCreatureDeath( CreaturePointer pVictim, UnitPointer pKiller ) {};
	virtual void				OnCreaturePushToWorld( CreaturePointer pCreature ) {};
	virtual void				OnGameObjectActivate( GameObjectPointer pGameObject, PlayerPointer pPlayer ) {};
	virtual void				OnGameObjectPushToWorld( GameObjectPointer pGameObject ) {};

	// Standard virtual methods
	virtual void				OnLoad() {};
	virtual void				UpdateEvent() {};
	virtual void				Destroy() {};

	// UpdateEvent
	void						RegisterUpdateEvent( uint32 pFrequency );
	void						ModifyUpdateEvent( uint32 pNewFrequency );
	void						RemoveUpdateEvent();

	// Something to return Instance's MapMgr
	MapMgrPointer				GetInstance() { return mInstance; };

protected:

	MapMgrPointer mInstance;
};

class SERVER_DECL HookInterface : public Singleton<HookInterface>
{
public:
	friend class ScriptMgr;

	bool OnNewCharacter(uint32 Race, uint32 Class, WorldSession * Session, const char * Name);
	void OnKillPlayer(PlayerPointer pPlayer, PlayerPointer pVictim);
	void OnFirstEnterWorld(PlayerPointer pPlayer);
	void OnEnterWorld(PlayerPointer pPlayer);
	void OnGuildCreate(PlayerPointer pLeader, Guild * pGuild);
	void OnGuildJoin(PlayerPointer pPlayer, Guild * pGuild);
	void OnDeath(PlayerPointer pPlayer);
	bool OnRepop(PlayerPointer pPlayer);
	void OnEmote(PlayerPointer pPlayer, uint32 Emote, UnitPointer pUnit);
	void OnEnterCombat(PlayerPointer pPlayer, UnitPointer pTarget);
	bool OnCastSpell(PlayerPointer pPlayer, SpellEntry * pSpell);
	bool OnLogoutRequest(PlayerPointer pPlayer);
	void OnLogout(PlayerPointer pPlayer);
	void OnQuestAccept(PlayerPointer pPlayer, Quest * pQuest, ObjectPointer pObject);
	void OnZone(PlayerPointer pPlayer, uint32 Zone, uint32 OldZone);
	bool OnChat(PlayerPointer pPlayer, uint32 Type, uint32 Lang, string Message, string Misc);
	void OnEnterWorld2(PlayerPointer pPlayer);
	void OnCharacterCreate(PlayerPointer pPlayer);
	void OnQuestCancelled(PlayerPointer pPlayer, Quest * pQuest);
	void OnQuestFinished(PlayerPointer pPlayer, Quest * pQuest, ObjectPointer pObject);
	void OnHonorableKill(PlayerPointer pPlayer, PlayerPointer pKilled);
	void OnArenaFinish(PlayerPointer pPlayer, uint32 type, ArenaTeam* pTeam, bool victory, bool rated);
	void OnContinentCreate(MapMgrPointer pMgr);
	void OnPostSpellCast(PlayerPointer pPlayer, SpellEntry * pSpell, UnitPointer pTarget);
	void OnAreaTrigger(PlayerPointer plr, uint32 areatrigger);
	void OnPlayerSaveToDB(PlayerPointer pPlayer, QueryBuffer* buf);
	void OnAuraRemove(PlayerPointer pPlayer, uint32 spellID);
};

#define sScriptMgr ScriptMgr::getSingleton()
#define sHookInterface HookInterface::getSingleton()

#endif
