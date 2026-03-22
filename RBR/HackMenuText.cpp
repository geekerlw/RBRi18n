#include "HackMenuText.h"
#include <Windows.h>
#include <unordered_map>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <unordered_set>
#include <cstdarg>
#include <cstdio>
#include "MinHook.h"
#include "RBRAPI.h"
#include "IRBRGame.h"
#include "../Utils/SimpleIni.h"
#include "../DirectX/D3D9Font/D3DFont.h"

namespace fs = std::filesystem;

typedef void (__thiscall* WriteText_t)(IRBRGame*, float, float, const char*);
typedef void (__thiscall* SetColor_t)(IRBRGame*, float, float, float, float);
typedef void (__thiscall* SetMenuColor_t)(IRBRGame*, int);
typedef void (__thiscall* SetFont_t)(IRBRGame*, IRBRGame::EFonts);
// sub_4728F0: void (int* a1, int a2, int a3, char* a4, ...) — a2/a3 = x/y, a4 = sprintf format
typedef int (__cdecl* MenuTextDraw_t)(int* a1, int a2, int a3, char* a4, ...);

static WriteText_t       g_OriginalWriteText       = nullptr;
static SetColor_t        g_OriginalSetColor        = nullptr;
static SetMenuColor_t    g_OriginalSetMenuColor    = nullptr;
static SetFont_t         g_OriginalSetFont         = nullptr;

// Cached function addresses from IRBRGame vtable (for unhooking)
static void* g_WriteTextAddr     = nullptr;
static void* g_SetFontAddr       = nullptr;
static void* g_SetMenuColorAddr  = nullptr;
static void* g_SetColorAddr      = nullptr;

static std::unordered_map<std::string, std::string> g_translations;

// One Chinese font per IRBRGame::EFonts value
static CD3DFont* g_pChineseFonts[IRBRGame::FONT_HEADING + 1] = { nullptr, nullptr, nullptr, nullptr };
// Dedicated font for sub_4728F0 menu text draw
static CD3DFont* g_pMenuTextFont = nullptr;

// Font config (read from INI, defaults used if not configured)
static std::wstring g_fontFamily = L"SimHei";
static int g_fontSizeSmall   = 7;
static int g_fontSizeBig     = 8;
static int g_fontSizeDebug   = 6;
static int g_fontSizeHeading = 8;
static int g_fontSizeMenu    = 8;
static bool g_hookRecordEnabled = true;
static bool g_hookRecordOnlyUntranslated = false;
static IRBRGame::EFonts g_currentFont = IRBRGame::FONT_SMALL;

static IRBRGame* g_pGameInstance = nullptr;
static DWORD g_dwCurrentColor = D3DCOLOR_ARGB(255, 255, 255, 255); // Default white

struct PendingText {
    float x;
    float y;
    std::wstring text;
    IRBRGame::EFonts font;
    DWORD color;
};

// Pending texts from sub_4728F0 (menu text draw); drawn on top in DrawPendingChineseMenuText
struct PendingMenuText {
    float x;
    float y;
    std::wstring text;
    DWORD color;
};

static std::vector<PendingText> g_pendingTexts;
static std::vector<PendingMenuText> g_pendingMenuTexts;

static std::ofstream g_hookRecordFile;
static std::mutex g_hookRecordMutex;
static std::unordered_set<std::string> g_hookRecordSeen;

static MenuTextDraw_t g_OriginalMenuTextDraw = nullptr;

// EndScene hook to flush pending Chinese text each frame
typedef HRESULT(WINAPI* EndScene_t)(IDirect3DDevice9*);
static EndScene_t g_OriginalEndScene = nullptr;

static std::string CurrentTimestampForFileName()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tmInfo = {};
    localtime_s(&tmInfo, &t);

    std::ostringstream oss;
    oss << std::put_time(&tmInfo, "%Y%m%d_%H%M%S");
    return oss.str();
}

