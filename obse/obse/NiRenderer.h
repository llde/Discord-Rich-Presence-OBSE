#pragma once

#include "obse/NiNodes.h"
#include "obse/NiProperties.h"
#include <d3d9.h>

class NiGeometryGroup;
class NiDX9VertexBufferManager;	// 0x100
class NiDX9IndexBufferManager;	// 0x4C
class NiDX9TextureManager;		// 0x10
class NiDX9LightManager;		// 0x240
class NiMaterialProperty;
class NiDynamicEffect;
class NiSkinPartition;
class NiTriShape;
class NiTriStrips;
class NiParticles;
class NiLines;
class NiDX9RenderedCubeMapData;
class NiDX9DynamicTextureData;
class NiD3DShaderInterface;
class NiAccumulator;
class NiGeometryGroupManager;
class NiDX9AdapterDesc;
class NiDX9DeviceDesc;
class NiD3DShader;
class NiDX92DBufferData;
class NiDX9ShaderConstantManager;

// 14
class Ni2DBuffer : public NiObject
{
public:
	Ni2DBuffer();
	~Ni2DBuffer();

	UInt32				width;	// 008
	UInt32				height;	// 00C
	NiDX92DBufferData*	data;	// 010
};

class NiDepthStencilBuffer : public Ni2DBuffer
{
public:
	NiDepthStencilBuffer();
	~NiDepthStencilBuffer();

};

class NiDX92DBufferData : public NiRefObject
{
public:
	NiDX92DBufferData();
	virtual ~NiDX92DBufferData();

	virtual UInt32		GetWidth(void);												// 01
	virtual UInt32		GetHeight(void);											// 02
	virtual void		*GetVar10(void);											// 03
	virtual NiRTTI		*GetRTTI(void);												// 04
	virtual	UInt32		func05(void);												// 05
	virtual	UInt32		func06(void);												// 06
	virtual UInt32		func07(void);												// 07
	virtual UInt32		func08(void);												// 08
	virtual UInt32		func09(void);												// 09
	virtual UInt32		func0A(void);												// 0A
	virtual bool		ReleaseSurface1(void);										// 0B
	virtual bool		GetBufferData(IDirect3DDevice9 *D3DDevice);					// 0C
	virtual bool		SetRenderTarget(IDirect3DDevice9 *D3DDevice, UInt32 index);	// 0D
	virtual bool		SetDepthTarget(IDirect3DDevice9 *D3DDevice);				// 0E
	virtual void		ReleaseSurface2(void);										// 0F

	Ni2DBuffer*				ParentData;												// 08
	IDirect3DSurface9*		Surface;												// 0C
};

class NiDX9ImplicitBufferData : public NiDX92DBufferData
{
public:
	NiDX9ImplicitBufferData();
	~NiDX9ImplicitBufferData();

	NiPixelFormat*			PixelFormat;											// 10
	D3DPRESENT_PARAMETERS	PresentParams;											// 14
	IDirect3DDevice9*		Device;													// 4C
};

class NiDX9ImplicitDepthStencilBufferData : public NiDX92DBufferData
{
public:
	NiDX9ImplicitDepthStencilBufferData();
	~NiDX9ImplicitDepthStencilBufferData();


};

class NiDX9TextureBufferData : public NiDX92DBufferData
{
public:
	NiDX9TextureBufferData();
	~NiDX9TextureBufferData();


};

// 24
class NiRenderTargetGroup : public NiObject
{
public:
	NiRenderTargetGroup();
	~NiRenderTargetGroup();

	virtual UInt32					GetWidth(UInt32 Index);											// 13
	virtual UInt32					GetHeight(UInt32 Index);										// 14
	virtual UInt32					GetDepthStencilWidth();											// 15
	virtual UInt32					GetDepthStencilHeight();										// 16
	virtual const NiPixelFormat*	GetPixelFormat(UInt32 Index);									// 17
	virtual const NiPixelFormat*	GetDepthStencilPixelFormat();									// 18
	virtual UInt32					GetBufferCount(void);											// 19
	virtual bool					AttachBuffer(Ni2DBuffer* Buffer, UInt32 Index);					// 1A
	virtual bool					AttachDepthStencilBuffer(NiDepthStencilBuffer* DepthBuffer);	// 1B
	virtual Ni2DBuffer*				GetBuffer(UInt32 Index);										// 1C
	virtual NiDepthStencilBuffer*	GetDepthStencilBuffer();										// 1D
	virtual void*					GetRendererData();												// 1E
	virtual void					SetRendererData(void* RendererData);							// 1F
	virtual void*					GetRenderTargetData(UInt32 RenderTargetIndex);					// 20
	virtual void*					GetDepthStencilBufferRendererData();							// 21

