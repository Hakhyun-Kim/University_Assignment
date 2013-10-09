//--------------------------------------------------------------------------------------
// File: DXUTHelper.cpp
//--------------------------------------------------------------------------------------

#include "DXUT.h"
#include "resource.h"
#include "TringeTaskApp.h"
#include "TringeTaskGUI.h"
#include "BoxRenderer.h"
#include "DXUTcamera.h"

CModelViewerCamera g_Camera;

bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

	if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
		return false;

	if( !( pCaps->DevCaps2 & D3DDEVCAPS2_STREAMOFFSET ) )
		return false;
    return true;
}

bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	pDeviceSettings->d3d9.pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    return true;
}

HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
	HRESULT hr;

	V_RETURN( TringeTaskGUI::GetSingletonePtr()->OnD3D9CreateDevice( pd3dDevice ) );
	V_RETURN( BoxRenderer::GetSingletonePtr()->OnCreateDevice( pd3dDevice ) );

    D3DXVECTOR3 vecEye( -24.0f, 36.0f, -36.0f );
    D3DXVECTOR3 vecAt ( 0.0f, 0.0f, 0.0f );
    g_Camera.SetViewParams( &vecEye, &vecAt );

	V_RETURN( BoxRenderer::GetSingletonePtr()->OnCreateBuffers( pd3dDevice ) );
	BoxRenderer::GetSingletonePtr()->LoadTexture( pd3dDevice );

    return S_OK;
}


HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
	BoxRenderer::GetSingletonePtr()->OnResetDevice();
	TringeTaskGUI::GetSingletonePtr()->OnResetDevice(pd3dDevice,pBackBufferSurfaceDesc->Width,pBackBufferSurfaceDesc->Height);

    float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI / 3, fAspectRatio, 0.1f, 10000.0f );
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

    g_Camera.SetScalers( 0.01f, 10.0f );

    g_Camera.SetEnableYAxisMovement( false );
    g_Camera.SetEnablePositionMovement( true );

    return S_OK;
}


void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	g_Camera.FrameMove( fElapsedTime );
	BoxRenderer::GetSingletonePtr()->OnFrameMove(fTime,fElapsedTime);
}


void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	BoxRenderer::GetSingletonePtr()->OnRender( pd3dDevice, fElapsedTime,
		g_Camera.GetWorldMatrix(), g_Camera.GetViewMatrix(), g_Camera.GetProjMatrix() );
}


LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	TringeTaskGUI::GetSingletonePtr()->MsgProc( hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing);

	if( *pbNoFurtherProcessing )
		return 0;
	
	g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}

void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	TringeTaskGUI::GetSingletonePtr()->KeyboardProc(nChar, bKeyDown, bAltDown);
}

void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	TringeTaskGUI::GetSingletonePtr()->OnLostDevice();
	BoxRenderer::GetSingletonePtr()->OnLostDevice();
}


void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	TringeTaskGUI::GetSingletonePtr()->OnDestroyDevice();
	BoxRenderer::GetSingletonePtr()->OnDestroyDevice();
	TringeTaskAPP::Release();
}


INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackKeyboard( KeyboardProc );
    DXUTSetCallbackFrameMove( OnFrameMove );

    DXUTInit( true, true ); // Parse the command line and show msgboxes
	
	TringeTaskAPP::Init();

	DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"TringeTaskProject" );
    DXUTCreateDevice( true, 1024, 768 );

    // Start the render loop
    DXUTMainLoop();

    return DXUTGetExitCode();
}


