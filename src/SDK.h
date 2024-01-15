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

enum tf_classes
{
	TF2_Scout	= 1,
	TF2_Soldier  = 3,
	TF2_Pyro	 = 7,
	TF2_Demoman  = 4,
	TF2_Heavy	= 6,
	TF2_Engineer = 9,
	TF2_Medic	= 5,
	TF2_Sniper   = 2,
	TF2_Spy		 = 8,
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
	Vector CBaseEntity::GetCollideableMins()
	{
		DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_Collision", "m_vecMins");
	}

	Vector CBaseEntity::GetCollideableMaxs()
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
	Vector get_hitbox_pos(int hitbox_idx);
	//CBaseCombatWeapon* GetActiveWeapon();

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
enum ETFClientClass {
    CClass_CTFWearableRazorback                  = 341,
    CClass_CTFWearableDemoShield                 = 338,
    CClass_CTFWearableLevelableItem              = 340,
    CClass_CTFWearableCampaignItem               = 337,
    CClass_CTFBaseRocket                         = 185,
    CClass_CTFWeaponBaseMerasmusGrenade          = 325,
    CClass_CTFWeaponBaseMelee                    = 324,
    CClass_CTFWeaponBaseGun                      = 323,
    CClass_CTFWeaponBaseGrenadeProj              = 322,
    CClass_CTFWeaponBase                         = 321,
    CClass_CTFWearableRobotArm                   = 342,
    CClass_CTFRobotArm                           = 287,
    CClass_CTFWrench                             = 344,
    CClass_CTFProjectile_ThrowableBreadMonster   = 279,
    CClass_CTFProjectile_ThrowableBrick          = 280,
    CClass_CTFProjectile_ThrowableRepel          = 281,
    CClass_CTFProjectile_Throwable               = 278,
    CClass_CTFThrowable                          = 319,
    CClass_CTFSyringeGun                         = 315,
    CClass_CTFKatana                             = 225,
    CClass_CTFSword                              = 314,
    CClass_CSniperDot                            = 118,
    CClass_CTFSniperRifleClassic                 = 308,
    CClass_CTFSniperRifleDecap                   = 309,
    CClass_CTFSniperRifle                        = 307,
    CClass_CTFChargedSMG                         = 197,
    CClass_CTFSMG                                = 306,
    CClass_CTFSlap                               = 305,
    CClass_CTFShovel                             = 304,
    CClass_CTFShotgunBuildingRescue              = 303,
    CClass_CTFPEPBrawlerBlaster                  = 241,
    CClass_CTFSodaPopper                         = 310,
    CClass_CTFShotgun_Revenge                    = 301,
    CClass_CTFScatterGun                         = 297,
    CClass_CTFShotgun_Pyro                       = 300,
    CClass_CTFShotgun_HWG                        = 299,
    CClass_CTFShotgun_Soldier                    = 302,
    CClass_CTFShotgun                            = 298,
    CClass_CTFRocketPack                         = 296,
    CClass_CTFCrossbow                           = 201,
    CClass_CTFRocketLauncher_Mortar              = 295,
    CClass_CTFRocketLauncher_AirStrike           = 293,
    CClass_CTFRocketLauncher_DirectHit           = 294,
    CClass_CTFRocketLauncher                     = 292,
    CClass_CTFRevolver                           = 286,
    CClass_CTFDRGPomson                          = 202,
    CClass_CTFRaygun                             = 284,
    CClass_CTFPistol_ScoutSecondary              = 246,
    CClass_CTFPistol_ScoutPrimary                = 245,
    CClass_CTFPistol_Scout                       = 244,
    CClass_CTFPistol                             = 243,
    CClass_CTFPipebombLauncher                   = 242,
    CClass_CTFWeaponPDA_Spy                      = 332,
    CClass_CTFWeaponPDA_Engineer_Destroy         = 331,
    CClass_CTFWeaponPDA_Engineer_Build           = 330,
    CClass_CTFWeaponPDAExpansion_Teleporter      = 334,
    CClass_CTFWeaponPDAExpansion_Dispenser       = 333,
    CClass_CTFWeaponPDA                          = 329,
    CClass_CTFParticleCannon                     = 239,
    CClass_CTFParachute_Secondary                = 238,
    CClass_CTFParachute_Primary                  = 237,
    CClass_CTFParachute                          = 236,
    CClass_CTFMinigun                            = 234,
    CClass_CTFMedigunShield                      = 231,
    CClass_CWeaponMedigun                        = 352,
    CClass_CTFProjectile_MechanicalArmOrb        = 263,
    CClass_CTFMechanicalArm                      = 230,
    CClass_CTFLunchBox_Drink                     = 229,
    CClass_CTFLunchBox                           = 228,
    CClass_CLaserDot                             = 78,
    CClass_CTFLaserPointer                       = 227,
    CClass_CTFKnife                              = 226,
    CClass_CTFGasManager                         = 212,
    CClass_CTFProjectile_JarGas                  = 261,
    CClass_CTFJarGas                             = 223,
    CClass_CTFProjectile_Cleaver                 = 254,
    CClass_CTFProjectile_JarMilk                 = 262,
    CClass_CTFProjectile_Jar                     = 260,
    CClass_CTFCleaver                            = 198,
    CClass_CTFJarMilk                            = 224,
    CClass_CTFJar                                = 222,
    CClass_CTFWeaponInvis                        = 328,
    CClass_CTFCannon                             = 196,
    CClass_CTFGrenadeLauncher                    = 216,
    CClass_CTFGrenadePipebombProjectile          = 217,
    CClass_CTFGrapplingHook                      = 215,
    CClass_CTFFlareGun_Revenge                   = 210,
    CClass_CTFFlareGun                           = 209,
    CClass_CTFFlameRocket                        = 207,
    CClass_CTFFlameThrower                       = 208,
    CClass_CTFFists                              = 205,
    CClass_CTFFireAxe                            = 204,
    CClass_CTFWeaponFlameBall                    = 327,
    CClass_CTFCompoundBow                        = 200,
    CClass_CTFClub                               = 199,
    CClass_CTFBuffItem                           = 195,
    CClass_CTFStickBomb                          = 312,
    CClass_CTFBreakableSign                      = 194,
    CClass_CTFBottle                             = 192,
    CClass_CTFBreakableMelee                     = 193,
    CClass_CTFBonesaw                            = 190,
    CClass_CTFBall_Ornament                      = 182,
    CClass_CTFStunBall                           = 313,
    CClass_CTFBat_Giftwrap                       = 188,
    CClass_CTFBat_Wood                           = 189,
    CClass_CTFBat_Fish                           = 187,
    CClass_CTFBat                                = 186,
    CClass_CTFProjectile_EnergyRing              = 256,
    CClass_CTFDroppedWeapon                      = 203, /* !!! */
    CClass_CTFWeaponSapper                       = 335,
    CClass_CTFWeaponBuilder                      = 326,
    CClass_C_TFWeaponBuilder                     = 0,
    CClass_CTFProjectile_Rocket                  = 264,
    CClass_CTFProjectile_Flare                   = 257,
    CClass_CTFProjectile_EnergyBall              = 255,
    CClass_CTFProjectile_GrapplingHook           = 258,
    CClass_CTFProjectile_HealingBolt             = 259,
    CClass_CTFProjectile_Arrow                   = 252,
    CClass_CMannVsMachineStats                   = 80,
    CClass_CTFTankBoss                           = 316,
    CClass_CTFBaseBoss                           = 183,
    CClass_CBossAlpha                            = 0,
    CClass_NextBotCombatCharacter                = 357,
    CClass_CTFProjectile_SpellKartBats           = 268,
    CClass_CTFProjectile_SpellKartOrb            = 269,
    CClass_CTFHellZap                            = 220,
    CClass_CTFProjectile_SpellLightningOrb       = 270,
    CClass_CTFProjectile_SpellTransposeTeleport  = 277,
    CClass_CTFProjectile_SpellMeteorShower       = 271,
    CClass_CTFProjectile_SpellSpawnBoss          = 274,
    CClass_CTFProjectile_SpellMirv               = 272,
    CClass_CTFProjectile_SpellPumpkin            = 273,
    CClass_CTFProjectile_SpellSpawnHorde         = 275,
    CClass_CTFProjectile_SpellSpawnZombie        = 276,
    CClass_CTFProjectile_SpellBats               = 266,
    CClass_CTFProjectile_SpellFireball           = 267,
    CClass_CTFSpellBook                          = 311,
    CClass_CHightower_TeleportVortex             = 74,
    CClass_CTeleportVortex                       = 160,
    CClass_CZombie                               = 354,
    CClass_CMerasmusDancer                       = 83,
    CClass_CMerasmus                             = 82,
    CClass_CHeadlessHatman                       = 73,
    CClass_CEyeballBoss                          = 48,
    CClass_CTFBotHintEngineerNest                = 191,
    CClass_CBotNPCMinion                         = 0,
    CClass_CBotNPC                               = 0,
    CClass_CPasstimeGun                          = 94,
    CClass_CTFViewModel                          = 320,
    CClass_CRobotDispenser                       = 112,
    CClass_CTFRobotDestruction_Robot             = 288,
    CClass_CTFReviveMarker                       = 285,
    CClass_CTFPumpkinBomb                        = 282,
    CClass_CTFProjectile_BallOfFire              = 253,
    CClass_CTFBaseProjectile                     = 184,
    CClass_CTFPointManager                       = 250,
    CClass_CBaseObjectUpgrade                    = 11,
    CClass_CTFRobotDestructionLogic              = 291,
    CClass_CTFRobotDestruction_RobotGroup        = 289,
    CClass_CTFRobotDestruction_RobotSpawn        = 290,
    CClass_CTFPlayerDestructionLogic             = 248,
    CClass_CPlayerDestructionDispenser           = 101,
    CClass_CTFMinigameLogic                      = 233,
    CClass_CTFHalloweenMinigame_FallingPlatforms = 219,
    CClass_CTFHalloweenMinigame                  = 218,
    CClass_CTFMiniGame                           = 232,
    CClass_CTFPowerupBottle                      = 251,
    CClass_CTFItem                               = 221,
    CClass_CHalloweenSoulPack                    = 71,
    CClass_CTFGenericBomb                        = 213,
    CClass_CBonusRoundLogic                      = 23,
    CClass_CTFGameRulesProxy                     = 211,
    CClass_CTETFParticleEffect                   = 179,
    CClass_CTETFExplosion                        = 178,
    CClass_CTETFBlood                            = 177,
    CClass_CTFFlameManager                       = 206,
    CClass_CHalloweenGiftPickup                  = 69,
    CClass_CBonusDuckPickup                      = 21,
    CClass_CHalloweenPickup                      = 70,
    CClass_CCaptureFlagReturnIcon                = 27,
    CClass_CCaptureFlag                          = 26,
    CClass_CBonusPack                            = 22,
    CClass_CTFTeam                               = 318,
    CClass_CTFTauntProp                          = 317,
    CClass_CTFPlayerResource                     = 249,
    CClass_CTFPlayer                             = 247, /* !!! */
    CClass_CTFRagdoll                            = 283,
    CClass_CTEPlayerAnimEvent                    = 165,
    CClass_CTFPasstimeLogic                      = 240,
    CClass_CPasstimeBall                         = 93,
    CClass_CTFObjectiveResource                  = 235,
    CClass_CTFGlow                               = 214,
    CClass_CTEFireBullets                        = 152,
    CClass_CTFBuffBanner                         = 0,
    CClass_CTFAmmoPack                           = 181, /* !!! */
    CClass_CObjectTeleporter                     = 89,  /* !!! */
    CClass_CObjectSentrygun                      = 88,  /* !!! */
    CClass_CTFProjectile_SentryRocket            = 265,
    CClass_CObjectSapper                         = 87,
    CClass_CObjectCartDispenser                  = 85,
    CClass_CObjectDispenser                      = 86, /* !!! */
    CClass_CMonsterResource                      = 84,
    CClass_CFuncRespawnRoomVisualizer            = 64,
    CClass_CFuncRespawnRoom                      = 63,
    CClass_CFuncPasstimeGoal                     = 61,
    CClass_CFuncForceField                       = 57,
    CClass_CCaptureZone                          = 28,
    CClass_CCurrencyPack                         = 31, /* !!! */
    CClass_CBaseObject                           = 10,
    CClass_CTestTraceline                        = 176,
    CClass_CTEWorldDecal                         = 180,
    CClass_CTESpriteSpray                        = 174,
    CClass_CTESprite                             = 173,
    CClass_CTESparks                             = 172,
    CClass_CTESmoke                              = 171,
    CClass_CTEShowLine                           = 169,
    CClass_CTEProjectedDecal                     = 167,
    CClass_CTEPlayerDecal                        = 166,
    CClass_CTEPhysicsProp                        = 164,
    CClass_CTEParticleSystem                     = 163,
    CClass_CTEMuzzleFlash                        = 162,
    CClass_CTELargeFunnel                        = 159,
    CClass_CTEKillPlayerAttachments              = 158,
    CClass_CTEImpact                             = 157,
    CClass_CTEGlowSprite                         = 156,
    CClass_CTEShatterSurface                     = 168,
    CClass_CTEFootprintDecal                     = 154,
    CClass_CTEFizz                               = 153,
    CClass_CTEExplosion                          = 151,
    CClass_CTEEnergySplash                       = 150,
    CClass_CTEEffectDispatch                     = 149,
    CClass_CTEDynamicLight                       = 148,
    CClass_CTEDecal                              = 146,
    CClass_CTEClientProjectile                   = 145,
    CClass_CTEBubbleTrail                        = 144,
    CClass_CTEBubbles                            = 143,
    CClass_CTEBSPDecal                           = 142,
    CClass_CTEBreakModel                         = 141,
    CClass_CTEBloodStream                        = 140,
    CClass_CTEBloodSprite                        = 139,
    CClass_CTEBeamSpline                         = 138,
    CClass_CTEBeamRingPoint                      = 137,
    CClass_CTEBeamRing                           = 136,
    CClass_CTEBeamPoints                         = 135,
    CClass_CTEBeamLaser                          = 134,
    CClass_CTEBeamFollow                         = 133,
    CClass_CTEBeamEnts                           = 132,
    CClass_CTEBeamEntPoint                       = 131,
    CClass_CTEBaseBeam                           = 130,
    CClass_CTEArmorRicochet                      = 129,
    CClass_CTEMetalSparks                        = 161,
    CClass_CSteamJet                             = 123,
    CClass_CSmokeStack                           = 117,
    CClass_DustTrail                             = 355,
    CClass_CFireTrail                            = 50,
    CClass_SporeTrail                            = 362,
    CClass_SporeExplosion                        = 361,
    CClass_RocketTrail                           = 359,
    CClass_SmokeTrail                            = 360,
    CClass_CPropVehicleDriveable                 = 108,
    CClass_ParticleSmokeGrenade                  = 358,
    CClass_CParticleFire                         = 90,
    CClass_MovieExplosion                        = 356,
    CClass_CTEGaussExplosion                     = 155,
    CClass_CEnvQuadraticBeam                     = 43,
    CClass_CEmbers                               = 36,
    CClass_CEnvWind                              = 47,
    CClass_CPrecipitation                        = 107,
    CClass_CBaseTempEntity                       = 17,
    CClass_CWeaponIFMSteadyCam                   = 351,
    CClass_CWeaponIFMBaseCamera                  = 350,
    CClass_CWeaponIFMBase                        = 349,
    CClass_CTFWearableVM                         = 343,
    CClass_CTFWearable                           = 336,
    CClass_CTFWearableItem                       = 339,
    CClass_CEconWearable                         = 35,
    CClass_CBaseAttributableItem                 = 3,
    CClass_CEconEntity                           = 34,
    CClass_CHandleTest                           = 72,
    CClass_CTeamplayRoundBasedRulesProxy         = 126,
    CClass_CTeamRoundTimer                       = 127,
    CClass_CSpriteTrail                          = 122,
    CClass_CSpriteOriented                       = 121,
    CClass_CSprite                               = 120,
    CClass_CRagdollPropAttached                  = 111,
    CClass_CRagdollProp                          = 110,
    CClass_CPoseController                       = 106,
    CClass_CGameRulesProxy                       = 68,
    CClass_CInfoLadderDismount                   = 75,
    CClass_CFuncLadder                           = 58,
    CClass_CEnvDetailController                  = 40,
    CClass_CWorld                                = 353,
    CClass_CWaterLODControl                      = 348,
    CClass_CWaterBullet                          = 347,
    CClass_CVoteController                       = 346,
    CClass_CVGuiScreen                           = 345,
    CClass_CPropJeep                             = 0,
    CClass_CPropVehicleChoreoGeneric             = 0,
    CClass_CTest_ProxyToggle_Networkable         = 175,
    CClass_CTesla                                = 170,
    CClass_CTeamTrainWatcher                     = 128,
    CClass_CBaseTeamObjectiveResource            = 16,
    CClass_CTeam                                 = 125,
    CClass_CSun                                  = 124,
    CClass_CParticlePerformanceMonitor           = 91,
    CClass_CSpotlightEnd                         = 119,
    CClass_CSlideshowDisplay                     = 116,
    CClass_CShadowControl                        = 115,
    CClass_CSceneEntity                          = 114,
    CClass_CRopeKeyframe                         = 113,
    CClass_CRagdollManager                       = 109,
    CClass_CPhysicsPropMultiplayer               = 98,
    CClass_CPhysBoxMultiplayer                   = 96,
    CClass_CBasePropDoor                         = 15,
    CClass_CDynamicProp                          = 33,
    CClass_CPointWorldText                       = 105,
    CClass_CPointCommentaryNode                  = 104,
    CClass_CPointCamera                          = 103,
    CClass_CPlayerResource                       = 102,
    CClass_CPlasma                               = 100,
    CClass_CPhysMagnet                           = 99,
    CClass_CPhysicsProp                          = 97,
    CClass_CPhysBox                              = 95,
    CClass_CParticleSystem                       = 92,
    CClass_CMaterialModifyControl                = 81,
    CClass_CLightGlow                            = 79,
    CClass_CInfoOverlayAccessor                  = 77,
    CClass_CFuncTrackTrain                       = 67,
    CClass_CFuncSmokeVolume                      = 66,
    CClass_CFuncRotating                         = 65,
    CClass_CFuncReflectiveGlass                  = 62,
    CClass_CFuncOccluder                         = 60,
    CClass_CFuncMonitor                          = 59,
    CClass_CFunc_LOD                             = 54,
    CClass_CTEDust                               = 147,
    CClass_CFunc_Dust                            = 53,
    CClass_CFuncConveyor                         = 56,
    CClass_CBreakableSurface                     = 25,
    CClass_CFuncAreaPortalWindow                 = 55,
    CClass_CFish                                 = 51,
    CClass_CEntityFlame                          = 38,
    CClass_CFireSmoke                            = 49,
    CClass_CEnvTonemapController                 = 46,
    CClass_CEnvScreenEffect                      = 44,
    CClass_CEnvScreenOverlay                     = 45,
    CClass_CEnvProjectedTexture                  = 42,
    CClass_CEnvParticleScript                    = 41,
    CClass_CFogController                        = 52,
    CClass_CEntityParticleTrail                  = 39,
    CClass_CEntityDissolve                       = 37,
    CClass_CDynamicLight                         = 32,
    CClass_CColorCorrectionVolume                = 30,
    CClass_CColorCorrection                      = 29,
    CClass_CBreakableProp                        = 24,
    CClass_CBasePlayer                           = 13,
    CClass_CBaseFlex                             = 8,
    CClass_CBaseEntity                           = 7,
    CClass_CBaseDoor                             = 6,
    CClass_CBaseCombatCharacter                  = 4,
    CClass_CBaseAnimatingOverlay                 = 2,
    CClass_CBoneFollower                         = 20,
    CClass_CBaseAnimating                        = 1, /* !!! */
    CClass_CInfoLightingRelative                 = 76,
    CClass_CAI_BaseNPC                           = 0,
    CClass_CBeam                                 = 19,
    CClass_CBaseViewModel                        = 18,
    CClass_CBaseProjectile                       = 14,
    CClass_CBaseParticleEntity                   = 12,
    CClass_CBaseGrenade                          = 9,
    CClass_CBaseCombatWeapon                     = 5
};



enum slots 
{
    WPN_SLOT_PRIMARY   = 0,
    WPN_SLOT_SECONDARY = 1,
    WPN_SLOT_MELEE     = 2,
    WPN_SLOT_GRENADES  = 3,
    WPN_SLOT_BOMB      = 4,
};

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


class IGameEventManager2
{
public:
	bool AddListener(IGameEventListener2 *listener, const char *name, bool bServerSide)
	{
		typedef bool(*OriginalFn)(void*, IGameEventListener2 *, const char *, bool);
		return getvfunc<OriginalFn>(this, 3)(this, listener, name, bServerSide);
	}
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
		return getvfunc<OriginalFn>(this, 327)(this);
	}

