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
