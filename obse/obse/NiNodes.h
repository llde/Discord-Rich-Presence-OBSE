#pragma once

#include "obse/NiTypes.h"
#include "obse/GameTypes.h"
#include "Utilities.h"
#include <d3d9.h>

/*** class hierarchy
 *	
 *	yet again taken from rtti information
 *	ni doesn't seem to use multiple inheritance
 *	
 *	thanks to the NifTools team for their work on the on-disk format
 *	thanks to netimmerse for NiObject::DumpAttributes
 *	
 *	all offsets here are assuming patch 1.2 as they changed dramatically
 *	0xE8 bytes were removed from NiObjectNET, and basically everything derives from that
 *	
 *	NiObject derives from NiRefObject
 *	
 *	BSFaceGenMorphData - derived from NiRefObject
 *		BSFaceGenMorphDataHead
 *		BSFaceGenMorphDataHair
 *	
 *	BSTempEffect - derived from NiObject
 *		BSTempEffectDecal
 *		BSTempEffectGeometryDecal
 *		BSTempEffectParticle
 *		MagicHitEffect
 *			MagicModelHitEffect
 *			MagicShaderHitEffect
 *	
 *	NiDX92DBufferData - derived from NiRefObject and something else
 *		NiDX9DepthStencilBufferData
 *			NiDX9SwapChainDepthStencilBufferData
 *			NiDX9ImplicitDepthStencilBufferData
 *			NiDX9AdditionalDepthStencilBufferData
 *		NiDX9TextureBufferData
 *		NiDX9OnscreenBufferData
 *			NiDX9SwapChainBufferData
 *			NiDX9ImplicitBufferData
 *	
 *	NiObject
 *		NiObjectNET
 *			NiProperty
 *				NiTexturingProperty
 *				NiVertexColorProperty
 *				NiWireframeProperty
 *				NiZBufferProperty
 *				NiMaterialProperty
 *				NiAlphaProperty
 *				NiStencilProperty
 *				NiRendererSpecificProperty
 *				NiShadeProperty
 *					BSShaderProperty
 *						SkyShaderProperty
 *						ParticleShaderProperty
 *						BSShaderLightingProperty
 *							DistantLODShaderProperty
 *							TallGrassShaderProperty
 *							BSShaderPPLightingProperty
 *								SpeedTreeShaderPPLightingProperty
 *									SpeedTreeBranchShaderProperty
 *								Lighting30ShaderProperty
 *								HairShaderProperty
 *							SpeedTreeShaderLightingProperty
 *								SpeedTreeLeafShaderProperty
 *								SpeedTreeFrondShaderProperty
 *							GeometryDecalShaderProperty
 *						PrecipitationShaderProperty
 *						BoltShaderProperty
 *						WaterShaderProperty
 *				NiSpecularProperty
 *				NiFogProperty
 *					BSFogProperty
 *				NiDitherProperty
 *			NiTexture
 *				NiDX9Direct3DTexture
 *				NiSourceTexture
 *					NiSourceCubeMap
 *				NiRenderedTexture
 *					NiRenderedCubeMap
 *			NiAVObject
 *				NiDynamicEffect
 *					NiLight
 *						NiDirectionalLight
 *						NiPointLight
 *							NiSpotLight
 *						NiAmbientLight
 *					NiTextureEffect
 *				NiNode
 *					SceneGraph
 *					BSTempNodeManager
 *					BSTempNode
 *					BSCellNode
 *					BSClearZNode
 *					BSFadeNode
 *					BSScissorNode
 *					BSTimingNode
 *					BSFaceGenNiNode
 *					NiBillboardNode
 *					NiSwitchNode
 *						NiLODNode
 *							NiBSLODNode
 *					NiSortAdjustNode
 *					NiBSPNode
 *					ShadowSceneNode
 *				NiCamera
 *					BSCubeMapCamera - RTTI data incorrect
 *					NiScreenSpaceCamera
 *				NiGeometry
 *					NiLines
 *					NiTriBasedGeom
 *						NiTriShape
 *							BSScissorTriShape
 *							NiScreenElements
 *							NiScreenGeometry
 *							TallGrassTriShape
 *						NiTriStrips
 *							TallGrassTriStrips
 *					NiParticles
 *						NiParticleSystem
 *							NiMeshParticleSystem
 *						NiParticleMeshes
 *			NiSequenceStreamHelper
 *		NiRenderer
 *			NiDX9Renderer
 *		NiPixelData
 *		NiCollisionObject
 *			NiCollisionData
 *			bhkNiCollisionObject
 *				bhkCollisionObject
 *					bhkBlendCollisionObject
 *						WeaponObject
 *						bhkBlendCollisionObjectAddRotation
 *				bhkPCollisionObject
 *					bhkSPCollisionObject
 *		NiControllerSequence
 *			BSAnimGroupSequence
 *		NiTimeController
 *			BSDoorHavokController
 *			BSPlayerDistanceCheckController
 *			NiD3DController
 *			NiControllerManager
 *			NiInterpController
 *				NiSingleInterpController
 *					NiTransformController
 *					NiPSysModifierCtlr
 *						NiPSysEmitterCtlr
 *						NiPSysModifierBoolCtlr
 *							NiPSysModifierActiveCtlr
 *						NiPSysModifierFloatCtlr
 *							NiPSysInitialRotSpeedVarCtlr
 *							NiPSysInitialRotSpeedCtlr
 *							NiPSysInitialRotAngleVarCtlr
 *							NiPSysInitialRotAngleCtlr
 *							NiPSysGravityStrengthCtlr
 *							NiPSysFieldMaxDistanceCtlr
 *							NiPSysFieldMagnitudeCtlr
 *							NiPSysFieldAttenuationCtlr
 *							NiPSysEmitterSpeedCtlr
 *							NiPSysEmitterPlanarAngleVarCtlr
 *							NiPSysEmitterPlanarAngleCtlr
 *							NiPSysEmitterLifeSpanCtlr
 *							NiPSysEmitterInitialRadiusCtlr
 *							NiPSysEmitterDeclinationVarCtlr
 *							NiPSysEmitterDeclinationCtlr
 *							NiPSysAirFieldSpreadCtlr
 *							NiPSysAirFieldInheritVelocityCtlr
 *							NiPSysAirFieldAirFrictionCtlr
 *					NiFloatInterpController
 *						NiFlipController
 *						NiAlphaController
 *						NiTextureTransformController
 *						NiLightDimmerController
 *					NiBoolInterpController
 *						NiVisController
 *					NiPoint3InterpController
 *						NiMaterialColorController
 *						NiLightColorController
 *					NiExtraDataController
 *						NiFloatsExtraDataPoint3Controller
 *						NiFloatsExtraDataController
 *						NiFloatExtraDataController
 *						NiColorExtraDataController
 *				NiMultiTargetTransformController
 *				NiGeomMorpherController
 *			bhkBlendController
 *			bhkForceController
 *			NiBSBoneLODController
 *			NiUVController
 *			NiPathController
 *			NiLookAtController
 *			NiKeyframeManager
 *			NiBoneLODController
 *			NiPSysUpdateCtlr
 *			NiPSysResetOnLoopCtlr
 *			NiFloatController
 *				NiRollController
 *		bhkRefObject
 *			bhkSerializable
 *				bhkWorld - NiRTTI has incorrect parent
 *					bhkWorldM
 *				bhkAction
 *					bhkUnaryAction
 *						bhkMouseSpringAction
 *						bhkMotorAction
 *					bhkBinaryAction
 *						bhkSpringAction
 *						bhkAngularDashpotAction
 *						bhkDashpotAction
 *				bhkWorldObject
 *					bhkPhantom
 *						bhkShapePhantom
 *							bhkSimpleShapePhantom
 *							bhkCachingShapePhantom
 *						bhkAabbPhantom
 *							bhkAvoidBox
 *					bhkEntity
 *						bhkRigidBody
 *							bhkRigidBodyT
 *				bhkConstraint
 *					bhkLimitedHingeConstraint
 *					bhkMalleableConstraint
 *					bhkBreakableConstraint
 *					bhkWheelConstraint
 *					bhkStiffSpringConstraint
 *					bhkRagdollConstraint
 *					bhkPrismaticConstraint
 *					bhkHingeConstraint
 *					bhkBallAndSocketConstraint
 *					bhkGenericConstraint
 *						bhkFixedConstraint
 *					bhkPointToPathConstraint
 *					bhkPoweredHingeConstraint
 *				bhkShape
 *					bhkTransformShape
 *					bhkSphereRepShape
 *						bhkConvexShape
 *							bhkSphereShape
 *							bhkCapsuleShape
 *							bhkBoxShape
 *							bhkTriangleShape
 *							bhkCylinderShape
 *							bhkConvexVerticesShape
 *								bhkCharControllerShape
 *							bhkConvexTransformShape
 *							bhkConvexSweepShape
 *						bhkMultiSphereShape
 *					bhkBvTreeShape
 *						bhkTriSampledHeightFieldBvTreeShape
 *						bhkMoppBvTreeShape
 *					bhkShapeCollection
 *						bhkListShape
 *						bhkPackedNiTriStripsShape
 *						bhkNiTriStripsShape
 *					bhkHeightFieldShape
 *						bhkPlaneShape
 *				bhkCharacterProxy
 *					bhkCharacterListenerArrow - no NiRTTI
 *					bhkCharacterListenerSpell - no NiRTTI
 *					bhkCharacterController - no NiRTTI
 *		NiExtraData
 *			TESObjectExtraData
 *			BSFaceGenAnimationData
 *			BSFaceGenModelExtraData
 *			BSFaceGenBaseMorphExtraData
 *			DebugTextExtraData
 *			NiStringExtraData
 *			NiFloatExtraData
 *				FadeNodeMaxAlphaExtraData
 *			BSFurnitureMarker
 *			NiBinaryExtraData
 *			BSBound
 *			NiSCMExtraData
 *			NiTextKeyExtraData
 *			NiVertWeightsExtraData
 *			bhkExtraData
 *			PArrayPoint
 *			NiIntegerExtraData
 *				BSXFlags
 *			NiFloatsExtraData
 *			NiColorExtraData
 *			NiVectorExtraData
 *			NiSwitchStringExtraData
 *			NiStringsExtraData
 *			NiIntegersExtraData
 *			NiBooleanExtraData
 *		NiAdditionalGeometryData
 *			BSPackedAdditionalGeometryData
 *		NiGeometryData
 *			NiLinesData
 *			NiTriBasedGeomData
 *				NiTriStripsData
 *					NiTriStripsDynamicData
 *				NiTriShapeData
 *					NiScreenElementsData
 *					NiTriShapeDynamicData
 *					NiScreenGeometryData
 *			NiParticlesData
 *				NiPSysData
 *					NiMeshPSysData
 *				NiParticleMeshesData
 *		NiTask
 *			BSTECreateTask
 *			NiParallelUpdateTaskManager::SignalTask
 *			NiGeomMorpherUpdateTask
 *			NiPSysUpdateTask
 *		NiSkinInstance
 *		NiSkinPartition
 *		NiSkinData
 *		NiRenderTargetGroup
 *		Ni2DBuffer
 *			NiDepthStencilBuffer
 *		NiUVData
 *		NiStringPalette
 *		NiSequence
 *		NiRotData
 *		NiPosData
 *		NiMorphData
 *		NiTransformData
 *		NiFloatData
 *		NiColorData
 *		NiBSplineData
 *		NiBSplineBasisData
 *		NiBoolData
 *		NiTaskManager
 *			NiParallelUpdateTaskManager
 *		hkPackedNiTriStripsData
 *		NiInterpolator
 *			NiBlendInterpolator
 *				NiBlendTransformInterpolator
 *				NiBlendAccumTransformInterpolator
 *				NiBlendFloatInterpolator
 *				NiBlendQuaternionInterpolator
 *				NiBlendPoint3Interpolator
 *				NiBlendColorInterpolator
 *				NiBlendBoolInterpolator
 *			NiLookAtInterpolator
 *			NiKeyBasedInterpolator
 *				NiFloatInterpolator
 *				NiTransformInterpolator
 *				NiQuaternionInterpolator
 *				NiPoint3Interpolator
 *				NiPathInterpolator
 *				NiColorInterpolator
 *				NiBoolInterpolator
 *					NiBoolTimelineInterpolator
 *			NiBSplineInterpolator
 *				NiBSplineTransformInterpolator
 *					NiBSplineCompTransformInterpolator
 *				NiBSplinePoint3Interpolator
 *					NiBSplineCompPoint3Interpolator
 *				NiBSplineFloatInterpolator
 *					NiBSplineCompFloatInterpolator
 *				NiBSplineColorInterpolator
 *					NiBSplineCompColorInterpolator
 *		NiAVObjectPalette
 *			NiDefaultAVObjectPalette
 *		BSReference
 *		BSNodeReferences
 *		NiPalette
 *		NiLODData
 *			NiRangeLODData
 *			NiScreenLODData
 *		NiPSysModifier
 *			BSWindModifier
 *			NiPSysMeshUpdateModifier
 *			NiPSysRotationModifier
 *			NiPSysEmitter
 *				NiPSysMeshEmitter
 *				NiPSysVolumeEmitter
 *					NiPSysCylinderEmitter
 *					NiPSysSphereEmitter
 *					NiPSysBoxEmitter
 *					BSPSysArrayEmitter
 *			NiPSysGravityModifier
 *			NiPSysSpawnModifier
 *			BSParentVelocityModifier
 *			NiPSysPositionModifier
 *			NiPSysGrowFadeModifier
 *			NiPSysDragModifier
 *			NiPSysColorModifier
 *			NiPSysColliderManager
 *			NiPSysBoundUpdateModifier
 *			NiPSysBombModifier
 *			NiPSysAgeDeathModifier
 *			NiPSysFieldModifier
 *				NiPSysVortexFieldModifier
 *				NiPSysTurbulenceFieldModifier
 *				NiPSysRadialFieldModifier
 *				NiPSysGravityFieldModifier
 *				NiPSysDragFieldModifier
 *				NiPSysAirFieldModifier
 *		NiPSysEmitterCtlrData
 *		NiAccumulator
 *			NiBackToFrontAccumulator
 *				NiAlphaAccumulator
 *					BSShaderAccumulator
 *		NiScreenPolygon
 *		NiScreenTexture
 *		NiPSysCollider
 *			NiPSysSphericalCollider
 *			NiPSysPlanarCollider
 *	
 *	NiShader
 *		NiD3DShaderInterface
 *			NiD3DShader
 *				NiD3DDefaultShader
 *					SkyShader
 *					ShadowLightShader
 *						ParallaxShader
 *						SkinShader
 *						HairShader
 *						SpeedTreeBranchShader
 *					WaterShaderHeightMap
 *					WaterShader
 *					WaterShaderDisplacement
 *					ParticleShader
 *					TallGrassShader
 *					PrecipitationShader
 *					SpeedTreeLeafShader
 *					BoltShader
 *					Lighting30Shader
 *					GeometryDecalShader
 *					SpeedTreeFrondShader
 *					DistantLODShader
 *	
 *	NiD3DShaderConstantMap
 *		NiD3DSCM_Vertex
 *		NiD3DSCM_Pixel
 *	
 ****/

