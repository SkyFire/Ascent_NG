#include "StdAfx.h"
#include "LUAEngine.h"
#include "LUAFunctions.h"
#include <ScriptSetup.h>

#if PLATFORM != PLATFORM_WIN32
#include <dirent.h>
#endif

bool lua_is_starting_up = false;
ScriptMgr * m_scriptMgr;
LuaEngineMgr g_luaMgr;
LuaEngine * g_engine;

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_SCRIPT_ENGINE | SCRIPT_TYPE_SCRIPT_ENGINE_LUA;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	m_scriptMgr = mgr;
	g_luaMgr.Startup();
}

template<typename T> const char * GetTClassName() { return "UNKNOWN"; }

template<>
const char * GetTClassName<Unit>()
{
	return "Unit";
}

template<>
const char * GetTClassName<GameObject>()
{
	return "GameObject";
}

template<typename T>
struct RegType
{
	const char * name;
	int(*mfunc)(lua_State*,T*);
};


/************************************************************************/
/* SCRIPT FUNCTION TABLES                                               */
/************************************************************************/
RegType<Unit> UnitMethods[] = {
	{ "GossipCreateMenu", &luaUnit_GossipCreateMenu },
	{ "GossipMenuAddItem", &luaUnit_GossipMenuAddItem },
	{ "GossipSendMenu", &luaUnit_GossipSendMenu },
	{ "GossipComplete", &luaUnit_GossipComplete },
	{ "GossipSendPOI", &luaUnit_GossipSendPOI },
	{ "GetName", &luaUnit_GetName },
	{ "SendChatMessage", &luaUnit_SendChatMessage },
	{ "MoveTo", &luaUnit_MoveTo },
	{ "SetMovementType", &luaUnit_SetMovementType },
	{ "CastSpell", &luaUnit_CastSpell },
	{ "FullCastSpell", &luaUnit_FullCastSpell },
	{ "CastSpellOnTarget", &luaUnit_CastSpellOnTarget },
	{ "FullCastSpellOnTarget", &luaUnit_FullCastSpellOnTarget },
	{ "SpawnCreature", &luaUnit_SpawnCreature },
	{ "SpawnGameObject", &luaUnit_SpawnGameObject },
	{ "GetX", &luaUnit_GetX },
	{ "GetY", &luaUnit_GetY },
	{ "GetZ", &luaUnit_GetZ },
	{ "GetO", &luaUnit_GetO },
	{ "IsPlayer", &luaUnit_IsPlayer },
	{ "IsCreature", &luaUnit_IsCreature },
	{ "RegisterEvent", &luaUnit_RegisterEvent },
	{ "RemoveEvents", &luaUnit_RemoveEvents },
	{ "SendBroadcastMessage", &luaUnit_SendBroadcastMessage },
	{ "SendAreaTriggerMessage", &luaUnit_SendAreaTriggerMessage },
	{ "KnockBack", &luaUnit_KnockBack },
	{ "MarkQuestObjectiveAsComplete", &luaUnit_MarkQuestObjectiveAsComplete },
	{ "LearnSpell", &luaUnit_LearnSpell },
	{ "UnlearnSpell", &luaUnit_UnlearnSpell },
	{ "HasFinishedQuest", &luaUnit_HasFinishedQuest },
	{ "GetItemCount", &luaUnit_GetItemCount },
	{ "IsInCombat", &luaUnit_IsInCombat },
	{ "GetMainTank", &luaUnit_GetMainTank },
	{ "GetAddTank", &luaUnit_GetAddTank },
	{ "ClearThreatList", &luaUnit_ClearThreatList },
	{ "GetTauntedBy", &luaUnit_GetTauntedBy },
	{ "SetTauntedBy", &luaUnit_SetTauntedBy },
	{ "SetSoulLinkedWith", &luaUnit_SetSoulLinkedWith },
	{ "GetSoulLinkedWith", &luaUnit_GetSoulLinkedWith },
	{ "ChangeTarget", &luaUnit_ChangeTarget },
	{ "GetHealthPct", &luaUnit_GetHealthPct },
	{ "SetHealthPct", &luaUnit_SetHealthPct },
	{ "GetManaPct", &luaUnit_GetManaPct },
	{ "Despawn", &luaUnit_Despawn },
	{ "GetUnitBySqlId", &luaUnit_GetUnitBySqlId },
	{ "PlaySoundToSet", &luaUnit_PlaySoundToSet },
	{ "RemoveAura", &luaUnit_RemoveAura },
	{ "StopMovement", &luaUnit_StopMovement },
	{ "Emote", &luaUnit_Emote },
	{ "GetInstanceID", &luaUnit_GetInstanceID },
	{ "GetClosestPlayer", &luaUnit_GetClosestPlayer },
	{ "GetRandomPlayer", &luaUnit_GetRandomPlayer },
	{ "GetRandomFriend", &luaUnit_GetRandomFriend },
	{ "AddItem", &luaUnit_AddItem },
	{ "RemoveItem", &luaUnit_RemoveItem },
	{ "CreateCustomWaypointMap", &luaUnit_CreateCustomWaypointMap },
	{ "CreateWaypoint", &luaUnit_CreateWaypoint },
	{ "MoveToWaypoint", &luaUnit_MoveToWaypoint },
	{ "DestroyCustomWaypointMap", &luaUnit_DestroyCustomWaypointMap },
	{ "SetCombatCapable", &luaUnit_SetCombatCapable },
	{ "SetCombatMeleeCapable", &luaUnit_SetCombatMeleeCapable },
	{ "SetCombatRangedCapable", &luaUnit_SetCombatRangedCapable },
	{ "SetCombatSpellCapable", &luaUnit_SetCombatSpellCapable },
	{ "SetCombatTargetingCapable", &luaUnit_SetCombatTargetingCapable },
	{ "SetNPCFlags", &luaUnit_SetNPCFlags },
	{ "SetModel", &luaUnit_SetModel },
	{ "SetScale", &luaUnit_SetScale },
	{ "SetFaction", &luaUnit_SetFaction },
	{ "SetStandState",&luaUnit_SetStandState },
	{ "Teleport" , &luaUnit_TeleportUnit },
	{ "GetPlayerClass", &luaUnit_GetPlayerClass },
	{ "ClearThreatList", &luaUnit_ClearHateList },
	{ "WipeThreatList", &luaUnit_WipeHateList },
	{ "WipeTargetList", &luaUnit_WipeTargetList },
	{ "WipeCurrentTarget", &luaUnit_WipeCurrentTarget },
	{ "GetHealth", &luaUnit_GetHealth },
	{ "GetMaxHealth", &luaUnit_GetMaxHealth },
	{ "SetHealth", &luaUnit_SetHealth },
	{ "SetMaxHealth", &luaUnit_SetMaxHealth },
	{ "HasAura", &luaUnit_HasAura },
	{ "Land", &luaUnit_Land },
	{ "SetFlying", &luaUnit_SetFlying },
	{ "SetMana", &luaUnit_SetMana },
	{ "SetMaxMana", &luaUnit_SetMaxMana },
	{ "GetDistance", &luaUnit_GetDistance },
	{ "GetGUID", &luaUnit_GetGUID },
	{ "GetCreatureNearestCoords", &luaUnit_GetCreatureNearestCoords },
    { "CastSpellAoF", &luaUnit_CastSpellAoF },
	{ "GetGameObjectNearestCoords", &luaUnit_GetGameObjectNearestCoords },
	{ "SetInFront", &luaUnit_SetInFront },
	{ "RemoveAllAuras", &luaUnit_RemoveAllAuras },
	{ "ReturnToSpawnPoint", &luaUnit_ReturnToSpawnPoint },
	{ "CancelSpell", &luaUnit_CancelSpell },
	{ "IsAlive", &luaUnit_IsAlive },
	{ "IsDead", &luaUnit_IsDead },
	{ "IsInWorld", &luaUnit_IsInWorld },
	{ "GetZoneId", &luaUnit_GetZoneId },
	{ "GetMana", &luaUnit_GetMana },
	{ "GetMaxMana", &luaUnit_GetMaxMana },
	{ "Root", &luaUnit_Root },
	{ "Unroot", &luaUnit_Unroot },
	{ "IsCreatureMoving", &luaUnit_IsCreatureMoving },
	{ "SetOutOfCombatRange", &luaUnit_SetOutOfCombatRange },
	{ "ModifyRunSpeed", &luaUnit_ModifyRunSpeed },
	{ "ModifyWalkSpeed", &luaUnit_ModifyWalkSpeed },
	{ "ModifyFlySpeed" , &luaUnit_ModifyFlySpeed },
	{ "GetCurrentSpell", &luaUnit_GetCurrentSpell },
	{ "IsFlying", &luaUnit_IsFlying },
	{ "SetRotation", &luaUnit_SetRotation },
	{ "SetOrientation", &luaUnit_SetOrientation },
	{ "GetSpawnX", &luaUnit_GetSpawnX },
	{ "GetSpawnY", &luaUnit_GetSpawnY },
	{ "GetSpawnZ", &luaUnit_GetSpawnZ },
	{ "GetSpawnO", &luaUnit_GetSpawnO },
	{ "GetInRangePlayersCount", &luaUnit_GetInRangePlayersCount },
	{ "GetEntry", &luaUnit_GetEntry },
	{ "SetMoveRunFlag", &luaUnit_SetMoveRunFlag },
	{ "GetAIState", &luaUnit_GetAIState },
	{ "ModUInt32Value", &luaUnit_ModUInt32Value },
	{ "ModFloatValue", &luaUnit_ModFloatValue },
	{ "SetUInt32Value", &luaUnit_SetUInt32Value },
	{ "SetUInt64Value", &luaUnit_SetUInt64Value },
	{ "SetFloatValue", &luaUnit_SetFloatValue },
	{ "GetUInt32Value", &luaUnit_GetUInt32Value },
	{ "GetUInt64Value", &luaUnit_GetUInt64Value },
	{ "GetFloatValue", &luaUnit_GetFloatValue },
	{ "InitPacket", &luaUnit_InitPacket },
	{ "AddDataToPacket", &luaUnit_AddDataToPacket },
	{ "AddGuidDataToPacket", &luaUnit_AddGuidDataToPacket },
	{ "SendData", &luaUnit_SendData },
	{ "AdvanceQuestObjective", &luaUnit_AdvanceQuestObjective },
	{ "Heal", &luaUnit_Heal },
	{ "Energize", &luaUnit_Energize },
	{ "SendChatMessageAlternateEntry", &luaUnit_SendChatMessageAlternateEntry },
	{ "SendChatMessageToPlayer", &luaUnit_SendChatMessageToPlayer },
	{ "SetPowerType", &luaUnit_SetPowerType },
	{ "Strike", &luaUnit_Strike },
	{ "SetAttackTimer", &luaUnit_SetAttackTimer },
	{ "Kill", &luaUnit_Kill },
	{ "DealDamage", &luaUnit_DealDamage },
	{ "IsInFront", &luaUnit_IsInFront },
	{ "IsInBack", &luaUnit_IsInBack },
	{ "IsPacified", &luaUnit_IsPacified },
	{ "IsStunned", &luaUnit_IsStunned },
	{ "IsFeared", &luaUnit_IsFeared },
	{ "CreateGuardian", &luaUnit_CreateGuardian },
	{ "HandleEvent", &luaUnit_HandleEvent },
	{ "HasInRangeObjects", &luaUnit_HasInRangeObjects },
	{ "IsInWater", &luaUnit_IsInWater },
	//{ "IsInArc", &luaUnit_IsInArc },
	{ "GetInRangeObjects", &luaUnit_GetInRangeGameObjects },
	{ "GetInRangeObjectsCount", &luaUnit_GetInRangeObjectsCount },
	{ "GetAITargetsCount", &luaUnit_GetAITargetsCount },
	{ "SetUnitToFollow", &luaUnit_SetUnitToFollow },
	{ "GetUnitToFollow", &luaUnit_GetUnitToFollow },
	{ "DismissPet", &luaUnit_DismissPet },
	{ "IsPet", &luaUnit_IsPet },
	{ "SetNextTarget", &luaUnit_SetNextTarget },
	{ "GetNextTarget", &luaUnit_GetNextTarget },
	{ "SetPetOwner", &luaUnit_SetPetOwner },
	{ "GetPetOwner", &luaUnit_GetPetOwner },
	{ "CalcToDistance", &luaUnit_CalcToDistance },
	{ "CalcAngle", &luaUnit_CalcAngle },
	{ "CalcRadAngle", &luaUnit_CalcRadAngle },
	{ "SetFacing", &luaUnit_SetFacing },
	{ "SetDeathState", &luaUnit_SetDeathState },
	{ "IsInvisible", &luaUnit_IsInvisible },
	{ "IsInvincible", &luaUnit_IsInvincible },
	{ "ResurrectPlayer", &luaUnit_ResurrectPlayer },
	{ "KickPlayer", &luaUnit_KickPlayer },
	{ "CanCallForHelp", &luaUnit_CanCallForHelp },
	{ "CallForHelpHp", &luaUnit_CallForHelpHp },
	{ "SetCreatureNameById", &luaUnit_SetCreatureName },
	//{ "GetAITargets", &luaUnit_GetAITargets },
	{ "GetInRangePlayers", &luaUnit_GetInRangePlayers },
	{ "GetUnitByGUID", &luaUnit_GetUnitByGUID },
	{ "RemoveFromWorld", &luaUnit_RemoveFromWorld },
	{ "GetFaction", &luaUnit_GetFaction },
	{ "EnableMoveFly", &luaUnit_MoveFly },
	{ "SpellNonMeleeDamageLog", &luaUnit_SpellNonMeleeDamageLog },
	{ "DisableRespawn", &luaUnit_NoRespawn },
	{ "ModThreat", &luaUnit_ModThreat },
	{ "GetThreat", &luaUnit_GetThreatByPtr },
	{ "GetInRangeFriends", &luaUnit_GetInRangeFriends },
	{ "GetPowerType", &luaUnit_GetPowerType },
	{ "GetMapId", &luaUnit_GetMapId },
	{ "CalcDistance" , &luaUnit_CalcDistance },
	{ "AttackReaction", &luaUnit_AttackReaction },
	{ "EventCastSpell", &luaUnit_EventCastSpell },
	{ "IsPlayerMoving", &luaUnit_IsPlayerMoving },
	{ "IsPlayerAttacking", &luaUnit_IsPlayerAttacking },
	//{ "IsPlayerAtWar", &luaUnit_IsPlayerAtWar },
	{ "RemoveThreat", &luaUnit_RemoveThreatByPtr },
	{ "SetPlayerAtWar", &luaUnit_SetPlayerAtWar },
	{ "GetFactionStanding", &luaUnit_GetFactionStanding },
	{ "PlaySpellVisual", &luaUnit_PlaySpellVisual },
	{ "GetPlayerLevel", &luaUnit_GetPlayerLevel },
	{ "SetPlayerLevel", &luaUnit_SetPlayerLevel },
	{ "SetStanding", &luaUnit_SetPlayerStanding },
	{ "GetStanding", &luaUnit_GetStanding },
	{ "HasItem", &luaUnit_HasItem },
	{ "AdvanceSkill", &luaUnit_AdvanceSkill },
	{ "AddSkill", &luaUnit_AddSkill },
	{ "RemoveSkill", &luaUnit_RemoveSkill },
	{ "EnableFlyCheat", &luaUnit_FlyCheat },
	{ "AddAssistTarget", &luaUnit_AddAssistTargets },
	{ "GetCurrentSpellId", &luaUnit_GetCurrentSpellId },
	{ "GetPlayerRace", &luaUnit_GetPlayerRace },
	{ "RemoveAurasByMechanic", &luaUnit_RemoveAurasByMechanic },
	{ "RemoveAurasType", &luaUnit_RemoveAurasType },
	{ "AddAuraVisual", &luaUnit_AddAuraVisual },

	{ NULL, NULL },
};


RegType<GameObject> GOMethods[] = {
	{ "GetName", &luaGameObject_GetName },
	{ "Teleport" , &luaGameObject_Teleport },
	{ NULL, NULL },
};

