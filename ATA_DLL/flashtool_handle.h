/*******************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2003
 *
 ******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *  flashtool_handle.h
 *
 * Project:
 * --------
 *  Flash Download/Readback/Format Library.
 *
 * Description:
 * ------------
 *  The exported C interface APIs to deal with FlashTool handles.
 *
 * Author:
 * -------
 *  Amos Hsu (mtk00539)
 *
 *******************************************************************************/
 
#ifndef _FLASHTOOL_HANDLE_H_
#define _FLASHTOOL_HANDLE_H_

#include "mtk_mcu.h"
#include "mtk_status.h"
#include "mtk_resource.h"
#include "DOWNLOAD.H"
#include "host.h"

#ifdef  __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Boolean                                                                      
//------------------------------------------------------------------------------
typedef enum {
    _FALSE = 0,
    _TRUE = 1
} _BOOL;

//------------------------------------------------------------------------------
// prototype of remote file operation method callback                           
//------------------------------------------------------------------------------
typedef int (__stdcall *CALLBACK_REMOTE_GET_FILE_LEN)(const char *filepath, unsigned int * const p_file_length, void *user_arg);
typedef int (__stdcall *CALLBACK_REMOTE_READ_FILE)(const char *filepath, unsigned char * const p_file_content, unsigned int file_length, void *user_arg);
typedef int (__stdcall *CALLBACK_REMOTE_READ_FILE_BY_PACKAGE)(const char *filepath, U64 u64CntStartOffset, unsigned char * const p_file_content, unsigned int package_length, void *user_arg);


//------------------------------------------------------------------------------
// prototype of remote file's signature operation method callback                           
//------------------------------------------------------------------------------
typedef int (__stdcall *CALLBACK_REMOTE_GET_SIG_LEN)(const char *filepath, unsigned int * const p_sig_length, void *user_arg);
typedef int (__stdcall *CALLBACK_REMOTE_READ_SIG)(const char *filepath, unsigned char * const p_sig_content, unsigned int sig_length, void *user_arg);

//------------------------------------------------------------------------------
// prototype of customized file integrity check callback                        
//------------------------------------------------------------------------------
typedef int (__stdcall *CALLBACK_FILE_INTEGRITY_CHECK)(unsigned char * const p_file_content, unsigned int * const p_file_length, void *user_arg);

//------------------------------------------------------------------------------
// BL_HANDLE: Exported Functions                                                
//------------------------------------------------------------------------------
/* data structure used only by MT6516 combo mcp */
typedef struct {
    unsigned int  m_nand_id;
    unsigned int    m_emi_gen_a;
    unsigned int    m_emi_gen_b;
    unsigned int    m_emi_gen_c;
    unsigned int    m_emi_gen_d;
    unsigned int    m_emi_con_i;
    unsigned int    m_emi_con_j;
    unsigned int    m_emi_con_k;
    unsigned int    m_emi_con_l;
    unsigned int    m_emi_con_m;
    unsigned int    m_emi_con_n;
    unsigned int    m_emi_del_a;
    unsigned int    m_emi_del_b;
    unsigned int    m_emi_del_c;
    unsigned int    m_emi_del_d;
    unsigned int    m_emi_del_e;
    unsigned int    m_emi_del_f;
    unsigned int    m_emi_del_g;
    unsigned int    m_emi_del_h;
    unsigned int    m_emi_del_i;
    unsigned int    m_emi_del_j;
} EMI_SETTINGS_v05;

/* data structure used only by MT6573 combo mcp */
typedef struct {
    unsigned int m_nand_id;
    unsigned int m_emi_coni;
    unsigned int m_emi_drva;
    unsigned int m_emi_drvb;
    unsigned int m_emi_conj;
    unsigned int m_emi_conk;
    unsigned int m_emi_conl;
    unsigned int m_emi_iocl;
    unsigned int m_emi_gena;
    unsigned int m_emi_gend;
    unsigned int m_emi_drct;
    unsigned int m_emi_ppct;
    unsigned int m_emi_slct;
    unsigned int m_emi_abct;
    unsigned int m_emi_dutb;
} EMI_SETTINGS_v06;

/* data structure used only by MT6573 combo mcp */
typedef struct {
    unsigned int m_nand_id;
    unsigned int m_emi_coni;
    unsigned int m_emi_drva;
    unsigned int m_emi_drvb;
    unsigned int m_emi_conj;
    unsigned int m_emi_conk;
    unsigned int m_emi_conl;
    unsigned int m_emi_iocl;
    unsigned int m_emi_gena;
    unsigned int m_emi_gend;
    unsigned int m_emi_drct;
    unsigned int m_emi_ppct;
    unsigned int m_emi_slct;
    unsigned int m_emi_abct;
    unsigned int m_emi_dutb;
    unsigned int m_emi_conn;
} EMI_SETTINGS_v07;

