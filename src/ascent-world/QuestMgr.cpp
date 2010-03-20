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

#include "StdAfx.h"

uint32 QuestMgr::CalcQuestStatus(Object* quest_giver, Player* plr, QuestRelation* qst)
{
	return CalcQuestStatus(quest_giver, plr, qst->qst, qst->type, false);
}

bool QuestMgr::isRepeatableQuestFinished(Player* plr, Quest *qst)
{
    uint32 i;

	for(i = 0; i < 4; ++i)
	{
		if(qst->required_item[i])
		{
			if(plr->GetItemInterface()->GetItemCount(qst->required_item[i]) < qst->required_itemcount[i])
			{
				return false;
			}
		}
	}

	return true;
}

uint32 QuestMgr::PlayerMeetsReqs(Player* plr, Quest* qst, bool skiplevelcheck)
{
	std::list<uint32>::iterator itr;
	uint32 status;

	if (!sQuestMgr.IsQuestRepeatable(qst))
		status = QMGR_QUEST_AVAILABLE;
	else
    {
		status = QMGR_QUEST_REPEATABLE;
    }

	if (plr->getLevel() < qst->min_level && !skiplevelcheck)
		return QMGR_QUEST_AVAILABLELOW_LEVEL;

	if(qst->required_class)
		if(!(qst->required_class & plr->getClassMask()))
			return QMGR_QUEST_NOT_AVAILABLE;

	if(qst->required_races)
	{
		if(!(qst->required_races & plr->getRaceMask()))
			return QMGR_QUEST_NOT_AVAILABLE;
	}

	if(qst->required_tradeskill)
	{
		if(!plr->_HasSkillLine(qst->required_tradeskill))
			return QMGR_QUEST_NOT_AVAILABLE;
		if (qst->required_tradeskill_value && plr->_GetSkillLineCurrent(qst->required_tradeskill) < qst->required_tradeskill_value)
			return QMGR_QUEST_NOT_AVAILABLE;
	}

	// Check reputation
	if(qst->required_rep_faction && qst->required_rep_value)
		if(plr->GetStanding(qst->required_rep_faction) < (int32)qst->required_rep_value)
			return QMGR_QUEST_NOT_AVAILABLE;

	uint32 fact;
	// Check reputation limit
	for(uint32 z = 0; z < qst->count_reward_facts; z++)
	{
		fact = qst->reward_repfaction[z];
		if( fact && qst->reward_replimit && plr->GetStanding(fact) >= (int32)qst->reward_replimit )
		{
			return QMGR_QUEST_NOT_AVAILABLE;
		}
	}

	if (plr->HasFinishedQuest(qst->id) && !qst->is_repeatable)
		return QMGR_QUEST_NOT_AVAILABLE;

	if (plr->HasFinishedDailyQuest(qst->id))
		return QMGR_QUEST_NOT_AVAILABLE;

	//Do we just need to complete one, or all quest requisitions?
	if( !qst->required_quest_and_or )
	{
		for(uint32 i = 0; i < qst->count_requiredquests; ++i)
		{
			if (qst->required_quests[i] > 0 && !( plr->HasFinishedQuest(qst->required_quests[i]) || plr->HasFinishedDailyQuest(qst->required_quests[i])))
				return QMGR_QUEST_NOT_AVAILABLE;
		}
	}
	else
	{
		bool check_req=false;
		for(uint32 i = 0; i < qst->count_requiredquests; ++i)
		{
			if(plr->HasFinishedQuest(qst->required_quests[i]) || plr->HasFinishedDailyQuest(qst->required_quests[i]))
				check_req=true;
		}
		if(!check_req)
			return QMGR_QUEST_NOT_AVAILABLE;
	}

	// check quest level
	if( plr->getLevel() >= ( qst->max_level + 5 ) )
		return QMGR_QUEST_CHAT;

	return status;
}

uint32 QuestMgr::CalcQuestStatus(Object* quest_giver, Player* plr, Quest* qst, uint8 type, bool skiplevelcheck)
{
	QuestLogEntry* qle;

	qle = plr->GetQuestLogForEntry(qst->id);

	if (!qle)
	{
		if (type & QUESTGIVER_QUEST_START)
		{
			return PlayerMeetsReqs(plr, qst, skiplevelcheck);
		}
	}
	else
	{		
		if (!qle->CanBeFinished())
		{
			return QMGR_QUEST_NOT_FINISHED;
		}
		else
		{
			if (type & QUESTGIVER_QUEST_END) 
			{
				return QMGR_QUEST_FINISHED;					
			}
			else
			{
				return QMGR_QUEST_NOT_AVAILABLE;
			}
		}
	}

	return QMGR_QUEST_NOT_AVAILABLE;
}

uint8 QuestMgr::CalcStatus(Object* quest_giver, Player* plr)
{
	uint32 status = QMGR_QUEST_NOT_AVAILABLE;
	std::list<QuestRelation *>::const_iterator itr;
	std::list<QuestRelation *>::const_iterator q_begin;
	std::list<QuestRelation *>::const_iterator q_end;
	bool bValid = false;

	if( quest_giver->GetTypeId() == TYPEID_GAMEOBJECT )
	{
        bValid = TO_GAMEOBJECT(quest_giver)->HasQuests();
        if(bValid)
		{
			q_begin = TO_GAMEOBJECT(quest_giver)->QuestsBegin();
			q_end = TO_GAMEOBJECT(quest_giver)->QuestsEnd();
		}
	} 
	else if( quest_giver->GetTypeId() == TYPEID_UNIT )
	{
		bValid = TO_CREATURE( quest_giver )->HasQuests();
		if(bValid)
		{
			q_begin = TO_CREATURE(quest_giver)->QuestsBegin();
			q_end = TO_CREATURE(quest_giver)->QuestsEnd();
		}
	}
    else if( quest_giver->GetTypeId() == TYPEID_ITEM )
    {
        if( TO_ITEM( quest_giver )->GetProto()->QuestId )
            bValid = true;
    }
	//This will be handled at quest share so nothing important as status
	else if(quest_giver->GetTypeId() == TYPEID_PLAYER)
	{
		status = QMGR_QUEST_AVAILABLE;
	}

	if(!bValid)
	{
        //anoying msg that is not needed since all objects dont exactly have quests 
		//OUT_DEBUG("QUESTS: Warning, invalid NPC "I64FMT" specified for CalcStatus. TypeId: %d.", quest_giver->GetGUID(), quest_giver->GetTypeId());
		return status;
	}

    if(quest_giver->GetTypeId() == TYPEID_ITEM)
    {
        Quest *pQuest = QuestStorage.LookupEntry( TO_ITEM(quest_giver)->GetProto()->QuestId );
        QuestRelation qr;
        qr.qst = pQuest;
        qr.type = 1;

        uint32 tmp_status = CalcQuestStatus(quest_giver,plr, &qr);
        if(tmp_status > status)
            status = tmp_status;
    }

	for(itr = q_begin; itr != q_end; ++itr)
	{
		uint32 tmp_status = CalcQuestStatus(quest_giver, plr, *itr);	// save a call
		if (tmp_status > status)
			status = tmp_status;
	}

    return status;
}

uint32 QuestMgr::ActiveQuestsCount(Object* quest_giver, Player* plr)
{
	std::list<QuestRelation *>::const_iterator itr;
	map<uint32, uint8> tmp_map;
	uint32 questCount = 0;

	std::list<QuestRelation *>::const_iterator q_begin;
	std::list<QuestRelation *>::const_iterator q_end;
	bool bValid = false;

	if(quest_giver->GetTypeId() == TYPEID_GAMEOBJECT)
	{
        bValid = TO_GAMEOBJECT(quest_giver)->HasQuests();
		if(bValid)
		{
			q_begin = TO_GAMEOBJECT(quest_giver)->QuestsBegin();
			q_end   = TO_GAMEOBJECT(quest_giver)->QuestsEnd();
			
		}
	} 
	else if(quest_giver->GetTypeId() == TYPEID_UNIT)
	{
		bValid = TO_CREATURE(quest_giver)->HasQuests();
		if(bValid)
		{
			q_begin = TO_CREATURE(quest_giver)->QuestsBegin();
			q_end   = TO_CREATURE(quest_giver)->QuestsEnd();
		}
	}

	if(!bValid)
	{
		OUT_DEBUG("QUESTS: Warning, invalid NPC "I64FMT" specified for ActiveQuestsCount. TypeId: %d.", quest_giver->GetGUID(), quest_giver->GetTypeId());
		return 0;
	}

	for(itr = q_begin; itr != q_end; ++itr)
	{
		if (CalcQuestStatus(quest_giver, plr, *itr) >= QMGR_QUEST_CHAT)
		{
			if (tmp_map.find((*itr)->qst->id) == tmp_map.end())
			{
				tmp_map.insert(std::map<uint32,uint8>::value_type((*itr)->qst->id, 1));
				questCount++;
			}
		}
	}

	return questCount;
}