template<typename T> RegType<T>* GetMethodTable() { return NULL; }
template<>
RegType<Unit>* GetMethodTable<Unit>() { return UnitMethods; }

template<>
RegType<GameObject>* GetMethodTable<GameObject>() { return GOMethods; }

template <typename T> class Lunar {
	typedef struct { T *pT; } userdataType;
public:
	typedef int (*mfp)(lua_State *L, T* ptr);
	typedef struct { const char *name; mfp mfunc; } RegType;

	static void Register(lua_State *L) {
		lua_newtable(L);
		int methods = lua_gettop(L);

		luaL_newmetatable(L, GetTClassName<T>());
		int metatable = lua_gettop(L);

		// store method table in globals so that
		// scripts can add functions written in Lua.
		lua_pushvalue(L, methods);
		set(L, LUA_GLOBALSINDEX, GetTClassName<T>());

		// hide metatable from Lua getmetatable()
		lua_pushvalue(L, methods);
		set(L, metatable, "__metatable");

		lua_pushvalue(L, methods);
		set(L, metatable, "__index");

		lua_pushcfunction(L, tostring_T);
		set(L, metatable, "__tostring");

		lua_pushcfunction(L, gc_T);
		set(L, metatable, "__gc");

		lua_newtable(L);                // mt for method table
		lua_pushcfunction(L, new_T);
		lua_pushvalue(L, -1);           // dup new_T function
		set(L, methods, "new");         // add new_T to method table
		set(L, -3, "__call");           // mt.__call = new_T
		lua_setmetatable(L, methods);

		// fill method table with methods from class T
		for (RegType *l = ((RegType*)GetMethodTable<T>()); l->name; l++) {
			lua_pushstring(L, l->name);
			lua_pushlightuserdata(L, (void*)l);
			lua_pushcclosure(L, thunk, 1);
			lua_settable(L, methods);
		}

		lua_pop(L, 2);  // drop metatable and method table
	}

	// call named lua method from userdata method table
	static int call(lua_State *L, const char *method,
		int nargs=0, int nresults=LUA_MULTRET, int errfunc=0)
	{
		int base = lua_gettop(L) - nargs;  // userdata index
		if (!luaL_checkudata(L, base, T::className)) {
			lua_settop(L, base-1);           // drop userdata and args
			lua_pushfstring(L, "not a valid %s userdata", T::className);
			return -1;
		}

		lua_pushstring(L, method);         // method name
		lua_gettable(L, base);             // get method from userdata
		if (lua_isnil(L, -1)) {            // no method?
			lua_settop(L, base-1);           // drop userdata and args
			lua_pushfstring(L, "%s missing method '%s'", T::className, method);
			return -1;
		}
		lua_insert(L, base);               // put method under userdata, args

		int status = lua_pcall(L, 1+nargs, nresults, errfunc);  // call method
		if (status) {
			const char *msg = lua_tostring(L, -1);
			if (msg == NULL) msg = "(error with no message)";
			lua_pushfstring(L, "%s:%s status = %d\n%s",
				T::className, method, status, msg);
			lua_remove(L, base);             // remove old message
			return -1;
		}
		return lua_gettop(L) - base + 1;   // number of results
	}

	// push onto the Lua stack a userdata containing a pointer to T object
	static int push(lua_State *L, T *obj, bool gc=false) {
		if (!obj) { lua_pushnil(L); return 0; }
		luaL_getmetatable(L, GetTClassName<T>());  // lookup metatable in Lua registry
		if (lua_isnil(L, -1)) luaL_error(L, "%s missing metatable", GetTClassName<T>());
		int mt = lua_gettop(L);
		subtable(L, mt, "userdata", "v");
		userdataType *ud =
			static_cast<userdataType*>(pushuserdata(L, obj, sizeof(userdataType)));
		if (ud) {
			ud->pT = obj;  // store pointer to object in userdata
			lua_pushvalue(L, mt);
			lua_setmetatable(L, -2);
			if (gc == false) {
				lua_checkstack(L, 3);
				subtable(L, mt, "do not trash", "k");
				lua_pushvalue(L, -2);
				lua_pushboolean(L, 1);
				lua_settable(L, -3);
				lua_pop(L, 1);
			}
		}
		lua_replace(L, mt);
		lua_settop(L, mt);
		return mt;  // index of userdata containing pointer to T object
	}

	// get userdata from Lua stack and return pointer to T object
	static T *check(lua_State *L, int narg) {
		userdataType *ud =
			static_cast<userdataType*>(luaL_checkudata(L, narg, GetTClassName<T>()));
		if(!ud) { luaL_typerror(L, narg, GetTClassName<T>()); return NULL; }
		return ud->pT;  // pointer to T object
	}

private:
	Lunar();  // hide default constructor

	// member function dispatcher
	static int thunk(lua_State *L) {
		// stack has userdata, followed by method args
		T *obj = check(L, 1);  // get 'self', or if you prefer, 'this'
		lua_remove(L, 1);  // remove self so member function args start at index 1
		// get member function from upvalue
		RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));
		//return (obj->*(l->mfunc))(L);  // call member function
		return l->mfunc(L,obj);
	}

	// create a new T object and
	// push onto the Lua stack a userdata containing a pointer to T object
	static int new_T(lua_State *L) {
		lua_remove(L, 1);   // use classname:new(), instead of classname.new()
		T *obj = NULL/*new T(L)*/;  // call constructor for T objects
		assert(false);
		push(L, obj, true); // gc_T will delete this object
		return 1;           // userdata containing pointer to T object
	}

	// garbage collection metamethod
	static int gc_T(lua_State *L) {
		if (luaL_getmetafield(L, 1, "do not trash")) {
			lua_pushvalue(L, 1);  // dup userdata
			lua_gettable(L, -2);
			if (!lua_isnil(L, -1)) return 0;  // do not delete object
		}
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
		T *obj = ud->pT;
		if (obj) delete obj;  // call destructor for T objects
		return 0;
	}

	static int tostring_T (lua_State *L) {
		char buff[32];
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
		T *obj = ud->pT;
		sprintf(buff, "%p", obj);
		lua_pushfstring(L, "%s (%s)", GetTClassName<T>(), buff);
		return 1;
	}

	static void set(lua_State *L, int table_index, const char *key) {
		lua_pushstring(L, key);
		lua_insert(L, -2);  // swap value and key
		lua_settable(L, table_index);
	}

	static void weaktable(lua_State *L, const char *mode) {
		lua_newtable(L);
		lua_pushvalue(L, -1);  // table is its own metatable
		lua_setmetatable(L, -2);
		lua_pushliteral(L, "__mode");
		lua_pushstring(L, mode);
		lua_settable(L, -3);   // metatable.__mode = mode
	}

	static void subtable(lua_State *L, int tindex, const char *name, const char *mode) {
		lua_pushstring(L, name);
		lua_gettable(L, tindex);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_checkstack(L, 3);
			weaktable(L, mode);
			lua_pushstring(L, name);
			lua_pushvalue(L, -2);
			lua_settable(L, tindex);
		}
	}

	static void *pushuserdata(lua_State *L, void *key, size_t sz) {
		void *ud = 0;
		lua_pushlightuserdata(L, key);
		lua_gettable(L, -2);     // lookup[key]
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);         // drop nil
			lua_checkstack(L, 3);
			ud = lua_newuserdata(L, sz);  // create new userdata
			lua_pushlightuserdata(L, key);
			lua_pushvalue(L, -2);  // dup userdata
			lua_settable(L, -4);   // lookup[key] = userdata
		}
		return ud;
	}
};

void report(lua_State * L)
{
	const char * msg= lua_tostring(L,-1);
	while(msg)
	{
		lua_pop(L,-1);
		printf("\t%s\n", msg);
		msg=lua_tostring(L,-1);
	}
}

LuaEngine::LuaEngine()
{
	this->L = lua_open();
}

LuaEngine::~LuaEngine()
{
	lua_close(L);
}

void LuaEngine::LoadScripts()
{
	set<string> luaFiles;
	set<string> luaBytecodeFiles;

#ifdef WIN32
	WIN32_FIND_DATA fd;
	HANDLE h;

	h = FindFirstFile("scripts\\*.*", &fd);
	if(h == INVALID_HANDLE_VALUE)
		return;

	do 
	{
		char * fn = strrchr(fd.cFileName, '\\');
		if(!fn)
			fn=fd.cFileName;
        char * ext = strrchr(fd.cFileName, '.');
		if(!stricmp(ext, ".lua"))
			luaFiles.insert(string(fn));
		else if(!stricmp(ext, ".luc"))
			luaBytecodeFiles.insert(string(fn));
	} while(FindNextFile(h, &fd));
	FindClose(h);
#else
	struct dirent ** list;
	int filecount = scandir("./scripts", &list, 0, 0);
	if(filecount <= 0 || !list)
		return;

	while(filecount--)
	{
		char* ext = strrchr(list[filecount]->d_name, '.');
		if(ext != NULL && !strcmp(ext, ".lua"))
			{
				string full_path = string(list[filecount]->d_name);
				luaFiles.insert(string(full_path.c_str()));
		}
		else if(!stricmp(ext, ".luc"))
		{
		string full_path = string(list[filecount]->d_name);
		luaBytecodeFiles.insert(string(full_path.c_str()));
		}

		free(list[filecount]);
	}
	free(list);
#endif

	// we prefer precompiled code.
	for(set<string>::iterator itr = luaBytecodeFiles.begin(); itr != luaBytecodeFiles.end(); ++itr)
	{
		set<string>::iterator it2 = luaFiles.find(*itr);
		if(it2 == luaFiles.end())
			luaFiles.erase(it2);
	}

	luaL_openlibs(L);
	RegisterCoreFunctions();

	if(lua_is_starting_up)
		Log.Notice("LuaEngine", "Loading Scripts...");

	char filename[200];

	for(set<string>::iterator itr = luaFiles.begin(); itr != luaFiles.end(); ++itr)
	{
#ifdef WIN32
			snprintf(filename, 200, "scripts\\%s", itr->c_str());
#else
			snprintf(filename, 200, "scripts/%s", itr->c_str());
#endif
		if(lua_is_starting_up)
			Log.Notice("LuaEngine", "%s...", itr->c_str());

		if(luaL_loadfile(L, filename) != 0)
		{
			printf("failed. (could not load)\n");
			const char * msg = lua_tostring(L, -1);
			if(msg!=NULL&&lua_is_starting_up)
				printf("\t%s\n",msg);
		}
		else
		{
			if(lua_pcall(L, 0, LUA_MULTRET, 0) != 0)
			{
				printf("failed. (could not run)\n");
				const char * msg = lua_tostring(L, -1);
				if(msg!=NULL&&lua_is_starting_up)
					printf("\t%s\n",msg);
			}
		}
	}
}

void LuaEngine::OnUnitEvent(Unit * pUnit, const char * FunctionName, uint32 EventType, Unit * pMiscUnit, uint32 Misc)
{
	if(FunctionName==NULL)
		return;

	m_Lock.Acquire();
	lua_pushstring(L, FunctionName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if(lua_isnil(L,-1))
	{
		printf("Tried to call invalid LUA function '%s' from Ascent (Unit)!\n", FunctionName);
		m_Lock.Release();
		return;
	}

	Lunar<Unit>::push(L, pUnit);
	lua_pushinteger(L,EventType);
	if(pMiscUnit!=NULL)
		Lunar<Unit>::push(L, pMiscUnit);
	else
		lua_pushnil(L);
	lua_pushinteger(L,Misc);
	
	int r = lua_pcall(L,4,LUA_MULTRET,0);
	if(r)
		report(L);

	m_Lock.Release();
}

void LuaEngine::OnQuestEvent(Player * QuestOwner, const char * FunctionName, uint32 QuestID, uint32 EventType, Object * QuestStarter)
{
	if(FunctionName==NULL)
		return;

	m_Lock.Acquire();
	lua_pushstring(L, FunctionName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if(lua_isnil(L,-1))
	{
		printf("Tried to call invalid LUA function '%s' from Ascent (Quest)!\n", FunctionName);
		m_Lock.Release();
		return;
	}

	if (QuestOwner)
		Lunar<Unit>::push(L, (Unit*)QuestOwner);
	else
		lua_pushnil(L);

	lua_pushinteger(L,EventType);

	if(QuestStarter!=NULL && QuestStarter->GetTypeId() == TYPEID_UNIT)
		Lunar<Unit>::push(L, (Unit*)QuestStarter);
	else
		lua_pushnil(L);

	int r = lua_pcall(L,3,LUA_MULTRET,0);
	if(r)
		report(L);

	m_Lock.Release();
	
}
void LuaEngine::CallFunction(Unit * pUnit, const char * FuncName)
{
	m_Lock.Acquire();
	lua_pushstring(L, FuncName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if(lua_isnil(L,-1))
	{
		printf("Tried to call invalid LUA function '%s' from Ascent (Unit)!\n", FuncName);
		m_Lock.Release();
		return;
	}

	Lunar<Unit>::push(L, pUnit);
	int r = lua_pcall(L,1,LUA_MULTRET,0);
	if(r)
		report(L);

	m_Lock.Release();
}

void LuaEngine::OnGameObjectEvent(GameObject * pGameObject, const char * FunctionName, uint32 EventType, Unit * pMiscUnit)
{
	if(FunctionName==NULL)
		return;

	m_Lock.Acquire();
	lua_pushstring(L, FunctionName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if(lua_isnil(L,-1))
	{
		printf("Tried to call invalid LUA function '%s' from Ascent! (GO)\n", FunctionName);
		m_Lock.Release();
		return;
	}

	Lunar<GameObject>::push(L, pGameObject);
	lua_pushinteger(L,EventType);
	if(!pMiscUnit)
		lua_pushnil(L);
	else
		Lunar<Unit>::push(L, pMiscUnit);

	int r = lua_pcall(L,3,LUA_MULTRET,0);
	if(r)
		report(L);

	m_Lock.Release();
}

static int RegisterUnitEvent(lua_State * L);
static int RegisterQuestEvent(lua_State * L);
static int RegisterGameObjectEvent(lua_State * L);

void LuaEngine::RegisterCoreFunctions()
{
	lua_pushcfunction(L, &RegisterUnitEvent);
	lua_setglobal(L, "RegisterUnitEvent");

	lua_pushcfunction(L, &RegisterGameObjectEvent);
	lua_setglobal(L, "RegisterGameObjectEvent");

	lua_pushcfunction(L, &RegisterQuestEvent);
	lua_setglobal(L, "RegisterQuestEvent");

	Lunar<Unit>::Register(L);
	Lunar<GameObject>::Register(L);
	//Lunar<Quest>::Register(L); quest isn't a class
}

static int RegisterUnitEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

	g_luaMgr.RegisterUnitEvent(entry,ev,str);
	return 0;
}

static int RegisterQuestEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

	g_luaMgr.RegisterQuestEvent(entry,ev,str);
	return 0;
}

static int RegisterGameObjectEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

	g_luaMgr.RegisterGameObjectEvent(entry,ev,str);
	return 0;
}

/************************************************************************/
/* Manager Stuff                                                        */
/************************************************************************/

class LuaCreature : public CreatureAIScript
{
public:
	LuaCreature(Creature* creature) : CreatureAIScript(creature) {};
	~LuaCreature() {};

	void OnCombatStart(Unit* mTarget)
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_ENTER_COMBAT] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_ENTER_COMBAT], CREATURE_EVENT_ON_ENTER_COMBAT, mTarget, 0 );
	}

	void OnCombatStop(Unit* mTarget)
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_LEAVE_COMBAT] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_LEAVE_COMBAT], CREATURE_EVENT_ON_LEAVE_COMBAT, mTarget, 0 );
	}

	void OnTargetDied(Unit* mTarget)
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_KILLED_TARGET] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_KILLED_TARGET], CREATURE_EVENT_ON_KILLED_TARGET, mTarget, 0 );
	}

	void OnDied(Unit *mKiller)
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_DIED] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_DIED], CREATURE_EVENT_ON_DIED, mKiller, 0 );
	}

	void OnLoad()
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_SPAWN] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_SPAWN], CREATURE_EVENT_ON_SPAWN, NULL, 0 );
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_REACH_WP] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_REACH_WP], CREATURE_EVENT_ON_REACH_WP, NULL, iWaypointId );
	}

	void AIUpdate()
	{
		if( m_binding->Functions[CREATURE_EVENT_AI_TICK] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_AI_TICK], CREATURE_EVENT_AI_TICK, NULL, 0 );
	}

	void StringFunctionCall(const char * pFunction)
	{
		g_engine->CallFunction( _unit, pFunction );
	}

	void Destroy()
	{
		delete this;
	}

	LuaUnitBinding * m_binding;
};

