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

#ifndef _PREALLOCATED_QUEUE_H
#define _PREALLOCATED_QUEUE_H

template<typename Type>
class PreallocatedQueue
{
public:
	PreallocatedQueue(uint32 size, uint32 realloc_size = 100)
	{
		// create buffer
		Buffer = (uint8*)malloc(size * sizeof(Type));
		m_readpos = m_writepos = 0;
		m_buffersize = size * sizeof(Type);
		type_size = sizeof(Type);
		m_reallocsize = realloc_size;
	}

	~PreallocatedQueue()
	{
		free(Buffer);
	}

	void push_back(Type ptr)
	{
		if((type_size + m_writepos) > m_buffersize)
			reallocate(m_buffersize + (m_reallocsize * type_size));

		memcpy(&Buffer[m_writepos], &ptr, type_size);
		m_writepos += type_size;
	}

	Type pop_front()
	{
		assert((m_readpos + type_size) <= m_writepos);
		Type returner;
		memcpy(&returner, &Buffer[m_readpos], type_size);
		m_readpos += type_size;

		// clear buffer completely if we're at the end of the buffer
		if(m_readpos == m_writepos)
			clear();

		return returner;
	}

	void clear()
	{
		m_readpos = m_writepos = 0;
	}

	bool empty()
	{
		if(m_readpos == m_writepos)
			return true;
		return false;
	}

	void reallocate(uint32 size)
	{
		Buffer = (uint8*)realloc(Buffer, size);
		assert(Buffer);
		m_buffersize = size;
	}

protected:

	uint8 * Buffer;

	uint32 m_readpos;
	uint32 m_writepos;
	uint32 m_writesize;
	uint32 m_buffersize;
	uint32 type_size;
	uint32 m_reallocsize;
};

#endif