#pragma once
#ifndef _WIN32
// Fichier de compatibilité pour porter le code LibGraph2 vers Linux
// Ce fichier définit les types Windows minimums requis par LibGraph2

#include <cstdint>

#define APIENTRY
#define CALLBACK
#define WINAPI

typedef void *HANDLE;
typedef void *HWND;
typedef void *HDC;
typedef void *HINSTANCE;
typedef char *LPTSTR;
typedef const char *LPCTSTR;
typedef uint32_t DWORD;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef int BOOL;

#define TRUE 1
#define FALSE 0

#define UNREFERENCED_PARAMETER(P) (void)(P)

#endif
