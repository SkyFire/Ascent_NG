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

#ifndef HONORHANDLER_H
#define HONORHANDLER_H

class HonorHandler
{
public:
	static int32 CalculateHonorPointsForKill(Player* pPlayer, Unit* pVictim);
	static void RecalculateHonorFields(Player* pPlayer);
	static void AddHonorPointsToPlayer(Player* pPlayer, uint32 uAmount);
	static void OnPlayerKilledUnit(Player* pPlayer, Unit* pVictim);
	static int32 CalculateHonorPointsFormula(uint32 AttackerLevel,uint32 VictimLevel);
	static void UpdateCurrencyItem(Player* pPlayer, uint32 itemid, uint32 amount);
};


#endif
