/*
 * Scripts for Ascent MMORPG Server
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

#ifndef _INSTANCE_SCRIPT_BASE_H_
#define _INSTANCE_SCRIPT_BASE_H_

#include "StdAfx.h"

#include <string>
#include <vector>
#include <map>

#define INVALIDATE_TIMER			-1
#define DEFAULT_UPDATE_FREQUENCY	1000	//milliseconds

#define ASCENTSCRIPT_INSTANCE_FACTORY_FUNCTION( ClassName, ParentClassName ) \
public:\
	ADD_INSTANCE_FACTORY_FUNCTION( ClassName );\
	typedef ParentClassName ParentClass;

enum EncounterState
{
	State_NotStarted			= 0,
	State_InProgress			= 1,
	State_Finished				= 2,
};

enum InstanceType
{
	Type_Default				= INSTANCE_NULL,
	Type_Raid					= INSTANCE_RAID,
	Type_NonRaid				= INSTANCE_NONRAID,
	Type_PvP					= INSTANCE_PVP,
	Type_MultiMode				= INSTANCE_MULTIMODE
};

enum InstanceMode
{
	Mode_Normal					= MODE_NORMAL,
	Mode_Heroic					= MODE_HEROIC,
	Mode_Epic					= MODE_EPIC
};

enum DataType
{
	Data_EncounterState			= 0,
	Data_EncounterProgress		= 1,
	Data_UnspecifiedType		= 2
};

enum GameObjectState
{
	State_Active				= 0,	// Door: open
	State_Inactive				= 1
};

struct BossData
{
	BossData( EncounterState pState )
	{
		mSqlId = mGuid = 0;
		mState = pState;
	};

	BossData( uint32 pId = 0, uint64 pGuid = 0, EncounterState pState = State_NotStarted )
	{
		mSqlId = pId;
		mGuid = pGuid;
		mState = pState;
	};

	~BossData()
	{
	};

	uint32			mSqlId;
	uint64			mGuid;
	EncounterState	mState;
};

class AscentInstanceScript;

typedef std::map< uint32, BossData >							EncounterMap;
typedef std::map< uint32, GameObjectState >						GameObjectEntryMap;
typedef std::vector< uint32 >									IdVector;
typedef std::set< uint32 >										IdSet;
typedef std::map< uint32, IdVector >							EntryIdMap;
typedef std::pair< int32, int32 >								TimerPair;
typedef std::vector< TimerPair >								TimerArray;
typedef std::tr1::hash_map< uint32, GameObject* >			GameObjectMap;
typedef std::tr1::unordered_set< Unit* >					UnitSet;
typedef std::tr1::unordered_set< Player* >				PlayerSet;
typedef std::tr1::unordered_set< Creature* >				CreatureSet;
typedef std::tr1::unordered_set< GameObject* >			GameObjectSet;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class AscentInstanceScript
class AscentInstanceScript : public InstanceScript
{
public:
	AscentInstanceScript( MapMgr* pMapMgr );
	virtual ~AscentInstanceScript();

	// Creature
	Creature*				GetCreatureBySqlId( uint32 pSqlId );
	Creature*				GetCreatureByGuid( uint32 pGuid );
	Creature*				FindClosestCreatureOnMap( uint32 pEntry, float pX, float pY, float pZ );
	Creature*				SpawnCreature( uint32 pEntry, float pX, float pY, float pZ, float pO );
	Creature*				SpawnCreature( uint32 pEntry, float pX, float pY, float pZ, float pO, uint32 pFactionId );
	CreatureSet					FindCreaturesOnMap( uint32 pEntry );

	// GameObject
	GameObject*			GetGameObjectBySqlId( uint32 pSqlId );
	GameObject*			GetGameObjectByGuid( uint32 pGuid );
	GameObject*			FindClosestGameObjectOnMap( uint32 pEntry, float pX, float pY, float pZ );
	GameObject*			SpawnGameObject( uint32 pEntry, float pX, float pY, float pZ, float pO );
	GameObjectSet				FindGameObjectsOnMap( uint32 pEntry );
	void						AddGameObjectStateByEntry( uint32 pEntry, GameObjectState pState, bool pUseQuery = false );
	void						AddGameObjectStateById( uint32 pId, GameObjectState pState );

	// Distance calculation
	float						GetRangeToObject( Object* pObjectA, Object* pObjectB );
	float						GetRangeToObject( Object* pObject, float pX, float pY, float pZ );
	float						GetRangeToObject( float pX1, float pY1, float pZ1, float pX2, float pY2, float pZ2 );

	// Player and instance - reimplementation for easier calling
	bool						HasPlayers();
	size_t						GetPlayerCount();
	Player*				GetPlayerByGuid( uint32 pGuid );
	bool						IsCombatInProgress();

	// Timers - reimplementation from AscentScriptCreatureAI
	uint32						AddTimer( int32 pDurationMillisec );
	int32						GetTimer( int32 pTimerId );
	void						RemoveTimer( int32& pTimerId );
	void						ResetTimer( int32 pTimerId, int32 pDurationMillisec );
	bool						IsTimerFinished( int32 pTimerId );
	void						CancelAllTimers();

	// Update Event
	void						RegisterScriptUpdateEvent();
	void						SetUpdateEventFreq( uint32 pUpdateFreq );
	uint32						GetUpdateEventFreq();

	// Cells
	void						SetCellForcedStates( float pMinX, float pMaxX, float pMinY, float pMaxY, bool pActivate = true );

	// Player
	virtual void				OnPlayerDeath( Player* pVictim, Unit* pKiller );

	// Area and AreaTrigger
	virtual void				OnPlayerEnter( Player* pPlayer );
	virtual void				OnAreaTrigger( Player* pPlayer, uint32 pAreaId );
	virtual void				OnZoneChange( Player* pPlayer, uint32 pNewZone, uint32 pOldZone );

	// Data get / set - idea taken from ScriptDev2
	virtual void				SetInstanceData( uint32 pType, uint32 pIndex, uint32 pData );
	virtual uint32				GetInstanceData( uint32 pType, uint32 pIndex );

	// Creature / GameObject
	virtual void				OnCreatureDeath( Creature* pVictim, Unit* pKiller );
	virtual void				OnCreaturePushToWorld( Creature* pCreature );
	virtual void				OnGameObjectActivate( GameObject* pGameObject, Player* pPlayer );
	virtual void				OnGameObjectPushToWorld( GameObject* pGameObject );

	// Reimplemented events
	virtual GameObject*	GetObjectForOpenLock( Player* pCaster, Spell* pSpell, SpellEntry* pSpellEntry );
	virtual void				SetLockOptions( uint32 pEntryId, GameObject* pGameObject );
	virtual uint32				GetRespawnTimeForCreature( uint32 pEntryId, Creature* pCreature);
	virtual void				OnLoad();
	virtual void				UpdateEvent();
	virtual void				Destroy();

protected:

	// Encounter generators //Here Some Problem!
	void						BuildEncounterMap();
	void						BuildEncounterMapWithEntries( IdVector pEntries );
	void						BuildEncounterMapWithIds( IdVector pIds );
	IdVector					BuildIdVector( uint32 pCount, ... );
	IdSet						BuildIdSet( uint32 pCount, ... ); //HERE!!!

	EncounterMap				mEncounters;
	GameObjectEntryMap			mGameObjects;
	uint32						mUpdateFrequency;
	TimerArray					mTimers;
	int32						mTimerIdCounter;
};

#endif /* _INSTANCE_SCRIPT_BASE_H_ */
