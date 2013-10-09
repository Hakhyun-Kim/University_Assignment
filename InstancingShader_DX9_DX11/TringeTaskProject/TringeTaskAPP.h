#pragma once

class BoxRenderer;
class InstanceProperty;
class TringeTaskGUI;
class ShaderManger;

class TringeTaskAPP
{
public:
	TringeTaskAPP(void);
	virtual ~TringeTaskAPP(void);

	static TringeTaskAPP* GetSingletonePtr();
	static TringeTaskAPP* Init();
	static void Release();

	void MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);

protected:
	static TringeTaskAPP* ms_pSingletone;

	BoxRenderer* m_pBoxRenderer;
	InstanceProperty* m_pInstanceProperty;
	TringeTaskGUI* m_pTringeTaksGUI;
	ShaderManger* m_pShaderManager;
};
