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

#if !defined(FIELD_H)
#define FIELD_H

class Field
{
public:

	ASCENT_INLINE void SetValue(char* value) { mValue = value; }

	ASCENT_INLINE const char *GetString() { return mValue; }
	ASCENT_INLINE float GetFloat() { return mValue ? static_cast<float>(atof(mValue)) : 0; }
	ASCENT_INLINE bool GetBool() { return mValue ? atoi(mValue) > 0 : false; }
	ASCENT_INLINE uint8 GetUInt8() { return mValue ? static_cast<uint8>(atol(mValue)) : 0; }
	ASCENT_INLINE int8 GetInt8() { return mValue ? static_cast<int8>(atol(mValue)) : 0; }
	ASCENT_INLINE uint16 GetUInt16() { return mValue ? static_cast<uint16>(atol(mValue)) : 0; }
	ASCENT_INLINE int16 GetInt16() { return mValue ? static_cast<int16>(atol(mValue)) : 0; }
	ASCENT_INLINE uint32 GetUInt32() { return mValue ? static_cast<uint32>(atol(mValue)) : 0; }
	ASCENT_INLINE int32 GetInt32() { return mValue ? static_cast<int32>(atol(mValue)) : 0; }
	uint64 GetUInt64() 
	{
		if(mValue)
		{
			uint64 value;
#if !defined(WIN32) && defined(X64)
			sscanf(mValue,I64FMTD,(long long unsigned int*)&value);
#else
			sscanf(mValue,I64FMTD,&value);
#endif
			return value;
		}
		else
			return 0;
	}

private:
		char *mValue;
};

#endif
