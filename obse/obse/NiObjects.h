#pragma once

#include "obse/NiNodes.h"

/***										type
 *	NiAVObject
 *		NiDynamicEffect
 *			NiLight
 *				NiDirectionalLight			01
 *				NiPointLight				02
 *					NiSpotLight				03
 *				NiAmbientLight				00
 *			NiTextureEffect					04
 *		NiNode
 *			SceneGraph
 *			BSTempNodeManager
 *			BSTempNode
 *			BSCellNode
 *			BSClearZNode
 *			BSFadeNode
 *			BSScissorNode
 *			BSTimingNode
 *			BSFaceGenNiNode
 *			NiBillboardNode
 *			NiSwitchNode
 *				NiLODNode
 *					NiBSLODNode
 *			NiSortAdjustNode
 *			NiBSPNode
 *			ShadowSceneNode
 *		NiCamera
 *			BSCubeMapCamera					RTTI data incorrect
 *			NiScreenSpaceCamera
 *		NiGeometry
 *			NiLines
 *			NiTriBasedGeom
 *				NiTriShape
 *					BSScissorTriShape
 *					NiScreenElements
 *					NiScreenGeometry
 *					TallGrassTriShape
 *				NiTriStrips
 *					TallGrassTriStrips
 *			NiParticles
 *				NiParticleSystem
 *					NiMeshParticleSystem
 *				NiParticleMeshes
 */

class NiCamera;
class NiLODData;
class BSImageSpaceShader;
class NiScreenPolygon;
class NiScreenTexture;
class NiPSysModifier;
class NiRenderer;
class NiD3DShader;
class NiAdditionalGeometryData;
class NiGeometryGroup;

// 0AC
class NiAVObject : public NiObjectNET
{
public:
	NiAVObject();
	~NiAVObject();
	
	virtual void			UpdateControllers(float fTime);
	virtual void			Unk_14(void);
	virtual void			Unk_15(void);
	virtual NiAVObject*		GetObjectByName(const char* Name);
	virtual void*			Unk_17(void);
	virtual void			UpdateDownwardPass(float fTime, bool bUpdateControllers);
	virtual void			UpdateSelectedDownwardPass(float fTime);
	virtual void			UpdateRigidDownwardPass(float fTime);
	virtual void			UpdatePropertiesDownward(NiPropertyState* ParentState);
	virtual void			UpdateEffectsDownward(NiDynamicEffectState* ParentState);
	virtual void			UpdateWorldData();
	virtual void			UpdateWorldBound();
	virtual void			OnVisible(NiCullingProcess* CullingProcess);
	virtual void			Unk_20(void * arg);			// get NiMaterialProperty, pass to arg if found

	enum
	{
		kFlag_AppCulled =					1 << 0,	// originally named m_bAppCulled but they appear to have used bitfields
		kFlag_SelUpdate =					1 << 1, // Selective update flags
		kFlag_SelUpdateTransforms =			1 << 2,
		kFlag_SelUpdatePropControllers =	1 << 3,
		kFlag_SelUpdateRigid =				1 << 4,
		kFlag_DisplayObject =				1 << 5, // For use with occlusion culling system
		kFlag_DisableSorting =				1 << 6, // For use with sorting/accumulation system
		kFlag_SelTransformsOverride =		1 << 7, // Selective update over-ride flags
		kFlag_IsNode =						1 << 8, // Is the object a leaf or a node
	};

	UInt16						m_flags;				// 018
	UInt8						pad01A[2];				// 01A
	NiNode*						m_parent;				// 01C
	NiSphere					m_kWorldBound;			// 020
	NiTransform					m_localTransform;		// 030
	NiTransform					m_worldTransform;		// 064
	NiTListBase <NiProperty>	m_propertyList;			// 098
	NiObject					* m_spCollisionObject;	// 0A8
};
#if OBLIVION
STATIC_ASSERT(sizeof(NiAVObject) == 0x0AC);
#endif

