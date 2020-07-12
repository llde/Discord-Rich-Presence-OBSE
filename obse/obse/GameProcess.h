#pragma once

#include "obse/GameForms.h"
#include "obse/GameExtraData.h"
#include "obse/GameThreads.h"
#include "obse/NiNodes.h"
#include "obse/NiHavokCharacters.h"
#include "obse/NiControllers.h"

class Actor;
class TESObjectREFR;
class MobileObject;
class ActiveEffect;
class NiObject;
class BSBound;
class PathingData;
class bhkCharacterController;
class ModelKF;

// size?
// This is used all over the game code to manage actors and occassionally other objects.
class ActorProcessManager
{
public:
	ActorProcessManager();
	~ActorProcessManager();

	struct ActorList {
		tList<Actor>			head;
		tList<Actor>::_Node		* tail;
	};

	ActorList				middleHighActors;		// 00
	ActorList				lowActors0C;			// 0C significantly smaller list than lowActors18. 
													//	  Plausible: actors currently scheduled for low processing
	ActorList				lowActors18;			// 18
	float					unk24;					// 24
	UInt32					unk28;					// 28
	void*					unk2C;					// 2C
	UInt32					unk30;					// 30
	void*					unk34;					// 34
	UInt32					unk38[4];				// 38
	tList<BSTempEffect>		tempEffects;			// 48
	UInt32					unk4C;					// 4C
	tList<MobileObject>		mobileObjects;			// 50
	UInt32					unk58;					// 58
	tList<Actor>			highActors;				// 5C
	Actor*					actor64;				// 64
	tList<Actor>::_Node*	unkNodes[3];			// 68 ##TODO: which lists do these belong to
	UInt32					unk74;					// 74 Possibly not a member. Definitely no more members following this.
};

extern ActorProcessManager * g_actorProcessManager;

// 14
class PathLow
{
public:
	PathLow();
	~PathLow();

	enum {
		kPathType_Low,
		kPathType_MiddleHigh,
		kPathType_High
	};

	virtual void	Destroy(bool bFreeMem) = 0;
	virtual UInt32	GetType(void) = 0;				// return one of kPathType_XXX
	virtual void	Unk_02(UInt32 arg0) = 0;
	virtual bool	Unk_03(void) = 0;
	virtual void	Unk_04(void) = 0;				// move to next path step?
	virtual void	Unk_05(Actor* actor, float* arg1, UInt32 arg2) = 0;	// arg1 looks like ptr to 3D vec. Build path?
	virtual bool	Unk_06(Actor* actor) = 0;		// update path?
	virtual void	Unk_07(UInt32 arg0, float arg1) = 0;
	virtual bool	Unk_08(Actor* actor) = 0;
	virtual void	Unk_09(Actor* actor, float* arg1) = 0;
	virtual void	Unk_0A(UInt32 arg0) = 0;
	virtual bool	GetPathingFailed(void) = 0;
	virtual void	SetPathingFailed(bool bSet) = 0;

//	void	** _vtbl;				// 00
	UInt32	unk04;					// 04
	UInt32	unk08;					// 08
	float	pathPointReachDistance;	// 0C
	UInt8	unk10;					// 10
	UInt8	pad11[3];				// 11
};

// 1C
class PathMiddleHigh : public PathLow
{
public:
	PathMiddleHigh();
	~PathMiddleHigh();

	UInt32	unk14;	// 14
	UInt32	unk18;	// 18
};

// 4C
class PathHigh : public PathMiddleHigh
{
public:
	PathHigh();
	~PathHigh();

	enum {
		kFlag_PathingFailed				= 1 << 3,
		kFlag_Turning					= 1 << 5,
	};

	// ###TODO: size?
	struct AvoidanceInfo {
		enum {
			kStatus_NONE,
			kStatus_CheckingObstacle,
			kStatus_AvoidingObstacle,
			kStatus_AnglingLeft,
			kStatus_AnglingRight,
			kStatus_Waiting,
			kStatus_ForcingNewPath,
			kStatus_FAILED
		};

		UInt32		unk00;
		UInt32		unk04;
		UInt32		unk08;
		UInt8		status;
		UInt8		unk0D[3];
		UInt32		unk10;
		float		unk14;
		float		waitTime;
		float		anglingTime;
		// ...?
	};


	float		unk1C;				// 1C
	float		unk20;				// 20
	UInt32		unk24;				// 24
	UInt32		unk28;				// 28
	UInt8		flags;				// 2C
	UInt8		unk2D[3];
	AvoidanceInfo	* avoidanceInfo;	// 30
	UInt32		unk34;				// 34
	UInt32		unk38;				// 38
	float		unk3C[3];			// 3C
	UInt32		unk48;				// 48
};

STATIC_ASSERT(sizeof(PathHigh) == 0x4C);

// 48
class PathBuilder : public BackgroundLoader
{
public:
	PathBuilder();
	~PathBuilder();

	typedef NiTPointerMap<PathingData>	ActorPathingMap;	// key is Actor*

	ActorPathingMap		actorPathingMap_10;		// 10	paths not started yet?
	ActorPathingMap		actorPathingMap_20;		// 20	paths in progress?
	ActorPathingMap		completedPaths;			// 30
	UInt32				unk40;					// 40
	UInt32				unk44;					// 44

	static PathBuilder * GetSingleton();
};

// 1C0
class CombatController : public TESPackage
{
public:
	CombatController();
	~CombatController();

	struct TargetInfo {
		Actor	* target;
		UInt32	unk04;		// looks like priority. seen values from 0-200
	};

	struct TargetList {
		TargetInfo	* info;
		TargetList	* next;
	};

	struct AvailableSpellExtraInfo {	// for item-based spells, e.g. potions and scrolls
		void		* unk00;
		UInt32		quantity;
		TESForm		* source;	// e.g. for EnchantmentItem on a scroll, a TESObjectBOOK*
		void		* unk0C;	// looks like pointer into owner's ExtraContainerChanges list for this item
	};

	struct AvailableSpellInfo {
		MagicItem				* item;
		AvailableSpellExtraInfo	* extraInfo;
	};

	struct AvailableSpellList {		// all the spells/magic items the actor has available. includes spells resolved from leveled lists
		AvailableSpellInfo	* info;
		AvailableSpellList	* next;
	};

