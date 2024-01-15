#pragma once

#include "windowsTypes.h"
#include "CGlobalVars.h"
#include "CSignature.h"
#include "Log.h"
#include "VMTHooks.h"
#include "Vector.h"
#include "WeaponList.h"
#include "dt_recv2.h"
#include "getvfunc.h"
#include <math.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <thread>
#include <wchar.h>
#include "CNetVars.h"
//#include "ConVar.h"
#include "SDK/studiohdr.h"
#include "SDK/Enums.h"
using namespace std;

typedef void *(*CreateInterface_t)(const char *, int *);
typedef void *(*CreateInterfaceFn)(const char *pName, int *pReturnCode);

using namespace toolkit;
class CNetVars;
typedef float matrix3x4[3][4];
class CGameTrace;
typedef CGameTrace trace_t;
class CBaseCombatWeapon;
#define me gInts.Engine->GetLocalPlayer()
#define GetBaseEntity gInts.EntList->GetClientEntity
#define MASK_AIMBOT 0x200400B
#define FL_ONGROUND (1 << 0)
#define FL_DUCKING (1 << 1)
#define CONTENTS_HITBOX 0x40000000
#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define PI 3.14159265358979323846f
#define DEG2RAD(x) ((float)(x) * (float)((float)(PI) / 180.0f))
#define RAD2DEG(x) ((float)(x) * (float)(180.0f / (float)(PI)))
#define square(x) (x * x)
#define RADPI 57.295779513082f
#define SQUARE(a) a *a
#define BLU_TEAM 3
#define RED_TEAM 2
struct old_movement_t {
	Vector angle;
	float fwd;
	float sdm;
};
typedef struct player_info_s
{
	char name[32];
	int userID;
	char guid[33];
	unsigned long friendsID;
	char friendsName[32];
	bool fakeplayer;
	bool ishltv;
	unsigned long customFiles[4];
	unsigned char filesDownloaded;
} player_info_t;

class ClientClass
{
private:
	uint8_t _chPadding[8];

public:
	char *chName;
	RecvTable *Table;
	ClientClass *pNextClass;
	int iClassID;
};

class CHLClient
{
public:
	ClientClass *GetAllClasses(void)
	{
		typedef ClientClass *(*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 8)(this); //Return the pointer to the head CClientClass.
	}
};

class CGlobals
{
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float curtime;
	float frametime;
	int maxclients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
};

class CUserCmd
{
public:
	virtual ~CUserCmd(){}; //Destructor 0
	int command_number;	//4
	int tick_count;		   //8
	Vector viewangles;	 //C
	float forwardmove;	 //18
	float sidemove;		   //1C
	float upmove;		   //20
	int buttons;		   //24
	uint8_t impulse;	   //28
	int weaponselect;	  //2C
	int weaponsubtype;	 //30
	int random_seed;	   //34
	short mousedx;		   //38
	short mousedy;		   //3A
	bool hasbeenpredicted; //3C;
};
class CBaseEntity
{
public: 

	Vector &GetAbsOrigin()
	{
    	typedef Vector &(*OriginalFn)(void *);
    	return getvfunc<OriginalFn>(this, 11)(this);
	}

