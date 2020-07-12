#pragma once

#include "obse/GameForms.h"
#include "obse/GameExtraData.h"
#include "obse/GameProcess.h"
#include "obse/GameTypes.h"
#include "obse/NiObjects.h"
#include "Utilities.h"
#include <vector>

/*** class hierarchy
 *	
 *	this information comes from the RTTI information embedded in the exe
 *	so no, I don't have magical inside information
 *	
 *	sadly bethesda decided to use /lots/ of multiple inheritance, so this is
 *	going to be very difficult to access externally.
 *	
 *	it'll probably be best to expose all of the rtti structs and the dynamic cast interface I guess
 *	
 *	TESObjectREFR can be directly cast to Character
 *	
 *													total vtbl size
 *	BaseFormComponent
 *		TESForm										0x37
 *			TESObjectREFR							0x69
 *				MobileObject						0x81
 *					Actor							0xEF
 *						Character					0xEF
 *							PlayerCharacter			0xEF
 *						Creature					0xEF
 *					ArrowProjectile					0x81
 *					MagicProjectile					0x89
 *						MagicBallProjectile			0x89
 *						MagicBoltProjectile			0x89
 *						MagicFogProjectile			0x89
 *						MagicSprayProjectile		0x89
 *			TESPackage
 *				FleePackage
 *				DialogPackage
 *				CombatController
 *				AlarmPackage
 *				SpectatorPackage
 *				TresspassPackage
 *			TESTopicInfo
 *			TESTopic
 *			TESSkill
 *			TESRace
 *			TESQuest
 *			TESObject
 *				TESBoundObject
 *					TESBoundAnimObject
 *						TESActorBase
 *							TESNPC
 *							TESCreature
 *						TESObjectCONT
 *					TESObjectMISC
 *					TESObjectBOOK
 *					TESLevItem
 *			Script
 *		TESFullName
 *			TESTopic
 *			TESRace
 *			TESQuest
 *			TESObject
 *		TESDescription
 *			TESSkill
 *			TESRace
 *		TESTexture
 *			TESSkill
 *			TESIcon
 *				TESQuest
 *		TESSpellList
 *			TESRace
 *			TESObject
 *		TESReactionForm
 *			TESRace
 *		TESScriptableForm
 *			TESQuest
 *			TESObject
 *		TESActorBaseData
 *			TESObject
 *		TESContainer
 *			TESObject
 *		TESAIForm
 *			TESObject
 *		TESHealthForm
 *			TESObject
 *		TESAttributes
 *			TESObject
 *		TESAnimation
 *			TESObject
 *		TESModel
 *			TESObject
 *		TESRaceForm
 *			TESObject
 *		TESGlobal
 *	
 *	TESMemContextForm
 *		TESObjectREFR
 *			...
 *	
 *	TESChildCell
 *		MobileObject
 *			...
 *	
 *	MagicCaster
 *		Actor
 *			...
 *		NonActorMagicCaster
 *			BSExtraData
 *	
 *	MagicTarget
 *		Character
 *			...
 *		NonActorMagicTarget
 *			BSExtraData
 *	
 *	BaseProcess
 *		LowProcess
 *			MiddleLowProcess
 *				MiddleHighProcess
 *					HighProcess
 *	
 *	IOTask
 *		LipTask
 *		SkyTask
 *	
 *	NiRefObject
 *		NiObject
 *			NiTimeController
 *				BSPlayerDistanceCheckController
 *				BSDoorHavokController
 *			NiExtraData
 *				Tile::Extra
 *				DebugTextExtraData
 *				BSFaceGenBaseMorphExtraData
 *				BSFaceGenModelExtraData
 *				BSFaceGenAnimationData
 *			BSTempEffect
 *				BSTempEffectParticle
 *				BSTempEffectGeometryDecal
 *				BSTempEffectDecal
 *				MagicHitEffect
 *					MagicModelHitEffect
 *					MagicShaderHitEffect
 *			NiSkinInstance
 *			NiTask
 *				BSTECreateTask
 *			bhkRefObject
 *				bhkSerializable
 *					bhkShape
 *						bhkSphereRepShape
 *							bhkConvexShape
 *								bhkBoxShape
 *								bhkCapsuleShape
 *							bhkMultiSphereShape
 *						bhkTransformShape
 *			NiObjectNET
 *				NiAVObject
 *					NiNode
 *						BSTreeNode
 *						BSFaceGenNiNode
 *			NiCollisionObject
 *				bhkNiCollisionObject
 *					bhkPCollisionObject
 *						bgkSPCollisionObject
 *					bhkCollisionObject
 *						bhkBlendCollisionObject
 *							WeaponObject
 *		BSTreeModel
 *		BSFaceGenMorphDataHair
 *			BSFaceGenMorphDataHead
 *		BSFaceGenModel
 *	
 *	BSFaceGenMorph
 *		BSFaceGenMorphStatistical
 *		BSFaceGenMorphDifferential
 *	
 *	Menu
 *		VideoDisplayMenu
 *		TrainingMenu
 *		StatsMenu
 *		SpellPurchaseMenu
 *		SpellMakingMenu
 *		SleepWaitMenu
 *		SkillsMenu
 *		SigilStoneMenu
 *		SaveMenu
 *		RepairMenu
 *		RechargeMenu
 *		RaceSexMenu
 *		QuickKeysMenu
 *		QuantityMenu
 *		PersuasionMenu
 *		PauseMenu
 *		OptionsMenu
 *		NegotiateMenu
 *		MessageMenu
 *		MapMenu
 *		MainMenu
 *		MagicPopupMenu
 *		MagicMenu
 *		LockPickMenu
 *		LoadgameMenu
 *		LoadingMenu
 *		LevelUpMenu
 *		InventoryMenu
 *		HUDSubtitleMenu
 *		HUDReticle
 *		HUDMainMenu
 *		HUDInfoMenu
 *		GenericMenu
 *		GameplayMenu
 *		EnchantmentMenu
 *		EffectSettingMenu
 *		DialogMenu
 *		CreditsMenu
 *		ContainerMenu
 *		ClassMenu
 *		BreathMenu
 *		BookMenu
 *		AudioMenu
 *		AlchemyMenu
 *		VideoMenu
 *		TextEditMenu
 *		ControlsMenu
 *	
 *	Tile
 *		TileWindow
 *		TileRect
 *			TileMenu
 *		Tile3D
 *		TileText
 *		TileImage
 *	
 *	BackgroundLoader
 *		BSFaceGenManager
 *	
 *	BSFaceGenKeyframe
 *		BSFaceGenKeyframeMultiple	14
 *	
 *	SkyObject
 *		Sun
 *		Stars
 *		Moon
 *		Clouds
 *		Atmosphere
 *	
 *	Sky
 *	
 *	PathLow
 *		PathMiddleHigh
 *			PathHigh
 *	
 *	ActiveEffect
 *		ValueModifierEffect
 *			AbsorbEffect
 *			CalmEffect
 *			ChameleonEffect
 *			DarknessEffect
 *			DetectLifeEffect
 *			FrenzyEffect
 *			InvisibilityEffect
 *			NightEyeEffect
 *			ParalysisEffect
 *			ShieldEffect
 *			TelekinesisEffect
 *		AssociatedItemEffect
 *			BoundItemEffect
 *			SummonCreatureEffect
 *		CommandEffect
 *			CommandCreatureEffect
 *			CommandHumanoidEffect
 *		CureEffect
 *		DemoralizeEffect
 *		DisintegrateArmorEffect
 *		DisintegrateWeaponEffect
 *		DispelEffect
 *		LightEffect
 *		LockEffect
 *		OpenEffect
 *		ReanimateEffect
 *		ScriptEffect
 *		SoulTrapEffect
 *		SunDamageEffect
 *		TurnUndeadEffect
 *		VampirismEffect
 *	
 *	[ havok stuff ]
 *		bhkCharacterListenerArrow
 *	
 *	Menu vtbl + 0x0C = event handler
 *	
 ***/

