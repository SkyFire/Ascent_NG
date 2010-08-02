/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
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

void WowCrypt::Init()
{
	_send_i = _send_j = _recv_i = _recv_j = 0;
	_initialized = true;
}

void WowCrypt::DecryptRecv(uint8 *data, size_t len)
{
	if (!_initialized) return;
	if (len < CRYPTED_RECV_LEN) return;
	uint8 x;

	for (size_t t = 0; t < CRYPTED_RECV_LEN; t++) {
		_recv_i %= _key.size();
		x = (data[t] - _recv_j) ^ _key[_recv_i];
		++_recv_i;
		_recv_j = data[t];
		data[t] = x;
	}
}

void WowCrypt::DecryptSixRecv(uint8 *data)
{
    if (!_initialized) return;
    
    uint8 x, KeySize = (uint8)_key.size();;

    // 0
    _recv_i %= KeySize;
    x = (data[0] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[0];
    data[0] = x;

    // 1
    _recv_i %= KeySize;
    x = (data[1] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[1];
    data[1] = x;

    // 2
    _recv_i %= KeySize;
    x = (data[2] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[2];
    data[2] = x;

    // 3
    _recv_i %= KeySize;
    x = (data[3] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[3];
    data[3] = x;

    // 4
    _recv_i %= KeySize;
    x = (data[4] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[4];
    data[4] = x;

    // 5
    _recv_i %= KeySize;
    x = (data[5] - _recv_j) ^ _key[_recv_i];
    ++_recv_i;
    _recv_j = data[5];
    data[5] = x;
}

void WowCrypt::EncryptSend(uint8 *data, size_t len)
{
	if (!_initialized) return;
	if (len < CRYPTED_SEND_LEN) return;

	for (size_t t = 0; t < CRYPTED_SEND_LEN; t++) {
		_send_i %= _key.size();
		data[t] = _send_j = (data[t] ^ _key[_send_i]) + _send_j;
        ++_send_i;
	}
}

void WowCrypt::EncryptFourSend(uint8 * data)
{
    if (!_initialized) return;

    uint8 KeySize = (uint8)_key.size();

    _send_i %= KeySize;
    data[0] = _send_j = (data[0] ^ _key[_send_i]) + _send_j;
    ++_send_i;

    _send_i %= KeySize;
    data[1] = _send_j = (data[1] ^ _key[_send_i]) + _send_j;
    ++_send_i;

    _send_i %= KeySize;
    data[2] = _send_j = (data[2] ^ _key[_send_i]) + _send_j;
    ++_send_i;

    _send_i %= KeySize;
    data[3] = _send_j = (data[3] ^ _key[_send_i]) + _send_j;
    ++_send_i;
}

void WowCrypt::SetKey(uint8 *key, size_t len)
{
	_key.resize(len);
	std::copy(key, key + len, _key.begin());
}

WowCrypt::~WowCrypt()
{
}
