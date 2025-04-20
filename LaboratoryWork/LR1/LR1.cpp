#include <vector>
#include <thread>
#include <fstream>
#include <map>

#include "asio.h"
#include "Session.h"
#include "SysProg.h"
#include "mydll.h"


using namespace std;


vector<Session*> sessions;


int MyThread(Session* session)
{
    SafeWrite("session", session->sessionID, "created");

    while (true)
    {
        Message m;
        if (session->getMessage(m))
        {
            switch (m.header.messageType)
            {
            case MT_CLOSE:
            {
                SafeWrite("session", session->sessionID, "closed");
                delete session;
                return 0;
            }
            case MT_DATA:
            {
                SafeWrite("session", session->sessionID, "data", m.data);
                wofstream file(L".data\\" + to_wstring(session->sessionID) + L".txt", std::ios::app);
                file.imbue(std::locale(".1251"));
                file << m.data << endl;
                break;
            }
            }
        }
    }

    return 0;
}



void processClient(tcp::socket s)
{
	try
	{
		while (true)
		{
			header h;
			wstring data = receive_server(s, h);
            switch (h.type)
            {
            case MT_INIT:
            {
                SafeWrite(L"Клиент подключился");
                break;
            }
            case MT_START:
            {
                sessions.push_back(new Session(sessions.size()));
                thread(MyThread, sessions.back()).detach();
                break;
            }
            case MT_STOP:
            {
                if (sessions.size() > 0) {
                    sessions.back()->addMessage(MT_CLOSE);
                    sessions.pop_back();
                }
                break;
            }
            case MT_EXIT:
            {
                SafeWrite(L"Клиент отключился");
                send_server(s, MT_CONFIRM, 0);
                return;
            }
            case MT_SEND:
            {
                if (h.addr == -2) {
                    SafeWrite(data);
                }
                else if (h.addr == -1)
                    for (auto session : sessions) {
                        session->addMessage(MT_DATA, data);

                    }
                else {
                    sessions[h.addr]->addMessage(MT_DATA, data);
                }
                break;
            }
            }
            send_server(s, MT_CONFIRM, 0);
            send_threads_num(s, sessions.size());
		}
	}
	catch (std::exception& e)
	{
		std::wcerr << "Exception: " << e.what() << endl;
	}
}


void start()
{
    try
    {
        int port = 12345;
        boost::asio::io_context io;
        tcp::acceptor a(io, tcp::endpoint(tcp::v4(), port));


        while (true)
        {
            std::thread(processClient, a.accept()).detach();
        }
    }
    catch (std::exception& e)
    {
        std::wcerr << "Exception: " << e.what() << endl;
    }
}


int main()
{
    std::locale::global(std::locale("rus_rus.866"));
    wcin.imbue(std::locale());
    wcout.imbue(std::locale());

    start();

    return 0;
}

