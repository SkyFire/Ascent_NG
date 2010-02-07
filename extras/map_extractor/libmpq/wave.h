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


#ifndef _WAVE_H
#define _WAVE_H

typedef union {
	unsigned short	*pw;
	unsigned char	*pb;
} byte_and_short;

int libmpq_wave_decompress(unsigned char *out_buf, int out_length, unsigned char *in_buf, int in_length, int channels);

#endif					/* _WAVE_H */
