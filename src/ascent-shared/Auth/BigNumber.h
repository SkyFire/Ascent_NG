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

#ifndef _AUTH_BIGNUMBER_H
#define _AUTH_BIGNUMBER_H

#include "Common.h"
#include "ByteBuffer.h"

//#include "openssl/bn.h"
struct bignum_st;

class BigNumber
{
	public:
		BigNumber();
		BigNumber(const BigNumber &bn);
		BigNumber(uint32);
		~BigNumber();

		void SetDword(uint32);
		void SetQword(uint64);
		void SetBinary(const uint8 *bytes, int len);
		void SetHexStr(const char *str);

		void SetRand(int numbits);

		BigNumber operator=(const BigNumber &bn);
//	  BigNumber operator=(Sha1Hash &hash);

		BigNumber operator+=(const BigNumber &bn);
		BigNumber operator+(const BigNumber &bn) {
			BigNumber t(*this);
			return t += bn;
		}
		BigNumber operator-=(const BigNumber &bn);
		BigNumber operator-(const BigNumber &bn) {
			BigNumber t(*this);
			return t -= bn;
		}
		BigNumber operator*=(const BigNumber &bn);
		BigNumber operator*(const BigNumber &bn) {
			BigNumber t(*this);
			return t *= bn;
		}
		BigNumber operator/=(const BigNumber &bn);
		BigNumber operator/(const BigNumber &bn) {
			BigNumber t(*this);
			return t /= bn;
		}
		BigNumber operator%=(const BigNumber &bn);
		BigNumber operator%(const BigNumber &bn) {
			BigNumber t(*this);
			return t %= bn;
		}

		BigNumber ModExp(const BigNumber &bn1, const BigNumber &bn2);
		BigNumber Exp(const BigNumber &);

		int GetNumBytes(void);

		struct bignum_st *BN() { return _bn; }

		uint32 AsDword();
		uint8* AsByteArray();
		ByteBuffer AsByteBuffer();
		std::vector<uint8> AsByteVector();

		const char *AsHexStr();
		const char *AsDecStr();

	private:
		struct bignum_st *_bn;
		uint8 *_array;
};

#endif
