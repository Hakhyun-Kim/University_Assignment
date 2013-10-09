#pragma once

#include <DXUTcamera.h>

class BoxRenderer
{
public:
	BoxRenderer(void);
	virtual ~BoxRenderer(void);
	HRESULT OnCreateBuffers( IDirect3DDevice9* pd3dDevice );
	void OnDestroyBuffers();
	void ResetBuffers();
	HRESULT OnCreateDevice(IDirect3DDevice9* pd3dDevice);
	void LoadTexture(IDirect3DDevice9* pd3dDevice);
	HRESULT OnResetDevice();
	void OnRenderShaderInstancing( IDirect3DDevice9* pd3dDevice );
	void OnRender( IDirect3DDevice9* pd3dDevice, float fElapsedTime,
		const D3DXMATRIX* pmWorld, const D3DXMATRIX* pmView, const D3DXMATRIX* pmProj );
	void OnLostDevice();
	void OnDestroyDevice();

	static BoxRenderer* GetSingletonePtr();
	static BoxRenderer* Init();
	static void Release();
	void OnFrameMove( double fTime, float fElapsedTime);
protected:
	ID3DXEffect* m_pEffect;
	D3DXHANDLE m_HandleWorld, m_HandleView, m_HandleProjection;
	D3DXHANDLE m_HandleTexture;

	D3DXHANDLE m_HandleTechnique;
	D3DXHANDLE m_HandleBoxInstance_Position;
	IDirect3DTexture9* m_pBoxTexture[8];

	IDirect3DVertexBuffer9* m_pVBBox;
	IDirect3DIndexBuffer9* m_pIBBox;

	D3DXVECTOR4* m_pvBoxInstance_Position;
	D3DXMATRIX m_mWorld;

	D3DXHANDLE m_HandleLightPosition;
	D3DXVECTOR4 m_vLightPosition;
	D3DXVECTOR4 m_vLightPositionView;

	static BoxRenderer* ms_pSingletone;
};