class LuaGameObject : public GameObjectAIScript
{
public:
	LuaGameObject(GameObject * go) : GameObjectAIScript(go) {}
	~LuaGameObject() {}

	void OnSpawn()
	{
		if( m_binding->Functions[GAMEOBJECT_EVENT_ON_SPAWN] != NULL )
			g_engine->OnGameObjectEvent( _gameobject, m_binding->Functions[GAMEOBJECT_EVENT_ON_SPAWN], GAMEOBJECT_EVENT_ON_SPAWN, NULL );
	}

	void OnActivate(Player * pPlayer)
	{
		if( m_binding->Functions[GAMEOBJECT_EVENT_ON_USE] != NULL )
			g_engine->OnGameObjectEvent( _gameobject, m_binding->Functions[GAMEOBJECT_EVENT_ON_USE], GAMEOBJECT_EVENT_ON_USE, pPlayer );
	}

	LuaGameObjectBinding * m_binding;
};

class LuaQuest : public QuestScript
{
public:
	LuaQuest() : QuestScript() {}
	~LuaQuest() {}

	void OnQuestStart(Player* mTarget, QuestLogEntry *qLogEntry)
	{
		if( m_binding->Functions[QUEST_EVENT_ON_ACCEPT] != NULL )
			g_engine->OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_ON_ACCEPT], qLogEntry->GetQuest()->id, QUEST_EVENT_ON_ACCEPT, mTarget );
	}

	void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
	{
		if( m_binding->Functions[QUEST_EVENT_ON_COMPLETE] != NULL )
			g_engine->OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_ON_COMPLETE], qLogEntry->GetQuest()->id, QUEST_EVENT_ON_COMPLETE, mTarget );
	}

	LuaQuestBinding * m_binding;
};

CreatureAIScript * CreateLuaCreature(Creature * src)
{
	LuaUnitBinding * pBinding = g_luaMgr.GetUnitBinding( src->GetEntry() );
	if( pBinding == NULL )
		return NULL;

	LuaCreature * pLua = new LuaCreature( src );
	pLua->m_binding = pBinding;
	return pLua;
}

GameObjectAIScript * CreateLuaGameObject(GameObject * src)
{
	LuaGameObjectBinding * pBinding = g_luaMgr.GetGameObjectBinding( src->GetEntry() );
	if( pBinding == NULL )
		return NULL;

	LuaGameObject * pLua = new LuaGameObject( src );
	pLua->m_binding = pBinding;
	return pLua;
}

QuestScript * CreateLuaQuestScript(uint32 id)
{
	LuaQuestBinding * pBinding = g_luaMgr.GetQuestBinding( id );
	if( pBinding == NULL )
		return NULL;

	LuaQuest * pLua = new LuaQuest();
	pLua->m_binding = pBinding;
	return pLua;
}

void LuaEngineMgr::Startup()
{
	Log.Notice("LuaEngineMgr", "Spawning Lua Engine...");
	m_engine = new LuaEngine();
	lua_is_starting_up = true;
	m_engine->LoadScripts();
	g_engine = m_engine;
	lua_is_starting_up = false;

	// stuff is registered, so lets go ahead and make our emulated C++ scripted lua classes.
	for(UnitBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
		m_scriptMgr->register_creature_script( itr->first, CreateLuaCreature );
	}

	for(GameObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
		m_scriptMgr->register_gameobject_script( itr->first, CreateLuaGameObject );
	}

	for(QuestBindingMap::iterator itr = m_questBinding.begin(); itr != m_questBinding.end(); ++itr)
	{
		QuestScript * qs = CreateLuaQuestScript( itr->first );
		if( qs != NULL )
			m_scriptMgr->register_quest_script( itr->first, qs );
	}
}

void LuaEngineMgr::RegisterUnitEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
	UnitBindingMap::iterator itr = m_unitBinding.find(Id);
	if(itr == m_unitBinding.end())
	{
		LuaUnitBinding ub;
		memset(&ub,0,sizeof(LuaUnitBinding));
		ub.Functions[Event] = strdup(FunctionName);
		m_unitBinding.insert(make_pair(Id,ub));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}

void LuaEngineMgr::RegisterQuestEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
	QuestBindingMap::iterator itr = m_questBinding.find(Id);
	if(itr == m_questBinding.end())
	{
		LuaQuestBinding qb;
		memset(&qb,0,sizeof(LuaQuestBinding));
		qb.Functions[Event] = strdup(FunctionName);
		m_questBinding.insert(make_pair(Id,qb));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}
void LuaEngineMgr::RegisterGameObjectEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
	GameObjectBindingMap::iterator itr = m_gameobjectBinding.find(Id);
	if(itr == m_gameobjectBinding.end())
	{
		LuaGameObjectBinding ub;
		memset(&ub,0,sizeof(LuaGameObjectBinding));
		ub.Functions[Event] = strdup(FunctionName);
		m_gameobjectBinding.insert(make_pair(Id,ub));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}

/*void LuaEngineMgr::ReloadScripts()
{
	m_lock.Acquire();

	// acquire the locks on all the luaengines so they don't do anything.
	for(LuaEngineMap::iterator itr = m_engines.begin(); itr != m_engines.end(); ++itr)
		itr->first->GetLock().Acquire();

	// remove all the function name bindings
	for(UnitBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
		for(uint32 i = 0; i < CREATURE_EVENT_COUNT; ++i)
			if(itr->second.Functions[i] != NULL)
				free((void*)itr->second.Functions[i]);
	}
	
	for(GameObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
		for(uint32 i = 0; i < GAMEOBJECT_EVENT_COUNT; ++i)
			if(itr->second.Functions[i] != NULL)
				free((void*)itr->second.Functions[i]);
	}

	// clear the maps
	m_gameobjectBinding.clear();
	m_unitBinding.clear();

	// grab the first lua engine in the list, use it to re-create all the binding names.
	LuaEngine * l = m_engines.begin()->first;
	lua_is_starting_up = true;
	l->Restart();
	lua_is_starting_up = false;

	// all our bindings have been re-created, go through the lua engines and restart them all, and then release their locks.
	for(LuaEngineMap::iterator itr = m_engines.begin(); itr != m_engines.end(); ++itr)
	{
		if(itr->first != l)		// this one is already done
		{
			itr->first->Restart();
			itr->first->GetLock().Release();
		}
	}

	// release the big lock
	m_lock.Release();
}*/

void LuaEngineMgr::Unload()
{
	/*m_lock.Acquire();
	for(LuaEngineMap::iterator itr = m_engines.begin(); itr != m_engines.end(); ++itr)
	{
		delete itr->first;
	}
	m_lock.Release();*/
}

void LuaEngine::Restart()
{
	m_Lock.Acquire();
	lua_close(L);
	L = lua_open();
	LoadScripts();
	m_Lock.Release();
}




/************************************************************************/
/* SCRIPT FUNCTION IMPLEMENTATION                                       */
/************************************************************************/
#define CHECK_TYPEID(expected_type) if(!ptr || !ptr->IsInWorld() || ptr->GetTypeId() != expected_type) { return 0; }
#define CHECK_TYPEID_RET(expected_type) if(!ptr || !ptr->IsInWorld() || ptr->GetTypeId() != expected_type) { lua_pushboolean(L,0); return 0; }
#define CHECK_TYPEID_RET_INT(expected_type) if(!ptr || !ptr->IsInWorld() || ptr->GetTypeId() != expected_type) { lua_pushinteger(L,0); return 0; }

/////////////////////////////////////////////////////
////////////////UNIT COMMANDS////////////////////////
/////////////////////////////////////////////////////
// Item Gossip Functions
int luaItem_GossipCreateMenu(lua_State * L, Item * ptr)
{
    int text_id = luaL_checkint(L, 1);
	Unit* target = Lunar<Unit>::check(L, 2);
 	int autosend = luaL_checkint(L, 3);
 
	Player* plr = (Player*)target;
    
    objmgr.CreateGossipMenuForPlayer(&Menu, ptr->GetGUID(), text_id, plr);
	if(autosend)
        Menu->SendTo(plr);
	return 1;
}

int luaItem_GossipMenuAddItem(lua_State * L, Item * ptr)
{
	int icon = luaL_checkint(L, 1);
	const char * menu_text = luaL_checkstring(L, 2);
    int IntId = luaL_checkint(L, 3);
    int extra = luaL_checkint(L, 4);

	Menu->AddItem(icon, menu_text, IntId, extra);
	return 1;
}

int luaItem_GossipSendMenu(lua_State * L, Item * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	Menu->SendTo(plr);
	return 1;
}

int luaItem_GossipComplete(lua_State * L, Item * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	plr->Gossip_Complete();
	return 1;
}

int luaItem_GossipSendPOI(lua_State * L, Item * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);
	int icon = luaL_checkint(L, 4);
	int flags = luaL_checkint(L, 5);
	int data = luaL_checkint(L, 6);
	const char * name = luaL_checkstring(L, 7);

	plr->Gossip_SendPOI(x, y, icon, flags, data, name);
	return 1;
}
////////////////////////////////////////////////////
//////////////UNIT GOSSIP///////////////////////////
////////////////////////////////////////////////////
int luaUnit_GossipCreateMenu(lua_State * L, Unit * ptr)
{
	int text_id = luaL_checkint(L, 1);
	Unit* target = Lunar<Unit>::check(L, 2);
 	int autosend = luaL_checkint(L, 3);

	Player* plr = (Player*)target;
    
    objmgr.CreateGossipMenuForPlayer(&Menu, ptr->GetGUID(), text_id, plr);
	if(autosend)
		Menu->SendTo(plr);
	return 1;
}

int luaUnit_GossipMenuAddItem(lua_State * L, Unit * ptr)
{
	int icon = luaL_checkint(L, 1);
   	const char * menu_text = luaL_checkstring(L, 2);
    int IntId = luaL_checkint(L, 3);
    int extra = luaL_checkint(L, 4);
    
	Menu->AddItem(icon, menu_text, IntId, extra);
	return 1;
}

int luaUnit_GossipSendMenu(lua_State * L, Unit * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
    Player* plr = (Player*)target;
	Menu->SendTo(plr);
	return 1;
}

int luaUnit_GossipSendPOI(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);
	int icon = luaL_checkint(L, 4);
	int flags = luaL_checkint(L, 5);
	int data = luaL_checkint(L, 6);
	const char * name = luaL_checkstring(L, 7);

	plr->Gossip_SendPOI(x, y, icon, flags, data, name);
	return 1;
}

int luaUnit_GossipComplete(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	Player * plr = (Player*)ptr;
	plr->Gossip_Complete();
	return 1;
}
////////////////////////////////////
////////////GAMEOBJECT GOSSIP///////
/////////////////////////////////////
int luaGameObject_GossipCreateMenu(lua_State * L, GameObject * ptr)
{
	int text_id = luaL_checkint(L, 1);
	Unit* target = Lunar<Unit>::check(L, 2);
 	int autosend = luaL_checkint(L, 3);
	Player* plr = (Player*)target;
    
    objmgr.CreateGossipMenuForPlayer(&Menu, ptr->GetGUID(), text_id, plr);
	if(autosend)
        Menu->SendTo(plr);
	return 1;
}
int luaGameObject_GossipMenuAddItem(lua_State * L, GameObject * ptr)
 {
	int icon = luaL_checkint(L, 1);
	const char * menu_text = luaL_checkstring(L, 2);
    int IntId = luaL_checkint(L, 3);
    int extra = luaL_checkint(L, 4);

	Menu->AddItem(icon, menu_text, IntId, extra);
	return 1;
}
int luaGameObject_GossipSendMenu(lua_State * L, GameObject * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	Menu->SendTo(plr);
	return 1;
}
int luaGameObject_GossipComplete(lua_State * L, GameObject * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
 	plr->Gossip_Complete();
 	return 1;
 }
int luaGameObject_GossipSendPOI(lua_State * L, GameObject * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);
	int icon = luaL_checkint(L, 4);
	int flags = luaL_checkint(L, 5);
	int data = luaL_checkint(L, 6);
	const char * name = luaL_checkstring(L, 7);

	plr->Gossip_SendPOI(x, y, icon, flags, data, name);
	return 1;
}