	char *GetName()
	{
		typedef char *(*OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 329)(this);
	}

	char *GetPrintName()
	{
		typedef char *(*OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 330)(this);
	}

	int GetItemDefinitionIndex()
	{
		DYNVAR_RETURN(int, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
	}

	float ChargedDamage()
	{
		DYNVAR_RETURN(float, this, "DT_TFSniperRifle", "SniperRifleLocalData", "m_flChargedDamage");
	}

	bool ReadyToBackstab()
	{
		DYNVAR_RETURN(bool, this, "DT_TFWeaponKnife", "m_bReadyToBackstab");
	}
};


enum playercontrols
{
	IN_ATTACK	= (1 << 0),
	IN_JUMP		 = (1 << 1),
	IN_DUCK		 = (1 << 2),
	IN_FORWARD   = (1 << 3),
	IN_BACK		 = (1 << 4),
	IN_USE		 = (1 << 5),
	IN_CANCEL	= (1 << 6),
	IN_LEFT		 = (1 << 7),
	IN_RIGHT	 = (1 << 8),
	IN_MOVELEFT  = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2   = (1 << 11),
	IN_RUN		 = (1 << 12),
	IN_RELOAD	= (1 << 13),
	IN_ALT1		 = (1 << 14),
	IN_ALT2		 = (1 << 15),
	IN_SCORE	 = (1 << 16), // Used by client.dll for when scoreboard is held down
	IN_SPEED	 = (1 << 17), // Player is holding the speed key
	IN_WALK		 = (1 << 18), // Player holding walk key
	IN_ZOOM		 = (1 << 19), // Zoom key for HUD zoom
	IN_WEAPON1   = (1 << 20), // weapon defines these bits
	IN_WEAPON2   = (1 << 21), // weapon defines these bits
	IN_BULLRUSH  = (1 << 22),
};

enum tf_cond
{
	TFCond_Slowed			   = (1 << 0),  //Toggled when a player is slowed down.
	TFCond_Zoomed			   = (1 << 1),  //Toggled when a player is zoomed.
	TFCond_Disguising		   = (1 << 2),  //Toggled when a Spy is disguising.
	TFCond_Disguised		   = (1 << 3),  //Toggled when a Spy is disguised.
	TFCond_Cloaked			   = (1 << 4),  //Toggled when a Spy is invisible.
	TFCond_Ubercharged		   = (1 << 5),  //Toggled when a player is \DCberCharged.
	TFCond_TeleportedGlow	  = (1 << 6),  //Toggled when someone leaves a teleporter and has glow beneath their feet.
	TFCond_Taunting			   = (1 << 7),  //Toggled when a player is taunting.
	TFCond_UberchargeFading	= (1 << 8),  //Toggled when the \DCberCharge is fading.
	TFCond_CloakFlicker		   = (1 << 9),  //Toggled when a Spy is visible during cloak.
	TFCond_Teleporting		   = (1 << 10), //Only activates for a brief second when the player is being teleported; not very useful.
	TFCond_Kritzkrieged		   = (1 << 11), //Toggled when a player is being crit buffed by the KritzKrieg.
	TFCond_TmpDamageBonus	  = (1 << 12), //Unknown what this is for. Name taken from the AlliedModders SDK.
	TFCond_DeadRingered		   = (1 << 13), //Toggled when a player is taking reduced damage from the Deadringer.
	TFCond_Bonked			   = (1 << 14), //Toggled when a player is under the effects of The Bonk! Atomic Punch.
	TFCond_Stunned			   = (1 << 15), //Toggled when a player's speed is reduced from airblast or a Sandman ball.
	TFCond_Buffed			   = (1 << 16), //Toggled when a player is within range of an activated Buff Banner.
	TFCond_Charging			   = (1 << 17), //Toggled when a Demoman charges with the shield.
	TFCond_DemoBuff			   = (1 << 18), //Toggled when a Demoman has heads from the Eyelander.
	TFCond_CritCola			   = (1 << 19), //Toggled when the player is under the effect of The Crit-a-Cola.
	TFCond_InHealRadius		   = (1 << 20), //Unused condition, name taken from AlliedModders SDK.
	TFCond_Healing			   = (1 << 21), //Toggled when someone is being healed by a medic or a dispenser.
	TFCond_OnFire			   = (1 << 22), //Toggled when a player is on fire.
	TFCond_Overhealed		   = (1 << 23), //Toggled when a player has >100% health.
	TFCond_Jarated			   = (1 << 24), //Toggled when a player is hit with a Sniper's Jarate.
	TFCond_Bleeding			   = (1 << 25), //Toggled when a player is taking bleeding damage.
	TFCond_DefenseBuffed	   = (1 << 26), //Toggled when a player is within range of an activated Battalion's Backup.
	TFCond_Milked			   = (1 << 27), //Player was hit with a jar of Mad Milk.
	TFCond_MegaHeal			   = (1 << 28), //Player is under the effect of Quick-Fix charge.
	TFCond_RegenBuffed		   = (1 << 29), //Toggled when a player is within a Concheror's range.
	TFCond_MarkedForDeath	  = (1 << 30), //Player is marked for death by a Fan O'War hit. Effects are similar to TFCond_Jarated.
	TFCond_NoHealingDamageBuff = (1 << 31), //Unknown what this is used for.

