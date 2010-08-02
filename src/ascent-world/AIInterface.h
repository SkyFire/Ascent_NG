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

#ifndef WOWSERVER_AIINTERFACE_H
#define WOWSERVER_AIINTERFACE_H

/* platforms that already define M_PI in math.h */
#ifdef M_PI
#undef M_PI
#endif

#define M_PI	   3.14159265358979323846
#define UNIT_MOVEMENT_INTERPOLATE_INTERVAL 400/*750*/ // ms smoother server/client side moving vs less cpu/ less b/w
#define TARGET_UPDATE_INTERVAL 600 // ms
#define oocr 50.0f // out of combat range
#define PLAYER_SIZE 1.5f

#define ENABLE_CREATURE_DAZE
#ifdef ENABLE_CREATURE_DAZE
	#define CREATURE_SPELL_TO_DAZE 1604
	#define CREATURE_DAZE_TRIGGER_ANGLE M_PI/2 //for the beginners this means 45 degrees 
#endif

#define DISTANCE_TO_SMALL_TO_WALK 1.0f

class Object;
class Creature;
class Unit;
class Player;
class WorldSession;
class SpellCastTargets;


enum AIType
{
	AITYPE_LONER,
	AITYPE_AGRO,
	AITYPE_SOCIAL,
	AITYPE_PET,
	AITYPE_TOTEM,
	AITYPE_GUARDIAN, //we got a master but he cannot control us, we follow and battle oposite factions
};

enum MovementType
{
	MOVEMENTTYPE_NONE,
	MOVEMENTTYPE_RANDOMWP,
	MOVEMENTTYPE_CIRCLEWP,
	MOVEMENTTYPE_WANTEDWP,
	MOVEMENTTYPE_DONTMOVEWP,
	MOVEMENTTYPE_QUEST = 10,
	MOVEMENTTYPE_FORWARDTHANSTOP = 11,
};


/*struct AI_Target
{
	Unit* target;
	int32 threat;
};*/


enum AI_Agent
{
	AGENT_NULL,
	AGENT_MELEE,
	AGENT_RANGED,
	AGENT_FLEE,
	AGENT_SPELL,
	AGENT_CALLFORHELP
};

enum AI_SpellType
{
	STYPE_NULL,
	STYPE_ROOT,
	STYPE_HEAL,
	STYPE_STUN,
	STYPE_FEAR,
	STYPE_SILENCE,
	STYPE_CURSE,
	STYPE_AOEDAMAGE,
	STYPE_DAMAGE,
	STYPE_SUMMON,
	STYPE_BUFF,
	STYPE_DEBUFF
};

enum AI_SpellTargetType
{
	TTYPE_NULL,
	TTYPE_SINGLETARGET,
	TTYPE_DESTINATION,
	TTYPE_SOURCE,
	TTYPE_CASTER,
	TTYPE_OWNER,
};

enum AI_State
{
	STATE_IDLE,
	STATE_ATTACKING,
	STATE_CASTING,
	STATE_FLEEING,
	STATE_FOLLOWING,
	STATE_EVADE,
	STATE_MOVEWP,
	STATE_FEAR,
	STATE_WANDER,
	STATE_STOPPED,
	STATE_SCRIPTMOVE,
	STATE_SCRIPTIDLE
};

enum MovementState
{
	MOVEMENTSTATE_MOVE,
	MOVEMENTSTATE_FOLLOW,
	MOVEMENTSTATE_STOP,
	MOVEMENTSTATE_FOLLOW_OWNER
};

enum CreatureState
{
	STOPPED,
	MOVING,
	ATTACKING
};

enum AiEvents
{
	EVENT_ENTERCOMBAT,
	EVENT_LEAVECOMBAT,
	EVENT_DAMAGETAKEN,
	EVENT_FEAR,
	EVENT_UNFEAR,
	EVENT_FOLLOWOWNER,
	EVENT_WANDER,
	EVENT_UNWANDER,
	EVENT_UNITDIED,
};

struct SpellEntry;
//enum MOD_TYPES;