int luaUnit_IsPlayer(lua_State * L, Unit * ptr)
{
	if(!ptr)
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	if(ptr->IsPlayer())
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

int luaUnit_IsCreature(lua_State * L, Unit * ptr)
{
	if(!ptr)
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	if(ptr->GetTypeId()==TYPEID_UNIT)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

int luaUnit_Emote(lua_State * L, Unit * ptr)
{
	if(ptr==NULL) return 0;
	uint32 emote_id = luaL_checkint(L, 1);
	uint32 time = luaL_checkint(L, 2);
	if(emote_id==0) 
		return 0;
	if (time)
		ptr->EventAddEmote((EmoteType)emote_id,time);
	else
		ptr->Emote((EmoteType)emote_id);
	return 1;
}

int luaUnit_GetManaPct(lua_State * L, Unit * ptr)
{
	if(!ptr) 
		return 0;
	if (ptr->GetPowerType() == POWER_TYPE_MANA)
		lua_pushnumber(L, (int)(ptr->GetUInt32Value(UNIT_FIELD_POWER1) * 100.0f / ptr->GetUInt32Value(UNIT_FIELD_MAXPOWER1)));
	else
		lua_pushnil(L);
	return 1;
}

int luaUnit_GetName(lua_State * L, Unit * ptr)
{
	if(!ptr)
		return 0;

	switch(ptr->GetTypeId())
	{
	case TYPEID_UNIT:
		lua_pushstring(L, ((Creature*)ptr)->GetCreatureInfo() ? ((Creature*)ptr)->GetCreatureInfo()->Name : "Unknown");
		break;

	case TYPEID_PLAYER:
		lua_pushstring(L, ((Player*)ptr)->GetName());
		break;

	default:
		lua_pushstring(L, "Unknown");
		break;
	}

	return 1;
}

int luaUnit_SendChatMessage(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	uint32 typ = luaL_checkint(L, 1);
	uint32 lang = luaL_checkint(L, 2);
	const char * message = luaL_checklstring(L, 3, NULL);
	if(message == NULL)
		return 0;

	ptr->SendChatMessage(typ, lang, message);
	return 0;
}

int luaUnit_MoveTo(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	double x = luaL_checknumber(L, 1);
	double y = luaL_checknumber(L, 2);
	double z = luaL_checknumber(L, 3);
	double o = luaL_checknumber(L, 4);

	if(x == 0 || y == 0 || z == 0)
		return 0;

	ptr->GetAIInterface()->MoveTo( (float)x, (float)y, (float)z, (float)o );
	return 0;
}

int luaUnit_SetMovementType(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	uint32 typ = luaL_checkint(L, 1);
	ptr->GetAIInterface()->setMoveType(typ);
	return 0;
}

int luaUnit_GetX(lua_State * L, Unit * ptr)
{
	if(ptr==NULL) return 0;
	lua_pushnumber(L, ptr->GetPositionX());
	return 1;
}

int luaUnit_GetY(lua_State * L, Unit * ptr)
{
	if(ptr==NULL) return 0;
	lua_pushnumber(L, ptr->GetPositionY());
	return 1;
}

int luaUnit_GetZ(lua_State * L, Unit * ptr)
{
	if(ptr==NULL) return 0;
	lua_pushnumber(L, ptr->GetPositionZ());
	return 1;
}

int luaUnit_GetO(lua_State * L, Unit * ptr)
{
	if(ptr==NULL) return 0;
	lua_pushnumber(L, ptr->GetOrientation());
	return 1;
}

int luaUnit_CastSpell(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	uint32 sp = luaL_checkint(L, 1);
	if(sp==0)
		return 0;
	ptr->CastSpell(ptr,dbcSpell.LookupEntry(sp),true);
	return 0;
}

int luaUnit_FullCastSpell(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	uint32 sp = luaL_checkint(L, 1);
	if(sp==0)
		return 0;
	ptr->CastSpell(ptr,dbcSpell.LookupEntry(sp),false);
	return 0;
}

int luaUnit_CastSpellOnTarget(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	uint32 sp = luaL_checkint(L, 1);
	Unit * target = Lunar<Unit>::check(L, 2);

	if(sp==0 || target==NULL)
		return 0;
	ptr->CastSpell(target,dbcSpell.LookupEntry(sp),true);
	return 0;
}

int luaUnit_FullCastSpellOnTarget(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	uint32 sp = luaL_checkint(L, 1);
	Unit * target = Lunar<Unit>::check(L, 2);

	if(sp==0 || target==NULL)
		return 0;
	ptr->CastSpell(target,dbcSpell.LookupEntry(sp),false);
	return 0;
}
int luaUnit_SpawnCreature(lua_State * L, Unit * ptr)
{
	if(ptr == NULL) return 0;
	uint32 entry_id = luaL_checkint(L, 1);
	float x = (float)luaL_checkint(L, 2);
	float y = (float)luaL_checkint(L, 3);
	float z = (float)luaL_checkint(L, 4);
	float o = (float)luaL_checkint(L, 5);
	uint32 faction = luaL_checkint(L, 6);
	uint32 duration = luaL_checkint(L, 7);

	if( !x || !y || !z || !entry_id || !faction /*|| !duration*/) //Shady: is it really required?
	{
		lua_pushnil(L);
		return 1;
	}
	CreatureProto *p = CreatureProtoStorage.LookupEntry(entry_id);
    
    if(p == NULL)
      return NULL;
	Creature * pCreature = ptr->GetMapMgr()->GetInterface()->SpawnCreature(entry_id,x,y,z,o,true,true,0,0);
	pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,faction);
	pCreature->_setFaction();
	pCreature->SetInstanceID(ptr->GetInstanceID());
	pCreature->SetMapId(ptr->GetMapId());
	if(duration)
		pCreature->Despawn(duration,0);
	Lunar<Unit>::push(L,pCreature);
	return 1;
}
int luaUnit_SpawnGameObject(lua_State * L, Unit * ptr)
{
	if(ptr == NULL) return 0;
	uint32 entry_id = luaL_checkint(L, 1);
	float x = (float)luaL_checkint(L, 2);
	float y = (float)luaL_checkint(L, 3);
	float z = (float)luaL_checkint(L, 4);
	float o = (float)luaL_checkint(L, 5);
	uint32 duration = luaL_checkint(L, 6);
	if(entry_id)
	{
		GameObject* pC = ptr->GetMapMgr()->GetInterface()->SpawnGameObject(entry_id,x,y,z,o,false,0,0);
		pC->SetInstanceID(ptr->GetInstanceID());
		pC->SetMapId(ptr->GetMapId());
		pC->Spawn(ptr->GetMapMgr());
		if(duration && duration > 0)
			sEventMgr.AddEvent(pC,&GameObject::ExpireAndDelete,EVENT_GAMEOBJECT_UPDATE,duration,1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		Lunar<GameObject>::push(L,pC,false);
	}
	return 1;
}

int luaUnit_RegisterEvent(lua_State * L, Unit * ptr)
{
	if(!ptr||ptr->GetTypeId()!=TYPEID_UNIT)
		return 0;

	const char * func_to_call = luaL_checkstring(L,1);
	int delay=luaL_checkint(L,2);
	int repeats=luaL_checkint(L,3);

	if(!func_to_call||!delay)
		return 0;

	Creature * pCreature = ((Creature*)ptr);
	string strFunc = string(func_to_call);
	sEventMgr.AddEvent(pCreature, &Creature::TriggerScriptEvent, strFunc, EVENT_CREATURE_UPDATE, (uint32)delay, (uint32)repeats, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	return 0;
}

int luaUnit_RemoveEvents(lua_State * L, Unit * ptr)
{
	if(!ptr||ptr->GetTypeId()!=TYPEID_UNIT)
		return 0;

	Creature * pCreature = ((Creature*)ptr);
	sEventMgr.RemoveEvents(pCreature);
	return 0;
}

int luaUnit_AddEventHolder(lua_State * L, Unit * ptr)
{
	EventableObjectHolder * holder = Lunar<EventableObjectHolder>::check(L,1);

	if(ptr&&holder)
	{
		sEventMgr.AddEventHolder(holder,ptr->GetInstanceID());
	}
	return 1;
}

int luaUnit_SetFaction(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int faction = luaL_checkint(L,1);
	if(!faction)
		return 0;

	ptr->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,faction);
	ptr->_setFaction();
	return 0;
}
int luaUnit_SetStandState(lua_State * L, Unit * ptr) //states 0..8
{
	if (!ptr)
		return 0;
	int state = luaL_checkint(L,1);
	if(state<=0)
		return 0;
	ptr->SetStandState(state);
	return 0;
}
int luaUnit_IsInCombat(lua_State * L, Unit * ptr)
{
	if (!ptr)
		return 0;
	if(ptr->CombatStatus.IsInCombat())
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}

int luaUnit_SetScale(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	double faction = luaL_checknumber(L,1);
	if(faction==0.0)
		return 0;

	ptr->SetFloatValue(OBJECT_FIELD_SCALE_X,(float)faction);
	return 0;
}

int luaUnit_SetModel(lua_State * L, Unit * ptr)
{
	if(!ptr||!ptr->IsUnit())
		return 0;

	int modelid = luaL_checkint(L,1);
	ptr->SetUInt32Value(UNIT_FIELD_DISPLAYID,modelid);
	return 0;
}

int luaUnit_SetNPCFlags(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int flags = luaL_checkint(L,1);
	if(!flags)
		return 0;

	ptr->SetUInt32Value(UNIT_NPC_FLAGS,flags);
	return 0;
}

int luaUnit_SetCombatCapable(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int enabled = luaL_checkint(L,1);
	ptr->GetAIInterface()->disable_combat = (enabled > 0) ? true : false;
	return 0;
}

int luaUnit_SetCombatMeleeCapable(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int enabled = luaL_checkint(L,1);
	ptr->GetAIInterface()->disable_melee = (enabled > 0) ? true : false;
	return 0;
}

int luaUnit_SetCombatRangedCapable(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int enabled = luaL_checkint(L,1);
	ptr->GetAIInterface()->disable_ranged = (enabled > 0) ? true : false;
	return 0;
}

int luaUnit_SetCombatSpellCapable(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int enabled = luaL_checkint(L,1);
	ptr->GetAIInterface()->disable_spell = (enabled > 0) ? true : false;
	return 0;
}

int luaUnit_SetCombatTargetingCapable(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int enabled = luaL_checkint(L,1);
	ptr->GetAIInterface()->disable_targeting = (enabled > 0) ? true : false;
	return 0;
}

int luaUnit_DestroyCustomWaypointMap(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	((Creature*)ptr)->DestroyCustomWaypointMap();
	return 0;
}

int luaUnit_CreateCustomWaypointMap(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	Creature * pCreature = (Creature*)ptr;
	if(pCreature->m_custom_waypoint_map)
	{
		for(WayPointMap::iterator itr = pCreature->m_custom_waypoint_map->begin(); itr != pCreature->m_custom_waypoint_map->end(); ++itr)
			delete (*itr);
		delete pCreature->m_custom_waypoint_map;
	}

	pCreature->m_custom_waypoint_map = new WayPointMap;
	pCreature->GetAIInterface()->SetWaypointMap(pCreature->m_custom_waypoint_map);
	return 0;
}

int luaUnit_CreateWaypoint(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	float x = (float)luaL_checknumber(L,1);
	float y = (float)luaL_checknumber(L,2);
	float z = (float)luaL_checknumber(L,3);
	float o = (float)luaL_checknumber(L,4);
	int waittime = luaL_checkint(L,5);
	int flags = luaL_checkint(L,6);
	int modelid = luaL_checkint(L,7);

	Creature * pCreature = ((Creature*)ptr);
	if(!pCreature->m_custom_waypoint_map)
	{
		pCreature->m_custom_waypoint_map = new WayPointMap;
		pCreature->GetAIInterface()->SetWaypointMap(pCreature->m_custom_waypoint_map);
	}

	if(!modelid)
		modelid = pCreature->GetUInt32Value(UNIT_FIELD_DISPLAYID);

	WayPoint * wp = new WayPoint;
	wp->id = (uint32)pCreature->m_custom_waypoint_map->size() + 1;
	wp->x = x;
	wp->y = y;
	wp->z = z;
	wp->o = o;
	wp->flags = flags;
	wp->backwardskinid = modelid;
	wp->forwardskinid = modelid;
	wp->backwardemoteid = wp->forwardemoteid = 0;
	wp->backwardemoteoneshot = wp->forwardemoteoneshot = false;
	wp->waittime = waittime;
	pCreature->m_custom_waypoint_map->push_back(wp);
	pCreature->GetAIInterface()->addWayPoint(wp);
	return 0;
}

int luaUnit_MoveToWaypoint(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int id = luaL_checkint(L,1);
	if(id)
		ptr->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		ptr->GetAIInterface()->setWaypointToMove(id);
	return 0;
}

int luaUnit_RemoveItem(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	int id = luaL_checkint(L,1);
	int count = luaL_checkint(L,2);

	((Player*)ptr)->GetItemInterface()->RemoveItemAmt(id,count);
	return 0;
}

int luaUnit_AddItem(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	int id = luaL_checkint(L,1);
	int count = luaL_checkint(L,2);

	Player * plr = (Player*)ptr;
	ItemPrototype * proto = ItemPrototypeStorage.LookupEntry(id);
	if(proto==NULL)
		return 0;

	Item * add = plr->GetItemInterface()->FindItemLessMax(id,count,false);
	if(add==NULL)
	{
		add=objmgr.CreateItem(id,plr);
		add->SetUInt32Value(ITEM_FIELD_STACK_COUNT,count);
		if(plr->GetItemInterface()->AddItemToFreeSlot(add))
			plr->GetSession()->SendItemPushResult(add,false,true,false,true,plr->GetItemInterface()->LastSearchItemBagSlot(),plr->GetItemInterface()->LastSearchItemSlot(),count);
		else
			delete add;
	}
	else
	{
		add->ModSignedInt32Value(ITEM_FIELD_STACK_COUNT,count);
		plr->GetSession()->SendItemPushResult(add,false,true,false,false,plr->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,count);
	}

	return 0;
}
int luaUnit_GetInstanceID(lua_State * L, Unit * ptr)
{
	if(ptr->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL)
		lua_pushnil(L);
	else
		lua_pushinteger(L,ptr->GetInstanceID());
	return 1;
}

int luaUnit_GetClosestPlayer(lua_State * L, Unit * ptr)
{
	if(!ptr)
		return 0;

	float dist, d2;
	Player * ret=NULL;

	for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
	{
		d2=(*itr)->GetDistanceSq(ptr);
		if(!ret||d2<dist)
		{
			dist=d2;
			ret=*itr;
		}
	}

	if(ret==NULL)
		lua_pushnil(L);
	else
		Lunar<Unit>::push(L,((Unit*)ret),false);

	return 1;
}
int luaUnit_GetRandomPlayer(lua_State * L, Unit * ptr)
{
	if( ptr == NULL )
		return 0;

	int flag = luaL_checkint( L, 1 );

	Player* ret = NULL;

	switch( flag )
	{
	case RANDOM_ANY:
		{
			uint32 count = (uint32)ptr->GetInRangePlayersCount();
			uint32 r = RandomUInt(count-1);
			count=0;

			for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
			{
				if (count==r)
				{
					ret=*itr;
					break;
				}
				++count;
			}
		}
		break;
	case RANDOM_IN_SHORTRANGE:
		{
			uint32 count = 0;
			for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
			{
				Player* obj = (Player*)(*itr);
				if (obj && obj->CalcDistance(obj,ptr)<=8)
					++count;
			}
			if (count)
			{
				uint32 r = RandomUInt(count-1);
				count=0;
				for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = (Player*)(*itr);
					if (obj && obj->CalcDistance(obj,ptr)<=8 && count==r)
					{
						ret=obj;
						break;
					}
					++count;
				}
			}
		}
		break;
	case RANDOM_IN_MIDRANGE:
		{
			uint32 count = 0;
			for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
			{
				Player* obj = (Player*)(*itr);
				if (!obj)
					continue;
				float distance = obj->CalcDistance(obj,ptr);
				if (distance<20 && distance>8)
					++count;
			}
			if (count)
			{
				uint32 r = RandomUInt(count-1);
				count=0;
				for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = (Player*)(*itr);
					if (!obj)
						continue;
					float distance = obj->CalcDistance(obj,ptr);
					if (distance<20 && distance>8 && count==r)
					{
						ret=obj;
						break;
					}
					++count;
				}
			}
		}
		break;
	case RANDOM_IN_LONGRANGE:
		{
			uint32 count = 0;
			for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
			{
				Player* obj = (Player*)(*itr);
				if (obj && obj->CalcDistance(obj,ptr)>=20)
					++count;
			}
			if (count)
			{
				uint32 r = RandomUInt(count-1);
				count=0;
				for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = (Player*)(*itr);
					if (obj && obj->CalcDistance(obj,ptr)>=20 && count==r)
					{
						ret=obj;
						break;
					}
					++count;
				}
			}
		}
		break;
	case RANDOM_WITH_MANA:
		{
			uint32 count = 0;
			for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
			{
				Player* obj = (Player*)(*itr);
				if (obj && obj->GetPowerType() == POWER_TYPE_MANA)
					++count;
			}
			if (count)
			{
				uint32 r = RandomUInt(count-1);
				count=0;
				for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = (Player*)(*itr);
					if (obj && obj->GetPowerType() == POWER_TYPE_MANA && count==r)
					{
						ret=obj;
						break;
					}
					++count;
				}
			}
		}
		break;
	case RANDOM_WITH_ENERGY:
		{
			uint32 count = 0;
			for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
			{
				Player* obj = (Player*)(*itr);
				if (obj && obj->GetPowerType() == POWER_TYPE_ENERGY)
					++count;
			}
			if (count)
			{
				uint32 r = RandomUInt(count-1);
				count=0;
				for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = (Player*)(*itr);
					if (obj && obj->GetPowerType() == POWER_TYPE_ENERGY && count==r)
					{
						ret=obj;
						break;
					}
					++count;
				}
			}
		}
		break;
	case RANDOM_WITH_RAGE:
		{
			uint32 count = 0;
			for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
			{
				Player* obj = (Player*)(*itr);
				if (obj && obj->GetPowerType() == POWER_TYPE_RAGE)
					++count;
			}
			if (count)
			{
				uint32 r = RandomUInt(count-1);
				count=0;
				for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = (Player*)(*itr);
					if (obj && obj->GetPowerType() == POWER_TYPE_RAGE && count==r)
					{
						ret=obj;
						break;
					}
					++count;
				}
			}
		}
		break;
	case RANDOM_NOT_MAINTANK:
		{
			uint32 count = 0;
			Unit* mt = ptr->GetAIInterface()->GetMostHated();
			if (!mt->IsPlayer())
				return 0;

			for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
			{
				Player* obj = (Player*)(*itr);
				if (obj != mt)
					++count;
			}
			if (count)
			{
				uint32 r = RandomUInt(count-1);
				count=0;
				for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* obj = (Player*)(*itr);
					if (obj && obj != mt && count==r)
					{
						ret=obj;
						break;
					}
					++count;
				}
			}
		}
		break;
	}

	if(ret==NULL)
		lua_pushnil(L);
	else
		Lunar<Unit>::push(L,((Unit*)ret),false);

	return 1;
}
int luaUnit_GetRandomFriend(lua_State * L, Unit * ptr)
{
	if(!ptr)
		return 0;

	Unit * ret=NULL;
	uint32 count = 0;

	for(set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
	{
		Object* obj = (Object*)(*itr);
		if (obj->IsUnit() && isFriendly(obj,ptr))
			++count;
	}

	if (count)
	{
		uint32 r = RandomUInt(count-1);
		count=0;
		for(set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
		{
			Object* obj = (Object*)(*itr);
			if (obj->IsUnit() && isFriendly(obj,ptr) && count==r)
			{
				ret=(Unit*)obj;
				break;
			}
			++count;
		}
	}
	if(ret==NULL)
		lua_pushnil(L);
	else
		Lunar<Unit>::push(L,(ret),false);
	return 1;
}

int luaUnit_StopMovement(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int tim = luaL_checkint(L,1);
	ptr->GetAIInterface()->StopMovement(tim);
	return 0;
}

int luaUnit_RemoveAura(lua_State * L, Unit * ptr)
{
	if(!ptr)return 0;
	int auraid = luaL_checkint(L,1);
	ptr->RemoveAura(auraid);
	return 0;
}

int luaUnit_PlaySoundToSet(lua_State * L, Unit * ptr)
{
	if(!ptr) return 0;
	int soundid = luaL_checkint(L,1);
	ptr->PlaySoundToSet(soundid);
	return 0;
}

int luaUnit_GetUnitBySqlId(lua_State * L, Unit * ptr)
{
	int sqlid = luaL_checkint(L,1);
	if(ptr || sqlid)
		Lunar<Unit>::push(L,ptr->GetMapMgr()->GetSqlIdCreature(sqlid));
	return 1;
}

int luaUnit_Despawn(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int delay = luaL_checkint(L,1);
	int respawntime = luaL_checkint(L,2);
	((Creature*)ptr)->Despawn(delay,respawntime);
	return 0;
}
int luaUnit_GetInRangeFriends(lua_State * L, Unit * ptr)
{
	Object * pC = NULL;
	uint32 count = 0;
	lua_newtable(L);
	for( set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); itr++)
	{
		if( (*itr) ->IsUnit() )
			count++,
			pC = *itr;
			lua_pushinteger(L,count);
			Lunar<Unit>::push(L,((Unit*)pC),false);
			lua_rawset(L,-3);
	}
	return 1;
}

int luaUnit_GetHealthPct(lua_State * L, Unit * ptr)
{
	if(!ptr)
		lua_pushinteger(L,0);
	else
		lua_pushinteger(L, ptr->GetHealthPct());

	return 1;
}
int luaUnit_SetHealthPct(lua_State * L, Unit * ptr)
{
	int val = luaL_checkint(L,1);
	if (val>0)
		ptr->SetHealthPct(val);
	return 1;
}
int luaUnit_GetItemCount(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET_INT(TYPEID_PLAYER);
	int itemid = luaL_checkint(L,1);
	lua_pushinteger(L, ((Player*)ptr)->GetItemInterface()->GetItemCount(itemid,false));
	return 1;
}

int luaUnit_GetMainTank(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET_INT(TYPEID_UNIT);
	Unit* ret = ptr->GetAIInterface()->GetMostHated();
	if(ret==NULL)
		lua_pushnil(L);
	else
		Lunar<Unit>::push(L,(ret),false);
	return 1;
}
int luaUnit_GetAddTank(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET_INT(TYPEID_UNIT);
	Unit* ret = ptr->GetAIInterface()->GetSecondHated();
	if(ret==NULL)
		lua_pushnil(L);
	else
		Lunar<Unit>::push(L,(ret),false);
	return 1;
}
int luaUnit_ClearThreatList(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET_INT(TYPEID_UNIT);
	ptr->ClearHateList();
	return 1;
}

int luaUnit_GetTauntedBy(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT)

		if (!ptr->GetAIInterface()->getTauntedBy())
			lua_pushnil(L);
		else
			Lunar<Unit>::push(L,ptr->GetAIInterface()->getTauntedBy(),false);
	return 1;
}
int luaUnit_SetTauntedBy(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT)
		Unit * target = Lunar<Unit>::check(L, 1);

	if (!target || ptr->GetAIInterface()->GetIsTaunted() || target==ptr)
		return 0;
	else
		ptr->GetAIInterface()->taunt(target);
	return 1;
}
int luaUnit_ModThreat(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L,1);
	int32 amount = luaL_checkint(L,2);
	if(ptr&&target&&amount)
		ptr->GetAIInterface()->modThreatByPtr(target,amount);
	return 0;
}
int luaUnit_GetThreatByPtr(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L,1);
	if(ptr&&target)
		lua_pushnumber(L,ptr->GetAIInterface()->getThreatByPtr(target));
	return 0;
}

