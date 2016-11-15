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
 * ATResult.cpp
 *
 * Project:
 * --------
 *   Maui_SW - phone suite
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

#include "ATResult.h"


ATParamElem::ATParamElem()
{

}


ATParamElem::~ATParamElem()
{

}

#if 0
ostream& operator << (ostream& s, ATParamElem& elem)
{
	switch(elem.type)
	{
	case AT_OMIT:
		cout << " ";
		break;

	case AT_STRING:
		cout << elem.str_value;
		break;

	case AT_INTERVAL:
		cout << "( " << elem.int_range_begin << "-" << elem.int_range_end << ")" ;
		break;

    case AT_INTEGER:
		cout << elem.int_value;
		break;

	case AT_PARA_LIST:
		cout << "( ";

		vector<ATParamElem>::iterator iter;
		iter= elem.paramLst.begin();

		for(iter; iter!=elem.paramLst.end(); iter++)
			cout << *iter;

		cout << ")";
		break;

	default:
		break;

	}
	cout << ",";
	return s;
}
#endif

ATParamLst::ATParamLst()
{
}

ATParamLst::~ATParamLst()
{
}


#if 0
ostream& operator << (ostream& s, ATParamLst& lst)
{

	vector<ATParamElem>::iterator iter, iterB, iterE;

	iterB = lst.eleLst.begin();
	iterE = lst.eleLst.end();

	for ( iter = iterB; iter!=iterE; iter++)
		cout << *iter;
		
	return s;
}



istream& operator >> (istream& s, ATParamLst& lst)
{

	return s;
}
#endif


ATResult::ATResult()
{

}


ATResult::~ATResult()
{

}

#if 0
istream&  operator>> (istream& s, ATResult& rt)
{
	

	return s;
}
	

ostream&  operator<< (ostream& s, ATResult& rt)
{
	vector<ATParamLst>::iterator iter;
	
	iter = rt.resultLst.begin();

#ifdef SP_DEBUG
	cout << "AT Result:\n";
	cout << "expect commmand: "<< rt.expectedCommand << endl;
	cout << "result:"  << rt.result << endl;
#endif

	for( iter; iter!=rt.resultLst.end(); iter++)
		cout << *iter << endl;

	return s;
}
#endif