struct AI_Spell
{
	~AI_Spell() { autocast_type=(uint32)-1; }
	uint32 entryId;
	uint16 agent;
	uint32 procChance;
	//int32 procCount;
	//uint32 procCountDB;
	SpellEntry * spell;
	uint8 spellType;
	uint8 spelltargetType;
	uint32 cooldown;
	uint32 cooldowntime;
	uint32 procCount;
	uint32 procCounter;
	float floatMisc1;
	uint32 Misc2;
	float minrange;
	float maxrange;
	uint32 autocast_type;
	bool custom_pointer;
};

bool isGuard(uint32 id);
uint32 getGuardId(uint32 id);

#if ENABLE_SHITTY_STL_HACKS == 1
typedef HM_NAMESPACE::hash_map<Unit*, int32> TargetMap;
#else
namespace HM_NAMESPACE
{
	template <>
	struct hash<Unit*>
	{
		union __vp {
			size_t s;
			Unit* p;
		};

		size_t operator()(Unit* __x) const
		{
			__vp vp;
			vp.p = __x;
			return vp.s;
		}
	};
};

typedef HM_NAMESPACE::hash_map<Unit*, int32, HM_NAMESPACE::hash<Unit*> > TargetMap;
#endif

typedef std::set<Unit*> AssistTargetSet;
typedef std::map<uint32, AI_Spell*> SpellMap;

class SERVER_DECL AIInterface
{
public:

	AIInterface();
	~AIInterface();

	// Misc
	void Init(Unit *un, AIType at, MovementType mt);
	void Init(Unit *un, AIType at, MovementType mt, Unit *owner); // used for pets
	Unit *GetUnit() { return m_Unit; }
	Unit *GetPetOwner() { return m_PetOwner; }
	void DismissPet();
	void SetUnitToFollow(Unit* un) { UnitToFollow = un; };
	void SetUnitToFear(Unit* un)  { UnitToFear = un; };
	void SetFollowDistance(float dist) { FollowDistance = dist; };
	void SetUnitToFollowAngle(float angle) { m_fallowAngle = angle; }
	bool setInFront(Unit* target);
	ASCENT_INLINE Unit* getUnitToFollow() { return UnitToFollow; }
	void setCreatureState(CreatureState state){ m_creatureState = state; }
	ASCENT_INLINE uint8 getAIState() { return m_AIState; }
	ASCENT_INLINE uint8 getAIType() { return m_AIType; }
	ASCENT_INLINE uint8 getCurrentAgent() { return m_aiCurrentAgent; }
	void setCurrentAgent(AI_Agent agent) { m_aiCurrentAgent = agent; }
	uint32	getThreatByGUID(uint64 guid);
	uint32	getThreatByPtr(Unit* obj);
	Unit	*GetMostHated();
	Unit	*GetSecondHated();
	bool	modThreatByGUID(uint64 guid, int32 mod);
	bool	modThreatByPtr(Unit* obj, int32 mod);
	void	RemoveThreatByPtr(Unit* obj);
	ASCENT_INLINE AssistTargetSet GetAssistTargets() { return m_assistTargets; }
	ASCENT_INLINE TargetMap *GetAITargets() { return &m_aiTargets; }
	void addAssistTargets(Unit* Friends);
	void ClearHateList();
	void WipeHateList();
	void WipeTargetList();
	bool taunt(Unit* caster, bool apply = true);
	Unit* getTauntedBy();
	bool GetIsTaunted();
	Unit* getSoullinkedWith();
	void SetSoulLinkedWith(Unit* target);
	bool GetIsSoulLinked();
	ASCENT_INLINE size_t getAITargetsCount() { return m_aiTargets.size(); }
	ASCENT_INLINE uint32 getOutOfCombatRange() { return m_outOfCombatRange; }
	void setOutOfCombatRange(uint32 val) { m_outOfCombatRange = val; }

	// Spell
	void CastSpell(Unit* caster, SpellEntry *spellInfo, SpellCastTargets targets);
	SpellEntry *getSpellEntry(uint32 spellId);
	SpellCastTargets setSpellTargets(SpellEntry *spellInfo, Unit* target);
	AI_Spell *getSpell();
	void addSpellToList(AI_Spell *sp);
	//don't use this until i finish it !!
//	void CancelSpellCast();

