#include <vector>
#include <conio.h>
#include "Session.h"
#include "SysProg.h"


using namespace std;


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


DWORD WINAPI MyThread(LPVOID lpParam)
{
    auto session = static_cast<Session*>(lpParam);
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
                Sleep(500 * session->sessionID);
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

    HANDLE hStartEvent = CreateEvent(NULL, FALSE, FALSE, L"StartEvent");
    HANDLE hStopEvent = CreateEvent(NULL, FALSE, FALSE, L"StopEvent");
    HANDLE hConfirmEvent = CreateEvent(NULL, FALSE, FALSE, L"ConfirmEvent");
    HANDLE hExitEvent = CreateEvent(NULL, FALSE, FALSE, L"ExitEvent");
    HANDLE hControlEvents[3] = { hStartEvent, hStopEvent, hExitEvent };

    SetEvent(hConfirmEvent); 

    while (threads_num + 1)
    {
        int n = WaitForMultipleObjects(3, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0;
        switch (n)
        {
        case 0:

            sessions.push_back(new Session(threads_num));
            CloseHandle(CreateThread(NULL, 0, MyThread, (LPVOID)sessions.back(), 0, NULL));
            SetEvent(hConfirmEvent);
            threads_num++;
            break;

        case 1:
            if (threads_num == 0) {
                threads_num = -1;
                SetEvent(hConfirmEvent);
                break;
            }
            sessions[--threads_num]->addMessage(MT_CLOSE);
            sessions.pop_back();
            SetEvent(hConfirmEvent);
            break;
        case 2:
            threads_num = -1;
            for (auto& session : sessions)
                delete session;
            SetEvent(hConfirmEvent);
            break;
        }
    }
}


int main()
{
    start();
    return 0;
}