	struct SelectedSpellInfo {
		MagicItem			* item;
		AvailableSpellExtraInfo	* availableInfo;	// for scrolls, potions, points to the entry to this (if any) in available spells list
													// is NULL for spells
	};

	struct AlliesList {
		Actor		* ally;
		AlliesList	* next;
	};

	Actor				* actor;					// 03C
	TargetList			* targets;					// 040
	float				unk044;						// 044
	UInt32				unk048;						// 048
	UInt32				unk04C;						// 04C
	UInt32				unk050;						// 050
	float				unk054;						// 054
	float				unk058;						// 058
	AvailableSpellList	* rangedSpells;				// 05C
	AvailableSpellList	* meleeSpells;				// 060
	AvailableSpellList	* restoreSpells;			// 064
	AvailableSpellList	* buffSpells;				// 068 anything that doesn't fit in the other 3 categories
	UInt32				Flag06C;					// 06C
	UInt32				Flag070;					// 070
	UInt32				unk074;						// 074
	UInt32				unk078;						// 078
	SelectedSpellInfo	* selectedMeleeSpell;		// 07C
	SelectedSpellInfo	* selectedRangedSpell;		// 080
	SelectedSpellInfo	* selectedRestoreSpell;		// 084
	UInt32				unk088;						// 088
	UInt32				unk08C;						// 08C
	SelectedSpellInfo	* selectedBuffSpell;		// 090
	SelectedSpellInfo	* selectedBoundArmorSpell;	// 094
	SelectedSpellInfo	* selectedBoundWeaponSpell; // 098
	SelectedSpellInfo	* selectedSummonSpell;		// 09C
	UInt32				unk0A0[(0x15C-0x0A0) >> 2];	// 0A0
	AlliesList			allies;						// 15C
	UInt32				unk164[(0x1C0-0x164) >> 2];	// 164
};
#if OBLIVION
STATIC_ASSERT(sizeof(CombatController) == 0x1C0);
#endif

// 004+
class BaseProcess
{
public:
	enum
	{
		//	RefIDIdx	combatController;
	};

	enum
	{
		kMovementFlag_Sneaking =	0x00000400,	// overridden by kMovementFlag_Swimming
		kMovementFlag_Swimming =	0x00000800
	};

	BaseProcess();
	~BaseProcess();

	virtual void	Destructor(void);
	virtual void	Copy(BaseProcess* SourceProcess);
	virtual UInt32	GetProcessLevel();	// 0 - high, 3 - low
	virtual void	Unk_03(Actor* Act, UInt32 Arg1);
	virtual void	ManagePackProcedure(Actor* Act);
	virtual void	Unk_05(UInt32 arg0, UInt32 arg1) = 0;
	virtual bool	Unk_06(UInt32 arg0, UInt32 arg1) = 0;
	virtual void	SetCurHour(float Hour);
	virtual void	Unk_08(void) = 0;
	virtual bool	Unk_09(void) = 0;
	virtual float	GetCurHour();
	virtual UInt32	GetCurPackedDate(void) = 0;
	virtual void	SetCurPackedDate(UInt32 arg0) = 0;
	virtual UInt32	GetCurDay();
	virtual UInt32	GetCurMonth();
	virtual UInt32	GetCurYear();
	virtual void	Unk_10(UInt32 arg0) = 0;
	virtual UInt32	Unk_11(void) = 0;
	virtual void	Unk_12(UInt32 arg0) = 0;
	virtual void	Unk_13(void) = 0;
	virtual UInt32	Unk_14(void) = 0;
	virtual void	Unk_15(UInt32 arg0) = 0;
	virtual void	Unk_16(UInt32 arg0, UInt32 arg1) = 0;
	virtual void	Unk_17(void) = 0;
	virtual void	Unk_18(UInt32 arg0) = 0;
	virtual bool	Unk_19(void) = 0;
	virtual void	Unk_1A(UInt32 arg0) = 0;
	virtual bool	Unk_1B(UInt32 arg0, UInt32 arg1) = 0;
	virtual void	Unk_1C(UInt32 arg0, UInt32 arg1, UInt32 arg2) = 0;
	virtual UInt32	Unk_1D(UInt32 arg0) = 0;
	virtual UInt32	Unk_1E(UInt32 arg0) = 0;
	virtual float	Unk_1F(UInt32 arg0) = 0;
	virtual void	Unk_20(UInt32 arg0, UInt32 arg1) = 0;
	virtual void	Unk_21(UInt32 arg0, UInt32 arg1, UInt32 arg2) = 0;
	virtual bool	Unk_22(void) = 0;
	virtual void	Unk_23(UInt32 arg0) = 0;
	virtual void	Unk_24(UInt32 arg0) = 0;
	virtual void	Unk_25(void) = 0;
	virtual void	UpdateUnk088(void) = 0;
	virtual float	GetUnk088(void) = 0;
	virtual void	SetUnk088(float arg0) = 0;
	virtual void	Unk_29(void) = 0;

	// returns HighProcess::DetectionList::Data* associated with detectedActor. detectionState is one of kDetectionState_XXX enum
	// ###TODO: move some of this stuff out of HighProcess class def so we can forward declare it
	virtual void *  SetDetected(Actor* detectedActor, UInt8 detectionState, bool hasLOS, SInt32 detectionLevel) = 0;
	virtual bool	Unk_2B(void) = 0;
	virtual void	Unk_2C(UInt32 arg0) = 0;

#if OBLIVION_VERSION >= OBLIVION_VERSION_1_2
	virtual void	Unk_2C_1_2(void) = 0;	// added in 1.2 patch
#endif

