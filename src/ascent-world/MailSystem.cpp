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

#include "StdAfx.h"
initialiseSingleton(MailSystem);

bool MailMessage::LoadFromDB(Field * fields)
{
//	Field * fields;
	uint32 i;
	char * str;
	char * p;
	uint64 itemguid;

	// Create message struct
	i = 0;
	items.clear();
	message_id = fields[i++].GetUInt32();
	message_type = fields[i++].GetUInt32();
	player_guid = fields[i++].GetUInt32();
	sender_guid = fields[i++].GetUInt32();
	subject = fields[i++].GetString();
	body = fields[i++].GetString();
	money = fields[i++].GetUInt32();
	str = (char*)fields[i++].GetString();
	p = strchr(str, ',');
	if( p == NULL )
	{
		itemguid = atoi(str);
		if( itemguid != 0 )
			items.push_back( itemguid );
	}
	else
	{
		while( p )
		{
			*p = 0;
			p++;
			itemguid = atol( str );
			if( itemguid != 0 )
				items.push_back( itemguid );
			str = p;
			p = strchr( str, ',' );
		}
	}

	cod = fields[i++].GetUInt32();
	stationary = fields[i++].GetUInt32();
	expire_time = fields[i++].GetUInt32();
	delivery_time = fields[i++].GetUInt32();
	copy_made = fields[i++].GetBool();
	read_flag = fields[i++].GetBool();
	deleted_flag = fields[i++].GetBool();
	returned_flag = fields[i++].GetBool();

/*
		if( msg.deleted_flag )
		{
			CharacterDatabase.WaitExecute( "DELETE FROM playeritems WHERE itemtext = %u", msg.message_id );
			CharacterDatabase.WaitExecute( "DELETE FROM mailbox WHERE message_id = %u", msg.message_id );
		}else
*/

	return (!deleted_flag || copy_made);
}

void MailMessage::SaveToDB()
{
	stringstream ss;
	vector< uint64 >::iterator itr;
	ss << "REPLACE INTO mailbox VALUES("
		<< message_id << ","
		<< message_type << ","
		<< player_guid << ","
		<< sender_guid << ",\""
		<< CharacterDatabase.EscapeString(subject) << "\",\""
		<< CharacterDatabase.EscapeString(body) << "\","
		<< money << ",'";

	for( itr = items.begin( ); itr != items.end( ); ++itr )
		ss << (*itr) << ",";

	ss << "'," 
		<< cod << ","
		<< stationary << ","
		<< expire_time << ","
		<< delivery_time << ","
		<< copy_made << ","
		<< read_flag << ","
		<< deleted_flag << ","
		<< returned_flag << ")";
	if(message_id == 0)
	{
		AsyncQuery * q = new AsyncQuery( new SQLClassCallbackP0<MailMessage>(this, &MailMessage::SaveToDBCallBack) );
		q->AddQuery(ss.str().c_str());
		q->AddQuery("SELECT LAST_INSERT_ID()");
		CharacterDatabase.QueueAsyncQuery(q);
	} else
	{
		CharacterDatabase.WaitExecute(ss.str().c_str());
	}
}

void MailMessage::SaveToDBCallBack(QueryResultVector & results)
{
	QueryResult *result = results[1].result;
	if(result)
	{
		Field *fields = result->Fetch();
		message_id = fields[0].GetUInt32();
	}
}

bool MailMessage::Expired()
{
	return (expire_time && expire_time < (uint32)UNIXTIME);
}
void Mailbox::AddMessage(MailMessage* Message)
{
	Messages[Message->message_id] = *Message;
}

void Mailbox::DeleteMessage(MailMessage* Message)
{
	if(Message->copy_made)
	{
		// we have the message as a copy on the item. we can't delete it or this item will no longer function.
		// deleted_flag prevents it from being shown in the mail list.
		Message->deleted_flag = true;
		Message->SaveToDB();
	}
	else
	{
		// delete the message, there are no other references to it.
		CharacterDatabase.WaitExecute("DELETE FROM mailbox WHERE message_id = %u", Message->message_id);
		Messages.erase(Message->message_id);
	}
}

