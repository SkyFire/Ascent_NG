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

#include "WowCrypt.h"
#include <algorithm>

WowCrypt::WowCrypt()
{
	_initialized = false;
}

void WowCrypt::Init(uint8 *K)
{
	const uint8 SeedKeyLen = 16;
    uint8 ServerEncryptionKey[SeedKeyLen] = { 0xCC, 0x98, 0xAE, 0x04, 0xE8, 0x97, 0xEA, 0xCA, 0x12, 0xDD, 0xC0, 0x93, 0x42, 0x91, 0x53, 0x57 };

	HMACHash auth;
	auth.Initialize(SeedKeyLen, (uint8*)ServerEncryptionKey);
	auth.UpdateData(K, 40);
	auth.Finalize();
	uint8 *encryptHash = auth.GetDigest();

    uint8 ServerDecryptionKey[SeedKeyLen] = { 0xC2, 0xB3, 0x72, 0x3C, 0xC6, 0xAE, 0xD9, 0xB5, 0x34, 0x3C, 0x53, 0xEE, 0x2F, 0x43, 0x67, 0xCE };
	HMACHash auth2;
	auth2.Initialize(SeedKeyLen, (uint8*)ServerDecryptionKey);
	auth2.UpdateData(K, 40);
	auth2.Finalize();
	uint8 *decryptHash = auth2.GetDigest();

    _Decrypt.Setup((uint8*)decryptHash, 20);
    _Encrypt.Setup((uint8*)encryptHash, 20);

	uint8 encryptRotateBuffer[1024];
    memset(encryptRotateBuffer, 0, 1024);
	_Encrypt.Process((uint8*)encryptRotateBuffer, (uint8*)encryptRotateBuffer, 1024);

	uint8 decryptRotateBuffer[1024];
	memset(decryptRotateBuffer, 0, 1024);
	_Decrypt.Process((uint8*)decryptRotateBuffer, (uint8*)decryptRotateBuffer, 1024);

    _initialized = true;
}

void WowCrypt::DecryptRecv(uint8 *data, unsigned int len)
{
    if (!_initialized)
        return;

    _Decrypt.Process((uint8*)data, (uint8*)data, len);
}

void WowCrypt::EncryptSend(uint8 *data, unsigned int len)
{
    if (!_initialized)
        return;

    _Encrypt.Process((uint8*)data, (uint8*)data, len);
}

WowCrypt::~WowCrypt()
{
}
