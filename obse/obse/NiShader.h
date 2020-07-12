#include "obse/NiRenderer.h"
#include "obse/NiTypes.h"

class NiD3DRenderStateGroup;
class NiD3DShaderProgram;

class NiD3DTextureStage
{
public:
	NiD3DTextureStage();
	~NiD3DTextureStage();

	UInt32				Stage;				// 00
	NiTexture*			Texture;			// 04
	UInt32				Unk08;				// 08

};

class NiD3DShaderConstantMapEntry
{
public:
	NiD3DShaderConstantMapEntry();
	virtual ~NiD3DShaderConstantMapEntry();

	UInt32				Unk04;				// 04
	UInt32				Unk08;				// 08
	char*				Key;				// 0C
	UInt32				Internal;			// 10
	UInt32				Flags;				// 14
	UInt32				Extra;				// 18
	UInt32				ShaderRegister;		// 1C
	UInt32				RegisterCount;		// 20
	char*				VariableName;		// 24
	UInt32				DataSize;			// 28
	UInt32				DataStride;			// 2C
	void*				DataSource;			// 30
	UInt8				Unk34;				// 34
	UInt8				pad34[3];

};

class NiD3DShaderConstantMap
{
public:
	NiD3DShaderConstantMap();
	virtual ~NiD3DShaderConstantMap();
	
	UInt32									Unk04;				// 04
	UInt32									Unk08;				// 08
	NiTArray<NiD3DShaderConstantMapEntry*>	Entries;			// 0C
	UInt8									Modified;			// 1C
	UInt8									pad1C[3];
	NiD3DShaderProgram*						LastShaderProgram;	// 20
	UInt32									Unk24;				// 24
	IDirect3DDevice9*						Device;				// 28
	NiDX9Renderer*							Renderer;			// 2C
	NiDX9RenderState*						RenderState;		// 30

};

class NiD3DSCMVertex : public NiD3DShaderConstantMap
{
public:
	NiD3DSCMVertex();
	virtual ~NiD3DSCMVertex();

	// no data

};

class NiD3DSCMPixel : public NiD3DShaderConstantMap
{
public:
	NiD3DSCMPixel();
	virtual ~NiD3DSCMPixel();

	// no data

};

class NiD3DShaderDeclaration : public NiObject
{
public:
	NiD3DShaderDeclaration();
	virtual ~NiD3DShaderDeclaration();

	NiDX9Renderer*				Renderer;		// 008
	NiDX9VertexBufferManager*	BufferManager;	// 00C
	IDirect3DDevice9*			Device;			// 010
	UInt32						Unk014;			// 014
	UInt32						Unk018;			// 018
	UInt32						Unk01C;			// 01C
	UInt32						StreamCount;	// 020
	UInt32						Unk024;			// 024
	UInt32						Unk028;			// 028

};

class NiDX9ShaderDeclaration : public NiD3DShaderDeclaration
{
public:
	NiDX9ShaderDeclaration();
	virtual ~NiDX9ShaderDeclaration();

	UInt32		Unk02C;			// 02C
	UInt32		Unk030;			// 030
	UInt32		Unk034;			// 034

};

class NiD3DShaderProgram : public NiRefObject
{
public:
	NiD3DShaderProgram(NiDX9Renderer* Renderer, UInt32 Arg2);
	virtual ~NiD3DShaderProgram();

	enum ProgramType
	{
		PROGRAM_VERTEX = 0,
		PROGRAM_PIXEL,
		PROGRAM_GEOMETRY,
		PROGRAM_MAX
	};

	ProgramType					eProgramType;		// 08
	char*						Name;				// 0C
	char*						ShaderProgramName;	// 10
	UInt32						CodeSize;			// 14
	void*						Unk018;				// 18
	IDirect3DDevice9*			Device;				// 1C
	NiDX9Renderer*				Renderer;			// 20
	NiDX9RenderState*			RenderState;		// 24

};

class NiD3DVertexShader : public NiD3DShaderProgram
{
public:
	NiD3DVertexShader(NiDX9Renderer* Renderer, UInt32 Arg2);
	virtual ~NiD3DVertexShader();

	UInt8							SoftwareVP;		// 28
	UInt8							pad[3];			// 29
	UInt32							Usage;			// 2C
	IDirect3DVertexShader9*			ShaderHandle;	// 30
	IDirect3DVertexDeclaration9*	Declaration;	// 34

};

