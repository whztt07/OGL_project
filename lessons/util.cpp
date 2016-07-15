#include <iostream>
#include <fstream>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#include "util.h"

long long GetCurrentTimeMillis()
{
#ifdef WIN32    
	return GetTickCount();
#else
	timeval t;
	gettimeofday(&t, NULL);

	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
#endif    
}

#ifdef WIN32
#if _MSC_VER != 1900
float fmax(float a, float b)
{
	if (a > b)
		return a;
	else
		return b;
}
#endif
#endif