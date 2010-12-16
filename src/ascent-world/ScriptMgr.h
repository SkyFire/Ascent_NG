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

#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

#define SCRIPT_MODULE void*
#define ADD_CREATURE_FACTORY_FUNCTION(cl) static CreatureAIScript * Create(Creature* c) { return new cl(c); }
#define ADD_INSTANCE_FACTORY_FUNCTION( ClassName ) static InstanceScript* Create( MapMgr* pMapMgr ) { return new ClassName( pMapMgr ); };

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
	SERVER_HOOK_EVENT_ON_DESTROY_BUILDING	= 30,
	SERVER_HOOK_EVENT_ON_DAMAGE_BUILDING	= 31,
	SERVER_HOOK_EVENT_ON_MOUNT_FLYING		= 32,
	SERVER_HOOK_EVENT_ON_PRE_AURA_REMOVE	= 33,
	SERVER_HOOK_EVENT_ON_SLOW_LOCK_OPEN		= 34,

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
typedef void(*tOnKillPlayer)(Player* pPlayer, Player* pVictim);
typedef void(*tOCharacterCreate)(Player* pPlayer);
typedef void(*tOnFirstEnterWorld)(Player* pPlayer);
typedef void(*tOnEnterWorld)(Player* pPlayer);
typedef void(*tOnGuildCreate)(Player* pLeader, Guild * pGuild);
typedef void(*tOnGuildJoin)(Player* pPlayer, Guild * pGuild);
typedef void(*tOnDeath)(Player* pPlayer);
typedef bool(*tOnRepop)(Player* pPlayer);
typedef void(*tOnEmote)(Player* pPlayer, uint32 Emote, Unit* pUnit);
typedef void(*tOnEnterCombat)(Player* pPlayer, Unit* pTarget);
typedef bool(*tOnCastSpell)(Player* pPlayer, SpellEntry * pSpell);
typedef void(*tOnTick)();
typedef bool(*tOnLogoutRequest)(Player* pPlayer);
typedef void(*tOnLogout)(Player* pPlayer);
typedef void(*tOnQuestAccept)(Player* pPlayer, Quest * pQuest, Object* pObject);
typedef void(*tOnZone)(Player* pPlayer, uint32 Zone, uint32 OldZone);
typedef bool(*tOnChat)(Player* pPlayer, uint32 Type, uint32 Lang, string Message, string Misc);
typedef bool(*ItemScript)(Item* pItem, Player* pPlayer);
typedef void(*tOnQuestCancel)(Player* pPlayer, Quest * pQuest);
typedef void(*tOnQuestFinished)(Player* pPlayer, Quest * pQuest, Object* pObject);
typedef void(*tOnHonorableKill)(Player* pPlayer, Player* pKilled);
typedef void(*tOnArenaFinish)(Player* pPlayer, uint32 type, ArenaTeam * pTeam, bool victory, bool rated);
typedef void(*tOnContinentCreate)(MapMgr* mgr);
typedef void(*tOnPostSpellCast)(Player* pPlayer, SpellEntry * pSpell, Unit* pTarget);
typedef void(*tOnAreaTrigger)(Player* plr, uint32 areatrigger);
typedef void(*tOnPlayerSaveToDB)(Player* pPlayer, QueryBuffer* buf);
typedef void(*tOnAuraRemove)(Player* pPlayer, uint32 spellID);
typedef void(*tOnDestroyBuilding)(GameObject* go);
typedef void(*tOnDamageBuilding)(GameObject* go);
typedef bool(*tOnMountFlying) (Player* plr);
typedef bool(*tOnPreAuraRemove)(Player* plr,uint32 spellID);
typedef void(*tOnSlowLockOpen)(GameObject* go,Player* plr);

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
typedef CreatureAIScript*(*exp_create_creature_ai)(Creature* pCreature);
typedef GameObjectAIScript*(*exp_create_gameobject_ai)(GameObject* pGameObject);
typedef InstanceScript* ( *exp_create_instance_ai )( MapMgr* pMapMgr );
typedef bool(*exp_handle_dummy_spell)(uint32 i, Spell* pSpell);
typedef bool(*exp_handle_dummy_aura)(uint32 i, Aura* pAura, bool apply);
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

	CreatureAIScript * CreateAIScriptClassForEntry(Creature* pCreature);
	GameObjectAIScript * CreateAIScriptClassForGameObject(uint32 uEntryId, GameObject* pGameObject);
	InstanceScript* CreateScriptClassForInstance( uint32 pMapId, MapMgr* pMapMgr );

	bool CallScriptedDummySpell(uint32 uSpellId, uint32 i, Spell* pSpell);
	bool CallScriptedDummyAura( uint32 uSpellId, uint32 i, Aura*  pAura, bool apply);
	bool CallScriptedItem(Item* pItem, Player* pPlayer);

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
	CreatureAIScript(Creature* creature);
	virtual ~CreatureAIScript() {};

	virtual void OnCombatStart(Unit* mTarget) {}
	virtual void OnCombatStop(Unit* mTarget) {}
	virtual void OnDamageTaken(Unit* mAttacker, float fAmount) {}
	virtual void OnCastSpell(uint32 iSpellId) {}
	virtual void OnTargetParried(Unit* mTarget) {}
	virtual void OnTargetDodged(Unit* mTarget) {}
	virtual void OnTargetBlocked(Unit* mTarget, int32 iAmount) {}
	virtual void OnTargetCritHit(Unit* mTarget, float fAmount) {}
	virtual void OnTargetDied(Unit* mTarget) {}
	virtual void OnParried(Unit* mTarget) {}
	virtual void OnDodged(Unit* mTarget) {}
	virtual void OnBlocked(Unit* mTarget, int32 iAmount) {}
	virtual void OnCritHit(Unit* mTarget, float fAmount) {}
	virtual void OnHit(Unit* mTarget, float fAmount) {}
	virtual void OnDied(Unit* mKiller) {}
	virtual void OnAssistTargetDied(Unit* mAssistTarget) {}
	virtual void OnFear(Unit* mFeared, uint32 iSpellId) {}
	virtual void OnFlee(Unit* mFlee) {}
	virtual void OnCallForHelp() {}
	virtual void OnLoad() {}
	virtual void OnReachWP(uint32 iWaypointId, bool bForwards) {}
	virtual void AIUpdate() {}
	virtual void OnEmote(Player* pPlayer, EmoteType Emote) {}
	virtual void StringFunctionCall(const char * pFunc) {}

	void RegisterAIUpdateEvent(uint32 frequency);
	void ModifyAIUpdateEvent(uint32 newfrequency);
	void RemoveAIUpdateEvent();

	virtual void Destroy() { delete this; }
	Creature* GetUnit() { return _unit; }