	Ni2DBuffer*						RenderTargets[4];			// 08
	UInt32							numRenderTargets;			// 18
	NiDepthStencilBuffer*			DepthStencilBuffer;			// 1C
	void*							RenderData;					// 20

};

class NiDX9RenderState : public NiRefObject
{
public:
	NiDX9RenderState();
	virtual ~NiDX9RenderState();

	virtual void					SetAlpha(void *AlphaProperty);					// 02
	virtual void					SetDither(void *DitherProperty);				// 03
	virtual void					SetFog(NiFogProperty *FogProperty);				// 04
	virtual void					SetMaterial(void *u1);							// 05
	virtual void					SetShadeMode(void *ShadeProperty);				// 06
	virtual void					SetSpecular(void *SpecularProperty);			// 07
	virtual void					SetStencil(void *StencilProperty);				// 08
	virtual void					SetWireframe(void *WireframeProperty);			// 09
	virtual void 					SetZBuffer(void *ZBufferProperty);				// 0A
	virtual void					RestoreAlpha(void);								// 0B
	virtual void					SetVertexBlending(UInt16 u1);					// 0C
	virtual void					SetNormalization(void *u1);						// 0D
	virtual void					func_0E(UInt32 u1);								// 0E - doesn't do anything
	virtual float					GetVar088(void);								// 0F
	virtual void					func_10(float u1);								// 10
	virtual void					func_11(float u1, float u2);					// 11
	virtual BOOL					func_12(void);									// 12
	virtual void					func_13(BOOL u1);								// 13
	virtual void					func_14(UInt32 u1);								// 14
	virtual void					InitializeRenderStates(void);					// 15
	virtual void					BackUpAllStates(void);							// 16
	virtual void 					RestoreRenderState(D3DRENDERSTATETYPE state);	// 17
	virtual void					RestoreAllRenderStates(void);					// 18
	virtual void 					SetRenderState(D3DRENDERSTATETYPE state, UInt32 value, UInt8 BackUp);	// 19
	virtual UInt32					GetRenderState(D3DRENDERSTATETYPE state);		// 1A
	virtual void					ClearPixelShaders(void);						// 1B
	virtual void					BackUpPixelShader(void);						// 1C
	virtual void					func_1D(void);									// 1D
	virtual void					SetPixelShader(IDirect3DPixelShader9* PixelShader, UInt8 BackUp);		// 1E
	virtual IDirect3DPixelShader9*	GetPixelShader();								// 1F
	virtual	void					RestorePixelShader(void);						// 20
	virtual void					RemovePixelShader(IDirect3DPixelShader9* PixelShader);					// 21
	virtual void					SetVertexShader(IDirect3DVertexShader9* pShader, UInt8 BackUp);			// 22 = 0
	virtual IDirect3DVertexShader9*	GetVertexShader();								// 23 = 0
	virtual void					RestoreVertexShader(void);						// 24 = 0
	virtual void					RemoveVertexShader(IDirect3DVertexShader9* pShader);						// 25 = 0
	virtual void					SetFVF(UInt32 FVF, UInt8 BackUp);					// 26 = 0
	virtual UInt32					GetFVF(void);									// 27 = 0
	virtual void					RestoreFVF(void);								// 28 = 0
	virtual void					RemoveFVF(UInt32 FVF);							// 29 = 0
	virtual void					SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl, UInt8 BackUp);	// 2A = 0
	virtual IDirect3DVertexDeclaration9*	GetVertexDeclaration(void);				// 2B = 0
	virtual void					RestoreVertexDeclaration(void);					// 2C = 0
	virtual void					RemoveVertexDeclatation(IDirect3DVertexDeclaration9* pDecl);			// 2D = 0
	virtual void					InitTextureStageState();						// 2E
	virtual void					func_2F(void);									// 2F
	virtual void					func_30(UInt32 u1, UInt32 u2);					// 30
	virtual void					func_31(void);									// 31
	virtual void 					SetTextureStageState(UInt32 Stage, D3DTEXTURESTAGESTATETYPE Type, UInt32 Value, UInt8 BackUp);	// 32
	virtual UInt32					func_33(UInt32 u1, UInt32 u2);					// 33
	virtual void					SetSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type, UInt32 Value, UInt8 BackUp);				// 34 = 0
	virtual UInt32 					GetSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type);				// 35 = 0
	virtual void					RestoreSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type);			// 36 = 0
	virtual void					ClearTextureList(void);							// 37
	virtual void					SetTexture(UInt32 Sampler, IDirect3DBaseTexture9* pTexture);	// 38
	virtual IDirect3DBaseTexture9*	GetTexture(UInt32 Sampler);						// 39
	virtual void					RemoveTexture(IDirect3DBaseTexture9* pTexture);	// 3A
	virtual void					SetSoftwareVertexProcessing(UInt8 bSoftware);	// 3B
	virtual UInt8					GetSoftwareVertexProcessing(void);				// 3C
	virtual void					SetVar_0FF4(UInt8 u1);							// 3D
	virtual UInt8					GetVar_0FF4(void);								// 3E
	virtual void					SetVar_0FF5(UInt8 u1);							// 3F
	virtual UInt8					GetVar_0FF5(void);								// 40
	virtual void					Reset(void);									// 41
	virtual void					func_042(void);									// 42 = 0
	
	struct NiRenderStateSetting
	{
		UInt32 CurrentValue;
		UInt32 PreviousValue;
	};
	
	UInt32							Flags;							// 0008
	UInt32							unk000C[(0x0074 - 0x000C) >> 2];// 000C
	NiAlphaProperty*				DisabledAlphaProperty;			// 0074
	float							CameraNear;						// 0078
	float							CameraFar;						// 007C
	float							CameraDepthRange;				// 0080
	float							MaxFogFactor;					// 0084
	float							MaxFogValue;					// 0088
	NiColor							CurrentFogColor;				// 008C
	UInt32							unk0098[(0x0120 - 0x0098) >> 2];// 0098
	NiRenderStateSetting			RenderStateSettings[256];		// 0120
	NiRenderStateSetting			TextureStageStateSettings[128];	// 0920
	NiRenderStateSetting			SamplerStateSettings[80];		// 0D20
	UInt32							unk0FA0[(0x0FF0 - 0x0FA0) >> 2];// 0FA0
	NiDX9ShaderConstantManager*		ShaderConstantManager;			// 0FF0
	UInt8							ForceNormalizeNormals;			// 0FF4
	UInt8							InternalNormalizeNormals;		// 0FF5
	UInt8							UsingSoftwareVP;				// 0FF6
	UInt8							Declaration;					// 0FF7
	IDirect3DDevice9*				Device;							// 0FF8
	NiDX9Renderer*					Renderer;						// 0FFC
	UInt32							unk1000[(0x1018 - 0x1000) >> 2];// 1000
	D3DCAPS9						Caps;							// 1018
};