int luaUnit_GetSoulLinkedWith(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT)

		if (!ptr->GetAIInterface()->GetIsSoulLinked())
			lua_pushnil(L);
		else
			Lunar<Unit>::push(L,ptr->GetAIInterface()->getSoullinkedWith(),false);
	return 1;
}
int luaUnit_SetSoulLinkedWith(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT)
		Unit * target = Lunar<Unit>::check(L, 1);

	if (!target || ptr->GetAIInterface()->GetIsSoulLinked() || target==ptr)
		return 0;
	else
		ptr->GetAIInterface()->SetSoulLinkedWith(ptr);
	return 1;
}
int luaUnit_ChangeTarget(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT)
		Unit * target = Lunar<Unit>::check(L, 1);

	if (!target || !isHostile(ptr,target) || ptr==target)
		return 0;
	else
		ptr->GetAIInterface()->SetNextTarget(target);
	return 1;
}

int luaUnit_HasFinishedQuest(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET(TYPEID_PLAYER);
	int questid = luaL_checkint(L,1);
	if(((Player*)ptr)->HasFinishedQuest(questid))
		lua_pushboolean(L,1);
	else
		lua_pushboolean(L,0);

	return 1;
}

int luaUnit_UnlearnSpell(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	int spellid = luaL_checkint(L,1);
	((Player*)ptr)->removeSpell(spellid,false,false,0);
	return 0;
}

int luaUnit_LearnSpell(lua_State * L, Unit* ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	int spellid = luaL_checkint(L,1);
	((Player*)ptr)->addSpell(spellid);
	return 0;
}

int luaUnit_MarkQuestObjectiveAsComplete(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	int questid = luaL_checkint(L,1);
	int objective = luaL_checkint(L,2);
	Player * pl = ((Player*)ptr);
	QuestLogEntry * qle = pl->GetQuestLogForEntry(questid);
	qle->SetMobCount(objective, qle->GetQuest()->required_mobcount[objective]);
	qle->SendUpdateAddKill(objective);
	if(qle->CanBeFinished())
		qle->SendQuestComplete();
        qle->UpdatePlayerFields();

	return 0;
}

int luaUnit_KnockBack(lua_State * L, Unit * ptr)
{
	if(!ptr) return 0;
	double dx = luaL_checknumber(L,1);
	double dy = luaL_checknumber(L,2);
	double affect1 = luaL_checknumber(L,3);
	double affect2 = luaL_checknumber(L,4);
	WorldPacket data(SMSG_MOVE_KNOCK_BACK, 30);
	data << ptr->GetNewGUID();
	data << getMSTime();
	data << dx << dy << affect1 << affect2;
	ptr->SendMessageToSet(&data, true);

	return 0;
}

int luaUnit_SendAreaTriggerMessage(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	const char * msg = luaL_checkstring(L,1);
	if(!msg) return 0;
	((Player*)ptr)->SendAreaTriggerMessage(msg);
	return 0;
}

int luaUnit_SendBroadcastMessage(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	const char * msg = luaL_checkstring(L,1);
	if(!msg) return 0;
	((Player*)ptr)->BroadcastMessage(msg);
	return 0;
}

int luaUnit_TeleportUnit(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	uint32 mapId = luaL_checkint(L, 1);
	float posX = (float)luaL_checknumber(L, 2);
	float posY = (float)luaL_checknumber(L, 3);
	float posZ = (float)luaL_checknumber(L, 4);
	if(!mapId || !posX || !posY || !posZ)
		return 0;
	LocationVector vec(posX,posY,posZ);
	static_cast<Player*>( ptr ) ->SafeTeleport(mapId,0,vec);
	return 0;
}

int luaUnit_GetHealth(lua_State * L, Unit * ptr)
{
	if( ptr == NULL )
		lua_pushinteger( L, 0 );
	else
		lua_pushinteger( L, ptr->GetUInt32Value( UNIT_FIELD_HEALTH ) );

	return 1;
}

int luaUnit_GetMaxHealth(lua_State * L, Unit * ptr)
{
	if( ptr == NULL )
		lua_pushinteger( L, 0 );
	else
		lua_pushinteger( L, ptr->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );

	return 1;
}

int luaUnit_SetHealth(lua_State * L, Unit * ptr)
{
	int val = luaL_checkint( L, 1 );
	if( ptr != NULL && val > 0 )
		if( (uint32)val > ptr->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) )
			ptr->SetUInt32Value( UNIT_FIELD_HEALTH, ptr->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
		else
			ptr->SetUInt32Value( UNIT_FIELD_HEALTH, val );
	return 1;
}

int luaUnit_SetMaxHealth(lua_State * L, Unit * ptr)
{
	int val = luaL_checkint( L, 1 );
	if( ptr != NULL && val > 0 )
		if( (uint32)val < ptr->GetUInt32Value( UNIT_FIELD_HEALTH ) )
			ptr->SetUInt32Value( UNIT_FIELD_HEALTH, val );
		ptr->SetUInt32Value( UNIT_FIELD_MAXHEALTH, val );
	return 1;
}

int luaUnit_WipeHateList(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET_INT(TYPEID_UNIT);
	ptr->WipeHateList();
	return 1;
}

int luaUnit_WipeTargetList(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET_INT(TYPEID_UNIT);
	ptr->GetAIInterface()->WipeTargetList();
	return 1;
}

int luaUnit_WipeCurrentTarget(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET_INT(TYPEID_UNIT);
	ptr->GetAIInterface()->WipeCurrentTarget();
	return 1;
}

int luaUnit_GetPlayerClass(lua_State * L, Unit * ptr)
{
   if(!ptr || ptr->GetTypeId()!=TYPEID_PLAYER) {
      lua_pushstring(L, "Unknown");
      return 1;
   }
   int plrclass = ((Player*)ptr)->getClass();

   switch(plrclass) {
      case 1:
         lua_pushstring(L, "Warrior");
         break;
      case 2:
         lua_pushstring(L, "Paladin");
         break;
      case 3:
         lua_pushstring(L, "Hunter");
         break;
      case 4:
         lua_pushstring(L, "Rogue");
         break;
      case 5:
         lua_pushstring(L, "Priest");
         break;
      //case 6:
      case 7:
         lua_pushstring(L, "Shaman");
         break;
      case 8:
         lua_pushstring(L, "Mage");
         break;
      case 9:
         lua_pushstring(L, "Warlock");
         break;
      //case 10:
      case 11:
         lua_pushstring(L, "Druid");
         break;
      default:
         lua_pushstring(L, "Unknown");
         break;
   }

   return 1;
}


int luaUnit_ClearHateList(lua_State * L, Unit * ptr)
{
 	CHECK_TYPEID_RET_INT(TYPEID_UNIT);
 	ptr->ClearHateList();
 	return 1;
}
int luaUnit_SetMana(lua_State * L, Unit * ptr)
{
	int val = luaL_checkint( L, 1);
	if( ptr != NULL)
		ptr->SetUInt32Value( UNIT_FIELD_POWER1, val );
	return 1;
}
int luaUnit_SetMaxMana(lua_State * L, Unit * ptr)
{
	int val = luaL_checkint( L, 1);
	if( ptr != NULL && val > 0 )
		if( (uint32)val < ptr->GetUInt32Value( UNIT_FIELD_POWER1) )
			ptr->SetUInt32Value( UNIT_FIELD_POWER1, val);
		ptr->SetUInt32Value( UNIT_FIELD_MAXPOWER1, val );
	return 1;
}
int luaUnit_GetPlayerRace(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET( TYPEID_PLAYER );
	lua_pushinteger( L, static_cast< Player* >( ptr )->getRace() );
	return 1;
}
int luaUnit_SetFlying(lua_State * L, Unit * ptr)
{
	WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
	data << ptr->GetNewGUID();
    data << uint32(0);
	ptr->SendMessageToSet(&data, true);
	ptr->GetAIInterface()->disable_melee = true;
	ptr->GetAIInterface()->m_moveFly = true;
	ptr->Emote(EMOTE_ONESHOT_LIFTOFF);
	return 0;
}
 
