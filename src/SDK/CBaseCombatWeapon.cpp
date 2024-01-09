#include "../SDK.h"
int CBaseCombatWeapon::GetMaxClip1()
{
	typedef int(*OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 318)(this);
}
int CBaseCombatWeapon::GetMaxClip2()
{
	typedef int(*OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 319)(this);
}

int CBaseCombatWeapon::GetSlot()
{
	typedef int(*OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 327)(this);
}

char *CBaseCombatWeapon::GetName()
{
	typedef char *(*OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 329)(this);
}

char *CBaseCombatWeapon::GetPrintName()
{
	typedef char *(*OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 330)(this);
}

int CBaseCombatWeapon::GetItemDefinitionIndex()
{
	DYNVAR_RETURN(int, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
}