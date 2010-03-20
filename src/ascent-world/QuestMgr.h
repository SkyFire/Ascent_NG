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

#ifndef __QUESTMGR_H
#define __QUESTMGR_H

enum RepeatableTypes
{
	REPEATABLE_QUEST	= 1,
	REPEATABLE_DAILY	= 2,
};

struct QuestRelation
{
	Quest *qst;
	uint8 type;
};

struct QuestAssociation
{
	Quest *qst;
	uint8 item_count;
};

class Item;

typedef std::list<QuestRelation *> QuestRelationList;
typedef std::list<QuestAssociation *> QuestAssociationList;

class SERVER_DECL QuestMgr :  public Singleton < QuestMgr >
{
public:

	~QuestMgr();

	uint32 PlayerMeetsReqs(Player* plr, Quest* qst, bool skiplevelcheck);

	uint8 CalcStatus(Object* quest_giver, Player* plr);
	uint32 CalcQuestStatus(Object* quest_giver, Player* plr, QuestRelation* qst);
	uint32 CalcQuestStatus(Object* quest_giver, Player* plr, Quest* qst, uint8 type, bool skiplevelcheck);
	uint32 ActiveQuestsCount(Object* quest_giver, Player* plr);

	//Packet Forging...
	void BuildOfferReward(WorldPacket* data,Quest* qst, Object* qst_giver, uint32 menutype, uint32 language, Player* plr);
	void BuildQuestDetails(WorldPacket* data, Quest* qst, Object* qst_giver, uint32 menutype, uint32 language, Player* plr);	
	void BuildRequestItems(WorldPacket* data, Quest* qst, Object* qst_giver, uint32 status, uint32 language);
	void BuildQuestComplete(Player* , Quest* qst);
	void BuildQuestList(WorldPacket* data, Object* qst_giver, Player* plr, uint32 language);
	bool OnActivateQuestGiver(Object* qst_giver, Player* plr);
    bool isRepeatableQuestFinished(Player* plr, Quest *qst);

	void SendQuestUpdateAddKill(Player* plr, uint32 questid, uint32 entry, uint32 count, uint32 tcount, uint64 guid);
	void BuildQuestUpdateAddItem(WorldPacket* data, uint32 itemid, uint32 count);
	void BuildQuestUpdateComplete(WorldPacket* data, Quest* qst);
	void BuildQuestFailed(WorldPacket* data, uint32 questid);
	void SendPushToPartyResponse(Player* plr, Player* pTarget, uint32 response);

	bool OnGameObjectActivate(Player* plr, GameObject* go);
	void _OnPlayerKill(Player* plr, uint32 creature_entry);
	void OnPlayerKill(Player* plr, Creature* victim);
	void OnPlayerCast(Player* plr, uint32 spellid, uint64& victimguid);
	void OnPlayerItemPickup(Player* plr, Item* item);
	void OnPlayerExploreArea(Player* plr, uint32 AreaID);
	void OnPlayerSlain(Player* plr, Player* victim);
	void OnQuestAccepted(Player* plr, Quest* qst, Object* qst_giver);
	void OnQuestFinished(Player* plr, Quest* qst, Object* qst_giver, uint32 reward_slot);
	bool SkippedKills( uint32 QuestID );

	void GiveQuestRewardReputation(Player* plr, Quest* qst, Object* qst_giver);
	void GiveQuestTitleReward(Player* plr, Quest* qst);

	uint32 GenerateQuestXP(Player* pl, Quest *qst);
	uint32 GenerateRewardMoney(Player* pl, Quest *qst);

	void SendQuestInvalid( INVALID_REASON reason, Player* plyr);
	void SendQuestFailed(FAILED_REASON failed, Quest *qst, Player* plyr);
	void SendQuestUpdateFailed(Quest *pQuest, Player* plyr);
	void SendQuestUpdateFailedTimer(Quest *pQuest, Player* plyr);
	void SendQuestLogFull(Player* plyr);
	
	void LoadNPCQuests(Creature* qst_giver);
	void LoadGOQuests(GameObject* go);

	QuestRelationList* GetCreatureQuestList(uint32 entryid);
	QuestRelationList* GetGOQuestList(uint32 entryid);
	QuestAssociationList* GetQuestAssociationListForItemId (uint32 itemId);

	uint32 GetGameObjectLootQuest(uint32 GO_Entry);
	void SetGameObjectLootQuest(uint32 GO_Entry, uint32 Item_Entry);
	ASCENT_INLINE bool IsQuestRepeatable(Quest *qst) { return (qst->is_repeatable!=0 ? true : false); }
	ASCENT_INLINE bool IsQuestDaily(Quest *qst) { return (qst->is_repeatable==2 ? true : false); }

	bool CanStoreReward(Player* plyr, Quest *qst, uint32 reward_slot);

	ASCENT_INLINE int32 QuestHasMob(Quest* qst, uint32 mob)
	{
		for(uint32 i = 0; i < 4; ++i)
			if(qst->required_mob[i] == mob)
				return qst->required_mobcount[i];
		return -1;
	}

	ASCENT_INLINE int32 GetOffsetForMob(Quest *qst, uint32 mob)
	{
		for(uint32 i = 0; i < 4; ++i)
			if(qst->required_mob[i] == mob)
				return i;

		return -1;
	}

	ASCENT_INLINE int32 GetOffsetForItem(Quest *qst, uint32 itm)
	{
		for(uint32 i = 0; i < 6; ++i)
			if(qst->required_item[i] == itm)
				return i;

		return -1;
	}
	void LoadExtraQuestStuff();

private:

	HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* > m_npc_quests;
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* > m_obj_quests;
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* > m_itm_quests;
	list<uint32> m_extraqueststuff_list;

	HM_NAMESPACE::hash_map<uint32, list<QuestAssociation *>* > m_quest_associations;
	HM_NAMESPACE::hash_map<uint32, list<QuestAssociation *>* >& GetQuestAssociationList(){return m_quest_associations;}

	HM_NAMESPACE::hash_map<uint32, uint32>		  m_ObjectLootQuestList;

	template <class T> void _AddQuest(uint32 entryid, Quest *qst, uint8 type);

	template <class T> HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* >& _GetList();

	void AddItemQuestAssociation( uint32 itemId, Quest *qst, uint8 item_count);

	// Quest Loading
	void _RemoveChar(char* c, std::string *str);
	void _CleanLine(std::string *str);
};

template<> ASCENT_INLINE HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* >& QuestMgr::_GetList<Creature>()
	{return m_npc_quests;}
template<> ASCENT_INLINE HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* >& QuestMgr::_GetList<GameObject>()
	{return m_obj_quests;}
template<> ASCENT_INLINE HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* >& QuestMgr::_GetList<Item>()
	{return m_itm_quests;}


#define sQuestMgr QuestMgr::getSingleton()

#endif