class NiAVObject;
class BSFadeNode;
class NiExtraData;
class NiTimeController;
class NiControllerManager;
class NiStringPalette;
class NiTextKeyExtraData;
class NiCamera;
class NiProperty;
class NiStream;
class TESAnimGroup;
class NiNode;
class NiGeometry;
class ParticleShaderProperty;
class TESObjectCELL;
class TESObjectREFR;
class TESEffectShader;
class NiRenderTargetGroup;
class Ni2DBuffer;
class NiDX9Renderer;
class NiTexture;

// member fn addresses
#if OBLIVION
	#if OBLIVION_VERSION == OBLIVION_VERSION_1_2_416
		const UInt32 kNiObjectNET_GetExtraData = 0x006FF9C0;
	#else
		#error unsupported oblivion version
	#endif
#endif

// 008
class NiRefObject
{
public:
	NiRefObject();
	~NiRefObject();

	virtual void		Destructor(bool freeThis);	// 00

//	void		** _vtbl;		// 000
	UInt32		m_uiRefCount;	// 004 - name known
};

// 008
class NiObject : public NiRefObject
{
public:
	NiObject();
	~NiObject();

	virtual NiRTTI *	GetType(void);
	virtual NiObject *	Unk_02(void);	// NiNode returns 'this'
	virtual UInt32		Unk_03(void);
	virtual UInt32		Unk_04(void);
	virtual UInt32		Unk_05(void);
	virtual NiObject *	Copy(void);
	virtual void		Load(NiStream * stream);
	virtual void		PostLoad(NiStream * stream);
	virtual void		FindNodes(NiStream * stream);	// give NiStream all of the NiNodes we own
	virtual void		Save(NiStream * stream);
	virtual bool		Compare(NiObject * obj);
	virtual void		DumpAttributes(NiTArray <char *> * dst);
	virtual void		DumpChildAttributes(NiTArray <char *> * dst);
	virtual void		Unk_0E(void);
	virtual void		Unk_0F(UInt32 arg);
	virtual void		Unk_10(void);
	virtual void		Unk_11(void);
	virtual void		Unk_12(UInt32 arg);
};