	TFCondEx_SpeedBuffAlly			= (1 << 0),  //Toggled when a player gets hit with the disciplinary action.
	TFCondEx_HalloweenCritCandy		= (1 << 1),  //Only for Scream Fortress event maps that drop crit candy.
	TFCondEx_CritCanteen			= (1 << 2),  //Player is getting a crit boost from a MVM canteen.
	TFCondEx_CritDemoCharge			= (1 << 3),  //From demo's shield
	TFCondEx_CritHype				= (1 << 4),  //Soda Popper crits.
	TFCondEx_CritOnFirstBlood		= (1 << 5),  //Arena first blood crit buff.
	TFCondEx_CritOnWin				= (1 << 6),  //End of round crits.
	TFCondEx_CritOnFlagCapture		= (1 << 7),  //CTF intelligence capture crits.
	TFCondEx_CritOnKill				= (1 << 8),  //Unknown what this is for.
	TFCondEx_RestrictToMelee		= (1 << 9),  //Unknown what this is for.
	TFCondEx_DefenseBuffNoCritBlock = (1 << 10), //MvM Buff.
	TFCondEx_Reprogrammed			= (1 << 11), //MvM Bot has been reprogrammed.
	TFCondEx_PyroCrits				= (1 << 12), //Player is getting crits from the Mmmph charge.
	TFCondEx_PyroHeal				= (1 << 13), //Player is being healed from the Mmmph charge.
	TFCondEx_FocusBuff				= (1 << 14), //Player is getting a focus buff.
	TFCondEx_DisguisedRemoved		= (1 << 15), //Disguised remove from a bot.
	TFCondEx_MarkedForDeathSilent   = (1 << 16), //Player is under the effects of the Escape Plan/Equalizer or GRU.
	TFCondEx_DisguisedAsDispenser   = (1 << 17), //Bot is disguised as dispenser.
	TFCondEx_Sapped					= (1 << 18), //MvM bot is being sapped.
	TFCondEx_UberchargedHidden		= (1 << 19), //MvM Related
	TFCondEx_UberchargedCanteen		= (1 << 20), //Player is receiving \DCberCharge from a canteen.
	TFCondEx_HalloweenBombHead		= (1 << 21), //Player has a bomb on their head from Merasmus.
	TFCondEx_HalloweenThriller		= (1 << 22), //Players are forced to dance from Merasmus.
	TFCondEx_BulletCharge			= (1 << 26), //Player is receiving 75% reduced damage from bullets.
	TFCondEx_ExplosiveCharge		= (1 << 27), //Player is receiving 75% reduced damage from explosives.
	TFCondEx_FireCharge				= (1 << 28), //Player is receiving 75% reduced damage from fire.
	TFCondEx_BulletResistance		= (1 << 29), //Player is receiving 10% reduced damage from bullets.
	TFCondEx_ExplosiveResistance	= (1 << 30), //Player is receiving 10% reduced damage from explosives.
	TFCondEx_FireResistance			= (1 << 31), //Player is receiving 10% reduced damage from fire.