class NiD3DPixelShader : public NiD3DShaderProgram
{
public:
	NiD3DPixelShader(NiDX9Renderer* Renderer, UInt32 Arg2);
	virtual ~NiD3DPixelShader();

	IDirect3DPixelShader9*			ShaderHandle;	// 28

};

class NiD3DPass
{
public:
	NiD3DPass();
	virtual ~NiD3DPass();

	char							Name[16];					// 04
	UInt32							CurrentStage;				// 14
	UInt32							StageCount;					// 18
	UInt32							TexturesPerPass;			// 1C
	NiTArray<NiD3DTextureStage*>	Stages;						// 20
	NiD3DRenderStateGroup*			RenderStateGroup;			// 30
	NiD3DShaderConstantMap*			PixelConstantMap;			// 34
	char*							PixelShaderProgramFile;		// 38
	char*							PixelShaderEntryPoint;		// 3C
	char*							PixelShaderTarget;			// 40
	NiD3DPixelShader*				PixelShader;				// 44
	NiD3DShaderConstantMap*			VertexConstantMap;			// 48
	char*							VertexShaderProgramFile;	// 4C
	char*							VertexShaderEntryPoint;		// 50
	char*							VertexShaderTarget;			// 54
	NiD3DVertexShader*				VertexShader;				// 58
	UInt8							SoftwareVP;					// 5C
	UInt8							RendererOwned;				// 5D
	UInt8							pad[2];
	UInt32							RefCount;					// 60

};

class NiShader : public NiRefObject
{
public:
	NiShader();
	virtual ~NiShader();

	char*		Name;					// 008
	UInt32		Unk00C;					// 00C

};

class NiD3DShaderInterface : public NiShader
{
public:
	NiD3DShaderInterface();
	virtual ~NiD3DShaderInterface();

	IDirect3DDevice9*	D3DDevice;		// 010
	NiDX9Renderer*		D3DRenderer;	// 014
	NiDX9RenderState*	D3DRenderState;	// 018
	UInt8				Unk01C;			// 01C
	UInt8				Unk01D;			// 01D
	UInt8				pad[2];

};

class NiD3DShader : public NiD3DShaderInterface
{
public:
	NiD3DShader();
	virtual ~NiD3DShader();

	UInt8					IsInitialized;		// 020
	UInt8					Unk021;				// 021
	UInt8					pad021[2];
	NiD3DShaderDeclaration*	ShaderDeclaration;	// 024
	NiD3DRenderStateGroup*	RenderStateGroup;	// 028
	NiD3DShaderConstantMap* PixelConstantMap;	// 02C
	NiD3DShaderConstantMap* VertexConstantMap;	// 030
	UInt32					CurrentPassIndex;	// 034
	UInt32					PassCount;			// 038
	NiD3DPass*				CurrentPass;		// 03C
	NiTArray<NiD3DPass*>	Passes;				// 040
	UInt32					Unk050[8];			// 050
	
};
STATIC_ASSERT(sizeof(NiD3DShader) == 0x70);

class BSShader : public NiD3DShader
{
public:
	BSShader();
	virtual ~BSShader();

	UInt32		Unk070;			// 070
	UInt32		Unk074;			// 074
	UInt32		Unk078;			// 078

};

class GeometryDecalShader : public BSShader
{
public:
	GeometryDecalShader();
	virtual ~GeometryDecalShader();

	UInt32				Unk07C[2];		// 07C
	NiD3DVertexShader*	Vertex[2];		// 084
	NiD3DPixelShader*	Pixel[2];		// 08C
	UInt32				Unk104[3];		// 094

};
STATIC_ASSERT(sizeof(GeometryDecalShader) == 0x0A0);

class WaterShader : public BSShader
{
public:
	WaterShader();
	virtual ~WaterShader();

	UInt32				Unk07C[16];		// 07C
	NiD3DVertexShader*	Vertex[2];		// 0BC
	NiD3DPixelShader*	Pixel[16];		// 0C4
	UInt32				Unk104[5];		// 104

};
STATIC_ASSERT(sizeof(WaterShader) == 0x118);

class TallGrassShader : public BSShader
{
public:
	TallGrassShader();
	virtual ~TallGrassShader();

