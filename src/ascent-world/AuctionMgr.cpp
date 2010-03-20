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

initialiseSingleton( AuctionMgr );

void AuctionMgr::LoadAuctionHouses()
{
	Log.Notice("AuctionMgr", "Loading Auction Houses...");

	QueryResult * res = CharacterDatabase.Query("SELECT MAX(auctionId) FROM auctions");
	if(res)
	{
		maxId = res->Fetch()[0].GetUInt32();
		delete res;
	}

	res = WorldDatabase.Query("SELECT DISTINCT `group` FROM auctionhouse");
	AuctionHouse * ah;
	map<uint32, AuctionHouse*> tempmap;
	if(res)
	{
		uint32 period = (res->GetRowCount() / 20) + 1;
		uint32 c = 0;
		do
		{
			ah = new AuctionHouse(res->Fetch()[0].GetUInt32());
			ah->LoadAuctions();
			auctionHouses.push_back(ah);
			tempmap.insert( make_pair( res->Fetch()[0].GetUInt32(), ah ) );
			if( !((++c) % period) )
				Log.Notice("AuctionHouse", "Done %u/%u, %u%% complete.", c, res->GetRowCount(), float2int32( (float(c) / float(res->GetRowCount()))*100.0f ));

		}while(res->NextRow());
		delete res;
	}

	res = WorldDatabase.Query("SELECT creature_entry, `group` FROM auctionhouse");
	if(res)
	{
		do 
		{
			auctionHouseEntryMap.insert( make_pair( res->Fetch()[0].GetUInt32(), tempmap[res->Fetch()[1].GetUInt32()] ) );
		} while(res->NextRow());
		delete res;
	}
}

AuctionHouse * AuctionMgr::GetAuctionHouse(uint32 Entry)
{
	HM_NAMESPACE::hash_map<uint32, AuctionHouse*>::iterator itr = auctionHouseEntryMap.find(Entry);
	if(itr == auctionHouseEntryMap.end()) return NULL;
	return itr->second;
}

void AuctionMgr::Update()
{
	if((++loopcount % 100))
		return;
		
	vector<AuctionHouse*>::iterator itr = auctionHouses.begin();
	for(; itr != auctionHouses.end(); ++itr)
	{
		(*itr)->UpdateDeletionQueue();

		// Actual auction loop is on a seperate timer.
		if(!(loopcount % 1200))
			(*itr)->UpdateAuctions();
	}
}
