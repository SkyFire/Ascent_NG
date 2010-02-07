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

#include "Instance_Base.h"

AscentInstanceScript::AscentInstanceScript( MapMgr* pMapMgr ) : InstanceScript( pMapMgr )
{
	mUpdateFrequency = DEFAULT_UPDATE_FREQUENCY;
};

AscentInstanceScript::~AscentInstanceScript()
{
};

Creature* AscentInstanceScript::GetCreatureBySqlId( uint32 pSqlId )
{
	if ( pSqlId == 0 )
		return NULL;

	return mInstance->GetSqlIdCreature( pSqlId );
};

Creature* AscentInstanceScript::GetCreatureByGuid( uint32 pGuid )
{
	if ( pGuid == 0 )
		return NULL;

	return mInstance->GetCreature( pGuid );
};

Creature*	AscentInstanceScript::FindClosestCreatureOnMap( uint32 pEntry, float pX, float pY, float pZ )
{
	CreatureSet Creatures = FindCreaturesOnMap( pEntry );

	if ( Creatures.size() == 0 )
		return NULL;
	if ( Creatures.size() == 1 )
		return *( Creatures.begin() );

	Creature* NearestCreature = NULL;
	float Distance, NearestDistance = 99999;
	for ( CreatureSet::iterator Iter = Creatures.begin(); Iter != Creatures.end(); ++Iter )
	{
		Distance = GetRangeToObject( *Iter, pX, pY, pZ );
		if ( Distance < NearestDistance )
		{
			NearestDistance = Distance;
			NearestCreature = ( *Iter );
		};
	};

	return NearestCreature;
};

Creature* AscentInstanceScript::SpawnCreature( uint32 pEntry, float pX, float pY, float pZ, float pO )
{
	Creature* NewCreature = mInstance->GetInterface()->SpawnCreature( pEntry, pX, pY, pZ, pO, true, true, NULL, NULL);
	return NewCreature;
};

Creature* AscentInstanceScript::SpawnCreature( uint32 pEntry, float pX, float pY, float pZ, float pO, uint32 pFactionId )
{
	Creature* NewCreature = mInstance->GetInterface()->SpawnCreature( pEntry, pX, pY, pZ, pO, true, true, NULL, NULL);
	if ( NewCreature != NULL )
		NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pFactionId);

	return NewCreature;
};

CreatureSet AscentInstanceScript::FindCreaturesOnMap( uint32 pEntry )
{
	Creature* CurrentCreature = NULL;
	CreatureSet ReturnSet;
	for ( uint32 i = 0; i != mInstance->m_CreatureHighGuid; ++i )
	{
		CurrentCreature = mInstance->m_CreatureStorage[ i ];
		if ( CurrentCreature != NULL )
		{ 
			if ( CurrentCreature->GetEntry() == pEntry )
				ReturnSet.insert( CurrentCreature );
		};
	};

	return ReturnSet;
};

GameObject* AscentInstanceScript::FindClosestGameObjectOnMap( uint32 pEntry, float pX, float pY, float pZ )
{
	GameObjectSet GameObjects = FindGameObjectsOnMap( pEntry );

	if ( GameObjects.size() == 0 )
		return NULL;
	if ( GameObjects.size() == 1 )
		return *( GameObjects.begin() );

	GameObject* NearestObject = NULL;
	float Distance, NearestDistance = 99999;
	for ( GameObjectSet::iterator Iter = GameObjects.begin(); Iter != GameObjects.end(); ++Iter )
	{
		Distance = GetRangeToObject( *Iter, pX, pY, pZ );
		if ( Distance < NearestDistance )
		{
			NearestDistance = Distance;
			NearestObject = ( *Iter );
		};
	};

	return NearestObject;
};

GameObject* AscentInstanceScript::SpawnGameObject( uint32 pEntry, float pX, float pY, float pZ, float pO )
{
	GameObject* pNewGO = mInstance->GetInterface()->SpawnGameObject( pEntry, pX, pY, pZ, pO, true, NULL, NULL );
	return pNewGO;
};