	virtual void	Unk_2D(UInt32 arg0) = 0;
	virtual void	Unk_2E(UInt32 arg0) = 0;
	virtual bool	Unk_2F(void) = 0;
	virtual void	Unk_30(UInt32 arg0) = 0;
	virtual bool	Unk_31(void) = 0;
	virtual TESObjectREFR *	GetUnk02C(void) = 0;
	virtual void	SetUnk02C(TESObjectREFR * obj) = 0;
	virtual UInt32	Unk_34(void) = 0;
	virtual void	Unk_35(UInt32 arg0) = 0;
	virtual void	Unk_36(UInt32 arg0) = 0;
	virtual UInt32	Unk_37(void) = 0;
	virtual void	Unk_38(UInt32 arg0) = 0;
	virtual UInt32	Unk_39(UInt32 arg0) = 0;
	virtual UInt32	Unk_3A(UInt32 arg0) = 0;
	virtual ExtraContainerChanges::EntryData* GetEquippedWeaponData(bool arg0) = 0;
	virtual UInt32	Unk_3C(UInt32 arg0) = 0;
	virtual ExtraContainerChanges::EntryData* GetEquippedAmmoData(bool arg0) = 0;
	virtual UInt32	Unk_3E(void) = 0;
	virtual void	Unk_3F(UInt32 arg0) = 0;
	virtual bool	Unk_40(UInt32 arg0, UInt32 arg1) = 0;
	virtual bool	Unk_41(UInt32 arg0) = 0;
	virtual bool	Unk_42(UInt32 arg0) = 0;
	virtual bool	Unk_43(UInt32 arg0) = 0;
	virtual bool	Unk_44(UInt32 arg0) = 0;
	virtual UInt32	Unk_45(UInt32 arg0) = 0;
	virtual UInt32	Unk_46(UInt32 arg0) = 0;
	virtual UInt32	Unk_47(UInt32 arg0) = 0;
	virtual UInt32	Unk_48(UInt32 arg0) = 0;
	virtual UInt32	Unk_49(UInt32 arg0) = 0;
	virtual UInt32	Unk_4A(UInt32 arg0) = 0;
	virtual UInt32	Unk_4B(UInt32 arg0) = 0;
	virtual UInt32	Unk_4C(UInt32 arg0) = 0;
	virtual bool	Unk_4D(void) = 0;
	virtual bool	Unk_4E(void) = 0;
	virtual UInt8	GetUnk084(void) = 0;
	virtual void	SetUnk084(UInt8 arg0) = 0;
	virtual bool	GetUnk01E(void) = 0;
	virtual void	SetUnk01E(UInt32 arg0) = 0;
	virtual void	Unk_53(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3) = 0;
	virtual void	SetUnk024(UInt32 arg) = 0;
	virtual void	Unk_55(void) = 0;
	virtual void	Unk_56(void) = 0;
	virtual void	Unk_57(void) = 0;
	virtual void	GetUnk028(float arg) = 0;
	virtual float	SetUnk028(void) = 0;
	virtual void	Unk_5A(void) = 0;
	virtual void	Unk_5B(void) = 0;
	virtual void	Unk_5C(void) = 0;
	virtual void					SetCurrentPackage(TESPackage* Package);
	virtual TESPackage::eProcedure	SetCurrentPackProcedure(TESPackage::eProcedure PackProcedure);
	virtual TESPackage::eProcedure	GetCurrentPackProcedure();
	virtual TESPackage*				GetCurrentPackage();	// returns MiddleHighProcess::pkg0C0 if not NULL, else BaseProcess::package
	virtual void					Unk_61(UInt32 arg0) = 0;			// marks ScriptEventList::kEvent_OnPackageDone
	virtual void**	GetCharProxy(void** characterProxy) = 0;	// increfs and returns the proxy (or sets to NULL)
	virtual void	Unk_63(void * obj) = 0;
	virtual void	Unk_64(void) = 0;
	virtual void	Unk_65(void) = 0;
	virtual void	Unk_66(void) = 0;
	virtual void	Unk_67(void) = 0;
	virtual void	Unk_68(void) = 0;
	virtual void	Unk_69(void) = 0;
	virtual void	Unk_6A(void) = 0;
	virtual void	Unk_6B(void) = 0;
	virtual void	Unk_6C(void) = 0;
	virtual void	Unk_6D(void) = 0;
	virtual void	Unk_6E(void) = 0;
	virtual void	Unk_6F(void) = 0;
	virtual void	Unk_70(void) = 0;
	virtual bool	GetLOS(UInt32 arg0, TESObjectREFR* target) = 0;	// arg0 unused
	virtual void	Unk_72(void) = 0;
	virtual void	Unk_73(void) = 0;
	virtual void	Unk_74(void) = 0;
	virtual void	Unk_75(void) = 0;
	virtual void	Unk_76(void) = 0;
	virtual void	Unk_77(void) = 0;
	virtual void	Unk_78(void) = 0;
	virtual void	Unk_79(void) = 0;
	virtual void	Unk_7A(void) = 0;
	virtual void	Unk_7B(void) = 0;
	virtual void	Unk_7C(void) = 0;
	virtual void	Unk_7D(void) = 0;
	virtual void	Unk_7E(void) = 0;
	virtual void	Unk_7F(void) = 0;
	virtual void	Unk_80(void) = 0;
	virtual void	Unk_81(void) = 0;
	virtual void	Unk_82(void) = 0;
	virtual UInt8	GetIsAlerted();
	virtual void	SetIsAlerted(UInt8 IsAlerted);
	virtual void	Unk_85(void) = 0;
	virtual void	Unk_86(void) = 0;
	virtual void	Unk_87(void) = 0;
	virtual void	Unk_88(void) = 0;
	virtual void	Unk_89(void) = 0;
	virtual void	Unk_8A(void) = 0;
	virtual void	Unk_8B(void) = 0;
	virtual void	Unk_8C(void) = 0;
	virtual void	Unk_8D(void) = 0;
	virtual void	Unk_8E(void) = 0;
	virtual void	Unk_8F(void) = 0;
	virtual void	Unk_90(void) = 0;
	virtual UInt8	GetUnk020(void) = 0;
	virtual void	SetUnk020(UInt8 arg) = 0;
	virtual void	Unk_93(void) = 0;
	virtual void	Unk_94(void) = 0;
	virtual void	Unk_95(void) = 0;
	virtual void	Unk_96(void) = 0;
	virtual void	Unk_97(void) = 0;
	virtual void	Unk_98(void) = 0;
	virtual void	Unk_99(void) = 0;
	virtual void	Unk_9A(void) = 0;
	virtual void	Unk_9B(void) = 0;
	virtual void	Unk_9C(void) = 0;
	virtual void	Unk_9D(void) = 0;
	virtual void	Unk_9E(void) = 0;
	virtual void	Unk_9F(void) = 0;
	virtual void	Unk_A0(void) = 0;
	virtual void	Unk_A1(void) = 0;
	virtual void	Unk_A2(void) = 0;
	virtual void	Unk_A3(void) = 0;
	virtual void	Unk_A4(void) = 0;
	virtual void	Unk_A5(void) = 0;
	virtual void	Unk_A6(void) = 0;
	virtual void	Unk_A7(void) = 0;
	virtual void	Unk_A8(void) = 0;
	virtual void	Unk_A9(void) = 0;
	virtual MagicItem	* GetQueuedMagicItem(void) = 0;	// returns MiddleHighProcess+144
	virtual void	Unk_AB(void) = 0;
	virtual void	Unk_AC(void) = 0;
	virtual void	Unk_AD(void) = 0;
	virtual void	Unk_AE(void) = 0;
	virtual UInt32	GetMovementFlags();
	virtual void	Unk_B0(void) = 0;
	virtual void	Unk_B1(void) = 0;
	virtual void	Unk_B2(void) = 0;
	virtual SInt16					GetCurrentAction();
	virtual BSAnimGroupSequence*	GetCurrentActionAnimSequence();
	virtual SInt16					SetCurrentAction(SInt16 action, BSAnimGroupSequence* sequence);
	virtual void	Unk_B6(void) = 0;
	virtual void	Unk_B7(void) = 0;
	virtual UInt8					GetKnockedState();
	virtual UInt8					SetKnockedState(UInt8 KnockedState);
	virtual void	Unk_BA(void) = 0;
	virtual void	Unk_BB(void) = 0;
	virtual void	KnockbackActor(Actor* target, float arg1, float arg2, float arg3, float arg4) = 0; // arg3 is a multiplier, arg4 appears to be base force to apply
	virtual void	Unk_BD(void) = 0;
	virtual UInt8	GetCombatMode();
	virtual UInt8	SetCombatMode(UInt8 CombatMode);
	virtual UInt8	GetWeaponOut();
	virtual UInt8	SetWeaponOut(UInt8 WeaponOut);
	virtual void	Unk_C2(void) = 0;
	virtual void *	Unk_C3(void) = 0;	// returns some pointer
	virtual void	Unk_C4(void) = 0;
	virtual void	Unk_C5(void) = 0;
	virtual void	SetUnk16C(UInt8 arg);
	virtual UInt8	GetUnk16C();
	virtual float	GetUnk0F8();
	virtual void	SetUnk0F8(float arg);
	virtual void	Unk_CA(void) = 0;
	virtual void	Unk_CB(void) = 0;
	virtual void	Unk_CC(void) = 0;
	virtual void	Unk_CD(void) = 0;
	virtual void	Unk_CE(void) = 0;
	virtual void	Unk_CF(void) = 0;
	virtual void	Unk_D0(void) = 0;
	virtual void	Unk_D1(void) = 0;
	virtual void	Unk_D2(void) = 0;
	virtual void	Unk_D3(void) = 0;
	virtual void	Unk_D4(void) = 0;
	virtual void	Unk_D5(void) = 0;
	virtual void	Unk_D6(void) = 0;
	virtual void	Unk_D7(void) = 0;
	virtual void	Unk_D8(void) = 0;
	virtual void	Unk_D9(void) = 0;
	virtual UInt8	GetSitSleepState();
	virtual void	Unk_DB(void) = 0;
	virtual void	Unk_DC(void) = 0;
	virtual TESObjectREFR*	GetFurniture();
	virtual void	Unk_DE(void) = 0;
	virtual void	Unk_DF(void) = 0;
	virtual void	Unk_E0(void) = 0;
	virtual void	Unk_E1(void) = 0;
	virtual void	Unk_E2(void) = 0;
	virtual UInt8	GetUnk01C();
	virtual void	SetUnk01C(UInt8 arg);
	virtual UInt8	GetUnk180();
	virtual void	SetUnk180(UInt8 arg);
	virtual void	Unk_E7(void) = 0;
	virtual void	Unk_E8(void) = 0;
	virtual void	Unk_E9(void) = 0;
	virtual float	GetLightAmount(Actor * actor, UInt32 unk1) = 0;
	virtual void	Unk_EB(void) = 0;
	virtual void	* GetDetectionState(Actor* target) = 0;	// returns HighProcess::DetectionList::Data *
	virtual void	Unk_ED(void) = 0;
	virtual void	Unk_EE(void) = 0;
	virtual void	Unk_EF(void) = 0;
	virtual void	Unk_F0(void) = 0;
	virtual void	Unk_F1(void) = 0;
	virtual void	Unk_F2(void) = 0;
	virtual void	Unk_F3(void) = 0;
	virtual void	Unk_F4(void) = 0;
	virtual void	Unk_F5(void) = 0;
	virtual void	Unk_F6(void) = 0;
	virtual void	Unk_F7(void) = 0;
	virtual void	Unk_F8(void) = 0;
	virtual void	Unk_F9(void) = 0;
	virtual void	Unk_FA(void) = 0;
	virtual void	Unk_FB(void) = 0;
	virtual void	Unk_FC(void) = 0;
	virtual void	Unk_FD(void) = 0;
	virtual void	Unk_FE(void) = 0;
	virtual void	Unk_FF(void) = 0;
	virtual void	Unk_100(void) = 0;
	virtual void	Unk_101(void) = 0;
	virtual PathLow * CreatePath(void) = 0;
	virtual PathLow	* GetCurrentPath(void) = 0;
	virtual void	Unk_104(void) = 0;
	virtual void	Unk_105(void) = 0;
	virtual void	Unk_106(void) = 0;
	virtual void	Unk_107(void) = 0;
	virtual void	Unk_108(void) = 0;
	virtual void	Unk_109(void) = 0;
	virtual void	Unk_10A(void) = 0;
	virtual void	Unk_10B(void) = 0;
	virtual void	Unk_10C(void) = 0;
	virtual void	Unk_10D(void) = 0;
	virtual void	Unk_10E(void) = 0;
	virtual void	Unk_10F(void) = 0;
	virtual void	Unk_110(void) = 0;
	virtual void	Unk_111(void) = 0;
	virtual void	Unk_112(void) = 0;
	virtual void	Unk_113(void) = 0;
	virtual void	Unk_114(void) = 0;
	virtual void	Unk_115(void) = 0;
	virtual void	Unk_116(void) = 0;
	virtual void	Unk_117(void) = 0;
	virtual void	Unk_118(void) = 0;
	virtual void	Unk_119(void) = 0;
	virtual void	Unk_11A(void) = 0;
	virtual void	Unk_11B(void) = 0;
	virtual void	Unk_11C(void) = 0;
	virtual void	Unk_11D(void) = 0;
	virtual void	Unk_11E(void) = 0;
	virtual void	Unk_11F(void) = 0;
	virtual void	Unk_120(void) = 0;
	virtual void	Unk_121(void) = 0;
	virtual void	Unk_122(void) = 0;
	virtual void	Unk_123(void) = 0;
	virtual void	Unk_124(void) = 0;
	virtual void	Unk_125(void) = 0;
	virtual void	Unk_126(void) = 0;
	virtual void	Unk_127(void) = 0;
	virtual void	Unk_128(void) = 0;
	virtual void	Unk_129(void) = 0;
	virtual void	Unk_12A(void) = 0;
	virtual void	Unk_12B(void) = 0;
	virtual void	Unk_12C(void) = 0;
	virtual void	Unk_12D(void) = 0;
	virtual void	Unk_12E(void) = 0;
	virtual void	Unk_12F(void) = 0;
	virtual void	Unk_130(void) = 0;
	virtual void	Unk_131(void) = 0;
	virtual void	Unk_132(void) = 0;
	virtual void	Unk_133(void) = 0;
	virtual const char *	Unk_134(void) = 0;
	virtual void	Unk_135(void) = 0;
	virtual void	Unk_136(void) = 0;
	virtual void	Unk_137(void) = 0;
	virtual void	Unk_138(void) = 0;
	virtual void	Unk_139(void) = 0;
	virtual void	Unk_13A(void) = 0;
	virtual float	GetUnk08C(void) = 0;
	virtual void	SetUnk08C(float arg) = 0;
	virtual void	Unk_13D(void) = 0;
	virtual void	Unk_13E(void) = 0;
	virtual void	Unk_13F(void) = 0;
	virtual void	Unk_140(void) = 0;

//	void					** _vtbl;				// 000
	TESPackage::eProcedure	editorPackProcedure;	// 004
	TESPackage				* editorPackage;		// 008
};

