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

#ifndef _WORDFILTER_H
#define _WORDFILTER_H

struct WordFilterMatch
{
	char * szMatch;
	char * szIgnoreMatch;
	void* pCompiledExpression;
	void* pCompiledIgnoreExpression;
	void* pCompiledExpressionOptions;
	void* pCompiledIgnoreExpressionOptions;
	int iType;
};

class WordFilter
{
	WordFilterMatch ** m_filters;
	size_t m_filterCount;

	bool CompileExpression(const char * szExpression, void** pOutput, void** pExtraOutput);

public:
	WordFilter() : m_filters(NULL),m_filterCount(0) {}
	~WordFilter();

	void Load(const char * szTableName);
	bool Parse(string& sMessage, bool bAllowReplace = true);
};

extern WordFilter * g_characterNameFilter;
extern WordFilter * g_chatFilter;

#endif		// _WORDFILTER_H
