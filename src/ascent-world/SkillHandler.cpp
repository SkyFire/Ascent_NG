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

#include "StdAfx.h"

/************************************************************************/
/* Talent Anti-Cheat                                                    */
/************************************************************************/

/*
	mage - arcane - 81			
	mage - fire - 41
	mage - frost - 61

	rogue - assassination - 182
	rogue - combat - 181
	rogue - subelty - 183

	warlock - afflication - 302
	warlock - demonology - 303
	warlock - destruction - 301

	warrior - arms - 161
	warrior - fury - 163
	warrior - protection - 164

	shaman - elemental - 261
	shaman - enchantment - 263
	shaman - restoration - 262

	paladin - holy - 382
	paladin - protection - 383
	paladin - retribution - 381

	priest - dicipline - 201
	priest - holy - 202
	priest - shadow - 203

	hunter - beast - 361
	hunter - marksmanship - 363
	hunter - survival - 362

	druid - balance - 283
	druid - feral combat - 281
	druid - restoration - 282
*/

void WorldSession::HandleLearnTalentOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
 	 
	uint32 talent_id, requested_rank;	
	recv_data >> talent_id >> requested_rank;

	_player->LearnTalent(talent_id, requested_rank);

	_player->UpdateTalentInspectBuffer();
	_player->smsg_TalentsInfo(true, talent_id, requested_rank);
}

void WorldSession::HandleLearnPreviewTalents( WorldPacket & recv_data )
{
	uint32 count = 0;
	uint32 talent_id, requested_rank;
	recv_data >> count;
	for(uint32 i = 0; i < count && recv_data.rpos() < recv_data.size(); i++)
	{
		recv_data >> talent_id >> requested_rank;
		_player->LearnTalent(talent_id, requested_rank);
	}
	_player->UpdateTalentInspectBuffer();
	_player->smsg_TalentsInfo(false, 0, 0);
}

void WorldSession::HandleUnlearnTalents( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
	uint32 playerGold = GetPlayer()->GetUInt32Value( PLAYER_FIELD_COINAGE );
	uint32 price = GetPlayer()->CalcTalentResetCost(GetPlayer()->GetTalentResetTimes());

	if( playerGold < price ) return;

	GetPlayer()->SetTalentResetTimes(GetPlayer()->GetTalentResetTimes() + 1);
	GetPlayer()->SetUInt32Value( PLAYER_FIELD_COINAGE, playerGold - price );
	GetPlayer()->Reset_Talents();

	GetPlayer()->GetAchievementInterface()->HandleAchievementCriteriaTalentResetCostTotal( price );
	GetPlayer()->GetAchievementInterface()->HandleAchievementCriteriaTalentResetCount();
}

void WorldSession::HandleUnlearnSkillOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN;
	uint32 skill_line;
	uint32 points_remaining=_player->GetUInt32Value(PLAYER_CHARACTER_POINTS2);
	recv_data >> skill_line;

	// Cheater detection
	// if(!_player->HasSkillLine(skill_line)) return;

	// Remove any spells within that line that the player has
	_player->RemoveSpellsFromLine(skill_line);
	
	// Remove any quests finished and active for this skill
	_player->RemoveQuestsFromLine(skill_line);

	// Finally, remove the skill line.
	_player->_RemoveSkillLine(skill_line);

	//added by Zack : This is probably wrong or already made elsewhere : restore skill learnability
	if(points_remaining==_player->GetUInt32Value(PLAYER_CHARACTER_POINTS2))
	{
		//we unlearned a kill so we enable a new one to be learned
		skilllineentry *sk=dbcSkillLine.LookupEntry(skill_line);
		if(!sk)
			return;
		if(sk->type==SKILL_TYPE_PROFESSION && points_remaining<2)
			_player->SetUInt32Value(PLAYER_CHARACTER_POINTS2,points_remaining+1);
	}
}