// DC+
class NiDynamicEffect : public NiAVObject
{
public:
	NiDynamicEffect();
	~NiDynamicEffect();

	virtual UInt32	GetEffectType(void);

	UInt8	enable;		// 0AC
	UInt8	pad0AD[3];	// 0AD
	UInt32	unk0B0;		// 0B0 - init'd to monotonically increasing number (uid?)
	UInt32	unk0B4;		// 0B4
	UInt32	unk0B8;		// 0B8 - init'd to 1

	NiTPointerList <NiNode>		affectedNodes;		// 0BC
	NiTPointerList <NiNode>		unaffectedNodes;	// 0CC
};

// 108
class NiLight : public NiDynamicEffect
{
public:
	NiLight();
	~NiLight();

	float	m_fDimmer;	// 0DC
	NiColor	m_kAmb;		// 0E0
	NiColor	m_kDiff;	// 0EC
	NiColor	m_kSpec;	// 0F8
	UInt32	unk104;		// 104
};

// 114
class NiDirectionalLight : public NiLight
{
public:
	NiDirectionalLight();
	~NiDirectionalLight();

	// UpdateTransform ticks unk0B8 and copies m_direction

	enum { kType = 1 };

	NiPoint3	m_direction;	// 108
};

// 114
class NiPointLight : public NiLight
{
public:
	NiPointLight();
	~NiPointLight();

	// UpdateTransform ticks unk0B8

	enum { kType = 2 };

	float	m_fAtten0;	// 108
	float	m_fAtten1;	// 10C
	float	m_fAtten2;	// 110
};

// 128
class NiSpotLight : public NiPointLight
{
public:
	NiSpotLight();
	~NiSpotLight();

	// UpdateTransform ticks unk0B8 and copies m_kWorldDir

	enum { kType = 3 };

	NiPoint3	m_kWorldDir;		// 114
	float		m_fSpotAngle;		// 120
	float		m_fSpotExponent;	// 124
};

// 108
class NiAmbientLight : public NiLight
{
public:
	NiAmbientLight();
	~NiAmbientLight();

	enum { kType = 0 };
};

// 174
class NiTextureEffect : public NiDynamicEffect
{
public:
	NiTextureEffect();
	~NiTextureEffect();

	enum { kType = 4 };

	enum
	{
		kTextureMode_ProjectedLight = 0,
		kTextureMode_ProjectedShadow,
		kTextureMode_EnvironmentMap,
		kTextureMode_FogMap,
	};

	enum
	{
		kCoordMode_WorldParallel = 0,
		kCoordMode_WorldPerspective,
		kCoordMode_SphereMap,
		kCoordMode_SpecularCubeMap,
		kCoordMode_DiffuseCubeMap,
	};

	NiMatrix33	m_kModelProjMat;	// 0DC
	NiPoint3	m_kModelProjTrans;	// 100
	NiMatrix33	m_kWorldProjMat;	// 10C
	NiPoint3	m_kWorldProjTrans;	// 130
	NiObject	* m_texture;		// 13C
	UInt32		m_eFilter;			// 140 - see NiTexturingProperty::kFilterMode_* without the shift
	UInt32		m_eClamp;			// 144 - see NiTexturingProperty::kClampMode_* without the shift
	UInt32		m_eTextureMode;		// 148
	UInt32		m_eCoordMode;		// 14C
	UInt8		m_bPlaneEnable;		// 150
	UInt8		pad151[3];			// 151
	NiPlane		m_kModelPlane;		// 154
	NiPlane		m_kWorldPlane;		// 164
};

// DC
class NiNode : public NiAVObject
{
public:
	NiNode();
	~NiNode();