// 
enum
{
	kFormID_DoorMarker			= 0x00000001,
	kFormID_TravelMarker,
	kFormID_NorthMarker,

	kFormID_DivineMarker		= 0x00000005,
	kFormID_TempleMarker,

	kFormID_MapMarker			= 0x00000010,

	kFormID_HorseMarker			= 0x00000012,
	// ...
};

#if OBLIVION
#if OBLIVION_VERSION == OBLIVION_VERSION
	static const UInt32 kTESObjectREFR_IsOffLimitsToPlayerAddr = 0x004DEBF0;
#else
#error unsupported oblivion version
#endif
#endif

class MagicTarget;
class MagicCaster;
class EffectItem;
class ActiveEffect;
class DialoguePackage;
class Creature;
class BoltShaderProperty;
class TESTopic;
class SpellItem;
class BSFogProperty;

// 00C
class MagicCaster
{
public:
	MagicCaster();
	~MagicCaster();

	virtual void	Unk_00(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_01(UInt32 arg0, UInt32 arg1, UInt32 arg2);
	virtual void	Unk_02(UInt32 arg0, UInt32 arg1, UInt32 arg2);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);

	// looks like returns true if can use magicItem, filling out type (and arg1 is magicka cost?)
	virtual bool	Unk_07(MagicItem* magicItem, float* arg1, UInt32* outMagicItemType, UInt32 arg3);
	virtual TESObjectREFR*	GetParentRefr(void);
	virtual NiNode	* GetMagicNode(void);	// looks up "magicnode" node in caster's NiNode
	virtual void	Unk_0A(void);
	virtual float	GetSpellEffectiveness(float arg0, float arg1);	// seen (0, 0)
	virtual MagicItem * GetQueuedMagicItem(void);		// calls through to MiddleHighProcess
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(MagicItem* magicItem, UInt32 mgefCode, UInt32 unk2);	// activate effect?

	enum {
		kState_Inactive = 0,
		kState_Aim = 1,
		kState_Cast = 2,
		kState_FindTargets = 4,

		// these seem to be considered "errors" by the game
		kState_SpellDisabled = 5,
		kState_AlreadyCasting = 6,
		kState_CannotCast = 7
	};

	//	void	** _vtbl;			// 000
	NiNode	* magicNode;		// 004 cached during casting anim
	UInt32	state;				// 008
};

// 008
class MagicTarget
{
public:
	MagicTarget();
	~MagicTarget();

	// 8
	struct EffectNode
	{
		ActiveEffect	* data;
		EffectNode		* next;

		ActiveEffect* Info() const { return data; }
		EffectNode* Next() const { return next; }
	};

	virtual void	Destructor(void);
	virtual TESObjectREFR *	GetParent(void);
	virtual EffectNode *	GetEffectList(void);

//	void	** _vtbl;	// 000
	UInt8	unk04;		// 004
	UInt8	pad05[3];
};

typedef Visitor<MagicTarget::EffectNode, ActiveEffect> ActiveEffectVisitor;

class NonActorMagicCaster : public BSExtraData
{
public:
	NonActorMagicCaster();
	~NonActorMagicCaster();

	//base class
	MagicCaster magicCaster;	//00C

	UInt32			unk01;			//018
	UInt32			unk02;			//01C
	TESObjectREFR	* caster;		//020
};

// 20
class NonActorMagicTarget : public BSExtraData
{
public:
	NonActorMagicTarget();
	~NonActorMagicTarget();

	// base
	MagicTarget		magicTarget;	// 00C
	TESObjectREFR	* targetRefr;	// 014 passed to c'tor, is the "nonactor" refr
	UInt32			unk18;			// 018
	UInt32			unk1C;			// 01C
};

STATIC_ASSERT(sizeof(NonActorMagicTarget) == 0x20);

// 058
class TESObjectREFR : public TESForm
{
public:
#if OBLIVION
	MEMBER_FN_PREFIX(TESObjectREFR);
	DEFINE_MEMBER_FN(IsOffLimitsToPlayer, bool, kTESObjectREFR_IsOffLimitsToPlayerAddr);
#endif

	enum
	{
		//	TESForm flags
		//	if(!IsActor())
				kChanged_IsEmpty =		0x00010000,
					// CHANGE_OBJECT_EMPTY
					// no data?
		
		kChanged_Inventory =			0x08000000,
			// CHANGE_REFR_INVENTORY
			// ### todo: see 0048BA40

		//	if((changed & ((version < 0x43) ? 0x177577F0 : 0x177577E0))) || IsActor())
		//	{
		//		// this is all part of its own function
		//		
		//	}

		//	if(!IsActor())
				kChanged_Animation =	0x02000000,
					// CHANGE_REFR_ANIMATION
					// UInt16	dataLen;
					// UInt8	data[dataLen];
		kChanged_Move =			0x00000004,
			// TESObjectCELL	cell
			// float			pos[3]
			// float			rot[3]
		kChanged_HavokMove =			0x00000008,
			// CHANGE_REFR_HAVOK_MOVE
		kChanged_MapMarkerFlags =		0x00000400,
			// CHANGE_MAPMARKER_EXTRA_FLAGS
		kChanged_HadHavokMoveFlag =		0x00000800,
			// CHANGEFLAG_REFR_HAD_HAVOK_MOVE_FLAG
			// if either of these are set
			// UInt16	dataLen;
			// UInt8	data[dataLen];

