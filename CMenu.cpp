#include "CMenu.h"
#include "SDK.h"
#include "CDrawManager.h"
#include "Colors.h"
#include "CConfig.h"
#include <cstring>
CCheatMenu gCheatMenu;
CMenuConfig gMenuVar;
int ScreenH;
int ScreenW;

char* szKeyNames[] = {
	"None", "Mouse 1", "Mouse 2", "Mouse 3", "Mouse 4", "Mouse 5", "Shift", "Alt", "F",
};

char* szHealthModes[] = {
	"OFF", "Text", "Bar", "Both",
};

char* szBoneModes[] = {
	"OFF", "White", "Health", "Team",
};

char* szHitboxes[] =
{
	"Auto", "Head", "Pelvis", "Lower Torse", "Lower Mid. Torse", "Upper Mid. Torse", "Upper Torse",
	"Left Upper Arm", "Left Lower Arm", "Left Hand", "Right Upper Arm",
	"Right Lower Arm", "Right Hand", "Left Hip", "Left Knee", "Left Foot",
	"Right Hip", "Right Knee", "Right Foot",
};

int CCheatMenu::AddItem(int nIndex, char szTitle[30], float* value, float flMin, float flMax, float flStep, bool isClassSwitch)
{
	strcpy(pMenu[nIndex].szTitle, szTitle);
	pMenu[nIndex].value = value;
	pMenu[nIndex].flMin = flMin;
	pMenu[nIndex].flMax = flMax;
	pMenu[nIndex].flStep = flStep;
	pMenu[nIndex].isClassSwitch = isClassSwitch;
	return (nIndex + 1);
}