	virtual void		AddObject(NiAVObject* nChild, UInt8 FirstAvail); // Add child node
	virtual NiAVObject*	RemoveObject(NiAVObject** RemovedChild, NiAVObject* nChild);
	virtual NiAVObject*	RemoveObjectAt(NiAVObject** RemovedChild, UInt32 Index);
	virtual NiAVObject*	SetObjectAt(NiAVObject** SetChild, UInt32 Index, NiAVObject* nChild);
	virtual void		ApplyTransform(const NiMatrix33& Mat, const NiPoint3& Trn, bool OnLeft);
	virtual bool		Unk_26(void);

	NiTArray <NiAVObject *>			m_children;			// 0AC
	NiTListBase <NiDynamicEffect *>	m_effects;			// 0BC
	NiSphere						m_combinedBounds;	// 0CC
};

// F0
class SceneGraph : public NiNode
{
public:
	SceneGraph();
	~SceneGraph();

	NiCamera			* camera;				// 0DC
	UInt32				unk0E0;					// 0E0
	NiCullingProcess	* cullingProcess;		// 0E4
	UInt32				IsMinFarPlaneDistance;	// 0E8 The farplane is set to 20480.0 when the flag is true. Probably used for interiors.
	float				cameraFOV;				// 0EC
};
#if OBLIVION
STATIC_ASSERT(sizeof(SceneGraph) == 0x0F0);
#endif

// E0
class BSTempNodeManager : public NiNode
{
public:
	BSTempNodeManager();
	~BSTempNodeManager();

	// overrides Unk_18 and related to scan children for BSTempNodes
	// removes them? tests against unk0DC

	float	unk0DC;	// 0DC
};

// E0
class BSTempNode : public NiNode
{
public:
	BSTempNode();
	~BSTempNode();

	float	unk0DC;	// 0DC
};

// BSCellNode - in RTTI but unref'd

// E8
class BSClearZNode : public NiNode
{
public:
	BSClearZNode();
	~BSClearZNode();

	// in Draw, scan for BSShaderAccumulator on the renderer, set 21E0 to 0 before calling parent Draw, reset to 1 afterwards

	UInt8	unk0DC;	// 0DC
	UInt8	unk0DD;	// 0DD
	float	unk0E0;	// 0E0 - init'd to 283840
	float	unk0E4;	// 0E4 - init'd to 100
};

// F0
class BSFadeNode : public NiNode
{
public:
	BSFadeNode();
	~BSFadeNode();

	// overload Draw to do LOD-based fade

	UInt8	unk0DC;			// 0DC
	UInt8	unk0DD[3];		// 0DD
	float	fNearDistSqr;	// 0E0
	float	fFarDistSqr;	// 0E4
	float	unk0E8;			// 0E8
	UInt8	cMultType;		// 0EC
	UInt8	unk0ED[3];		// 0ED
};

// EC
class BSScissorNode : public NiNode
{
public:
	BSScissorNode();
	~BSScissorNode();

	// mess about with renderer in Draw (presumably to set scissor mode)

	UInt32	unk0DC;	// 0DC
	UInt32	unk0E0;	// 0E0
	UInt32	unk0E4;	// 0E4
	UInt32	unk0E8;	// 0E8
};

// BSTimingNode - unref'd

class Actor;

// 118
class BSFaceGenNiNode : public NiNode
{
public:
	BSFaceGenNiNode();
	~BSFaceGenNiNode();

	virtual NiObject *	GetAnimationData(void);
	virtual void		SetAnimationData(NiObject * obj);
	virtual float *		GetRotation(float * dst);
	virtual void		SetRotation(float * src);	// ticks bRotatedLastUpdate
	virtual bool		GetAnimationUpdate(void);
	virtual void		SetAnimationUpdate(bool arg);
	virtual bool		GetApplyRotationToParent(void);
	virtual void		SetApplyRotationToParent(bool arg);
	virtual bool		GetFixedNormals(void);
	virtual void		SetFixedNormals(bool arg);
	virtual void		Unk_31(void * arg);	// scans children, "could not find bone for the head node"

	// spAnimationData->Unk15(idx) returns emotion weighting (max 0x0C)
	// spAnimationData->Unk17(idx) returns movement weighting (max 0x10)
	// spAnimationData->Unk1A(idx) returns phoneme weighting (max 0x0F)