		//	if(version > 0x43)
				kChanged_Scale =		0x00000010,
					// CHANGE_REFR_SCALE
					// float	scale;

		kChanged_DoorOpenDefaultState =	0x00040000,
			// CHANGE_DOOR_OPEN_DEFAULT_STATE
			// no data

		kChanged_DoorOpenState =		0x00080000,
			// CHANGE_DOOR_OPEN_STATE
			// no data
		
		kChanged_DoorExtraTeleport =	0x00100000,
			// CHANGE_DOOR_EXTRA_TELEPORT

		kChanged_ExtraOwner =			0x00000080,
			// CHANGE_OBJECT_EXTRA_OWNER
	};

	enum
	{
		kFlags_Persistent	= 0x00000400,		//shared bit with kFormFlags_QuestItem
		kFlags_Disabled =	  0x00000800,
		kFlags_Unk00000002	= 0x00000002,		// set when an item reference is picked up by an actor
		kFlags_Deleted		= 0x00000020,		// refr removed from .esp or savegame
		kFlags_Unk128		= 0x80000000,
		kFlags_Temporary	= 0x00004000,

		// both flags are set when an item picked up from gameworld
		// one or the other flag by itself is not sufficient
		kFlags_Taken		= kFlags_Deleted | kFlags_Unk00000002
	};

	TESObjectREFR();
	~TESObjectREFR();

	virtual void	Unk_37(void) = 0;
	virtual void	Unk_38(void) = 0;	// 38
	virtual void	Unk_39(void) = 0;
	virtual void	Unk_3A(void) = 0;
	virtual float	GetScale(void) = 0;
	virtual void	GetStartingAngle(float * pos) = 0;
	virtual void	GetStartingPos(float * pos) = 0;
	virtual void	Unk_3E(void) = 0;
	virtual void	Unk_3F(void) = 0;
	virtual void	RemoveItem(TESForm* toRemove, BaseExtraList* extraList, int Quantity, UInt8 unk4, UInt8 unk5, TESObjectREFR* destRef, float* unk7, float* unk8, UInt8 unk9, UInt8 unk10);	// 40
	virtual void	Unk_41(void) = 0;
	virtual UInt8	Equip(TESForm* toEquip, int Quantity, BaseExtraList* extraList, UInt32 unk4);
	virtual UInt8	Unequip(TESForm* toUnequip, int Quantity, BaseExtraList* extraList);
	virtual void	Unk_44(void) = 0;
	virtual void	AddItem(TESForm* toAdd, BaseExtraList* extraList, int Quantity);
	virtual void	Unk_46(void) = 0;
	virtual void	Unk_47(void) = 0;
	virtual void	Unk_48(void) = 0;
	virtual MagicTarget *	GetMagicTarget(void) = 0;
	virtual void	Unk_4A(void) = 0;
	virtual void	Unk_4B(void) = 0;
	virtual void	Unk_4C(void) = 0;
	virtual void	Unk_4D(void) = 0;
	virtual void	Unk_4E(void) = 0;
	virtual void	Unk_4F(void) = 0;
	virtual void	Unk_50(void) = 0;	// 50
	virtual void	Unk_51(void) = 0;
	virtual void	Unk_52(void) = 0;			// inits animation-related data, and more
	virtual NiNode*	GenerateNiNode(void) = 0;
	virtual void	Set3D(NiNode* niNode);
	virtual NiNode*	GetNiNode();
	virtual void	Unk_56(void) = 0;
	virtual void	Unk_57(UInt32 arg0) = 0;
	virtual void	Unk_58(void) = 0;
	virtual ActorAnimData* GetAnimData();
	virtual void	Unk_5A(void) = 0;
	virtual void	Unk_5B(void) = 0;
	virtual TESForm*	GetBaseForm();	// returns type this object references
	virtual NiPoint3*	GetPos();
	virtual void	Unk_5E(void) = 0;
	virtual void	Unk_5F(void) = 0;
	virtual UInt32	Unk_60(UInt32 Arg1);	// gets something from the ridden creature or actor
	virtual void	Unk_61(void) = 0;
	virtual void	Unk_62(void) = 0;
	virtual UInt8	GetSitSleepState(); // calls the Process::GetSitSleepState
	virtual bool	IsActor();
	virtual void	ChangeCell(TESObjectCELL * newCell) = 0;
	virtual UInt8	GetDead(UInt8 Arg1);
	virtual UInt8	GetKnockedState(); // calls the Process::GetKnockedState
	virtual void	Unk_68(void) = 0;
	virtual void	Unk_69(void) = 0;

	TESChildCell	childCell;		// 018
	TESForm*		baseForm;		// 01C
	NiPoint3		rot;			// 020
	NiPoint3		pos;			// 02C
	float			scale;			// 038 
	NiNode*			niNode;			// 03C
	TESObjectCELL*	parentCell;		// 040
	BaseExtraList	baseExtraList;	// 044

	ScriptEventList*	GetEventList() const;
	bool				IsTaken() const		{	return ((flags & kFlags_Taken) == kFlags_Taken) ? true : false;	}
	bool				IsDeleted() const;	
	void				SetTaken(bool bTaken) { flags = (bTaken) ? (flags | kFlags_Taken) : (flags & ~kFlags_Taken); }
	bool				IsDisabled() { return flags & kFlags_Disabled ? true : false; }
	void				SetDisabled(bool bDisabled) { flags = bDisabled ? (flags | kFlags_Disabled) : (flags & ~kFlags_Disabled); }
	bool				IsPersistent() { return (flags & kFlags_Persistent) ? true : false; }
	bool				IsTemporary() { return (flags & kFlags_Temporary) ? true : false; }
	TESForm*			GetInventoryItem(UInt32 itemIndex, bool bGetWares);
	void				Disable();
	void				Enable();
	bool				RunScripts();		// runs magic effect and object scripts plus any scripts on items in inventory
	bool				GetTeleportCellName(BSStringT* outName);
	bool				Update3D();
	TESContainer*		GetContainer();
	bool				IsMapMarker();
	float				GetDistance(TESObjectREFR* other, bool bIncludeDisabled);
	ExtraTeleport::Data* GetExtraTeleportData();

	static TESObjectREFR* Create(bool bTemp = false);
};

// 05C+
class MobileObject : public TESObjectREFR
{
public:
	enum
	{
		//	UInt8	processLevel;
		//		FF - none
		//		00 - high
		//		01 - medium high
		//		02 - medium low
		//		03 - low
	};