protected:
	Creature* _unit;
};

class SERVER_DECL GameObjectAIScript
{
public:
	GameObjectAIScript(GameObject* goinstance);
	virtual ~GameObjectAIScript() {}

	virtual void OnCreate() {}
	virtual void OnSpawn() {}
	virtual void OnDespawn() {}
	virtual void OnActivate(Player* pPlayer) {}
	virtual void AIUpdate() {}
	virtual void Destroy() { delete this; }

	void RegisterAIUpdateEvent(uint32 frequency);
	void RemoveAIUpdateEvent();

protected:

	GameObject* _gameobject;
};

class SERVER_DECL GossipScript
{
public:
	GossipScript();
	virtual ~GossipScript() {} 

	virtual void GossipHello(Object* pObject, Player* Plr, bool AutoSend);
	virtual void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * EnteredCode);
	virtual void GossipEnd(Object* pObject, Player* Plr);
	virtual void Destroy();
};

class SERVER_DECL QuestScript
{
public:
	QuestScript() {};
	virtual ~QuestScript() {};

	virtual void OnQuestStart(Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnQuestCancel(Player* mTarget) {}
	virtual void OnGameObjectActivate(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnCreatureKill(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnExploreArea(uint32 areaId, Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnPlayerItemPickup(uint32 itemId, uint32 totalCount, Player* mTarget, QuestLogEntry *qLogEntry) {}
};

/* * Class InstanceScript
   * Instanced class created for each instance of the map, holds all 
   * scriptable exports
*/

class SERVER_DECL InstanceScript
{
public:
	InstanceScript( MapMgr* pMapMgr );
	virtual ~InstanceScript() {};

	// Procedures that had been here before
	virtual GameObject*	GetObjectForOpenLock( Player* pCaster, Spell* pSpell, SpellEntry* pSpellEntry ) { return NULL; };
	virtual void				SetLockOptions( uint32 pEntryId, GameObject* pGameObject ) {};
	virtual uint32				GetRespawnTimeForCreature( uint32 pEntryId, Creature* pCreature) { return 240000; };

	// Player
	virtual void				OnPlayerDeath( Player* pVictim, Unit* pKiller ) {};

	// Area and AreaTrigger
	virtual void				OnPlayerEnter( Player* pPlayer ) {};
	virtual void				OnAreaTrigger( Player* pPlayer, uint32 pAreaId ) {};
	virtual void				OnZoneChange( Player* pPlayer, uint32 pNewZone, uint32 pOldZone ) {};

	// Data get / set - idea taken from ScriptDev2
	virtual void				SetInstanceData( uint32 pType, uint32 pIndex, uint32 pData ) {};
	virtual uint32				GetInstanceData( uint32 pType, uint32 pIndex ) { return 0; };

	// Creature / GameObject - part of it is simple reimplementation for easier use Creature / GO < --- > Script
	virtual void				OnCreatureDeath( Creature* pVictim, Unit* pKiller ) {};
	virtual void				OnCreaturePushToWorld( Creature* pCreature ) {};
	virtual void				OnGameObjectActivate( GameObject* pGameObject, Player* pPlayer ) {};
	virtual void				OnGameObjectPushToWorld( GameObject* pGameObject ) {};

	// Standard virtual methods
	virtual void				OnLoad() {};
	virtual void				UpdateEvent() {};
	virtual void				Destroy() {};

	// UpdateEvent
	void						RegisterUpdateEvent( uint32 pFrequency );
	void						ModifyUpdateEvent( uint32 pNewFrequency );
	void						RemoveUpdateEvent();

	// Something to return Instance's MapMgr
	MapMgr*				GetInstance() { return mInstance; };

protected:

	MapMgr* mInstance;
};

class SERVER_DECL HookInterface : public Singleton<HookInterface>
{
public:
	friend class ScriptMgr;

	bool OnNewCharacter(uint32 Race, uint32 Class, WorldSession * Session, const char * Name);
	void OnKillPlayer(Player* pPlayer, Player* pVictim);
	void OnFirstEnterWorld(Player* pPlayer);
	void OnEnterWorld(Player* pPlayer);
	void OnGuildCreate(Player* pLeader, Guild * pGuild);
	void OnGuildJoin(Player* pPlayer, Guild * pGuild);
	void OnDeath(Player* pPlayer);
	bool OnRepop(Player* pPlayer);
	void OnEmote(Player* pPlayer, uint32 Emote, Unit* pUnit);
	void OnEnterCombat(Player* pPlayer, Unit* pTarget);
	bool OnCastSpell(Player* pPlayer, SpellEntry * pSpell);
	bool OnLogoutRequest(Player* pPlayer);
	void OnLogout(Player* pPlayer);
	void OnQuestAccept(Player* pPlayer, Quest * pQuest, Object* pObject);
	void OnZone(Player* pPlayer, uint32 Zone, uint32 OldZone);
	bool OnChat(Player* pPlayer, uint32 Type, uint32 Lang, string Message, string Misc);
	void OnEnterWorld2(Player* pPlayer);
	void OnCharacterCreate(Player* pPlayer);
	void OnQuestCancelled(Player* pPlayer, Quest * pQuest);
	void OnQuestFinished(Player* pPlayer, Quest * pQuest, Object* pObject);
	void OnHonorableKill(Player* pPlayer, Player* pKilled);
	void OnArenaFinish(Player* pPlayer, uint32 type, ArenaTeam* pTeam, bool victory, bool rated);
	void OnContinentCreate(MapMgr* pMgr);
	void OnPostSpellCast(Player* pPlayer, SpellEntry * pSpell, Unit* pTarget);
	void OnAreaTrigger(Player* plr, uint32 areatrigger);
	void OnPlayerSaveToDB(Player* pPlayer, QueryBuffer* buf);
	void OnAuraRemove(Player* pPlayer, uint32 spellID);
	void OnDestroyBuilding(GameObject* go);
	void OnDamageBuilding(GameObject* go);
	bool OnMountFlying(Player* plr);
	bool OnPreAuraRemove(Player* remover,uint32 spellID);
	void OnSlowLockOpen(GameObject* go,Player* plr);
};

#define sScriptMgr ScriptMgr::getSingleton()
#define sHookInterface HookInterface::getSingleton()

#endif
