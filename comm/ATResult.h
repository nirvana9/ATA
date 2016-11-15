/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 * ATResult.h
 *
 * Project:
 * --------
 *   Maui_SW factory hardware testing tool.
 *
 * Description:
 * ------------
 *	 This module implement the AT Command respond's results.
 *	
 * Author:
 * -------
 * Spancer 	(mtk00264)
 *
 *==============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#ifndef _CATRESULT_H
#define _CATRESULT_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;



typedef enum AtParaTypeTag
{	
	AT_OMIT,
	AT_STRING,
	AT_INTERVAL,
	AT_INTEGER,
	AT_PARA_LIST
}
AtParaType;

typedef enum
{
	AT_SUCCESS,
	AT_TIMEOUT,
	AT_WRITE_FAIL,
	AT_PARSE_FAIL
}
AtResultType;

class ATParamElem
{
public:
	AtParaType  type;
	string		str_value;				//string type, maybe pdu
	char        str_char[1024*10];
	int         str_len;
	int			int_value;				//value type
	int int_range_begin, int_range_end;	//range type
   
	vector<ATParamElem> paramLst;		//vector type
public:
	ATParamElem();
	~ATParamElem();

	friend ostream& operator << (ostream& s, ATParamElem& elem);
	friend istream& operator >> (istream& s, ATParamElem& elem);

};


class ATParamLst
{
public:
    vector<ATParamElem> eleLst;

public:
	ATParamLst();
	~ATParamLst();

	friend ostream& operator << (ostream& s, ATParamLst& lst);
	friend istream& operator >> (istream& s, ATParamLst& lst);

};


class ATResult
{
public:
	string	expectedCommand;
	AtResultType   result;	
	vector<ATParamLst> resultLst;

public:


	ATResult();
	~ATResult();
    void clear(){expectedCommand = ""; resultLst.clear();}

#if 0
	friend istream&  operator>> (istream& s, ATResult& rt);
	friend ostream&  operator<< (ostream& s, ATResult& rt);
#endif
};


#endif
