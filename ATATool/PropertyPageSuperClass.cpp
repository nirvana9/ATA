
#include "stdafx.h"
#include "PropertyPageSuperClass.h"

CProperyPageSuperClass::CProperyPageSuperClass()
{
    p_sCommonCFG	= NULL;
    p_sTestSpec		= NULL;
}

CProperyPageSuperClass::~CProperyPageSuperClass()
{
}

void CProperyPageSuperClass::SetCommonCFG (S_ATADLL_COMMON_CFG_T *common_cfg)
{
    this->p_sCommonCFG		= common_cfg;
}

void CProperyPageSuperClass::SetTestItemSpec (S_ATADLL_TESTITEM_SPEC *spec)
{
    this->p_sTestSpec		= spec;
}