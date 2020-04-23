#pragma once

#include "SDK/IVEngineClient.h"
#include "SDK/Vector.h"
#include "SDK\ISurface.h"
#include <d3d9.h>
#include <d3dx9math.h>
struct CHudTexture
{
	char	szShortName[64];	//0x0000
	char	szTextureFile[64];	//0x0040
	bool	bRenderUsingFont;	//0x0080
	bool	bPrecached;			//0x0081
	char	cCharacterInFont;	//0x0082
	BYTE	pad_0x0083;			//0x0083
	int		hFont;				//0x0084
	int		iTextureId;			//0x0088
	float	afTexCoords[4];		//0x008C
	int		iPosX[4];			//0x009C
}; //Size=0x00AC
namespace Engine
{
	typedef struct D3DTLVERTEX
	{
		float x;
		float y;
		float z;
		float rhw;
		D3DCOLOR dxColor;
	} *PD3DTLVERTEX;

	class CRender
	{
	public:

		CRender(IDirect3DDevice9* pDevice);
		~CRender();
		//[swap_lines]
		void OnLostDevice();
		void OnResetDevice();

		void BeginRender();
		void EndRender();
		void TextToConsole(int x, int y, bool center, bool shadow, Color color, const char* format, ...);
		void DrawLine(int x1, int y1, int x2, int y2, Color color);

		void DrawWave1(Vector loc, float radius, Color color);

		void Line(Vector2D start_pos, Vector2D end_pos, Color col);

		void Circle(Vector2D position, float points, float radius, Color color);
		void DrawCircle3D(Vector position, float points, float radius, Color color);

		void DrawBox(int x, int y, int w, int h, Color color);
		void DrawFillBox(int x, int y, int w, int h, Color color);
		void DrawOutlineBox(int x, int y, int w, int h, Color color);

		void DrawCoalBox(int x, int y, int w, int h, Color color);
		void DrawOutlineCoalBox(int x, int y, int w, int h, Color color);

		void GenuineBox(int x, int y, int w, int h, Color color);
		void GenuineOutlineBox(int x, int y, int w, int h, Color color);

		void DrawOctagonBox(int x, int y, int w, int h, Color color);
		void DrawOctagonOutlineBox(int x, int y, int w, int h, Color color);
		void DrawOctagonCoalBox(int x, int y, int w, int h, Color color);

		void DrawAlphaBox(int x, int y, int w, int h, Color color);
		void DrawOutlineFillBox(int x, int y, int w, int h, Color color);

		void DrawWindow(int x, int y, int w, int h, Color Border, Color Background);

		void DrawHorBar(int x, int y, int w, int h, int val, Color color1, Color color2);
		void DrawVerBar(int x, int y, int w, int h, int val, Color color1, Color color2);

		void Text(int x, int y, bool center, bool shadow, Color color, const char* format, ...);
		void WepIcon(int x, int y, bool center, bool shadow, Color color, const char* format, ...);
		//[/swap_lines]
		IDirect3DDevice9* m_pDevice;

	private:
		//[swap_lines]
		bool CreateObject();
		int GetTextHeight(const char* text);
		void SetVertexState();

		void CreateVertex(int x, int y, DWORD dxColor, PD3DTLVERTEX pVertext, DWORD index);

		ID3DXFont* LogFont;
		ID3DXFont* m_pFont;
		ID3DXFont* m_pIconz;
		CHudTexture* m_pIcon;		// 0x724
		IDirect3DStateBlock9* m_pStateBlockDraw;
		IDirect3DStateBlock9* m_pStateBlockText;

		DWORD					dwOld_D3DRS_COLORWRITEENABLE;
		//[/swap_lines]
	};

	namespace D3D9
	{
		enum TABLE
		{
			QueryInterface,
			AddRef,
			Release,
			TestCooperativeLevel,
			GetAvailableTextureMem,
			EvictManagedResources,
			GetDirect3D,
			GetDeviceCaps,
			GetDisplayMode,
			GetCreationParameters,
			SetCursorProperties,
			SetCursorPosition,
			ShowCursor,
			CreateAdditionalSwapChain,
			GetSwapChain,
			GetNumberOfSwapChains,
			Reset,
			Present,
			GetBackBuffer,
			GetRasterStatus,
			SetDialogBoxMode,
			SetGammaRamp,
			GetGammaRamp,
			CreateTexture,
			CreateVolumeTexture,
			CreateCubeTexture,
			CreateVertexBuffer,
			CreateIndexBuffer,
			CreateRenderTarget,
			CreateDepthStencilSurface,
			UpdateSurface,
			UpdateTexture,
			GetRenderTargetData,
			GetFrontBufferData,
			StretchRect,
			ColorFill,
			CreateOffscreenPlainSurface,
			SetRenderTarget,
			GetRenderTarget,
			SetDepthStencilSurface,
			GetDepthStencilSurface,
			BeginScene,
			EndScene,
			Clear,
			SetTransform,
			GetTransform,
			MultiplyTransform,
			SetViewport,
			GetViewport,
			SetMaterial,
			GetMaterial,
			SetLight,
			GetLight,
			LightEnable,
			GetLightEnable,
			SetClipPlane,
			GetClipPlane,
			SetRenderState,
			GetRenderState,
			CreateStateBlock,
			BeginStateBlock,
			EndStateBlock,
			SetClipStatus,
			GetClipStatus,
			GetTexture,
			SetTexture,
			GetTextureStageState,
			SetTextureStageState,
			GetSamplerState,
			SetSamplerState,
			ValidateDevice,
			SetPaletteEntries,
			GetPaletteEntries,
			SetCurrentTexturePalette,
			GetCurrentTexturePalette,
			SetScissorRect,
			GetScissorRect,
			SetSoftwareVertexProcessing,
			GetSoftwareVertexProcessing,
			SetNPatchMode,
			GetNPatchMode,
			DrawPrimitive,
			DrawIndexedPrimitive,
			DrawPrimitiveUP,
			DrawIndexedPrimitiveUP,
			ProcessVertices,
			CreateVertexDeclaration,
			SetVertexDeclaration,
			GetVertexDeclaration,
			SetFVF,
			GetFVF,
			CreateVertexShader,
			SetVertexShader,
			GetVertexShader,
			SetVertexShaderConstantF,
			GetVertexShaderConstantF,
			SetVertexShaderConstantI,
			GetVertexShaderConstantI,
			SetVertexShaderConstantB,
			GetVertexShaderConstantB,
			SetStreamSource,
			GetStreamSource,
			SetStreamSourceFreq,
			GetStreamSourceFreq,
			SetIndices,
			GetIndices,
			CreatePixelShader,
			SetPixelShader,
			GetPixelShader,
			SetPixelShaderConstantF,
			GetPixelShaderConstantF,
			SetPixelShaderConstantI,
			GetPixelShaderConstantI,
			SetPixelShaderConstantB,
			GetPixelShaderConstantB,
			DrawRectPatch,
			DrawTriPatch,
			DeletePatch,
			CreateQuery
		};
	}
}
extern Engine::CRender* g_pRender;