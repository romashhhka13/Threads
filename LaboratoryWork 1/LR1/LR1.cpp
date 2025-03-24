#include <vector>
#include <thread>
#include <fstream>
#include <mutex>

#include "Session.h"
#include "SysProg.h"
#include "lib.h"


using namespace std;


mutex m;


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

void start()
{
    vector<Session*> sessions;
    int threads_num = 0;

    HANDLE hMutex = CreateMutex(NULL, FALSE, L"hMutex");

    HANDLE hStartEvent = CreateEvent(NULL, FALSE, FALSE, L"StartEvent");
    HANDLE hStopEvent = CreateEvent(NULL, FALSE, FALSE, L"StopEvent");
    HANDLE hConfirmEvent = CreateEvent(NULL, FALSE, FALSE, L"ConfirmEvent");
    HANDLE hExitEvent = CreateEvent(NULL, FALSE, FALSE, L"ExitEvent");
    HANDLE hGetMessageEvent = CreateEvent(NULL, FALSE, FALSE, L"hGetMessageEvent");
    HANDLE hControlEvents[4] = { hStartEvent, hStopEvent, hExitEvent, hGetMessageEvent };

    SetEvent(hConfirmEvent); 

    while (threads_num + 1)
    {
        int n = WaitForMultipleObjects(4, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0;
        switch (n)
        {
        case 0:
        {
            sessions.push_back(new Session(threads_num));
            thread t(MyThread, sessions.back());
            t.detach();
            SetEvent(hConfirmEvent);
            threads_num++;
            break;
        }
        case 1:
        {
            if (threads_num == 0) {
                threads_num = -1;
                SetEvent(hConfirmEvent);
                break;
            }
            sessions[--threads_num]->addMessage(MT_CLOSE);
            sessions.pop_back();
            SetEvent(hConfirmEvent);
            break;
        }
        case 2:
        {
            threads_num = -1;
            for (auto& session : sessions)
                delete session;
            SetEvent(hConfirmEvent);
            break;
        }
        case 3:
        {
            header h;
            WaitForSingleObject(hMutex, INFINITE);
            wstring s = mapreceive(h);
            ReleaseMutex(hMutex);

            if (h.addr == -2) {
                SafeWrite(s);
            }
            else if (h.addr == -1)
                for (auto session : sessions) {
                    session->addMessage(MT_DATA, s);
                    
                }
            else {
                sessions[h.addr]->addMessage(MT_DATA, s);
            }
            SetEvent(hConfirmEvent);
            break;
        }
        }
    }
}


int main()
{
    std::wcin.imbue(std::locale("rus_rus.866"));
    std::wcout.imbue(std::locale("rus_rus.866"));
    start();
    return 0;
}