	// Event Handler
	void HandleEvent(uint32 event, Unit* pUnit, uint32 misc1);
	void OnDeath(Object* pKiller);
	void AttackReaction(Unit *pUnit, uint32 damage_dealt, uint32 spellId = 0);
	bool HealReaction(Unit* caster, Unit* victim, uint32 amount);
	void Event_Summon_EE_totem(uint32 summon_duration);
	void Event_Summon_FE_totem(uint32 summon_duration);

	// Update
	void Update(uint32 p_time);

	// Movement
	void SendMoveToPacket(float toX, float toY, float toZ, float toO, uint32 time, uint32 MoveFlags);
	//void SendMoveToSplinesPacket(std::list<Waypoint> wp, bool run);
	void MoveTo(float x, float y, float z, float o);
	uint32 getMoveFlags();
	void UpdateMove();
	void SendCurrentMove(Player* plyr/*uint64 guid*/);
	bool StopMovement(uint32 time);
	uint32 getCurrentWaypoint() { return m_currentWaypoint; }
	void changeWayPointID(uint32 oldwpid, uint32 newwpid);
	bool addWayPoint(WayPoint* wp);
	bool saveWayPoints();
	bool showWayPoints(Player* pPlayer, bool Backwards);
	bool hideWayPoints(Player* pPlayer);
	WayPoint* getWayPoint(uint32 wpid);
	void deleteWayPoint(uint32 wpid);
	void deleteWaypoints();
	ASCENT_INLINE bool hasWaypoints() { return m_waypoints!=NULL; }
	ASCENT_INLINE void setMoveType(uint32 movetype) { m_moveType = movetype; }
	ASCENT_INLINE uint32 getMoveType() { return m_moveType; }
	ASCENT_INLINE void setMoveRunFlag(bool f) { m_moveRun = f; }
	ASCENT_INLINE bool getMoveRunFlag() { return m_moveRun; }
	void setWaypointToMove(uint32 id) { m_currentWaypoint = id; }
	bool IsFlying();

	// Calculation
	float _CalcAggroRange(Unit* target);
	void _CalcDestinationAndMove(Unit *target, float dist);
	float _CalcCombatRange(Unit* target, bool ranged);
	float _CalcDistanceFromHome();
	uint32 _CalcThreat(uint32 damage, SpellEntry * sp, Unit* Attacker);
	
	void SetAllowedToEnterCombat(bool val) { m_AllowedToEnterCombat = val; }
	ASCENT_INLINE bool GetAllowedToEnterCombat(void) { return m_AllowedToEnterCombat; }

	void CheckTarget(Unit* target);
	ASCENT_INLINE void SetAIState(AI_State newstate) { m_AIState = newstate; }

	// Movement
	bool m_canMove;
	bool m_WayPointsShowing;
	bool m_WayPointsShowBackwards;
	uint32 m_currentWaypoint;
	bool m_moveBackward;
	uint32 m_moveType;
	bool m_moveRun;
	bool m_moveFly;
	bool m_moveSprint;
	CreatureState m_creatureState;
	size_t GetWayPointsCount()
	{
		if(m_waypoints && !m_waypoints->empty())
			return m_waypoints->size()-1;	/* ignore 0 */
		else
			return 0;
	}
	bool m_canFlee;
	bool m_canCallForHelp;
	bool m_canRangedAttack;
	float m_FleeHealth;
	uint32 m_FleeDuration;
	float m_CallForHelpHealth;
	uint32 m_totemspelltimer;
	uint32 m_totemspelltime;
	SpellEntry * totemspell;

