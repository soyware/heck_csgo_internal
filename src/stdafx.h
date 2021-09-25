#pragma once

#define WINVER 0x0601
#define _WIN32_WINNT 0x0601
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <thread>
#include <vector>
#include <cstdio>
#include <Psapi.h>
#include <intrin.h>
#include <map>
#include <cmath>
#include <filesystem>
#include <cassert>

#include "SDK/Utl/UtlSymbol.h"
#include "SDK/Utl/UtlString.h"
#include "SDK/Utl/UtlMemory.h"
#include "SDK/Utl/UtlVector.h"

using namespace std::chrono_literals;

#pragma warning(disable : 26495) // Variable 'cplane_t::dist' is uninitialized
#pragma warning(disable : 4100) // '_edx': unreferenced formal parameter
#pragma warning(disable : 6387) // 'Temp_value_#12768' could be '0'
#pragma warning(disable : 26812) // The enum type 'Hooks::ClientFrameStage_t' is unscoped