#include "pch.h"
#include "Message.h"


int Message::clientID = 0;


void Message::send(tcp::socket& s, int to, int from, int type, const wstring& data)
{
	Message m(to, from, type, data);
	m.send(s);
}


Message Message::send(int to, int type, const wstring& data)
{
	boost::asio::io_context io;
	tcp::socket s(io);
	tcp::resolver r(io);
	boost::asio::connect(s, r.resolve("127.0.0.1", "12345"));

	Message m(to, clientID, type, data);
	m.send(s);
	m.receive(s);
	if (type == MT_INIT) {
		clientID = m.header.to;
	}

	return m;
}