#define MCP_MASK_DEV  0xFF00
#define MCP_MASK_RAM  0x00FF
#define MCP_DISC_RAM  0x0000 
#define MCP_NAND_DEV  0x0100
#define MCP_EMMC_DEV  0x0200
#define MCP_DDR1_RAM  0x0001
#define MCP_DDR2_RAM  0x0002
#define MCP_DDR3_RAM  0x0003
#define MCP_DISC_DDR1 MCP_DDR1_RAM
#define MCP_DISC_DDR2 MCP_DDR2_RAM
#define MCP_DISC_DDR3 MCP_DDR3_RAM
#define MCP_NAND_DDR1 (MCP_NAND_DEV | MCP_DDR1_RAM)
#define MCP_NAND_DDR2 (MCP_NAND_DEV | MCP_DDR2_RAM)
#define MCP_NAND_DDR3 (MCP_NAND_DEV | MCP_DDR3_RAM)
#define MCP_EMMC_DDR1 (MCP_EMMC_DEV | MCP_DDR1_RAM)
#define MCP_EMMC_DDR2 (MCP_EMMC_DEV | MCP_DDR2_RAM)
#define MCP_EMMC_DDR3 (MCP_EMMC_DEV | MCP_DDR3_RAM)
#define MCP_NAND_ID_LEN 5
#define MCP_EMMC_ID_LEN 9
typedef struct {
    unsigned int m_type;         /* 0x0000 : Invalid
                                    0x0001 : Discrete DDR1
                                    0x0002 : Discrete DDR2
                                    0x0003 : Discrete DDR3
                                    0x0101 : MCP(NAND+DDR1)
                                    0x0102 : MCP(NAND+DDR2)
                                    0x0103 : MCP(NAND+DDR3)
                                    0x0201 : MCP(eMMC+DDR1)
                                    0x0202 : MCP(eMMC+DDR2)
                                    0x0203 : MCP(eMMC+DDR3)
                                 */
    unsigned char m_id[12];
    int     m_emi_cona;             //@0x3000
    int     m_dramc_drvctl0;        //@0x40B8               -> customized TX I/O driving
    int     m_dramc_drvctl1;        //@0x40BC               -> customized TX I/O driving
    int     m_dramc_dle;            //40E4[4]:407c[6:4]     -> customized
    int     m_dramc_actim;          //@0x4000
    int     m_dramc_gddr3ctl;       //@0x40F4
    int     m_dramc_conf1;          //@0x4004
    int     m_dramc_ddr2ctl;        //@0x407C 
    int     m_dramc_test2_3;        //@0x4044
    int     m_dramc_conf2;          //@0x4008
    int     m_dramc_pd_ctrl;        //@0x41DC
    int     m_dramc_padctl3;        //@0x4014               -> customized TX DQS delay
    int     m_dramc_dqodly;         //@0x4200~0x420C        -> customized TX DQ delay
    int     m_dramc_addr_output_dly;// for E1 DDR2 only
    int     m_dramc_clk_output_dly; // for E1 DDR2 only
    int     m_reserved[10];

    union
    {
        struct _DDR1 
        {
            unsigned int m_mode;
            unsigned int m_ext_mode;    // dram driving strength -> customized
        } ddr1;
        struct _DDR2
        {
            unsigned int m_mode1;
            unsigned int m_mode2;
            unsigned int m_mode3;       // dram driving strength  -> customized
            unsigned int m_mode10;
            unsigned int m_mode63;
        } ddr2;
        struct
        {
            unsigned int m_mode0;
            unsigned int m_mode1;
            unsigned int m_mode2;
            unsigned int m_mode3;            
        } ddr3;        
    }u;    
} EMI_SETTINGS_v08;

/* For EMMC FW Check*/
typedef struct {
    unsigned int m_type;         /* 0x0000 : Invalid
                                    0x0001 : Discrete DDR1
                                    0x0002 : Discrete DDR2
                                    0x0003 : Discrete DDR3
                                    0x0101 : MCP(NAND+DDR1)
                                    0x0102 : MCP(NAND+DDR2)
                                    0x0103 : MCP(NAND+DDR3)
                                    0x0201 : MCP(eMMC+DDR1)
                                    0x0202 : MCP(eMMC+DDR2)
                                    0x0203 : MCP(eMMC+DDR3)
                                 */
    unsigned char m_id[12];
    int     m_emi_cona;             //@0x3000
    int     m_dramc_drvctl0;        //@0x40B8               -> customized TX I/O driving
    int     m_dramc_drvctl1;        //@0x40BC               -> customized TX I/O driving
    int     m_dramc_dle;            //40E4[4]:407c[6:4]     -> customized
    int     m_dramc_actim;          //@0x4000
    int     m_dramc_gddr3ctl;       //@0x40F4
    int     m_dramc_conf1;          //@0x4004
    int     m_dramc_ddr2ctl;        //@0x407C
    int     m_dramc_test2_3;        //@0x4044
    int     m_dramc_conf2;          //@0x4008
    int     m_dramc_pd_ctrl;        //@0x41DC
    int     m_dramc_padctl3;        //@0x4014               -> customized TX DQS delay
    int     m_dramc_dqodly;         //@0x4200~0x420C        -> customized TX DQ delay
    int     m_dramc_addr_output_dly;// for E1 DDR2 only
    int     m_dramc_clk_output_dly; // for E1 DDR2 only
	//Modification of  m_reserved[10];
	//int     m_reserved[10];
	int     DRAM_RANK_SIZE[4];
	int     m_reserved[2];
	char  m_emmc_fw_version[8];//For EMMC FW Version, Sandisk 6 byte, Others 1 byte
	//For EMMC: m_id_length = EMMC CID length(9 byte) + EMMC FW version bytes
	//For NAND: m_id_length = NAND ID length
	int     m_id_length;
	int     m_bloader_sub_version;

	union
    {
        struct _DDR1
        {
            unsigned int m_mode;
            unsigned int m_ext_mode;    // dram driving strength -> customized
        } ddr1;
        struct _DDR2
        {
            unsigned int m_mode1;
            unsigned int m_mode2;
            unsigned int m_mode3;       // dram driving strength  -> customized
            unsigned int m_mode10;
            unsigned int m_mode63;
        } ddr2;
        struct
        {
            unsigned int m_mode0;
            unsigned int m_mode1;
            unsigned int m_mode2;
            unsigned int m_mode3;
        } ddr3;
    }u;
} EMI_SETTINGS_v0801;