	UInt32				Unk07C[6];			// 07C
	NiD3DVertexShader*	Vertex1[20];		// 094 Vertex1 + Vertex2 are a unique array but we split it for fast use (1 are compiled 1_1 and 2 are compiled 2_0)
	NiD3DVertexShader*	Vertex2[20];		// 0E4
	NiD3DPixelShader*	Pixel1[2];			// 134 Pixel1 + Pixel2 are a unique array but we split it for fast use (1 are compiled 1_1 and 2 are compiled 2_0)
	NiD3DPixelShader*	Pixel2[7];			// 13C
	float*				InstanceData;		// 158 Pointer to a struct of 912 floats (228 registers)
	UInt32				Unk15C;				// 15C
	UInt16				InstanceDataCount;	// 160
	UInt16				pad160;				// 162
	UInt32				Unk164[12];			// 164

};
STATIC_ASSERT(sizeof(TallGrassShader) == 0x194);

class BSImageSpaceShader : public BSShader
{
public:
	BSImageSpaceShader();
	virtual ~BSImageSpaceShader();

	UInt32		Unk07C;		// 07C
	UInt32		Unk080;		// 080
	UInt32		Unk084;		// 084
	UInt32		Unk088;		// 088
	UInt32		Unk08C;		// 08C

};

class WaterShaderDisplacement : public BSImageSpaceShader
{
public:
	WaterShaderDisplacement();
	virtual ~WaterShaderDisplacement();

	UInt32				Unk090[9];		// 090
	NiD3DVertexShader*	Vertex[8];		// 0B4
	NiD3DPixelShader*	Pixel[8];		// 0D4
	UInt32				Unk0F4[13];		// 0F4

};
STATIC_ASSERT(sizeof(WaterShaderDisplacement) == 0x128);

class WaterShaderHeightMap : public BSImageSpaceShader
{
public:
	WaterShaderHeightMap();
	virtual ~WaterShaderHeightMap();

	UInt32				Unk090[8];		// 090
	NiD3DVertexShader*	Vertex;			// 0B0
	NiD3DPixelShader*	Pixel[7];		// 0B4
	UInt32				Unk0D0[15];		// 0D0

};
STATIC_ASSERT(sizeof(WaterShaderHeightMap) == 0x10C);

class HDRShader : public BSImageSpaceShader
{
public:
	HDRShader();
	virtual ~HDRShader();

	NiD3DVertexShader*	Vertex[8];		// 090
	NiD3DPixelShader*	Pixel[8];		// 0B0
	UInt32				Unk0D0;			// 0D0
	UInt32				Unk0D4[13];		// 0D4
	UInt32				Unk108[7];		// 108

};
STATIC_ASSERT(sizeof(HDRShader) == 0x124);

class SpeedTreeLeafShader : public BSShader
{
public:
	SpeedTreeLeafShader();
	virtual ~SpeedTreeLeafShader();
	
	NiVector4			LeafData[48];	// 07C
	NiD3DVertexShader*	Vertex[4];		// 37C
	NiD3DPixelShader*	Pixel[2];		// 38C
	UInt32				Unk394;			// 394

};
STATIC_ASSERT(sizeof(SpeedTreeLeafShader) == 0x398);

class ShadowLightShader : public BSShader
{
public:
	ShadowLightShader(UInt32 Arg1, UInt32 Arg2, UInt32 Arg3, UInt32 Arg4);
	virtual ~ShadowLightShader();

	UInt32				Unk07C[8];		// 07C

};

class ParallaxShader : public ShadowLightShader
{
public:
	ParallaxShader(UInt32 Arg1, UInt32 Arg2);
	virtual ~ParallaxShader();

	NiD3DVertexShader*	Vertex[36];		// 09C
	NiD3DPixelShader*	Pixel[30];		// 12C

};
STATIC_ASSERT(sizeof(ParallaxShader) == 0x1A4);

class SkinShader : public ShadowLightShader
{
public:
	SkinShader(UInt32 Arg1, UInt32 Arg2);
	virtual ~SkinShader();

	NiD3DVertexShader*	Vertex[20];		// 09C
	NiD3DPixelShader*	Pixel[10];		// 0EC

};
STATIC_ASSERT(sizeof(SkinShader) == 0x114);

struct ShaderDefinition
{
	NiDX9ShaderDeclaration* ShaderDeclaration;
	BSShader*				Shader;
};