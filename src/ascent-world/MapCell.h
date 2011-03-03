/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */


#ifndef __MAP_CELL_H
#define __MAP_CELL_H

class Map;

#define MAKE_CELL_EVENT(x,y) ( ((x) * 1000) + 200 + y )
#define DECODE_CELL_EVENT(dest_x, dest_y, ev) (dest_x) = ((ev-200)/1000); (dest_y) = ((ev-200)%1000);

class SERVER_DECL MapCell
{
	friend class MapMgr;
public:
	MapCell();
	~MapCell();

	typedef unordered_set<Object* > ObjectSet;

	//Init
	void Init(uint32 x, uint32 y, uint32 mapid, MapMgr* mapmgr);

	//Object Managing
	void AddObject(Object* obj); 
	void RemoveObject(Object* obj);
	bool HasObject(Object* obj) { return (_objects.find(obj) != _objects.end()); }
	bool HasPlayers() { return ((_playerCount > 0) ? true : false); }
	ASCENT_INLINE size_t GetObjectCount() { return _objects.size(); }
	void RemoveObjects();
	ASCENT_INLINE ObjectSet::iterator Begin() { return _objects.begin(); }
	ASCENT_INLINE ObjectSet::iterator End() { return _objects.end(); }

	//State Related
	void SetActivity(bool state);

	ASCENT_INLINE bool IsActive() { return _active; }
	ASCENT_INLINE bool IsLoaded() { return _loaded; }

	//Object Loading Managing
	void LoadObjects(CellSpawns * sp);
	ASCENT_INLINE uint32 GetPlayerCount() { return _playerCount; }

	//ING Events
	void RemoveEventIdObjects(uint8 eventToRemove);
	void ModifyEventIdSetting(bool active, uint8 eventId);
	void LoadEventIdObjects(CellSpawns * sp, uint8 eventId);

	ASCENT_INLINE bool IsUnloadPending() { return _unloadpending; }
	ASCENT_INLINE void SetUnloadPending(bool up) { _unloadpending = up; }
	void QueueUnloadPending();
	void CancelPendingUnload();
	void Unload();

	void SetPermanentActivity(bool val) { _forcedActive = val; }
	bool IsForcedActive() { return _forcedActive; }

	uint16 GetPositionX() { return _x; }
	uint16 GetPositionY() { return _y; }

	ObjectSet _respawnObjects;

private:
	bool _forcedActive;
	uint16 _x,_y;
	ObjectSet _objects;
	bool _active, _loaded;
	bool _unloadpending;

	uint16 _playerCount;
	MapMgr* _mapmgr;
};

#endif