/* For NAND Extension */
typedef struct {
    unsigned int m_type;         /* 0x0000 : Invalid
                                    0x0001 : Discrete DDR1
                                    0x0002 : Discrete DDR2
                                    0x0003 : Discrete DDR3
                                    0x0101 : MCP(NAND+DDR1)
                                    0x0102 : MCP(NAND+DDR2)
                                    0x0103 : MCP(NAND+DDR3)
                                    0x0201 : MCP(eMMC+DDR1)
                                    0x0202 : MCP(eMMC+DDR2)
                                    0x0203 : MCP(eMMC+DDR3)
                                 */
    unsigned char m_id[12];
	unsigned int m_id_number;
	unsigned int m_bloader_sub_version;
    unsigned int m_emi_coni;
    unsigned int m_emi_drva;
    unsigned int m_emi_drvb;
    unsigned int m_emi_conj;
    unsigned int m_emi_conk;
    unsigned int m_emi_conl;
    unsigned int m_emi_iocl;
    unsigned int m_emi_gena;
    unsigned int m_emi_gend;
    unsigned int m_emi_drct;
    unsigned int m_emi_ppct;
    unsigned int m_emi_slct;
    unsigned int m_emi_abct;
    unsigned int m_emi_dutb;
    unsigned int m_emi_conn;
	int     m_reserved[4];
} EMI_SETTINGS_v09;

#define DRAM_CFG_ENABLE_CALIBRATION 0x00000001
#define DRAM_CFG_ENABLE_DUMP        0x80000000
typedef struct {
    _BOOL               m_valid;    // _TRUE -> dram setting is valid 
    
    unsigned int        m_emi_gen_a;
    unsigned int        m_emi_gen_b;
    unsigned int        m_emi_gen_c;
    unsigned int        m_emi_gen_d;
    unsigned int        m_emi_con_i;
    unsigned int        m_emi_con_i_ext;
    unsigned int        m_emi_con_j;
    unsigned int        m_emi_con_k;
    unsigned int        m_emi_con_l;
    unsigned int        m_emi_con_m;
    unsigned int        m_emi_con_n;
    unsigned int        m_emi_del_a;
    unsigned int        m_emi_del_b;
    unsigned int        m_emi_del_c;
    unsigned int        m_emi_del_d;
    unsigned int        m_emi_del_e;
    unsigned int        m_emi_del_f;
    unsigned int        m_emi_del_g;
    unsigned int        m_emi_del_h;
    unsigned int        m_emi_del_i;
    unsigned int        m_emi_del_j;
    unsigned int        m_emi_arb_a;
    unsigned int        m_emi_arb_b;
    unsigned int        m_emi_arb_c;
    unsigned int        m_emi_arb_d;
    unsigned int        m_emi_arb_e;
    unsigned int        m_emi_arb_f;
    unsigned int        m_emi_arb_g;
    unsigned int        m_emi_arb_h;
    unsigned int        m_emi_arb_i;
    unsigned int        m_emi_drv_a;
    unsigned int        m_emi_drv_b;
    unsigned int        m_emi_iocl;
    unsigned int        m_emi_drct;
    unsigned int        m_emi_ppct;
    unsigned int        m_emi_slct;
    unsigned int        m_emi_abct;
    unsigned int        m_emi_dutb;

    // used in chips for SV5
    unsigned int        m_ver; 
    union
    {
        EMI_SETTINGS_v05 m_emi05;
        EMI_SETTINGS_v06 m_emi06;
        EMI_SETTINGS_v07 m_emi07;
        EMI_SETTINGS_v08 m_emi08;
        EMI_SETTINGS_v09 m_emi09;
    };        
} DRAM_SETTING;

typedef struct {
    char                m_version[5];
    char                m_filepath[512];
    unsigned int        m_filesize;
    unsigned int        m_start_addr;
    unsigned int        m_chksum_seed;
    unsigned int        m_chksum;
    DRAM_SETTING        m_dram_cfg;
    _BOOL               m_ext_bootloader_exist;
    char                m_ext_filepath[512];
    unsigned int        m_ext_filesize;
    unsigned int        m_ext_chksum;
    
    _BOOL               m_enable;   // _FALSE -> disbale 
                                    // _TRUE  -> enable  
} BL_INFO;