	bool IsDormant()
	{
		void* pNetworkable = (void*)(this + 0x8);
		typedef bool(*OriginalFn)(void*);
		return getvfunc<OriginalFn>(pNetworkable, 8)(pNetworkable);
	}
	Vector &GetAbsAngles()
	{
    	typedef Vector &(*OriginalFn)(void *);
    	return getvfunc<OriginalFn>(this, 12)(this);
	}
	Vector &GetShootPos() // DONT ASK, I DONT KNOW IF THIS WILL WORK! (Pasted from 8dcc's tf2 cheat)
	{
    	typedef Vector &(*OriginalFn)(void *);
    	return getvfunc<OriginalFn>(this, 302)(this);
	}
	void GetWorldSpaceCenter(Vector &vWorldSpaceCenter)
	{
    	Vector vMin, vMax;
    	this->GetRenderBounds(vMin, vMax);
    	vWorldSpaceCenter = this->GetAbsOrigin();
    	vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2;
	}
	bool SetupBones(matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		void* pRenderable = (void*)(this + 0x4);
		typedef bool(*OriginalFn)(void *, matrix3x4*, int, int, float);
		return getvfunc<OriginalFn>(pRenderable, 16)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
	ClientClass *GetClientClass()
	{
    	typedef ClientClass *(*OriginalFn)(void *);
    	return getvfunc<OriginalFn>(this, 17)(this);
	}
	bool IsAlive() /*From 8dcc/tf2-cheat*/
	{
    	typedef bool (*OriginalFn)(void *);
    	return getvfunc<OriginalFn>(this, 183)(this);
	}
	int GetIndex()
	{
    	typedef int (*OriginalFn)(void *);
    	return getvfunc<OriginalFn>(this, 79)(this);
	}
	int GetHealth() /* From 8dcc/tf2-cheat */ /* there's a netvar for it but im too lazy.*/
	{
    	typedef int (*OriginalFn)(void *);
    	return getvfunc<OriginalFn>(this, 152)(this);
	}
	void GetRenderBounds(Vector &mins, Vector &maxs)
	{
    	typedef void (*OriginalFn)(void *, Vector &, Vector &);
    	getvfunc<OriginalFn>(this, 60)(this, mins, maxs);
	}
	int GetCond()
	{
		DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerCond");
	}
	Vector GetEyePosition()
	{
		DYNVAR_RETURN(Vector, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]") + this->GetAbsOrigin();
	}
	int GetTeamNum()
	{
		DYNVAR_RETURN(int, this, "DT_BaseEntity", "m_iTeamNum");
	}
	int GetClassNum()
	{
		DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_PlayerClass", "m_iClass");
	}
	Vector GetVecOrigin()
	{
		DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_vecOrigin");
	}
	int GetHitboxSet()
	{
		DYNVAR_RETURN(int, this, "DT_BaseAnimating", "m_nHitboxSet");
	}
	int GetFlags()
	{
		DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_fFlags");
	}
	void GetRenderable() 
	{
    	//return (Renderable*)((uint32_t)ent + 0x4);
		void* pRenderable = (void*)(this + 0x4);
		return pRenderable;
	}
	matrix3x4& GetRgflCoordinateFrame()
	{
		void* pRenderable = static_cast<void*>(this + 0x4);
		typedef matrix3x4 &(*OriginalFn)(void *);
		return getvfunc<OriginalFn>(pRenderable, 34)(pRenderable);
	}

	int GetMaxHealth()
	{
		typedef int(*OriginalFn)(void*);	
		return getvfunc<OriginalFn>(this, 107)(this);
	}
	Vector GetCollideableMins()
	{
		DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_Collision", "m_vecMins");
	}

	Vector GetCollideableMaxs()
	{
		DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_Collision", "m_vecMaxs");
	}
	//m_vecOrigin          = gNetvars.get_offset("DT_BaseEntity", "m_vecOrigin");
	//Vector GetHitboxPosition(CBaseEntity* pEntity, int iHitbox);
	uintptr_t* GetModel()
	{
		void* pRenderable = (void*)(this + 0x4);
		typedef uintptr_t* (*OriginalFn)(void*);
		return getvfunc<OriginalFn>(pRenderable, 9)(pRenderable);
	}
	/*
	CBaseCombatWeapon* CBaseEntity::GetActiveWeapon()
	{
		DYNVAR(pHandle, DWORD, "DT_BaseCombatCharacter", "m_hActiveWeapon");
		return (CBaseCombatWeapon *)gInts.EntList->GetClientEntityFromHandle(pHandle.GetValue(this));
	}
	*/
	char* szGetClass()
	{
		DYNVAR(iClass, int, "DT_TFPlayer", "m_PlayerClass", "m_iClass");

		switch (iClass.GetValue(this))
		{
			case TF2_Scout:
				return "Scout";
			case TF2_Soldier:
				return "Soldier";
			case TF2_Pyro:
				return "Pyro";
			case TF2_Demoman:
				return "Demoman";
			case TF2_Heavy:
				return "Heavy";
			case TF2_Engineer:
				return "Engineer";
			case TF2_Medic:
				return "Medic";
			case TF2_Sniper:
				return "Sniper";
			case TF2_Spy:
				return "Spy";
			default:
				return "Unknown class";
		}

		return "Unknown class"; //Just in case
	}
	void setfov(int fov) // From Dumpster fire
	{
		DYNVAR(n, int, "DT_BasePlayer", "m_iFOV");
		DYNVAR(n2, int, "DT_BasePlayer", "m_iDefaultFOV");
		n.SetValue(this, fov);
		n2.SetValue(this, fov);
	}
	void ForceTauntCam(bool t) // From Dumpster fire
	{
		DYNVAR(n, bool, "DT_TFPlayer", "m_nForceTauntCam");
		return n.SetValue(this, t);
	}
	float m_flLastFireTime()
	{
		DYNVAR_RETURN(float, this, "DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flLastFireTime");
	}
	bool IsScoped()
	{
		return (GetCond() & TFCond_Zoomed) != 0;
	}
	bool ReadyToBackstab()
	{
		DYNVAR_RETURN(bool, this, "DT_TFWeaponKnife", "m_bReadyToBackstab");
	}
	Vector get_hitbox_pos(int hitbox_idx);
	CBaseCombatWeapon* GetActiveWeapon();

};
/*
enum hitboxes
{
    HITBOX_HEAD = 0,
    HITBOX_PELVIS = 1,
    HITBOX_spine_0 = 2,
    HITBOX_spine_1 = 3,
    HITBOX_spine_2 = 4,
    HITBOX_spine_3 = 5,
    HITBOX_upperArm_L = 6,
    lowerArm_L = 7,
    hand_L = 8,
    upperArm_R = 9,
    lowerArm_R = 10,
    hand_R = 11,
    hip_L = 12,
    knee_L = 13,
    foot_L = 14,
    hip_R = 15,
    knee_R = 16,
    foot_R = 17,	
};
*/
class EngineClient
{
public:
	void GetScreenSize(int &width, int &height)
	{
		typedef void (*OriginalFn)(void *, int &, int &);
		return getvfunc<OriginalFn>(this, 5)(this, width, height);
	}
	bool GetPlayerInfo(int ent_num, player_info_t *pinfo)
	{
		typedef bool (*OriginalFn)(void *, int, player_info_t *);
		return getvfunc<OriginalFn>(this, 8)(this, ent_num, pinfo);
	}
	bool Con_IsVisible(void)
	{
		typedef bool (*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 11)(this);
	}
	int GetLocalPlayer(void)
	{
		typedef int (*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 12)(this);
	}
	float Time(void)
	{
		typedef float (*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 14)(this);
	}
	void GetViewAngles(Vector &va)
	{
		typedef void (*OriginalFn)(void *, Vector &va);
		return getvfunc<OriginalFn>(this, 19)(this, va);
	}
	void SetViewAngles(Vector &va)
	{
		typedef void (*OriginalFn)(void *, Vector &va);
		return getvfunc<OriginalFn>(this, 20)(this, va);
	}
	int GetPlayerForUserID(int va)
	{
		typedef bool*(*OriginalFn)(void *, int va);
		return getvfunc<OriginalFn>(this, 9)(this, va);
	}
	int GetMaxClients(void)
	{
		typedef int (*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 21)(this);
	}
	bool IsInGame(void)
	{
		typedef bool (*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 26)(this);
	}
	bool IsConnected(void)
	{
		typedef bool (*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 27)(this);
	}
	bool IsDrawingLoadingImage(void)
	{
		typedef bool (*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 28)(this);
	}
	const matrix3x4 &WorldToScreenMatrix(void)
	{
		typedef const matrix3x4 &(*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 36)(this);
	}
	bool IsTakingScreenshot(void)
	{
		typedef bool (*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 85)(this);
	}
	void *GetNetChannelInfo(void)
	{
		typedef void *(*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 72)(this);
	}
	void ClientCmd_Unrestricted(const char *chCommandString)
	{
		typedef void (*OriginalFn)(void *, const char *);
		return getvfunc<OriginalFn>(this, 106)(this, chCommandString);
	}
};

class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char *(*OriginalFn)(void *, unsigned int);
		return getvfunc<OriginalFn>(this, 37)(this, vguiPanel);
	}
};

