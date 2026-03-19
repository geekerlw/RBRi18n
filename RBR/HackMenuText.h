#pragma once

// Hook IRBRGame WriteText vtable to support Chinese text rendering
void HookIRBRGameWriteText();
void UnhookIRBRGameWriteText();

// Flush all pending translated menu texts (called from OnEndScene to draw on top)
void DrawPendingChineseMenuText();

// Load translations from RBRi18n/*.{lang} files based on Language setting in RichardBurnsRally.ini
void LoadTranslations();
