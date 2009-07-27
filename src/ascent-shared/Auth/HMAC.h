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