class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void (*OriginalFn)(void *, int, int, int, int);
		getvfunc<OriginalFn>(this, 10)(this, r, g, b, a);
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void (*OriginalFn)(void *, int, int, int, int);
		getvfunc<OriginalFn>(this, 12)(this, x0, y0, x1, y1);
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void (*OriginalFn)(void *, int, int, int, int);
		getvfunc<OriginalFn>(this, 14)(this, x0, y0, x1, y1);
	}
	void DrawSetTextFont(unsigned long font)
	{
		typedef void (*OriginalFn)(void *, unsigned long);
		getvfunc<OriginalFn>(this, 17)(this, font);
	}
	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void (*OriginalFn)(void *, int, int, int, int);
		getvfunc<OriginalFn>(this, 18)(this, r, g, b, a);
	}
	void DrawSetTextPos(int x, int y)
	{
		typedef void (*OriginalFn)(void *, int, int);
		getvfunc<OriginalFn>(this, 20)(this, x, y);
	}
	void DrawPrintText(const wchar_t *text, int textLen)
	{
		typedef void (*OriginalFn)(void *, const wchar_t *, int, int);
		return getvfunc<OriginalFn>(this, 22)(this, text, textLen, 0);
	}
	void DrawPrintText(const char *text, int textLen)
	{
		typedef void (*OriginalFn)(void *, const char *, int, int);
		return getvfunc<OriginalFn>(this, 22)(this, text, textLen, 0);
	}
	unsigned long CreateFont()
	{
		typedef unsigned int (*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 66)(this);
	}
	void SetFontGlyphSet(unsigned long &font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void (*OriginalFn)(void *, unsigned long, const char *, int, int, int, int, int, int, int);
		getvfunc<OriginalFn>(this, 67)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	void GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void (*OriginalFn)(void *, unsigned long, const wchar_t *, int &, int &);
		getvfunc<OriginalFn>(this, 75)(this, font, text, wide, tall);
	}
	void DrawSetTextureRGBA(int textureID, unsigned char  const* colors, int w, int h)
	{
		typedef void*(*oDrawSetTextColor)(void*, int, unsigned char  const*, int, int);
		return getvfunc<oDrawSetTextColor>(this, 37)(this, textureID, colors, w, h);
	}
	int CreateNewTextureID(bool procedural)
	{
		typedef int(*oDrawSetTextColor)(void*, bool);
		return getvfunc<oDrawSetTextColor>(this, 43)(this, procedural);
	}

	void IPlaySound(const char* fileName)
	{
		typedef void*(*OriginalFn)(void*, const char*);
		return getvfunc<OriginalFn>(this, 82)(this, fileName);
	}


};

