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

#ifndef __TAXIMGR_H
#define __TAXIMGR_H

#define TAXI_TRAVEL_SPEED 32

class Player;

struct TaxiNode {
	uint32 id;
	float x,y,z;
	uint32 mapid;
	uint32 horde_mount;
	uint32 alliance_mount;
};

struct TaxiPathNode 
{
	float x,y,z;
	uint32 mapid;
};

class SERVER_DECL TaxiPath {
	friend class TaxiMgr;

public:
	TaxiPath() 
	{ 
	}

	~TaxiPath() 
	{
		while(m_pathNodes.size())
		{
			TaxiPathNode *pn = m_pathNodes.begin()->second;
			m_pathNodes.erase(m_pathNodes.begin());
			delete pn;
		}
	}

	void ComputeLen();
	void SetPosForTime(float &x, float &y, float &z, uint32 time, uint32* lastnode, uint32 mapid);
	ASCENT_INLINE uint32 GetID() { return id; }
	void SendMoveForTime(Player *riding, Player *to, uint32 time);
	void AddPathNode(uint32 index, TaxiPathNode* pn) { m_pathNodes[index] = pn; }
	ASCENT_INLINE size_t GetNodeCount() { return m_pathNodes.size(); }
	TaxiPathNode* GetPathNode(uint32 i);
	ASCENT_INLINE uint32 GetPrice() { return price; }
	ASCENT_INLINE uint32 GetSourceNode() { return from; }

protected:

	std::map<uint32, TaxiPathNode*> m_pathNodes;

	float m_length1;
	uint32 m_map1;

	float m_length2;
	uint32 m_map2;
	uint32 id, to, from, price;
};


class SERVER_DECL TaxiMgr :  public Singleton < TaxiMgr >
{
public:
	TaxiMgr() 
	{
		_LoadTaxiNodes();
		_LoadTaxiPaths();
	}

	~TaxiMgr() 
	{ 
		while(m_taxiPaths.size())
		{
			TaxiPath *p = m_taxiPaths.begin()->second;
			m_taxiPaths.erase(m_taxiPaths.begin());
			delete p;
		}
		while(m_taxiNodes.size())
		{
			TaxiNode *n = m_taxiNodes.begin()->second;
			m_taxiNodes.erase(m_taxiNodes.begin());
			delete n;
		}
	}

	TaxiPath *GetTaxiPath(uint32 path);
	TaxiPath *GetTaxiPath(uint32 from, uint32 to);
	TaxiNode *GetTaxiNode(uint32 node);

	uint32 GetNearestTaxiNode( float x, float y, float z, uint32 mapid );
	bool GetGlobalTaxiNodeMask( uint32 curloc, uint32 *Mask );


private:
	void _LoadTaxiNodes();
	void _LoadTaxiPaths();

	HM_NAMESPACE::hash_map<uint32, TaxiNode*> m_taxiNodes;
	HM_NAMESPACE::hash_map<uint32, TaxiPath*> m_taxiPaths;
};

#define sTaxiMgr TaxiMgr::getSingleton()

#endif