void QuestMgr::BuildOfferReward(WorldPacket *data, Quest* qst, Object* qst_giver, uint32 menutype, uint32 language, Player* plr)
{
	LocalizedQuest * lq = (language>0) ? sLocalizationMgr.GetLocalizedQuest(qst->id,language):NULL;
	ItemPrototype * it;
	data->SetOpcode(SMSG_QUESTGIVER_OFFER_REWARD);
	*data << qst_giver->GetGUID();
	*data << qst->id;

	if(lq)
	{
		*data << lq->Title;
		*data << lq->CompletionText;
	}
	else
	{
		*data << qst->title;
		*data << qst->completiontext;
	}
	
	//uint32 a = 0, b = 0, c = 1, d = 0, e = 1;

	*data << (qst->next_quest_id ? uint32(1) : uint32(0));	  // next quest shit
	*data << uint32(0);										 // maybe required money
	*data << uint32(1);										 // emotes count
	*data << uint32(0);										 // emote delay
	*data << uint32(1);										 // emote type

	*data << qst->count_reward_choiceitem;
	if (qst->count_reward_choiceitem)
    {
        for(uint32 i = 0; i < 6; ++i)
        {
            if(qst->reward_choiceitem[i])
            {
                *data << qst->reward_choiceitem[i];
                *data << qst->reward_choiceitemcount[i];
                it = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[i]);
                *data << (it ? it->DisplayInfoID : uint32(0));
            }
        }
    }
    

	*data << qst->count_reward_item;
    if (qst->count_reward_item)
    {
        for(uint32 i = 0; i < 4; ++i)
        {
            if(qst->reward_item[i])
            {
                *data << qst->reward_item[i];
                *data << qst->reward_itemcount[i];
                it = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
                *data << (it ? it->DisplayInfoID : uint32(0));
            }
        }
    }
	

	*data << GenerateRewardMoney(plr, qst);
	*data << qst->reward_honor;
	*data << uint32(0);
	*data << qst->reward_spell;
	*data << qst->effect_on_player;
	*data << qst->reward_title;
	*data << qst->reward_talents;
}

void QuestMgr::BuildQuestDetails(WorldPacket *data, Quest* qst, Object* qst_giver, uint32 menutype, uint32 language, Player* plr)
{
	LocalizedQuest * lq = (language>0) ? sLocalizationMgr.GetLocalizedQuest(qst->id,language):NULL;
	std::map<uint32, uint8>::const_iterator itr;

	data->SetOpcode( SMSG_QUESTGIVER_QUEST_DETAILS );

	*data << qst_giver->GetGUID();
	*data << uint64(0);										// 3.0.2 unk
	*data << qst->id;
	if(lq)
	{
		*data << lq->Title;
		*data << lq->Objectives;
		*data << lq->Details;
	}
	else
	{
		*data <<  qst->title;
		*data <<  qst->objectives;
		*data <<  qst->details;
	}

	*data << uint32(1);								// Is Acceptable
	*data << qst->suggested_players;				// "Suggested players"
	*data << uint8(0);								// Pvp warning?
	*data << uint8(0);								// new field in 3.1

	*data << qst->count_reward_choiceitem;
	ItemPrototype *ip;
	uint32 i;

	for(i = 0; i < 6; ++i)
	{
		ip = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[i]);
		if(!qst->reward_choiceitem[i]) continue;

		*data << qst->reward_choiceitem[i];
		*data << qst->reward_choiceitemcount[i];
		if(ip)
			*data << ip->DisplayInfoID;
		else
			*data << uint32(0);
	}

	*data << qst->count_reward_item;

	for(i = 0; i < 4; ++i)
	{
		ip = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
		if(!qst->reward_item[i]) continue;

		*data << qst->reward_item[i];
		*data << qst->reward_itemcount[i];
		if(ip)
			*data << ip->DisplayInfoID;
		else
			*data << uint32(0);
	}

	*data << GenerateRewardMoney(plr, qst);		// Reward Money
	*data << qst->reward_honor;					// Bonus Honor
	*data << qst->reward_spell;					// Reward Spell Id
	*data << qst->effect_on_player;				// Cast Spell Id
	*data << qst->reward_title;					// Reward Title Id
	*data << qst->reward_talents;				// 3.0.2 - Reward Talents

	*data << uint32(4);							// Quantity of emotes, always four
	*data << uint32(1);							// Emote id 1
	*data << uint32(0);							// Emote delay/player emote
	*data << uint32(1);							// Emote id 2
	*data << uint32(0);							// Emote delay/player emote
	*data << uint32(0);							// Emote id 3
	*data << uint32(0);							// Emote delay/player emote
	*data << uint32(0);							// Emote id 4
	*data << uint32(0);							// Emote delay/player emote
}

void QuestMgr::BuildRequestItems(WorldPacket *data, Quest* qst, Object* qst_giver, uint32 status, uint32 language)
{
	LocalizedQuest * lq = (language>0) ? sLocalizationMgr.GetLocalizedQuest(qst->id,language):NULL;
	ItemPrototype * it;
	data->SetOpcode( SMSG_QUESTGIVER_REQUEST_ITEMS );

	*data << qst_giver->GetGUID();
	*data << qst->id;

	if(lq)
	{
		*data << lq->Title;
		*data << ((lq->IncompleteText[0]) ? lq->IncompleteText : lq->Details);
	}
	else
	{
		*data << qst->title;
		*data << (qst->incompletetext[0] ? qst->incompletetext : qst->details);
	}
	
	*data << uint32(0);
	*data << uint32(1);				 // Emote count

	*data << uint32(0);				 // Emote delay
	*data << uint32(1);				 // Emote type
	*data << qst->required_money;	   // Required Money

	// item count
	*data << qst->count_required_item;
	
	// (loop for each item)
	for(uint32 i = 0; i < 4; ++i)
	{
		if(qst->required_item[i] != 0)
		{
			*data << qst->required_item[i];
			*data << qst->required_itemcount[i];
			it = ItemPrototypeStorage.LookupEntry(qst->required_item[i]);
			*data << (it ? it->DisplayInfoID : uint32(0));
		}
	}

	// wtf is this?
    if(status == QMGR_QUEST_NOT_FINISHED)
    {
	    *data << uint32(0); //incomplete button
    }
    else
    {
        *data << uint32(2);
    }

	*data << uint32(4);
	*data << uint32(8);
	*data << uint32(16);
}

