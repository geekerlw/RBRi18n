#include <filesystem>
#include "MinHook.h"
#include "Utils/LogUtil.h"
#include "RBR/RBRAPI.h"
#include "RBRi18n.h"

namespace fs = std::filesystem;

extern RBRi18n* g_pRBRPlugin;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MH_Initialize();
        break;
    case DLL_PROCESS_DETACH:
        MH_Uninitialize();
        break;
    }
    return TRUE;
}

extern "C"
{
    __declspec(dllexport) IPlugin* RBR_CreatePlugin(IRBRGame* pGame)
    {
        if (g_pRBRPlugin == nullptr)
        {
            fs::path pluginPath = fs::path(GetRBRRootPath()).append("RBRi18n");
            fs::create_directory(pluginPath);
            fs::path logfilePath = pluginPath.append("RBRi18n.log");
            LogUtil::SetLogFile(logfilePath.string().c_str());

            g_pRBRPlugin = new RBRi18n(pGame);
        }
        return g_pRBRPlugin;
    }
}