// 018 (used to be 100, delta E8)
class NiObjectNET : public NiObject
{
public:
	NiObjectNET();
	~NiObjectNET();

#if OBLIVION
	MEMBER_FN_PREFIX(NiObjectNET);
	DEFINE_MEMBER_FN(GetExtraData, NiExtraData*, kNiObjectNET_GetExtraData, const char* name);
#endif

	const char			* m_pcName;						// 008 - name known
	NiTimeController	* m_controller;					// 00C

	// doesn't appear to be part of a class?
	NiExtraData			** m_extraDataList;				// 010
	UInt16				m_extraDataListLen;				// 014
	UInt16				m_extraDataListCapacity;		// 016
	// 018

	void SetName(const char* newName);

#if OBLIVION_VERSION <= OBLIVION_VERSION_1_1

	// this padding is probably in NiObjectNET but might actually be in NiAVObject
	// it went away in runtime 1.2 (and probably in cs 1.2 as well)
	UInt32				unk018[(0x080 - 0x018) >> 2];	// 018
	CRITICAL_SECTION	critSection;					// 080
	UInt32				unk098[(0x0AC - 0x098) >> 2];	// 098
	NiTArray <NiAVObject>	unk0AC;						// 0AC
	UInt32				unk0BC[(0x0F8 - 0x0BC) >> 2];	// 0BC
	UInt32				unk0F8;							// 0F8
	UInt32				unk0FC;							// 0FC
	// 100

#endif
};

