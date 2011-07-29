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

#include "Sha1.h"
#include <stdarg.h>

Sha1Hash::Sha1Hash()
{
	SHA1_Init(&mC);
}

Sha1Hash::~Sha1Hash()
{
	// nothing.. lol
}

void Sha1Hash::UpdateData(const uint8 *dta, int len)
{
	SHA1_Update(&mC, dta, len);
}

void Sha1Hash::UpdateData(const std::string &str)
{
	UpdateData((uint8 *)str.c_str(), (int)str.length());
}

void Sha1Hash::UpdateBigNumbers(BigNumber *bn0, ...)
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

void Sha1Hash::Initialize()
{
	SHA1_Init(&mC);
}

void Sha1Hash::Finalize(void)
{
	SHA1_Final(mDigest, &mC);
}