	NiObject	* spAnimationData;		// 0DC
	float		m_rotation[9];			// 0E0 - init'd to identity mtx
	UInt8		bForceBaseMorph;		// 104
	UInt8		bFixedNormals;			// 105
	UInt8		bAnimationUpdate;		// 106
	UInt8		bRotatedLastUpdate;		// 107
	UInt8		bApplyRotationToParent;	// 108
	UInt8		pad109[3];				// 109
	float		fLastTime;				// 10C - init'd to -1
	UInt8		bUsingLoResHead;		// 110
	UInt8		bIAmPlayerCharacter;	// 111
	UInt8		bIAmInDialouge;			// 112
	UInt8		pad113;					// 113
	Actor		* pActor;				// 114
};

// E4
class NiBillboardNode : public NiNode
{
public:
	NiBillboardNode();
	~NiBillboardNode();

	enum
	{
		kMode_AlwaysFaceCamera =	0,
		kMode_RotateAboutUp,
		kMode_RigidFaceCamera,
		kMode_AlwaysFaceCenter,
		kMode_RigidFaceCenter,
		kMode_BSRotateAboutUp,
		kMode_Mask =				7,

		kFlag_Unk3 =				1 << 3,
	};

	// update functions set kFlag_Unk3

	UInt16	flags;				// 0DC
	UInt8	pad0DE[2];			// 0DE
	float	m_lastUpdateTime;	// 0E0
};

// FC
class NiSwitchNode : public NiNode
{
public:
	NiSwitchNode();
	~NiSwitchNode();

	// only updates/displays/etc child with index m_iIndex
	// keeps track of the last time each child was updated to save work

	enum
	{
		kUpdateOnlyActive =	1 << 0,
	};

	UInt16				flags;			// 0DC
	UInt8				pad0DE[2];		// 0DE
	UInt32				m_iIndex;		// 0E0 - active child idx
	float				lastUpdateTime;	// 0E4
	UInt32				curUpdateIdx;	// 0E8
	NiTArray <UInt32>	childUpdateIdx;	// 0EC - last update idx for each child
};

// 104
class NiLODNode : public NiSwitchNode
{
public:
	NiLODNode();
	~NiLODNode();

	NiLODData	* lodData;	// 0FC
	UInt8		unk100;		// 100
	UInt8		pad101[3];	// 101
};

// NiBSLODNode - unref'd

// E0
class NiSortAdjustNode : public NiNode
{
public:
	NiSortAdjustNode();
	~NiSortAdjustNode();

	UInt32	unk0DC;	// 0DC
};

// FC
class NiBSPNode : public NiNode
{
public:
	NiBSPNode();
	~NiBSPNode();

	NiPlane	m_kModelPlane;	// 0DC
	NiPlane	m_kWorldPlane;	// 0EC
};

// 64
struct BVIntersectionData
{
	NiPlane							planes[6];				// 00
	UInt32							intersections;			// 60 - bit field, mask = 0x3F (6 bits)
};
STATIC_ASSERT(sizeof(BVIntersectionData) == 0x64);

// 124
class NiCamera : public NiAVObject
{
public:
	NiCamera();
	~NiCamera();

	UInt32		unk0AC[(0xEC - 0xAC) >> 2];	// 0AC
	NiFrustum	Frustum;					// 0EC
	float		MinNearPlaneDist;			// 108
	float		MaxFarNearRatio;			// 10C
	NiViewport	ViewPort;					// 110
	float		unk120;						// 120
};

// 150
class BSCubeMapCamera : public NiCamera
{
public:
	BSCubeMapCamera();
	~BSCubeMapCamera();

	// 1C
	class Str14C
	{
	public:
		Str14C();
		~Str14C();

		NiTPointerList <BSImageSpaceShader *>	shaders;	// 00
		NiObject	* unk010;	// 10
		NiObject	* unk014;	// 14
		UInt32		unk018;		// 18
	};