void QuestMgr::BuildQuestComplete(Player* plr, Quest* qst)
{
	uint32 xp = 0;
	if(plr->getLevel() < plr->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
	{
		xp = float2int32(GenerateQuestXP(plr,qst) * sWorld.getRate(RATE_QUESTXP));
		plr->GiveXP(xp, 0, false);
	}
  
	WorldPacket data( SMSG_QUESTGIVER_QUEST_COMPLETE,72 );

	data << qst->id;
	data << xp;
	data << uint32(GenerateRewardMoney(plr, qst));
	data << uint32(qst->reward_honor);		// Honor Points
	data << uint32(0);						// 3.0.2
	data << uint32(qst->count_reward_item); //Reward item count

	for(uint32 i = 0; i < 4; ++i)
	{
		if(qst->reward_item[i])
		{
			data << qst->reward_item[i];
			data << qst->reward_itemcount[i];
		}
	}
	plr->GetSession()->SendPacket(&data);
}

void QuestMgr::BuildQuestList(WorldPacket *data, Object* qst_giver, Player* plr, uint32 language)
{
	uint32 status;
	list<QuestRelation *>::iterator it,it2;
	list<QuestRelation *>::iterator st;
	list<QuestRelation *>::iterator ed;
	map<uint32, uint8> tmp_map;

	data->Initialize( SMSG_QUESTGIVER_QUEST_LIST );

	*data << qst_giver->GetGUID();
	*data << "How can I help you?"; //Hello line 
	*data << uint32(1);//Emote Delay
	*data << uint32(1);//Emote

	bool bValid = false;
	if(qst_giver->GetTypeId() == TYPEID_GAMEOBJECT)
	{
		bValid = TO_GAMEOBJECT(qst_giver)->HasQuests();
		if(bValid)
		{
			st = TO_GAMEOBJECT(qst_giver)->QuestsBegin();
			ed = TO_GAMEOBJECT(qst_giver)->QuestsEnd();
		}
	} 
	else if(qst_giver->GetTypeId() == TYPEID_UNIT)
	{
		bValid = TO_CREATURE(qst_giver)->HasQuests();
		if(bValid)
		{
			st = TO_CREATURE(qst_giver)->QuestsBegin();
			ed = TO_CREATURE(qst_giver)->QuestsEnd();
		}
	}

	if(!bValid)
	{
		*data << uint8(0);
		return;
	}
	
	*data << uint8(sQuestMgr.ActiveQuestsCount(qst_giver, plr));

	for (it = st; it != ed;)
	{
		it2 = it;
		++it;
		status = sQuestMgr.CalcQuestStatus(qst_giver, plr, *it2);
		if (status >= QMGR_QUEST_CHAT)
		{
			if (tmp_map.find((*it2)->qst->id) == tmp_map.end())
			{
				tmp_map.insert(std::map<uint32,uint8>::value_type((*it2)->qst->id, 1));
				LocalizedQuest * lq = (language>0) ? sLocalizationMgr.GetLocalizedQuest((*it2)->qst->id,language):NULL;

				*data << (*it2)->qst->id;
				
				switch(status)
				{
				case QMGR_QUEST_NOT_FINISHED:
					*data << uint32(4) << uint32(0);
					break;

				case QMGR_QUEST_FINISHED:
					*data << uint32(4) << uint32(1);
					break;

				case QMGR_QUEST_CHAT:
					*data << uint32( QMGR_QUEST_AVAILABLE ) << uint32( 0 );
					break;

				default:
					*data << status << uint32(0);
				}
				if(lq)
					*data << lq->Title;
				else
					*data << (*it2)->qst->title;
			}
		}
	}
}

void QuestMgr::BuildQuestUpdateAddItem(WorldPacket* data, uint32 itemid, uint32 count)
{
	data->Initialize(SMSG_QUESTUPDATE_ADD_ITEM);
	*data << itemid << count;
}

void QuestMgr::SendQuestUpdateAddKill(Player* plr, uint32 questid, uint32 entry, uint32 count, uint32 tcount, uint64 guid)
{
	WorldPacket data(32);
	data.SetOpcode(SMSG_QUESTUPDATE_ADD_KILL);
	data << questid << entry << count << tcount << guid;
	plr->GetSession()->SendPacket(&data);
}

void QuestMgr::BuildQuestUpdateComplete(WorldPacket* data, Quest* qst)
{
	data->Initialize(SMSG_QUESTUPDATE_COMPLETE);

	*data << qst->id;
}

void QuestMgr::SendPushToPartyResponse(Player* plr, Player* pTarget, uint32 response)
{
	WorldPacket data(MSG_QUEST_PUSH_RESULT, 13);
	data << pTarget->GetGUID();
	data << response;
	data << uint8(0);
	plr->GetSession()->SendPacket(&data);
}

bool QuestMgr::OnGameObjectActivate(Player* plr, GameObject* go)
{
	uint32 i, j;
	QuestLogEntry *qle;
	uint32 entry = go->GetEntry();

	for(i = 0; i < 25; ++i)
	{
		qle = plr->GetQuestLogInSlot( i );
		if( qle != NULL )
		{
			// dont waste time on quests without mobs
			if( qle->GetQuest()->count_required_mob == 0 )
				continue;

			for( j = 0; j < 4; ++j )
			{
				if( qle->GetQuest()->required_mob[j] == entry &&
					qle->GetQuest()->required_mobtype[j] == QUEST_MOB_TYPE_GAMEOBJECT &&
					qle->m_mobcount[j] < qle->GetQuest()->required_mobcount[j] )
				{
					// add another kill.
					// (auto-dirtys it)
					qle->SetMobCount( j, qle->m_mobcount[j] + 1 );
					qle->SendUpdateAddKill( j );
					CALL_QUESTSCRIPT_EVENT( qle, OnGameObjectActivate )( entry, plr, qle );

					if( qle->CanBeFinished() )
						qle->SendQuestComplete();

					qle->UpdatePlayerFields();
					qle->SaveToDB(NULL);
					return true;
				}
			}
		}
	}
	return false;
}

void QuestMgr::OnPlayerKill(Player* plr, Creature* victim) //acctually only needed for external script support
{
	uint32 entry = victim->GetEntry();
	_OnPlayerKill( plr, entry);
}

void QuestMgr::_OnPlayerKill(Player* plr, uint32 creature_entry)
{
	if(!plr)
		return;

	uint32 i, j;
	QuestLogEntry *qle;

	if (plr->HasQuestMob(creature_entry))
	{
		for(i = 0; i < 25; ++i)
		{
			qle = plr->GetQuestLogInSlot( i );
			if( qle != NULL )
			{
				// dont waste time on quests without mobs
				if( qle->GetQuest()->count_required_mob == 0 )
					continue;

				for( j = 0; j < 4; ++j )
				{
					if( qle->GetQuest()->required_mob[j] == creature_entry &&
						qle->GetQuest()->required_mobtype[j] == QUEST_MOB_TYPE_CREATURE &&
						qle->m_mobcount[j] < qle->GetQuest()->required_mobcount[j] )
					{
						// don't update killcount for these questslogentries
						if ( SkippedKills( qle->GetQuest()->id) )
							return;

						// add another kill.(auto-dirtys it)
						qle->SetMobCount( j, qle->m_mobcount[j] + 1 );
						qle->SendUpdateAddKill( j );
						CALL_QUESTSCRIPT_EVENT( qle, OnCreatureKill)( creature_entry, plr, qle );
						qle->UpdatePlayerFields();
						qle->SaveToDB(NULL);
						break;
					}
				}
			}
		}
	}

	// Shared kills
	Player* gplr = NULL;

	if(plr->InGroup())
	{
		if(Group* pGroup = plr->GetGroup())
		{
//			removed by Zack How the hell will healers get the kills then ?
//			if(pGroup->GetGroupType() != GROUP_TYPE_PARTY) 
//				return;  // Raid's don't get shared kills.

			GroupMembersSet::iterator gitr;
			pGroup->Lock();
			for(uint32 k = 0; k < pGroup->GetSubGroupCount(); k++)
			{
				for(gitr = pGroup->GetSubGroup(k)->GetGroupMembersBegin(); gitr != pGroup->GetSubGroup(k)->GetGroupMembersEnd(); ++gitr)
				{
					gplr = (*gitr)->m_loggedInPlayer;
					if(gplr && gplr != plr && plr->isInRange(gplr,300) && gplr->HasQuestMob(creature_entry)) // dont double kills also dont give kills to party members at another side of the world
					{
						for( i = 0; i < 25; ++i )
						{
							qle = gplr->GetQuestLogInSlot(i);
							if( qle != NULL )
							{
								// dont waste time on quests without mobs
								if( qle->GetQuest()->count_required_mob == 0 )
									continue;

								for( j = 0; j < 4; ++j )
								{
									if( qle->GetQuest()->required_mob[j] == creature_entry &&
										qle->GetQuest()->required_mobtype[j] == QUEST_MOB_TYPE_CREATURE &&
										qle->m_mobcount[j] < qle->GetQuest()->required_mobcount[j] )
									{
										// don't update killcount for these questslogentries
										if ( SkippedKills( qle->GetQuest()->id) )
											return;

										// add another kill.
										// (auto-dirtys it)
										qle->SetMobCount(j, qle->m_mobcount[j] + 1);
										qle->SendUpdateAddKill( j );
										CALL_QUESTSCRIPT_EVENT( qle, OnCreatureKill )( creature_entry, plr, qle );
										qle->UpdatePlayerFields();

										if( qle->CanBeFinished() )
											qle->SendQuestComplete();

										qle->SaveToDB(NULL);
										break;
									}
								}
							}
						}
					}
				}
			}
			pGroup->Unlock();
		}
	}
}

void QuestMgr::OnPlayerSlain(Player* plr, Player* victim)
{
	if(!plr || !victim)
		return;

	QuestLogEntry *qle;
	uint32 i;
	for(i = 0; i < 25; ++i)
	{
		if((qle = plr->GetQuestLogInSlot(i)) && qle->m_quest->required_kill_player)
		{
			qle->SetPlayerSlainCount(qle->m_player_slain + 1);
			if(qle->CanBeFinished())
				qle->SendQuestComplete();
			qle->SaveToDB(NULL);
		}
	}

	// Shared kills
	Player* gplr = NULL;

	if(plr->InGroup())
	{
		if(Group* pGroup = plr->GetGroup())
		{
			GroupMembersSet::iterator gitr;
			pGroup->Lock();
			for(uint32 k = 0; k < pGroup->GetSubGroupCount(); k++)
			{
				for(gitr = pGroup->GetSubGroup(k)->GetGroupMembersBegin(); gitr != pGroup->GetSubGroup(k)->GetGroupMembersEnd(); ++gitr)
				{
					gplr = (*gitr)->m_loggedInPlayer;
					if(gplr && gplr != plr && plr->isInRange(gplr,300)) // dont double kills also dont give kills to party members at another side of the world
					{
						for( i = 0; i < 25; ++i )
						{
							qle = gplr->GetQuestLogInSlot(i);
							if( qle != NULL )
							{
								// dont waste time on quests without mobs
								if( qle->GetQuest()->required_kill_player == 0 )
									continue;

								qle->SetPlayerSlainCount(qle->m_player_slain + 1);

								if(qle->CanBeFinished())
									qle->SendQuestComplete();
								qle->SaveToDB(NULL);
							}
						}
					}
				}
			}
			pGroup->Unlock();
		}
	}
}

void QuestMgr::OnPlayerCast(Player* plr, uint32 spellid, uint64& victimguid)
{
	if(!plr || !plr->HasQuestSpell(spellid))
		return;

	Unit* victim = plr->GetMapMgr() ? plr->GetMapMgr()->GetUnit(victimguid) : NULL;
	if(victim==NULL)
		return;

	uint32 i, j;
	uint32 entry = victim->GetEntry();
	QuestLogEntry *qle;
	for(i = 0; i < 25; ++i)
	{
		if((qle = plr->GetQuestLogInSlot(i)))
		{
			// dont waste time on quests without casts
			if(!qle->IsCastQuest())
				continue;

			for(j = 0; j < 4; ++j)
			{
				if(qle->GetQuest()->required_mob[j] == entry &&
					qle->GetQuest()->required_spell[j] == spellid &&
					qle->m_mobcount[j] < qle->GetQuest()->required_mobcount[j] &&
					!qle->IsUnitAffected(victim))
				{
					// add another kill.(auto-dirtys it)
					qle->AddAffectedUnit(victim);
					qle->SetMobCount(j, qle->m_mobcount[j] + 1);
					qle->SendUpdateAddKill(j);
					qle->UpdatePlayerFields();
					qle->SaveToDB(NULL);
					break;
				}
			}
		}
	}
}



void QuestMgr::OnPlayerItemPickup(Player* plr, Item* item)
{
	uint32 i, j;
	uint32 pcount;
	uint32 entry = item->GetEntry();
	QuestLogEntry *qle;
	for( i = 0; i < 25; ++i )
	{
		if( ( qle = plr->GetQuestLogInSlot( i ) ) )
		{
			if( qle->GetQuest()->count_required_item == 0 )
				continue;

			for( j = 0; j < 4; ++j )
			{
				if( qle->GetQuest()->required_item[j] == entry )
				{
					pcount = plr->GetItemInterface()->GetItemCount(entry, true);
					CALL_QUESTSCRIPT_EVENT(qle, OnPlayerItemPickup)(entry, pcount, plr, qle);
					if(pcount < qle->GetQuest()->required_itemcount[j])
					{
						WorldPacket data(8);
						data.SetOpcode(SMSG_QUESTUPDATE_ADD_ITEM);
						data << qle->GetQuest()->required_item[j] << uint32(1);
						plr->GetSession()->SendPacket(&data);
						if(qle->CanBeFinished())
						{
							plr->UpdateNearbyGameObjects();
							qle->SendQuestComplete();
						}
						break;
					}
				}
			}
		}
	}
}

void QuestMgr::OnPlayerExploreArea(Player* plr, uint32 AreaID)
{
	uint32 i, j;
	QuestLogEntry *qle;
	for( i = 0; i < 25; ++i )
	{
		if((qle = plr->GetQuestLogInSlot(i)))
		{
			// dont waste time on quests without triggers
			if( qle->GetQuest()->count_requiredtriggers == 0 )
				continue;

			for( j = 0; j < 4; ++j )
			{
				if(qle->GetQuest()->required_triggers[j] == AreaID &&
					!qle->m_explored_areas[j])
				{
					qle->SetTrigger(j);
					CALL_QUESTSCRIPT_EVENT(qle, OnExploreArea)(qle->m_explored_areas[j], plr, qle);
					qle->UpdatePlayerFields();
					if(qle->CanBeFinished())
					{
						plr->UpdateNearbyGameObjects();
						qle->SendQuestComplete();
					}
					qle->SaveToDB(NULL);
					break;
				}
			}
		}
	}
}

void QuestMgr::GiveQuestRewardReputation(Player* plr, Quest* qst, Object* qst_giver)
{
	// Reputation reward
	for(int z = 0; z < 5; z++)
	{
		uint32 fact = 19;   // default to 19 if no factiondbc
		int32 amt = 0;   // guess
		if( qst->reward_repfaction[z] )
		{
			fact = qst->reward_repfaction[z];
			if(qst->reward_repvalue[z])
				amt = qst->reward_repvalue[z];
		}
		else 
			continue;

		if(amt)
		{
			amt = float2int32( float( amt ) * sWorld.getRate( RATE_QUESTREPUTATION ) ); // reputation rewards 
			if(qst->reward_replimit && plr->GetStanding(fact)+ amt >= (int32)qst->reward_replimit)
			{
				amt = (int32)qst->reward_replimit - plr->GetStanding(fact);
				//prevent substraction when current_rep > limit (this quest should not be available?)
				amt = amt<0 ? 0 : amt;
			}

			plr->ModStanding(fact, amt);
		}
	}
}
void QuestMgr::OnQuestAccepted(Player* plr, Quest* qst, Object* qst_giver)
{
	
}

void QuestMgr::GiveQuestTitleReward(Player* plr, Quest* qst)
{
	if(!qst->reward_title || qst->reward_title > TITLE_END)
		return;

	plr->SetKnownTitle(qst->reward_title, true);
}

void QuestMgr::OnQuestFinished(Player* plr, Quest* qst, Object* qst_giver, uint32 reward_slot)
{
	QuestLogEntry *qle = NULL;
	qle = plr->GetQuestLogForEntry(qst->id);
	if(!qle)
		return;
	BuildQuestComplete(plr, qst);
	CALL_QUESTSCRIPT_EVENT(qle, OnQuestComplete)(plr, qle);

	for (uint32 x=0;x<4;x++)
	{
		if (qst->required_spell[x]!=0)
		{
			if (plr->HasQuestSpell(qst->required_spell[x]))
				plr->RemoveQuestSpell(qst->required_spell[x]);
		}
		else if (qst->required_mob[x]!=0)
		{
			if (plr->HasQuestMob(qst->required_mob[x]))
				plr->RemoveQuestMob(qst->required_mob[x]);
		} 
	}
	qle->ClearAffectedUnits();

	// Cleanup aquired/required spells, items and kills.
	for( uint32 x=0;x<4;x++)
	{
		if( IsQuestRepeatable(qst) || IsQuestDaily(qst) ) //reset kill-counter in case of repeatable's
		{
			if( qst->required_mob[x] && plr->HasQuestMob(qst->required_mob[x]) )
				qle->SetMobCount(x,0);
		}
		else 
		{
			//Remove aquired spells
			if( qst->required_spell[x] && plr->HasQuestSpell(qst->required_spell[x]) )
				plr->RemoveQuestSpell(qst->required_spell[x]);

			//Remove Killed npc's
			if( qst->required_mob[x] && plr->HasQuestMob(qst->required_mob[x]) )
				plr->RemoveQuestMob(qst->required_mob[x]);
		}

		//always remove collected items (need to be recollectable again in case of repeatable).
		if( qst->required_item[x] )
			plr->GetItemInterface()->RemoveItemAmt(qst->required_item[x],qst->required_itemcount[x]);
	}

	qle->Finish();
	
	if(qst_giver->GetTypeId() == TYPEID_UNIT)
	{
		if(!TO_CREATURE(qst_giver)->HasQuest(qst->id, 2))
		{
			//sCheatLog.writefromsession(plr->GetSession(), "tried to finish quest from invalid npc.");
			plr->GetSession()->Disconnect();
			return;
		}
	}

    //details: hmm as i can remember, repeatable quests give faction rep still after first completation
    if(IsQuestRepeatable(qst))
    {
		plr->ModUnsigned32Value(PLAYER_FIELD_COINAGE, GenerateRewardMoney(plr, qst));
		plr->ModUnsigned32Value(PLAYER_CHARACTER_POINTS1, qst->reward_talents);
		HonorHandler::AddHonorPointsToPlayer(plr, qst->reward_honor);
		// Reputation reward
		GiveQuestRewardReputation(plr, qst, qst_giver);
		GiveQuestTitleReward(plr, qst);
        // Static Item reward
	    for(uint32 i = 0; i < 4; ++i)
	    {
		    if(qst->reward_item[i])
		    {
			    ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
			    if(!proto)
			    {
				    OUT_DEBUG("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_item[i], qst->id);
			    }
			    else
			    {   
					Item* add;
					SlotResult slotresult;
					add = plr->GetItemInterface()->FindItemLessMax(qst->reward_item[i], qst->reward_itemcount[i], false);
					if (!add)
					{
						slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
						if(!slotresult.Result)
						{
							plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
						}
						else
						{
							Item* itm = objmgr.CreateItem(qst->reward_item[i], plr);
							itm->SetUInt32Value(ITEM_FIELD_STACK_COUNT, uint32(qst->reward_itemcount[i]));
							if( !plr->GetItemInterface()->SafeAddItem(itm,slotresult.ContainerSlot, slotresult.Slot) )
							{
								itm->Destructor();
							}
						}
					}
					else
					{
						add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + qst->reward_itemcount[i]);
						add->m_isDirty = true;
					}
			    }
		    }
	    }

	    // Choice Rewards
	    if(qst->reward_choiceitem[reward_slot])
	    {
		    ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[reward_slot]);
		    if(!proto)
		    {
			    OUT_DEBUG("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_choiceitem[reward_slot], qst->id);
		    }
		    else
		    {
				Item* add;
				SlotResult slotresult;
				add = plr->GetItemInterface()->FindItemLessMax(qst->reward_choiceitem[reward_slot], qst->reward_choiceitemcount[reward_slot], false);
				if (!add)
				{
					slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
					if(!slotresult.Result)
					{
						plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
					}
					else
					{
						Item* itm = objmgr.CreateItem(qst->reward_choiceitem[reward_slot], plr);
						itm->SetUInt32Value(ITEM_FIELD_STACK_COUNT, uint32(qst->reward_choiceitemcount[reward_slot]));
						if( !plr->GetItemInterface()->SafeAddItem(itm,slotresult.ContainerSlot, slotresult.Slot) )
						{
							itm->Destructor();
						}

					}
				}
				else
				{
					add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + qst->reward_choiceitemcount[reward_slot]);
					add->m_isDirty = true;
				}
		    }
	    }

        // cast Effect Spell
	    if(qst->effect_on_player)
	    {
		    SpellEntry  * inf =dbcSpell.LookupEntry(qst->effect_on_player);
		    if(inf)
		    {
			    Spell* spe(new Spell(qst_giver,inf,true,NULL));
			    SpellCastTargets tgt;
			    tgt.m_unitTarget = plr->GetGUID();
			    spe->prepare(&tgt);
		    }
	    }
    }
    else
    {
	    plr->ModUnsigned32Value(PLAYER_FIELD_COINAGE, GenerateRewardMoney(plr, qst));
		plr->ModUnsigned32Value(PLAYER_CHARACTER_POINTS1, qst->reward_talents);
		HonorHandler::AddHonorPointsToPlayer(plr, qst->reward_honor);
  	
	    // Reputation reward
		GiveQuestRewardReputation(plr, qst, qst_giver);
		GiveQuestTitleReward(plr, qst);
	    // Static Item reward
	    for(uint32 i = 0; i < 4; ++i)
	    {
		    if(qst->reward_item[i])
		    {
			    ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
			    if(!proto)
			    {
				    OUT_DEBUG("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_item[i], qst->id);
			    }
			    else
			    {   
					Item* add;
					SlotResult slotresult;
					add = plr->GetItemInterface()->FindItemLessMax(qst->reward_item[i], qst->reward_itemcount[i], false);
					if (!add)
					{
						slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
						if(!slotresult.Result)
						{
							plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
						}
						else
						{
							Item* itm = objmgr.CreateItem(qst->reward_item[i], plr);
							itm->SetUInt32Value(ITEM_FIELD_STACK_COUNT, uint32(qst->reward_itemcount[i]));
							if( !plr->GetItemInterface()->SafeAddItem(itm,slotresult.ContainerSlot, slotresult.Slot) )
							{
								itm->Destructor();
							}
						}
					}
					else
					{
						add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + qst->reward_itemcount[i]);
						add->m_isDirty = true;
					}
			    }
		    }
	    }

	    // Choice Rewards
	    if(qst->reward_choiceitem[reward_slot])
	    {
		    ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[reward_slot]);
		    if(!proto)
		    {
			    OUT_DEBUG("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_choiceitem[reward_slot], qst->id);
		    }
		    else
		    {
				Item* add;
				SlotResult slotresult;
				add = plr->GetItemInterface()->FindItemLessMax(qst->reward_choiceitem[reward_slot], qst->reward_choiceitemcount[reward_slot], false);
				if (!add)
				{
					slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
					if(!slotresult.Result)
					{
						plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
					}
					else 
					{
						Item* itm = objmgr.CreateItem(qst->reward_choiceitem[reward_slot], plr);
						itm->SetUInt32Value(ITEM_FIELD_STACK_COUNT, uint32(qst->reward_choiceitemcount[reward_slot]));
						if( !plr->GetItemInterface()->SafeAddItem(itm,slotresult.ContainerSlot, slotresult.Slot) )
						{
							itm->Destructor();
						}
					}
				}
				else
				{
					add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + qst->reward_choiceitemcount[reward_slot]);
					add->m_isDirty = true;
				}
			}
		}


		// cast learning spell
		if(qst->reward_spell)
		{
			if(!plr->HasSpell(qst->reward_spell))
			{
				// "Teaching" effect
				WorldPacket data(SMSG_SPELL_START, 42);
				data << qst_giver->GetNewGUID() << qst_giver->GetNewGUID();
				data << uint32(7763);
				data << uint8(0);
				data << uint16(0);
				data << uint32(0);
				data << uint16(2);
				data << plr->GetGUID();
				plr->GetSession()->SendPacket( &data );

				data.Initialize( SMSG_SPELL_GO );
				data << qst_giver->GetNewGUID() << qst_giver->GetNewGUID();
				data << uint32(7763);		   // spellID
				data << uint8(0) << uint8(1);   // flags
				data << uint8(1);			   // amount of targets
				data << plr->GetGUID();		 // target
				data << uint8(0);
				data << uint16(2);
				data << plr->GetGUID();
				plr->GetSession()->SendPacket( &data );

				// Teach the spell
				plr->addSpell(qst->reward_spell);
			}
		}

		// cast Effect Spell
		if(qst->effect_on_player)
		{
			SpellEntry  * inf =dbcSpell.LookupEntry(qst->effect_on_player);
			if(inf)
			{
				Spell* spe(new Spell(qst_giver,inf,true,NULL));
				SpellCastTargets tgt;
				tgt.m_unitTarget = plr->GetGUID();
				spe->prepare(&tgt);
			}
		}
	}

	//Add to finished quests
	if(qst->is_repeatable == REPEATABLE_DAILY)
		plr->AddToFinishedDailyQuests(qst->id);
	else if(!IsQuestRepeatable(qst))
		plr->AddToFinishedQuests(qst->id);

	//Remove any timed events
	if (sEventMgr.HasEvent(plr,EVENT_TIMED_QUEST_EXPIRE))
		sEventMgr.RemoveEvents(plr, EVENT_TIMED_QUEST_EXPIRE); 	

	if(qst->zone_id)
		plr->GetAchievementInterface()->HandleAchievementCriteriaCompleteQuestsInZone(qst->zone_id);
}