int luaUnit_Land(lua_State * L, Unit * ptr)
{
	WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
	data << ptr->GetNewGUID();
    data << uint32(0);
	ptr->SendMessageToSet(&data, true);
	ptr->GetAIInterface()->m_moveFly = false;
	ptr->GetAIInterface()->disable_melee = false;
	ptr->Emote(EMOTE_ONESHOT_LAND);
	return 0;
}
int luaUnit_HasAura(lua_State * L, Unit * ptr)
{
	int spellid = luaL_checkint (L,1);
	if (!ptr || !spellid )
		return 0;
	else
		if(ptr->HasAura(spellid))
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
	return 1;
}
int luaUnit_ReturnToSpawnPoint(lua_State * L, Unit * ptr)
{
	float x = ptr->GetSpawnX();
	float y = ptr->GetSpawnY();
	float z = ptr->GetSpawnZ();
	float o = ptr->GetSpawnO();
	if(ptr)
		ptr->GetAIInterface()->MoveTo(x, y, z, o);
	return 0;
}
int luaUnit_GetGUID(lua_State * L, Unit * ptr)
{
	if(!ptr) return 0;
	lua_pushinteger(L,(int)ptr->GetGUID()); return 0;
}
int luaUnit_GetDistance(lua_State * L, Unit * ptr)
{
	if(!ptr)
		return 0;
	Unit * target = Lunar<Unit>::check(L, 1);
	lua_pushnumber(L,(float)ptr->GetDistance2dSq(target));
	return 1;
}
int luaUnit_GetCreatureNearestCoords(lua_State * L, Unit * ptr)
{
    if(!ptr) return 0;
    uint32 entryid = luaL_checkint(L,4);
    float x = (float)luaL_checknumber(L,1);
    float y = (float)luaL_checknumber(L,2);
    float z = (float)luaL_checknumber(L,3);
    if(entryid==NULL) 
        lua_pushnil(L);
    else
        Lunar<Unit>::push(L,ptr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(x, y, z, entryid), false);
    return 1;
    
}
int luaUnit_GetGameObjectNearestCoords(lua_State *L, Unit * ptr)
{
    if(!ptr) return 0;
    uint32 entryid = luaL_checkint(L,4);
    float x = (float)luaL_checknumber(L,1);
    float y = (float)luaL_checknumber(L,2);
    float z = (float)luaL_checknumber(L,3);
    if(entryid==NULL) 
        lua_pushnil(L);
    else
        Lunar<GameObject>::push(L,ptr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(x, y, z, entryid), false);
    return 1;
}
int luaUnit_CastSpellAoF(lua_State * L, Unit * ptr)
{
    CHECK_TYPEID(TYPEID_UNIT);
    uint32 sp = luaL_checkint(L, 4);
    float x = (float)luaL_checknumber(L,1);
    float y = (float)luaL_checknumber(L,2);
    float z = (float)luaL_checknumber(L,3);
    if(!sp | !x | !y | !z)
        return 0;
	ptr->CastSpellAoF(x,y,z,dbcSpell.LookupEntry(sp), true);
	return 0;
}
int luaUnit_SetInFront(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	if(!target)
		return 0;
	ptr->GetAIInterface()->setInFront(target);
	return 1;
}
int luaUnit_RemoveAllAuras(lua_State *L, Unit * ptr)
{
	if (!ptr)
		return 0;
	ptr->RemoveAllAuras();
	return 1;
}
int luaUnit_CancelSpell(lua_State * L, Unit * ptr)
{
	if(!ptr)
		return 0;
	ptr->CancelSpell(ptr->GetCurrentSpell());
	return 0;
}
int luaUnit_IsAlive(lua_State * L, Unit * ptr)
{
	if(ptr)
		if(ptr->isAlive())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaUnit_IsDead(lua_State * L, Unit * ptr)
{
	if(ptr)
		if(ptr->isDead())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaUnit_IsInWorld(lua_State * L, Unit * ptr)
{
	if (ptr)
		if(ptr->IsInWorld())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaUnit_GetZoneId(lua_State *L, Unit * ptr)
{
	if(!ptr)
		return 0;
	lua_pushinteger(L,(ptr->GetZoneId()));
	return 1;
}
int luaUnit_GetMana(lua_State * L, Unit * ptr)
{
	if( ptr == NULL )
		lua_pushinteger( L, 0 );
	else
		lua_pushinteger( L, ptr->GetUInt32Value( UNIT_FIELD_POWER1 ) );

	return 1;
}

int luaUnit_GetMaxMana(lua_State * L, Unit * ptr)
{
	if( ptr == NULL )
		lua_pushinteger( L, 0 );
	else
		lua_pushinteger( L, ptr->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );

	return 1;
}
int luaUnit_Root(lua_State * L, Unit * ptr)
{
	if(ptr)
		ptr->Root();
	return 0;
}
int luaUnit_Unroot(lua_State * L, Unit * ptr)
{
	if(ptr)
		ptr->Unroot();
	return 0;
}
int luaUnit_IsCreatureMoving(lua_State * L, Unit * ptr)
{
	if(ptr)
		if(ptr->GetAIInterface()->m_creatureState == MOVING)
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaUnit_SetOutOfCombatRange(lua_State * L, Unit * ptr)
{
	int range = luaL_checkint(L, 1);
	if(ptr != NULL || range != NULL)
		ptr->GetAIInterface()->setOutOfCombatRange(range);
	return 0;
}
int luaUnit_ModifyRunSpeed(lua_State * L, Unit * ptr)
{
	float Speed = (float)luaL_checkint(L, 1);
	if(ptr)
		ptr->m_runSpeed = Speed;
	return 0;
}
int luaUnit_ModifyWalkSpeed(lua_State * L, Unit * ptr)
{
	float Speed = (float)luaL_checkint(L,1);
	if(ptr&&Speed)
		ptr->m_walkSpeed = Speed;
	return 0;
}
int luaUnit_ModifyFlySpeed(lua_State * L, Unit * ptr)
{
	float Speed = (float)luaL_checkint(L,1);
	if(ptr&&Speed)
		ptr->m_flySpeed = Speed;
	return 0;
}
int luaUnit_IsFlying(lua_State * L, Unit * ptr)
{
	if(ptr)
		if(ptr->GetAIInterface()->IsFlying())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaUnit_SetRotation(lua_State * L, Unit * ptr)
{
	if(ptr)
		ptr->SetRotation(ptr->GetGUID());
	return 1;
}
int luaUnit_SetOrientation(lua_State * L, Unit * ptr)
{
	float O = (float)luaL_checknumber(L, 1);
	if(ptr)
		ptr->SetOrientation(O);
	return 0;
}
int luaUnit_CalcDistance(lua_State * L, Unit * ptr)
{
	Object * ob = Lunar<Object>::check(L,1);
	if(ob)
		lua_pushnumber(L,(float)ptr->CalcDistance(ob));
	return 1;
}
int luaUnit_GetSpawnX(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnX());
	return 1;
}
int luaUnit_GetSpawnY(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnY());
	return 1;
}
int luaUnit_GetSpawnZ(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnZ());
	return 1;
}
int luaUnit_GetSpawnO(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnO());
	return 1;
}
int luaUnit_GetInRangePlayersCount(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetInRangePlayersCount());
	return 1;
}
int luaUnit_GetEntry(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetEntry());
	return 1;
}
int luaUnit_SetMoveRunFlag(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	int enabled = luaL_checkint(L, 1);
	ptr->GetAIInterface()->setMoveRunFlag((enabled > 0) ? true : false);
	return 0;
}

int luaUnit_HandleEvent(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	Unit * target = Lunar<Unit>::check(L, 1);
	int event_id = luaL_checkint(L, 2);
	int misc_1 = luaL_checkint(L, 3);
	ptr->GetAIInterface()->HandleEvent(event_id, target, misc_1);
	return 1;
}
int luaUnit_GetCurrentSpellId(lua_State * L, Unit * ptr)
{
	if(ptr->GetCurrentSpell())
		lua_pushnumber(L, ptr->GetCurrentSpell()->GetSpellProto()->Id);
	else 
		lua_pushnil(L);
	return 1;
}
int luaUnit_GetCurrentSpell(lua_State * L, Unit * ptr)
{
	if(ptr)
		if(ptr->GetCurrentSpell() != NULL)
		{
			lua_pushlstring(L,ptr->GetCurrentSpell()->GetSpellProto()->Name,NULL);
		}
	return 1;
}
int luaUnit_AddAssistTargets(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	Unit * Friend = Lunar<Unit>::check(L,1);
	if(ptr && Friend)
	{
		if( isFriendly(ptr, Friend))
			ptr->GetAIInterface()->addAssistTargets(Friend);
	}
	return 0;
}
int luaUnit_GetAIState(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	lua_pushnumber(L, ptr->GetAIInterface()->getAIState());
	return 1;
}

int luaUnit_GetFloatValue(lua_State * L, Unit * ptr)
{
	int field = luaL_checkint(L, 1);
	if( ptr != NULL ) 
		lua_pushnumber(L, ptr->GetFloatValue(field));
	return 1;
}

int luaUnit_InitPacket(lua_State * L, Unit * ptr)
{
	int packet_id = luaL_checkint(L, 1);
	WorldPacket data;
	data.Initialize(packet_id);
	return 1;
}

int luaUnit_AddDataToPacket(lua_State * L, Unit * ptr)
{
	WorldPacket dat;
	int type = luaL_checkint(L, 1);
	switch(type)
	{
		case 0:
			// int
			dat << luaL_checkint(L, 2);
			break;
		case 1:
			// uint8
			dat << (uint8)luaL_checknumber(L, 2);
			break;
		case 2:
			// uint16
			dat << (uint16)luaL_checknumber(L, 2);
			break;
		case 3:
			// uint32
			dat << (uint32)luaL_checknumber(L, 2);
			break;
		case 4:
			// uint64
			dat << (uint64)luaL_checknumber(L, 2);
			break;
		case 5:
			// float
			dat << (float)luaL_checknumber(L, 2);
			break;
		case 6:
			// double
			dat << (double)luaL_checknumber(L, 2);
			break;
		case 7:
			// string
			dat << luaL_checkstring(L, 2);
			break;
	}
	return 1;
}

int luaUnit_AddGuidDataToPacket(lua_State * L, Unit * ptr)
{
	WorldPacket dat;
	int type = luaL_checkint(L, 1);
	switch(type)
	{
		// not sure... maybee we should use ptr->GetMapMgr->GetGUID??
		case 0:
			dat << ptr->GetGUID();
			break;
		case 1:
			dat << ptr->GetTypeFromGUID();
			break;
		case 2:
			dat << ptr->GetUIdFromGUID();
			break;
		case 3:
			dat << ptr->GetNewGUID(); 
			break;
	}
	return 1;
}

int luaUnit_SendData(lua_State * L, Unit * ptr)
{
	WorldPacket dat;
	ptr->SendMessageToSet(&dat, true);
	return 1;
}

int luaUnit_ModUInt32Value(lua_State * L, Unit * ptr)
{
	int field = luaL_checkint(L, 1);
	int value = luaL_checkint(L, 2);
	if( ptr != NULL ) ptr->ModSignedInt32Value(field, value);
	return 1;
}

int luaUnit_ModFloatValue(lua_State * L, Unit * ptr)
{
	int field = luaL_checkint(L, 1);
	float value = (float)luaL_checknumber(L, 2);
	if( ptr != NULL ) ptr->ModFloatValue(field, value);
	return 1;
}

int luaUnit_SetUInt32Value(lua_State * L, Unit * ptr)
{
	int field = luaL_checkint(L, 1);
	int value = luaL_checkint(L, 2);
	if( ptr != NULL ) ptr->SetUInt32Value(field, value);
	return 1;
}

int luaUnit_SetUInt64Value(lua_State * L, Unit * ptr)
{
	int field = luaL_checkint(L, 1);
	int value = luaL_checkint(L, 2);
	if( ptr != NULL ) ptr->SetUInt64Value(field, value);
	return 1;
}

int luaUnit_SetFloatValue(lua_State * L, Unit * ptr)
{
	int field = luaL_checkint(L, 1);
	float value = (float)luaL_checknumber(L, 2);
	if( ptr != NULL ) ptr->SetFloatValue(field, value);
	return 1;
}

int luaUnit_GetUInt32Value(lua_State * L, Unit * ptr)
{
	int field = luaL_checkint(L, 1);
	if( ptr != NULL ) 
		lua_pushnumber(L, ptr->GetUInt32Value(field));
	return 1;
}

int luaUnit_GetUInt64Value(lua_State * L, Unit * ptr)
{
	int field = luaL_checkint(L, 1);
	if( ptr != NULL )
		lua_pushinteger(L, (int)ptr->GetUInt64Value(field));
	return 1;
}

int luaUnit_AdvanceQuestObjective(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	int questid = luaL_checkint(L,1);
	int objective = luaL_checkint(L,2);
	Player * pl = ((Player*)ptr);
	QuestLogEntry * qle = pl->GetQuestLogForEntry(questid);
	qle->SetMobCount(objective, qle->GetMobCount(objective) + 1);
	qle->SendUpdateAddKill(objective);
	if(qle->CanBeFinished())
		qle->SendQuestComplete();

	qle->UpdatePlayerFields();
	return 0;
}
int luaUnit_Heal(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	uint32 spellid = luaL_checkint(L, 2);
	uint32 amount = luaL_checkint(L, 3);
	if (!target || !spellid || !amount || !ptr)
		return 0;
	ptr->Heal(target,spellid,amount);
	return 0;
}
int luaUnit_Energize(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	uint32 spellid = luaL_checkint(L, 2);
	uint32 amount = luaL_checkint(L, 3);
	uint32 type = luaL_checkint(L, 4);
	if(!target||!spellid||!amount||!type||!ptr)
		return 0;
	ptr->Energize(target,spellid,amount,type);
	return 0;
}
int luaUnit_SendChatMessageAlternateEntry(lua_State * L, Unit * ptr)
{
	uint32 entry = luaL_checkint(L, 1);
	uint8 type = luaL_checkint(L, 2);
	uint32 lang = luaL_checkint(L, 3);
	const char * msg = luaL_checkstring(L, 4);
	if(!entry||!type||!lang||!msg)
		return 0;
	ptr->SendChatMessageAlternateEntry(entry,type,lang,msg);
	return 0;
}
int luaUnit_SendChatMessageToPlayer(lua_State * L, Unit * ptr)
{
	uint8 type = luaL_checkint(L, 1);
	uint32 lang = luaL_checkint(L, 2);
	const char *msg = luaL_checkstring(L,3);
	Player *plr = Lunar<Player>::check(L,4);
	if(!plr||!msg)
		return 0;
	ptr->SendChatMessageToPlayer(type,lang,msg,plr);
	return 0;
}
int luaUnit_SetPowerType(lua_State * L, Unit * ptr)
{
	const char * message = luaL_checklstring(L,1,NULL);
	if(!ptr||!message)
		return 0;
	if( message == "health")
		ptr->SetPowerType(POWER_TYPE_HEALTH);
	else if( message == "mana")
		ptr->SetPowerType(POWER_TYPE_MANA);
	else if(message == "rage")
		ptr->SetPowerType(POWER_TYPE_RAGE);
	else if(message == "focus")
		ptr->SetPowerType(POWER_TYPE_FOCUS);
	else if (message == "energy")
		ptr->SetPowerType(POWER_TYPE_ENERGY);
	return 1;
}
int luaUnit_GetPowerType(lua_State * L, Unit * ptr)
{
	if(ptr)
	{
		switch(ptr->GetPowerType())
		{
		case POWER_TYPE_HEALTH:
			lua_pushstring(L,"Health");
		case POWER_TYPE_MANA:
			lua_pushstring(L, "Mana");
		case POWER_TYPE_RAGE:
			lua_pushstring(L, "Rage");
		case POWER_TYPE_FOCUS:
			lua_pushstring(L, "Focus");
		case POWER_TYPE_ENERGY:
			lua_pushstring(L, "Energy");
		}
	}
	return 1;
}
int luaUnit_Strike(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET(TYPEID_UNIT);

	Unit * target = Lunar<Unit>::check(L, 1);
	uint32 weapon_damage_type = luaL_checkint(L, 2);
	uint32 sp = luaL_checkint(L, 3);
	int32 adddmg = luaL_checkint(L, 4);
	uint32 exclusive_damage = luaL_checkint(L, 5);
	int32 pct_dmg_mod = luaL_checkint(L, 6);

	if(!target||!weapon_damage_type||!sp||!adddmg||!pct_dmg_mod)
		return 0;
	ptr->Strike(target,weapon_damage_type,dbcSpell.LookupEntry(sp),adddmg,pct_dmg_mod,exclusive_damage,false,false);
	return 0;
}
int luaUnit_SetAttackTimer(lua_State * L, Unit * ptr)
{
	int32 timer = luaL_checkint(L, 1);
	uint32 offhand = luaL_checkint(L,2);
	if(!timer||!ptr||!offhand||offhand != 1||offhand != 0)
		return 0;
	ptr->setAttackTimer(timer,false);
	return 0;
}
int luaUnit_Kill(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	Unit * target = Lunar<Unit>::check(L, 1);
	if (!ptr ||!target)
		return 0;
	ptr->DealDamage(target,target->GetUInt32Value(UNIT_FIELD_HEALTH),0,0,0,true);
	return 0;
}
int luaUnit_DealDamage(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	Unit * target = Lunar<Unit>::check(L, 1);
	uint32 damage = luaL_checkint(L, 2);
	uint32 spellid = luaL_checkint(L, 3);
	if(!ptr||!target)
		return 0;
	ptr->DealDamage(target,damage,0,0,spellid,true);
	return 0;
}
int luaUnit_SetNextTarget(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	if(ptr && target)
		ptr->GetAIInterface()->SetNextTarget(target);
	return 0;
}
int luaUnit_GetNextTarget(lua_State * L, Unit * ptr)
{
	if(ptr || ptr->IsInWorld())
		Lunar<Unit>::push(L, ptr->GetAIInterface()->GetNextTarget());
	return 0;
}
int luaUnit_SetPetOwner(lua_State * L, Unit * ptr)
{
	Unit * owner = Lunar<Unit>::check(L, 1);
	if(ptr || owner)
		ptr->GetAIInterface()->SetPetOwner(owner);
	return 0;
}
int luaUnit_DismissPet(lua_State * L, Unit * ptr)
{
	if(ptr)
		ptr->GetAIInterface()->DismissPet();
	return 0;
}
int luaUnit_IsPet(lua_State * L, Unit * ptr)
{
	if(ptr)
		if(ptr->IsPet())
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaUnit_GetPetOwner(lua_State * L, Unit * ptr)
{
	if(ptr)
		Lunar<Unit>::push(L, ptr->GetAIInterface()->GetPetOwner());
	return 0;
}
int luaUnit_SetUnitToFollow(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	float dist = (float)luaL_checkint(L, 2);
	float angle = (float)luaL_checkint(L, 3);
	if(!target||!ptr||!dist||dist<=0||!angle)
		return 0;
	ptr->GetAIInterface()->SetUnitToFollow(target);
	ptr->GetAIInterface()->SetFollowDistance(dist);
	ptr->GetAIInterface()->SetUnitToFollowAngle(angle);
	return 0;
}
int luaUnit_GetUnitToFollow(lua_State * L, Unit * ptr)
{
	if(ptr)
		Lunar<Unit>::push(L,ptr->GetAIInterface()->getUnitToFollow());
	return 0;
}
int luaUnit_IsInFront(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	if(ptr && target)
		if(ptr->isInFront(target))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}

int luaUnit_IsInBack(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	if(ptr && target)
		if(ptr->isInBack(target))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaUnit_IsPacified(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushboolean(L,(ptr->IsPacified())?1:0);
	return 1;
}
int luaUnit_IsFeared(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushboolean(L,(ptr->IsFeared())?1:0);
	return 1;
}
int luaUnit_IsStunned(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushboolean(L,(ptr->IsStunned())?1:0);
	return 1;
}
int luaUnit_CreateGuardian(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);

	uint32 guardian_entry = luaL_checkint(L,1);
	uint32 duration = luaL_checkint(L, 2);
	float angle = (float)luaL_checkint(L, 3);
	uint32 lvl  = luaL_checkint(L, 4);
	if(!ptr||!guardian_entry||!lvl)
		return 0;
	ptr->create_guardian(guardian_entry,duration,angle,lvl);
	return 0;
}
/*int luaUnit_IsInArc(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	float degrees = (float)luaL_checkint(L, 2);
	if(!target||!ptr||!degrees)
		return 0;
	else
		if(ptr->isInArc(target,degrees))
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
	return 1;
}*/
int luaUnit_IsInWater(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	if(ptr)
		if(static_cast< Player* >( ptr )->m_UnderwaterState)
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
	
int luaUnit_GetAITargetsCount(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetAIInterface()->getAITargetsCount());
	return 1;
}
int luaUnit_GetUnitByGUID(lua_State * L, Unit * ptr)
{
	uint64 guid = luaL_checkint(L,1);
	if(ptr && guid)
		Lunar<Unit>::push(L,ptr->GetMapMgr()->GetUnit(guid));
	return 1;
}

/*int luaUnit_GetAITargets(lua_State * L, Unit * ptr)
{
	Unit * ret = NULL;
	TargetMap::iterator itr;
	lua_newtable(L);
	int count = 0;
	for( itr = ptr->GetAIInterface()->GetAITargets()->begin();itr!= ptr->GetAIInterface()->GetAITargets()->end();itr++)
	{
		ret = ptr->GetMapMgr()->GetUnit(itr->first);
		count++;
		lua_pushvalue(L,count);
		Lunar<Unit>::push(L,ret,false);
		lua_rawset(L,-3);
	}
	return 1;
}*/
int luaUnit_GetInRangeObjectsCount(lua_State * L, Unit * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetInRangeCount());
	return 1;
}
int luaUnit_GetInRangePlayers(lua_State * L, Unit * ptr)
{
	Player * ret = NULL;
	uint32 count = 0;
	lua_newtable(L);
	for(std::set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); itr++)
	{
		if( (*itr) ->GetTypeId() == TYPEID_PLAYER)
		{
			count++,
			ret = *itr;
			lua_pushinteger(L,count);
			Lunar<Unit>::push(L,((Unit*)ret),false);
			lua_rawset(L,-3);
		}
	}
	return 1;
}
int luaUnit_GetInRangeGameObjects(lua_State * L, Unit * ptr)
{
	Object * ret = NULL;
	lua_newtable(L);
	uint32 count = 0;
	for (std::set<Object*>::iterator itr = ptr->GetInRangeSetBegin();itr!= ptr->GetInRangeSetEnd();itr++)
	{
		if( (*itr) ->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			count++,
			lua_pushinteger(L,count);
			Lunar<GameObject>::push(L,(GameObject*)(*itr));
			lua_rawset(L,-3);
		}
	}
	return 1;
}
	
