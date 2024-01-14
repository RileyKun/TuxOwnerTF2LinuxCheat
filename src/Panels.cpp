#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"
#include "Features/ESP/ESP.h"
#include "CNetVars.h"
#include "CMenu.h"
#include "CSignature.h"
CScreenSize gScreenSize;
//===================================================================================
void Hooked_PaintTraverse( void *pPanels, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	try
	{
		VMTManager& hook = VMTManager::GetHook(pPanels); //Get a pointer to the instance of your VMTManager with the function GetHook.
		hook.GetMethod<void(*)(void *, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.

		static unsigned int vguiMatSystemTopPanel;
		static unsigned int vguiFocusOverlayPanel;
		/*
		if (vguiMatSystemTopPanel == 0)
		{
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if( szName[0] == 'M' && szName[3] == 'S') //Look for MatSystemTopPanel without using slow operations like strcmp or strstr.
			{
				vguiMatSystemTopPanel = vguiPanel;
				Intro();
			}
		}
		*/
		if (vguiFocusOverlayPanel == NULL)
		{											//FocusOverlayPanel
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if( szName[0] == 'F' && szName[5] == 'O' &&  szName[12] == 'P' )
			{
				vguiFocusOverlayPanel = vguiPanel;
				Intro();
			}
		}

		
		
		if ( vguiFocusOverlayPanel == vguiPanel ) //If we're on MatSystemTopPanel, call our drawing code.
		{
			/*
			Draw the menu Doesnt really matter where we draw it tho lol
			*/
			//if (gCheatMenu.misc_svpure)
			//}
			//This section will be called when the player is not at the menu game and can see the screen or not taking a screenshot.
			//gDrawManager.DrawString( 200, 200, gDrawManager.dwGetTeamColor(3), "Welcome to Darkstorm"); //Remove this if you want.

			gCheatMenu.HandleControls(); // call the Handle Controls function to handle controls of the menu.. Duh!
			if (gCheatMenu.bMenuActive)
			{
				gCheatMenu.DrawMenu();
				gCheatMenu.Render();
			}



			CBaseEntity* pBaseLocalEnt = gInts.EntList->GetClientEntity(me);  //Grab the local player's entity.

			if (pBaseLocalEnt == NULL) //Always check for null pointers.
				return;
			gESP.StartThisshit(pBaseLocalEnt);
			

		}
	}
	catch(...)
	{
		//Log::Fatal("Failed PaintTraverse");
	}
}
//===================================================================================
void Intro( void )
{
	try
	{
		gDrawManager.Initialize(); //Initalize the drawing class.
		gNetVars.Initialize(); // if we dont do this, we will crash.
		Log::Msg("Injection Successful"); //If the module got here without crashing, it is good day.

		Log::Debug("Height=%d, Width=%d",gScreenSize.iScreenHeight, gScreenSize.iScreenWidth);
	}
	catch(...)
	{
		//Log::Fatal("Failed Intro");
	}
}
