#include "stdafx.h"

TERMS PolySum(TERMS a,TERMS b)
{
	//sort 는 미리 해둔다.
	//sort(a.begin(),a.end(),BinaryTerm);
	//sort(b.begin(),b.end(),BinaryTerm);

	TERMS result;
	copy(result.end(),a.begin(),a.end());
	copy(result.end(),b.begin(),b.end());

	return result;
}

bool IsZeroTerm(TERM term)
{
	if(term.coefficient == 0)
		return true;
	else
		return false;
}



void RemoveZeroTerm(TERMS terms)
{
	terms.erase(remove_if(terms.begin(),terms.end(),IsZeroTerm),terms.end());
}