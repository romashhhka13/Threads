#pragma once

#ifndef MYDLL_H
#define MYDLL_H

#ifdef MYDLL_EXPORTS  
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif


#include "Message.h"


struct MessageStruct
{
	MessageHeader header;
	wchar_t data[256];

	MessageStruct() = default;
	MessageStruct(MessageHeader& h, const std::wstring& str) : header(h) 
	{
		wcsncpy_s(data, str.c_str(), _TRUNCATE);
	};
};


extern "C"
{
	MYDLL_API MessageStruct sendMessageC(int to, int type = MT_DATA, const wchar_t* data = L"");
}


#endif