	MobileObject();
	~MobileObject();

	virtual void	Unk_6A(void) = 0;	// 6A
	virtual void	Unk_6B(void) = 0;
	virtual void	Unk_6C(void) = 0;
	virtual void	Move(void) = 0;
	virtual void	Jump(void) = 0;	// jump?
	virtual void	Unk_6F(void) = 0;
	virtual void	Unk_70(void) = 0;	// 70
	virtual void	Unk_71(void) = 0;
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
	virtual float	GetJumpScale(void) = 0;
	virtual void	Unk_7E(void) = 0;
	virtual void	Unk_7F(void) = 0;
	virtual void	Unk_80(void) = 0;	// 80
	virtual SInt32	GetFame();

	BaseProcess	* process;			// 058
};

typedef std::vector<TESForm*> EquippedItemsList;
typedef std::vector<ExtraContainerChanges::EntryData*> ExtraContainerDataList;

// 104+
class Actor : public MobileObject
{
public:
	Actor();
	~Actor();

	virtual SInt32	GetInfamy();
	virtual void	Resurrect(UInt8 Unk1, UInt8 Unk2, UInt8 Unk3);
	virtual void	Unk_84(void) = 0;
	virtual void	Unk_85(void) = 0;
	virtual void	Unk_86(void) = 0;
	virtual void	Unk_87(void) = 0;

	// applies damage based on game difficulty modifier. Difficulty only applies if attacker != NULL
	// invoked for fall damage (attacker == NULL), melee attacks, not spell damage.
	virtual void	ApplyDamage(float damage, float arg1, Actor* attacker) = 0;
	virtual void	Unk_89(void) = 0;
	virtual void	ProcessControls();
	virtual void	Unk_8B(void) = 0;
	virtual void	SetPackageDismount();
	virtual void	Unk_8D(void) = 0;
	virtual void	Unk_8E(void) = 0;
	virtual void	Unk_8F(void) = 0;
	virtual void	OnAlarmAttack(Actor* attackVictim, UInt32 arg1) = 0;	// 90 tries to send alarm when 'this' attacks victim
	virtual void	Unk_91(void) = 0;
	virtual void	Unk_92(void) = 0;	// SendTrespassAlarm
	virtual void	Unk_93(void) = 0;
	virtual void	Unk_94(void) = 0;
	virtual void	Unk_95(void) = 0;
	virtual void	Unk_96(void) = 0;
	virtual void	Unk_97(void) = 0;
	virtual bool	GetVampireHasFed();			
	virtual void	SetVampireHasFed(bool bFed);
	virtual void	GetBirthSign();
	virtual void	Unk_9B(void) = 0;
	virtual void	Unk_9C(void) = 0;
	virtual void	Unk_9D(void) = 0;
	virtual void	Unk_9E(void) = 0;
	virtual void	Unk_9F(void) = 0;
	virtual void	Unk_A0(void) = 0;	// A0
	virtual UInt32	GetActorValue(UInt32 avCode) = 0;								// current, cumulative value
	virtual float	GetAV_F(UInt32 avCode) = 0;
	virtual void	SetAV_F(UInt32 avCode, float val) = 0;							// base value
	virtual void	SetActorValue(UInt32 avCode, UInt32 val) = 0;
	virtual void	ModMaxAV_F(UInt32 avCode, float amt, Actor* arg2) = 0;
	virtual void	ModMaxAV(UInt32 avCode, SInt32 amt, Actor* arg2) = 0;
	virtual void	ApplyScriptAVMod_F(UInt32 avCode, float amt, UInt32 arg2) = 0;	// script cmds Mod/ForceAV
	virtual void	ApplyScriptAVMod(UInt32 avCode, SInt32 amt, Actor* arg2) = 0;
	virtual void	DamageAV_F(UInt32 avCode, float amt, Actor* arg2) = 0;			// modifier <= 0, console ModAV cmd, damage health, etc
	virtual void	DamageAV(UInt32 value, UInt32 amount, UInt32 unk) = 0;
	virtual void	ModBaseAV_F(UInt32 avCode, float amt) = 0;							
	virtual void	ModBaseAV(UInt32 avCode, SInt32 amt) = 0;
	virtual void	Unk_AD(void) = 0;
	virtual void	Unk_AE(void) = 0;
	virtual void	Unk_AF(void) = 0;
	virtual void	Unk_B0(void) = 0;	// B0
	virtual void	Unk_B1(void) = 0;
	virtual void	Unk_B2(void) = 0;
	virtual void	Unk_B3(TESObjectREFR* activatedRefr, UInt32 quantity) = 0; // called after Activate by TESForm::Unk33()
	virtual void	Unk_B4(void) = 0;
	virtual void	Unk_B5(void) = 0;
	virtual void	Unk_B6(void) = 0;
	virtual void	Unk_B7(void) = 0;	// AddSpell?
	virtual void	Unk_B8(void) = 0;	// RemoveSpell?
	virtual void	Unk_B9(void) = 0;
	virtual void	Unk_BA(void) = 0;
	virtual void	Unk_BB(void) = 0;
	virtual void	Unk_BC(void) = 0;
	virtual void	Unk_BD(void) = 0;
	virtual void	Unk_BE(void) = 0;
	virtual void	Unk_BF(void) = 0;
	virtual void	Unk_C0(void) = 0;	// C0
	virtual void	Unk_C1(void) = 0;
	virtual void	Unk_C2(void) = 0;
	virtual void	Unk_C3(void) = 0;
	virtual void	ManageAlarm();
	virtual void	Unk_C5(void) = 0;
	virtual void	Unk_C6(void) = 0;
	virtual void	Unk_C7(void) = 0;
	virtual void	AddPackageWakeUp();
	virtual void	Unk_C9(void) = 0;
	virtual void	Unk_CA(void) = 0;
	virtual void	Unk_CB(void) = 0;
	virtual CombatController*	GetCombatController(void) = 0;
	virtual bool	IsInCombat(bool unk) = 0;
	virtual TESForm *	GetCombatTarget(void) = 0;
	virtual void	Unk_CF(void) = 0;
	virtual void	Unk_D0(void) = 0;	// D0
	virtual void	Unk_D1(void) = 0;
	virtual void	Unk_D2(void) = 0;
	virtual void	Unk_D3(void) = 0;
	virtual bool	IsYielding();
	virtual void	Unk_D5(void) = 0;
	virtual void	Unk_D6(void) = 0;
	virtual void	Unk_D7(void) = 0;
	virtual void	Unk_D8(void) = 0;
	virtual void	Unk_D9(void) = 0;
	virtual void	Unk_DA(void) = 0;
	virtual void	Unk_DB(void) = 0;
	virtual void	Unk_DC(void) = 0;
	virtual void	Unk_DD(void) = 0;
	virtual void	Unk_DE(void) = 0;
	virtual void	Unk_DF(void) = 0;
	virtual Creature * GetMountedHorse();	// E0 returns this->horseOrRider, only for Character
	virtual void	Unk_E1(void) = 0;
	virtual void	Unk_E2(void) = 0;
	virtual void	Unk_E3(void) = 0;
	virtual void	Unk_E4(void) = 0;
	virtual void	Unk_E5(void) = 0;
	virtual void	Unk_E6(void) = 0;
	virtual void	ModExperience(UInt32 actorValue, UInt32 scaleIndex, float baseDelta) = 0;
	virtual void	Unk_E8(void) = 0;
	virtual void	Unk_E9(void) = 0;
	virtual void	Unk_EA(void) = 0;
	virtual void	AttackHandling(UInt32 unused, TESObjectREFR* arrowRef, TESObjectREFR * target) = 0;	// args all null for melee attacks
	virtual void	Unk_EC(void) = 0;
	virtual void	Unk_ED(void) = 0;	// something with blocking
	virtual void	OnHealthDamage(Actor* attacker, float damage) = 0;

