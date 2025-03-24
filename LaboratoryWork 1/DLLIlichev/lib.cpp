#include "pch.h"
#include "lib.h"


std::wstring mapreceive(header& h)
{
	HANDLE hFile = CreateFile(L"filemap.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);

	// Первый раз открываем на размер Header
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, sizeof(header), L"MyMap");

	LPVOID buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(header));
	h = *((header*)buff);
	UnmapViewOfFile(buff);
	CloseHandle(hFileMap);

	int n = h.size + sizeof(header);
	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, n, L"MyMap");
	buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, n);
	std::wstring s((wchar_t*)((BYTE*)buff + sizeof(header)), h.size / 2);

	UnmapViewOfFile(buff);
	CloseHandle(hFileMap);
	CloseHandle(hFile);

	return s;
}


void mapsend(int addr, const wchar_t* str)
{
	header h = { addr, int(wcslen(str) * 2) };
	HANDLE hFile = CreateFile(L"filemap.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);

	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, h.size + sizeof(header), L"MyMap");
	BYTE* buff = (BYTE*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.size + sizeof(header));

	memcpy(buff, &h, sizeof(header));
	memcpy(buff + sizeof(header), str, h.size);

	UnmapViewOfFile(buff);
	CloseHandle(hFileMap);
	CloseHandle(hFile);
	
	SetEvent(OpenEvent(EVENT_MODIFY_STATE, FALSE, L"hGetMessageEvent"));
	//SetEvent(CreateEvent(NULL, FALSE, FALSE, L"hGetMessageEvent"));
}



