#include "pch.h"
#include "mydll.h"


boost::asio::io_context io;
std::shared_ptr<tcp::socket> MySocket = nullptr;

void send_server(tcp::socket& s, int type, int addr, const wchar_t* data)
{
	header h;

	if (data != nullptr)
		h = { type, addr, (int)(wcslen(data) * sizeof(wchar_t)) };
	else
		h = { type, addr, 0 };

	sendData(s, &h);
	if (h.size)
	{
		sendData(s, data, h.size);
	}
}

std::wstring receive_server(tcp::socket& s, header& h)
{
	receiveData(s, &h);
	std::wstring data;
	if (h.size)
	{
		data.resize(h.size / sizeof(wchar_t));
		receiveData(s, data.data(), h.size);
	}
	return data;
}


header send_client(int type, int addr, const wchar_t* data)
{
	if ((MessageType)type == MT_INIT)
	{
		MySocket = std::make_shared<tcp::socket>(io);
		tcp::resolver r(io);
		boost::asio::connect(*MySocket, r.resolve("127.0.0.1", "12345"));
	}

	send_server(*MySocket, type, addr, data);

	header hConfirm = { 0 };
	receiveData(*MySocket, &hConfirm);

	if ((MessageType)type == MT_EXIT)
	{
		MySocket->close();
		//MySocket.reset();
	}

	return hConfirm;
}

void send_threads_num(tcp::socket& s, int n)
{
	sendData(s, &n);
}

int receive_threads_num()
{
	int n = 0;
	receiveData(*MySocket, &n);
	return n;
}