	UInt32		unk124;			// 124
	NiObject	* unk128[6];	// 128
	NiObject	* unk140;		// 140
	UInt32		pad144;			// 144
	NiObject	* unk148;		// 148
	Str14C		* unk14C;		// 14C
};

// 144
class NiScreenSpaceCamera : public NiCamera
{
public:
	NiScreenSpaceCamera();
	~NiScreenSpaceCamera();

	NiTArray <NiScreenPolygon *>	polygons;	// 124
	NiTArray <NiScreenTexture *>	textures;	// 134
};

class NiVBChip
{
public:
	NiVBChip();
	~NiVBChip();

	UInt32					Index;      // 00
	UInt32					Unk04;		// 04 union { NiVBBlock* m_pkBlock; NiVBDynamicSet* m_pkDynamicSet; };
	IDirect3DVertexBuffer9*	VB;			// 08
	UInt32					Offset;		// 0C
	UInt32					LockFlags;	// 10
	UInt32					Size;		// 14

	NiVBChip*				Next;		// 18
	NiVBChip*				Prev;		// 1C

};

class NiGeometryBufferData
{
public:
	NiGeometryBufferData();
	~NiGeometryBufferData();

	UInt32							Flags;					// 00
	NiGeometryGroup*				GeometryGroup;			// 04
	UInt32							FVF;					// 08
	IDirect3DVertexDeclaration9*	VertexDeclaration;		// 0C
	UInt32							unk10;					// 10
	UInt32							VertCount;				// 14
	UInt32							MaxVertCount;			// 18
	UInt32							StreamCount;			// 1C
	UInt32*							VertexStride;			// 20
	NiVBChip**						VBChip;					// 24
	UInt32							IndexCount;				// 28
	UInt32							IBSize;					// 2C
	IDirect3DIndexBuffer9*			IB;						// 30
	UInt32							BaseVertexIndex;		// 34
	D3DPRIMITIVETYPE				PrimitiveType;			// 38
	UInt32							TriCount;				// 3C
	UInt32							MaxTriCount;			// 40
	UInt32							NumArrays;				// 44
	UInt16*							ArrayLengths;			// 48
	UInt16*							IndexArray;				// 4C

};

// 40
class NiGeometryData : public NiObject
{
public:
	NiGeometryData();
	~NiGeometryData();

	enum Consistency
	{
		MUTABLE	=			0x0000,
		STATIC =			0x4000,
		VOLATILE =			0x8000,
		CONSISTENCY_MASK =	0xF000,
	};

	UInt16						Vertices;			// 008
	UInt16						Unk00A;				// 00A
	UInt32						Unk00C;				// 00C
	UInt32						Unk010;				// 010
	UInt32						Unk014;				// 014
	UInt32						Unk018;				// 018
	NiPoint3*					Vertex;				// 01C
	NiPoint3*					Normal;				// 020
	NiColorAlpha*				Color;				// 024
	UInt32						Texture;			// 028 NiPoint2*
	UInt16						ActiveVertices;		// 02C
	UInt16						DataFlags;			// 02E
	UInt8						KeepFlags;			// 030
	UInt8						CompressFlags;		// 031
	UInt8						pad030[2];
	NiAdditionalGeometryData*	AdditionalGeomData;	// 034
	NiGeometryBufferData*		BuffData;			// 038
	UInt8						Unk03C;				// 03C
	UInt8						Unk03D;				// 03D
	UInt8						pad03C[2];
};

class NiSkinPartition;

// 48
class NiSkinData : public NiObject
{
public:
	NiSkinData();
	~NiSkinData();

	class  BoneVertData
	{
	public:
		BoneVertData();

		UInt16	Verts;		// 00
		float	Weight;		// 04

	};

	class BoneData
	{
	public:
		BoneData();

