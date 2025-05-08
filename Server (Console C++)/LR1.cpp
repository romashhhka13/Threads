#include <vector>
#include <thread>
#include <fstream>
#include <sstream>
#include <map>

#include "mydll.h"
#include "Session.h"
#include "SysProg.h"


using namespace std;


int maxID = MR_USER;
map<int, shared_ptr<Session>> sessions;


void cleanupInactiveSessions()
{
    const auto timeout = chrono::seconds(4);
    while (true)
    {
        this_thread::sleep_for(chrono::seconds(2));

        auto now = clock_type::now();
        vector<int> expired;

        for (auto& [id, session] : sessions)
        {
            if (now - session->lastAccess > timeout)
                expired.push_back(id);
        }

        for (int id : expired)
        {
            sessions.erase(id);
            SafeWrite(L"Клиент#", id, L"отключён (просрочен таймаут)");

            for (auto& [sess_id, sess] : sessions)
            {
                auto m = Message(sess_id, id, MT_EXIT);
                sess->add(m);
            }
        }
    }
}


void processClient(tcp::socket s)
{
    try
    {
        Message m;
        int type = m.receive(s);
        SafeWrite(L"Type:", type, L"to:", m.header.to, L"from:", m.header.from, L"mess:", m.data);
        switch (type)
        {
        case MT_INIT:
        {
            auto new_session = make_shared<Session>(++maxID, m.data);
            sessions[new_session->id] = new_session;
            Message::send(s, new_session->id, MR_BROKER, MT_CONFIRM);
            SafeWrite(L"Клиент", new_session->id, L"подключился");

            for (auto& [id, session] : sessions) {
                if (id != new_session->id) {
                    m = Message(id, new_session->id, MT_INIT);
                    session->add(m);
                    m = Message(new_session->id, id, MT_INIT);
                    new_session->add(m);
                }
            }

            break;
        }

        /*case MT_EXIT:
        {
            auto iSession = sessions.find(m.header.from);
            if (iSession != sessions.end())
            {
                Message::send(s, m.header.from, MR_BROKER, MT_CONFIRM);
                sessions.erase(m.header.from);
                SafeWrite(L"Клиент", m.header.from, L"отключился");

                for (auto& [id, s] : sessions) {
                    m = Message(id, m.header.from, MT_EXIT);
                    s->add(m);
                }
            }
            else
            {
                Message::send(s, m.header.from, MR_BROKER, MT_NODATA);
            }
            break;
        }*/

        case MT_GETDATA:
        {
            auto iSession = sessions.find(m.header.from);
            if (iSession != sessions.end())
            {
                iSession->second->send(s);
            }
            else
            {
                Message::send(s, MR_USER, MR_BROKER, MT_NODATA);
            }
            break;
        }

        default:
        {
            auto iSessionFrom = sessions.find(m.header.from);
            if (iSessionFrom != sessions.end())
            {
                auto iSessionTo = sessions.find(m.header.to);
                if (iSessionTo != sessions.end())
                {
                    iSessionTo->second->add(m);
                }
                else if (m.header.to == MR_ALL)
                {   
                    for (auto& [id, session] : sessions)
                    {
                        if (id != m.header.from)
                            session->add(m);
                    }
                }
                Message::send(s, m.header.from, MR_BROKER, MT_CONFIRM);
            }
            break;
        }
        }
    }
    catch (const std::exception& e)
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


        std::thread(cleanupInactiveSessions).detach();

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