// 44
struct NiPixelFormat
{
	enum Format
	{
		kFormat_RGB = 0,
		kFormat_RGBA,
		kFormat_PAL,
		kFormat_PALALPHA,
		kFormat_DXT1,
		kFormat_DXT3,
		kFormat_DXT5,
		kFormat_RGB24NONINTERLEAVED,
		kFormat_BUMP,
		kFormat_BUMPLUMA,
		kFormat_RENDERERSPECIFIC,
		kFormat_ONE_CHANNEL,
		kFormat_TWO_CHANNEL,
		kFormat_THREE_CHANNEL,
		kFormat_FOUR_CHANNEL,
		kFormat_DEPTH_STENCIL,
		kFormat_UNKNOWN,
		kFormat_MAX
	};

	enum Component
	{
		kComp_RED = 0,
		kComp_GREEN,
		kComp_BLUE,
		kComp_ALPHA,
		kComp_COMPRESSED,
		kComp_OFFSET_U,
		kComp_OFFSET_V,
		kComp_OFFSET_W,
		kComp_OFFSET_Q,
		kComp_LUMA,
		kComp_HEIGHT,
		kComp_VECTOR_X,
		kComp_VECTOR_Y,
		kComp_VECTOR_Z,
		kComp_PADDING,
		kComp_INTENSITY,
		kComp_INDEX,
		kComp_DEPTH,
		kComp_STENCIL,
		kComp_EMPTY,
		kComp_MAX
	};

