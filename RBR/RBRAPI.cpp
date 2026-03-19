#include "RBRAPI.h"
#include <filesystem>

DWORD g_dwRBRBaseAddress = 0;
LPDIRECT3DDEVICE9 g_pRBRIDirect3DDevice9 = nullptr;
RECT g_rectRBRWndClient;
RBRGameConfig*  g_pRBRGameConfig  = nullptr;
RBRGameMode*    g_pRBRGameMode    = nullptr;
RBRMenuSystem*  g_pRBRMenuSystem  = nullptr;
IRBRGame*       g_pRBRGame        = nullptr;

const std::string& GetRBRRootPath()
{
    static std::string sPath;
    if (sPath.empty()) {
        char buf[_MAX_PATH * 2] = {};
        ::GetModuleFileNameA(NULL, buf, sizeof(buf) - 1);
        sPath = std::filesystem::path(buf).parent_path().string();
    }
    return sPath;
}

BOOL RBRAPI_InitializeObjReferences()
{
    if (g_dwRBRBaseAddress == 0) {
        HMODULE hMod = nullptr;
        if (!::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, nullptr, &hMod) || !hMod)
            return FALSE;
        g_dwRBRBaseAddress = (DWORD)hMod;
    }

    if (!g_pRBRGameConfig)  g_pRBRGameConfig  = (RBRGameConfig*)*(DWORD*)C_RBR_ADDR_TO_POINTER(0x007EAC48);
    if (!g_pRBRGameMode)    g_pRBRGameMode    = (RBRGameMode*)*(DWORD*)C_RBR_ADDR_TO_POINTER(0x007EAC48);
    if (!g_pRBRMenuSystem)  g_pRBRMenuSystem  = (RBRMenuSystem*)*(DWORD*)C_RBR_ADDR_TO_POINTER(0x0165FA48);

    if (!g_pRBRIDirect3DDevice9)
        g_pRBRIDirect3DDevice9 = (LPDIRECT3DDEVICE9)*(DWORD*)(*(DWORD*)(*(DWORD*)C_RBR_ADDR_TO_POINTER(0x007EA990) + 0x28) + 0xF4);

    if (g_pRBRIDirect3DDevice9) {
        D3DDEVICE_CREATION_PARAMETERS params;
        g_pRBRIDirect3DDevice9->GetCreationParameters(&params);
        GetClientRect(params.hFocusWindow, &g_rectRBRWndClient);
    }

    return TRUE;
}

void RBRAPI_MapRBRPointToScreenPoint(float x, float y, LONG width, LONG height, float* cx, float* cy, LONG* cwidth, LONG* cheight)
{
    if (cx) {
        float offsetX = (g_rectRBRWndClient.right - g_pRBRGameConfig->resolutionX) / 2.0f;
        *cx = offsetX + x * (g_pRBRGameConfig->resolutionX / 640.0f);
    }
    if (cy)
        *cy = y * (g_rectRBRWndClient.bottom / 480.0f);
    if (cwidth)
        *cwidth = static_cast<LONG>(width * (g_rectRBRWndClient.bottom / 480.0f));
    if (cheight)
        *cheight = static_cast<LONG>(height * (g_rectRBRWndClient.bottom / 480.0f));
}
