#pragma once

#ifndef MYDLL_H
#define MYDLL_H

#ifdef MYDLL_EXPORTS  
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif

#include "asio.h"

enum MessageType
{
	MT_INIT,
	MT_EXIT,
	MT_SEND,
	MT_CONFIRM,
	MT_STOP,
	MT_START
};


struct header {

	int type;
	int addr;
	int size;
};


extern "C"
{

	MYDLL_API void send_server(tcp::socket& s, int type, int addr, const wchar_t* data = nullptr);
	MYDLL_API std::wstring receive_server(tcp::socket& s, header& h);

	MYDLL_API header send_client(int type, int addr, const wchar_t* data);


	MYDLL_API void send_threads_num(tcp::socket& s, int n);
	MYDLL_API int receive_threads_num();
}



#endif