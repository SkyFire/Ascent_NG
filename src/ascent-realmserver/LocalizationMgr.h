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

#ifndef __LOCALIZATIONMGR_H
#define __LOCALIZATIONMGR_H

struct LocalizedCreatureName
{
	char * Name;
	char * SubName;
};

struct LocalizedGameObjectName
{
	char * Name;
};

struct LocalizedNpcText
{
	char * Texts[8][2];
};

struct LocalizedItemPage
{
	char * Text;
};

struct LocalizedItem
{
	char * Name;
	char * Description;
};

struct LocalizedQuest
{
	char * Title;
	char * Details;
	char * Objectives;
	char * CompletionText;
	char * IncompleteText;
	char * EndText;
	char * ObjectiveText[4];
};

class LocalizationMgr
{
public:
	void Shutdown();

	void Reload(bool first);
	void Lower(string& conv);
	uint32 GetLanguageId(uint32 full);
	uint32 GetLanguageId(string langstr)
	{
		string ns = langstr;
		Lower(ns);

		uint32 lid = *(uint32*)ns.c_str();
		return GetLanguageId(lid);
	}

	LocalizedQuest *			GetLocalizedQuest(uint32 id, uint32 language);
	LocalizedItem*				GetLocalizedItem(uint32 id, uint32 language);
	LocalizedNpcText *			GetLocalizedNpcText(uint32 id, uint32 language);
	LocalizedCreatureName *		GetLocalizedCreatureName(uint32 id, uint32 language);
	LocalizedGameObjectName *	GetLocalizedGameObjectName(uint32 id, uint32 language);
	LocalizedItemPage *			GetLocalizedItemPage(uint32 id, uint32 language);

	template<typename T>
		void CopyHashMap(HM_NAMESPACE::hash_map<uint32,T> * src, HM_NAMESPACE::hash_map<uint32,T> * dest)
	{
		for(typename HM_NAMESPACE::hash_map<uint32,T>::iterator itr = src->begin(); itr != src->end(); ++itr)
			dest->insert(make_pair(itr->first,itr->second));
	}

private:
	HM_NAMESPACE::hash_map<uint32, LocalizedQuest>				* m_Quests;
	HM_NAMESPACE::hash_map<uint32, LocalizedItem>				* m_Items;
	HM_NAMESPACE::hash_map<uint32, LocalizedNpcText>			* m_NpcTexts;
	HM_NAMESPACE::hash_map<uint32, LocalizedCreatureName>		* m_CreatureNames;
	HM_NAMESPACE::hash_map<uint32, LocalizedGameObjectName>		* m_GameObjectNames;
	HM_NAMESPACE::hash_map<uint32, LocalizedItemPage>			* m_ItemPages;
	vector<pair<uint32,uint32> >								  m_languages;
	bool														  m_disabled;
};

extern LocalizationMgr sLocalizationMgr;

#endif
