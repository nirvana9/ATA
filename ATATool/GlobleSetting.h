
#ifndef __GLOBLESETTING_H__
#define __GLOBLESETTING_H__

#define MAX_DUT_NUM     4

typedef struct
{
    S_ATADLL_COMPORT	com_port_info;
    int					bActive;
} S_COMPORT_ACTIVE_INFO;

typedef struct
{
    int     loop_times;
    int     fail_times[E_TEST_ITEM_COUNT];
} S_TESTITEM_LOOP_TEST_INFO;

#endif