// 90
class LowProcess : public BaseProcess
{
public:
	enum
	{
		//	BaseProcess flags

		//	
	};

	LowProcess();
	~LowProcess();

	virtual void	Unk_141(void) = 0;
	virtual void	Unk_142(void) = 0;
	virtual void	Unk_143(void) = 0;
	virtual void	Unk_144(void) = 0;
	virtual void	Alarm(Actor* Act);
	virtual void	Unk_146(void) = 0;
	virtual void	Unk_147(void) = 0;
	virtual void	Unk_148(void) = 0;
	virtual void	Unk_149(void) = 0;
	virtual void	Unk_14A(void) = 0;
	virtual void	Unk_14B(void) = 0;
	virtual void	Unk_14C(void) = 0;
	virtual void	Unk_14D(void) = 0;
	virtual void	Unk_14E(void) = 0;
	virtual void	Unk_14F(void) = 0;
	virtual void	Unk_150(void) = 0;
	virtual void	Unk_151(void) = 0;
	virtual void	Unk_152(void) = 0;
	virtual void	Unk_153(void) = 0;
	virtual void	Unk_154(void) = 0;
	virtual void	Unk_155(void) = 0;
	virtual void	Unk_156(void) = 0;
	virtual UInt8	MountHorse(Actor* Act);				// 560
	virtual UInt8	DismountHorse(Actor* Act);			// 564
	virtual void	Unk_159(void) = 0;

