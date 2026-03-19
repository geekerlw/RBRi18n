//
// Copyright 2020, MIKA-N. https://github.com/mika-n
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this softwareand associated
// documentation files(the "Software"), to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions :
// 
// - The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// - The derived work or derived parts are also "open sourced" and the source code or part of the work using components
//   from this project is made publicly available with modifications to this base work.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS
// OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include "IPlugin.h"
#include "IRBRGame.h"
#include <d3d9.h>
#include <string>

#define C_RBR_ADDR_TO_POINTER(absoluteAddr) (g_dwRBRBaseAddress + ((DWORD)(absoluteAddr) - 0x400000))

// Offset 0x007EAC48 — minimal, only resolutionX
struct RBRGameConfig {
#pragma pack(push,1)
    BYTE pad1[0x54];
    __int32 resolutionX;  // 0x54
#pragma pack(pop)
};

// Offset 0x007EAC48 + 0x728 — minimal, only gameMode
struct RBRGameMode {
#pragma pack(push,1)
    BYTE pad1[0x728];
    __int32 gameMode;     // 0x728
#pragma pack(pop)
};

// Menu object — pointer comparison only
struct RBRMenuObj;

#define RBRMENUSYSTEM_NUM_OF_MENUS 0x70
#define RBRMENUIDX_MAIN 00

// Offset 0x165FA48
struct RBRMenuSystem {
#pragma pack(push,1)
    BYTE pad1[0x08];
    RBRMenuObj* currentMenuObj;  // 0x08
    BYTE pad2[0x78 - 0x08 - sizeof(void*)];
    RBRMenuObj* menuObj[RBRMENUSYSTEM_NUM_OF_MENUS]; // 0x78
#pragma pack(pop)
};

//--- Globals ---

extern DWORD g_dwRBRBaseAddress;
extern LPDIRECT3DDEVICE9 g_pRBRIDirect3DDevice9;
extern RECT g_rectRBRWndClient;
extern RBRGameConfig*  g_pRBRGameConfig;
extern RBRGameMode*    g_pRBRGameMode;
extern RBRMenuSystem*  g_pRBRMenuSystem;
extern IRBRGame*       g_pRBRGame;

//--- Functions ---

const std::string& GetRBRRootPath();
BOOL RBRAPI_InitializeObjReferences();
void RBRAPI_MapRBRPointToScreenPoint(float x, float y, LONG width, LONG height, float* cx, float* cy, LONG* cwidth, LONG* cheight);