class IGameEvent
{
public:
	virtual ~IGameEvent()
	{
	};
	virtual const char *GetName() const = 0; // get event name

	virtual bool IsReliable() const = 0;				  // if event handled reliable
	virtual bool IsLocal() const = 0;					  // if event is never networked
	virtual bool IsEmpty(const char *keyName = NULL) = 0; // check if data field exists

														  // Data access
	virtual bool GetBool(const char *keyName = NULL, bool defaultValue = false) = 0;
	virtual int GetInt(const char *keyName = NULL, int defaultValue = 0) = 0;
	virtual float GetFloat(const char *keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char *GetString(const char *keyName = NULL, const char *defaultValue = "") = 0;

	virtual void SetBool(const char *keyName, bool value) = 0;
	virtual void SetInt(const char *keyName, int value) = 0;
	virtual void SetFloat(const char *keyName, float value) = 0;
	virtual void SetString(const char *keyName, const char *value) = 0;
};

class IGameEventListener2
{
public:
	virtual ~IGameEventListener2(void)
	{
	};

	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	virtual void FireGameEvent(IGameEvent *event) = 0;
};

/*
class IGameEventManager2
{
public:
	bool AddListener(IGameEventListener2 *listener, const char *name, bool bServerSide)
	{
		typedef bool(*OriginalFn)(void*, IGameEventListener2 *, const char *, bool);
		return getvfunc<OriginalFn>(this, 3)(this, listener, name, bServerSide);
	}
};
*/
class IGameEventManager2 {
public:
	virtual ~IGameEventManager2( void ) {};

