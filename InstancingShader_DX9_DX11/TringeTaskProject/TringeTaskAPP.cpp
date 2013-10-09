#include "DXUT.h"
#include "TringeTaskAPP.h"

#include "InstanceProperty.h"
#include "BoxRenderer.h"
#include "TringeTaskGUI.h"

TringeTaskAPP* TringeTaskAPP::ms_pSingletone = NULL;

TringeTaskAPP* TringeTaskAPP::Init()
{
	assert( ms_pSingletone == NULL );
	ms_pSingletone = new TringeTaskAPP;
	return GetSingletonePtr();
}

void TringeTaskAPP::Release()
{
	assert( ms_pSingletone != NULL );
	SAFE_DELETE( ms_pSingletone );
}

TringeTaskAPP* TringeTaskAPP::GetSingletonePtr()
{
	assert( ms_pSingletone != NULL );
	return ms_pSingletone;
}

TringeTaskAPP::TringeTaskAPP(void)
{
	//Initialize
	m_pInstanceProperty = InstanceProperty::Init();
	m_pBoxRenderer = BoxRenderer::Init();
	m_pTringeTaksGUI = TringeTaskGUI::Init();
}

TringeTaskAPP::~TringeTaskAPP(void)
{
	TringeTaskGUI::Release();
	BoxRenderer::Release();
	InstanceProperty::Release();
}
