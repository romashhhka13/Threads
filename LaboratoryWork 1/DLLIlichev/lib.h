#ifndef LIB_H
#define LIB_H

struct header {
	int addr;
	int size;
};

#ifdef LIB_EXPORTS  
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __declspec(dllimport)
#endif

extern "C"
{
	LIB_API std::wstring mapreceive(header& h);
	LIB_API void mapsend(int addr, const wchar_t* str);
}


#endif