void Mailbox::Load(QueryResult * result)
{
	if(!result)
		return;

	MailMessage msg;
	do 
	{
		if (msg.LoadFromDB(result->Fetch()))
		{
			AddMessage(&msg);// Add to the mailbox
		}
	} while(result->NextRow());
}

WorldPacket * Mailbox::MailboxListingPacket()
{
	WorldPacket * data = new WorldPacket(SMSG_MAIL_LIST_RESULT, 500);
	MessageMap::iterator itr;
	uint32 realcount = 0;
	uint32 count = 0;
	uint32 t = (uint32)UNIXTIME;
	*data << uint32(0);	 // realcount - this can be used to tell the client we have more mail than that fits into this packet
	*data << uint8(0);	 // size placeholder

	for(itr = Messages.begin(); itr != Messages.end(); ++itr)
	{

		if(count >= 50)
		{
			++realcount;
			continue;
		}

		if(AddMessageToListingPacket(*data, &itr->second))
		{
			++count;
			++realcount;
		}
	}

	data->put<uint32>(0, realcount); 
	data->put<uint8>(4, count); 

	// do cleanup on request mail
	//CleanupExpiredMessages();
	return data;
}

bool Mailbox::AddMessageToListingPacket(WorldPacket& data,MailMessage *msg)
{
	uint8 i = 0;
	uint32 j;
	size_t pos;
	vector<uint64>::iterator itr;
	Item* pItem;

	// add stuff
	if(msg->deleted_flag || msg->Expired() || (uint32)UNIXTIME < msg->delivery_time)
		return false;

	data << uint16(0x0032);
	data << msg->message_id;
	data << uint8(msg->message_type);
	if(msg->message_type)
		data << uint32(msg->sender_guid);
	else
		data << msg->sender_guid;

	data << msg->cod;
	data << uint32(0);
	data << msg->stationary;
	data << msg->money; // money
	data << uint32(0);
	data << uint32(0x10);
	data << uint32((msg->expire_time - uint32(UNIXTIME)) / 86400.0f);
	data << msg->subject;
	data << msg->body; // subjectbody
	pos = data.wpos();
	data << uint8(0);		// item count

	if( !msg->items.empty( ) )
	{
		for( itr = msg->items.begin( ); itr != msg->items.end( ); ++itr )
		{
			pItem = objmgr.LoadItem( *itr );
			if( pItem == NULL )
				continue;

			data << uint8(i++);
			data << pItem->GetUInt32Value(OBJECT_FIELD_GUID);
			data << pItem->GetEntry();

			for( j = 0; j < 7; ++j )
			{
				data << pItem->GetUInt32Value( ITEM_FIELD_ENCHANTMENT_1_1 + ( j * 3 ) );
				data << pItem->GetUInt32Value( (ITEM_FIELD_ENCHANTMENT_1_1 + 1) + ( j * 3 ) );
				data << pItem->GetUInt32Value( ITEM_FIELD_ENCHANTMENT_1_3 + ( j * 3 ) );
			}

			data << pItem->GetUInt32Value( ITEM_FIELD_RANDOM_PROPERTIES_ID );
			if( ( (int32)pItem->GetUInt32Value( ITEM_FIELD_RANDOM_PROPERTIES_ID ) ) < 0 )
				data << pItem->GetItemRandomSuffixFactor();
			else
				data << uint32( 0 );

			data << uint8( pItem->GetUInt32Value(ITEM_FIELD_STACK_COUNT) );
			data << uint32( pItem->GetChargesLeft() );
			data << pItem->GetUInt32Value( ITEM_FIELD_MAXDURABILITY );
			data << pItem->GetUInt32Value( ITEM_FIELD_DURABILITY );
			data << uint32(0);
			delete pItem;
		}
		data.put< uint8 >( pos, i );
	}

	return true;
}