	TFCondEx2_Stealthed				= (1 << 0),
	TFCondEx2_MedigunDebuff			= (1 << 1),
	TFCondEx2_StealthedUserBuffFade = (1 << 2),
	TFCondEx2_BulletImmune			= (1 << 3),
	TFCondEx2_BlastImmune			= (1 << 4),
	TFCondEx2_FireImmune			= (1 << 5),
	TFCondEx2_PreventDeath			= (1 << 6),
	TFCondEx2_MVMBotRadiowave		= (1 << 7),
	TFCondEx2_HalloweenSpeedBoost   = (1 << 8),  //Wheel has granted player speed boost.
	TFCondEx2_HalloweenQuickHeal	= (1 << 9),  //Wheel has granted player quick heal.
	TFCondEx2_HalloweenGiant		= (1 << 10), //Wheel has granted player giant mode.
	TFCondEx2_HalloweenTiny			= (1 << 11), //Wheel has granted player tiny mode.
	TFCondEx2_HalloweenInHell		= (1 << 12), //Wheel has granted player in hell mode.
	TFCondEx2_HalloweenGhostMode	= (1 << 13), //Wheel has granted player ghost mode.
	TFCondEx2_Parachute				= (1 << 16), //Player has deployed the BASE Jumper.
	TFCondEx2_BlastJumping			= (1 << 17), //Player has sticky or rocket jumped.

