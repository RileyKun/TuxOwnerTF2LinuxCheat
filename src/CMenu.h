#pragma once
// pasted from potassium tf2 
// credits: gir and kalvin for this and source file

struct CMenuItems_t
{
	char szTitle[30];
	float* value;
	float flMin;
	float flMax;
	float flStep;
	bool isClassSwitch;
};

class CCheatMenu
{
public:
	void Render(void);
	void DrawMenu(void);
	int	 AddItem(int nIndex, char szTitle[30], float* value, float flMin, float flMax, float flStep, bool isClassHead);
    void HandleControls();
	//private:
	bool bMenuActive;
	int iMenuIndex;
	int iMenuItems;
	CMenuItems_t pMenu[120];
	int iAimbotIndex = -1;
	
	float settings_switch;
	float iMenu_Pos_X = 300;
	float iMenu_Pos_Y = 75;
	float szsubstest;

	float aimbot_switch;
	float aimbot_silent;
	float aimbot_active;
	//float aimbot_key;
	float aimbot_hitscan;
	float aimbot_hitbox;
	float aimbot_autoshoot;
	float aimbot_fov;

	float triggerbot_switch;
	float triggerbot_active;
	float triggerbot_key;
	float triggerbot_headonly;

	float esp_switch;
	float esp_active;
	float esp_enemyonly;
	float esp_box;
	float esp_name;
	float esp_class;
	float esp_health;
	float esp_bones;

	float misc_switch;
	float misc_bunnyhop;
	float misc_autostrafe;
	float misc_noisemaker_spam;
	float misc_anti_anti_aim;
	float misc_anti_anti_aim_unsafe_x;
	
	float playerlist_switch;
	float PlayerMode[64] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};
};

extern CCheatMenu gCheatMenu;