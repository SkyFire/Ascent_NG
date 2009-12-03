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

#include "WowCrypt.h"
#include <algorithm>

WowCrypt::WowCrypt()
{
	_initialized = false;
}

void WowCrypt::Init(uint8 *K)
{
	const uint8 SeedKeyLen = 16;
    uint8 ServerEncryptionKey[SeedKeyLen] = { 0x22, 0xBE, 0xE5, 0xCF, 0xBB, 0x07, 0x64, 0xD9, 0x00, 0x45, 0x1B, 0xD0, 0x24, 0xB8, 0xD5, 0x45 };

	HMACHash auth;
	auth.Initialize(SeedKeyLen, (uint8*)ServerEncryptionKey);
	auth.UpdateData(K, 40);
	auth.Finalize();
	uint8 *encryptHash = auth.GetDigest();

    uint8 ServerDecryptionKey[SeedKeyLen] = { 0xF4, 0x66, 0x31, 0x59, 0xFC, 0x83, 0x6E, 0x31, 0x31, 0x02, 0x51, 0xD5, 0x44, 0x31, 0x67, 0x98 };
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