//------------------------------------------------------------------------------
// DL_HANDLE: Exported Functions                                                
//------------------------------------------------------------------------------
typedef enum {
    ABSOLUTE_ADDR = 0,
    OFFSET_VALUE
} REGION_ADDR_TYPE;

typedef enum {
    NORMAL_ROM      = 0x01,         //kghost:SV5 it is 
    RESOURCE_BIN    = 0x02,
    JUMPTABLE_BIN   = 0x03,
    ENFB_BIN        = 0x04,
    UA_BIN          = 0x05,
    BL_BIN          = 0x06,
      // kghost ---------------------------
    SV5_BL_BIN      = 0x07,
    //---------------------------------
    /* Koshi, 2011.03.21,  { */
    YAFFS_IMG       = 0x08,
    /* Koshi, 2011.03.21,  } */
    MBR_BIN         = 0x09,
    
    UNKNOWN_BIN     = 0xFF
} ROM_TYPE;

typedef struct {
    char                name[64];
    U64        region_addr;
    REGION_ADDR_TYPE    addr_type;  // ABSOLUTE_ADDR -> region_addr is absolute address 
                                    // OFFSET_VALUE  -> region_addr is offset value 
    U64        begin_addr;
    U64        end_addr;
    char                filepath[256];
    unsigned int        filesize;
    unsigned short      index;

    ROM_TYPE            rom_type;

    _BOOL               enable; // _FALSE -> disbale 
                                // _TRUE  -> enable  
    /* Koshi, enable the item list in flashtool listview */                                         
    _BOOL               item_is_visable;    // _FALSE -> disbale 
                                            // _TRUE  -> enable
}ROM_INFO;

typedef struct {
    unsigned int        m_is_nfb:1;
    unsigned int        m_security_supported:1;
    unsigned int        m_cust_para_supported:1;
    unsigned int        m_system_drive_on_nand:1;
    unsigned int        m_fota_full:1;
} DL_Config_T;

typedef struct {
    DL_Config_T         m_config;
} DL_INFO;

typedef struct{
	char m_szBBChipType[256];
	char m_szStorageType[256];
}DL_PlatformInfo;

/* PC image checksum 2012/06/08 */
typedef int (__stdcall *CALLBACK_ROM_MEM_CHECKSUM_PROGRESS_INIT)(void *usr_arg, const char* image_name);
typedef int (__stdcall *CALLBACK_ROM_MEM_CHECKSUM_PROGRESS)(unsigned char finished_percentage, unsigned int finished_bytes, unsigned int total_bytes, void *usr_arg);
typedef struct {
    unsigned short index;
    unsigned int chksum;
    // Checksum progress call back
    CALLBACK_ROM_MEM_CHECKSUM_PROGRESS_INIT m_cb_rom_mem_checksum_init;
    void * m_cb_rom_mem_checksum_init_arg;
    CALLBACK_ROM_MEM_CHECKSUM_PROGRESS m_cb_rom_mem_checksum;
    void * m_cb_rom_mem_checksum_arg;
    // Stop flag
    int *p_stopflag;
}RomMemChecksumArg;
typedef struct {
    // dummy 
} RomMemChecksumResult;
/* PC image checksum 2012/06/08 */

struct DA_REPORT_T;
struct DL_HANDLE;
typedef struct DL_HANDLE *  DL_HANDLE_T;
LIBEXPORT int __stdcall DL_Create(DL_HANDLE_T  *p_dl_handle);
LIBEXPORT int __stdcall DL_Destroy(DL_HANDLE_T  *p_dl_handle);
LIBEXPORT int __stdcall DL_ImgTypeTblEntryAdd(DL_HANDLE_T dl_handle, const char *name, ROM_TYPE type);
LIBEXPORT int __stdcall DL_ImgTypeTblEntryList(DL_HANDLE_T dl_handle);
LIBEXPORT int __stdcall DL_ImgTypeTblEntryReset(DL_HANDLE_T dl_handle);
LIBEXPORT int __stdcall DL_GetPlatformInfo(DL_HANDLE_T  dl_handle,DL_PlatformInfo* pPlatformInfo);
LIBEXPORT int __stdcall DL_LoadSCIScatter(DL_HANDLE_T  dl_handle, const char *szSCI_filepath);
LIBEXPORT int __stdcall DL_LoadScatter(DL_HANDLE_T  dl_handle, const char *scatter_filepath, const char *chip_name);
LIBEXPORT int __stdcall DL_SetChecksumEnable(DL_HANDLE_T  dl_handle, bool b_enable_checksum);
LIBEXPORT int __stdcall DL_GetBMTBlockCount(DL_HANDLE_T  dl_handle, unsigned int *p_bmt_block_count);
LIBEXPORT int __stdcall DL_VerifyROMMemBuf(const DL_HANDLE_T dl_handle, 
										   const RomMemChecksumArg *p_rom_mem_check_arg,
										   RomMemChecksumResult *p_rom_mem_check_result);
