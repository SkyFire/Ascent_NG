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


#include "mpq.h"
//#include <vector>
#include "Stormlib.h"
#define __STORMLIB_SELF__

typedef std::vector<MPQArchive*> ArchiveSet;
ArchiveSet gOpenArchives;
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
MPQArchive::MPQArchive(const char* filename)
{
	BOOL succ = SFileOpenArchive(filename, 0, 0,&hMPQ);
	if (succ) 
	{
		MPQArchive*ar = (MPQArchive*)(hMPQ);
		printf("Opening %s\n", filename);
		gOpenArchives.push_back(ar);
        succ = true;
		
	} 
	else 
	{
		printf("Error!!!Not open archive %s\n", filename);
	}
}

void MPQArchive::close()
{
	SFileCloseArchive(hMPQ);
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
MPQFile::MPQFile(const char* filename):
	eof(false),
	buffer(0),
	pointer(0),
	size(0)
{
	for(ArchiveSet::iterator i=gOpenArchives.begin(); i!=gOpenArchives.end();++i)
	{  
		
	 HANDLE hFile = "";
	 MPQArchive*(hMPQ) = *i;
	 BOOL succ = SFileOpenFileEx(hMPQ,filename,0, &hFile);
	   if (succ) 
	   {
		 DWORD s = SFileGetFileSize(hFile, 0);
		 if (!s)
		 {
			eof = true;
			buffer = 0;
			return;
		 }
         size = (size_t)s;
		 buffer = new char[s];
		 SFileReadFile(hFile, buffer, s, 0, 0);
		 SFileCloseFile(hFile);
		 
		 eof = false;
		 return;
	   }
	}   
     
	eof = true;
	buffer = 0;
	  
}

MPQFile::~MPQFile()
{
	close();
}

size_t MPQFile::read(void* dest, size_t bytes)
{
	if (eof) 
	return 0;

	size_t rpos = pointer + bytes;
	if (rpos > size) 
	{
		bytes = size - pointer;
		eof = true;
	}

	memcpy(dest, &(buffer[pointer]), bytes);

	pointer = rpos;

	return bytes;
}

bool MPQFile::isEof()
{
    return eof;
}

void MPQFile::seek(int offset)
{
	pointer = offset;
	eof = (pointer >= size);
}

void MPQFile::seekRelative(int offset)
{
	pointer += offset;
	eof = (pointer >= size);
}

void MPQFile::close()
{
	if (buffer) 
	delete[] buffer;
	buffer = 0;
	eof = true;
}

size_t MPQFile::getSize()
{
	return size;
}

size_t MPQFile::getPos()
{
	return pointer;
}

char* MPQFile::getBuffer()
{
	return buffer;
}

char* MPQFile::getPointer()
{
	return buffer + pointer;
}

