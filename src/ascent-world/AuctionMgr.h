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

#ifndef __AUCTIONMGR_H
#define __AUCTIONMGR_H

class AuctionHouse;
class AuctionMgr : public Singleton <AuctionMgr>
{
public:
	AuctionMgr()
	{
		loopcount = 0;
		maxId = 1;
	}

	~AuctionMgr()
	{
		vector<AuctionHouse*>::iterator itr = auctionHouses.begin();
		for(; itr != auctionHouses.end(); ++itr)
			delete (*itr);
	}

	void LoadAuctionHouses();
	void Update();

	AuctionHouse * GetAuctionHouse(uint32 Entry);

	uint32 GenerateAuctionId()
	{ 
		lock.Acquire();
		uint32 id=++maxId;
		lock.Release();
		return id;
	}

	vector<AuctionHouse*>::iterator GetAuctionHousesBegin() { return auctionHouses.begin(); }
	vector<AuctionHouse*>::iterator GetAuctionHousesEnd() { return auctionHouses.end(); }

private:
	HM_NAMESPACE::hash_map<uint32, AuctionHouse*> auctionHouseEntryMap;
	vector<AuctionHouse*> auctionHouses;
	volatile uint32 maxId;
	Mutex lock;
	uint32 loopcount;
};

#define sAuctionMgr AuctionMgr::getSingleton()

#endif
