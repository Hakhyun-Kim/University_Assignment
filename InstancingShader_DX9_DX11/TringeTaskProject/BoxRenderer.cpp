#include "DXUT.h"
#include "BoxRenderer.h"
#include "InstanceProperty.h"
#include "TringeTaskGUI.h"

BoxRenderer* BoxRenderer::ms_pSingletone = NULL;

const int NumTextures = 8;

BoxRenderer* BoxRenderer::Init()
{
	assert( ms_pSingletone == NULL );
	ms_pSingletone = new BoxRenderer;
	return GetSingletonePtr();
}

void BoxRenderer::Release()
{
	assert( ms_pSingletone != NULL );
	SAFE_DELETE( ms_pSingletone );
}

BoxRenderer* BoxRenderer::GetSingletonePtr()
{
	assert( ms_pSingletone != NULL );
	return ms_pSingletone;
}

BoxRenderer::BoxRenderer(void):
m_pEffect(NULL),
m_pVBBox(NULL),
m_pIBBox(NULL),
m_pvBoxInstance_Position(NULL)
{
	int nMaxBoxes = InstanceProperty::GetSingletonePtr()->GetProperty("MaxBoxes");
	m_pvBoxInstance_Position = new D3DXVECTOR4[nMaxBoxes];
	m_vLightPosition = D3DXVECTOR4( 4.0f, 2.0f, 18.0f, 1.0f );
	D3DXMatrixIdentity(&m_mWorld);
}

BoxRenderer::~BoxRenderer(void)
{
	SAFE_DELETE_ARRAY(m_pvBoxInstance_Position);
}

struct BOX_VERTEX
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
	float u, v;
	float boxInstance;
};

IDirect3DVertexBuffer9*         g_pVBInstanceData = 0;

IDirect3DVertexDeclaration9*    g_pVertexDeclShader = NULL;
D3DVERTEXELEMENT9 g_VertexElemShader[] =
{
	{ 0, 0,     D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
	{ 0, 3 * 4, D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,    0 },
	{ 0, 6 * 4, D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
	{ 0, 8 * 4, D3DDECLTYPE_FLOAT1,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  1 },
	D3DDECL_END()
};

void FillFace( BOX_VERTEX* pVerts, WORD* pIndices,int iFace,
			  D3DXVECTOR3 vCenter, D3DXVECTOR3 vNormal, D3DXVECTOR3 vUp, WORD iInstanceIndex )
{
	D3DXVECTOR3 vRight;
	D3DXVec3Cross( &vRight, &vNormal, &vUp );

	WORD offsetIndex = iInstanceIndex * 6 * 2 * 3;
	WORD offsetVertex = iInstanceIndex * 4 * 6;

	pIndices[ offsetIndex + iFace * 6 + 0 ] = (WORD)(offsetVertex + iFace * 4 + 0);
	pIndices[ offsetIndex + iFace * 6 + 1 ] = (WORD)(offsetVertex + iFace * 4 + 1);
	pIndices[ offsetIndex + iFace * 6 + 2 ] = (WORD)(offsetVertex + iFace * 4 + 2);
	pIndices[ offsetIndex + iFace * 6 + 3 ] = (WORD)(offsetVertex + iFace * 4 + 3);
	pIndices[ offsetIndex + iFace * 6 + 4 ] = (WORD)(offsetVertex + iFace * 4 + 2);
	pIndices[ offsetIndex + iFace * 6 + 5 ] = (WORD)(offsetVertex + iFace * 4 + 1);

	pVerts[ offsetVertex + iFace * 4 + 0 ].pos = vCenter + vRight + vUp;
	pVerts[ offsetVertex + iFace * 4 + 1 ].pos = vCenter + vRight - vUp;
	pVerts[ offsetVertex + iFace * 4 + 2 ].pos = vCenter - vRight + vUp;
	pVerts[ offsetVertex + iFace * 4 + 3 ].pos = vCenter - vRight - vUp;

	for( int i = 0; i < 4; i++ )
	{
		pVerts[ offsetVertex + iFace * 4 + i ].boxInstance = ( float )iInstanceIndex;
		pVerts[ offsetVertex + iFace * 4 + i ].u = ( float )( ( i / 2 ) & 1 ) * 1.0f;
		pVerts[ offsetVertex + iFace * 4 + i ].v = ( float )( ( i )&1 ) * 1.0f;
		pVerts[ offsetVertex + iFace * 4 + i ].norm = vNormal;
	}
}

HRESULT BoxRenderer::OnCreateDevice(IDirect3DDevice9* pd3dDevice)
{
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
		
	HRESULT hr;
	V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, L"TringeTask.fx", NULL, NULL, dwShaderFlags,
		NULL, &m_pEffect, NULL ) );

	m_HandleWorld = m_pEffect->GetParameterBySemantic( NULL, "WORLD" );
	m_HandleView = m_pEffect->GetParameterBySemantic( NULL, "VIEW" );
	m_HandleProjection = m_pEffect->GetParameterBySemantic( NULL, "PROJECTION" );
	m_HandleTexture = m_pEffect->GetParameterBySemantic( NULL, "TEXTURE" );
	return S_OK;
}