	// load game event descriptions from a file eg "resource\gameevents.res"
	virtual int LoadEventsFromFile( const char* filename ) = 0;

	// removes all and anything
	virtual void Reset( ) = 0;

	// adds a listener for a particular event
	virtual bool AddListener( IGameEventListener2* listener, const char* name, bool bServerSide ) = 0;

	// returns true if this listener is listens to given event
	virtual bool FindListener( IGameEventListener2* listener, const char* name ) = 0;

	// removes a listener 
	virtual void RemoveListener( IGameEventListener2* listener ) = 0;

	// create an event by name, but doesn't fire it. returns NULL is event is not
	// known or no listener is registered for it. bForce forces the creation even if no listener is active
	virtual IGameEvent* CreateEvent( const char* name, bool bForce = false, int* pCookie = nullptr ) = 0;

	// fires a server event created earlier, if bDontBroadcast is set, event is not send to clients
	virtual bool FireEvent( IGameEvent* event, bool bDontBroadcast = false ) = 0;

	// fires an event for the local client only, should be used only by client code
	virtual bool FireEventClientSide( IGameEvent* event ) = 0;

	// create a new copy of this event, must be free later
	virtual IGameEvent* DuplicateEvent( IGameEvent* event ) = 0;

	// if an event was created but not fired for some reason, it has to bee freed, same UnserializeEvent
	virtual void FreeEvent( IGameEvent* event ) = 0;

	// write/read event to/from bitbuffer
	virtual bool SerializeEvent( IGameEvent* event, int* buf ) = 0;
	virtual IGameEvent* UnserializeEvent( void* buf ) = 0; // create new KeyValues, must be deleted
};


class CEntList
{
public:
	CBaseEntity *GetClientEntity(int entnum)
	{
		typedef CBaseEntity *(*OriginalFn)(void *, int);
		return getvfunc<OriginalFn>(this, 3)(this, entnum);
	}
	CBaseEntity *GetClientEntityFromHandle(int hEnt)
	{
		typedef CBaseEntity *(*OriginalFn)(void *, int);
		return getvfunc<OriginalFn>(this, 4)(this, hEnt);
	}
	int GetHighestEntityIndex(void)
	{
		typedef int (*OriginalFn)(void *);
		return getvfunc<OriginalFn>(this, 6)(this);
	}
};
/*
class CBaseCombatWeapon : public CBaseEntity
{
public:
	int GetMaxClip1()
	{
		typedef int(*OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 318)(this);
	}

	int GetMaxClip2()
	{
		typedef int(*OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 319)(this);
	}

	int GetSlot()
	{
		typedef int(*OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 398)(this); // was 327, according to cathook, its 398
	}

	char *GetName()
	{
		typedef char *(*OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 329)(this);
	}

	char *GetPrintName()
	{
		typedef char *(*OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 401)(this); // was 330, according to cathook its 401
	}

	int GetItemDefinitionIndex()
	{
		DYNVAR_RETURN(int, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
	}



	float ChargedDamage()
	{
		DYNVAR_RETURN(float, this, "DT_TFSniperRifle", "SniperRifleLocalData", "m_flChargedDamage");
	}

	
};
*/
class CHudChat
{
public:
    void ChatPrintf(int pindex, const char *fmt, ...)
    {
    	typedef void(*ChatPrintfFn)(void*,int, int, const char*, ...);
    	getvfunc<ChatPrintfFn>(this, 22)(this,pindex ,0, fmt); // was 19, now 22?
    }
};

class ClientModeShared
{
public:
	char pad[28];
	CHudChat* m_pChat;
	/*
	bool IsChatPanelOutOfFocus(void)
	{
		typedef void *(*OriginalFn)(void *);
		void *CHudChat = getvfunc<OriginalFn>(this, 19)(this);
		if(CHudChat)
		{
			return *(float *)((uintptr_t)CHudChat + 0xFC) == 0;
		}
		return false;
	}
	*/
};

class alignas(16)VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};

	inline VectorAligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

	explicit VectorAligned(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};

