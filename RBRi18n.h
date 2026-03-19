#pragma once

#include "RBR/IPlugin.h"
#include "RBR/IRBRGame.h"

class RBRi18n : public IPlugin {
public:
    RBRi18n(IRBRGame* pGame);
    virtual ~RBRi18n();

    const char* GetName(void);
    void DrawFrontEndPage(void);
    void DrawResultsUI(void);
    void HandleFrontEndEvents(char txtKeyboard, bool bUp, bool bDown, bool bLeft, bool bRight, bool bSelect);
    void TickFrontEndPage(float fTimeDelta);
    void StageStarted(int iMap, const char* ptxtPlayerName, bool bWasFalseStart);
    void HandleResults(float fCheckPoint1, float fCheckPoint2, float fFinishTime, const char* ptxtPlayerName);
    void CheckPoint(float fCheckPointTime, int iCheckPointID, const char* ptxtPlayerName);

    IRBRGame* m_pGame;
};