void CCheatMenu::Render(void)
{
	gInts.Engine->GetScreenSize(ScreenW, ScreenH);

	int i = 0;

	i = AddItem(i, "Aimbot", &gMenuVar.aimbot_switch, 0, 1, 1, true);
	if (gMenuVar.aimbot_switch)
	{
		i = AddItem(i, " - Enabled", &gMenuVar.aimbot_active, 0, 1, 1, false);
		i = AddItem(i, " - Silent", &gMenuVar.aimbot_silent, 0, 1, 1, false);
		i = AddItem(i, " - Key", &gMenuVar.aimbot_key, 0, 8, 1, false);
		i = AddItem(i, " - Hitscan", &gMenuVar.aimbot_hitscan, 0, 1, 1, false);
		i = AddItem(i, " - Hitbox", &gMenuVar.aimbot_hitbox, 0, 18, 1, false);
		i = AddItem(i, " - Autoshoot", &gMenuVar.aimbot_autoshoot, 0, 1, 1, false);
	}

	i = AddItem(i, "Triggerbot", &gMenuVar.triggerbot_switch, 0, 1, 1, true);
	if (gMenuVar.triggerbot_switch)
	{
		i = AddItem(i, " - Enabled", &gMenuVar.triggerbot_active, 0, 1, 1, false);
		i = AddItem(i, " - Key", &gMenuVar.triggerbot_key, 0, 8, 1, false);
		i = AddItem(i, " - Head Only", &gMenuVar.triggerbot_headonly, 0, 1, 1, false);
	}

	i = AddItem(i, "Player List", &gMenuVar.playerlist_switch, 0, 1, 1, true);
	for (int p = 1; p <= gInts.Engine->GetMaxClients(); p++)
	{
		if (p == me)
			continue;

		CBaseEntity *pPlayer = GetBaseEntity(p);

		if (pPlayer == NULL)
			continue;

		player_info_t pInfo;
		gInts.Engine->GetPlayerInfo(p, &pInfo);

		char szString[256];
		sprintf(szString, " - %s", pInfo.name);

		if (gMenuVar.playerlist_switch)
			i = AddItem(i, szString, &gMenuVar.PlayerMode[pPlayer->GetIndex()], 0, 2, 1, false);
	}


	i = AddItem(i, "ESP", &gMenuVar.esp_switch, 0, 1, 1, true);
	if (gMenuVar.esp_switch)
	{
		i = AddItem(i, " - Enabled", &gMenuVar.esp_active, 0, 1, 1, false);
		i = AddItem(i, " - Enemies Only", &gMenuVar.esp_enemyonly, 0, 1, 1, false);
		i = AddItem(i, " - Box", &gMenuVar.esp_box, 0, 1, 1, false);
		i = AddItem(i, " - Name", &gMenuVar.esp_name, 0, 1, 1, false);
		i = AddItem(i, " - Class", &gMenuVar.esp_class, 0, 1, 1, false);
		i = AddItem(i, " - Health", &gMenuVar.esp_health, 0, 3, 1, false);
		i = AddItem(i, " - Bones", &gMenuVar.esp_bones, 0, 3, 1, false);
	}

	i = AddItem(i, "Settings", &gMenuVar.settings_switch, 0, 1, 1, true);
	if (gMenuVar.settings_switch)
	{
		i = AddItem(i, " - Menu Postion X", &gMenuVar.iMenu_Pos_X, 0, ScreenW, 25, false);
		i = AddItem(i, " - Menu Postion Y", &gMenuVar.iMenu_Pos_Y, 0, ScreenH, 25, false);
	}

	i = AddItem(i, "Misc", &gMenuVar.misc_switch, 0, 1, 1, true);
	if (gMenuVar.misc_switch)
	{
		i = AddItem(i, " - Bunnyhop", &gMenuVar.misc_bunnyhop, 0, 1, 1, false);
		i = AddItem(i, " - Autostrafe", &gMenuVar.misc_autostrafe, 0, 1, 1, false);
		i = AddItem(i, " - Noisemaker Spam", &gMenuVar.misc_noisemaker_spam, 0, 1, 1, false);
		i = AddItem(i, " - Anti Anti Aim", &gMenuVar.misc_anti_anti_aim, 0, 1, 1, false);

		if (gMenuVar.misc_anti_anti_aim) {
			i = AddItem(i, " - Allow Unsafe Pitch Correction", &gMenuVar.misc_anti_anti_aim_unsafe_x, 0, 1, 1, false);
		}
	}

	iMenuItems = i;
}
#include <SDL2/SDL.h>
const Uint8 *keystate = SDL_GetKeyboardState(NULL);
void CCheatMenu::HandleControls(void)
{
    
    //if (eventcode == 1)
	//{
	if (keystate[SDL_SCANCODE_INSERT]) //insert
	{
		gCheatMenu.bMenuActive = !gCheatMenu.bMenuActive;
    }

	if (gCheatMenu.bMenuActive)
	{
		if (keystate[SDL_SCANCODE_UP]) // Up
		{

			if (gCheatMenu.iMenuIndex > 0) gCheatMenu.iMenuIndex--;
			else gCheatMenu.iMenuIndex = gCheatMenu.iMenuItems - 1;
			//return 0;
		}
		if (keystate[SDL_SCANCODE_DOWN]) // Down
		{
			if (gCheatMenu.iMenuIndex < gCheatMenu.iMenuItems - 1) gCheatMenu.iMenuIndex++;
			else gCheatMenu.iMenuIndex = 0;
			//return 0;
		}
		if (keystate[SDL_SCANCODE_LEFT]) // Left
		{
			if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value)
			{
				gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] -= gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flStep;
				if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] < gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMin)
					gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] = gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMax;
			}
		}
		if (keystate[SDL_SCANCODE_RIGHT]) // Right
		{
			if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value)
			{
				gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] += gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flStep;
				if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] > gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMax)
					gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] = gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMin;
			}
		}

	}
	//}
}

