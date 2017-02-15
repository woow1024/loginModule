#pragma once

#include <mem_macro.h>
#include <str_macro.h>
#include <std_macro.h>
#include <socket_macro.h>
#include <debug_macro.h>

#define DLLIMPORT						__declspec(dllimport)
#define DLLEXPORT						__declspec(dllexport)

#define C_DLLIMPORT						extern "C" DLLIMPORT
#define C_DLLEXPORT						extern "C" DLLEXPORT




