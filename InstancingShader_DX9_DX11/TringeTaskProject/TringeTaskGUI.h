#pragma once

#include "DXUTsettingsdlg.h"

class TringeTaskGUI
{
public:
	TringeTaskGUI(void);
	virtual ~TringeTaskGUI(void);

	static TringeTaskGUI* GetSingletonePtr();
	static TringeTaskGUI* Init();
	static void Release();

	void CALLBACK OnTringeGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
	HRESULT OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice);
	HRESULT OnResetDevice(IDirect3DDevice9* pd3dDevice,UINT width, UINT height);
	void OnRender(float fElapsedTime);
	void RenderText();
	void OnLostDevice();
	void MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
	void KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown );
	void OnDestroyDevice();

protected:
	BOOL m_bShowUI;

	ID3DXFont* m_pFont;
	ID3DXSprite* m_pTextSprite;
	BOOL m_bShowHelp;

	CDXUTDialogResourceManager      m_DialogResourceManager;
	CD3DSettingsDlg                 m_SettingsDlg;
	CDXUTDialog                     m_HUD;
	CDXUTDialog                     m_BoxUI;

	static TringeTaskGUI* ms_pSingletone;
};
