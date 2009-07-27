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

#ifndef _COLLIDEINTERFACE_H
#define _COLLIDEINTERFACE_H

/* imports */
#define NO_WMO_HEIGHT -100000.0f

extern VMAP::IVMapManager* CollisionMgr;

class SERVER_DECL CCollideInterface
{
public:
	void Init();
	void DeInit();

	void ActivateTile(uint32 mapId, uint32 tileX, uint32 tileY);
	void DeactivateTile(uint32 mapId, uint32 tileX, uint32 tileY);
	void ActivateMap(uint32 mapId);
	void DeactivateMap(uint32 mapId);

	bool CheckLOS(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2);
	bool GetFirstPoint(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2, float & outx, float & outy, float & outz, float distmod);
	bool IsIndoor(uint32 mapId, float x, float y, float z);
	bool IsOutdoor(uint32 mapId, float x, float y, float z);
	float GetHeight(uint32 mapId, float x, float y, float z);
};

extern SERVER_DECL CCollideInterface CollideInterface;

#endif