	// unk1 looks like quantity, usu. 1; ignored for ammo (equips entire stack)
	// itemExtraList is NULL as the container changes entry is not resolved before the call
	void				EquipItem(TESForm * objType, UInt32 unk1, BaseExtraList* itemExtraList, UInt32 unk3, bool lockEquip);
	void				UnequipItem(TESForm* objType, UInt32 unk1, BaseExtraList* itemExtraList, UInt32 unk3, bool lockUnequip, UInt32 unk5);

	UInt32				GetBaseActorValue(UInt32 value);
	EquippedItemsList	GetEquippedItems();
	ExtraContainerDataList	GetEquippedEntryDataList();
	bool				CanCastGreaterPower(SpellItem* power);
	void				SetCanUseGreaterPower(SpellItem* power, bool bAllowUse, float timer = -1);
	void				UnequipAllItems();

	// 8
	struct PowerListData {
		SpellItem	* power;
		float		timer;		// init'ed to (3600 / TimeScale) * 24 <- TimeScale is a TESGlobal
	};

	struct PowerListEntry {
		PowerListData	* data;
		PowerListEntry  * next;

		PowerListData * Info() const	{ return data; }
		PowerListEntry * Next() const	{ return next; }
		void SetNext(PowerListEntry* nuNext) { next = nuNext; }
		void Delete();
		void DeleteHead(PowerListEntry* replaceWith);
	};

	// 8
	struct Unk0A4
	{
		// 8+
		struct Data
		{
			UInt32	unk0;
			Actor	* unk4;
		};

		Data	* data;
		Unk0A4	* next;
	};

	// bases
	MagicCaster		magicCaster;					// 05C
	MagicTarget		magicTarget;					// 068
	UInt32			unk070;							// 070
	UInt32			unk074;							// 074
	UInt8			unk078;							// 078
	UInt8			pad078[3];						// 079
	Actor*			unk07C;							// 07C
	UInt32			unk080;							// 080
	float			unk084;							// 084
	ActorValues		avModifiers;					// 088
	PowerListEntry  greaterPowerList;				// 09C
	Unk0A4			unk0A4;							// 0A4
	float			unk0AC;							// 0AC
	UInt32			DeadState;						// 0B0
	UInt32			unk0B4;							// 0B4
	UInt32			unk0B8;							// 0B8
	float			unk0BC;							// 0BC
	UInt8			unk0C0;							// 0C0
	UInt8			pad0C0[3];						// 0C1
	UInt32			unk0C4;							// 0C4
	UInt8			unk0C8;							// 0C8
	UInt8			unk0C9;							// 0C9
	UInt8			unk0CA;							// 0CA
	UInt8			pad0C8;							// 0CB
	TESObjectREFR*	unk0CC;							// 0CC
	UInt32			unk0D0;							// 0D0
	Actor*			horseOrRider;					// 0D4 For Character, currently ridden horse; For horse (Creature), currently riding Character
	UInt8			unk0D8;							// 0D8
	UInt8			pad0D8[3];						// 0D9
	float			unk0DC;							// 0DC
	UInt32			unk0E0;							// 0E0
	Actor*			unk0E4;							// 0E4
	UInt32			unk0E8;							// 0E8
	UInt32			unk0EC;							// 0EC
	UInt32			unk0F0;							// 0F0
	float			unk0F4;							// 0F4
	UInt32			unk0F8;							// 0F8
	UInt8			unk0FC;							// 0FC
	UInt8			unk0FD;							// 0FD //It is set to 0 when Process::GetCurDay == TimeGlobals::GetDay
	UInt8			pad0FC[2];						// 0FE
	float			unk100;							// 100
	// 104

	TESPackage*		GetCurrentPackage();
	bool			IsObjectEquipped(TESForm* object);
	float			GetAVModifier(eAVModifier mod, UInt32 avCode);
	float			GetCalculatedBaseAV(UInt32 avCode);
	bool			IsAlerted();
	void			SetAlerted(bool bAlerted);
	void			EvaluatePackage();
	bool			IsTalking();
};
#if OBLIVION
STATIC_ASSERT(sizeof(Actor) == 0x104);
#endif

// 10C
class Character : public Actor
{
public:
	Character();
	~Character();

	SkinInfo*	ActorSkinInfo;								// 104
	UInt32		unk108;										// 108
};
#if OBLIVION
STATIC_ASSERT(sizeof(Character) == 0x10C);
#endif

// 804
class PlayerCharacter : public Character
{
public:
	enum
	{
		kMiscStat_DaysInPrison = 0,
		kMiscStat_DaysPassed,
		kMiscStat_SkillIncreases,
		kMiscStat_TrainingSessions,
		kMiscStat_LargestBounty,
		kMiscStat_CreaturesKilled,
		kMiscStat_PeopleKilled,
		kMiscStat_PlacesDiscovered,
		kMiscStat_LocksPicked,
		kMiscStat_LockpicksBroken,
		kMiscStat_SoulsTrapped,	// 10
		kMiscStat_IngredientsEaten,
		kMiscStat_PotionsMade,
		kMiscStat_OblivionGatesShut,
		kMiscStat_HorsesOwned,
		kMiscStat_HousesOwned,
		kMiscStat_StoresInvestedIn,
		kMiscStat_BooksRead,
		kMiscStat_SkillBooksRead,
		kMiscStat_ArtifactsFound,
		kMiscStat_HoursSlept,	// 20
		kMiscStat_HoursWaited,
		kMiscStat_DaysAsAVampire,
		kMiscStat_LastDayAsAVampire,
		kMiscStat_PeopleFedOn,
		kMiscStat_JokesTold,
		kMiscStat_DiseasesContracted,
		kMiscStat_NirnrootsFound,
		kMiscStat_ItemsStolen,
		kMiscStat_ItemsPickpocketed,
		kMiscStat_Trespasses,	// 30
		kMiscStat_Assaults,
		kMiscStat_Murders,
		kMiscStat_HorsesStolen,