WorldPacket * Mailbox::MailboxTimePacket()
{

	WorldPacket * data = new WorldPacket(MSG_QUERY_NEXT_MAIL_TIME, 100);
	uint32 count = 0;
	MessageMap::iterator iter;


	*data << uint32(0) << uint32(0);

	for( iter = Messages.begin(); iter != Messages.end(); ++iter )
	{
		if(AddMessageToTimePacket(* data, &iter->second))
		{
			++count;
		}
	}

	if(count==0)
	{
		data->put(0, uint32(0xc7a8c000));
//		*(uint32*)(data->contents()[0])=0xc7a8c000;
	}
	else
	{
		data->put(4, uint32(count));
//		*(uint32*)(data->contents()[0])=uint32(0);
//		*(uint32*)(data->contents()[4])=count;
	}
	return data;
}

bool Mailbox::AddMessageToTimePacket(WorldPacket& data,MailMessage *msg)
{
	if ( msg->deleted_flag == 1 || msg->read_flag == 1  || msg->Expired() || (uint32)UNIXTIME < msg->delivery_time )
		return false;
	// unread message, w00t.
	data << uint64(msg->sender_guid);
	data << uint32(0);
	data << uint32(0);// money or smth?
	data << uint32(msg->stationary);
	//data << float(UNIXTIME-msg->delivery_time);
	data << float(-9.0f);	// maybe the above?

	return true;
}

void Mailbox::OnMessageCopyDeleted(uint32 msg_id)
{
	MailMessage * msg = GetMessage(msg_id);
	if(msg == 0) return;

	msg->copy_made = false;

	if(msg->deleted_flag)   // we've deleted from inbox
		DeleteMessage(msg);   // wipe the message
	else
		msg->SaveToDB();
}
void MailSystem::StartMailSystem()
{

}

void MailSystem::DeliverMessage(MailMessage* message)
{
	message->SaveToDB();

	Player* plr = objmgr.GetPlayer((uint32)message->player_guid);
	if(plr != NULL)
	{
		plr->m_mailBox->AddMessage(message);
		if((uint32)UNIXTIME >= message->delivery_time)
		{
			uint32 v = 0;
			plr->GetSession()->OutPacket(SMSG_RECEIVED_MAIL, 4, &v);
		}
	}
}

void MailSystem::ReturnToSender(MailMessage* message)
{
	MailMessage msg = *message;

	// re-assign the owner/sender
	msg.player_guid = message->sender_guid;
	msg.sender_guid = message->player_guid;

	// remove the old message
	Player* plr = objmgr.GetPlayer((uint32)message->player_guid);
	if(plr != NULL)
	{
		plr->m_mailBox->DeleteMessage(message);
	} else
	{
		if(message->copy_made)
		{
			message->deleted_flag = true;
			message->SaveToDB();
		}
		else
		{
			CharacterDatabase.WaitExecute("DELETE FROM mailbox WHERE message_id = %u", message->message_id);
		}
	}

	// return mail
	msg.message_id = 0;
	msg.read_flag = false;
	msg.deleted_flag = false;
	msg.copy_made = false;
	msg.returned_flag = true;
	msg.cod = 0;
	msg.delivery_time = msg.items.empty() ? (uint32)UNIXTIME : (uint32)UNIXTIME + 3600;
	// returned mail's don't expire
	msg.expire_time = 0;
	// add to the senders mailbox
	sMailSystem.DeliverMessage(&msg);
}

void MailSystem::DeliverMessage(uint32 type, uint64 sender, uint64 receiver, string subject, string body,
									  uint32 money, uint32 cod, uint64 item_guid, uint32 stationary, bool returned)
{
	// This is for sending automated messages, for example from an auction house.
	MailMessage msg;
	msg.message_id = 0;
	msg.message_type = type;
	msg.sender_guid = sender;
	msg.player_guid = receiver;
	msg.subject = subject;
	msg.body = body;
	msg.money = money;
	msg.cod = cod;
	if( GUID_LOPART(item_guid) != 0 )
		msg.items.push_back( GUID_LOPART(item_guid) );

	msg.stationary = stationary;
	msg.delivery_time = (uint32)UNIXTIME;
	msg.expire_time = 0;
	msg.read_flag = false;
	msg.copy_made = false;
	msg.deleted_flag = false;
	msg.returned_flag = returned;

	// Send the message.
	DeliverMessage(&msg);
}