GameObjectSet AscentInstanceScript::FindGameObjectsOnMap( uint32 pEntry )
{
	GameObject* CurrentObject = NULL;
	GameObjectSet ReturnSet;
	for ( GameObjectMap::iterator Iter = mInstance->m_gameObjectStorage.begin(); Iter != mInstance->m_gameObjectStorage.end(); ++Iter )
	{
		CurrentObject = ( *Iter ).second;
		if ( CurrentObject != NULL )
		{ 
			if ( CurrentObject->GetEntry() == pEntry )
				ReturnSet.insert( CurrentObject );
		};
	};

	return ReturnSet;
};

GameObject* AscentInstanceScript::GetGameObjectBySqlId( uint32 pSqlId )
{
	if ( pSqlId == 0 )
		return NULL;

	return mInstance->GetSqlIdGameObject( pSqlId );
};

GameObject* AscentInstanceScript::GetGameObjectByGuid( uint32 pGuid )
{
	if ( pGuid == 0 )
		return NULL;

	return mInstance->GetGameObject( pGuid );
};

void AscentInstanceScript::AddGameObjectStateByEntry( uint32 pEntry, GameObjectState pState, bool pUseQuery )
{
	if ( pEntry == 0 )
		return;

	GameObjectEntryMap::iterator Iter = mGameObjects.find( pEntry );
	if ( Iter != mGameObjects.end() )
		( *Iter ).second = pState;
	else
		mGameObjects.insert( GameObjectEntryMap::value_type( pEntry, pState ) );

	GameObject* CurrentObject = NULL;
	if ( !pUseQuery )
	{
		for ( GameObjectMap::iterator Iter = mInstance->m_gameObjectStorage.begin(); Iter != mInstance->m_gameObjectStorage.end(); ++Iter )
		{
			CurrentObject = ( *Iter ).second;
			if ( CurrentObject == NULL || CurrentObject->GetEntry() != pEntry )
				continue;

			CurrentObject->SetState( pState );
		};
	}
	else
	{
		QueryResult* Result = WorldDatabase.Query( "SELECT id FROM gameobject_spawns WHERE entry = %u", pEntry );
		if ( Result != NULL )
		{
			do
			{
				CurrentObject = GetGameObjectBySqlId( Result->Fetch()[ 0 ].GetUInt32() );
				if ( CurrentObject != NULL )
					CurrentObject->SetState( pState );
			}
			while ( Result->NextRow() );

			delete Result;
		};
	};
};

void AscentInstanceScript::AddGameObjectStateById( uint32 pId, GameObjectState pState )
{
	if ( pId == 0 )
		return;

	GameObject* StateObject = GetGameObjectBySqlId( pId );
	GameObjectEntryMap::iterator Iter;
	if ( StateObject != NULL )
	{
		StateObject->SetState( pState );
		Iter = mGameObjects.find( StateObject->GetEntry() );
		if ( Iter != mGameObjects.end() )
			( *Iter ).second = pState;
		else
			mGameObjects.insert( GameObjectEntryMap::value_type( StateObject->GetEntry(), pState ) );
	}
	else
	{
		QueryResult* Result = WorldDatabase.Query( "SELECT entry FROM gameobject_spawns WHERE id = %u", pId );
		if ( Result != NULL )
		{
			uint32 Entry = Result->Fetch()[ 0 ].GetUInt32();
			Iter = mGameObjects.find( Entry );
			if ( Iter != mGameObjects.end() )
				( *Iter ).second = pState;
			else
				mGameObjects.insert( GameObjectEntryMap::value_type( Entry, pState ) );

			delete Result;
		};
	};
};