static std::string CurrentTimestampForLine()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::tm tmInfo = {};
    localtime_s(&tmInfo, &t);

    std::ostringstream oss;
    oss << std::put_time(&tmInfo, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

static std::string EscapeRecordText(const std::string& text)
{
    std::string escaped;
    escaped.reserve(text.size());
    for (char c : text) {
        if (c == '\r') {
            escaped += "\\r";
        } else if (c == '\n') {
            escaped += "\\n";
        } else {
            escaped.push_back(c);
        }
    }
    return escaped;
}

static void AppendHookRecord(const char* source, const std::string& originalText, const char* resultTag)
{
    if (g_hookRecordOnlyUntranslated) {
        if (!resultTag || std::string(resultTag) != "Original") {
            return;
        }
    }

    std::lock_guard<std::mutex> lock(g_hookRecordMutex);
    if (!g_hookRecordFile.is_open()) {
        return;
    }

    // Deduplicate: only record each unique text once per session
    if (g_hookRecordSeen.count(originalText)) {
        return;
    }
    g_hookRecordSeen.insert(originalText);

    g_hookRecordFile
        << '[' << CurrentTimestampForLine() << "]"
        << " [" << (source ? source : "Unknown") << "]"
        << " [" << (resultTag ? resultTag : "Unknown") << "] "
        << EscapeRecordText(originalText)
        << std::endl;
}

static void StartHookRecordFile()
{
    if (!g_hookRecordEnabled) {
        return;
    }

    std::lock_guard<std::mutex> lock(g_hookRecordMutex);
    if (g_hookRecordFile.is_open()) {
        return;
    }

    fs::path logDir = fs::path(GetRBRRootPath()) / "RBRi18n";
    fs::create_directories(logDir);
    fs::path recordFile = logDir / ("HookRecord_" + CurrentTimestampForFileName() + ".txt");

    g_hookRecordFile.open(recordFile.string(), std::ios::out | std::ios::trunc);
    if (g_hookRecordFile.is_open()) {
        g_hookRecordFile << "# RBRi18n hook capture log" << std::endl;
        g_hookRecordFile << "# File created at startup" << std::endl;
    }
}

static void StopHookRecordFile()
{
    std::lock_guard<std::mutex> lock(g_hookRecordMutex);
    if (!g_hookRecordFile.is_open()) {
        return;
    }
    g_hookRecordFile.flush();
    g_hookRecordFile.close();
    g_hookRecordSeen.clear();
}

// Normalize INI key/value token:
// - Trim leading/trailing whitespace
// - If wrapped in double quotes, strip the outer quotes
// This allows translations.ini to use quoted keys/values to safely
// contain special characters, spaces, or non-ASCII text.
static std::string NormalizeIniToken(const std::string& in)
{
    if (in.empty())
        return {};

    size_t start = 0;
    size_t end = in.size();

    // Trim leading whitespace
    while (start < end && (in[start] == ' ' || in[start] == '\t' || in[start] == '\r' || in[start] == '\n')) {
        ++start;
    }
    // Trim trailing whitespace
    while (end > start && (in[end - 1] == ' ' || in[end - 1] == '\t' || in[end - 1] == '\r' || in[end - 1] == '\n')) {
        --end;
    }

    if (end <= start)
        return {};

    // Strip surrounding double quotes, if present
    if (in[start] == '\"' && in[end - 1] == '\"' && end - start >= 2) {
        ++start;
        --end;
    }

    return in.substr(start, end - start);
}

// Hook function for IRBRGame::WriteText
void __fastcall Hook_WriteText(IRBRGame* pThis, void* edx, float x, float y, const char* ptxtText)
{
    if (ptxtText && g_OriginalWriteText) {
        std::string text(ptxtText);
        auto it = g_translations.find(text);
        if (it != g_translations.end()) {
            AppendHookRecord("WriteText", text, "Translated");
            // Convert UTF-8 to wide string and queue for drawing at end of frame
            int size = MultiByteToWideChar(CP_UTF8, 0, it->second.c_str(), -1, nullptr, 0);
            if (size > 0) {
                std::wstring wideText(size, 0);
                MultiByteToWideChar(CP_UTF8, 0, it->second.c_str(), -1, &wideText[0], size);

                PendingText pt;
                pt.x = x;
                pt.y = y;
                pt.text = std::move(wideText);
                pt.font = g_currentFont;
                pt.color = g_dwCurrentColor;
                g_pendingTexts.push_back(std::move(pt));
                return; // Do not let original draw English text
            }
        }

        AppendHookRecord("WriteText", text, "Original");
    }

    // Call original
    g_OriginalWriteText(pThis, x, y, ptxtText);
}

// Hook function for IRBRGame::SetColor
void __fastcall Hook_SetColor(IRBRGame* pThis, void* edx, float r, float g, float b, float a)
{
    // Track current color for Chinese font
    g_dwCurrentColor = D3DCOLOR_ARGB((int)(a * 255), (int)(r * 255), (int)(g * 255), (int)(b * 255));
    
    // Call original
    if (g_OriginalSetColor) {
        g_OriginalSetColor(pThis, r, g, b, a);
    }
}

// Hook function for IRBRGame::SetMenuColor
void __fastcall Hook_SetMenuColor(IRBRGame* pThis, void* edx, int eColor)
{
    // Let the game handle its internal menu color state.
    if (g_OriginalSetMenuColor) {
        g_OriginalSetMenuColor(pThis, eColor);
    }

    // Also map menu color enum to an approximate ARGB value for our CD3DFont.
    // You can tweak these values to better match RBR's originals.
    switch (eColor) {
        case IRBRGame::MENU_BKGROUND:
            g_dwCurrentColor = D3DCOLOR_ARGB(255, 50, 50, 50);
            break;
        case IRBRGame::MENU_SELECTION:
            g_dwCurrentColor = D3DCOLOR_ARGB(255, 255, 0, 0);       // red
            break;
        case IRBRGame::MENU_ICON:
            g_dwCurrentColor = D3DCOLOR_ARGB(255, 200, 200, 200);   // light gray
            break;
        case IRBRGame::MENU_TEXT:
            g_dwCurrentColor = D3DCOLOR_ARGB(255, 255, 255, 255);   // white
            break;
        case IRBRGame::MENU_HEADING:
            g_dwCurrentColor = D3DCOLOR_ARGB(255, 255, 255, 255);   // white
            break;
        default:
            // Fallback: leave as-is (usually whatever SetColor last set)
            break;
    }
}

// Hook function for IRBRGame::SetFont
void __fastcall Hook_SetFont(IRBRGame* pThis, void* edx, IRBRGame::EFonts eFont)
{
    // Track current font for Chinese rendering
    if (eFont >= IRBRGame::FONT_SMALL && eFont <= IRBRGame::FONT_HEADING) {
        g_currentFont = eFont;
    }

    // Forward to original so the game keeps its own behavior
    if (g_OriginalSetFont) {
        g_OriginalSetFont(pThis, eFont);
    }
}

void DrawPendingChineseMenuText()
{
    if (!g_pRBRIDirect3DDevice9) {
        g_pendingTexts.clear();
        g_pendingMenuTexts.clear();
        return;
    }

    // Draw WriteText-path pending texts first (under layer)
    for (const auto& pt : g_pendingTexts) {
        if (pt.font < IRBRGame::FONT_SMALL || pt.font > IRBRGame::FONT_HEADING) {
            continue;
        }
        CD3DFont* pFont = g_pChineseFonts[pt.font];
        if (!pFont) {
            continue;
        }

        FLOAT cx = 0.0f, cy = 0.0f;
        LONG cw = 0, ch = 0;
        RBRAPI_MapRBRPointToScreenPoint(pt.x, pt.y, 0, 0, &cx, &cy, &cw, &ch);
        int drawX = static_cast<int>(cx);
        int drawY = static_cast<int>(cy);

        pFont->DrawText(drawX, drawY, pt.color, pt.text.c_str());
    }
    g_pendingTexts.clear();

    // Draw sub_4728F0 menu-text pending items on top (same mapping as WriteText path above).
    // Main menu (RBRMENUIDX_MAIN) uses right align; inner menus use left align. Use g_pRBRMenuSystem, no SetDrawMode hook.
    if (g_pMenuTextFont) {
        bool onMainMenu = (g_pRBRMenuSystem && g_pRBRMenuSystem->currentMenuObj &&
            g_pRBRMenuSystem->currentMenuObj == g_pRBRMenuSystem->menuObj[RBRMENUIDX_MAIN]);

        for (const auto& pm : g_pendingMenuTexts) {
            // sub_4728F0 already scales coordinates internally.
            // Only add widescreen X offset to center on ultrawide.
            int offsetX = (g_rectRBRWndClient.right - g_pRBRGameConfig->resolutionX) / 2;
            int drawX = offsetX + static_cast<int>(pm.x);
            int drawY = static_cast<int>(pm.y);

            SIZE extent = { 0, 0 };
            if (SUCCEEDED(g_pMenuTextFont->GetTextExtent(pm.text.c_str(), &extent)) && extent.cx > 0) {
                if (onMainMenu) {
                    drawX -= extent.cx;
                }
            }
            g_pMenuTextFont->DrawText(drawX, drawY, pm.color, pm.text.c_str());
        }
    }
    g_pendingMenuTexts.clear();
}

// Hook for sub_4728F0(int* a1, int a2, int a3, wchar_t* a4, ...). a2/a3 = x/y (may be int or float bits), a4 = wide format.
// Format with vswprintf, convert to UTF-8 for lookup; if found draw with our font and skip original.
int __cdecl Hook_MenuTextDraw(int* a1, int a2, int a3, char* a4, ...)
{
    // Game passes a4 as wchar_t* (UTF-16LE)
    const wchar_t* formatW = (const wchar_t*)a4;
    // a2 / a3 are actually integer coordinates (DWORD), not float bit patterns.
    // Treat them as ints and convert to float for our mapping.
    float fx = static_cast<float>(a2);
    float fy = static_cast<float>(a3);

    // Never call the original: it is variadic and we cannot pass ... from C. Calling it with
    // only (a1,a2,a3,a4) corrupts stack/state and breaks logging. We always draw ourselves.
    if (!formatW || !formatW[0]) {
        return 0;
    }

    va_list ap;
    va_start(ap, a4);
    wchar_t wbuf[512];
    int n = _vsnwprintf_s(wbuf, 512, _TRUNCATE, formatW, ap);
    va_end(ap);

    if (n < 0 || n >= 512) {
        return 0;
    }
    wbuf[n] = L'\0';

    // Convert to UTF-8 for translation lookup
    char utf8buf[1024];
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, utf8buf, (int)sizeof(utf8buf), nullptr, nullptr);
    if (utf8Len <= 0) {
        return 0;
    }
    std::string key(utf8buf);

    std::wstring textToDraw;
    auto it = g_translations.find(key);
    if (it != g_translations.end()) {
        AppendHookRecord("MenuTextDraw", key, "Translated");
        int wsize = MultiByteToWideChar(CP_UTF8, 0, it->second.c_str(), -1, nullptr, 0);
        if (wsize > 0) {
            textToDraw.resize(wsize, L'\0');
            MultiByteToWideChar(CP_UTF8, 0, it->second.c_str(), -1, &textToDraw[0], wsize);
        }
    }
    if (textToDraw.empty()) {
        AppendHookRecord("MenuTextDraw", key, "Original");
        textToDraw = wbuf;
    }

    if (!textToDraw.empty()) {
        PendingMenuText pm = {};
        pm.x = fx;
        pm.y = fy;
        pm.text = std::move(textToDraw);
        pm.color = g_dwCurrentColor;
        g_pendingMenuTexts.push_back(std::move(pm));
    }

    return 0;
}