		NiTransform		SkinToBone;		// 00
		NiPoint3		Center;			// 34
		float			Radius;			// 38
		BoneVertData*	BoneVertData;	// 3C
		UInt16			Verts;			// 40
		UInt16			pad44;
	};

	NiSkinPartition*	SkinPartition;		// 08
	NiTransform			RootParentToSkin;	// 0C
	UInt32				Bones;				// 40
	BoneData*			BoneData;			// 44

};

// 10
class NiSkinPartition : public NiObject
{
public:
	NiSkinPartition();
	~NiSkinPartition();

	class Partition
	{
	public:
		Partition();
		virtual ~Partition();

		UInt16*					pBones;			// 04
		float*					Weights;		// 08
		UInt32					Unk04[4];		// 0C
		UInt16					Vertices;		// 1C
		UInt16					Triangles;		// 1E
		UInt16					Bones;			// 20
		UInt16					Strips;			// 22
		UInt16					BonesPerVertex; // 24
		UInt16					pad28;
		NiGeometryBufferData*	BuffData;		// 28
	};
	
	UInt32				PartitionsCount;		// 08
	Partition*			Partitions;				// 0C

};

// 2C
class NiSkinInstance : public NiObject
{
public:
	NiSkinInstance();
	~NiSkinInstance();

	NiSkinData*			SkinData;				// 08
	NiSkinPartition*	SkinPartition;			// 0C
	NiAVObject*			RootParent;				// 10
	NiAVObject**		BonesObject;			// 14
	UInt32				FrameID;				// 18
	UInt32				Bones;					// 1C
	void*				BoneMatrixes;			// 20 Matrixes array for the bones D3DMATRIX
	UInt32				BoneRegisters;			// 24
	void*				SkinToWorldWorldToSkin;	// 28 D3DMATRIX

};

// C0
class NiGeometry : public NiAVObject
{
public:
	NiGeometry();
	~NiGeometry();

	virtual void	Render(NiRenderer* arg);
	virtual void	Unk_22(NiRenderer* arg);
	virtual void	SetGeomData(NiObject* obj);
	virtual void	Unk_24(void);		// geomData->Unk_15()
	virtual void	Unk_25(NiRenderer* arg);

	NiPropertyState*		propertyState;		// 0AC
	NiDynamicEffectState*	dynamicEffectState;	// 0B0
	NiGeometryData*			geomData;			// 0B4
	NiSkinInstance*			skinInstance;		// 0B8
	NiD3DShader*			shader;				// 0BC BSShader
};

// C0
class NiLines : public NiGeometry
{
public:
	NiLines();
	~NiLines();
};

// C0
class NiTriBasedGeom : public NiGeometry
{
public:
	NiTriBasedGeom();
	~NiTriBasedGeom();

	// use this API for SUPER MAX SPEED
	virtual void	GetTri(UInt32 idx, NiPoint3 ** a, NiPoint3 ** b, NiPoint3 ** c) = 0;
};

// C0
class NiTriShape : public NiTriBasedGeom
{
public:
	NiTriShape();
	~NiTriShape();
};

// D0
class BSScissorTriShape : public NiTriShape
{
public:
	BSScissorTriShape();
	~BSScissorTriShape();

	UInt32	unk0C0;	// 0C0
	UInt32	unk0C4;	// 0C4
	UInt32	unk0C8;	// 0C8
	UInt32	unk0CC;	// 0CC
};

// C0
class NiScreenElements : public NiTriShape
{
public:
	NiScreenElements();
	~NiScreenElements();
};

// C0
class NiScreenGeometry : public NiTriShape
{
public:
	NiScreenGeometry();
	~NiScreenGeometry();
};

// D4
class TallGrassTriShape : public NiTriShape
{
public:
	TallGrassTriShape();
	~TallGrassTriShape();

	UInt16		instanceCount;	// 0C0
	UInt8		pad0C2[2];		// 0C2
	NiSphere	pLocalBound;	// 0C4
};