float AscentInstanceScript::GetRangeToObject( Object* pObjectA, Object* pObjectB )
{
	if ( pObjectA == NULL || pObjectB == NULL )
		return 0.0f;

	return GetRangeToObject( pObjectA->GetPositionX(), pObjectA->GetPositionY(), pObjectA->GetPositionZ(), pObjectB->GetPositionX(), pObjectB->GetPositionY(), pObjectB->GetPositionZ() );
};

float AscentInstanceScript::GetRangeToObject( Object* pObject, float pX, float pY, float pZ )
{
	if ( pObject == NULL )
		return 0.0f;

	return GetRangeToObject( pObject->GetPositionX(), pObject->GetPositionY(), pObject->GetPositionZ(), pX, pY, pZ );
};

float AscentInstanceScript::GetRangeToObject( float pX1, float pY1, float pZ1, float pX2, float pY2, float pZ2 )
{
	float dX = pX1 - pX2;
	float dY = pY1 - pY2;
	float dZ = pZ1 - pZ2;

	return sqrtf( dX * dX + dY * dY + dZ * dZ );
};

bool AscentInstanceScript::HasPlayers()
{
	return mInstance->GetPlayerCount() > 0;
};

size_t AscentInstanceScript::GetPlayerCount()
{
	return mInstance->GetPlayerCount();
};

Player* AscentInstanceScript::GetPlayerByGuid( uint32 pGuid )
{
	if ( pGuid == 0 )
		return NULL;

	return mInstance->GetPlayer( pGuid );
};

bool AscentInstanceScript::IsCombatInProgress()
{
	return mInstance->_combatProgress.size() > 0;
};

uint32 AscentInstanceScript::AddTimer( int32 pDurationMillisec )
{
	int32 Index = mTimerIdCounter++;
	mTimers.push_back( std::make_pair( Index, pDurationMillisec ) );
	return Index;
}

int32 AscentInstanceScript::GetTimer( int32 pTimerId )
{
	for ( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if ( TimerIter->first == pTimerId )
			return TimerIter->second;
	};

	return 0;
};

void AscentInstanceScript::RemoveTimer( int32& pTimerId )
{
	for ( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if ( TimerIter->first == pTimerId )
		{
			mTimers.erase( TimerIter );
			pTimerId = INVALIDATE_TIMER;
			break;
		};
	};
};

void AscentInstanceScript::ResetTimer( int32 pTimerId, int32 pDurationMillisec )
{
	for ( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if ( TimerIter->first == pTimerId )
		{
			TimerIter->second = pDurationMillisec;
			break;
		};
	};
};

bool AscentInstanceScript::IsTimerFinished( int32 pTimerId )
{
	for ( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if ( TimerIter->first == pTimerId )
			return ( TimerIter->second <= 0 ) ? true : false;
	};

	return false;
};

void AscentInstanceScript::CancelAllTimers()
{
	mTimers.clear();
	mTimerIdCounter = 0;
};

void AscentInstanceScript::RegisterScriptUpdateEvent()
{
	RegisterUpdateEvent( mUpdateFrequency );
};

void AscentInstanceScript::SetUpdateEventFreq( uint32 pUpdateFreq )
{
	if ( mUpdateFrequency != pUpdateFreq )
	{
		mUpdateFrequency = pUpdateFreq;
		ModifyUpdateEvent( mUpdateFrequency );
	};
};

uint32 AscentInstanceScript::GetUpdateEventFreq()
{
	return mUpdateFrequency;
};

void AscentInstanceScript::SetCellForcedStates( float pMinX, float pMaxX, float pMinY, float pMaxY, bool pActivate )
{
	if ( pMinX == pMaxX || pMinY == pMaxY )
		return;

	float Y = pMinY;
	while ( pMinX < pMaxX )
	{
		while ( pMinY < pMaxY )
		{
			MapCell* CurrentCell = mInstance->GetCellByCoords( pMinX, pMinY );
			if ( pActivate && CurrentCell == NULL )
			{
				CurrentCell = mInstance->CreateByCoords( pMinX, pMinY );
				if ( CurrentCell != NULL )
					CurrentCell->Init( mInstance->GetPosX( pMinX ), mInstance->GetPosY( pMinY ), mInstance->GetMapId(), mInstance );
			};

			if ( CurrentCell != NULL )
			{
				if ( pActivate )
					mInstance->AddForcedCell( CurrentCell );
				else
					mInstance->RemoveForcedCell( CurrentCell );
			};

			pMinY += 40.0f;
		};

		pMinY = Y;
		pMinX += 40.0f;
	};
};