static HRESULT WINAPI Hook_EndScene(IDirect3DDevice9* pDevice)
{
    DrawPendingChineseMenuText();
    return g_OriginalEndScene(pDevice);
}

void HookIRBRGameWriteText()
{
    static bool hookInstalled = false;
    if (hookInstalled) return;
    
    if (!g_pRBRGame) return;
    
    hookInstalled = true;
    g_pGameInstance = g_pRBRGame;
    StartHookRecordFile();
    
    // Create fonts for each IRBRGame font size, scaled by resolution.
    // Base heights are designed for 480px (RBR's native 640x480).
    if (g_pRBRIDirect3DDevice9) {
        const int baseFontHeights[IRBRGame::FONT_HEADING + 1] = {
            g_fontSizeSmall,
            g_fontSizeBig,
            g_fontSizeDebug,
            g_fontSizeHeading
        };

        float fontScale = static_cast<float>(g_rectRBRWndClient.bottom) / 480.0f;

        for (int i = IRBRGame::FONT_SMALL; i <= IRBRGame::FONT_HEADING; ++i) {
            if (!g_pChineseFonts[i]) {
                int scaledHeight = max(8, static_cast<int>(baseFontHeights[i] * fontScale + 0.5f));
                g_pChineseFonts[i] = new CD3DFont(g_fontFamily.c_str(), scaledHeight, 0L);
                g_pChineseFonts[i]->InitDeviceObjects(g_pRBRIDirect3DDevice9);
                g_pChineseFonts[i]->RestoreDeviceObjects();
            }
        }
    }
    
    // Get vtable and hook functions.
    // IRBRGame vtable order (from IRBRGame.h):
    // 0: StartGame
    // 1: WriteGameMessage
    // 2: WriteText
    // ...
    // 10: SetFont
    // 11: SetMenuColor
    // 12: SetColor
    void** vtable = *(void***)g_pRBRGame;

    g_WriteTextAddr     = vtable[2];
    g_SetFontAddr       = vtable[10];
    g_SetMenuColorAddr  = vtable[11];
    g_SetColorAddr      = vtable[12];

    if (MH_CreateHook(g_WriteTextAddr, &Hook_WriteText, (LPVOID*)&g_OriginalWriteText) != MH_OK) return;
    if (MH_EnableHook(g_WriteTextAddr) != MH_OK) return;

    if (MH_CreateHook(g_SetFontAddr, &Hook_SetFont, (LPVOID*)&g_OriginalSetFont) != MH_OK) return;
    if (MH_EnableHook(g_SetFontAddr) != MH_OK) return;

    if (MH_CreateHook(g_SetMenuColorAddr, &Hook_SetMenuColor, (LPVOID*)&g_OriginalSetMenuColor) != MH_OK) return;
    if (MH_EnableHook(g_SetMenuColorAddr) != MH_OK) return;

    if (MH_CreateHook(g_SetColorAddr, &Hook_SetColor, (LPVOID*)&g_OriginalSetColor) != MH_OK) return;
    if (MH_EnableHook(g_SetColorAddr) != MH_OK) return;

    // Dedicated font for sub_4728F0 menu text, scaled by resolution
    if (g_pRBRIDirect3DDevice9 && !g_pMenuTextFont) {
        float fontScale = static_cast<float>(g_rectRBRWndClient.bottom) / 480.0f;
        int scaledMenuHeight = max(8, static_cast<int>(g_fontSizeMenu * fontScale + 0.5f));
        g_pMenuTextFont = new CD3DFont(g_fontFamily.c_str(), scaledMenuHeight, 0L);
        g_pMenuTextFont->InitDeviceObjects(g_pRBRIDirect3DDevice9);
        g_pMenuTextFont->RestoreDeviceObjects();
    }

    {
        LPVOID addr = (LPVOID)C_RBR_ADDR_TO_POINTER(0x4728F0);
        if (MH_CreateHook(addr, &Hook_MenuTextDraw, (LPVOID*)&g_OriginalMenuTextDraw) != MH_OK) return;
        if (MH_EnableHook(addr) != MH_OK) return;
    }

    // Hook IDirect3DDevice9::EndScene (vtable slot 42) to flush pending text
    if (g_pRBRIDirect3DDevice9) {
        void** d3dVtable = *(void***)g_pRBRIDirect3DDevice9;
        void* endSceneAddr = d3dVtable[42];
        if (MH_CreateHook(endSceneAddr, &Hook_EndScene, (LPVOID*)&g_OriginalEndScene) != MH_OK) return;
        if (MH_EnableHook(endSceneAddr) != MH_OK) return;
    }
}