		kMiscStat_Max			// 34
	};

	struct TopicList {
		TESTopic	* topic;
		TopicList	* next;
	};

	struct Unk5B0 {
		float unk[21];
	};

	PlayerCharacter();
	~PlayerCharacter();

	// [ vtbl ]
	// +000 = PlayerCharacter
	// +018 = TESChildCell
	// +05C = MagicCaster
	// +068 = MagicTarget
	// +784 = NiTMapBase

	// [ objects ]
    // +01C TESNPC *
    // +03C BSFadeNode *
    // +040 TESObjectCELL *
    // +048 ExtraContainerChanges *
    // +058 HighProcess *
    // +1F4 hkAllCdPointCollector *
    // +570 TESObjectREFR *
    // +5E4 TESTopic *
    // +5F4 TESQuest *
    // +614    float amountFenced
    // +624 SpellItem *
    // +644 BirthSign *
    // +650 TESClass *
    // +6E8 TESRegion *
    // +700 TESObjectREFR *
    // +728 TESWorldSpace *
    // +740 TESWorldSpace *

	// [ data ]
	// +11C haggle amount?
	// +588 UInt8, bit 0x01 is true if we're in third person?
	// +590 UInt8, is time passing?
	// +5A9 UInt8, fast travel disabled
	// +658	UInt32, misc stat array
	// +70C	'initial state' buffer

	UInt8			unk10C;										// 10C
	UInt8			pad10D[3];									// 10D
	UInt32			unk110;										// 110
	UInt8			unk114;										// 114
	UInt8			unk115;										// 115
	UInt8			isInSEWorld;								// 116
	UInt8			unk117;										// 117
	DialoguePackage* dialoguePackage;							// 118
	UInt32			unk11C;										// 11C
	UInt32			unk120;										// 120
	UInt32			unk124;										// 124
	UInt32			unk128;										// 128
	UInt8			isMovingToNewSpace;							// 12C
	UInt8			pad12C[3];
	float			skillExp[21];								// 130	current experience for each skill
	UInt32			majorSkillAdvances;							// 184
	UInt32			skillAdv[21];								// 188 number of advances for each skill
	UInt8			bCanLevelUp;								// 1DC
	UInt8			unk1DD[3];									// 1DD
	Creature*		lastRiddenHorse;							// 1E0
	UInt32			unk1E4;										// 1E4
	UInt32			unk1E8;										// 1E8
	UInt32			unk1EC;										// 1EC
	UInt32			unk1F0;										// 1F0
	UInt32			unk1F4;										// 1F4
	UInt32			unk1F8;										// 1F8
	UInt32			unk1FC;										// 1FC
	UInt32			unk200;										// 200
	float			maxAVModifiers[kActorVal_OblivionMax];		// 204
	float			scriptAVModifiers[kActorVal_OblivionMax];	// 324
	float			unk444;							// 444
	float			unk448;							// 448
	float			unk44C;							// 44C
	UInt32			unk450[(0x570 - 0x450) >> 2];	// 450
	TESObjectREFR*	lastActivatedLoadDoor;			// 570 - most recently activated load door
	UInt32			unk574;							// 574
	UInt32			unk578;							// 578
	UInt32			unk57CState;					// 57C
	UInt32			unk580;							// 580
	UInt32			unk584;							// 584
	UInt8			isThirdPerson;					// 588
	UInt8			DisableFading;					// 589
	UInt8			unk58A;							// 58A
	UInt8			AlwaysRun;						// 58B
	UInt8			AutoMove;						// 58C
	UInt8			pad58D[3];						// 58D
	UInt32			HoursToSleep;					// 590
	UInt8			isSleeping;						// 594
	UInt8			pad594[3];
	float			FoV;							// 598
	float			unk59C;							// 59C
	float			unk5A0;							// 5A0
	float			unk5A4;							// 5A4
	UInt32			unk5A8;							// 5A8
	UInt32			unk5AC;							// 5AC
	Unk5B0*			unk5B0;							// 5B0 - freed when running SetInCharGen 0
	UInt8**			attributeBonuses;				// 5B4
	UInt16			unk5B8;							// 5B8
	UInt8			unk5BA;							// 5BA
	UInt8			unk5BB;							// 5BB
	UInt32			trainingSessionsUsed;			// 5BC reset on level-up
	UInt8			unk5C0;							// 5C0
	UInt8			pad5C0[3];
	TESObjectREFR*	ObjectToActivate;				// 5C4
	SkinInfo*		firstPersonSkinInfo;			// 5C8
	ActorAnimData*	firstPersonAnimData;			// 5CC
	NiNode*			firstPersonNiNode;				// 5D0
	float			firstPersonNiNodeWTranslateZ;	// 5D4
	UInt32			unk5D8;							// 5D8
	ActorAnimData*	defaultAnimData;				// 5DC
	UInt32			unk5E0;							// 5E0
	TESTopic*		unk5E4;							// 5E4
	UInt32			unk5E8;							// 5E8
	UInt32			unk5EC;							// 5EC
	UInt32			unk5F0;							// 5F0
	TESQuest*		activeQuest;					// 5F4
	UInt32			unk5F8;							// 5F8
	UInt32			unk5FC;							// 5FC
	UInt32			unk600;							// 600
	UInt32			unk604;							// 604
	UInt32			JailedState;					// 608
	UInt32			unk60C;							// 60C
	UInt8			unk610;							// 610
	UInt8			isAMurderer;					// 611
	UInt8			pad612[2];						// 612
	UInt32			AmountStolenSold;				// 614
	float			unk618;							// 618
	float			unk61C;							// 61C
	UInt32			unk620;							// 620
	MagicItem*		activeMagicItem;				// 624
	TESObjectBOOK*	book;							// 628
	UInt32			unk62C;							// 62C
	UInt32			unk630;							// 630
	UInt32			unk634;							// 634
	UInt32			unk638;							// 638
	UInt32			unk63C;							// 63C
	float			unk640;							// 640
	BirthSign*		birthSign;						// 644
	UInt32			unk648[(0x650 - 0x648) >> 2];	// 648
	TESClass*		wtfClass;						// 650 - this is not the player class! use OBLIVION_CAST(this, TESForm, TESNPC)->npcClass
	UInt32			unk654;							// 654
	UInt32			miscStats[kMiscStat_Max];		// 658
	AlchemyItem*	alchemyItem;					// 6E0
	UInt8			bVampireHasFed; 				// 6E4 returned by vtbl+260, set by vtbl+264
	UInt8			isInCharGen;					// 6E5
	UInt8			pad6E6[2];						// 6E6
	TESRegion*		currentRegion;					// 6E8
	UInt32			unk6EC;							// 6EC
	UInt32			unk6F0;							// 6F0
	UInt32			unk6F4;							// 6F4
	UInt32			unk6F8;							// 6F8
	UInt32			unk6FC;							// 6FC
	UInt32			unk700;							// 700
	UInt32			unk704;							// 704
	UInt32			unk708;							// 708
	UInt32			unk70C;							// 70C
	UInt32			TickCount;						// 710
	UInt32			unk714;							// 714
	UInt32			unk718;							// 718
	UInt8			isTravelPackage;				// 71C
	UInt8			isWakeUpPackage;				// 71D
	UInt8			pad71E[2];
	UInt32			unk720;							// 720
	UInt32			unk724;							// 724
	UInt32			unk728;							// 728
	UInt32			unk72C;							// 72C
	UInt32			unk730;							// 730
	float			gameDifficultyLevel;			// 734 ranges from -1 to 1
	UInt32			unk738;							// 738
	UInt32			unk73C;							// 73C
	UInt32			unk740;							// 740
	UInt32			unk744;							// 744
	UInt8			isFlyCam;						// 748
	UInt8			pad748[3];						// 749
	float			FlyCamRotZ;						// 74C
	float			FlyCamRotX;						// 750
	float			FlyCamPosX;						// 754
	float			FlyCamPosY;						// 758
	float			FlyCamPosZ;						// 75C
	UInt32			unk760[(0x7A4 - 0x760) >> 2];	// 760
	float			requiredSkillExp[21];			// 7A4 total amt of exp needed to increase each skill
	UInt32			unk7F8;							// 7F8
	float			unk7FC;							// 7FC
	float			unk800;							// 800

