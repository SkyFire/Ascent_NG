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

#ifndef _AUTH_SHA1_H
#define _AUTH_SHA1_H

#include <stdlib.h>
#include "Common.h"
#include <openssl/hmac.h>
#include "Auth/BigNumber.h"

class HMACHash
{
	public:
		HMACHash();
		~HMACHash();

		void UpdateFinalizeBigNumbers(BigNumber *bn0, ...);
		void UpdateBigNumber(BigNumber *bn0, ...);

		void UpdateData(const uint8 *dta, int len);
		void UpdateData(const std::string &str);

		void Initialize(uint32 len, uint8 *seed);
		void Finalize();

		uint8 *GetDigest(void) { return mDigest; };
		int GetLength(void) { return 20; };

		BigNumber GetBigNumber();

	private:
		HMAC_CTX mC;
		uint8 mDigest[20];
};

#endif