	float	unk00C;				// 00C - initialized to -1
	float	unk010;				// 010
	float	curHour;			// 014 - initialized to -1
	UInt32	curPackedDate;		// 018 - (year << 13) | (month << 9) | day
	UInt8	unk01C;				// 01C
	UInt8	unk01D;				// 01D
	UInt8	unk01E;				// 01E
	UInt8	IsAlerted;			// 01F
	UInt8	unk020;				// 020
	UInt8	pad021[3];			// 021
	TESForm * usedItem;			// 024 for idles like reading book, making potions, etc
	float	unk028;				// 028
	Actor*	Follow;				// 02C
	TESObjectREFR   * unk030;	// 030 seen XMarkerHeading refs here
	PathLow	* pathing;			// 034
	UInt32	unk038;				// 038
	UInt32	unk03C;				// 03C
	UInt32	unk040;				// 040
	UInt32	unk044;				// 044
	UInt32	unk048;				// 048
	UInt32	unk04C;				// 04C
	UInt32	unk050;				// 050
	UInt32	unk054;				// 054
	UInt32	unk058;				// 058
	UInt32	unk05C;				// 05C
	UInt32	unk060;				// 060
	UInt32	unk064;				// 064
	UInt32	unk068;				// 068
	UInt32	unk06C;				// 06C
	ActorValues	avDamageModifiers;	// 070
	UInt8	unk084;				// 084
	UInt8	pad085[3];			// 085
	float	unk088;				// 088 - counter in seconds
	float	unk08C;				// 08C
};

STATIC_ASSERT(sizeof(LowProcess) == 0x90);

// A8
class MiddleLowProcess : public LowProcess
{
public:
	MiddleLowProcess();
	~MiddleLowProcess();

	virtual void	Unk_15A(void) = 0;

	UInt32			unk090;				// 090
	ActorValues		maxAVModifiers;		// 094
};

STATIC_ASSERT(sizeof(MiddleLowProcess) == 0xA8);

class AnimSequenceBase
{
public:
	AnimSequenceBase();
	~AnimSequenceBase();
	
	virtual void					Destructor(UInt8 Arg);
	virtual void					AddAnimGroupSequence(BSAnimGroupSequence* AnimGroupSequence);
	virtual void					Unk_02(void) = 0;
	virtual UInt8					IsSingle();
	virtual BSAnimGroupSequence*	GetAnimGroupSequence(int Index); // Index is not used if Single (returns the anim); Index = -1 returns a random anim in the NiTList<BSAnimGroupSequence>* for Multiple
	virtual void					Unk_05(void) = 0;
};

class AnimSequenceSingle : public AnimSequenceBase
{
public:
	AnimSequenceSingle();
	~AnimSequenceSingle();
	
	BSAnimGroupSequence*	Anim;	// 04
};