void MailSystem::UpdateMessages()
{
	if((++loopcount % 1200))
		return;

	QueryResult *result = CharacterDatabase.Query("SELECT * FROM mailbox WHERE expiry_time > 0 and expiry_time <= %u and deleted_flag = 0",(uint32)UNIXTIME);

	if(!result)
		return;

	MailMessage msg;
	do 
	{
		if (msg.LoadFromDB(result->Fetch()))
		{
			if (/*msg.Expired() &&*/ msg.items.size() == 0 && msg.money == 0)
			{
				if(msg.copy_made)
				{
					msg.deleted_flag = true;
					msg.SaveToDB();
				} else
				{
					CharacterDatabase.WaitExecute("DELETE FROM mailbox WHERE message_id = %u", msg.message_id);
				}
			} else
			{
				ReturnToSender(&msg);
			}

		}
	} while(result->NextRow());
	delete result;
}

//delete
void MailSystem::SendAutomatedMessage(uint32 type, uint64 sender, uint64 receiver, string subject, string body,
									  uint32 money, uint32 cod, uint64 item_guid, uint32 stationary)
{
	// This is for sending automated messages, for example from an auction house.
	MailMessage msg;
	msg.message_type = type;
	msg.sender_guid = sender;
	msg.player_guid = receiver;
	msg.subject = subject;
	msg.body = body;
	msg.money = money;
	msg.cod = cod;
	if( GUID_LOPART(item_guid) != 0 )
		msg.items.push_back( GUID_LOPART(item_guid) );

	msg.stationary = stationary;
	msg.delivery_time = (uint32)UNIXTIME;
	msg.expire_time = 0;
	msg.read_flag = false;
	msg.copy_made = false;
	msg.deleted_flag = false;

	// Send the message.
	DeliverMessage(&msg);
}
//delete