	enum Representation
	{
		kRep_NORM_INT = 0,
		kRep_HALF,
		kRep_FLOAT,
		kRep_INDEX,
		kRep_COMPRESSED,
		kRep_UNKNOWN,
		kRep_INT,
		kRep_MAX
	};

	enum Tiling
	{
		kTile_NONE = 0,
		kTile_XENON,
		kTile_MAX
	};

	UInt8	BitsPerPixel;	// 00
	UInt8	SRGBSpace;		// 01
	UInt8	pad02[2];
	Format	eFormat;		// 04
	Tiling	eTiling;		// 08
	UInt32	RendererHint;	// 0C
	UInt32	ExtraData;		// 10

	struct NiComponentSpec
	{
		Component		eComponent;			// 0
		Representation	eRepresentation;	// 4
		UInt8			BitsPerComponent;	// 8
		UInt8			Signed;				// 9
		UInt8			padA[2];			// A
	};

	NiComponentSpec	Components[4];	// 14

	void InitFromD3DFMT(UInt32 fmt);
};

class NiTextureData
{
public:
	NiTextureData();
	virtual ~NiTextureData();

	NiTexture*		pTexture;		// 04
	NiDX9Renderer*	pRenderer;		// 08
	NiPixelFormat	PixelFormat;	// 0C
};

class NiDX9TextureData : public NiTextureData
{
public:
	NiDX9TextureData();
	virtual ~NiDX9TextureData();
	
	IDirect3DBaseTexture9*	dTexture;		// 50
	UInt32					Width;			// 54
	UInt32					Height;			// 58
	UInt32					Levels;			// 5C
};

class NiDX9RenderedTextureData : public NiDX9TextureData
{
public:
	NiDX9RenderedTextureData();
	~NiDX9RenderedTextureData();

	UInt32					unk60;			// 60
};

class NiDX9SourceTextureData : public NiDX9TextureData
{
public:
	NiDX9SourceTextureData();
	~NiDX9SourceTextureData();

	UInt32					unk60;				// 60
	UInt8					ReplacementData;	// 64
	UInt8					Mipmap;				// 65
	UInt8					pad64;
	UInt32					FormattedSize;		// 68
	UInt32					Palette;			// 6C NiPalette
	UInt32					LevelsSkipped;		// 70
	UInt32					SourceRevID;		// 74
	UInt32					PalRevID;			// 78
};


// 030
class NiTexture : public NiObjectNET
{
public:
	NiTexture();
	~NiTexture();

	virtual UInt32	GetWidth();
	virtual UInt32	GetHeight();

	enum PixelLayout
	{
		kPixelLayout_Palette8BPP = 0,
		kPixelLayout_Raw16BPP,
		kPixelLayout_Raw32BPP,
		kPixelLayout_Compressed,
		kPixelLayout_Bumpmap,
		kPixelLayout_Palette4BPP,
		kPixelLayout_Default,
	};
	