class AnimSequenceMultiple : public AnimSequenceBase
{
public:
	AnimSequenceMultiple();
	~AnimSequenceMultiple();
	
	NiTList<BSAnimGroupSequence>*	Anims;	// 04
};

// DC
class ActorAnimData
{
public:
	ActorAnimData();
	~ActorAnimData();

	UInt32					unk00;					//00 no virtual table
	NiNode*					niNode04;				//04 seen BSFadeNode for 3rd Person, NiNode for 1st
	NiNode*					AccumNode;				//08
	UInt32					unk0C[6];				//0C
	NiNode*					nBip01;					//24
	NiNode*					nLForearm;				//28
	NiNode*					nTorch;					//2C
	NiNode*					nWeapon;				//30
	NiNode*					nHead;					//34
	UInt32					unk38;					//38
	UInt16					animsMapKey[5];			//3C
	UInt16					unk44;					//44
	int						unk48State[5];			//48
	UInt32					unk4C[15];				//4C
	NiControllerManager*	manager;				//98
	NiTPointerMap<AnimSequenceBase>*	animsMap;	//9C
	BSAnimGroupSequence*	animSequences[5];		//A0
	ModelKF*				modelB4;				//B4
	ModelKF*				modelB8;				//B8
	float					unkBC;					//BC
	float					unkC0;					//C0
	float					unkC4;					//C4
	UInt32					unkC8[3];				//C8
	UInt32					unkD4;					//D4
	void*					unkD8;					//D8 looks like struct with idle anim transform data

	bool FindAnimInRange(UInt32 lowBound, UInt32 highBound = -1);
	bool PathsInclude(const char* subString);
};

STATIC_ASSERT(sizeof(ActorAnimData) == 0xDC);

// 18C
class MiddleHighProcess : public MiddleLowProcess
{
public:
	MiddleHighProcess();
	~MiddleHighProcess();

	virtual void	Unk_15B(void) = 0;
	virtual void	Unk_15C(void) = 0;
	virtual void	Unk_15D(void) = 0;
	virtual void	Unk_15E(void) = 0;
	virtual void	Unk_15F(void) = 0;
	virtual void	Dialogue(Actor* Act);
	virtual void	RemoveWornItems(Actor* Act, UInt8 Arg2, int Arg3);
	virtual void	Travel(Actor* Act, UInt8 Arg2, float Arg3, int Arg4 = 0);
	virtual void	Unk_163(void) = 0;
	virtual void	Unk_164(void) = 0;

	// 10
	struct Unk128
	{
		UInt32	unk0;	// 0
		UInt32	unk4;	// 4
		UInt32	unk8;	// 8
		UInt16	unkC;	// C
		UInt8	unkE;	// E
		UInt8	padF;	// F
	};

	// 8
	struct EffectListNode
	{
		ActiveEffect	* effect;	// 0
		EffectListNode	* next;		// 4
	};

	UInt32				unk0A8;		// 0A8
	UInt32				unk0AC;		// 0AC
	UInt32				unk0B0;		// 0B0
	UInt32				unk0B4;		// 0B4
	UInt32				unk0B8;		// 0B8
	float				unk0BC;		// 0BC
	TESPackage			* currentPackage;	// 0C0 if null, editorPackage applies
	UInt32				unk0C4;		// 0C4
	UInt8				unk0C8;		// 0C8
	UInt8				pad0C9[3];	// 0C9
	TESPackage::eProcedure	currentPackProcedure; // 0CC
	UInt8				unk0D0;		// 0D0
	UInt8				pad0D0[15];	// 0D1 - never initialized
	UInt32				unk0E0;		// 0E0
	ExtraContainerChanges::EntryData* equippedWeaponData;	// 0E4
	ExtraContainerChanges::EntryData* equippedLightData;	// 0E8
	ExtraContainerChanges::EntryData* equippedAmmoData;		// 0EC
	ExtraContainerChanges::EntryData* equippedShieldData;   // 0F0
	UInt8				unk0F4;		// 0F4
	UInt8				unk0F5;		// 0F5
	UInt8				pad0F6[2];	// 0F6
	float				unk0F8;		// 0F8
	NiNode*				WeaponNode;		// 0FC
	NiNode*				TorchNode;		// 100
	NiNode*				LForearmTwistNode;	// 104
	NiNode*				SideWeaponNode;	// 108
	NiNode*				QuiverNode;		// 10C
	NiNode*				unk110;		// 110
	UInt8				CombatMode;	// 114
	UInt8				WeaponState;// 115
	UInt8				pad116[2];	// 116
	void	* charProxy;	// 118 - seen bhkCharacterProxy
	UInt8				KnockedState;	// 11C
	UInt8				SitSleepState;	// 11D
	UInt8				pad11E;		// 11E
	UInt8				pad11F;		// 11F
	TESObjectREFR*		Furniture;	// 120
	UInt8				unk124;		// 124 - init'd to 0x7F
	Unk128				unk128;		// 128
	UInt16				unk138;		// 138
	UInt8				pad13A[2];	// 13A
	UInt32				unk13C;		// 13C
	UInt32				unk140;		// 140
	MagicItem			* queuedMagicItem;	// 144 set before calling sub_69AF30 after Addspell cmd, unset upon return
	UInt32				unk148;		// 148
	UInt8				unk14C;		// 14C looks like true if casting, or possibly a casting state
	UInt8				pad14D[3];	// 14D
	UInt32				unk150;		// 150
	float				actorAlpha;	// 154 valid values 0-1
	float				unk158;		// 158
	NiExtraData			* unk15C;	// 15C seen BSFaceGenAnimationData*, reset when modifying character face
	UInt8				unk160;		// 160
	UInt8				unk161;		// 161
	UInt8				pad162[2];	// 162
	UInt32				unk164;		// 164
	UInt8				unk168;		// 168
	UInt8				unk169;		// 169
	UInt8				unk16A;		// 16A
	UInt8				unk16B;		// 16B
	UInt8				unk16C;		// 16C
	UInt8				unk16D;		// 16D
	UInt8				pad16E[2];	// 16E
	UInt32				unk170;		// 170
	EffectListNode		* effectList;	// 174
	UInt32				unk178;		// 178
	ActorAnimData		* animData;	// 17C
	UInt8				unk180;		// 180
	UInt8				pad181[3];	// 181
	NiObject			* unk184;	// 184 - seen BSShaderPPLightingProperty
	BSBound				* boundingBox;	// 188