void WorldSession::HandleSendMail(WorldPacket & recv_data )
{
	MailMessage msg;
	uint64 gameobject;
	uint32 unk2;
	uint8 itemcount;
	uint8 itemslot;
	uint8 i;
	uint64 itemguid;
	vector< Item* > items;
	vector< Item* >::iterator itr;
	string recepient;
	Item* pItem;
	int8 real_item_slot;

	recv_data >> gameobject >> recepient;
	// Search for the recipient
	PlayerInfo* player = ObjectMgr::getSingleton().GetPlayerInfoByName(recepient.c_str());
	if( player == NULL )
	{
		SendMailError(MAIL_ERR_RECIPIENT_NOT_FOUND);
		return;
	}
	msg.player_guid = player->guid;
	msg.sender_guid = _player->GetGUID();

	recv_data >> msg.subject >> msg.body >> msg.stationary;

	// Check attached items
	recv_data >> unk2 >> itemcount;
	for( i = 0; i < itemcount; ++i )
	{
		recv_data >> itemslot;
		recv_data >> itemguid;

        pItem = _player->GetItemInterface()->GetItemByGUID( itemguid );
		real_item_slot = _player->GetItemInterface()->GetInventorySlotByGuid( itemguid );
		if( pItem == NULL || pItem->IsSoulbound() || pItem->HasFlag( ITEM_FIELD_FLAGS, ITEM_FLAG_CONJURED ) || 
			( pItem->IsContainer() && (TO_CONTAINER( pItem ))->HasItems() ) || real_item_slot >= 0 && real_item_slot < INVENTORY_SLOT_ITEM_START )
		{
			SendMailError(MAIL_ERR_INTERNAL_ERROR);
			return;
		}
		items.push_back( pItem );
	}
	if( items.size() > 12 || msg.body.find("%") != string::npos || msg.subject.find("%") != string::npos)
	{
		SendMailError(MAIL_ERR_INTERNAL_ERROR);
		return;
	}

	recv_data >> msg.money;
	recv_data >> msg.cod;
	// left over: (TODO- FIX ME BURLEX!)
	// uint32
	// uint32
	// uint8

	// Check if we're sending mail to ourselves
	if(msg.player_guid == msg.sender_guid && !GetPermissionCount())
	{
		SendMailError(MAIL_ERR_CANNOT_SEND_TO_SELF);
		return;
	}

	// Check stationary
	if( msg.stationary == 0x3d && !HasGMPermissions())
	{
		SendMailError(MAIL_ERR_INTERNAL_ERROR);
		return;
	}

	// Set up the cost
	int32 cost = 0;
	if( !sMailSystem.MailOption( MAIL_FLAG_DISABLE_POSTAGE_COSTS ) && !( GetPermissionCount() && sMailSystem.MailOption( MAIL_FLAG_NO_COST_FOR_GM ) ) )
	{
		cost = 30;
	}
	// Check for attached money
	if( msg.money > 0 )
		cost += msg.money;
	if( cost < 0 )
	{
		SendMailError(MAIL_ERR_INTERNAL_ERROR);
		return;
	}
	// check that we have enough in our backpack
	if( (int32)_player->GetUInt32Value( PLAYER_FIELD_COINAGE ) < cost )
	{
		SendMailError( MAIL_ERR_NOT_ENOUGH_MONEY );
		return;
	}

	// Check we're sending to the same faction (disable this for testing)
	bool interfaction = (sMailSystem.MailOption( MAIL_FLAG_CAN_SEND_TO_OPPOSITE_FACTION ) || (HasGMPermissions() && sMailSystem.MailOption( MAIL_FLAG_CAN_SEND_TO_OPPOSITE_FACTION_GM ) ));
	if(!interfaction)
	{
		if(player->team != _player->GetTeam())
		{
			SendMailError( MAIL_ERR_NOT_YOUR_ALLIANCE );
			return;
		}
	}

	msg.message_id = 0;
	msg.message_type = 0;
	msg.copy_made = false;
	msg.read_flag = false;
	msg.deleted_flag = false;
	msg.returned_flag = false;
	msg.delivery_time = (uint32)UNIXTIME;

	if(msg.money != 0 || msg.cod != 0 || !items.size() && player->acct != _player->GetSession()->GetAccountId())
	{
		if(!sMailSystem.MailOption(MAIL_FLAG_DISABLE_HOUR_DELAY_FOR_ITEMS))
			msg.delivery_time += 3600;  // +1hr
	}

	msg.expire_time = 0;
	if(!sMailSystem.MailOption(MAIL_FLAG_NO_EXPIRY))
	{
		msg.expire_time = (uint32)UNIXTIME + (TIME_DAY * 30);
		if (msg.cod != 0)
		{
			msg.expire_time = (uint32)UNIXTIME + (TIME_DAY * 3);
		}
	}

	// Sending Message
	// take the money
	_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -cost);
	// Check for the item, and required item.
	if( !items.empty( ) )
	{
		for( itr = items.begin(); itr != items.end(); ++itr )
		{
			pItem = *itr;
			if( _player->GetItemInterface()->SafeRemoveAndRetreiveItemByGuid(pItem->GetGUID(), false) != pItem )
				continue;		// should never be hit.

			pItem->RemoveFromWorld();
			pItem->SetOwner( NULL );
			pItem->SaveToDB( INVENTORY_SLOT_NOT_SET, 0, true, NULL );
			msg.items.push_back( pItem->GetUInt32Value(OBJECT_FIELD_GUID) );
				
			if( GetPermissionCount() > 0 )
			{
				/* log the message */
				sGMLog.writefromsession(this, "sent mail with item entry %u to %s, with gold %u.", pItem->GetEntry(), player->name, msg.money);
			}

			delete pItem;
		}
	}

	// Great, all our info is filled in. Now we can add it to the other players mailbox.
	sMailSystem.DeliverMessage(&msg);

	// Success packet :)
	SendMailError(MAIL_OK);
}