void BoxRenderer::ResetBuffers()
{
	OnDestroyBuffers();
	OnCreateBuffers( DXUTGetD3D9Device() );
}

void BoxRenderer::OnDestroyBuffers()
{
	SAFE_RELEASE( m_pVBBox );
	SAFE_RELEASE( m_pIBBox );
	SAFE_RELEASE( g_pVBInstanceData );
	SAFE_RELEASE( g_pVertexDeclShader );
}

HRESULT BoxRenderer::OnCreateBuffers( IDirect3DDevice9* pd3dDevice )
{
	HRESULT hr;

	m_HandleTechnique = m_pEffect->GetTechniqueByName( "TShader_TringeTask" );
	m_HandleBoxInstance_Position = m_pEffect->GetParameterBySemantic( NULL, "BOXINSTANCEARRAY_POSITION" );
	m_HandleLightPosition = m_pEffect->GetParameterBySemantic( NULL, "LIGHTPOSITIONVIEW" );

	V_RETURN( pd3dDevice->CreateVertexDeclaration( g_VertexElemShader, &g_pVertexDeclShader ) );

	int nNumBatchInstance = InstanceProperty::GetSingletonePtr()->GetProperty("NumBatchInstance");
	V_RETURN( pd3dDevice->CreateVertexBuffer( nNumBatchInstance * 4 * 6 * sizeof( BOX_VERTEX ), 0, 0,
		D3DPOOL_MANAGED, &m_pVBBox, 0 ) );

	V_RETURN( pd3dDevice->CreateIndexBuffer( nNumBatchInstance * ( 6 * 2 * 3 ) * sizeof( WORD ), 0,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIBBox, 0 ) );

	BOX_VERTEX* pVerts;

	hr = m_pVBBox->Lock( 0, NULL, ( void** )&pVerts, 0 );

	if( SUCCEEDED( hr ) )
	{
		WORD* pIndices;
		hr = m_pIBBox->Lock( 0, NULL, ( void** )&pIndices, 0 );

		if( SUCCEEDED( hr ) )
		{
			for( WORD iInstance = 0; iInstance < nNumBatchInstance; iInstance++ )
			{
				FillFace( pVerts, pIndices, 0,
					D3DXVECTOR3( 0.f, 0.f, -1.f ),
					D3DXVECTOR3( 0.f, 0.f, -1.f ),
					D3DXVECTOR3( 0.f, 1.f, 0.f ),
					iInstance );

				FillFace( pVerts, pIndices, 1,
					D3DXVECTOR3( 0.f, 0.f, 1.f ),
					D3DXVECTOR3( 0.f, 0.f, 1.f ),
					D3DXVECTOR3( 0.f, 1.f, 0.f ),
					iInstance );

				FillFace( pVerts, pIndices, 2,
					D3DXVECTOR3( 1.f, 0.f, 0.f ),
					D3DXVECTOR3( 1.f, 0.f, 0.f ),
					D3DXVECTOR3( 0.f, 1.f, 0.f ),
					iInstance );

				FillFace( pVerts, pIndices, 3,
					D3DXVECTOR3( -1.f, 0.f, 0.f ),
					D3DXVECTOR3( -1.f, 0.f, 0.f ),
					D3DXVECTOR3( 0.f, 1.f, 0.f ),
					iInstance );

				FillFace( pVerts, pIndices, 4,
					D3DXVECTOR3( 0.f, 1.f, 0.f ),
					D3DXVECTOR3( 0.f, 1.f, 0.f ),
					D3DXVECTOR3( 1.f, 0.f, 0.f ),
					iInstance );

				FillFace( pVerts, pIndices, 5,
					D3DXVECTOR3( 0.f, -1.f, 0.f ),
					D3DXVECTOR3( 0.f, -1.f, 0.f ),
					D3DXVECTOR3( 1.f, 0.f, 0.f ),
					iInstance );
			}

			m_pIBBox->Unlock();
		}
		else
		{
			DXUT_ERR( L"Could not lock box model IB", hr );
		}
		m_pVBBox->Unlock();
	}
	else
	{
		DXUT_ERR( L"Could not lock box model VB", hr );
	}

	int nNumBoxes = InstanceProperty::GetSingletonePtr()->GetProperty("NumBoxes");
	
	int lineMaxValue = (int)pow((float)nNumBoxes,1/3.0f);

	if( lineMaxValue*lineMaxValue*lineMaxValue != nNumBoxes )
		lineMaxValue += 1;
	
	int nRemainingBoxes = nNumBoxes;
	for( BYTE iY = 0; iY < lineMaxValue; iY++ )
		for( BYTE iZ = 0; iZ < lineMaxValue; iZ++ )
			for( BYTE iX = 0; iX < lineMaxValue && nRemainingBoxes > 0; iX++, nRemainingBoxes-- )
			{
				m_pvBoxInstance_Position[nNumBoxes - nRemainingBoxes].x = iX * 24 / 255.0f;
				m_pvBoxInstance_Position[nNumBoxes - nRemainingBoxes].z = iZ * 24 / 255.0f;
				m_pvBoxInstance_Position[nNumBoxes - nRemainingBoxes].y = iY * 24 / 255.0f;
				m_pvBoxInstance_Position[nNumBoxes - nRemainingBoxes].w = ( ( WORD )iX + ( WORD )iZ + ( WORD )
					iY ) * 8 / 255.0f;
			}

	nRemainingBoxes = nNumBoxes;
	for( BYTE iY = 0; iY < lineMaxValue; iY++ )
		for( BYTE iZ = 0; iZ < lineMaxValue; iZ++ )
			for( BYTE iX = 0; iX < lineMaxValue && nRemainingBoxes > 0; iX++, nRemainingBoxes-- )
			{				
				int toChangeIndex = rand() % nRemainingBoxes;
				D3DXVECTOR4 temp = m_pvBoxInstance_Position[nNumBoxes - nRemainingBoxes];
				m_pvBoxInstance_Position[nNumBoxes - nRemainingBoxes] = m_pvBoxInstance_Position[toChangeIndex];
				m_pvBoxInstance_Position[toChangeIndex] = temp;
			}



	return S_OK;
}