	TFCond_MiniCrits	  = (TFCond_Buffed | TFCond_CritCola),
	TFCond_IgnoreStates   = (TFCond_Ubercharged | TFCond_Bonked),
	TFCondEx_IgnoreStates = (TFCondEx_PyroHeal)
};



enum source_lifestates
{
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};

class ClientModeShared
{
public:
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

struct cplane_t // windows : byte linux: unsigned char
{
	Vector normal;
	float	dist;
	unsigned char	type;
	unsigned char	signbits;
	unsigned char	pad[2];
};

struct csurface_t
{
	const char		*name;
	short			surfaceProps;
	unsigned short	flags;
};

enum SurfaceFlags_t
{
	DISPSURF_FLAG_SURFACE = (1 << 0),
	DISPSURF_FLAG_WALKABLE = (1 << 1),
	DISPSURF_FLAG_BUILDABLE = (1 << 2),
	DISPSURF_FLAG_SURFPROP1 = (1 << 3),
	DISPSURF_FLAG_SURFPROP2 = (1 << 4),
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
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
	//ICvar* cvar;
};
//#include "CConfig.h"
//#include "Colors.h"
//extern CMenuConfig gMenuVar;
extern CInterfaces gInts;
extern CPlayerVariables gPlayerVars;
extern COffsets gOffsets;
//extern ShitColors gColors;