void WorldSession::HandleMarkAsRead(WorldPacket & recv_data )
{
	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	MailMessage * message = _player->m_mailBox->GetMessage(message_id);
	if(message == 0 || message->Expired()) return;

	message->read_flag = true;
	if(!message->returned_flag)
	{
		// mail now has a 3 day expiry time
		if(!sMailSystem.MailOption(MAIL_FLAG_NO_EXPIRY))
			message->expire_time = (uint32)UNIXTIME + (TIME_DAY * 3);
	}
	message->SaveToDB();
}

void WorldSession::HandleMailDelete(WorldPacket & recv_data )
{
	uint64 mailbox;
	uint32 message_id;
	uint32 mailtemplet;
	recv_data >> mailbox >> message_id >> mailtemplet;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_DELETED);

	MailMessage * message = _player->m_mailBox->GetMessage(message_id);
	if(message == 0 || message->Expired())
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);
		return;
	}

	_player->m_mailBox->DeleteMessage(message);

	data << uint32(MAIL_OK);
	SendPacket(&data);
}

void WorldSession::HandleTakeItem(WorldPacket & recv_data )
{
	uint64 mailbox;
	uint32 message_id;
	uint32 lowguid;
	vector< uint64 >::iterator itr;

	recv_data >> mailbox >> message_id >> lowguid;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_ITEM_TAKEN);
	
	MailMessage * message = _player->m_mailBox->GetMessage(message_id);
	if(message == 0 || message->Expired() || message->items.empty())
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	for( itr = message->items.begin( ); itr != message->items.end( ); ++itr )
	{
		if ( (*itr) == lowguid )
			break;
	}

	if( itr == message->items.end( ) )
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	// check for cod credit
	if(message->cod > 0)
	{
		if(_player->GetUInt32Value(PLAYER_FIELD_COINAGE) < message->cod)
		{
			data << uint32(MAIL_ERR_NOT_ENOUGH_MONEY);
			SendPacket(&data);
			return;
		}
	}

	// grab the item
	Item* item = objmgr.LoadItem( *itr );
	if(item == 0)
	{
		// doesn't exist
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);
		
		return;
	}

	// find a free bag slot
	SlotResult result = _player->GetItemInterface()->FindFreeInventorySlot(item->GetProto());
	if(result.Result == 0)
	{
		// no free slots left!
		data << uint32(MAIL_ERR_BAG_FULL);
		SendPacket(&data);

		delete item;
		return;
	}

	item->m_isDirty = true;

	if( !_player->GetItemInterface()->SafeAddItem(item, result.ContainerSlot, result.Slot) )
	{
		if( !_player->GetItemInterface()->AddItemToFreeSlot(item) )
		{
			// no free slots left!
			data << uint32(MAIL_ERR_BAG_FULL);
			SendPacket(&data);
			delete item;
			return;
		}
	}
	else												// true so it waitexecute's
		item->SaveToDB(result.ContainerSlot, result.Slot, true, NULL);

	// send complete packet
	data << uint32(MAIL_OK);
	data << item->GetUInt32Value(OBJECT_FIELD_GUID);
	data << item->GetUInt32Value(ITEM_FIELD_STACK_COUNT);

	message->items.erase( itr );

	if ((message->items.size() == 0) && (message->money == 0))
	{
		// mail now has a 3 day expiry time
		if(!sMailSystem.MailOption(MAIL_FLAG_NO_EXPIRY))
			message->expire_time = (uint32)UNIXTIME + (TIME_DAY * 3);
	}

	if( message->cod > 0 )
	{
		_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -int32(message->cod));
		string subject = "COD Payment: ";
		subject += message->subject;
		sMailSystem.DeliverMessage(NORMAL, message->player_guid, message->sender_guid, subject, "", message->cod, 0, 0, 1, true);

		message->cod = 0;
	}

	// re-save (update the items field)
	message->SaveToDB();// sMailSystem.SaveMessageToSQL( message);
	SendPacket(&data);

	// prolly need to send an item push here
}

