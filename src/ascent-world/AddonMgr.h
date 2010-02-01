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

#ifndef __ADDONMGR_H
#define __ADDONMGR_H

struct AddonEntry
{
	std::string name;
	uint32 crc;
	bool banned;
	bool isNew;
	bool showinlist;
};

class AddonMgr :  public Singleton < AddonMgr >
{
public:
	AddonMgr();
	~AddonMgr();

	void LoadFromDB();
	void SaveToDB();

	void SendAddonInfoPacket(WorldPacket *source, uint32 pos, WorldSession *m_session);
	bool AppendPublicKey(WorldPacket& data, string AddonName, uint32 CRC);

private:
	std::map<std::string, AddonEntry*> KnownAddons;
	map<string, ByteBuffer> AddonData;

	bool IsAddonBanned(uint32 crc, std::string name = "");
	bool IsAddonBanned(std::string name, uint32 crc = 0);
	bool ShouldShowInList(std::string name, uint32 crc);
};

#define sAddonMgr AddonMgr::getSingleton()

#endif