/////////////////////////////////////
//		Quest Management		 //
/////////////////////////////////////

void QuestMgr::LoadNPCQuests(Creature* qst_giver)
{
	qst_giver->SetQuestList(GetCreatureQuestList(qst_giver->GetEntry()));
}

void QuestMgr::LoadGOQuests(GameObject* go)
{
	go->SetQuestList(GetGOQuestList(go->GetEntry()));
}

QuestRelationList* QuestMgr::GetGOQuestList(uint32 entryid)
{
	HM_NAMESPACE::hash_map<uint32, QuestRelationList* > &olist = _GetList<GameObject>();
	HM_NAMESPACE::hash_map<uint32, QuestRelationList* >::iterator itr = olist.find(entryid);
	return (itr == olist.end()) ? 0 : itr->second;
}

QuestRelationList* QuestMgr::GetCreatureQuestList(uint32 entryid)
{
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* > &olist = _GetList<Creature>();
	HM_NAMESPACE::hash_map<uint32, QuestRelationList* >::iterator itr = olist.find(entryid);
	return (itr == olist.end()) ? 0 : itr->second;
}

QuestAssociationList* QuestMgr::GetQuestAssociationListForItemId (uint32 itemId)
{
	HM_NAMESPACE::hash_map<uint32, QuestAssociationList* > &associationList = GetQuestAssociationList();
	HM_NAMESPACE::hash_map<uint32, QuestAssociationList* >::iterator itr = associationList.find( itemId );
	return (itr != associationList.end()) ? itr->second : 0;
}