STATIC_ASSERT(sizeof(NiDX9RenderState) == 0x1148);

// 210
class NiRenderer : public NiObject
{
public:
	NiRenderer();
	~NiRenderer();
	
	enum ClearFlags
	{
		kClear_BACKBUFFER = 0x1,
		kClear_STENCIL = 0x2,
		kClear_ZBUFFER = 0x4,
		kClear_NONE = 0,
		kClear_ALL = kClear_BACKBUFFER | kClear_STENCIL | kClear_ZBUFFER
	};

	enum
	{
		kCopyFilterMode_None = 0,
		kCopyFilterMode_Point,
		kCopyFilterMode_Linear,
	};

	virtual bool			Unk_13(UInt32 arg);
	virtual const char *	GetRendererDesc(void) = 0;	// builds a string "<adapter id> (<dev type>-<dev flags>)"
	virtual UInt32			GetFlags(void) = 0;
	virtual void			SetClearDepth(float arg) = 0;
	virtual float			GetClearDepth(void) = 0;
	virtual void			SetClearColor4(float * src) = 0;
	virtual void			SetClearColor3(float * src) = 0;
	virtual void			GetClearColor(float * dst) = 0;
	virtual void			SetClearStencil(UInt32 src) = 0;
	virtual UInt32			GetClearStencil(void) = 0;
	virtual bool			Unk_1D(NiRenderTargetGroup * renderTarget) = 0;
	virtual void			Unk_1E(void) = 0;
	virtual NiRenderTargetGroup *	GetDefaultRT(void) = 0;	// get back buffer rt
	virtual NiRenderTargetGroup *	GetCurrentRT(void) = 0;	// get currentRTGroup
	virtual UInt8			Unk_21(void) = 0;			// get unkA90 (u8)
	virtual void			Unk_22(void) = 0;			// passthrough to currentRTGroup2->Fn1D
	virtual void			Unk_23(void) = 0;			// currentRTGroup2->Fn1C(0);
	virtual void			Unk_24(UInt32 arg) = 0;		// sub_773960(arg, &unk6F4);
	virtual void *			Unk_25(void * arg0, UInt32 arg1, UInt32 arg2) = 0;	// something with pixel format conversion
	virtual void *			Unk_26(void * arg);			// Unk_25(arg, 0x20, 8) - something with pixel format conversion
	virtual UInt32			Unk_27(void) = 0;			// get unk894 - current/max num render targets?
	virtual UInt32			Unk_28(void) = 0;			// get unk898
	virtual void			Unk_29(void) = 0;			// passthrough to textureManager->Fn03
	virtual bool			Unk_2A(UInt32 arg) = 0;		// set dword_B42050, return true
	virtual UInt32			Unk_2B(void) = 0;			// get dword_B42050
	virtual bool			Unk_2C(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3);
	virtual void			PurgeGeometry(NiGeometryData * geo) = 0;
	virtual void			PurgeMaterial(NiMaterialProperty * material) = 0;			// passthrough to renderState->DeleteMaterial
	virtual void			PurgeEffect(NiDynamicEffect * effect) = 0;
	virtual void			PurgeScreenTexture(void) = 0;
	virtual void			PurgeSkinPartition(NiSkinPartition * skinPartition) = 0;
	virtual void			PurgeSkinInstance(NiSkinInstance * skinInstance) = 0;
	virtual void			Unk_33(void) = 0;
	virtual bool			Unk_34(void) = 0;
	virtual void			Unk_35(void) = 0;
	virtual bool			FastCopy(void * src, void * dst, RECT * srcRect, SInt32 xOffset, SInt32 yOffset) = 0;
	virtual bool			Copy(void * src, void * dst, RECT * srcRect, RECT * dstRect, UInt32 filterMode) = 0;
	virtual void			Unk_38(void) = 0;			// passthrough to renderState->Fn12
	virtual bool			Unk_39(void * arg) = 0;		// renderState->Fn13(arg);
	virtual void			Unk_3A(void) = 0;			// passthrough to renderState->Fn0F
	virtual void			Unk_3B(float arg) = 0;		// renderState->Fn10(arg);
	virtual void *			Unk_3C(UInt32 size) = 0;	// FormHeap_Allocate(size);
	virtual void			Unk_3D(UInt32 arg0, UInt32 arg1) = 0;
	virtual void			Unk_3E(UInt32 arg0, UInt32 arg1) = 0;
	virtual void			Unk_3F(void * buf) = 0;		// FormHeap_Free(buf);
	virtual bool			Unk_40(UInt32 arg0);
	virtual void			CreateSourceTexture(NiSourceTexture * texture) = 0;			// locks cs2
	virtual bool			CreateRenderedTexture(NiRenderedTexture * arg) = 0;	// make a NiDX9RenderedTextureData, store it in unk8C0
	virtual bool			CreateSourceCubeMap(NiSourceCubeMap * arg) = 0;				// make a NiDX9SourceCubeMapData
	virtual bool			CreateRenderedCubeMap(NiRenderedCubeMap * arg) = 0;	// make a NiDX9RenderedCubeMapData, store it in unk8D0
	virtual bool			CreateDynamicTexture(void * arg) = 0;					// make a NiDX9DynamicTextureData, store it in unk8E0
	virtual void			CreatePalette(void * arg) = 0;
	virtual bool			CreateDepthStencil(NiDepthStencilBuffer * arg, void * textureFormat) = 0;
	virtual void			LockDynamicTexture(void * arg0, void * arg1) = 0;
	virtual void			UnLockDynamicTexture(void * arg) = 0;
	virtual void			LockRenderer(void);
	virtual void			UnlockRenderer(void);
	virtual bool			BeginScene(void) = 0;
	virtual bool			EndScene(void) = 0;
	virtual void			DisplayScene(void) = 0;
	virtual void			Clear(float * rect, UInt32 flags) = 0;
	virtual void			SetupCamera(NiPoint3 * pos, NiPoint3 * at, NiPoint3 * up, NiPoint3 * right, NiFrustum * frustum, float * viewport) = 0;
	virtual void			SetupScreenSpaceCamera(float* viewport);
	virtual bool			BeginUsingRenderTargetGroup(NiRenderTargetGroup* renderTarget, ClearFlags clearFlags);
	virtual bool			EndUsingRenderTargetGroup(void) = 0;
	virtual void			BeginBatch(UInt32 arg0, UInt32 arg1);	// set unk61C, unk620
	virtual void			EndBatch(void);
	virtual void			BatchRenderShape(void * arg);
	virtual void			BatchRenderStrips(void * arg);
	virtual void			RenderTriShape(NiTriShape * obj) = 0;
	virtual void			RenderTriStrips(NiTriStrips * obj) = 0;		// points to the same function as above
	virtual void			RenderTriShape2(NiTriShape * obj) = 0;		// calls DrawIndexedPrimitive
	virtual void			RenderTriStrips2(NiTriStrips * obj) = 0;	// calls DIP/DP
	virtual void			RenderParticles(NiParticles * obj) = 0;
	virtual void			RenderLines(NiLines * obj) = 0;				// calls DIP/SetStreamSource/SetIndices
	virtual void			RenderScreenTexture(void) = 0;

