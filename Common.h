#pragma once
#pragma warning(disable: 4244 4800)

#include "DXUT.h"

using namespace std;

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif