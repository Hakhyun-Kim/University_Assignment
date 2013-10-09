#include "DXUT.h"
#include "InstanceProperty.h"

const int NumBatchInstance = 48; //same as .fx file value.
const int NumMaxBoxes = 16*16*16;
const int NumInitialBoxes = 12*12*12;

InstanceProperty* InstanceProperty::ms_pSingletone = NULL;

InstanceProperty* InstanceProperty::Init()
{
	assert( ms_pSingletone == NULL );
	ms_pSingletone = new InstanceProperty;
	return GetSingletonePtr();
}

void InstanceProperty::Release()
{
	assert( ms_pSingletone != NULL );
	SAFE_DELETE( ms_pSingletone );
}

InstanceProperty::InstanceProperty(void)
{
	SetProperty("MaxBoxes", NumMaxBoxes);
	SetProperty("NumBoxes", NumInitialBoxes);
	SetProperty("NumBatchInstance", NumBatchInstance);

	//simple unit test
	assert( GetProperty("MaxBoxes") == NumMaxBoxes);
	assert( GetProperty("NumBoxes") == NumInitialBoxes);
	assert( GetProperty("NumBatchInstance") == NumBatchInstance);
}

InstanceProperty::~InstanceProperty(void)
{
}


InstanceProperty* InstanceProperty::GetSingletonePtr()
{
	assert( ms_pSingletone != NULL );
	return ms_pSingletone;
}

int InstanceProperty::GetProperty( std::string name )
{
	assert( m_properties.find(name) != m_properties.end() );
	return m_properties[name];
}

void InstanceProperty::SetProperty( std::string name, int val )
{
	//assert( m_properties.find(name) != m_properties.end() );
	m_properties[name] = val;
}