void AscentInstanceScript::OnPlayerDeath( Player* pVictim, Unit* pKiller )
{
};

void AscentInstanceScript::OnPlayerEnter( Player* pPlayer )
{
};

void AscentInstanceScript::OnAreaTrigger( Player* pPlayer, uint32 pAreaId )
{
};

void AscentInstanceScript::OnZoneChange( Player* pPlayer, uint32 pNewZone, uint32 pOldZone )
{
};

void AscentInstanceScript::SetInstanceData( uint32 pType, uint32 pIndex, uint32 pData )
{
};

uint32 AscentInstanceScript::GetInstanceData( uint32 pType, uint32 pIndex )
{
	return 0;
};

void AscentInstanceScript::OnCreatureDeath( Creature* pVictim, Unit* pKiller )
{
};

void AscentInstanceScript::OnCreaturePushToWorld( Creature* pCreature )
{
};

void AscentInstanceScript::OnGameObjectActivate( GameObject* pGameObject, Player* pPlayer )
{
};

void AscentInstanceScript::OnGameObjectPushToWorld( GameObject* pGameObject )
{
	// Dunno how part of those would happen
	if ( mGameObjects.size() == 0 || pGameObject == NULL )
		return;

	GameObjectEntryMap::iterator Iter = mGameObjects.find( pGameObject->GetEntry() );
	if ( Iter != mGameObjects.end() )
		pGameObject->SetState( ( *Iter ).second );
};

GameObject* AscentInstanceScript::GetObjectForOpenLock( Player* pCaster, Spell* pSpell, SpellEntry* pSpellEntry )
{
	return NULL;
};

void AscentInstanceScript::SetLockOptions( uint32 pEntryId, GameObject* pGameObject )
{
};

uint32 AscentInstanceScript::GetRespawnTimeForCreature( uint32 pEntryId, Creature* pCreature)
{
	return 240000;
};

void AscentInstanceScript::OnLoad()
{
};

void AscentInstanceScript::UpdateEvent()
{
	uint32 CurrentTime = static_cast< uint32 >( time( NULL ) );
	for ( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		TimerIter->second -= mUpdateFrequency;
	};
};

void AscentInstanceScript::Destroy()
{
	delete this;
};

void AscentInstanceScript::BuildEncounterMap()
{
	if ( mInstance->pInstance == NULL )
		return;

	QueryResult* KillResult = WorldDatabase.Query( "SELECT id, entry FROM creature_spawns WHERE map = %u AND entry IN ( SELECT entry FROM creature_names WHERE rank = 3 )", mInstance->GetMapId() );
	if ( KillResult != NULL )
	{
		uint32 Id = 0, Entry = 0;
		Field* CurrentField = NULL;
		EncounterMap::iterator EncounterIter;
		EncounterState State = State_NotStarted;
		bool StartedInstance = mInstance->pInstance->m_killedNpcs.size() > 0;
		do
		{
			CurrentField = KillResult->Fetch();
			Id = CurrentField[ 0 ].GetUInt32();
			Entry = CurrentField[ 1 ].GetUInt32();

			EncounterIter = mEncounters.find( Entry );
			if ( EncounterIter != mEncounters.end() )
				continue;

			if ( StartedInstance )
			{
				if ( mInstance->pInstance->m_killedNpcs.find( Id ) != mInstance->pInstance->m_killedNpcs.end() )
					State = State_Finished;
				else
					State = State_NotStarted;
			};

			mEncounters.insert( EncounterMap::value_type( Entry, BossData( Id, 0, State ) ) );
		}
		while ( KillResult->NextRow() );

	delete KillResult;
	};
};