LIBEXPORT int __stdcall DL_SetPacketLength(DL_HANDLE_T  dl_handle, unsigned short length);
LIBEXPORT int __stdcall DL_GetCount(DL_HANDLE_T  dl_handle, unsigned short *p_rom_count);
LIBEXPORT int __stdcall DL_GetInfo(DL_HANDLE_T  dl_handle, DL_INFO  *p_dl_info);
LIBEXPORT int __stdcall DL_IsReady(DL_HANDLE_T  dl_handle, ROM_INFO  *p_rom_info, BL_INFO  *p_bl_info, _BOOL  check_if_updated);
LIBEXPORT int __stdcall DL_GetJumpTableInfo(DL_HANDLE_T  dl_handle, MTK_JumpTable  *p_jmptbl_info);
LIBEXPORT int __stdcall DL_GetProjectId(DL_HANDLE_T  dl_handle, char *buf, unsigned int buf_len);
LIBEXPORT int __stdcall DL_GetDRAMSetting(DL_HANDLE_T  dl_handle, DRAM_SETTING  *dram_cfg, DA_REPORT_T *da_report);
LIBEXPORT int __stdcall DL_ResourceProjectIdComparisonSetting(DL_HANDLE_T  dl_handle, _BOOL  enable);
LIBEXPORT int __stdcall DL_AutoLoadByScatRegionName(DL_HANDLE_T  dl_handle, const char *full_filepath, unsigned char allow_rom_type, _BOOL is_scatter_file);
LIBEXPORT int __stdcall DL_GetBBChipType(DL_HANDLE_T  dl_handle, BBCHIP_TYPE  *p_bbchip);
LIBEXPORT int __stdcall DL_GetBBChipName(DL_HANDLE_T  dl_handle, char *buf, unsigned int buf_len);
LIBEXPORT int __stdcall DL_GetCustomName(DL_HANDLE_T  dl_handle, char *buf, unsigned int buf_len);
LIBEXPORT int __stdcall DL_Rom_GetInfo(DL_HANDLE_T  dl_handle, unsigned short index, ROM_INFO  *p_rom_info);
LIBEXPORT int __stdcall DL_Rom_GetInfoAll(DL_HANDLE_T  dl_handle, ROM_INFO  *p_rom_info, unsigned short max_rom_count);
LIBEXPORT int __stdcall DL_Rom_GetResInfo(DL_HANDLE_T  dl_handle, unsigned short index, MTK_Resource  *p_res_info);
LIBEXPORT int __stdcall DL_Rom_Load(DL_HANDLE_T  dl_handle, unsigned short index, const char *rom_filepath);
LIBEXPORT int __stdcall DL_Rom_Unload(DL_HANDLE_T  dl_handle, unsigned short index);
LIBEXPORT int __stdcall DL_Rom_UnloadAll(DL_HANDLE_T  dl_handle);
LIBEXPORT int __stdcall DL_Rom_SetEnableAttr(DL_HANDLE_T  dl_handle, unsigned short index, _BOOL  attr);
LIBEXPORT int __stdcall DL_Rom_SetEnableAttrByRomName(DL_HANDLE_T  dl_handle, const char*szRomName, _BOOL  attr);
LIBEXPORT int __stdcall DL_IsNFB(DL_HANDLE_T  dl_handle, _BOOL  *p_is_nfb);
LIBEXPORT int __stdcall DL_SetCallback(DL_HANDLE_T  dl_handle, 
                            CALLBACK_REMOTE_GET_FILE_LEN            cb_remote_get_file_len,
                            void *                                  cb_remote_get_file_len_arg,
                            CALLBACK_REMOTE_READ_FILE               cb_remote_read_file,
                            void *                                  cb_remote_read_file_arg,
                            CALLBACK_REMOTE_READ_FILE_BY_PACKAGE    cb_remote_read_file_by_package,
                            void *                                  cb_remote_read_file_by_package_arg,
                            CALLBACK_REMOTE_GET_SIG_LEN             cb_remote_get_sig_len,
                            void *                                  cb_remote_get_sig_len_arg,
                            CALLBACK_REMOTE_READ_SIG                cb_remote_read_sig,
                            void *                                  cb_remote_read_sig_arg,
                            CALLBACK_FILE_INTEGRITY_CHECK           cb_file_integrity_check,
                            void *                                  cb_file_integrity_check_arg);
// BL_HANDLE related 
LIBEXPORT int __stdcall DL_BL_IsReady(DL_HANDLE_T  dl_handle, BL_INFO  *p_bl_info, _BOOL  check_if_updated);
LIBEXPORT int __stdcall DL_BL_Load(DL_HANDLE_T  dl_handle, const char *bl_filepath);
LIBEXPORT int __stdcall DL_BL_EXT_Load(DL_HANDLE_T  dl_handle, const char *bl_ext_filepath);
LIBEXPORT int __stdcall DL_BL_GetInfo(DL_HANDLE_T  dl_handle, BL_INFO  *p_bl_info);
LIBEXPORT int __stdcall DL_BL_SetEnableAttr(DL_HANDLE_T  dl_handle, _BOOL  attr);