void CCheatMenu::DrawMenu(void)
{
	int x = gMenuVar.iMenu_Pos_X,
		xx = x + 150,
		y = gMenuVar.iMenu_Pos_Y,
		w = 200,
		h = 14;

	CBaseEntity *pLocal = GetBaseEntity(me);

	int clrColor = COLORBLUTM;

	gDrawManager.DrawRect(x, y - (h + 4), w, iMenuItems * h + 21, COLORCODE(20, 20, 20, 128));
	gDrawManager.OutlineRect(x, y - (h + 4), w, (h + 4), clrColor);

	gDrawManager.DrawRect(x + 2, y - (h + 4), w - 4, (h + 4), clrColor);
	gDrawManager.OutlineRect(x - 1, y - (h + 4) - 1, w + 2, (h + 4), COLORCODE(0, 0, 0, 255)); // test
	gDrawManager.OutlineRect(x + 1, y - (h + 4) + 1, w - 2, (h + 4), COLORCODE(0, 0, 0, 255)); // test

	gDrawManager.OutlineRect(x, y - (h + 4), w, iMenuItems * h + 21, clrColor);

	gDrawManager.OutlineRect(x - 1, (y - (h + 4)) - 1, w + 2, (iMenuItems * h + 21) + 2, COLORCODE(0, 0, 0, 255));
	gDrawManager.OutlineRect(x + 1, (y - (h + 4)) + 1, w - 2, (iMenuItems * h + 21) - 2, COLORCODE(0, 0, 0, 255));

	gDrawManager.DrawString(x + 4, y - 16, clrColor, "Polly.xyz");

	for (int i = 0; i < iMenuItems; i++)
	{
		if (i != iMenuIndex)
		{
			if (pMenu[i].isClassSwitch)
			{
				if (!(pMenu[i].value[0]))
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[+] %s", pMenu[i].szTitle);
				}
				else if (pMenu[i].value[0])
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[-] %s", pMenu[i].szTitle);
				}
			}
			else
			{
				gDrawManager.DrawString(x + 4, y + (h * i), COLORWHITE, pMenu[i].szTitle);

				if (!strcmp(pMenu[i].szTitle, " - Health"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? COLORWHITE : COLORCODE(105, 105, 105, 255), "%s", szHealthModes[(int)pMenu[i].value[0]]);
				}

				else if (!strcmp(pMenu[i].szTitle, " - Bones"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? COLORWHITE : COLORCODE(105, 105, 105, 255), "%s", szBoneModes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 18)
				{
					gDrawManager.DrawString(xx, y + (h * i), COLORWHITE, "%s", szHitboxes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 8)
				{
					gDrawManager.DrawString(xx, y + (h * i), COLORWHITE, "%s", szKeyNames[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 2)
				{
					gDrawManager.DrawString(xx, y + (h * i), COLORWHITE, !pMenu[i].value[0] ? "Ignore" : pMenu[i].value[0] == 1 ? "Normal" : "Rage");
				}

				else if (pMenu[i].flMax == 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? COLORWHITE : COLORCODE(105, 105, 105, 255), pMenu[i].value[0] ? "ON" : "OFF");
				}

				else if (pMenu[i].value[0] >= 1 && pMenu[i].flMax > 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] >= 1 ? COLORWHITE : COLORCODE(105, 105, 105, 255), "%0.0f", pMenu[i].value[0]);
				}
			}
		}
		else
		{
			gDrawManager.DrawRect(x + 1, y + (h * i), w - 2, h, COLORCODE(255, 255, 255, 80));

			if (pMenu[i].isClassSwitch)
			{
				if (!(pMenu[i].value[0]))
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[+] %s", pMenu[i].szTitle);
				}
				else if (pMenu[i].value[0])
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[-] %s", pMenu[i].szTitle);
				}
			}
			else
			{
				gDrawManager.DrawString(x + 4, y + (h * i), clrColor, pMenu[i].szTitle);

				if (!strcmp(pMenu[i].szTitle, " - Health"))
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szHealthModes[(int)pMenu[i].value[0]]);
				}

				else if (!strcmp(pMenu[i].szTitle, " - Bones"))
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szBoneModes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 18)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szHitboxes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 8)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szKeyNames[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 2)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, !pMenu[i].value[0] ? "Ignore" : pMenu[i].value[0] == 1 ? "Normal" : "Rage");
				}

				else if (pMenu[i].flMax == 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, pMenu[i].value[0] ? "ON" : "OFF");
				}

				else
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%0.0f", pMenu[i].value[0]);
				}
			}
		}
	}
}