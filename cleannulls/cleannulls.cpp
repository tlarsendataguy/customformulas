#include "stdafx.h"

struct FormulaAddInData
{
	int nVarType; // 1 for double, 2 for wchar_t
	int isNull; // 1 if NULL, 0 if valid
	double dVal; // valid if nVarType==1 && isNull==0
	const wchar_t * pVal; // valid if nVarType==2 && isNull==0
};

typedef long(_stdcall * FormulaAddInPlugin)(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue);

void SetString(FormulaAddInData *pReturnValue, const wchar_t *pString)
{
	size_t nLen = wcslen(pString);
	wchar_t *pStringRet = (wchar_t *)GlobalAlloc(GMEM_FIXED, (nLen + 1) * sizeof(wchar_t));
	wcscpy(pStringRet, pString);
	pReturnValue->pVal = pStringRet;
	pReturnValue->nVarType = 2;
	pReturnValue->isNull = 0;
}

void SetDouble(FormulaAddInData *pReturnValue, double value)
{
	pReturnValue->dVal = value;
	pReturnValue->nVarType = 1;
	pReturnValue->isNull = 0;
}

extern "C" long _declspec(dllexport) _stdcall CleanNulls(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	if (nNumArgs != 1)
	{
		SetString(pReturnValue, L"Only one parameter can be provided.");
		return 0;
	}

	FormulaAddInData arg = pArgs[0];
	if (arg.isNull == 1)
	{
		if (arg.nVarType == 2)
		{
			SetString(pReturnValue, L"");
		}
		else
		{
			SetDouble(pReturnValue, 0.0);
		}
	}
	else
	{
		if (arg.nVarType == 2)
		{
			SetString(pReturnValue, arg.pVal);
		}
		else
		{
			SetDouble(pReturnValue, arg.dVal);
		}
	}

	return 1;
}
