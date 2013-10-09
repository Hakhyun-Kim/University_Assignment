//
// The GUI for The TringeTaskProject.
// Responsible for scale cube, statistics
// harry

#include "DXUT.h"
#include "SDKmisc.h"
#include "TringeTaskGUI.h"
#include "InstanceProperty.h"
#include "BoxRenderer.h"

//for DXUT GUI
enum
{
	IDC_NUMBERBOXES_STATIC = 1,
	IDC_NUMBERBOXES_SLIDER,
	IDC_TOGGLEUI,
};

TringeTaskGUI* TringeTaskGUI::ms_pSingletone = NULL;

TringeTaskGUI* TringeTaskGUI::Init()
{
	assert( ms_pSingletone == NULL );
	ms_pSingletone = new TringeTaskGUI;
	return GetSingletonePtr();
}

void TringeTaskGUI::Release()
{
	assert( ms_pSingletone != NULL );
	SAFE_DELETE( ms_pSingletone );
}

TringeTaskGUI* TringeTaskGUI::GetSingletonePtr()
{
	assert( ms_pSingletone != NULL );
	return ms_pSingletone;
}

HRESULT TringeTaskGUI::OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice)
{
	HRESULT hr;
	V_RETURN( m_DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Arial", &m_pFont ) );

	return S_OK;
}

void CALLBACK TringeTaskGUI::OnTringeGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	WCHAR szMessage[100];
	switch( nControlID )
	{
	case IDC_TOGGLEUI:
		m_bShowUI = !m_bShowUI;
		break;
	case IDC_NUMBERBOXES_SLIDER:
		{
			int numBoxes = m_BoxUI.GetSlider( IDC_NUMBERBOXES_SLIDER )->GetValue();
			InstanceProperty::GetSingletonePtr()->SetProperty("NumBoxes",numBoxes);
			swprintf_s( szMessage, 100, L"# Boxes: %d", numBoxes ); szMessage[99] = 0;
			m_BoxUI.GetStatic( IDC_NUMBERBOXES_STATIC )->SetText( szMessage );
			BoxRenderer::GetSingletonePtr()->ResetBuffers();
		}		
		break;
	}
}

void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	TringeTaskGUI::GetSingletonePtr()->OnTringeGUIEvent(nEvent, nControlID, pControl, pUserContext);
}

TringeTaskGUI::TringeTaskGUI(void):
m_bShowUI(true),
m_pFont(NULL),
m_pTextSprite(NULL)
{
	// Initialize dialogs
	m_SettingsDlg.Init( &m_DialogResourceManager );
	m_HUD.Init( &m_DialogResourceManager );
	m_BoxUI.Init( &m_DialogResourceManager );

	m_HUD.SetCallback( OnGUIEvent ); int iY = 10;
	m_HUD.AddButton( IDC_TOGGLEUI, L"Toggle UI (key = F1)", 35, iY += 24, 125, 22 );

	WCHAR szMessage[300];
	m_BoxUI.SetCallback( OnGUIEvent ); iY = 0;

	int nNumBoxes = InstanceProperty::GetSingletonePtr()->GetProperty("NumBoxes");
	int nMaxBoxes = InstanceProperty::GetSingletonePtr()->GetProperty("MaxBoxes");
	swprintf_s( szMessage, 300, L"# Boxes: %d", nNumBoxes );
	m_BoxUI.AddStatic( IDC_NUMBERBOXES_STATIC, szMessage, 0, iY += 24, 200, 22 );
	m_BoxUI.AddSlider( IDC_NUMBERBOXES_SLIDER, 50, iY += 24, 100, 22, 1, nMaxBoxes, nNumBoxes );

	m_BoxUI.EnableKeyboardInput( true );
	m_BoxUI.FocusDefaultControl();
}

TringeTaskGUI::~TringeTaskGUI(void)
{
}


HRESULT TringeTaskGUI::OnResetDevice(IDirect3DDevice9* pd3dDevice,UINT width, UINT height)
{
	HRESULT hr;

    V_RETURN( m_DialogResourceManager.OnD3D9ResetDevice() );

    if( m_pFont )
        V_RETURN( m_pFont->OnResetDevice() );
	
    V_RETURN( D3DXCreateSprite( pd3dDevice, &m_pTextSprite ) );

	m_HUD.SetLocation( width - 170, 0 );
    m_HUD.SetSize( 170, 170 );
    m_BoxUI.SetLocation( width - 170, height/2 - 200 );
    m_BoxUI.SetSize( 170, 400 );
	
	return S_OK;
}

void TringeTaskGUI::RenderText()
{
    CDXUTTextHelper txtHelper( m_pFont, m_pTextSprite, 15 );

    txtHelper.Begin();
    txtHelper.SetInsertionPos( 5, 5 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    txtHelper.DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
    
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    
    txtHelper.End();
}

void TringeTaskGUI::OnRender(float fElapsedTime)
{	
	HRESULT hr;
	if( m_bShowUI )
	{
		RenderText();
		V( m_HUD.OnRender( fElapsedTime ) );
		V( m_BoxUI.OnRender( fElapsedTime ) );
	}

}

void TringeTaskGUI::KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown )
{
    if( bKeyDown )
    {
        switch( nChar )
        {
            case VK_F1:
                m_bShowUI = !m_bShowUI; 
				break;
        }
    }
}


void TringeTaskGUI::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing)
{
	*pbNoFurtherProcessing = m_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return;

    if( m_bShowUI )
    {
        *pbNoFurtherProcessing = m_BoxUI.MsgProc( hWnd, uMsg, wParam, lParam );
        if( *pbNoFurtherProcessing )
            return;
        *pbNoFurtherProcessing = m_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
        if( *pbNoFurtherProcessing )
            return;        
    }
}

void TringeTaskGUI::OnLostDevice()
{
	m_DialogResourceManager.OnD3D9LostDevice();
    if( m_pFont )
        m_pFont->OnLostDevice();
    SAFE_RELEASE( m_pTextSprite );
}

void TringeTaskGUI::OnDestroyDevice()
{
	m_DialogResourceManager.OnD3D9DestroyDevice();
	SAFE_RELEASE( m_pFont );
    SAFE_RELEASE( m_pTextSprite );
}