	enum AlphaFormat
	{
		kAlpha_None = 0,
		kAlpha_Binary,	// 1bpp
		kAlpha_Smooth,	// 8bpp
		kAlpha_Default,
	};

	enum MipMapFlag
	{
		kMipMap_Disabled = 0,
		kMipMap_Enabled,
		kMipMap_Default,
	};
	
	struct FormatPrefs {
		PixelLayout pixelLayout;
		AlphaFormat alphaFormat;
		MipMapFlag	mipmapFormat;
	};

	UInt32			pixelLayout;	// 018
	UInt32			alphaFormat;	// 01C
	UInt32			mipmapFormat;	// 020
	NiTextureData*	rendererData;	// 024
	NiTexture*		nextTex;		// 028 - linked list updated in ctor/dtor
	NiTexture*		prevTex;		// 02C
};

// 048
class NiSourceTexture : public NiTexture
{
public:
	NiSourceTexture();
	~NiSourceTexture();

	virtual void	Unk_15(void);
	virtual void	FreePixelData(void);
	virtual bool	Unk_17(void);

	UInt8		unk030;				// 030 - is static?
	UInt8		unk031[3];			// 031
	void		* unk034;			// 034
	const char	* fileName;			// 038
	NiObject	* pixelData;		// 03C - NiPixelData
	UInt8		loadDirectToRender;	// 040
	UInt8		persistRenderData;	// 041
	UInt8		pad042[2];			// 042
	void		* unk044;			// 044
};

// 04C
class NiSourceCubeMap : public NiSourceTexture
{
public:
	NiSourceCubeMap();
	~NiSourceCubeMap();

	UInt32	unk048;	// 048
};

// 040
class NiRenderedTexture : public NiTexture
{
public:
	NiRenderedTexture();
	~NiRenderedTexture();

	virtual void*	Unk_15(void);

	Ni2DBuffer* buffer;		// 030
	UInt8		unk034;		// 034
	UInt8		pad034[3];
	D3DFORMAT	format;		// 038
	UInt8		unk03C;		// 03C
	UInt8		pad03C;

};

// 05C
class NiRenderedCubeMap : public NiRenderedTexture
{
public:
	NiRenderedCubeMap();
	~NiRenderedCubeMap();

	UInt32		unk040;		// 040
	Ni2DBuffer* faces[6];	// 044
};

class BSRenderedTexture : public NiRefObject
{
public:
	BSRenderedTexture();
	~BSRenderedTexture();
	
	NiRenderTargetGroup*	RenderTargetGroup;	// 008
	UInt32					unk008[5];			// 00C
	NiRenderedTexture*		RenderedTexture;	// 020
};

// 018
class NiSequenceStreamHelper : public NiObjectNET
{
public:
	NiSequenceStreamHelper();
	~NiSequenceStreamHelper();
};

// 070
class NiPixelData : public NiObject
{
public:
	NiPixelData();
	~NiPixelData();

	// face size = unk05C[mipmapLevels]
	// total size = face size * numFaces

	NiPixelFormat	format;		// 008
	NiRefObject		* unk04C;	// 04C
	UInt32	unk050;			// 050
	UInt32	* width;		// 054 - array for mipmaps?
	UInt32	* height;		// 058
	UInt32	* unk05C;		// 05C - sizes?
	UInt32	mipmapLevels;	// 060
	UInt32	unk064;			// 064
	UInt32	unk068;			// 068
	UInt32	numFaces;		// 06C
};

// 068
class NiControllerSequence : public NiObject
{
public:
	NiControllerSequence();
	~NiControllerSequence();

	enum
	{
		kState_Inactive = 0,
		kState_Animating,
		kState_EaseIn,
		kState_EaseOut,
		kState_TransSource,
		kState_TransDest,
		kState_MorphSource
	};

	enum
	{
		kCycle_Loop = 0,
		kCycle_Reverse,
		kCycle_Clamp,
	};

	// 10
	struct ArrayItem
	{
		NiRefObject*	unk00;		// 00
		NiRefObject*	unk04;		// 04
		UInt32			unk08;		// 08
		UInt8			unk0C;		// 0C
		UInt8			pad0C[3];
	};

	// 10
	struct Unk018
	{
		NiRefObject	* unk00;	// 00
		UInt16		unk04;		// 04
		UInt16		unk06;		// 06
		UInt16		unk08;		// 08
		UInt16		unk0A;		// 0A
		UInt16		unk0C;		// 0C
		UInt8		pad0E[2];	// 0E
	};