void BoxRenderer::LoadTexture(IDirect3DDevice9* pd3dDevice)
{
	HRESULT hr;
	WCHAR szMessage[100];
	for (int i = 0; i < NumTextures; i++)
	{
		swprintf_s( szMessage, 100, L"tringeTask_tex%d.dds", i+1 ); szMessage[99] = 0;
		hr = D3DXCreateTextureFromFile( pd3dDevice, szMessage, &m_pBoxTexture[i] );
		if( FAILED( hr ) )
			DXTRACE_ERR( L"D3DXCreateTextureFromFile", hr );
	}
}

void BoxRenderer::OnRenderShaderInstancing( IDirect3DDevice9* pd3dDevice )
{
    HRESULT hr;
    UINT iPass, cPasses;

    V( pd3dDevice->SetVertexDeclaration( g_pVertexDeclShader ) );

    V( pd3dDevice->SetStreamSource( 0, m_pVBBox, 0, sizeof( BOX_VERTEX ) ) );
    V( pd3dDevice->SetIndices( m_pIBBox ) );

	V( m_pEffect->SetTechnique( m_HandleTechnique ) );

    V( m_pEffect->Begin( &cPasses, 0 ) );

	int nNumBoxes = InstanceProperty::GetSingletonePtr()->GetProperty("NumBoxes");
	int nNumBatchInstance = InstanceProperty::GetSingletonePtr()->GetProperty("NumBatchInstance");

    for( iPass = 0; iPass < cPasses; iPass++ )
    {
        V( m_pEffect->BeginPass( iPass ) );

        int nRemainingBoxes = nNumBoxes;
		int index = 0;
		int totalBlocks = nNumBoxes / nNumBatchInstance;
		int opaqueBlocks = totalBlocks*2/3;
		int alphaBlocks = totalBlocks - opaqueBlocks;
		int lastTexture = -1;
		int iTexture = -1;
		bool bEnalbeAlpha = false;

        while( nRemainingBoxes > 0 )
        {
            int nRenderBoxes = min( nRemainingBoxes, nNumBatchInstance );
		
			if(index <=  alphaBlocks)
			{
				if(bEnalbeAlpha == false)	//alpha sorted,,,=> just one time per frame.
				{
					bEnalbeAlpha = true;
					pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
				}
				iTexture = (int)( index*7.0f/alphaBlocks );
			}
			else	//opaque
			{
				if(bEnalbeAlpha == true)
				{
					bEnalbeAlpha = false;
					pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
				}
				iTexture = (int) ( (index-alphaBlocks)*7.0f/opaqueBlocks );
			}

			if(lastTexture != iTexture)	//textures sorted. total 8 times(alpha/nonalpha independtly) called
				V( m_pEffect->SetTexture( m_HandleTexture, m_pBoxTexture[iTexture]  ) );

			index++;

            V( m_pEffect->SetVectorArray( m_HandleBoxInstance_Position, m_pvBoxInstance_Position + nNumBoxes -
                                          nRemainingBoxes, nRenderBoxes ) );
            V( m_pEffect->CommitChanges() );
            V( pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, nRenderBoxes * 4 * 6, 0,
                                                 nRenderBoxes * 6 * 2 ) );
            nRemainingBoxes -= nRenderBoxes;
        }

        V( m_pEffect->EndPass() );
    }
    V( m_pEffect->End() );
}

