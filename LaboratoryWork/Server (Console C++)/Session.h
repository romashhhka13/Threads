#pragma once
#include <queue>
#include <string>
#include <mutex>
#include <chrono>


using namespace std;
using clock_type = std::chrono::steady_clock;


class Session
{
public:
	int id;
	wstring name;
	queue<Message> messages;
	chrono::time_point<clock_type> lastAccess;

	mutex mx;
	Session(int id, wstring name)
		:id(id), name(name), lastAccess(clock_type::now())
	{
	}

	void touch() {
		lastAccess = clock_type::now();
	}

	void add(Message& m)
	{
		lock_guard<mutex> lg(mx);
		messages.push(m);
	}

	void send(tcp::socket& s)
	{
		touch();
		lock_guard<mutex> lg(mx);
		if (messages.empty())
		{
			Message::send(s, id, MR_BROKER, MT_NODATA);
		}
		else
		{
			messages.front().send(s);
			messages.pop();
		}
	}
};