	bhkCharacterController* GetCharacterController();
};

STATIC_ASSERT(sizeof(MiddleHighProcess) == 0x18C);

// 2EC
class HighProcess : public MiddleHighProcess
{
public:
	HighProcess();
	~HighProcess();

	enum
	{
		kActionType_Default = 0,
		kActionType_Action,
		kActionType_Script,
		kActionType_Combat,
		kActionType_Dialog,

		kActionType_Max
	};

	enum {
		kDetectionState_Lost = 0,
		kDetectionState_Unseen,
		kDetectionState_Noticed,
		kDetectionState_Seen,

		kDetectionState_Max
	};

	// 8
	struct DetectionList
	{
		struct Data
		{
			Actor			* actor;
			UInt8			detectionState;
			UInt8			pad04[3];
			UInt8			hasLOS;
			UInt8			pad08[3];
			SInt32			detectionLevel;
		};

		Data			* data;
		DetectionList	* next;

		Data* Info() const { return data; }
		DetectionList* Next() const { return next; }
	};

	typedef Visitor<DetectionList, DetectionList::Data> DetectionListVisitor;

	// this appears to be a common linked list class
	// 4
	struct Node190
	{
		void	* data;
		Node190	* next;
	};

	// 10
	struct Unk1BC
	{
		UInt32	unk0;
		UInt32	unk4;
		UInt32	unk8;
		UInt32	unkC;
	};

	// C
	struct Unk20C
	{
		UInt32	unk0;
		UInt32	unk4;
		UInt32	unk8;
	};

	// C
	struct Unk27C
	{
		UInt32	unk0;
		UInt32	unk4;
		UInt32	unk8;
	};

	enum{
		kAction_None			= -1,
		kAction_EquipWeapon,
		kAction_UnequipWeapon,
		kAction_Attack,
		kAction_AttackFollowThrough,
		kAction_AttackBow,
		kAction_AttackBowArrowAttached,
		kAction_Block,
		kAction_Recoil,
		kAction_Stagger,
		kAction_Dodge,
		kAction_LowerBodyAnim,
		kAction_SpecialIdle,
		kAction_ScriptAnimation,
	};

	enum
	{
		kMovement_Forward =		0x0001,
		kMovement_Backward =	0x0002,
		kMovement_Left =		0x0004,
		kMovement_Right =		0x0008,
		kMovement_TurnLeft =	0x0010,
		kMovement_TurnRight =	0x0020,

		kMovement_Walk =		0x0100,
		kMovement_Run =			0x0200,
		kMovement_Sneak =		0x0400,	// overridden by kMovementFlag_Swimming
		kMovement_Swim =		0x0800,
		kMovement_Jump =		0x1000, //Jump and above appear not to be used.
		kMovement_Fly =			0x2000,
		kMovement_Fall =		0x4000,
		kMovement_Slide =		0x8000,
	};

	DetectionList	* detectionList;	// 18C
	Node190	unk190;		// 190
	UInt32	unk198;		// 198
	float	unk19C;		// 19C - idle chatter comment timer
	UInt8	unk1A0;		// 1A0
	UInt8	pad1A1[3];	// 1A1
	UInt32	unk1A4;		// 1A4
	UInt32	unk1A8;		// 1A8
	UInt32	unk1AC;		// 1AC
	UInt32	unk1B0;		// 1B0 - uninitialized
	UInt32	unk1B4;		// 1B4 - uninitialized
	UInt32	unk1B8;		// 1B8
	Unk1BC	unk1BC;		// 1BC
	UInt32	unk1CC;		// 1CC - uninitialized
	UInt8	unk1D0;		// 1D0
	UInt8	unk1D1;		// 1D1
	UInt32	unk1D4;		// 1D4
	UInt32	unk1D8;		// 1D8
	UInt32	unk1DC;		// 1DC
	UInt32	unk1E0;		// 1E0
	UInt8	unk1E4;		// 1E4
	UInt8	pad1E5[3];	// 1E5
	UInt32	unk1E8;		// 1E8
	NiBSBoneLODController*	LODController;		// 1EC
	UInt32	unk1F0;								// 1F0
	SInt16					currentAction;		// 1F4
	UInt8	pad1F6[2];							// 1F6
	BSAnimGroupSequence*	currentActionAnim;	// 1F8
	UInt16	movementFlags;						// 1FC
	UInt8	pad1FE[2];	// 1FE
	UInt32	unk200;		// 200
	float	unk204;		// 204
	UInt16	unk208;		// 208
	UInt8	pad20A[2];	// 20A
	Unk20C	unk20C;		// 20C
	UInt32	unk218;		// 218
	float	unk21C;		// 21C
	UInt32	unk220;		// 220
	UInt32  lightSound; // 224 TESGameSound* (ITMTorchHeldLP sound). It is set/removed when equipping/unequipping torch.
	UInt8	unk228;		// 228
	UInt8	pad229[3];	// 229
	float	unk22C;		// 22C
	float	unk230;		// 230 - initialized to ((rand() % 5000) * .001) + 10
	UInt32	unk234;		// 234 - not initialized
	float	swimBreath;	// 238 - initialized to 0x41A00000
	UInt8	unk23C;		// 23C
	UInt8	unk23D[3];	// 23D
	UInt32	unk240;		// 240
	UInt8	unk244;		// 244
	UInt8	unk245[3];	// 245
	float	unk248;		// 248
	UInt8	unk24C;		// 24C
	UInt8	pad24D[3];	// 24D
	UInt32	unk250;		// 250
	UInt32	unk254;		// 254
	UInt32	unk258;		// 258
	UInt8	unk25C;		// 25C
	UInt8	unk25D;		// 25D
	UInt8	pad25E[2];	// 25E
	float	unk260;		// 260
	UInt32	unk264;		// 264
	NiObject	* unk268;	// 268 - decref'able pointer
	float	unk26C;		// 26C
	UInt32	unk270;		// 270
	UInt32	unk274;		// 274
	UInt8	unk278;		// 278
	UInt8	pad279[3];	// 279
	Unk27C	unk27C;		// 27C
	UInt32	unk288;		// 288
	UInt32	unk28C;		// 28C
	UInt8	unk290;		// 290
	UInt8	pad291[3];	// 291
	float	unk294;		// 294
	UInt32	unk298;		// 298
	UInt32	unk29C;		// 29C
	UInt32	unk2A0;		// 2A0
	UInt32	unk2A4;		// 2A4
	UInt8	unk2A8;		// 2A8
	UInt8	unk2A9;		// 2A9
	UInt8	pad2AA[2];	// 2AA
	float	unk2AC;		// 2AC
	float	unk2B0;		// 2B0
	UInt32	unk2B4;		// 2B4
	UInt8	unk2B8;		// 2B8
	UInt8	unk2B9;		// 2B9
	UInt8	pad2BA[2];	// 2BA
	UInt32	unk2BC;		// 2BC
	float	unk2C0;		// 2C0
	TESObjectREFR* unk2C4;	// 2C4
	TESObjectREFR* actionTarget[kActionType_Max];	// 2C8
	UInt8			actionActive[kActionType_Max];		// 2DC - true if the specified action is running
	UInt8	pad2E1[3];	// 2E1
	TESObjectREFR* unk2E4;	// 2E4
	UInt8	unk2E8;		// 2E8
	UInt8	pad2E9[3];	// 2E9