void QuestMgr::AddItemQuestAssociation( uint32 itemId, Quest *qst, uint8 item_count)
{
	HM_NAMESPACE::hash_map<uint32, list<QuestAssociation *>* > &associationList = GetQuestAssociationList();
	std::list<QuestAssociation *>* tempList;
	QuestAssociation *ptr = NULL;
	
	// look for the item in the associationList
	if (associationList.find( itemId ) == associationList.end() )
	{
		// not found. Create a new entry and QuestAssociationList
		tempList = new std::list<QuestAssociation *>;

		associationList.insert(HM_NAMESPACE::hash_map<uint32, list<QuestAssociation *>* >::value_type(itemId, tempList));
	}
	else
	{
		// item found, now we'll search through its QuestAssociationList
		tempList = associationList.find( itemId )->second;
	}
	
	// look through this item's QuestAssociationList for a matching quest entry
	list<QuestAssociation *>::iterator it;
	for (it = tempList->begin(); it != tempList->end(); ++it)
	{
		if ((*it)->qst == qst)
		{
			// matching quest found
			ptr = (*it);
			break;
		}
	}

	// did we find a matching quest?
	if (ptr == NULL)
	{
		// nope, create a new QuestAssociation for this item and quest
		ptr = new QuestAssociation;
		ptr->qst = qst;
		ptr->item_count = item_count;

		tempList->push_back( ptr );
	}
	else
	{
		// yep, update the QuestAssociation with the new item_count information 
		ptr->item_count = item_count;
		sLog.outDebug( "WARNING: Duplicate entries found in item_quest_association, updating item #%d with new item_count: %d.", itemId, item_count );
	}
}