	char				* filePath;		// 008
	UInt32				arraySize;		// 00C
	UInt32				arrayGrowBy;	// 010
	ArrayItem*			interpArray;	// 014
	Unk018				* unk018;		// 018
	float				weight;			// 01C
	NiTextKeyExtraData	* TextKeys;		// 020
	UInt32				cycleType;		// 024
	float				freq;			// 028
	float				beginkey;		// 02C
	float				endkey;			// 030
	float				last;			// 034
	float				weightLast;		// 038
	float				lastScaled;		// 03C
	NiControllerManager * controllerMgr;// 040
	UInt32				state;			// 044
	float				offset;			// 048
	float				start;			// 04C - offset * -1?
	float				end;			// 050
	UInt32				unk054;			// 054
	UInt32				unk058;			// 058
	char				* accumRootName;// 05C
	NiAVObject			* accumRoot;	// 060
	NiStringPalette		* unk064;		// 064
};

// 06C
class BSAnimGroupSequence : public NiControllerSequence
{
public:
	BSAnimGroupSequence();
	~BSAnimGroupSequence();

	TESAnimGroup		* animGroup;	//068
};

// 02C+
class TESAnimGroup : public NiRefObject
{
public:
	// derived from NiRefObject
	TESAnimGroup();
	~TESAnimGroup();

	virtual void Destructor(bool arg0);

	enum{
		kAnimGroup_Idle 		=	0,
		kAnimGroup_DynamicIdle,
		kAnimGroup_SpecialIdle,
		kAnimGroup_Forward,	
		kAnimGroup_Backward,
		kAnimGroup_Left,
		kAnimGroup_Right,
		kAnimGroup_FastForward,
		kAnimGroup_FastBackward,
		kAnimGroup_FastLeft,
		kAnimGroup_FastRight,
		kAnimGroup_DodgeForward,
		kAnimGroup_DodgeBack,
		kAnimGroup_DodgeLeft,
		kAnimGroup_DodgeRight,
		kAnimGroup_TurnLeft,
		kAnimGroup_TurnRight,
		kAnimGroup_Equip,
		kAnimGroup_Unequip,
		kAnimGroup_AttackBow,
		kAnimGroup_AttackLeft,
		kAnimGroup_AttackRight,
		kAnimGroup_AttackPower, 
		kAnimGroup_AttackForwardPower,
		kAnimGroup_AttackBackPower,
		kAnimGroup_AttackLeftPower,
		kAnimGroup_AttackRightPower,
		kAnimGroup_BlockIdle,
		kAnimGroup_BlockHit,
		kAnimGroup_BlockAttack,
		kAnimGroup_Recoil,
		kAnimGroup_Stagger,
		kAnimGroup_Death,
		kAnimGroup_TorchIdle,
		kAnimGroup_CastSelf,
		kAnimGroup_CastTouch,
		kAnimGroup_CastTarget,
		kAnimGroup_CastSelfAlt,
		kAnimGroup_CastTouchAlt,
		kAnimGroup_CastTargetAlt,
		kAnimGroup_JumpStart,
		kAnimGroup_JumpLoop,
		kAnimGroup_JumpLand,

		kAnimGroup_Max,
	};

	// 24
	struct AnimGroupInfo {
		const char	* name;				// 00
		UInt8		sequenceType;		// 04	0 = AnimSequenceSingle, 1 = AnimSequenceMultiple
		UInt8		pad[3];
		UInt32		prioritySlot;		// 08
		UInt32		loopingMode;		// 0C
		UInt32		unk10;				// 10
		UInt32		unk14[4];			// 14
	};

	UInt8		animGroup;		//008 This value is used with the animType (as UInt16) as key for the ActorAnimData NiTPointerMap<AnimSequenceBase>*
	UInt8		animType;		//009
	UInt8		animOR;			//00A Oblivion doesn't use this. Used by Oblivion Reloaded if installed.
	UInt8		pad008;			//00B
	UInt32		numFrames;		//00C count of group frames (Start, Detach, Attack, End, etc)
	float		** frameData;	//010 pointer to float array of group frame times (size numFrames)
	UInt32		unk014;			//014
	UInt32		unk018;			//018
	UInt32		unk01C;			//01C
	UInt8		unk020;			//020
	UInt8		unk021;
	UInt8		pad022[2];
	UInt32		unk024;			//024
	void		* unk028;		//028

	static const char* StringForAnimGroupCode(UInt32 groupCode);
	static UInt32 AnimGroupForString(const char* groupName);
};

// derived from NiFile, which derives from NiBinaryStream
// 154
class BSFile
{
public:
	BSFile();
	~BSFile();