void WorldSession::HandleTakeMoney(WorldPacket & recv_data )
{
	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_MONEY_TAKEN);

	MailMessage * message = _player->m_mailBox->GetMessage(message_id);
	if(message == 0 || message->Expired() || !message->money)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	// add the money to the player
	if((_player->GetUInt32Value(PLAYER_FIELD_COINAGE) + message->money) >= PLAYER_MAX_GOLD )
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);
		return;
	}
	else
		_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, message->money);
	
	// force save
	_player->SaveToDB(false);

	// message no longer has any money
	message->money = 0;

	if ((message->items.size() == 0) && (message->money == 0))
	{
		// mail now has a 3 day expiry time
		if(!sMailSystem.MailOption(MAIL_FLAG_NO_EXPIRY))
			message->expire_time = (uint32)UNIXTIME + (TIME_DAY * 3);
	}
	// update in sql!
	message->SaveToDB();

	// send result
	data << uint32(MAIL_OK);
	SendPacket(&data);
}

void WorldSession::HandleReturnToSender(WorldPacket & recv_data )
{
	uint64 mailbox;
	uint32 message_id;
	uint64 returntoguid;
	recv_data >> mailbox >> message_id >> returntoguid;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_RETURNED_TO_SENDER);

	MailMessage * msg = _player->m_mailBox->GetMessage(message_id);
	if(msg == 0 || msg->Expired())
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}
	if(msg->returned_flag)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	sMailSystem.ReturnToSender(msg);

	// finish the packet
	data << uint32(MAIL_OK);
	SendPacket(&data);
}

void WorldSession::HandleMailCreateTextItem(WorldPacket & recv_data )
{
	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_MADE_PERMANENT);

	ItemPrototype * proto = ItemPrototypeStorage.LookupEntry(8383);
	MailMessage * message = _player->m_mailBox->GetMessage(message_id);
	if(message == 0 || message->Expired() || message->copy_made || !proto)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	SlotResult result = _player->GetItemInterface()->FindFreeInventorySlot(proto);
	if(result.Result == 0)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	Item* pItem = objmgr.CreateItem(8383, _player);
	pItem->SetUInt32Value(ITEM_FIELD_PAD, message_id);
	if( _player->GetItemInterface()->AddItemToFreeSlot(pItem) )
	{
		// mail now has an item after it
		message->copy_made = true;
		// update in sql
		message->SaveToDB();

		data << uint32(MAIL_OK);
		SendPacket(&data);
	}
	else
	{
		delete pItem;
	}
}

void WorldSession::HandleItemTextQuery(WorldPacket & recv_data)
{
	uint64 itemGuid;
	recv_data >> itemGuid;

	string body = "Internal Error";

	Item* item = _player->GetItemInterface()->GetItemByGUID(itemGuid);
	WorldPacket data(SMSG_ITEM_TEXT_QUERY_RESPONSE, body.length() + 9);
	if(!item)
		data << uint8(1);
	else
	{
//		item->gett
		data << uint8(0) << itemGuid << body;
	}
	SendPacket(&data);
}

void WorldSession::HandleMailTime(WorldPacket & recv_data)
{
	WorldPacket * data = _player->m_mailBox->MailboxTimePacket();
	SendPacket(data);
	delete data;
}

void WorldSession::SendMailError(uint32 error)
{
	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << uint32(0);
	data << uint32(MAIL_RES_MAIL_SENT);
	data << error;
	SendPacket(&data);
}

void WorldSession::HandleGetMail(WorldPacket & recv_data )
{
	WorldPacket * data = _player->m_mailBox->MailboxListingPacket();
	SendPacket(data);
	delete data;
}