	// 080
	struct CriticalSection
	{
		CRITICAL_SECTION	cs;	// 000
		UInt32	pad018[(0x078 - 0x018) >> 2];	// 018
		UInt32	curThread;	// 078
		UInt32	entryCount;	// 07C
	};

	NiAccumulator			* accumulator;			// 008 BSShaderAccumulator
	NiPropertyState			* propertyState;		// 00C
	NiDynamicEffectState	* dynamicEffectState;	// 010
	UInt32				pad014[(0x080 - 0x014) >> 2];	// 00C
	CriticalSection		RendererLock;				// 080
	CriticalSection		PrecacheCriticalSection;	// 100
	CriticalSection		SourceDataCriticalSection;	// 180
	UInt32				unk200;			// 200
	UInt32				unk204;			// 204
	UInt32				unk208;			// 208
	UInt8				unk20C;			// 20C
	UInt8				unk20D;			// 20D
	UInt8				pad20E[2];		// 20E
};

class LightEntry
{
public:
	LightEntry();
	~LightEntry();
};

// 10
class NiDX9LightManager : public NiTPointerMap <LightEntry *>
{
public:
	NiDX9LightManager();
	~NiDX9LightManager();
};

// B00
class NiDX9Renderer : public NiRenderer
{
public:
	NiDX9Renderer();
	~NiDX9Renderer();

