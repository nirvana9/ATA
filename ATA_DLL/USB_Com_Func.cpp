#include "StdAfx.h"
#include "Interface.h"

void scan_exist_com_port_str(
    char *exist_symbolic_str[] /*out*/,
    char *exist_port_str[] /*out*/,
    char * PID_Scan, char * VID_Scan, GUID guid, int  *device_num)
{
    unsigned long	device_number = 0, i = 0;
    char		port_name[2048];
    unsigned long	port_name_length = 0;
    char		symbolic_name[2048];
    unsigned long	symbolic_name_length = 0;

    device_number =  GetDeviceNumber(guid, PID_Scan, VID_Scan);//Get_Device_Number
    *device_num=device_number;
    if( device_number == 0)
    {
        return;
    }

    for( i=0; i<device_number; i++)
    {
        memset (port_name, 0x0, sizeof(char) * 2048);
        memset (symbolic_name, 0x0, sizeof(char) * 2048);
        if(GetPresentDevicePath(guid, PID_Scan, VID_Scan, i,
                                port_name, sizeof(port_name), (unsigned long *) &port_name_length,
                                symbolic_name, sizeof(symbolic_name), (unsigned long *) &symbolic_name_length))//Get_Device_Path
        {
            *(exist_symbolic_str + i) = symbolic_name;
            *(exist_port_str + i) = port_name;
        }
    }
}


int get_new_created_com_port_str (char *symbolic_return_value, char *port_return_value,
                                  char * exist_symbolic_str[], char * exist_port_str[],
                                  char * PID_Scan, char * VID_Scan, GUID guid)
{
    char *          currentSymbolicName[20] = {"\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0"};
    char *          currentPortName[20] = {"\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0"};
    int             device_num=0;
    int             i = 0, j = 0;

    scan_exist_com_port_str(currentSymbolicName, currentPortName, PID_Scan, VID_Scan, guid, &device_num);

    for (i = 0; i < device_num; i++)
    {
        /**
        char buff[256];
        sprintf (buff, "%s\r\n", *(currentPortName + i));
        TRACE (buff);
        **/

        if (strcmp(*(currentSymbolicName + i), *(exist_symbolic_str + i)) != 0)
        {
            j = strlen(*(currentSymbolicName+i));
            memcpy(symbolic_return_value, *(currentSymbolicName+i), j);

            j = strlen (*(currentPortName+i));
            memcpy (port_return_value, *(currentPortName+i), j);

            return 1;
        }
    }

    return 0;
}