//------------------------------------------------------------------------------
// DL_HANDLE_LIST: Exported Functions                                           
//------------------------------------------------------------------------------
struct DL_HANDLE_LIST;
typedef struct DL_HANDLE_LIST * DL_HANDLE_LIST_T;
LIBEXPORT int __stdcall DL_CreateList(DL_HANDLE_LIST_T * dl_handle_list);
LIBEXPORT int __stdcall DL_DestroyList(DL_HANDLE_LIST_T * dl_handle_list);
LIBEXPORT int __stdcall DL_AddHandleToList(DL_HANDLE_LIST_T dl_handle_list, DL_HANDLE_T dl_handle);
LIBEXPORT int __stdcall DL_DeleteHandleFromList(DL_HANDLE_LIST_T dl_handle_list, DL_HANDLE_T dl_handle);
LIBEXPORT int __stdcall DL_GetHandleNumber(DL_HANDLE_LIST_T dl_handle_list, int * HandleNumber);
LIBEXPORT int __stdcall DL_CheckHandleInList(DL_HANDLE_LIST_T dl_handle_list, DL_HANDLE_T dl_handle);
LIBEXPORT int __stdcall DL_CheckHandleSameSWVersionInList(DL_HANDLE_LIST_T dl_handle_list, DL_HANDLE_T dl_handle);

//------------------------------------------------------------------------------
// DA_HANDLE: Exported Functions                                                
//------------------------------------------------------------------------------

typedef struct {
    char            filepath[256];
    unsigned int    start_addr;
    unsigned char   *buf;
    unsigned int    buf_len;
    unsigned int    main_prog_size;
    char            version[13];
    char            last_modified_date[256];
}DA_INFO;

/* 2010/11/15 PangYen added for JRD - start */
#define MAX_DA_REGION_NUM    5

typedef struct{
    unsigned short hw_code;
    unsigned short hw_ver;
    unsigned short sw_ver;
}CHIP_INFO;

typedef CHIP_INFO*  CHIP_INFO_T;

typedef struct {
    unsigned int   start_addr;
    unsigned char *buf;
    unsigned int   buf_len;
}DA_PARSE_INFO_REGION;

typedef struct {
    unsigned short count;
    unsigned int   jump_addr;
    DA_PARSE_INFO_REGION da_region_list[MAX_DA_REGION_NUM];
}DA_PARSE_INFO;
/* 2010/11/15 PangYen added for JRD - end */

struct DA_HANDLE;
typedef struct DA_HANDLE *      DA_HANDLE_T;
LIBEXPORT int __stdcall DA_Create(DA_HANDLE_T  *p_da_handle);
LIBEXPORT int __stdcall DA_Destroy(DA_HANDLE_T  *p_da_handle);
LIBEXPORT int __stdcall DA_IsReady(DA_HANDLE_T  da_handle, DA_INFO  *p_da_info, _BOOL  check_if_updated);
LIBEXPORT int __stdcall DA_GetInfo(DA_HANDLE_T  da_handle, DA_INFO  *p_da_info);
LIBEXPORT int __stdcall DA_SetDefaultStartAddr(DA_HANDLE_T  da_handle, unsigned int da_start_addr);
LIBEXPORT int __stdcall DA_Load(DA_HANDLE_T  da_handle, const char *da_filepath, _BOOL bDA_Validation, _BOOL bDA_has_sig);
LIBEXPORT int __stdcall DA_LoadBuf(DA_HANDLE_T  da_handle, const unsigned char *da_buf, unsigned int uDaBufLen, _BOOL bDA_Validation, _BOOL bDA_has_sig);
LIBEXPORT int __stdcall DA_Unload(DA_HANDLE_T  da_handle);
LIBEXPORT int __stdcall DA_SetCallback(DA_HANDLE_T  da_handle,
                            CALLBACK_REMOTE_GET_FILE_LEN            cb_remote_get_file_len,
                            void *                                  cb_remote_get_file_len_arg,
                            CALLBACK_REMOTE_READ_FILE               cb_remote_read_file,
                            void *                                  cb_remote_read_file_arg,
                            CALLBACK_REMOTE_READ_FILE_BY_PACKAGE    cb_remote_read_file_by_package,
                            void *                                  cb_remote_read_file_by_package_arg,
                            CALLBACK_REMOTE_GET_SIG_LEN             cb_remote_get_sig_len,
                            void *                                  cb_remote_get_sig_len_arg,
                            CALLBACK_REMOTE_READ_SIG                cb_remote_read_sig,
                            void *                                  cb_remote_read_sig_arg,
                            CALLBACK_FILE_INTEGRITY_CHECK           cb_file_integrity_check,
                            void *                                  cb_file_integrity_check_arg);
LIBEXPORT int __stdcall DA_Parsing(DA_HANDLE_T da_handle, DA_PARSE_INFO *da_parse_info, CHIP_INFO_T chip_info);