int luaUnit_HasInRangeObjects(lua_State * L, Unit * ptr)
{
	if(ptr)
		if(ptr->HasInRangeObjects())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaUnit_SetFacing(lua_State * L, Unit * ptr)
{
	float newo = (float)luaL_checkint(L, 1);
	if(!ptr|!newo)
		return 0;
	ptr->SetFacing(newo);
	return 0;
}
int luaUnit_CalcToDistance(lua_State * L, Unit * ptr)
{
	float x = (float)luaL_checkint(L,1 );
	float y = (float)luaL_checkint(L, 2);
	float z = (float)luaL_checkint(L, 3);
	if (!ptr|!x|!y|!z)
		return 0;
	lua_pushnumber(L,ptr->CalcDistance(x,y,z));
	return 1;
}
int luaUnit_CalcAngle(lua_State * L, Unit * ptr)
{
	float x = (float)luaL_checkint(L,1 );
	float y = (float)luaL_checkint(L, 2);
	float x2 = (float)luaL_checkint(L, 3);
	float y2 = (float)luaL_checkint(L, 4);
	if(!x||!y||!x2||!y2||!ptr)
		return 0;
	lua_pushnumber(L,ptr->calcAngle(x,y,x2,y2));
	return 1;
}
int luaUnit_CalcRadAngle(lua_State * L, Unit * ptr)
{
	float ang = NULL;
	float x = (float)luaL_checkint(L,1 );
	float y = (float)luaL_checkint(L, 2);
	float dx = (float)luaL_checkint(L, 3);
	float dy = (float)luaL_checkint(L, 4);
	if(!x||!y||!dx||!dy||!ptr)
		return 0;
	else
		ang = ptr->calcRadAngle(x,y,dx,dy);
	lua_pushnumber(L,ang);
	return 1;
}
int luaUnit_IsInvisible(lua_State * L, Unit * ptr)
{
	int enabled = luaL_checkint(L, 1);
	if(!ptr|!enabled)
		return 0;
	if(enabled > 0)
		ptr->m_invisFlag = INVIS_FLAG_TOTAL;
		ptr->m_invisible = true;
	return 1;
}
int luaUnit_MoveFly(lua_State * L, Unit * ptr)
{
	int enabled = luaL_checkint(L,1);
	if(ptr)
		ptr->GetAIInterface()->m_moveFly = ((enabled > 0)?true:false);
	return 1;
}
int luaUnit_IsInvincible(lua_State * L, Unit * ptr)
{
	int enabled = luaL_checkint(L, 1);
	if(ptr)
		ptr->bInvincible = ((enabled > 0)? true : false );
	return 0;
}
int luaUnit_ResurrectPlayer(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	if(ptr)
		static_cast< Player * > ( ptr ) ->ResurrectPlayer( TO_PLAYER(ptr) );
	return 0;
}
int luaUnit_KickPlayer(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	int delay = luaL_checkint(L,1);
	if(ptr)
		static_cast<Player*>(ptr)->Kick(delay);
	return 0;
}
int luaUnit_CanCallForHelp(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT)
	int enabled = luaL_checkint(L, 1);
	if(!ptr|!enabled)
		return 0;
	ptr->GetAIInterface()->m_canCallForHelp = ((enabled < 0)? true: false);
	return 0;
}
int luaUnit_CallForHelpHp(lua_State * L, Unit * ptr)
{
	float hp = (float)luaL_checkint(L, 1);
	if(ptr)
		ptr->GetAIInterface()->m_CallForHelpHealth = hp;
	return 0;
}
int luaUnit_SetDeathState(lua_State * L, Unit * ptr)
{
	int state = luaL_checkint(L, 1);
	if(ptr)
		switch(state)
	{
		case 0:
			ptr->setDeathState (ALIVE);
			break;
		case 1:
			ptr->setDeathState (JUST_DIED);
			break;
		case 2:
			ptr->setDeathState (CORPSE);
			break;
		case 3:
			ptr->setDeathState (DEAD);
			break;
	}
	return 1;
}
int luaUnit_SetCreatureName(lua_State * L, Unit * ptr)
{
	uint32 id = luaL_checkint(L,1);
	if(!ptr|!id)
		return 0;
	static_cast<Creature*>(ptr)->SetCreatureName(CreatureNameStorage.LookupEntry(id));
	return 0;
}
int luaUnit_GetSpellId(lua_State * L, Unit * ptr)
{
	uint32 spellid = luaL_checkint(L,1);
	if(ptr)
		Lunar<SpellEntry>::push(L,ptr->GetAIInterface()->getSpellEntry(spellid));
	return 1;
}
int luaUnit_SetNextSpell(lua_State * L, Unit * ptr)
{
	SpellEntry * sp = dbcSpell.LookupEntry(luaL_checkint(L,1));
	if(ptr || sp)
		ptr->GetAIInterface()->SetNextSpell((AI_Spell*)sp);
	return 0;
}
int luaUnit_Possess(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);

	Unit * target = Lunar<Unit>::check(L,1);
	if(target)
		static_cast<Player*>(ptr)->Possess(target);
	return 0;
}
int luaUnit_Unpossess(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	Player* target = Lunar<Player>::check(L,1);
	if(target)
		target->UnPossess();
	return 0;
}
int luaUnit_RemoveFromWorld(lua_State * L, Unit * ptr)
{
	if(ptr)
		ptr->RemoveFromWorld(true);
	return 0;
}
int luaUnit_GetFaction(lua_State * L, Unit * ptr)
{
	if(ptr)
	{
		lua_pushnumber(L,ptr->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	}
	return 1;
}
int luaUnit_SpellNonMeleeDamageLog(lua_State * L, Unit * ptr)
{
	Unit * pVictim = Lunar<Unit>::check(L,1);
	uint32 spellid = luaL_checkint(L,2);
	uint32 damage = luaL_checkint(L,3);
	int allowproc = luaL_checkint(L,4);
	int static_dmg = luaL_checkint(L,5);
	int no_remove_auras = luaL_checkint(L,6);
	if(pVictim&&spellid&&damage&&allowproc&&static_dmg&&no_remove_auras)
	{
		ptr->SpellNonMeleeDamageLog(pVictim,spellid,damage,((allowproc >0)?true:false),((static_dmg>0)?true:false),((no_remove_auras>0)?true:false));
	}
	return 1;
}
int luaUnit_NoRespawn(lua_State * L, Unit * ptr)
{
	int enabled = luaL_checkint(L,1);
	if(ptr&&enabled)
		static_cast<Creature*>( ptr ) ->m_noRespawn = ((enabled < 0)? true:false);
	return 1;
}
int luaUnit_GetMapId(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET(TYPEID_UNIT || TYPEID_PLAYER);

	if(ptr->GetMapId() == NULL)
		lua_pushnil(L);
	else
		lua_pushinteger(L,ptr->GetMapId());
	return 1;
}
int luaUnit_AttackReaction(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L,1);
	uint32 damage = luaL_checkint(L,2);
	uint32 spell = luaL_checkint(L,3);
	if(ptr && target && damage)
	{
		ptr->GetAIInterface()->AttackReaction(target,damage,spell);
	}
	return 1;
}
int luaUnit_EventCastSpell(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT || TYPEID_PLAYER);
	Unit * target = Lunar<Unit>::check(L, 1);
	uint32 sp = luaL_checkint(L,2);
	uint32 delay = luaL_checkint(L,3);
	uint32 repeats = luaL_checkint(L,4);
	if ( ptr && sp)
	{
		switch(ptr->GetTypeId())
		{
		case TYPEID_PLAYER:
			sEventMgr.AddEvent(ptr, &Player::EventCastSpell,target,dbcSpell.LookupEntry(sp),EVENT_PLAYER_UPDATE,delay,repeats,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		case TYPEID_UNIT:
			sEventMgr.AddEvent(ptr, &Unit::EventCastSpell,target,dbcSpell.LookupEntry(sp),EVENT_CREATURE_UPDATE,delay,repeats,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
	}
	return 0;
}
int luaUnit_IsPlayerMoving(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET(TYPEID_PLAYER);
	if( static_cast<Player*>( ptr ) ->m_isMoving)
		lua_pushboolean(L,1);
	else
		lua_pushboolean(L,0);
	return 1;
}
int luaUnit_IsPlayerAttacking(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET(TYPEID_PLAYER);
	if( static_cast<Player*>( ptr ) ->IsAttacking())
		lua_pushboolean(L,1);
	else
		lua_pushboolean(L,0);
	return 1;
}
int luaUnit_GetFactionStanding(lua_State * L, Unit * ptr)
{
	uint32 faction = luaL_checkint(L,1);
	if (ptr && faction)
	{
		switch(((Player*)ptr)->GetStanding(faction))
		{
		case STANDING_HATED:
			lua_pushstring(L, "Hated");
		case STANDING_HOSTILE:
			lua_pushstring(L, "Hostile");
		case STANDING_UNFRIENDLY:
			lua_pushstring(L, "Unfriendly");
		case STANDING_NEUTRAL:
			lua_pushstring(L, "Neutral");
		case STANDING_FRIENDLY:
			lua_pushstring(L, "Friendly");
		case STANDING_HONORED:
			lua_pushstring(L, "Honored");
		case STANDING_REVERED:
			lua_pushstring(L, "Revered");
		case STANDING_EXALTED:
			lua_pushstring(L, "Exalted");
		}
	}
return 1;
}
int luaUnit_SetPlayerAtWar(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET(TYPEID_PLAYER);
	uint32 faction = luaL_checkint(L,1);
	uint32 set = luaL_checkint(L,2);
	if(ptr && faction && set)
	{
		((Player*)ptr)->SetAtWar(faction,((set > 0)? true : false));
	}
	return 0;
}
/*int luaUnit_IsPlayerAtWar(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET(TYPEID_PLAYER);
	uint32 faction = luaL_checkint(L,1);
	if (ptr && faction)
		if( ((Player*)ptr)->IsAtWar(faction))
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
	return 1;
}*/
int luaUnit_SetPlayerStanding(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	uint32 faction = luaL_checkint(L,1);
	uint32 value = luaL_checkint(L,2);
	if(faction && value)
		((Player*)ptr)->SetStanding(faction,value);
	return 0;
}
int luaUnit_GetStanding(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	uint32 faction = luaL_checkint(L,1);
	if(ptr && faction)
		lua_pushinteger(L,((Player*)ptr)->GetStanding(faction));
	return 0;
}
int luaUnit_RemoveThreatByPtr(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	if( ptr && target)
	{
		ptr->GetAIInterface()->RemoveThreatByPtr(target);
	}
	return 0;
}
int luaUnit_HasItem(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	uint32 itemid = luaL_checkint(L,1);
	if(ptr && itemid)
	{
		if( ((Player*)ptr)->GetItemInterface()->GetItemCount(itemid,false) > 0 )
			lua_pushboolean(L,1);
		else
			lua_pushboolean(L,0);
	}
	return 1;
}
int luaUnit_PlaySpellVisual(lua_State * L, Unit * ptr)
{
	Unit * target = Lunar<Unit>::check(L,1);
	uint32 spell = luaL_checkint(L,2);
	if( ptr && target && spell)
	{
		ptr->PlaySpellVisual(target->GetGUID(),spell);
	}
	return 0;
}
int luaUnit_GetPlayerLevel(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET(TYPEID_PLAYER);
	if(ptr)
	{
		lua_pushnumber(L,static_cast<Player*>( ptr ) ->getLevel());
	}
	return 0;
}
int luaUnit_SetPlayerLevel(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID_RET(TYPEID_PLAYER);
	uint32 level = luaL_checkint(L,1);
	if( level <= 70)
	{
		LevelInfo * Info = objmgr.GetLevelInfo(ptr->getRace(),ptr->getClass(),level);
		if (Info != 0)
		{
			static_cast<Player*>(ptr) ->ApplyLevelInfo(Info, level);
		}
	}
	return 0;
}
int luaUnit_AddSkill(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	uint32 skill = luaL_checkint(L,1);
	uint32 current = luaL_checkint(L,2);
	uint32 max = luaL_checkint(L,3);
	if(!max) max = 375;
	if(current > max)
		((Player*)ptr)->BroadcastMessage("CURRENT LEVEL CAN'T BE GREATER THAN MAX LEVEL");
		return 0;
	((Player*)ptr)->_AddSkillLine(skill,current,max);
	((Player*)ptr)->_UpdateMaxSkillCounts();
	return 0;
}
int luaUnit_RemoveSkill(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	uint32 skill = luaL_checkint(L,1);
	if(!ptr ||!skill) return 0;
	((Player*)ptr)->_RemoveSkillLine(skill);
	((Player*)ptr)->_UpdateMaxSkillCounts();
	return 1;
}
int luaUnit_FlyCheat(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	uint32 enabled = luaL_checkint(L,1);
	if(ptr && enabled)
		((Player*)ptr)->FlyCheat = ((enabled > 0)? true : false);
	return 0;
}
int luaUnit_AdvanceSkill(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	uint32 skill = luaL_checkint(L,1);
	uint32 count = luaL_checkint(L,2);
	if(skill && count)
	{
		if( ((Player*)ptr)->_HasSkillLine(skill))
			((Player*)ptr)->_AdvanceSkillLine(skill,count);
	}
	return 0;
}
int luaUnit_RemoveAurasByMechanic(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT || TYPEID_PLAYER);
	uint32 mechanic = luaL_checkint(L,1);
	uint32 hostileonly = luaL_checkint(L,2);
	if(ptr && mechanic && hostileonly)
	{
		ptr->RemoveAllAurasByMechanic(mechanic,-1,((hostileonly > 0)? true : false));
	}
	return 0;
}
int luaUnit_RemoveAurasType(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT || TYPEID_PLAYER);
	uint32 type = luaL_checkint(L,1);
	if(ptr && type)
	{
		ptr->RemoveAllAuraType(type);
	}
	return 0;
}
int luaUnit_AddAuraVisual(lua_State * L, Unit * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT || TYPEID_PLAYER);
	uint32 spellid = luaL_checkint(L,1);
	uint32 count = luaL_checkint(L,2);
	uint32 positive = luaL_checkint(L,3);
	if(ptr && spellid)
	{
		ptr->AddAuraVisual(spellid, count,((positive > 0)? true : false));
	}
	return 0;
}
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////GAMEOBJECT'S COMMANDS///////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
/*int luaGameObject_EventCastSpell(lua_State * L, GameObject * ptr)
{
	uint32 guid = luaL_checkint(L,1);
	uint32 sp = luaL_checkint(L,2);
	uint32 delay = luaL_checkint(L,3);
	uint32 repeats = luaL_checkint(L,4);
	if(guid && sp && delay)
	{
		sEventMgr.AddEvent(ptr,&GameObject::EventCastSpell,guid,sp,false,EVENT_GAMEOBJECT_UPDATE,delay,repeats,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	return 0;
}*/
int luaGameObject_GetMapId(lua_State * L, GameObject * ptr)
{
	if(ptr)
	{
		lua_pushinteger(L,ptr->GetMapId());
	}
	return 1;
}
int luaGameObject_RemoveFromWorld(lua_State * L, GameObject * ptr)
{
	if(ptr)
		ptr->RemoveFromWorld(true);
	return 0;
}
//int luaGameObject_GetAreaID(lua_State * L, GameObject * ptr)
//{
//	float x = (float)luaL_checkint(L, 1);
//	float y = (float)luaL_checkint(L, 2);
//	if(ptr)
//		lua_pushinteger(L,ptr->GetMapMgr()->GetAreaID(x, y));
//	return 1;
//}
int luaGameObject_GetName(lua_State * L, GameObject * ptr)
{
	if(!ptr||ptr->GetTypeId()!=TYPEID_GAMEOBJECT||!ptr->GetInfo())
	{
		lua_pushstring(L,"Unknown");
		return 1;
	}

	lua_pushstring(L,ptr->GetInfo()->Name);
	return 1;
}
int luaGameObject_Teleport(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
	Player* target = Lunar<Player>::check(L, 1);
	int mapId = luaL_checkint(L, 2);
	double posX = luaL_checknumber(L, 3);
	double posY = luaL_checknumber(L, 4);
	double posZ = luaL_checknumber(L, 5);
	if(!mapId || !posX || !posY || !posZ)
		return 0;
	LocationVector vec((float)posX, (float)posY, (float)posZ);
	((Player*)target)->SafeTeleport((uint32)mapId, 0, vec);
	return 0;
}
int luaGameObject_GetCreatureNearestCoords(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
    if(!ptr) return 0;
    uint32 entryid = luaL_checkint(L,4);
    float x = (float)luaL_checknumber(L,1);
    float y = (float)luaL_checknumber(L,2);
    float z = (float)luaL_checknumber(L,3);
    if(entryid==NULL) 
        lua_pushnil(L);
    else
        Lunar<Unit>::push(L,ptr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(x, y, z, entryid), false);
    return 1;
}
int luaGameObject_GetGameObjectNearestCoords(lua_State *L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
    if(!ptr) return 0;
    uint32 entryid = luaL_checkint(L,4);
    float x = (float)luaL_checknumber(L,1);
    float y = (float)luaL_checknumber(L,2);
    float z = (float)luaL_checknumber(L,3);
    if(entryid==NULL) 
        lua_pushnil(L);
    else
        Lunar<GameObject>::push(L,ptr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(x, y, z, entryid), false);
    return 1;
}
int luaGameObject_AddItem(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_PLAYER);
	int id = luaL_checkint(L,1);
	int count = luaL_checkint(L,2);

	Player * plr = (Player*)ptr;
	ItemPrototype * proto = ItemPrototypeStorage.LookupEntry(id);
	if(proto==NULL)
		return 0;

	Item * add = plr->GetItemInterface()->FindItemLessMax(id,count,false);
	if(add==NULL)
	{
		add=objmgr.CreateItem(id,plr);
		add->SetUInt32Value(ITEM_FIELD_STACK_COUNT,count);
		if(plr->GetItemInterface()->AddItemToFreeSlot(add))
			plr->GetSession()->SendItemPushResult(add,false,true,false,true,plr->GetItemInterface()->LastSearchItemBagSlot(),plr->GetItemInterface()->LastSearchItemSlot(),count);
		else
			delete add;
	}
	else
	{
		add->ModSignedInt32Value(ITEM_FIELD_STACK_COUNT,count);
		plr->GetSession()->SendItemPushResult(add,false,true,false,false,plr->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,count);
	}

	return 0;
}
int luaGameObject_GetClosestPlayer(lua_State * L, GameObject * ptr)
{
	if(!ptr)
		return 0;

	float dist, d2;
	Player * ret=NULL;

	for(set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); itr)
	{
		d2=(*itr)->GetDistanceSq(ptr);
		if(!ret||d2<dist)
		{
			dist=d2;
			ret=*itr;
		}
	}

	if(ret==NULL)
		lua_pushnil(L);
	else
		Lunar<Unit>::push(L,((Unit*)ret),false);

	return 1;
}
int luaGameObject_GetDistance(lua_State * L, GameObject * ptr)
{
	if(!ptr)
		return 0;
	Unit * target = Lunar<Unit>::check(L, 1);
	lua_pushnumber(L,ptr->GetDistance2dSq(target));
	return 1;
}
int luaGameObject_IsInWorld(lua_State * L, GameObject * ptr)
{
	if (ptr)
		if(ptr->IsInWorld())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaGameObject_GetZoneId(lua_State *L, GameObject * ptr)
{
	if(!ptr)
		return 0;
	lua_pushinteger(L,(ptr->GetZoneId()));
	return 1;
}
int luaGameObject_PlaySoundToSet(lua_State * L, GameObject * ptr)
{
	if(!ptr) return 0;
	int soundid = luaL_checkint(L,1);
	ptr->PlaySoundToSet(soundid);
	return 0;
}
int luaGameObject_SpawnCreature(lua_State * L, GameObject * ptr)
{
	if(ptr == NULL) return 0;
	uint32 entry_id = luaL_checkint(L, 1);
	float x = (float)luaL_checkint(L, 2);
	float y = (float)luaL_checkint(L, 3);
	float z = (float)luaL_checkint(L, 4);
	float o = (float)luaL_checkint(L, 5);
	uint32 faction = luaL_checkint(L, 6);
	uint32 duration = luaL_checkint(L, 7);

	if( !x || !y || !z || !entry_id || !faction /*|| !duration*/) //Shady: is it really required?
	{
		lua_pushnil(L);
		return 1;
	}
	CreatureProto *p = CreatureProtoStorage.LookupEntry(entry_id);
    
    if(p == NULL)
      return NULL;

    Creature *pCreature = ptr->GetMapMgr()->CreateCreature(entry_id);
    pCreature->m_spawn = 0;
    pCreature->Load(p,x,y,z,0);
	if(faction)
		pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,faction);
	pCreature->_setFaction();
    pCreature->SetMapId(ptr->GetMapId());
    pCreature->SetOrientation(o);
    pCreature->Despawn(duration, 0);
    pCreature->SetInstanceID(ptr->GetInstanceID());
    pCreature->PushToWorld(ptr->GetMapMgr());
	if(duration)
	{
		pCreature->Despawn(duration,0);
	}
	Lunar<Unit>::push(L,pCreature);
	return 0;
}
int luaGameObject_GetItemCount(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID_RET_INT(TYPEID_PLAYER);
	int itemid = luaL_checkint(L,1);
	lua_pushinteger(L, ((Player*)ptr)->GetItemInterface()->GetItemCount(itemid,false));
	return 1;
}
int luaGameObject_SpawnGameObject(lua_State * L, GameObject * ptr)
{
	if(ptr == NULL) return 0;
	uint32 entry_id = luaL_checkint(L, 1);
	float x = (float)luaL_checkint(L, 2);
	float y = (float)luaL_checkint(L, 3);
	float z = (float)luaL_checkint(L, 4);
	float o = (float)luaL_checkint(L, 5);
	uint32 duration = luaL_checkint(L, 6);

	if(!entry_id || !duration)
	{
		lua_pushnil(L);
		return 1;
	}

	GameObjectInfo *goi = GameObjectNameStorage.LookupEntry(entry_id);
	if (goi == NULL) return 0;
    GameObject *pC = ptr->GetMapMgr()->CreateGameObject(entry_id);
    pC->m_spawn=0;
    pC->CreateFromProto(entry_id, ptr->GetMapId(), (float)x, (float)y, (float)z, (float)o);
    pC->SetMapId(ptr->GetMapId());
    pC->SetInstanceID(ptr->GetInstanceID());
	pC->Spawn(ptr->GetMapMgr());
	if(duration)
	{
		sEventMgr.AddEvent(pC, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_UPDATE, duration, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	Lunar<GameObject>::push(L,pC);
	return 0;
}
int luaGameObject_CalcDistance(lua_State * L, GameObject * ptr)
{
	Object * ob = Lunar<Object>::check(L,1);
	if(ob)
		lua_pushnumber(L,ptr->CalcDistance(ob));
	return 1;
}
int luaGameObject_GetSpawnX(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnX());
	return 1;
}
int luaGameObject_GetSpawnY(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnY());
	return 1;
}
int luaGameObject_GetSpawnZ(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnZ());
	return 1;
}
int luaGameObject_GetSpawnO(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnO());
	return 1;
}
int luaGameObject_GetX(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetPositionX());
	return 0;
}
int luaGameObject_GetY(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetPositionY());
	return 0;
}
int luaGameObject_GetZ(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetPositionZ());
	return 0;
}
int luaGameObject_GetO(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetOrientation());
	return 0;
}