	class PrePackObject;

	// 58
	class Unk6F4
	{
	public:
		UInt32	unk00;
		UInt32	unk04;
		UInt32	unk08;
		UInt32	unk0C;
		UInt32	unk10;
		UInt32	unk14;
		UInt32	unk18;
		UInt32	unk1C;
		UInt32	unk20;
		UInt32	unk24;
		UInt32	unk28;
		UInt32	unk2C;
		UInt32	unk30;
		UInt32	unk34;
		UInt32	unk38;
		UInt32	unk3C;
		UInt32	unk40;
		UInt32	unk44;
		UInt32	unk48;
		UInt32	unk4C;
		UInt32	unk50;
		UInt32	unk54;
	};

	enum FrameBufferFormat
	{
		FBFMT_UNKNOWN = 0,
		FBFMT_R8G8B8,
		FBFMT_A8R8G8B8,
		FBFMT_X8R8G8B8,
		FBFMT_R5G6B5,
		FBFMT_X1R5G5B5,
		FBFMT_A1R5G5B5,
		FBFMT_A4R4G4B4,
		FBFMT_R3G3B2,
		FBFMT_A8,
		FBFMT_A8R3G3B2,
		FBFMT_X4R4G4B4,
		FBFMT_R16F,
		FBFMT_G16R16F,
		FBFMT_A16B16G16R16F,
		FBFMT_R32F,
		FBFMT_G32R32F,
		FBFMT_A32B32G32R32F,
		FBFMT_NUM
	};

	enum DepthStencilFormat
	{
		DSFMT_UNKNOWN      =   0,
		DSFMT_D16_LOCKABLE =  70,
		DSFMT_D32          =  71,
		DSFMT_D15S1        =  73,
		DSFMT_D24S8        =  75,
		DSFMT_D16          =  80,
		DSFMT_D24X8        =  77,
		DSFMT_D24X4S4      =  79,
	};

	enum PresentationInterval
	{
		PRESENT_INTERVAL_IMMEDIATE = 0,
		PRESENT_INTERVAL_ONE = 1,
		PRESENT_INTERVAL_TWO = 2,
		PRESENT_INTERVAL_THREE = 3,
		PRESENT_INTERVAL_FOUR = 4,
		PRESENT_INTERVAL_NUM
	};

	enum SwapEffect
	{
		SWAPEFFECT_DEFAULT,
		SWAPEFFECT_DISCARD,
		SWAPEFFECT_FLIP,
		SWAPEFFECT_COPY,
		SWAPEFFECT_NUM
	};

	enum FrameBufferMode
	{
		FBMODE_DEFAULT,
		FBMODE_LOCKABLE,
		FBMODE_MULTISAMPLES_2           = 0x00010000,
		FBMODE_MULTISAMPLES_3           = 0x00020000,
		FBMODE_MULTISAMPLES_4           = 0x00030000,
		FBMODE_MULTISAMPLES_5           = 0x00040000,
		FBMODE_MULTISAMPLES_6           = 0x00050000,
		FBMODE_MULTISAMPLES_7           = 0x00060000,
		FBMODE_MULTISAMPLES_8           = 0x00070000,
		FBMODE_MULTISAMPLES_9           = 0x00080000,
		FBMODE_MULTISAMPLES_10          = 0x00090000,
		FBMODE_MULTISAMPLES_11          = 0x000a0000,
		FBMODE_MULTISAMPLES_12          = 0x000b0000,
		FBMODE_MULTISAMPLES_13          = 0x000c0000,
		FBMODE_MULTISAMPLES_14          = 0x000d0000,
		FBMODE_MULTISAMPLES_15          = 0x000e0000,
		FBMODE_MULTISAMPLES_16          = 0x000f0000,
		FBMODE_MULTISAMPLES_NONMASKABLE = 0x80000000,
		FBMODE_QUALITY_MASK             = 0x0000FFFF,
		FBMODE_NUM = 18
	};

