// Ktmfc.h : Ktmfc DLL ����ͷ�ļ�
//

#pragma once

//#ifndef __AFXWIN_H__
//	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
//#endif

#include "resource.h"		// ������

#ifdef KT_EXPORTS
#define KT_API __declspec(dllexport)
#else
#define KT_API __declspec(dllimport)
#endif

KT_API bool glbKtMFC(void);//Key test