template <class T> void QuestMgr::_AddQuest(uint32 entryid, Quest *qst, uint8 type)
{
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* > &olist = _GetList<T>();
	std::list<QuestRelation *>* nlist;
	QuestRelation *ptr = NULL;

	if (olist.find(entryid) == olist.end())
	{
		nlist = new std::list<QuestRelation *>;

		olist.insert(HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* >::value_type(entryid, nlist));
	}
	else
	{
		nlist = olist.find(entryid)->second;
	}

	list<QuestRelation *>::iterator it;
	for (it = nlist->begin(); it != nlist->end(); ++it)
	{
		if ((*it)->qst == qst)
		{
			ptr = (*it);
			break;
		}
	}

	if (ptr == NULL)
	{
		ptr = new QuestRelation;
		ptr->qst = qst;
		ptr->type = type;

		nlist->push_back(ptr);
	}
	else
	{
		ptr->type |= type;
	}
}



void QuestMgr::_CleanLine(std::string *str) 
{
	_RemoveChar((char*)"\r", str);
	_RemoveChar((char*)"\n", str);

	while (str->c_str()[0] == 32) 
	{
		str->erase(0,1);
	}
}

void QuestMgr::_RemoveChar(char *c, std::string *str) 
{
	string::size_type pos = str->find(c,0);

	while (pos != string::npos)
	{
		str->erase(pos, 1);
		pos = str->find(c, 0);
	}	
}

uint32 QuestMgr::GenerateRewardMoney( Player* pl, Quest * qst )
{
	if ( qst->reward_money < 0 )
		return 0;

	if ( pl && pl->getLevel() >= pl->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL) && qst->is_repeatable == 0 )
		return qst->reward_money + float2int32( GenerateQuestXP( pl, qst ) * sWorld.getRate( RATE_QUESTXP ) ) * 6;
	else
		return qst->reward_money;
}

uint32 QuestMgr::GenerateQuestXP(Player* plr, Quest *qst)	
{	
	if(qst->is_repeatable)
		return 0;	
	{	
  if( plr->getLevel() <= qst->max_level +  5 )
      return qst->reward_xp;	
  if( plr->getLevel() == qst->max_level +  6 )
      return (uint32)(qst->reward_xp * 0.8);
  if( plr->getLevel() == qst->max_level +  7 )
      return (uint32)(qst->reward_xp * 0.6);
  if( plr->getLevel() == qst->max_level +  8 )
      return (uint32)(qst->reward_xp * 0.4);
  if( plr->getLevel() == qst->max_level +  9 )
      return (uint32)(qst->reward_xp * 0.2);
		     
  else
      return 0;
   }   
}
/*
#define XP_INC 50
#define XP_DEC 10
#define XP_INC100 15
#define XP_DEC100 5
	double xp, pxp, mxp, mmx;

	// hack fix
	xp  = qst->max_level * XP_INC;
	if(xp <= 0)
		xp = 1;

	pxp  = xp + (xp / 100) * XP_INC100;

	xp   = XP_DEC;

	mxp  = xp + (xp / 100) * XP_DEC100;

	mmx = (pxp - mxp);

	if(qst->quest_flags & QUEST_FLAG_SPEAKTO)
		mmx *= 0.6;
	if(qst->quest_flags & QUEST_FLAG_TIMED)
		mmx *= 1.1;
	if(qst->quest_flags & QUEST_FLAG_EXPLORATION)
		mmx *= 1.2;

	if(mmx < 0)
		return 1;

	mmx *= sWorld.getRate(RATE_QUESTXP);
	return (int)mmx;*/


void QuestMgr::SendQuestInvalid(INVALID_REASON reason, Player* plyr)
{
	if(!plyr)
		return;
	plyr->GetSession()->OutPacket(SMSG_QUESTGIVER_QUEST_INVALID, 4, &reason);
	OUT_DEBUG("WORLD:Sent SMSG_QUESTGIVER_QUEST_INVALID");
}

void QuestMgr::SendQuestFailed(FAILED_REASON failed, Quest * qst, Player* plyr)
{
	if(!plyr)
		return;

    WorldPacket data(8);
    data.Initialize(SMSG_QUESTGIVER_QUEST_FAILED);
    data << uint32(qst->id);
    data << failed;
    plyr->GetSession()->SendPacket(&data);
	OUT_DEBUG("WORLD:Sent SMSG_QUESTGIVER_QUEST_FAILED");
}

void QuestMgr::SendQuestUpdateFailedTimer(Quest *pQuest, Player* plyr)
{
	if(!plyr)
		return;

	plyr->GetSession()->OutPacket(SMSG_QUESTUPDATE_FAILEDTIMER, 4, &pQuest->id);
	OUT_DEBUG("WORLD:Sent SMSG_QUESTUPDATE_FAILEDTIMER");
}

void QuestMgr::SendQuestUpdateFailed(Quest *pQuest, Player* plyr)
{
	if(!plyr)
		return;

	plyr->GetSession()->OutPacket(SMSG_QUESTUPDATE_FAILED, 4, &pQuest->id);
	OUT_DEBUG("WORLD:Sent SMSG_QUESTUPDATE_FAILED");
}

