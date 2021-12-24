#pragma once

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)
#pragma warning( disable : 4244)

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

typedef unsigned long   ulong; // DWORD NOT ALLOWED
typedef unsigned char   byte;

#include <stdint.h>

#include "SDK.h"
#include "enumeration.h"

#include "renderer.h"

#include "utils.h"
#include "string.h"
#include "hash.h"
#include "checksum.h"

#include "vector.h"
#include "math.h"

#include "pattern.h"
#include "vmthook.h"
#include "erase.h"