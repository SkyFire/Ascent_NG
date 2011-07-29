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

#ifndef _AUTOPATCHER_H
#define _AUTOPATCHER_H

struct Patch
{
	uint32 FileSize;
	uint8 MD5[16];
	uint8 * Data;
	uint32 Version;
	char Locality[5];
	uint32 uLocality;
};

class PatchJob
{
	Patch * m_patchToSend;
	AuthSocket * m_client;
	uint32 m_bytesSent;
	uint32 m_bytesLeft;
	uint8 * m_dataPointer;

public:
	PatchJob(Patch * patch,AuthSocket* client,uint32 skip) : m_patchToSend(patch),m_client(client),m_bytesSent(skip),m_bytesLeft(patch->FileSize-skip),m_dataPointer(patch->Data+skip) {}
	ASCENT_INLINE AuthSocket * GetClient() { return m_client; }
	bool Update();
};

class PatchMgr : public Singleton<PatchMgr>
{
public:
	PatchMgr();
	~PatchMgr();

	Patch * FindPatchForClient(uint32 Version, const char * Locality);
	void BeginPatchJob(Patch * pPatch, AuthSocket * pClient, uint32 Skip);
	void UpdateJobs();
	void AbortPatchJob(PatchJob * pJob);
	bool InitiatePatch(Patch * pPatch, AuthSocket * pClient);

protected:
	vector<Patch*> m_patches;

	Mutex m_patchJobLock;
	list<PatchJob*> m_patchJobs;
};

#endif
