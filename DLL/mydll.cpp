#include "pch.h"
#include "mydll.h"


using namespace std;


MessageStruct sendMessageC(int to, int type, const wchar_t* data)
{
	auto m = Message::send(to, type, data);
	MessageStruct mess(m.header, m.data);
	return mess;
}