//------------------------------------------------------------------------------
// RB_HANDLE: Exported Functions                                                
//------------------------------------------------------------------------------
typedef struct {
    char                filepath[256];
    U64        readback_addr;
    unsigned int        readback_len;
    unsigned int        part_id;
    _BOOL               enable; // _FALSE -> disbale 
                                // _TRUE  -> enable  

    unsigned short      index;

    NUTL_ReadFlag_E     m_read_flag;

}RB_INFO;

struct RB_HANDLE;
typedef struct RB_HANDLE *  RB_HANDLE_T;
LIBEXPORT int __stdcall RB_Create(RB_HANDLE_T  *p_rb_handle);
LIBEXPORT int __stdcall RB_Destroy(RB_HANDLE_T  *p_rb_handle);
LIBEXPORT int __stdcall RB_SetPacketLength(RB_HANDLE_T  rb_handle, U32 length);
LIBEXPORT int __stdcall RB_GetCount(RB_HANDLE_T  rb_handle, unsigned short *p_rb_count);
LIBEXPORT int __stdcall RB_IsReady(RB_HANDLE_T  rb_handle, RB_INFO  *p_rb_info);
LIBEXPORT int __stdcall RB_GetInfo(RB_HANDLE_T  rb_handle, unsigned short index, RB_INFO  *p_rb_info);
LIBEXPORT int __stdcall RB_GetInfoAll(RB_HANDLE_T  rb_handle, RB_INFO  *p_rb_info, unsigned short max_rb_count);
LIBEXPORT int __stdcall RB_Append(RB_HANDLE_T  rb_handle, const char *rb_filepath, U64 rb_addr, unsigned int rb_len);
LIBEXPORT int __stdcall RB_Append_ReadToBuf(RB_HANDLE_T  rb_handle, U64 rb_addr, unsigned int rb_len, char *buf, unsigned int buf_len);
LIBEXPORT int __stdcall RB_Delete(RB_HANDLE_T  rb_handle, unsigned short index);
LIBEXPORT int __stdcall RB_SetFilepath(RB_HANDLE_T  rb_handle, unsigned short index, const char *rb_filepath);
LIBEXPORT int __stdcall RB_SetAddrTypeFlag(RB_HANDLE_T  rb_handle, unsigned short index, NUTL_AddrTypeFlag_E  flag);
LIBEXPORT int __stdcall RB_SetReadbackFlag(RB_HANDLE_T  rb_handle, unsigned short index, NUTL_ReadFlag_E  flag);
LIBEXPORT int __stdcall RB_SetReadbackAddr(RB_HANDLE_T  rb_handle, unsigned short index, U64 rb_addr);
LIBEXPORT int __stdcall RB_SetReadbackLength(RB_HANDLE_T  rb_handle, unsigned short index, unsigned int rb_len);
LIBEXPORT int __stdcall RB_SetReadbackPartition(RB_HANDLE_T rb_handle, unsigned short index, unsigned int part_id);
LIBEXPORT int __stdcall RB_ClearAll(RB_HANDLE_T  rb_handle);
LIBEXPORT int __stdcall RB_SetEnableAttr(RB_HANDLE_T  rb_handle, unsigned short index, _BOOL  attr);
LIBEXPORT int __stdcall RB_EnableReadbackFAT(RB_HANDLE_T  rb_handle, unsigned short index, _BOOL  attr);
LIBEXPORT int __stdcall RB_SetNANDInfo(RB_HANDLE_T  rb_handle, unsigned int page_len, unsigned int spare_len);

//------------------------------------------------------------------------------
// IMGDEC_HANDLE: Exported Functions                                              
//------------------------------------------------------------------------------
#define AES_KEY_SIZE    (32) // bytes

typedef struct {
    unsigned char       m_aes_key[AES_KEY_SIZE];
    bool                m_force_dec;
} IMGDEC_INFO;

struct IMGDEC_HANDLE;
typedef struct IMGDEC_HANDLE *    IMGDEC_HANDLE_T;
LIBEXPORT int __stdcall IMGDEC_Create(IMGDEC_HANDLE_T  *p_imgdec_handle);
LIBEXPORT int __stdcall IMGDEC_Destroy(IMGDEC_HANDLE_T  *p_imgdec_handle);
LIBEXPORT int __stdcall IMGDEC_IsReady(IMGDEC_HANDLE_T  imgdec_handle, IMGDEC_INFO  *p_imgdec_info, _BOOL  bCheckIfUpdated);
LIBEXPORT int __stdcall IMGDEC_GetInfo(IMGDEC_HANDLE_T  imgdec_handle, IMGDEC_INFO  *p_imgdec_info);
LIBEXPORT int __stdcall IMGDEC_Load(IMGDEC_HANDLE_T  imgdec_handle, const char *imgdec_filepath);
LIBEXPORT int __stdcall IMGDEC_Unload(IMGDEC_HANDLE_T  imgdec_handle);

//------------------------------------------------------------------------------
// AUTH_HANDLE: Exported Functions                                              
//------------------------------------------------------------------------------

typedef struct {
    unsigned int        m_version;
    char                m_filepath[512];
    unsigned char       m_secure_custom_name[32];
    unsigned char       m_description[512];
} AUTH_INFO;