// Dynamic data creation that still involves MySQL   //Have some problem wit this function
void AscentInstanceScript::BuildEncounterMapWithEntries( IdVector pEntries )
{
	if ( mInstance->pInstance == NULL || pEntries.size() == 0 )
		return;

	std::stringstream Query;
	Query << "SELECT id, entry FROM creature_spawns WHERE entry IN ( ";
	Query << pEntries[ 0 ];
	for ( size_t i = 1; i < pEntries.size(); ++i )
	{
		Query << ", ";
		Query << pEntries[ i ];
	};

	Query << " )";
	QueryResult* KillResult = WorldDatabase.Query( Query.str().c_str() );
	if ( KillResult != NULL )
	{
		uint32 Id = 0, Entry = 0;
		Field* CurrentField = NULL;
		EncounterMap::iterator EncounterIter;
		EncounterState State = State_NotStarted;
		bool StartedInstance = mInstance->pInstance->m_killedNpcs.size() > 0;
		do
		{
			CurrentField = KillResult->Fetch();
			Id = CurrentField[ 0 ].GetUInt32();
			Entry = CurrentField[ 1 ].GetUInt32();

			EncounterIter = mEncounters.find( Entry );
			if ( EncounterIter != mEncounters.end() )
				continue;

			if ( StartedInstance )
			{
				if ( mInstance->pInstance->m_killedNpcs.find( Id ) != mInstance->pInstance->m_killedNpcs.end() )
					State = State_Finished;
				else
					State = State_NotStarted;
			};

			mEncounters.insert( EncounterMap::value_type( Entry, BossData( Id, 0, State ) ) );
		}
		while ( KillResult->NextRow() );

		delete KillResult;
	};
};

// Static data creation without MySQL use
void AscentInstanceScript::BuildEncounterMapWithIds( IdVector pIds )
{
	// Won't work with spawns that are not in world - would work well with instance fully loaded
	if ( mInstance->pInstance == NULL || pIds.size() == 0 )
		return;

	uint32 CurrentId = 0;
	EncounterState State = State_NotStarted;
	Creature* Boss = NULL;
	unordered_set< uint32 >::iterator Iter;
	EncounterMap::iterator EncounterIter;
	for ( size_t i = 0; i < pIds.size(); ++i )
	{
		CurrentId = pIds[ i ];
		if ( CurrentId == 0 )
			continue;

		Iter = mInstance->pInstance->m_killedNpcs.find( CurrentId );
		if ( Iter != mInstance->pInstance->m_killedNpcs.end() )
			State = State_Finished;

		Boss = mInstance->GetCreature( CurrentId );
		if ( Boss != NULL )
		{
			EncounterIter = mEncounters.find( Boss->GetEntry() );
			if ( EncounterIter != mEncounters.end() )
				continue;

			mEncounters.insert( EncounterMap::value_type( Boss->GetEntry(), BossData( CurrentId, Boss->GetGUID(), State ) ) );
		};

		State = State_NotStarted;
	};
};

IdVector AscentInstanceScript::BuildIdVector( uint32 pCount, ... )
{
	IdVector NewVector;
	va_list List;
	va_start( List, pCount );
	for ( uint32 i = 0; i < pCount; ++i )
	{
		NewVector.push_back( va_arg( List, uint32 ) );
	};
		
	va_end( List );
	return NewVector;
};

IdSet AscentInstanceScript::BuildIdSet( uint32 pCount, ... )//Have some problem this function!
{
	IdSet NewSet;
	va_list List;
	va_start( List, pCount );
	for ( uint32 i = 0; i < pCount; ++i )
	{
		NewSet.insert( va_arg( List, uint32 ) );
	};
		
	va_end( List );
	return NewSet;
};