void UnhookIRBRGameWriteText()
{
    if (g_WriteTextAddr)    MH_DisableHook(g_WriteTextAddr);
    if (g_SetFontAddr)      MH_DisableHook(g_SetFontAddr);
    if (g_SetMenuColorAddr) MH_DisableHook(g_SetMenuColorAddr);
    if (g_SetColorAddr)     MH_DisableHook(g_SetColorAddr);
    if (g_OriginalMenuTextDraw) {
        LPVOID addr = (LPVOID)C_RBR_ADDR_TO_POINTER(0x4728F0);
        MH_DisableHook(addr);
        g_OriginalMenuTextDraw = nullptr;
    }
    if (g_OriginalEndScene) {
        void** d3dVtable = *(void***)g_pRBRIDirect3DDevice9;
        MH_DisableHook(d3dVtable[42]);
        g_OriginalEndScene = nullptr;
    }
    if (g_pMenuTextFont) {
        g_pMenuTextFont->InvalidateDeviceObjects();
        g_pMenuTextFont->DeleteDeviceObjects();
        delete g_pMenuTextFont;
        g_pMenuTextFont = nullptr;
    }
    for (int i = IRBRGame::FONT_SMALL; i <= IRBRGame::FONT_HEADING; ++i) {
        if (g_pChineseFonts[i]) {
            g_pChineseFonts[i]->InvalidateDeviceObjects();
            g_pChineseFonts[i]->DeleteDeviceObjects();
            delete g_pChineseFonts[i];
            g_pChineseFonts[i] = nullptr;
        }
    }
    StopHookRecordFile();
}

