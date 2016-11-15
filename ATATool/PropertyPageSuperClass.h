#ifndef __PROPERYPAGE_SUPERCLASS_H__
#define __PROPERYPAGE_SUPERCLASS_H__

#include "..\ATA_DLL\ATA_DLL.h"

class CProperyPageSuperClass
{
public:
    CProperyPageSuperClass ();
    ~CProperyPageSuperClass ();

    void SetCommonCFG (S_ATADLL_COMMON_CFG_T *common_cfg);
    void SetTestItemSpec (S_ATADLL_TESTITEM_SPEC *spec);

    virtual void UpdateParameters () = 0;
    virtual void InitParameters () = 0;

protected:
    S_ATADLL_COMMON_CFG_T	*p_sCommonCFG;
    S_ATADLL_TESTITEM_SPEC	*p_sTestSpec;
};

#endif