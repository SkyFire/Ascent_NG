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

#include "RStdAfx.h"
#include <Console/BaseConsole.h>
#include "ConsoleCommands.h"

struct ConsoleCommand
{
	bool(*CommandPointer)(BaseConsole*, int, const char*[]);
	const char * Name;					// 10 chars
	const char * ArgumentFormat;		// 30 chars
	const char * Description;			// 40 chars
	// = 70 chars
};
void HandleConsoleInput(BaseConsole * pConsole, const char * szInput)
{
	static ConsoleCommand Commands[] = {
		{ &HandleAnnounceCommand, "a", "<announce string>", "Shows the message in all client chat boxes." },
		{ &HandleAnnounceCommand, "announce", "<announce string>", "Shows the message in all client chat boxes." },
		{ &HandleBanAccountCommand, "ban", "<account> <timeperiod>", "Bans account x for time y." },
		{ &HandleBanAccountCommand, "banaccount", "<account> <timeperiod>", "Bans account x for time y." },
		{ &HandleBackupDBCommand, "backupdb", "none", "Backups Character Database" },
		{ &HandleCancelCommand, "cancel", "none", "Cancels a pending shutdown." },
		{ &HandleCreateAccountCommand, "createaccount", "<name> <pass> <email> <flags>", "Creates an account." },
		{ &HandleInfoCommand, "info", "none", "Gives server runtime information." },
		{ &HandleGMsCommand, "gms", "none", "Shows online GMs." },
		{ &HandleKickCommand, "kick", "<plrname> <reason>", "Kicks player x for reason y." },
		{ &HandleMOTDCommand, "getmotd", "none", "View the current MOTD" },
		{ &HandleMOTDCommand, "setmotd", "<new motd>", "Sets a new MOTD" },
		{ &HandleOnlinePlayersCommand, "online", "none", "Shows online players." },
		{ &HandlePlayerInfoCommand, "playerinfo", "<plrname>", "Shows information about a player." },
		{ &HandleQuitCommand, "exit", "[delay]", "Shuts down server with optional delay in seconds." },
		{ &HandleQuitCommand, "quit", "[delay]", "Shuts down server with optional delay in seconds." },
		{ &HandleQuitCommand, "shutdown", "[delay]", "Shuts down server with optional delay in seconds." },
		{ &HandleRehashCommand, "rehash", "none", "Reloads the config file" },
		{ &HandleUnbanAccountCommand, "unban", "<account>", "Unbans account x." },
		{ &HandleUnbanAccountCommand, "unbanaccount", "<account>", "Unbans account x." },
		{ &HandleWAnnounceCommand, "w", "<wannounce string>", "Shows the message in all client title areas." },
		{ &HandleWAnnounceCommand, "wannounce", "<wannounce string>", "Shows the message in all client title areas." },
		{ &HandleWhisperCommand, "whisper","<player> <message>", "Whispers a message to someone from the console." },

		{ NULL, NULL, NULL, NULL },
	};

	uint32 i;
	char * p, *q;

	// let's tokenize into arguments.
	vector<const char*> tokens;

	q = (char*)szInput;
	p = strchr(q, ' ');
	while( p != NULL )
	{
		*p = 0;
		tokens.push_back(q);

		q = p+1;
		p = strchr(q, ' ');
	}

	if( q != NULL && *q != '\0' )
		tokens.push_back( q	);

	if( tokens.empty() )
		return;

	if( !stricmp(tokens[0], "help") || tokens[0][0] == '?' )
	{
		pConsole->Write("=========================================================================================================\r\n");
		pConsole->Write("| %15s | %30s | %50s |\r\n", "Name", "Arguments", "Description");
		pConsole->Write("=========================================================================================================\r\n");		
		for(i = 0; Commands[i].Name != NULL; ++i)
		{
			pConsole->Write("| %15s | %30s | %50s |\r\n", Commands[i].Name, Commands[i].ArgumentFormat, Commands[i].Description);
		}
		//pConsole->Write("=========================================================================================================\r\n");		
		//pConsole->Write("| type 'quit' to terminate a Remote Console Session                                                     |\r\n");
		//pConsole->Write("=========================================================================================================\r\n");		
	}
	else
	{
		for(i = 0; Commands[i].Name != NULL; ++i)
		{
			if( !stricmp( Commands[i].Name, tokens[0] ) )
			{
				if( !Commands[i].CommandPointer( pConsole, (int)tokens.size(), &tokens[0] ) )
				{
					pConsole->Write("[!]Error, '%s' used an incorrect syntax, the correct syntax is: '%s'.\r\n\r\n", Commands[i].Name, Commands[i].ArgumentFormat );
					return;
				}
				else
					return;
			}
		}

		pConsole->Write("[!]Error, Command '%s' doesn't exist. Type '?' or 'help'to get a command overview.\r\n\r\n", tokens[0]);
	}
}