struct Ray_t
{
	VectorAligned   m_Start;
	VectorAligned   m_Delta;
	VectorAligned   m_StartOffset;
	VectorAligned   m_Extents;

	bool    m_IsRay;
	bool    m_IsSwept;

	void Init(Vector& start, Vector& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.Length2DSqr() != 0);

		m_Extents.Init();
		m_IsRay = true;

		m_StartOffset.Init();
		m_Start = start;
	}

	void Init(Vector& start, Vector& end, Vector& mins, Vector& maxs)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.Length2DSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.Length2DSqr() < 1e-6);

		m_StartOffset = mins + maxs;
		m_StartOffset *= 0.5f;
		m_Start = start - m_StartOffset;
		m_StartOffset *= -1.0f;
	}
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void *pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		CBaseEntity *pEntity = (CBaseEntity *)pEntityHandle;

		switch (pEntity->GetClientClass()->iClassID)
		{
		case 55: // Portal Window
		case 64: // Spawn Door visualizers
		case 117: // Sniper Dots
		case 225: // Medigun Shield
		case CClass_CObjectSentrygun:
			return false;
			break;
		}

		return !(pEntityHandle == pSkip);
	}

	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};

class CBaseTrace
{
public:
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}

private:
	CBaseTrace(const CBaseTrace& vOther);
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;

	bool DidHitNonWorldEntity() const;

	int GetEntityIndex() const;

	bool DidHit() const
	{
		return fraction < 1 || allsolid || startsolid;
	}

public:
	float			fractionleftsolid;
	csurface_t		surface;

	int				hitgroup;

	short			physicsbone;

	CBaseEntity*	m_pEnt;
	int				hitbox;

	CGameTrace() {}
	CGameTrace(const CGameTrace& vOther);
};


class IEngineTrace
{
public:	  //We really only need this I guess...
	void TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)//5
	{
		typedef void(*TraceRayFn)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		return getvfunc<TraceRayFn>(this, 4)(this, ray, fMask, pTraceFilter, pTrace);
	}
};

class IVModelInfo
{
public:
	uintptr_t *GetModel(int index)
	{
		typedef uintptr_t*(*GetModelFn)(void*, int);
		return getvfunc<GetModelFn>(this, 1)(this, index);
	}

	int	GetModelIndex(const char* name)
	{
		typedef int( * GetModelIndexFn)(void*, const char*);
		return getvfunc< GetModelIndexFn >(this, 3)(this, name); // 3
	}

	const char* GetModelName(const uintptr_t* model)
	{
		typedef const char* ( * GetModelNameFn)(void*, const uintptr_t*);
		return getvfunc< GetModelNameFn >(this, 3)(this, model);
	}

	studiohdr_t* GetStudiomodel(const uintptr_t *mod)
	{
		typedef studiohdr_t* ( * GetStudiomodelFn)(void*, const uintptr_t*);
		return getvfunc< GetStudiomodelFn >(this, 29)(this, mod); // 29 apparently old 28
	}
};
#include "SDK/ConVar.h"
class IAppSystem
{
public:
	// Here's where the app systems get to learn about each other
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;

	// Here's where systems can access other interfaces implemented by this object
	// Returns NULL if it doesn't implement the requested interface
	virtual void *QueryInterface(const char *pInterfaceName) = 0;

	// Init, shutdown
	virtual int Init() = 0;
	virtual void Shutdown(char* reason) = 0;

};

class ICvar : public IAppSystem
{
public:
	// Allocate a unique DLL identifier
	virtual int AllocateDLLIdentifier() = 0;

	// Register, unregister commands
	virtual void			RegisterConCommand(ConCommandBase *pCommandBase) = 0;
	virtual void			UnregisterConCommand(ConCommandBase *pCommandBase) = 0;
	virtual void			UnregisterConCommands(int id) = 0;

	// If there is a +<varname> <value> on the command line, this returns the value.
	// Otherwise, it returns NULL.
	virtual const char*		GetCommandLineValue(const char *pVariableName) = 0;

