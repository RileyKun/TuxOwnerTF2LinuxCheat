#include "SDK.h"


CBaseCombatWeapon* CBaseEntity::GetActiveWeapon()
{
	DYNVAR(pHandle, DWORD, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	return (CBaseCombatWeapon *)gInts.EntList->GetClientEntityFromHandle(pHandle.GetValue(this));
}