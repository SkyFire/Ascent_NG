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

#include "HMAC.h"
#include <stdarg.h>

HMACHash::HMACHash()
{
}

void HMACHash::Initialize(uint32 len, uint8 *seed)
{
	HMAC_CTX_init(&mC);
	HMAC_Init_ex(&mC, seed, len, EVP_sha1(), NULL);
}

HMACHash::~HMACHash()
{
    HMAC_CTX_cleanup(&mC);
}

void HMACHash::UpdateBigNumber(BigNumber *bn0, ...)
{
	va_list v;
	BigNumber *bn;

	va_start(v, bn0);
	bn = bn0;
	while (bn)
	{
		UpdateData(bn->AsByteArray(), bn->GetNumBytes());
		bn = va_arg(v, BigNumber *);
	}
	va_end(v);
}

void HMACHash::UpdateData(const uint8 *data, int length)
{
    HMAC_Update(&mC, data, length);
}

void HMACHash::UpdateData(const std::string &str)
{
	UpdateData((uint8 *)str.c_str(), (int)str.length());
}

void HMACHash::Finalize()
{
    uint32 length = 0;
    HMAC_Final(&mC, (uint8*)mDigest, &length);
}