void BoxRenderer::OnFrameMove( double fTime, float fElapsedTime)
{
	D3DXMatrixRotationY(&m_mWorld,(float)fTime / 10.0f);

	D3DXMATRIX lightRot;
	D3DXMatrixRotationY(&lightRot,(float)fTime / 3.0f);
	D3DXVECTOR4	lightPos = D3DXVECTOR4( -24.0f, 36.0f, -36.0f, 1.0f );
	D3DXVec4Transform(&m_vLightPosition, &lightPos, &lightRot);

}

void BoxRenderer::OnRender( IDirect3DDevice9* pd3dDevice, float fElapsedTime, 
						   const D3DXMATRIX* pmWorld, const D3DXMATRIX* pmView, const D3DXMATRIX* pmProj )
{
	
	HRESULT hr;
	D3DXMATRIXA16 mWorld;
    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;

    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 170, 50 ), 1.0f, 0 ) );

    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		mWorld = *pmWorld * m_mWorld;
        mView = *pmView;
        mProj = *pmProj;

        V( m_pEffect->SetMatrix( m_HandleWorld, &m_mWorld ) );
        V( m_pEffect->SetMatrix( m_HandleView, &mView ) );
        V( m_pEffect->SetMatrix( m_HandleProjection, &mProj ) );
		D3DXVec4Transform( &m_vLightPositionView, &m_vLightPosition, pmView );
		V( m_pEffect->SetVector( m_HandleLightPosition, &m_vLightPositionView) );

		//D3DXVECTOR4 test;
		//V( m_pEffect->GetVector( m_HandleLightPosition, &test) );

        OnRenderShaderInstancing( pd3dDevice );
        
		TringeTaskGUI::GetSingletonePtr()->OnRender( fElapsedTime );

        V( pd3dDevice->EndScene() );
    }
}

HRESULT BoxRenderer::OnResetDevice()
{
	HRESULT hr;

	if( m_pEffect )
        V_RETURN( m_pEffect->OnResetDevice() );

	return S_OK;
}

void BoxRenderer::OnLostDevice()
{
    if( m_pEffect )
        m_pEffect->OnLostDevice();
}

void BoxRenderer::OnDestroyDevice()
{
	SAFE_RELEASE( m_pEffect );
	for(int i = 0; i < NumTextures; i++)
		SAFE_RELEASE( m_pBoxTexture[i] );
    OnDestroyBuffers();
}