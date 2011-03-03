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

class WServer;
class Session;
class WSSocket : public Socket
{
	bool _authenticated;
	uint32 _remaining;
	uint16 _cmd;
	WServer * _ws;
public:
	uint32 m_id;

	WSSocket(SOCKET fd);
	~WSSocket();

	void SendPacket(WorldPacket * pck);
	void SendWoWPacket(Session * from, WorldPacket * pck);
	void OnRead();

	void HandleAuthRequest(WorldPacket & pck);
	void HandleRegisterWorker(WorldPacket & pck);
	void OnConnect();
	void OnDisconnect();

};