	// Try to find the cvar pointer by name
	virtual ConCommandBase *FindCommandBase(const char *name) = 0;
	virtual const ConCommandBase *FindCommandBase(const char *name) const = 0;
	virtual ConVar			*FindVar(const char *var_name) = 0;
	virtual const ConVar	*FindVar(const char *var_name) const = 0;
	virtual ConCommand		*FindCommand(const char *name) = 0;
	virtual const ConCommand *FindCommand(const char *name) const = 0;

	// Get first ConCommandBase to allow iteration
	virtual ConCommandBase	*GetCommands(void) = 0;
	virtual const ConCommandBase *GetCommands(void) const = 0;

	// Install a global change callback (to be called when any convar changes)
	virtual void			InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void			RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void			CallGlobalChangeCallbacks(ConVar *var, const char *pOldString, float flOldValue) = 0;

	// Install a console printer
	virtual void			InstallConsoleDisplayFunc(void* pDisplayFunc) = 0;
	virtual void			RemoveConsoleDisplayFunc(void* pDisplayFunc) = 0;
	virtual void			ConsoleColorPrintf(const int& clr, const char *pFormat, ...) const = 0;
	virtual void			ConsolePrintf(const char *pFormat, ...) const = 0;
	virtual void			ConsoleDPrintf(const char *pFormat, ...) const = 0;

	// Reverts cvars which contain a specific flag
	virtual void			RevertFlaggedConVars(int nFlag) = 0;

	// Method allowing the engine ICvarQuery interface to take over
	// A little hacky, owing to the fact the engine is loaded
	// well after ICVar, so we can't use the standard connect pattern
	virtual void			InstallCVarQuery(void *pQuery) = 0;

#if defined( _X360 )
	virtual void			PublishToVXConsole() = 0;
#endif
	virtual bool			IsMaterialThreadSetAllowed() const = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar *pConVar, const char *pValue) = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar *pConVar, int nValue) = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar *pConVar, float flValue) = 0;
	virtual bool			HasQueuedMaterialThreadConVarSets() const = 0;
	virtual int				ProcessQueuedMaterialThreadConVarSets() = 0;

protected:	class ICVarIteratorInternal;
public:
	/// Iteration over all cvars.
	/// (THIS IS A SLOW OPERATION AND YOU SHOULD AVOID IT.)
	/// usage:
	/// { ICVar::Iterator iter(g_pCVar);
	///   for ( iter.SetFirst() ; iter.IsValid() ; iter.Next() )
	///   {
	///       ConCommandBase *cmd = iter.Get();
	///   }
	/// }
	/// The Iterator class actually wraps the internal factory methods
	/// so you don't need to worry about new/delete -- scope takes care
	//  of it.
	/// We need an iterator like this because we can't simply return a
	/// pointer to the internal data type that contains the cvars --
	/// it's a custom, protected class with unusual semantics and is
	/// prone to change.
	class Iterator
	{
	public:
		inline Iterator(ICvar *icvar);
		inline ~Iterator(void);
		inline void		SetFirst(void);
		inline void		Next(void);
		inline bool		IsValid(void);
		inline ConCommandBase *Get(void);
	private:
		ICVarIteratorInternal *m_pIter;
	};

protected:
	// internals for  ICVarIterator
	class ICVarIteratorInternal
	{
	public:
		// warning: delete called on 'ICvar::ICVarIteratorInternal' that is abstract but has non-virtual destructor [-Wdelete-non-virtual-dtor]
		virtual ~ICVarIteratorInternal()
		{
		}
		virtual void		SetFirst(void) = 0;
		virtual void		Next(void) = 0;
		virtual	bool		IsValid(void) = 0;
		virtual ConCommandBase *Get(void) = 0;
	};

	virtual ICVarIteratorInternal	*FactoryInternalIterator(void) = 0;
	friend class Iterator;
};

class CInterfaces
{
public:
	CEntList *EntList;
	EngineClient *Engine;
	IPanel *Panels;
	ISurface *Surface;
	ClientModeShared *ClientMode;
	CHLClient *Client;
	IEngineTrace* EngineTrace;
	IVModelInfo* ModelInfo;
	IGameEventManager2* EventManager;
	ICvar* cvar;
};
extern CInterfaces gInts;
extern CPlayerVariables gPlayerVars;
extern COffsets gOffsets;