	enum RefreshRate
	{
		REFRESHRATE_DEFAULT   = 0
	};

	typedef bool (* CallbackA98)(bool arg0, void * arg1);
	typedef bool (* LostDeviceCallback)(void * arg);

	// these may be wrong
	virtual bool			DeleteRenderedCubeMap(NiRenderedCubeMap * arg);	// unk8D0 - may also handle textures?
	virtual bool			DeleteTexture(NiTexture * arg);					// unk8C0
	virtual bool			DeleteDynamicTexture(UInt32 arg);				// unk8E0

	// 210
	UInt32					pad210[(0x280 - 0x210) >> 2];	// 210
	IDirect3DDevice9*		device;						// 280
	D3DCAPS9				caps;						// 284
	HANDLE					deviceWindow;				// 3B4
	HANDLE					focusWindow;				// 3B8
	char					rendererInfo[0x200];		// 3BC
	UInt32					adapterIdx;					// 5BC
	UInt32					d3dDevType;					// 5C0 - D3DDEVTYPE
	UInt32					d3dDevFlags;				// 5C4 - D3DCREATE
	UInt8					softwareVertexProcessing;	// 5C8 - !D3DCREATE_HARDWARE_VERTEXPROCESSING
	UInt8					mixedVertexProcessing;		// 5C9 - D3DCREATE_MIXED_VERTEXPROCESSING
	UInt8					pad5CA[2];					// 5CA
	NiDX9AdapterDesc*		adapterDesc;				// 5CC
	NiDX9DeviceDesc*		deviceDesc;					// 5D0
	D3DCOLOR				clearColor;					// 5D4
	float					clearDepth;					// 5D8
	UInt32					clearStencil;				// 5DC
	UInt32					rendFlags;					// 5E0
	char					behavior[32];				// 5E4
	NiTPointerMap <PrePackObject *>	PrePackObjects;		// 604 - NiTPointerMap <NiVBBlock *, NiDX9Renderer::PrePackObject *>
	UInt32					pad624[(0x648 - 0x614) >> 2];	// 614
	NiPoint3				camRight;						// 648
	NiPoint3				camUp;							// 654
	NiPoint3				modelCamRight;					// 660
	NiPoint3				modelCamUp;						// 66C
	float					NearDepth;						// 678
	float					DepthRange;						// 67C
	D3DMATRIX				identityMatrix;					// 680
	D3DVIEWPORT9			viewport;						// 6C0
	UInt32					HWBones;						// 6D8
	UInt32					MaxStreams;						// 6DC
	UInt32					Unk6E0;							// 6E0
	UInt32					Unk6E4;							// 6E4
	UInt8					pad6E8;							// 6E8
	UInt8					unk6E9;							// 6E9
	UInt8					pad6EA[2];						// 6EA
	UInt32					ResetCounter;					// 6EC
	UInt8					lostDevice;						// 6F0 - disables drawing
	UInt8					pad6F1[3];						// 6F1
	Unk6F4					unk6F4[4];						// 6F4
	UInt32					DefaultTextureFormat[4];		// 854 //NiPixelFormat*
	NiPixelData*			DefaultTextureData[4];			// 864
	UInt32					unk874;							// 874 - init'd to 0x16
	NiRenderTargetGroup*	defaultRTGroup;					// 878 - probably back buffer
	NiRenderTargetGroup*	currentRTGroup;					// 87C
	NiRenderTargetGroup*	currentscreenRTGroup;			// 880
	NiTPointerMap <NiRenderTargetGroup *>	screenRTGroups;	// 884 - NiTPointerMap <HWND *, NiPointer <NiRenderTargetGroup> >
	UInt32					unk894;							// 894
	UInt8					unk898;							// 898
	UInt8					pad899[3];						// 899
	NiObject*				unk89C;							// 89C - NiPropertyState (0x30)
	NiGeometryGroupManager* geometryGroupMgr;				// 8A0
	NiGeometryGroup*		unk8A4;				// 8A4 - NiUnsharedGeometryGroup
	NiGeometryGroup*		unk8A8;				// 8A8 - NiDynamicGeometryGroup
	NiDX9RenderState*		renderState;		// 8AC
	NiDX9VertexBufferManager* vertexBufferMgr;	// 8B0
	NiDX9IndexBufferManager* indexBufferMgr;	// 8B4
	NiDX9TextureManager*	textureMgr;			// 8B8
	NiDX9LightManager*		lightMgr;			// 8BC
	NiTPointerMap <NiDX9RenderedTextureData *>	renderedTextures;	// 8C0 - NiTPointerMap <NiRenderedTexture *, NiDX9RenderedTextureData *>
	NiTPointerMap <NiDX9RenderedCubeMapData *>	renderedCubeMaps;	// 8D0 - NiTPointerMap <NiRenderedCubeMap *, NiDX9RenderedCubeMapData *>
	NiTPointerMap <NiDX9DynamicTextureData *>	dynamicTextures;	// 8E0 - NiTPointerMap <NiDynamicTexture *, NiDX9DynamicTextureData *>
	NiTexture*				ClipperImage;							// 8F0
	NiTPointerList <NiDX92DBufferData *>	atDisplayFrame;	// 8F4
	NiTPointerList <NiD3DShaderInterface *>	shaderInterfaces;	// 904
	UInt32					Unk914;		// 914
	UInt32					Unk918;		// 918
	UInt32					Unk91C;		// 91C
	UInt32					Unk920;		// 920
	UInt32					Unk924;		// 924
	UInt32					Unk928;		// 928
	UInt32					Unk92C;		// 92C
	UInt32					Unk930;		// 930
	UInt32					Unk934;		// 934
	UInt32					Unk938;		// 938
	UInt32					Unk93C;		// 93C
	D3DXMATRIXA16			worldMatrix;				// 940
	D3DMATRIX				viewMatrix;					// 980
	D3DMATRIX				projMatrix;					// 9C0
	D3DMATRIX				invViewMatrix;				// A00
	UInt32					ScreenTextureVerts;			// A40 NiPoint2*
	NiColorAlpha*			ScreenTextureColors;		// A44
	UInt32					ScreenTextureTexCoords;		// A48 NiPoint2*
	UInt16					unkA4C;						// A4C
	UInt8					unkA4E[2];					// A4E
	UInt16*					ScreenTextureIndices;		// A50
	UInt32					NumScreenTextureIndices;	// A54
	UInt32					width;						// A58
	UInt32					height;						// A5C
	UInt32					flags;						// A60
	UInt32					windowDevice;				// A64
	UInt32					windowFocus;				// A68
	UInt32					adapterType;				// A6C
	UInt32					deviceType;					// A70
	FrameBufferFormat		frameBufferFormat;			// A74
	DepthStencilFormat		depthStencilFormat;			// A78
	PresentationInterval	presentationInterval;		// A7C
	SwapEffect				swapEffect;					// A80
	FrameBufferMode			frameBufferMode;			// A84
	UInt32					backBufferCount;			// A88
	RefreshRate				refreshRate;				// A8C
	UInt8					unkA90;						// A90
	UInt8					padA91[3];					// A91
	NiD3DShader*			defaultShader;				// A94
	NiTArray <CallbackA98>	unkA98;						// A98
	NiTArray <void *>		unkAA8;						// AA8
	NiTArray <LostDeviceCallback>	lostDeviceCallbacks;	// AB8
	NiTArray <void *>		lostDeviceCallbacksRefcons;	// AC8
	UInt32					padAD0[(0xB00 - 0xAD8) >> 2];	// AD8
};