	bool			SetActiveSpell(MagicItem * item);
	UInt8			GetAttributeBonus(UInt32 whichAttribute) { return whichAttribute < kActorVal_Luck ? (*attributeBonuses)[whichAttribute] : -1; }
	void			SetAttributeBonus(UInt32 whichAttr, UInt8 newVal) { if (whichAttr < kActorVal_Luck)	(*attributeBonuses)[whichAttr] = newVal; }
	UInt32			GetSkillAdvanceCount(UInt32 valSkill) { return IsSkill(valSkill) ? skillAdv[valSkill - kActorVal_Armorer] : 0; }
	void			SetSkillAdvanceCount(UInt32 valSkill, UInt32 val) { if (IsSkill(valSkill)) skillAdv[valSkill - kActorVal_Armorer] = val; }
	UInt32			ModSkillAdvanceCount(UInt32 valSkill, SInt32 mod);
	UInt32			ModMajorSkillAdvanceCount(SInt32 mod) { SInt32 adjustedVal = majorSkillAdvances + mod; majorSkillAdvances = (adjustedVal > 0) ? adjustedVal : 0; return majorSkillAdvances; }
	MagicItem*		GetActiveMagicItem();
	bool			IsThirdPerson() { return isThirdPerson ? true : false; }
	void			TogglePOV(bool bFirstPerson);
	void			SetBirthSign(BirthSign* birthSign);
	void			ChangeExperience(UInt32 actorValue, UInt32 scaleIndex, float baseDelta);
	void			ChangeExperience(UInt32 actorValue, float amount);
	float			ExperienceNeeded(UInt32 skill, UInt32 atLevel);
	TESClass*		GetPlayerClass() const;
	bool			SetSkeletonPath(const char* newPath);

	static void UpdateHead(void);
};
#if OBLIVION
STATIC_ASSERT(sizeof(PlayerCharacter) == 0x804);
#endif

extern PlayerCharacter ** g_thePlayer;

class Creature : public Actor
{
public:
	Creature();
	~Creature();

	UInt32		unk104;				// 104
};
#if OBLIVION
STATIC_ASSERT(sizeof(Creature) == 0x108);
#endif

class SkyObject
{
public:
	SkyObject();
	virtual ~SkyObject();										// 00

	virtual NiNode*				GetObjectNode(void);			// 01
	virtual void				Initialize(UInt32 u1);			// 02
	virtual void				func_03(UInt32 u1, UInt32 u2);	// 03

	NiNode*						RootNode;						// 04	
};

class NiPick;

class Sun: public SkyObject
{
public:
	Sun();
	virtual ~Sun();										// 00

	NiBillboardNode*			SunBillboard;			// 08
	NiBillboardNode*			SunGlareBillboard;		// 0C
	NiTriShape*					SunGeometry;			// 10
	NiTriShape*					SunGlareGeometry;		// 14
	NiTArray<NiPick*>*			SunPickList;			// 18 NiPick::Record
	NiDirectionalLight*			SunDirLight;			// 1C
	float						unk20;					// 20
	UInt8						unk24;					// 24
	UInt8						pad25[3];				// 25

};

class Atmosphere: public SkyObject
{
public:
	Atmosphere();
	virtual ~Atmosphere();								// 00

	NiAVObject*					Mesh;					// 08
	BSFogProperty*				fogProperty;			// 0C
	UInt32						unk10;					// 10
	NiNode*						Quad;					// 14
	UInt8						unk18;					// 18
	UInt8						pad18[3];

};

class Stars: public SkyObject
{
public:
	Stars();
	virtual ~Stars();									// 00

	UInt32						unk08;					// 08
	float						unk0C;					// 0C

};

class Clouds: public SkyObject
{
public:
	Clouds();
	virtual ~Clouds();									// 00

	UInt32						unk08;					// 08
	UInt32						unk0C;					// 0C
	UInt32						unk10;					// 10
	UInt32						unk14;					// 14

};

class Moon: public SkyObject
{
public:
	Moon();
	virtual ~Moon();									// 00