	virtual void	Destructor(bool freeMemory);				// 00
	virtual void	Unk_01(void);								// 04
	virtual void	Unk_02(void);								// 08
	virtual void	Unk_03(void);								// 0C
	virtual void	Unk_04(void);								// 10
	virtual void	DumpAttributes(NiTArray <char *> * dst);	// 14
	virtual UInt32	GetSize(void);								// 18
	virtual void	Unk_07(void);								// 1C
	virtual void	Unk_08(void);								// 20
	virtual void	Unk_09(void);								// 24
	virtual void	Unk_0A(void);								// 28
	virtual void	Unk_0B(void);								// 2C
	virtual void	Unk_0C(void);								// 30
	virtual void	Unk_Read(void);								// 34
	virtual void	Unk_Write(void);							// 38

//	void	** m_vtbl;		// 000
	void	* m_readProc;	// 004 - function pointer
	void	* m_writeProc;	// 008 - function pointer
	UInt32	m_bufSize;		// 00C
	UInt32	m_unk010;		// 010 - init'd to m_bufSize
	UInt32	m_unk014;		// 014
	void	* m_buf;		// 018
	FILE	* m_file;		// 01C
	UInt32	m_writeAccess;	// 020
	UInt8	m_good;			// 024
	UInt8	m_pad025[3];	// 025
	UInt8	m_unk028;		// 028
	UInt8	m_pad029[3];	// 029
	UInt32	m_unk02C;		// 02C
	UInt32	m_pos;			// 030
	UInt32	m_unk034;		// 034
	UInt32	m_unk038;		// 038
	char	m_path[0x104];	// 03C
	UInt32	m_unk140;		// 140
	UInt32	m_unk144;		// 144
	UInt32	m_pos2;			// 148 - used if m_pos is 0xFFFFFFFF
	UInt32	m_unk14C;		// 14C
	UInt32	m_fileSize;		// 150
};

/**** misc non-NiObjects ****/

// 30
class NiPropertyState : public NiRefObject
{
public:
	NiPropertyState();
	~NiPropertyState();

	NiProperty* prop[10]; // 008 Array size is NiProperty::kType_MAXTYPES
	//  0 08
	//  1 0C
	//  2 10
	//  3 14
	//  4 18
	//  5 1C
	//  6 20
	//  7 24
	//  8 28
	//  9 2C
};

// 20
class NiDynamicEffectState : public NiRefObject
{
public:
	NiDynamicEffectState();
	~NiDynamicEffectState();

	UInt8	unk008;		// 008
	UInt8	pad009[3];	// 009
	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
	UInt32	unk01C;		// 01C
};

class NiVisibleArray
{
public:
	NiVisibleArray();
	~NiVisibleArray();

	NiGeometry**	geo;			// 00
	UInt32			numItems;		// 04
	UInt32			bufLen;			// 08
	UInt32			bufGrowSize;	// 0C
};

// 90
class NiCullingProcess
{
public:
	NiCullingProcess();
	~NiCullingProcess();

	virtual void	Destructor(bool freeMemory);
	virtual void	ProcessCull(NiAVObject* Object);
	virtual void	Process(NiCamera* Camera, NiAVObject* Object, NiVisibleArray* VisibleGeo);
	virtual void	AppendVirtual(NiGeometry* Geo);

	UInt8			UseAppendVirtual;	// 04 - always false
	UInt8			pad05[3];			// 05
	NiVisibleArray*	VisibleGeo;			// 08
	NiCamera*		Camera;				// 0C
	NiFrustum		CameraFrustum;		// 10
	NiFrustumPlanes	Planes;				// 2C
};

STATIC_ASSERT(sizeof(NiCullingProcess) == 0x90);

/**** BSTempEffects ****/

// 18
class BSTempEffect : public NiObject
{
public:
	BSTempEffect();
	~BSTempEffect();

	float			duration;		// 08
	TESObjectCELL*	cell;			// 0C
	float			unk10;			// 10
	UInt8			unk14;			// 14
	UInt8			pad15[3];
};

// 28
class MagicHitEffect : public BSTempEffect
{
public:
	MagicHitEffect();
	~MagicHitEffect();

	UInt32			unk18;		// 18
	TESObjectREFR	* target;	// 1C
	float			unk20;		// 20
	UInt8			unk24;		// 24
	UInt8			pad25[3];
};

// 4C
class MagicShaderHitEffect : public MagicHitEffect
{
public:
	MagicShaderHitEffect();
	~MagicShaderHitEffect();

	UInt8					unk28;			// 28
	UInt8					pad29[3];
	UInt32					unk2C;			// 2C
	UInt32					unk30;			// 30
	TESEffectShader			* effectShader;	// 34
	float					unk38;			// 38
	ParticleShaderProperty	* particleProp;	// 3C
	NiNode					* niNode;		// 40
	UInt8					unk44;			// 44
	UInt8					pad45[3];
	void					* textureEffectData;	// 48 seen TextureEffectData< BSSahderLightingProperty >
};