STATIC_ASSERT(offsetof(NiDX9Renderer, viewport) == 0x6C0);
STATIC_ASSERT(offsetof(NiDX9Renderer, height) == 0xA5C);
STATIC_ASSERT(sizeof(NiDX9Renderer) == 0xB00);
STATIC_ASSERT(offsetof(D3DCAPS9, MaxTextureBlendStages) == 0x94);

extern NiRenderer **	g_renderer;

// 0C
class NiAccumulator : public NiObject
{
public:
	NiAccumulator();
	~NiAccumulator();

	virtual void	Start(NiCamera* camera);
	virtual void	Stop();
	virtual void	Add(NiVisibleArray* list);
	virtual bool	Fn_16(void);

	NiCamera*		camera;	// 08
};

// 34
class NiBackToFrontAccumulator : public NiAccumulator
{
public:
	NiBackToFrontAccumulator();
	~NiBackToFrontAccumulator();

	virtual void	Fn_17(void);

	UInt32			pad0C[(0x20 - 0x0C) >> 2];	// 0C
	UInt32			NumItems;	// 20
	UInt32			MaxItems;	// 24
	NiGeometry**	Items;		// 28
	float*			Depths;		// 2C
	UInt32			CurrItem;	// 30

};

// 38
class NiAlphaAccumulator : public NiBackToFrontAccumulator
{
public:
	NiAlphaAccumulator();
	~NiAlphaAccumulator();