	NiNode*						MoonNode;				// 08
	NiNode*						ShadowNode;				// 0C
	NiNode*						MoonMesh;				// 10
	NiNode*						ShadowMesh;				// 14
	char*						texture_full;			// 18
	UInt32						unk1C;					// 1C
	char*						texture_three_wan;		// 20
	UInt32						unk24;					// 24
	char*						texture_half_wan;		// 28
	UInt32						unk2C;					// 2C
	char*						texture_one_wan;		// 30
	UInt32						unk34;					// 34
	UInt32						unk38;					// 38
	UInt32						unk3C;					// 3C
	char*						texture_one_wax;		// 40
	UInt32						unk44;					// 44
	char*						texture_half_wax;		// 48
	UInt32						unk4C;					// 4C
	char*						texture_three_wax;		// 50
	UInt32						unk54;					// 54
	float						unk58;					// 58
	float						unk5C;					// 5C
	float						unk60;					// 60
	float						unk64;					// 64
	float						unk68;					// 68
	UInt32						unk6C;					// 6C
	UInt32						unk70;					// 70
	float						unk74;					// 74
	float						unk78;					// 78

};

class Precipitation
{
public:
	Precipitation();
	virtual ~Precipitation();							// 00

	UInt32						unk04;					// 04
	UInt32						unk08;					// 08
	UInt32						unk0C;					// 0C
	float						unk10;					// 10
	UInt32						unk14;					// 14

};

// 104
class Sky
{
public:
	Sky();
	~Sky();

	virtual void	Destructor(void);
	// no more virtual functions

	static Sky *	GetSingleton(void);

	void	RefreshClimate(TESClimate * climate, UInt32 unk1);	// pass 1 for unk1 to pick new random weather etc

//	void		** _vtbl;						// 000
	NiNode*		nodeSkyRoot;					// 004
	NiNode*		nodeMoonsRoot;					// 008
	TESClimate* firstClimate;					// 00C
	TESWeather* firstWeather;					// 010
	TESWeather*	secondWeather014;				// 014 Previous weather when in transition
	TESWeather*	weather018;						// 018
	TESWeather* weatherOverride;				// 01C
	Atmosphere* atmosphere;						// 020
	Stars*		stars;							// 024
	Sun*		sun;							// 028
	Clouds*		clouds;							// 02C
	Moon*		masserMoon;						// 030
	Moon*		secundaMoon;					// 034
	Precipitation* precipitation;				// 038
	UInt32		unk03C[(0x0B4 - 0x03C) >> 2];	// 03C
	float		unk0B4;							// 0B4
	float		unk0B8;							// 0B8
	float		unk0BC;							// 0BC
	float		windSpeed;						// 0C0
	float		unk0C4;							// 0C4
	float		unk0C8;							// 0C8
	float		unk0CC;							// 0CC
	float		unk0D0;							// 0D0
	float		unk0D4;							// 0D4
	float		weatherPercent;					// 0D8
	UInt32		unk0DC;							// 0DC
	UInt32		unk0E0;							// 0E0
	float		unk0E4;							// 0E4
	UInt32		unk0E8;							// 0E8
	UInt32		unk0EC;							// 0EC
	float		unk0F0;							// 0F0
	float		unk0F4;							// 0F4
	UInt32		unk0F8;							// 0F8
	UInt32		Flags0FC;						// 0FC
	UInt8		unk100;							// 100
	UInt8		unk101[3];						// 101
};

STATIC_ASSERT(sizeof(Sky) == 0x104);

enum
{
	kProjectileType_Arrow,
	kProjectileType_Ball,
	kProjectileType_Fog,
	kProjectileType_Bolt,
};							//arbitrary

//78
class MagicProjectile : public MobileObject
{
public:
	float			speed;				// 5C base speed * GMST fMagicProjectileBaseSpeed
	float			distanceTraveled;	// 60 speed * elapsedTime while in flight
	float			elapsedTime;		// 64 length of time projectile has existed
	MagicCaster		* caster;			// 68 whoever/whatever cast the spell
										//    For NonActorMagicCaster, != casting reference
	MagicItem		* magicItem;		// 6C can always cast to SpellItem? NO - can be EnchantmentItem for staves
	UInt32			effectCode;			// 70 i.e. 'SEFF'
	EffectSetting	* effectSetting;	// 74
};

//90
class MagicBallProjectile : public MagicProjectile
{
public:
	MagicBallProjectile();
	~MagicBallProjectile();

	float				unk078;				//078
	UInt32				unk07C;				//07C
	UInt32				unk080;				//080 - looks like flags - (1 in flight, 2 hit target?)
	float				unk084;				//084 - value changes after projectile hits something 
	UInt32				unk088;				//088
	UInt32				unk08C;				//08C
};

//9C
class MagicFogProjectile : public MagicProjectile
{
public:
	MagicFogProjectile();
	~MagicFogProjectile();

	float				unk078;				//078
	float				unk07C;				//07C
	float				unk080;				//080
	float				unk084;				//084
	UInt32				unk088;				//088 - looks like flags - (0 in flight, 1 hit target?)
	float				unk08C;				//08C
	UInt32				unk090;				//090 - pointer?
	UInt32				unk094;				//094
	UInt32				unk098;				//098 - pointer?
};

//A4
class MagicBoltProjectile : public MagicProjectile
{
public:
	MagicBoltProjectile();
	~MagicBoltProjectile();

	float				unk078;				//078
	BoltShaderProperty	* boltShaderProperty;//07C
	UInt32				unk080;				//080
	UInt32				unk084;				//084
	NiNode				* niNode088;		//088
	UInt32				unk08C;				//08C
	UInt32				unk090;				//090
	NiNode				* niNode094;		//094
	UInt32				unk098;				//098
	UInt32				unk09C;				//09C - pointer?
	UInt32				unk0A0;				//0A0
};

//9C
class ArrowProjectile : public MobileObject
{
public:
	ArrowProjectile();
	~ArrowProjectile();

	// 54
	struct CollisionData
	{
		// not sure if this data is generated for collisions with immobile objects too, or if struct is unique to ArrowProjectile (probably not)
		float			unk00[9];	// 00 presumably a matrix or set of 3 3-dimensional vectors
		TESObjectREFR	* refr;		// 24 what it hit
		NiNode			* ninode;	// 28 seen "Bip01 Spine" for Creature
		float			unk2C[9];	// 2C again
	};

	CollisionData	* unk05C;		//05C
	UInt32			unk060;			//060
	float			unk064;			//064
	float			elapsedTime;	//068
	float			speed;			//06C - base speed * GMST fArrowSpeedMult
	float			unk070;			//070
	float			unk074;			//074
	Actor			* shooter;		//078;
	EnchantmentItem	* arrowEnch;	//07C
	EnchantmentItem	* bowEnch;		//080
	AlchemyItem		* poison;		//084
	float			unk088;			//088
	float			unk08C;			//08C
	float			unk090;			//090
	UInt32			unk094;			//094
	UInt32			unk098;			//098
};
