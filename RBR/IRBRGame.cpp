#include "IRBRGame.h"
#include "RBRAPI.h"

using namespace std;

/*
#define GEN_BOX_LINE_MAKER 1253
#define GEN_BOX_LINE_MAKER_LONG 1254

void IRBRGame::drawBox(float x, float y, float width, float height)
{
	// Calculate the number of texture repetitions needed
	int numTextureRepetitionsX = static_cast<int>(width);
	int numTextureRepetitionsY = static_cast<int>(height);

	// Draw the repeated 1x1 texture to form the box shape
	for (int i = 0; i < numTextureRepetitionsX; ++i)
	{
		DrawBox(GEN_BOX_LINE_MAKER, x + i, y);
		DrawBox(GEN_BOX_LINE_MAKER, x + i, y + height);
	}

	for (int j = 0; j < numTextureRepetitionsY; ++j)
	{
		DrawBox(GEN_BOX_LINE_MAKER, x, y + j);
		DrawBox(GEN_BOX_LINE_MAKER, x + width, y + j);
	}
}

void IRBRGame::drawLine(float x1, float y1, float x2, float y2)
{
	// Calculate the differences in x and y coordinates
	float dx = x2 - x1;
	float dy = y2 - y1;

	// Determine the number of steps based on the longer distance
	int numSteps = static_cast<int>(max(std::abs(dx), std::abs(dy)));

	// Calculate the increments for each step
	float xIncrement = dx / numSteps;
	float yIncrement = dy / numSteps;

	// Draw the repeated 1x1 texture for each step to form the line shape
	float x = x1;
	float y = y1;
	for (int i = 0; i <= numSteps; ++i)
	{
		DrawBox(GEN_BOX_LINE_MAKER, x, y);
		x += xIncrement;
		y += yIncrement;
	}
}

void IRBRGame::drawLineThick(float x1, float y1, float x2, float y2)
{
	// Calculate the differences in x and y coordinates
	float dx = x2 - x1;
	float dy = y2 - y1;

	// Determine the number of steps based on the longer distance
	int numSteps = static_cast<int>(max(std::abs(dx), std::abs(dy)));

	// Calculate the increments for each step
	float xIncrement = dx / numSteps;
	float yIncrement = dy / numSteps;

	// Draw the repeated 1x1 texture for each step to form the line shape
	float x = x1;
	float y = y1;
	for (int i = 0; i <= numSteps; ++i)
	{
		DrawBox(GEN_BOX_LINE_MAKER_LONG, x, y);
		x += xIncrement;
		y += yIncrement;
	}
}
*/

// Draws a progress bar message. Progress is 0.0-1.0 and incomplete progress is drawn in red color and the completed in green color
void IRBRGame::DrawTextProgress(const char* text, float progress, int index)
{
	// Draw a progress message and a box percentage of the current progress (0.0 - 1.0f)
	int yPos = (g_pRBRGameMode->gameMode != 0x03 ? 250 : 20);

	SetColor(0, 0, 0, 1.0f);
	DrawFlatBox(320.0f, static_cast<float>(yPos + index * 20), 100.0f, 19.0f);
	//SetMenuColor(IRBRGame::MENU_SELECTION);
	if (progress < 1.0f)
	{
		// Incomplete progress. Draw red color
		if (progress < 0.0f)
			progress = 0.0f;

		SetColor(1.0f, 0.0f, 0, 1.0f);
	}
	else
	{
		// Completed progress. Draw gree color
		progress = 1.0f;
		SetColor(0, 1.0f, 0, 1.0f);
	}

	DrawFlatBox(320.0f, static_cast<float>(yPos + index * 20), 100.0f * progress, 19.0f);

	SetDrawMode(GFX_DRAW_TEXT_SHADOW /* | GFX_DRAW_CENTER_X | GFX_DRAW_CENTER_Y */);
	SetFont(IRBRGame::FONT_BIG);
	SetMenuColor(IRBRGame::MENU_TEXT);
	WriteText(320.0f, static_cast<float>(yPos + index * 20), text);
	ReSetDrawMode(GFX_DRAW_TEXT_SHADOW /* | GFX_DRAW_CENTER_X | GFX_DRAW_CENTER_Y */);
}