static void LoadTranslationFile(const fs::path& filePath)
{
    if (!fs::exists(filePath)) return;

    CSimpleIniA simpleIni;
    simpleIni.SetUnicode();
    if (simpleIni.LoadFile(filePath.string().c_str()) != SI_OK) return;

    CSimpleIniA::TNamesDepend keys;
    simpleIni.GetAllKeys("Translations", keys);

    for (const auto& key : keys) {
        const char* value = simpleIni.GetValue("Translations", key.pItem, "");
        if (value && value[0]) {
            std::string normKey = NormalizeIniToken(std::string(key.pItem ? key.pItem : ""));
            std::string normVal = NormalizeIniToken(std::string(value));
            if (!normKey.empty() && !normVal.empty()) {
                g_translations[normKey] = normVal;
            }
        }
    }
}

void LoadTranslations()
{
    fs::path rootPath = fs::path(GetRBRRootPath());

    // Read config from RichardBurnsRally.ini [RBRi18n]
    std::string lang = "zh";
    fs::path iniPath = rootPath / "RichardBurnsRally.ini";
    if (fs::exists(iniPath)) {
        CSimpleIniA ini;
        ini.SetUnicode();
        if (ini.LoadFile(iniPath.string().c_str()) == SI_OK) {
            const char* val = ini.GetValue("RBRi18n", "Language", "zh");
            if (val && val[0]) lang = NormalizeIniToken(std::string(val));
            g_hookRecordEnabled = (ini.GetLongValue("RBRi18n", "HookRecordEnabled", 1) != 0);
            g_hookRecordOnlyUntranslated = (ini.GetLongValue("RBRi18n", "HookRecordOnlyUntranslated", 0) != 0);

            // Font config (optional, defaults kept if not set)
            const char* ff = ini.GetValue("RBRi18n", "FontFamily", nullptr);
            if (ff && ff[0]) {
                std::string s = NormalizeIniToken(std::string(ff));
                g_fontFamily = std::wstring(s.begin(), s.end());
            }
            g_fontSizeSmall   = static_cast<int>(ini.GetLongValue("RBRi18n", "FontSizeSmall",   g_fontSizeSmall));
            g_fontSizeBig     = static_cast<int>(ini.GetLongValue("RBRi18n", "FontSizeBig",     g_fontSizeBig));
            g_fontSizeDebug   = static_cast<int>(ini.GetLongValue("RBRi18n", "FontSizeDebug",   g_fontSizeDebug));
            g_fontSizeHeading = static_cast<int>(ini.GetLongValue("RBRi18n", "FontSizeHeading", g_fontSizeHeading));
            g_fontSizeMenu    = static_cast<int>(ini.GetLongValue("RBRi18n", "FontSizeMenu",    g_fontSizeMenu));
        }
    }

    // Load translation files: RBRi18n/*.{lang}
    fs::path i18nDir = rootPath / "RBRi18n";
    if (!fs::exists(i18nDir)) return;

    std::string ext = "." + lang;
    for (const auto& entry : fs::directory_iterator(i18nDir)) {
        if (entry.is_regular_file() && entry.path().extension().string() == ext) {
            LoadTranslationFile(entry.path());
        }
    }
}