struct AUTH_HANDLE;
typedef struct AUTH_HANDLE *    AUTH_HANDLE_T;
LIBEXPORT int __stdcall AUTH_Create(AUTH_HANDLE_T  *p_auth_handle);
LIBEXPORT int __stdcall AUTH_Destroy(AUTH_HANDLE_T  *p_auth_handle);
LIBEXPORT int __stdcall AUTH_IsReady(AUTH_HANDLE_T  auth_handle, AUTH_INFO  *p_auth_info, _BOOL  bCheckIfUpdated);
LIBEXPORT int __stdcall AUTH_GetInfo(AUTH_HANDLE_T  auth_handle, AUTH_INFO  *p_auth_info);
LIBEXPORT int __stdcall AUTH_Load(AUTH_HANDLE_T  auth_handle, const char *auth_filepath);
LIBEXPORT int __stdcall AUTH_Unload(AUTH_HANDLE_T  auth_handle);
LIBEXPORT int __stdcall AUTH_SetCallback(AUTH_HANDLE_T  auth_handle, 
                            CALLBACK_REMOTE_GET_FILE_LEN            cb_remote_get_file_len,
                            void *                                  cb_remote_get_file_len_arg,
                            CALLBACK_REMOTE_READ_FILE               cb_remote_read_file,
                            void *                                  cb_remote_read_file_arg,
                            CALLBACK_REMOTE_READ_FILE_BY_PACKAGE    cb_remote_read_file_by_package,
                            void *                                  cb_remote_read_file_by_package_arg,
                            CALLBACK_REMOTE_GET_SIG_LEN             cb_remote_get_sig_len,
                            void *                                  cb_remote_get_sig_len_arg,
                            CALLBACK_REMOTE_READ_SIG                cb_remote_read_sig,
                            void *                                  cb_remote_read_sig_arg,
                            CALLBACK_FILE_INTEGRITY_CHECK           cb_file_integrity_check,
                            void *                                  cb_file_integrity_check_arg);

//------------------------------------------------------------------------------
// SCERT_HANDLE: Exported Functions                                              
//------------------------------------------------------------------------------

typedef struct {
    unsigned int        m_version;
    char                m_filepath[512];
    unsigned char       m_secure_custom_name[32];
    unsigned char       m_description[512];
} SCERT_INFO;

struct SCERT_HANDLE;
typedef struct SCERT_HANDLE *   SCERT_HANDLE_T;
LIBEXPORT int __stdcall SCERT_Create(SCERT_HANDLE_T  *p_scert_handle);
LIBEXPORT int __stdcall SCERT_Destroy(SCERT_HANDLE_T  *p_scert_handle);
LIBEXPORT int __stdcall SCERT_IsReady(SCERT_HANDLE_T  scert_handle, SCERT_INFO  *p_scert_info, _BOOL  bCheckIfUpdated);
LIBEXPORT int __stdcall SCERT_GetInfo(SCERT_HANDLE_T  scert_handle, SCERT_INFO  *p_scert_info);
LIBEXPORT int __stdcall SCERT_Load(SCERT_HANDLE_T  cert_handle, const char *cert_filepath);
LIBEXPORT int __stdcall SCERT_Unload(SCERT_HANDLE_T  cert_handle);
LIBEXPORT int __stdcall SCERT_SetCallback(SCERT_HANDLE_T  scert_handle, 
                            CALLBACK_REMOTE_GET_FILE_LEN            cb_remote_get_file_len,
                            void *                                  cb_remote_get_file_len_arg,
                            CALLBACK_REMOTE_READ_FILE               cb_remote_read_file,
                            void *                                  cb_remote_read_file_arg,
                            CALLBACK_REMOTE_READ_FILE_BY_PACKAGE    cb_remote_read_file_by_package,
                            void *                                  cb_remote_read_file_by_package_arg,
                            CALLBACK_REMOTE_GET_SIG_LEN             cb_remote_get_sig_len,
                            void *                                  cb_remote_get_sig_len_arg,
                            CALLBACK_REMOTE_READ_SIG                cb_remote_read_sig,
                            void *                                  cb_remote_read_sig_arg,
                            CALLBACK_FILE_INTEGRITY_CHECK           cb_file_integrity_check,
                            void *                                  cb_file_integrity_check_arg);
//------------------------------------------------------------------------------
// PART_HANDLE: Exported Functions                                              
//------------------------------------------------------------------------------
typedef struct PART_HANDLE* PART_HANDLE_T;
typedef struct {
    char                name[64];
    U64        begin_addr;
    U64        image_length;
} PART_INFO;
extern int PART_Create(PART_HANDLE_T* p_part_handle, void* handle);
extern int PART_Destroy(PART_HANDLE_T * p_part_handle);
extern int PART_GetCount(PART_HANDLE_T part_handle, unsigned int *p_part_count);
extern int PART_GetInfo(PART_HANDLE_T part_handle, PART_INFO *p_part_info, unsigned int max_part_count);

#ifdef  __cplusplus
}
#endif

#endif