	bool IsAttacking()
		{	return currentAction >= kAction_Attack && currentAction <= kAction_AttackBowArrowAttached;	}
	bool IsBlocking()
		{	return currentAction == kAction_Block;	}
	bool IsRecoiling()
		{	return currentAction == kAction_Recoil;	}
	bool IsStaggered()
		{	return currentAction == kAction_Stagger;	}
	bool IsDodging()
		{	return currentAction == kAction_Dodge;	}
	bool IsMovementFlagSet(UInt32 flag)
		{	return (movementFlags & flag) == flag;	}
};

STATIC_ASSERT(sizeof(HighProcess) == 0x2EC);

class SkinInfo
{
public:
	SkinInfo();
	~SkinInfo();

	NiNode*			Bip01Node;				// 000
	UInt32			unk004;
	NiNode*			HeadNode;				// 008
	UInt32			unk00C;
	NiNode*			Finger1Node;			// 010
	UInt32			unk014;
	NiNode*			LFinger1Node;			// 018
	UInt32			unk01C;
	NiNode*			WeaponNode;				// 020
	UInt32			unk024;
	NiNode*			BackWeaponNode;			// 028
	UInt32			unk02C;
	NiNode*			SideWeaponNode;			// 030
	UInt32			unk034;
	NiNode*			QuiverNode;				// 038
	UInt32			unk03C;
	NiNode*			LForearmTwistNode;		// 040
	UInt32			unk044;
	NiNode*			TorchNode;				// 048
	UInt32			unk04C;
	UInt32			unk050;
	Actor*			Actor054;				// 054
	UInt32			unk058;
	TESForm*		unk05C;
	TESModel*		unk060;
	NiNode*			unk064;
	UInt32			unk068;
	TESForm*		UpperBodyForm;			// 06C
	TESModel*		UpperBodyModel;			// 070
	NiNode*			UpperBodyObject;		// 074
	UInt32			unk078;
	TESForm*		LowerBodyForm;			// 07C
	TESModel*		LowerBodyModel;			// 080
	NiNode*			LowerBodyObject;		// 084
	UInt32			unk088;
	TESForm*		HandForm;				// 08C
	TESModel*		HandModel;				// 090
	NiNode*			HandObject;				// 094
	UInt32			unk098;
	TESForm*		FootForm;				// 09C
	TESModel*		FootModel;				// 0A0
	NiNode*			FootObject;				// 0A4
	UInt32			unk0A8;
	UInt32			unk0AC;
	UInt32			unk0B0;
	NiNode*			unk0B4;
	UInt32			unk0B8;
	UInt32			unk0BC;
	UInt32			unk0C0;
	NiNode*			unk0C4;
	UInt32			unk0C8;
	UInt32			unk0CC;
	UInt32			unk0D0;
	NiNode*			unk0D4;
	UInt32			unk0D8;
	TESObjectWEAP*	WeaponForm;				// 0DC
	TESModel*		WeaponModel;			// 0E0
	NiNode*			WeaponObject;			// 0E4
	UInt32			unk0E8;
	TESForm*		unk0EC;
	TESModel*		unk0F0;
	NiNode*			unk0F4;
	UInt32			unk0F8;
	TESForm*		unk0FC;
	TESModel*		unk100;
	NiNode*			unk104;
	UInt32			unk108;
	TESForm*		AmmoForm;				// 10C
	TESModel*		AmmoModel;				// 110
	NiNode*			AmmoObject;				// 114
	UInt32			unk118;
	TESObjectARMO*	ShieldForm;				// 11C
	TESModel*		ShieldModel;			// 120
	NiNode*			ShieldObject;			// 124
	UInt32			unk128;
	TESObjectLIGH*	LightForm;				// 12C
	TESModel*		LightModel;				// 130
	NiNode*			LightObject;			// 134
	UInt32			unk138;
	UInt32			unk13C;
	UInt32			unk140;
	UInt32			unk144;
	UInt32			unk148;
	UInt32			unk14C;
	Actor*			Actor150;				// 150
};

STATIC_ASSERT(sizeof(SkinInfo) == 0x154);

// 14
class AStarWorldNode
{
public:
	AStarWorldNode();
	~AStarWorldNode();

	UInt32			id;					// 00
	TESObjectREFR	* fromDoorRef;		// 04
	TESForm			* from;				// 08 TESObjectCELL, TESWorldSpace, TESSubspace refr
	TESObjectREFR	* toDoorRef;		// 0C
	TESForm			* to;				// 10 as with from

	typedef tList<AStarWorldNode>	List;
};

// A "low" path is one that traverses a graph where each node is a cell, worldspace, or subspace, with edges defined by load doors
// Used whenever an actor's destination is outside of its current cell/space, regardless of actor's process level
typedef NiTPointerMap <AStarWorldNode::List>	AStarCellNodeMap;		// key is TESForm* (cell, worldspace, subspace)
typedef NiTPointerMap <AStarCellNodeMap>		LowPathWorld;			// key is TESForm* (cell, worldspace, subspace)

typedef NiTListBase<AStarWorldNode>	AStarWorldNodeList;