// C0
class NiTriStrips : public NiTriBasedGeom
{
public:
	NiTriStrips();
	~NiTriStrips();
};

// D4
class TallGrassTriStrips : public NiTriStrips
{
public:
	TallGrassTriStrips();
	~TallGrassTriStrips();

	virtual void		SetLocalBound(float x, float y, float z, float radius);
	virtual void		SetInstanceCount(UInt16 count);
	virtual NiSphere *	GetLocalBound(void);

	UInt16		instanceCount;	// 0C0
	UInt8		pad0C2[2];		// 0C2
	NiSphere	pLocalBound;	// 0C4
};

// C0
class NiParticles : public NiGeometry
{
public:
	NiParticles();
	~NiParticles();
};

// F0
class NiParticleSystem : public NiParticles
{
public:
	NiParticleSystem();
	~NiParticleSystem();

	virtual void	Unk_26(float arg);
	virtual void	Update(float time);

	UInt8	inWorldSpace;		// 0C0
	UInt8	pad0C1[3];			// 0C1
	NiTPointerList <NiPSysModifier *>	modifiers;		// 0C4
	NiTPointerMap <NiPSysModifier *>	modifierMap;	// 0D4 - wrong, should be NiTStringPointerMap <NiPSysModifier *>
	UInt8	unk0E4;				// 0E4
	UInt8	pad0E5[3];			// 0E5
	float	systemUpdateTime;	// 0E8
	UInt8	unk0EC;				// 0EC
	UInt8	pad0ED[3];			// 0ED
};

// F8
class NiMeshParticleSystem : public NiParticleSystem
{
public:
	NiMeshParticleSystem();
	~NiMeshParticleSystem();

	float	transformUpdateTime;	// 0F0
	UInt8	unk0F4;					// 0F4
	UInt8	unk0F5[3];				// 0F5
};

// C8
class NiParticleMeshes : public NiParticles
{
public:
	NiParticleMeshes();
	~NiParticleMeshes();

	float	transformUpdateTime;	// 0C0
	UInt8	unk0C4;					// 0C4
	UInt32	unk0C5[3];				// 0C5
};

class NiAVObjectPalette : public NiObject
{
public:
	NiAVObjectPalette();
	~NiAVObjectPalette();
};

class NiDefaultAVObjectPalette : public NiAVObjectPalette
{
public:
	NiDefaultAVObjectPalette();
	~NiDefaultAVObjectPalette();

	NiTPointerMap <NiAVObject>	avObjectMap;	// 008
	UInt32						unk018;			// 018
	NiNode*						niNode;			// 01C
};

// 220
class ShadowSceneLight : public NiRefObject
{
public:
	ShadowSceneLight();
	~ShadowSceneLight();