	UInt8	unk34;		// 34
	UInt8	unk35;		// 35
	UInt8	pad36[2];	// 36
};

STATIC_ASSERT(sizeof(NiAlphaAccumulator) == 0x38);

// ??
class ReferenceVolume
{
public:
	ReferenceVolume();
	~ReferenceVolume();
};

// 226C - yes, really
class BSShaderAccumulator : public NiAlphaAccumulator
{
public:
	BSShaderAccumulator();
	~BSShaderAccumulator();

	// ?
	class GeometryGroup
	{
	public:
		GeometryGroup();
		~GeometryGroup();
	};

	// ?
	class ImmediateGeometryGroup
	{
	public:
		ImmediateGeometryGroup();
		~ImmediateGeometryGroup();
	};

	// ?
	class ShadowVolumeRPList
	{
	public:
		ShadowVolumeRPList();
		~ShadowVolumeRPList();
	};

	// 14
	struct Unk00C8
	{
		UInt32	unk00;		// 00
		UInt8	unk04;		// 04
		UInt8	pad05[3];	// 05
		float	unk08;		// 08
		UInt32	unk0C;		// 0C
		UInt32	unk10;		// 10
	};

	virtual void	Fn_18(void);

	UInt32												pad0038[(0x003C - 0x0038) >> 2];		// 0038
	NiTPointerList <GeometryGroup *>					geoGroups;							// 003C
	NiTPointerList <GeometryGroup *>					geoGroups2;							// 004C
	UInt32												unk005C;							// 005C
	UInt32												unk0060;							// 0060
	NiTPointerList <ImmediateGeometryGroup *>			immGeoGroups;						// 0064
	UInt32												unk0074;							// 0074
	UInt32												unk0078;							// 0078
	BSTPersistentList <BSShaderProperty::RenderPass *>	unk007C;							// 007C
	BSTPersistentList <BSShaderProperty::RenderPass *>	unk0090;							// 0090
	BSTPersistentList <BSShaderProperty::RenderPass *>	unk00A4;							// 00A4
	UInt32												pad00B8[(0x00BC - 0x00B8) >> 2];		// 00B8
	UInt32												unk00BC;							// 00BC
	UInt8												unk00C0;							// 00C0
	UInt8												pad00C1[3];							// 00C1
	float												unk00C4;							// 00C4 - init'd to 1
	Unk00C8												unk00C8[3];							// 00C8
	BSTPersistentList <BSShaderProperty::RenderPass *>	unk0104[419];						// 0104 - no, really
	UInt32												pad21C0[(0x21D0 - 0x21C0) >> 2];		// 21C0
	NiTPointerList <ShadowVolumeRPList *>				unk21D0;							// 21D0
	UInt8												unk21E0;							// 21E0
	UInt8												unk21E1;							// 21E1
	UInt8												unk21E2;							// 21E2
	UInt8												unk21E3;							// 21E3
	UInt16												unk21E4;							// 21E4
	UInt16												unk21E6;							// 21E6
	void*												unk21E8;							// 21E8 - C8 byte buffer
	BSTPersistentList <NiGeometry *>					unk21EC;							// 21EC
	BSTPersistentList <NiGeometry *>					unk2200;							// 2200
	BSTPersistentList <NiGeometry *>					unk2214;							// 2214
	UInt32												unk2228;							// 2228
	NiTPointerList <ReferenceVolume *>					unk222C;							// 222C
	UInt32												unk223C;							// 223C
	UInt8												unk2240;							// 2240
	UInt8												pad2241[3];							// 2241
	NiTPointerList <NiGeometry *>						unk2244;							// 2244
	NiTPointerList <NiGeometry *>						unk2254;							// 2254
	void*												unk2264;							// 2264 - 4 byte buffer
	UInt8												unk2268;							// 2268
	UInt8												pad2269[3];							// 2269
};

STATIC_ASSERT(offsetof(BSShaderAccumulator, unk0060) == 0x0060);
STATIC_ASSERT(offsetof(BSShaderAccumulator, unk007C) == 0x007C);
STATIC_ASSERT(offsetof(BSShaderAccumulator, unk00BC) == 0x00BC);
STATIC_ASSERT(offsetof(BSShaderAccumulator, unk00C8) == 0x00C8);
STATIC_ASSERT(offsetof(BSShaderAccumulator, unk0104) == 0x0104);
STATIC_ASSERT(sizeof(BSShaderAccumulator) == 0x226C);