void QuestMgr::SendQuestLogFull(Player* plyr)
{
	if(!plyr)
		return;

	plyr->GetSession()->OutPacket(SMSG_QUESTLOG_FULL);
	OUT_DEBUG("WORLD:Sent QUEST_LOG_FULL_MESSAGE");
}

uint32 QuestMgr::GetGameObjectLootQuest(uint32 GO_Entry)
{
	HM_NAMESPACE::hash_map<uint32, uint32>::iterator itr = m_ObjectLootQuestList.find(GO_Entry);
	if(itr == m_ObjectLootQuestList.end()) return 0;
	
	return itr->second;
}

void QuestMgr::SetGameObjectLootQuest(uint32 GO_Entry, uint32 Item_Entry)
{
	if(m_ObjectLootQuestList.find(GO_Entry) != m_ObjectLootQuestList.end())
	{
		//sLog.outError("WARNING: Gameobject %d has more than 1 quest item allocated in it's loot template!", GO_Entry);
	}

	// Find the quest that has that item
	uint32 QuestID = 0;
	uint32 i;
	StorageContainerIterator<Quest> * itr = QuestStorage.MakeIterator();
	while(!itr->AtEnd())
	{
		Quest *qst = itr->Get();
		for(i = 0; i < 4; ++i)
		{
			if(qst->required_item[i] == Item_Entry)
			{
				QuestID = qst->id;
				m_ObjectLootQuestList[GO_Entry] = QuestID;
				itr->Destruct();
				return;
			}
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	//sLog.outError("WARNING: No coresponding quest was found for quest item %d", Item_Entry);
}

void QuestMgr::BuildQuestFailed(WorldPacket* data, uint32 questid)
{
	data->Initialize(SMSG_QUESTUPDATE_FAILEDTIMER);
	*data << questid;
}

bool QuestMgr::OnActivateQuestGiver(Object* qst_giver, Player* plr)
{
	if(qst_giver->GetTypeId() == TYPEID_GAMEOBJECT && !TO_GAMEOBJECT(qst_giver)->HasQuests())
		return false;

	uint32 questCount = sQuestMgr.ActiveQuestsCount(qst_giver, plr);
	WorldPacket data(1000);	

	if (questCount == 0) 
	{
		OUT_DEBUG("WORLD: Invalid NPC for CMSG_QUESTGIVER_HELLO.");
		return false;
	}
	else if (questCount == 1)
	{
		std::list<QuestRelation *>::const_iterator itr;
		std::list<QuestRelation *>::const_iterator q_begin;
		std::list<QuestRelation *>::const_iterator q_end;

		bool bValid = false;

		if(qst_giver->GetTypeId() == TYPEID_GAMEOBJECT)
		{
            bValid = TO_GAMEOBJECT(qst_giver)->HasQuests();
            if(bValid)
            {
				q_begin = TO_GAMEOBJECT(qst_giver)->QuestsBegin();
				q_end   = TO_GAMEOBJECT(qst_giver)->QuestsEnd();
			}
		} 
		else if(qst_giver->GetTypeId() == TYPEID_UNIT)
		{
			bValid = TO_CREATURE(qst_giver)->HasQuests();
			if(bValid)
			{
				q_begin = TO_CREATURE(qst_giver)->QuestsBegin();
				q_end   = TO_CREATURE(qst_giver)->QuestsEnd();
			}
		}

		if(!bValid)
		{
			OUT_DEBUG("QUESTS: Warning, invalid NPC "I64FMT" specified for OnActivateQuestGiver. TypeId: %d.", qst_giver->GetGUID(), qst_giver->GetTypeId());
			return false;
		}
		
		for(itr = q_begin; itr != q_end; ++itr) 
			if (sQuestMgr.CalcQuestStatus(qst_giver, plr, *itr) >= QMGR_QUEST_CHAT)
				break;

		if (sQuestMgr.CalcStatus(qst_giver, plr) < QMGR_QUEST_CHAT)
			return false; 

		ASSERT(itr != q_end);

		uint8 status = sQuestMgr.CalcStatus(qst_giver, plr);

		if ((status == QMGR_QUEST_AVAILABLE) || (status == QMGR_QUEST_REPEATABLE) || (status == QMGR_QUEST_CHAT))
		{
			sQuestMgr.BuildQuestDetails(&data, (*itr)->qst, qst_giver, 1, plr->GetSession()->language, plr);		// 1 because we have 1 quest, and we want goodbye to function
			plr->GetSession()->SendPacket(&data);
			DEBUG_LOG( "WORLD"," Sent SMSG_QUESTGIVER_QUEST_DETAILS." );
		}
		else if (status == QMGR_QUEST_FINISHED)
		{
			sQuestMgr.BuildOfferReward(&data, (*itr)->qst, qst_giver, 1, plr->GetSession()->language, plr);
			plr->GetSession()->SendPacket(&data);
			//ss
			DEBUG_LOG( "WORLD"," Sent SMSG_QUESTGIVER_OFFER_REWARD." );
		}
		else if (status == QMGR_QUEST_NOT_FINISHED)
		{
			sQuestMgr.BuildRequestItems(&data, (*itr)->qst, qst_giver, status, plr->GetSession()->language);
			plr->GetSession()->SendPacket(&data);
			DEBUG_LOG( "WORLD"," Sent SMSG_QUESTGIVER_REQUEST_ITEMS." );
		}
	}
	else 
	{
		sQuestMgr.BuildQuestList(&data, qst_giver ,plr, plr->GetSession()->language);
		plr->GetSession()->SendPacket(&data);
		DEBUG_LOG( "WORLD"," Sent SMSG_QUESTGIVER_QUEST_LIST." );
	}
	return true;
}

QuestMgr::~QuestMgr()
{
	HM_NAMESPACE::hash_map<uint32, Quest*>::iterator itr1;
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* >::iterator itr2;
	list<QuestRelation*>::iterator itr3;
	HM_NAMESPACE::hash_map<uint32, list<QuestAssociation *>* >::iterator itr4;
	std::list<QuestAssociation*>::iterator itr5;
	std::list<uint32>::iterator itr6;

	// clear relations
	for(itr2 = m_obj_quests.begin(); itr2 != m_obj_quests.end(); ++itr2)
	{
		if(!itr2->second)
			continue;

		itr3 = itr2->second->begin();
		for(; itr3 != itr2->second->end(); ++itr3)
		{
			delete (*itr3);
		}
		itr2->second->clear();
		delete itr2->second;
	}
	m_obj_quests.clear(); 

	for(itr2 = m_npc_quests.begin(); itr2 != m_npc_quests.end(); ++itr2)
	{
		if(!itr2->second)
			continue;

		itr3 = itr2->second->begin();
		for(; itr3 != itr2->second->end(); ++itr3)
		{
			delete (*itr3);
		}
		itr2->second->clear();
		delete itr2->second;
	}
	m_npc_quests.clear();

	for(itr2 = m_itm_quests.begin(); itr2 != m_itm_quests.end(); ++itr2)
	{
		if(!itr2->second)
			continue;

		itr3 = itr2->second->begin();
		for(; itr3 != itr2->second->end(); ++itr3)
		{
			delete (*itr3);
		}
		itr2->second->clear();
		delete itr2->second;
	}
	m_itm_quests.clear(); 

	for(itr4 = m_quest_associations.begin(); itr4 != m_quest_associations.end(); ++itr4)
	{
		if(!itr4->second)
			continue;

		itr5 = itr4->second->begin();
		for(; itr5 != itr4->second->end(); ++itr5)
		{
			delete (*itr5);
		}
		itr4->second->clear();
		delete itr4->second;
	}
	m_quest_associations.clear();

	for(itr6 = m_extraqueststuff_list.begin(); itr6 != m_extraqueststuff_list.end(); ++itr6)
	{
		GameObjectInfo *inf = GameObjectNameStorage.LookupEntry(*itr6);
		if( inf == NULL )
			continue;

		delete [] inf->InvolvedQuestIds;
	}
}


bool QuestMgr::CanStoreReward(Player* plyr, Quest *qst, uint32 reward_slot)
{
    uint32 available_slots = 0;
    uint32 slotsrequired = 0;
    available_slots = plyr->GetItemInterface()->CalculateFreeSlots(NULL);
    // Static Item reward
    for(uint32 i = 0; i < 4; ++i)
    {
        if(qst->reward_item[i])
        {
            slotsrequired++;
            ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
            if(!proto)
                OUT_DEBUG("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_item[i], qst->id);
            else if(plyr->GetItemInterface()->CanReceiveItem(proto, qst->reward_itemcount[i], NULL))
				return false;
        }
    }

    // Choice Rewards
    if(qst->reward_choiceitem[reward_slot])
    {
        slotsrequired++;
        ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[reward_slot]);
        if(!proto)
            OUT_DEBUG("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_choiceitem[reward_slot], qst->id);
        else if(plyr->GetItemInterface()->CanReceiveItem(proto, qst->reward_choiceitemcount[reward_slot], NULL))
			return false;
    }
    if(available_slots < slotsrequired)
	{
        return false;
	}

	return true;
}

void QuestMgr::LoadExtraQuestStuff()
{
	StorageContainerIterator<Quest> * it = QuestStorage.MakeIterator();
	Quest * qst;
	map<uint32, set<uint32> > tmp_map;
	map<uint32, vector<uint32> > loot_map;

	lootmgr.LoadLoot();
	lootmgr.FillObjectLootMap(&loot_map);
	DEBUG_LOG("QuestMgr","Creating gameobject involved quest map...");

	while(!it->AtEnd())
	{
		qst = it->Get();

		// 0 them out
		qst->count_required_item = 0;
		qst->count_required_mob = 0;
		qst->count_requiredtriggers = 0;
		qst->count_receiveitems = 0;
		qst->count_reward_item = 0;
		qst->count_reward_choiceitem = 0;

		qst->required_mobtype[0] = 0;
		qst->required_mobtype[1] = 0;
		qst->required_mobtype[2] = 0;
		qst->required_mobtype[3] = 0;

		qst->count_requiredquests = 0;
		qst->count_reward_facts = 0;

		if(qst->is_repeatable != 0 && (qst->quest_flags & 4096))
			qst->is_repeatable = REPEATABLE_DAILY;

		for(int i = 0 ; i < 4; ++i)
		{
			if(qst->required_mob[i])
			{
				GameObjectInfo *go_info = GameObjectNameStorage.LookupEntry(qst->required_mob[i]);
				CreatureInfo   *c_info  = CreatureNameStorage.LookupEntry(qst->required_mob[i]);
				if(go_info && (go_info->Type == GAMEOBJECT_TYPE_GOOBER || qst->quest_flags == 10 || !c_info))
				{
					qst->required_mobtype[i] = QUEST_MOB_TYPE_GAMEOBJECT;
					tmp_map[go_info->ID].insert(qst->id);
				}
				else
				{
					qst->required_mobtype[i] = QUEST_MOB_TYPE_CREATURE;
				}

				qst->count_required_mob++;
			}

			if(qst->required_item[i])
			{
				qst->count_required_item++;
				map<uint32, vector<uint32> >::iterator tt = loot_map.find(qst->required_item[i]);
				if( tt != loot_map.end() )
				{
					vector<uint32>::iterator tt2 = tt->second.begin();
					for( ; tt2 != tt->second.end(); ++tt2 )
					{
						// this only applies if the only items under the loot template are quest items
						LootStore::iterator itr = lootmgr.GOLoot.find((*tt2));
						bool has_other = false;
						bool has_quest = true;
						if( itr != lootmgr.GOLoot.end() )
						{
							for(uint32 xx = 0; xx < itr->second.count; ++xx )
							{
								if( itr->second.items[xx].item.itemproto == NULL )
									continue; 

								if( itr->second.items[xx].item.itemproto->Class != ITEM_CLASS_QUEST )
								{
									has_other = true;
									break;
								}
								else
									has_quest = true;
							}
						}

						if( !has_other && has_quest )
							tmp_map[(*tt2)].insert(qst->id);
					}
				}
			}

			if(qst->reward_item[i])
				qst->count_reward_item++;

			if(qst->required_triggers[i])
				qst->count_requiredtriggers++;

			if(qst->receive_items[i])
				qst->count_receiveitems++;

			if(qst->required_quests[i])
				qst->count_requiredquests++;
		}

		for(int i = 0; i < 6; ++i)
			if(qst->reward_choiceitem[i])
				qst->count_reward_choiceitem++;

		for(int i = 0; i < 5; ++i)
		{
			if(qst->reward_repfaction[i])
				qst->count_reward_facts++;
		}

		if(!it->Inc())
			break;
	}

	it->Destruct();

	for(map<uint32, set<uint32> >::iterator itr = tmp_map.begin(); itr != tmp_map.end(); ++itr)
	{
		GameObjectInfo *inf = GameObjectNameStorage.LookupEntry(itr->first);
		if( inf == NULL )
			continue;

		inf->InvolvedQuestIds = new uint32[itr->second.size()];
		m_extraqueststuff_list.push_back(itr->first);
		set<uint32>::iterator vtr = itr->second.begin();
		uint32 j = 0;

		for(;vtr != itr->second.end(); ++vtr, ++j)
			inf->InvolvedQuestIds[j] = *vtr;

		inf->InvolvedQuestCount = (uint32)itr->second.size();
	}

	// load creature starters
	uint32 creature, quest;

	QueryResult * pResult = WorldDatabase.Query("SELECT * FROM creature_quest_starter");
	uint32 pos = 0;
	uint32 total;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do 
		{
			Field *data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
				Log.Warning("QuestMgr","Tried to add starter to npc %d for non-existant quest %d.", creature, quest);
			else 
				_AddQuest<Creature>(creature, qst, 1);  // 1 = starter
		} while(pResult->NextRow());
		delete pResult;
		Log.Notice("QuestMgr","Marked %u creatures as quest starter", total);
	}

	pResult = WorldDatabase.Query("SELECT * FROM creature_quest_finisher");
	pos = total = 0;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do 
		{
			Field *data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
				Log.Warning("QuestMgr","Tried to add finisher to npc %d for non-existant quest %d.", creature, quest);
			else 
				_AddQuest<Creature>(creature, qst, 2);  // 1 = starter
		} while(pResult->NextRow());
		delete pResult;
		Log.Notice("QuestMgr","Marked %u creatures as quest finisher", total);
	}

	pResult = WorldDatabase.Query("SELECT * FROM gameobject_quest_starter");
	pos = total = 0;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do 
		{
			Field *data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
				Log.Warning("QuestMgr","Tried to add starter to go %d for non-existant quest %d.\n", creature, quest);
			else
				_AddQuest<GameObject>(creature, qst, 1);  // 1 = starter
		} while(pResult->NextRow());
		delete pResult;
		Log.Notice("QuestMgr","Marked %u gameobjects as quest starter", total);
	}

	pResult = WorldDatabase.Query("SELECT * FROM gameobject_quest_finisher");
	pos = total = 0;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do 
		{
			Field *data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
				Log.Warning("QuestMgr","Tried to add finisher to go %d for non-existant quest %d.\n", creature, quest);
			else 
				_AddQuest<GameObject>(creature, qst, 2);  // 2 = finish
		} while(pResult->NextRow());
		delete pResult;
		Log.Notice("QuestMgr","Marked %u gameobjects as quest finisher", total);
	}

	//load item quest associations
	pResult = WorldDatabase.Query("SELECT * FROM item_quest_association");
	pos = total = 0;
	if( pResult != NULL)
	{
		uint32 item = 0;
		uint8 item_count = 0;
		total = pResult->GetRowCount();
		do 
		{
			Field *data = pResult->Fetch();
			item = data[0].GetUInt32();
			quest = data[1].GetUInt32();
			item_count = data[2].GetUInt8();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
				Log.Warning("QuestMgr","Tried to add association to item %d for non-existant quest %d.\n", item, quest);
			else 
				AddItemQuestAssociation( item, qst, item_count );
		} while( pResult->NextRow() );
		delete pResult;
		Log.Notice("QuestMgr","Loaded %u item-quest associations", total);
	}

	//Proccess the stuff
	objmgr.ProcessGameobjectQuests();
}

bool QuestMgr::SkippedKills( uint32 QuestID )
{
	switch(QuestID)
	{
		case 6061:
		case 6062:
		case 6063:
		case 6064:
		case 6082:
		case 6083:
		case 6084:
		case 6085:
		case 6087:
		case 6088:
		case 6101:
		case 6102:
		case 9484:
		case 9485:
		case 9486:
		case 9591:
		case 9592:
		case 9593:
			return true;
	}
	return false;
}