	UInt32										unk08[(0xD4 - 0x8) >> 2];
	float										unkD4;				// D4 - fade alpha related
	float										currentFadeAlpha;	// D8
	float										unkDC;				// DC
	float										unkE0;				// E0
	NiTPointerList<NiTriBasedGeom>				unkE4;				// E4 light receiver geometry?
	UInt8										unkF4;				// F4 shadow map rendered/casts shadow flag?
	UInt8										unkF5;				// F5 use cubemap camera when set?
	UInt8										unkF5Pad[2];
	float										unkF8;				// F8
	UInt8										unkFC;				// FC
	UInt8										unkFCPad[3];
	NiPointLight*								sourceLight;		// 100 - parent light
	UInt8										unk104;				// 104
	UInt8										unk104Pad[3];
	NiPoint3									unk108;				// 108 sourceLight->m_worldTranslate
	BSRenderedTexture*							shadowMap;			// 114 - shadow map texture
	UInt16										lightState;			// 118 when 0xFF, light source is culled (not active)
	UInt16										unk11A;				// 11A
	UInt32										unk11C;				// 11C
	UInt8										unk120;				// 120 force render shadow map?
	UInt8										pad121[3];
	NiPointer<BSCubeMapCamera>					unk124;				// 124 light camera?
	UInt32										unk128;				// 128
	UInt8										showDebug;			// 12C - debug shader toggle
	UInt8										unk12CPad[3];
	NiNode*										sourceNode;			// 130 - node being lighted/shadowed
	NiTPointerList<NiAVObject>					unk134;				// 134
	NiTPointerList<NiTriBasedGeom>::Node*		unk144;				// 144 points to the fence trishape
	NiPointer<NiTriShape>						unk148;				// 148 name set as "fence"
	NiCamera*									camera;				// 14C used when performing LOS checks/frustum culling
	BVIntersectionData							unk150;				// 150 used when updating shadow receiver geometry
	BVIntersectionData							unk1B4;				// 1B4
	UInt32										unk218;				// 218
	UInt32										unk21C;				// 21C
};
STATIC_ASSERT(offsetof(ShadowSceneLight, unkD4) == 0xD4);
STATIC_ASSERT(offsetof(ShadowSceneLight, sourceLight) == 0x100);
STATIC_ASSERT(offsetof(ShadowSceneLight, sourceNode) == 0x130);
STATIC_ASSERT(offsetof(ShadowSceneLight, unk150) == 0x150);
STATIC_ASSERT(sizeof(ShadowSceneLight) == 0x220);

// 130
class ShadowSceneNode : public NiNode
{
public:
	ShadowSceneNode();
	virtual ~ShadowSceneNode();

	NiObject*									object1;			// 0DC
	UInt16										unk0E0;				// 0E0
	UInt8										pad0E2[2];			// 0E2
	NiTPointerList <ShadowSceneLight>			lights;				// 0E4
	NiTPointerList <ShadowSceneLight>			actorShadowCasters;	// 0F4
	NiTPointerList <ShadowSceneLight>::Node*	unk104;				// 104 - used as a buffer when walking the shadowCasters list
	NiTPointerList <ShadowSceneLight>::Node*	unk108;				// 108
	NiTPointerList <ShadowSceneLight>::Node*	unk10C;				// 10C
	ShadowSceneLight*							sceneLight1;		// 110
	ShadowSceneLight*							sceneLight2;		// 114
	ShadowSceneLight*							directionalLight;	// 118
	UInt8										unk11C;				// 11C
	UInt8										pad11D[3];			// 11D
	NiObject*									object2;			// 120
	BSCubeMapCamera*							cubeMapCamera;		// 124
	UInt32										unk128;				// 128
	UInt8										unk12C;				// 12C
	UInt8										pad12D[3];			// 12D
};

//58
class BSTreeModel : public NiRefObject
{
public:
	BSTreeModel();
	virtual ~BSTreeModel();

	UInt32					Unk08;				// 08
	UInt32					Unk0C;				// 0C
	UInt32					Unk10;				// 10
	UInt32					Unk14;				// 14
	UInt32					Unk18;				// 18
	UInt32					Unk1C;				// 1C
	UInt32					Unk20;				// 20
	UInt32					Unk24;				// 24
	UInt32					Unk28;				// 28
	UInt32					Unk2C;				// 2C
	NiSourceTexture*		BranchesTexture;	// 30
	NiTexturingProperty*	BranchesProp;		// 34
	NiSourceTexture*		LeavesTexture;		// 38
	NiTexturingProperty*	LeavesProp;			// 3C
	UInt32					Unk40;				// 40
	UInt32					Unk44;				// 44
	UInt32					Unk48;				// 48
	UInt32					Unk4C;				// 4C
	UInt32					Unk50;				// 50
	UInt32					Unk54;				// 54

};

// F0
class BSTreeNode : public NiNode
{
public:
	BSTreeNode();
	virtual ~BSTreeNode();

	BSTreeModel*		TreeModel;		// DC
	UInt32				UnkE0;			// E0
	UInt32				UnkE4;			// E0
	UInt32				UnkE8;			// E0
	float				UnkEC;			// EC

};