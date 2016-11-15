
#ifndef __USB_COM_FUNC_H__
#define __USB_COM_FUNC_H__

#include "Interface.h"

GUID  Guid_ANDROID_USB2Ser = {0xA5DCBF10L,0x6530,0x11D2,{0x90,0x1F,0x00,0xC0,0x4F,0xB9,0x51,0xED}};
#define  PID_ANDROID       "2000"
#define  VID_ANDROID       "0E8D"
#define  InterfaceClassGuidConstant  Guid_ANDROID_USB2Ser
#define  PID                      PID_ANDROID
#define  VID                      VID_ANDROID

#define  PID_KERNEL        "0005"
#define  VID_KERNEL        "0BB4"

void scan_exist_com_port_str(
    char *exist_symbolic_str[] /*out*/,
    char *exist_port_str[] /*out*/,
    char * PID_Scan, char * VID_Scan, GUID guid, int *device_num);

int get_new_created_com_port_str (char *symbolic_return_value, char *port_return_value,
                                  char * exist_symbolic_str[], char * exist_port_str[],
                                  char * PID_Scan, char * VID_Scan, GUID guid);

#endif