int luaGameObject_GetInRangePlayersCount(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetInRangePlayersCount());
	return 1;
}
int luaGameObject_GetEntry(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetEntry());
	return 1;
}
int luaGameObject_SetOrientation(lua_State * L, GameObject * ptr)
{
	float newo = (float)luaL_checkint(L, 1);
	if(!newo|!ptr)
		return 0;
	ptr->SetOrientation(newo);
	return 0;
}
int luaGameObject_CalcRadAngle(lua_State * L, GameObject * ptr)
{
	float x = (float)luaL_checkint(L,1 );
	float y = (float)luaL_checkint(L, 2);
	float x2 = (float)luaL_checkint(L, 3);
	float y2 = (float)luaL_checkint(L, 4);
	if(!x||!y||!x2||!y2||!ptr)
		return 0;
	lua_pushnumber(L,ptr->calcRadAngle(x,y,x2,y2));
	return 0;
}
int luaGameObject_GetInstanceID(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	if(ptr->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL)
		lua_pushnil(L);
	else
	lua_pushinteger(L,ptr->GetInstanceID());
	return 1;
}
int luaGameObject_GetInRangePlayers(lua_State * L, GameObject * ptr)
{
	uint32 count = 0;
	lua_newtable(L);
	for(std::set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); itr++)
	{
		if( (*itr) ->IsUnit())
		{
			count++,
			lua_pushinteger(L,count);
			Lunar<Unit>::push(L,((Unit*)*itr),false);
			lua_rawset(L,-3);
		}
	}
	return 1;
}

int luaGameObject_GetInRangeGameObjects(lua_State * L, GameObject * ptr)
{
	uint32 count = 0;
	lua_newtable(L);
	for (std::set<Object*>::iterator itr = ptr->GetInRangeSetBegin();itr!= ptr->GetInRangeSetEnd();itr++)
	{
		if( (*itr) ->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			count++,
			lua_pushinteger(L,count);
			Lunar<GameObject>::push(L,((GameObject*)*itr),false);
			lua_rawset(L,-3);
		}
	}
	return 1;
}
int luaGameObject_IsInFront(lua_State * L, GameObject * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	if(ptr && target)
		if(ptr->isInFront(target))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaGameObject_IsInBack(lua_State * L, GameObject * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	if(ptr && target)
		if(ptr->isInBack(target))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaGameObject_GetUInt32Value(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	if (ptr)
		lua_pushinteger(L,ptr->GetUInt32Value(field));
	return 1;
}
int luaGameObject_GetUInt64Value(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	if (ptr && field)
	{
		lua_pushinteger(L,(int)ptr->GetUInt64Value(field));
	}
	return 1;
}
int luaGameObject_SetUInt32Value(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	int value = luaL_checkint(L,2);
	if(ptr && field && value)
	{
		ptr->SetUInt32Value(field,value);
	}
	return 1;
}
int luaGameObject_SetUInt64Value(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	int value = luaL_checkint(L,2);
	if(ptr && field && value)
	{
		ptr->SetUInt64Value(field,value);
	}
	return 1;
}
int luaGameObject_SetFloatValue(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	float value = (float)luaL_checkint(L,2);
	if( ptr && value)
	{
		ptr->SetFloatValue(field,value);
	}
	return 1;
}
int luaGameObject_GetFloatValue(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	if( ptr && field)
	{
		lua_pushnumber(L,ptr->GetFloatValue(field));
	}
	return 1;
}
int luaGameObject_ModUInt32Value(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	int value = luaL_checkint(L,2);
	if( ptr && field && value)
	{
		ptr->ModSignedInt32Value(field,value);
	}
	return 1;
}
int luaGameObject_CastSpell(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
	uint32 sp = luaL_checkint(L,1);
	if( !ptr || !sp|| sp == 0) return 0;
	
	Spell * spp = new Spell( ptr, dbcSpell.LookupEntry(sp), true, NULL);
	SpellCastTargets tar(ptr->GetGUID());
	spp->prepare(&tar);
	return 0;
}
int luaGameObject_FullCastSpell(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
	uint32 sp = luaL_checkint(L,1);
	if( !ptr || !sp|| sp == 0) return 0;
	
	Spell * nspell = new Spell(ptr,dbcSpell.LookupEntry(sp),false,NULL);
	SpellCastTargets tar(ptr->GetGUID());
	nspell->prepare(&tar);
	return 0;
}
int luaGameObject_CastSpellOnTarget(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
	uint32 sp = luaL_checkint(L,1);
	Unit * target = Lunar<Unit>::check(L,2);
	if( !ptr || !sp || sp == 0) return 0;

	Spell * nspell = new Spell(ptr,dbcSpell.LookupEntry(sp),true,NULL);
	SpellCastTargets tar(target->GetGUID());
	nspell->prepare(&tar);
	return 0;
}
int luaGameObject_FullCastSpellOnTarget(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
	uint32 sp = luaL_checkint(L,1);
	Unit * target = Lunar<Unit>::check(L,2);
	if( !ptr || !sp || sp == 0) return 0;
	Spell * nspell = new Spell(ptr,dbcSpell.LookupEntry(sp),false,NULL);
	SpellCastTargets tar(target->GetGUID());
	nspell->prepare(&tar);
	return 0;
}
int luaGameObject_GetGUID(lua_State * L, GameObject* ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);

	lua_pushinteger(L,ptr->GetGUID());
	return 1;
}

