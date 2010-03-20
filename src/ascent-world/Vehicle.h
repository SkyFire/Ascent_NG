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

#ifndef _VEHICLE_H
#define _VEHICLE_H

class SERVER_DECL Vehicle : public Creature
{
public:
	Vehicle(uint64 guid);
	~Vehicle();
	virtual void Destructor();

	void Init();
	void InitSeats(uint32 vehicleEntry, Player* pRider = NULL);
	virtual void Update(uint32 time);
	bool Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info);
	void Load(CreatureProto * proto_, float x, float y, float z, float o = 0.0f);
	void Despawn(uint32 delay, uint32 respawntime);
	void DeleteMe();
	void SafeDelete();
	void MoveVehicle(float x, float y, float z, float o);
	void AddPassenger(Unit* pPassenger);
	void RemovePassenger(Unit* pPassenger);
	bool HasPassenger(Unit* pPassenger);
	void SendSpells(uint32 entry, Player* plr);
	void setDeathState(DeathState s);
	void SetSpeed(uint8 SpeedType, float value);

	//---------------------------------------
	// Accessors
	//---------------------------------------
	uint32 getMaxPassengerCount() { return maxPassengers; }
	uint32 getPassengerCount() { return passengerCount; }

	void setPassengerCount(uint32 newCount) { passengerCount = newCount; };
	void setMaxPassengers(uint32 newMax) { maxPassengers = newMax; };

	uint32 getVehicleEntry() { return vehicleEntry; }
	void setVehicleEntry(uint32 entry) { vehicleEntry = entry; }

	Unit* getControllingUnit() { return controllingUnit; }
	void setControllingUnit(Unit* pUnit) { controllingUnit = pUnit; }

	uint8 getPassengerSlot(Unit* pPassenger);
	
	VehicleSeatEntry* getVehicleSeat(uint8 slotId);

	uint32 getMountSpell() { return mountSpell; }; 
	void setMountSpell(uint32 newSpell) { mountSpell = newSpell; };

	void setInitializedStatus(bool newStatus) { Initialised = newStatus; };
	void setCreatedFromSpell(bool newStatus) { createdFromSpell = newStatus; };

	bool isFull() { return passengerCount == maxPassengers; }
	bool isInitialized() { return Initialised; };
	bool isCreatedFromSpell() { return createdFromSpell; };
	//---------------------------------------
	// End accessors
	//---------------------------------------

private:
	void AddToSlot(Unit* pPassenger, uint8 slot);
	
	bool Initialised;
	bool createdFromSpell;

	Unit* controllingUnit;

	vector<VehicleSeatEntry*> vehicleSeats;
	vector<Unit*> passengers;

	uint8 passengerCount;
	uint8 maxPassengers;
	uint32 vehicleEntry;

	uint32 mountSpell;
	
};

#endif
