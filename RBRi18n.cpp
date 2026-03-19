#include "RBRi18n.h"
#include "RBR/RBRAPI.h"
#include "RBR/HackMenuText.h"

#define PLUGIN_NAME "RBRi18n"

RBRi18n* g_pRBRPlugin = nullptr;

RBRi18n::RBRi18n(IRBRGame* pGame) : m_pGame(pGame) {}
RBRi18n::~RBRi18n() { UnhookIRBRGameWriteText(); }

const char* RBRi18n::GetName(void)
{
    if (!g_pRBRGame) {
        g_pRBRGame = m_pGame;
        RBRAPI_InitializeObjReferences();
        LoadTranslations();
        HookIRBRGameWriteText();
    }
    return PLUGIN_NAME;
}

void RBRi18n::DrawFrontEndPage(void) {}
void RBRi18n::DrawResultsUI(void) {}
void RBRi18n::HandleFrontEndEvents(char, bool, bool, bool, bool, bool) {}
void RBRi18n::TickFrontEndPage(float) {}
void RBRi18n::StageStarted(int, const char*, bool) {}
void RBRi18n::HandleResults(float, float, float, const char*) {}
void RBRi18n::CheckPoint(float, int, const char*) {}