	float m_sourceX, m_sourceY, m_sourceZ;
	uint32 m_totalMoveTime;
	ASCENT_INLINE void AddStopTime(uint32 Time) { m_moveTimer += Time; }
	ASCENT_INLINE void SetNextSpell(AI_Spell * sp) { m_nextSpell = sp; }
	ASCENT_INLINE Unit* GetNextTarget() { return m_nextTarget; }
	ASCENT_INLINE void SetNextTarget (Unit *nextTarget) 
	{ 
		m_nextTarget = nextTarget; 
		if(nextTarget)
		{
			m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, m_nextTarget->GetGUID());
#ifdef ENABLE_GRACEFULL_HIT
			have_graceful_hit=false;
#endif
		}
		else m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
	}

	/*ASCENT_INLINE void ResetProcCounts()
	{
		AI_Spell * sp;
		for(list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
				{
					sp = *itr;
					sp->procCount =sp->procCountDB;
				}
	}*/

	Creature * m_formationLinkTarget;
	float m_formationFollowDistance;
	float m_formationFollowAngle;
	uint32 m_formationLinkSqlId;

	void WipeReferences();
	WayPointMap *m_waypoints;
	ASCENT_INLINE void SetPetOwner(Unit * owner) { m_PetOwner = owner; }
 
	list<AI_Spell*> m_spells;

	bool disable_combat;

	bool disable_melee;
	bool disable_ranged;
	bool disable_spell;
	bool disable_targeting;

	bool waiting_for_cooldown;

	uint32 next_spell_time;

	void CheckNextSpell(AI_Spell * sp)
	{
		if(m_nextSpell == sp)
			m_nextSpell = 0;
	}

	void ResetProcCounts();

	ASCENT_INLINE void SetWaypointMap(WayPointMap * m) { m_waypoints = m; }
	bool m_isGuard;
//	bool m_fastMove;
	void setGuardTimer(uint32 timer) { m_guardTimer = timer; }
	void _UpdateCombat(uint32 p_time);

protected:
	bool m_AllowedToEnterCombat;

	// Update
	void _UpdateTargets();
	void _UpdateMovement(uint32 p_time);
	void _UpdateTimer(uint32 p_time);
	int m_updateAssist;
	int m_updateTargets;
	uint32 m_updateAssistTimer;
	uint32 m_updateTargetsTimer;

	// Misc
	bool firstLeaveCombat;
	Unit* FindTarget();
	Unit* FindTargetForSpell(AI_Spell *sp);
	bool FindFriends(float dist);
	AI_Spell *m_nextSpell;
	Unit* m_nextTarget;
	uint32 m_fleeTimer;
	bool m_hasFleed;
	bool m_hasCalledForHelp;
	uint32 m_outOfCombatRange;

	Unit *m_Unit;
	Unit *m_PetOwner;
	float FollowDistance;
	float FollowDistance_backup;
	float m_fallowAngle;

	//std::set<AI_Target> m_aiTargets;
	TargetMap m_aiTargets;
	AssistTargetSet m_assistTargets;
	AIType m_AIType;
	AI_State m_AIState;
	AI_Agent m_aiCurrentAgent;

	Unit* tauntedBy; //This mob will hit only tauntedBy mob.
	bool isTaunted;
	Unit* soullinkedWith; //This mob can be hitten only by soullinked unit
	bool isSoulLinked;


	// Movement
	float m_walkSpeed;
	float m_runSpeed;
	float m_flySpeed;
	float m_destinationX;
	float m_destinationY;
	float m_destinationZ;
	
	float m_nextPosX;
	float m_nextPosY;
	float m_nextPosZ;

	//Return position after attacking a mob
	float m_returnX;
	float m_returnY;
	float m_returnZ;

	float m_lastFollowX;
	float m_lastFollowY;
	//typedef std::map<uint32, WayPoint*> WayPointMap;
	Unit *UnitToFollow;
	Unit *UnitToFollow_backup;//used unly when forcing creature to wander (blind spell) so when effect wears off we can follow our master again (guardian)
	Unit *UnitToFear;
	uint32 m_timeToMove;
	uint32 m_timeMoved;
	uint32 m_moveTimer;
	uint32 m_FearTimer;
	uint32 m_WanderTimer;

	MovementType m_MovementType;
	MovementState m_MovementState;
	uint32 m_guardTimer;
	int32 m_currentHighestThreat;
public:
	bool m_is_in_instance;
	bool skip_reset_hp;

	void WipeCurrentTarget();
	bool CheckCurrentTarget();
	bool TargetUpdateCheck(Unit * ptr);
};
#endif
