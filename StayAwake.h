#ifndef STAYAWAKE_H
#define STAYAWAKE_H

#include <Windows.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

void errorHandle(DWORD dwError, LPWSTR lpszFunction);
int main();

#endif /* STAYAWAKE_H */