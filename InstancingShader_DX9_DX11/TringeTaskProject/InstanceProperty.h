//
// Instancing need some property value.
// Also test project for component pattern.
// it needs fast string, but here i jsut use std:string.
//

#pragma once

#include <map>
#include <string>
class InstanceProperty
{
public:
	
	InstanceProperty(void);
	virtual ~InstanceProperty(void);

	int GetProperty(std::string name);

	void SetProperty(std::string name, int val);

	static InstanceProperty* GetSingletonePtr();
	static InstanceProperty* Init();
	static void Release();

protected:
	std::map<std::string, int> m_properties;

	static InstanceProperty* ms_pSingletone;
};
