#include "stdafx.h"
#include "IDtest.h"
#include <windows.h>
#include "md5.h"
#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")

CString _getWmiInfo(IWbemClassObject *pClassObject, LPCTSTR lpszName)
{
 
            CComVariant varValue ;
			CComBSTR bstrName(lpszName);
 			CString str ;
			if( pClassObject->Get( bstrName , 0 , &varValue , NULL , 0 ) == S_OK )
			{

				// �Բ�ͬ���������ͷֱ���
				if( varValue.vt == VT_BSTR )	//�ַ���
				{
					str = CString(varValue.bstrVal) ;
				}
				else 
					if( varValue.vt == VT_ARRAY ) //����
					{
						long iLowBound = 0 , iUpBound = 0 ;
                        SafeArrayGetLBound( varValue.parray , 1 , &iLowBound ) ;
                        SafeArrayGetUBound( varValue.parray , 1 , &iUpBound ) ;
						for( long j = iLowBound ; j <= iUpBound ; j ++ )
						{
							VARIANT *pvar = NULL ;
                            long temp = j ;
							if( SafeArrayGetElement( varValue.parray , &temp , pvar ) == S_OK && pvar )
							{
								CComVariant varTemp ;
                                if( varTemp.ChangeType( VT_BSTR , pvar ) == S_OK )
                                {
									if( !str.IsEmpty() )
										str += _T(",") ;
									str += varTemp.bstrVal ;
								} //if( varTemp...
							} //if( SafeArrayGet...
						} //for( long j=iLowBound;...
					} //if (varValue.vt ...
					else
					{
						switch( varValue.vt)
						{
						case VT_I4:
							str.Format("%d", varValue.lVal);
							break;
						case VT_I8:
							str.Format("%l", varValue.llVal);
							break;
						case VT_I2:
							str.Format("%d", varValue.iVal);
							break;
						case VT_UI1:
							str.Format("%uc", varValue.bVal);
							break;
						case VT_R4:
							str.Format("%f", varValue.fltVal);
							break;
						case VT_R8:
							str.Format("%lf", varValue.dblVal);
							break;
						case VT_I1:
							str.Format("%c", varValue.cVal);
							break;
						case VT_UI2:
							str.Format("%ud", varValue.uiVal);
							break;
						case VT_UI4:
							str.Format("%ud", varValue.ulVal);
							break;
						case VT_UI8:
							str.Format("%ul", varValue.ullVal);
							break;
						case VT_BOOL:
							cout<<"bool***************\n";
							break;
						case VT_NULL:
							str = "NULL";
							break;
						default:
                            if( varValue.ChangeType( VT_BSTR ) == S_OK )
								str = varValue.bstrVal ;
							else
								cout<<"Cannot handle the the data type of "<<CString(bstrName)<<": "<<varValue.vt<<endl;
						}
					}
					
					// ��ӡ��Ӳ����Ϣ
					//cout<<CString(bstrName)<<" = "<<str<<endl;
			}
			else
				str = "";

			return str;
}

 
CString GetSeiralNumberOrID( LPCTSTR lpszClass, LPCTSTR lpszName)
{
	CString str("");
	// ��ʼ��COMģ��
    CoUninitialize();
    HRESULT hres= CoInitialize(NULL);
	if ( hres != S_OK)
		return str;	

    // ��ȡ���� WMI Ȩ��

    if( CoInitializeSecurity( NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE,NULL, EOAC_NONE, 0 ) == S_OK )
	{
		// ͨ�� IWbemLocator �� IWbemServices ������ COM �ӿڷ��� WMI, ��ȡϵͳ��Ϣ
		
		CComPtr<IWbemLocator> spWbemLocator ;
		if( spWbemLocator.CoCreateInstance( CLSID_WbemAdministrativeLocator , 0 , CLSCTX_INPROC_SERVER| CLSCTX_LOCAL_SERVER ) == S_OK )
		{
			CComPtr<IWbemServices> spWbemServices ;
			if( spWbemLocator->ConnectServer( L"root\\cimv2" , NULL, NULL, NULL, 0, NULL, NULL, &spWbemServices ) == S_OK )
			{
				// ����ָ����Ϣ

				USES_CONVERSION ;
				CComPtr<IEnumWbemClassObject> spEnumWbemClassObject ;
				CComBSTR bstrQuery ( L"Select * from " ) ;
				bstrQuery += T2OLE(lpszClass) ;
				if( spWbemServices->ExecQuery( L"WQL" , bstrQuery , WBEM_FLAG_RETURN_IMMEDIATELY , NULL, &spEnumWbemClassObject ) == S_OK )
				{
					ULONG uCount = 1, uReturned;
					CComPtr<IWbemClassObject> spClassObject ;
					if( spEnumWbemClassObject->Reset()  == S_OK )
					{
						int iEnumIdx = 0;
						while( spEnumWbemClassObject->Next( WBEM_INFINITE,uCount, &spClassObject, &uReturned) == S_OK )
						{
							str = _getWmiInfo( spClassObject, lpszName) ;
							spClassObject.Release() ;
						}
					}
				}
			}
		}
	}

    CoUninitialize();

	return str;
}



char * CreateKey(char *pSRC)
{
	char *pDST;
	pDST = MD5String(pSRC);
	return pDST;
}

bool ID_test(const char* license)
{
    CString BoardID = GetSeiralNumberOrID("Win32_BaseBoard", "SerialNumber");
    CString ProcessorID = GetSeiralNumberOrID("Win32_Processor", "ProcessorID");
    CString MemoryID = GetSeiralNumberOrID("Win32_PhysicalMemory", "SerialNumber");
    CString HardDiskID = GetSeiralNumberOrID("Win32_DiskDrive", "PNPDeviceID");
    cout << "BoardID: " << BoardID << endl;	// �װ�
    cout << "ProcessorID: " << ProcessorID << endl;	// CPU
    cout << "MemoryID: " << MemoryID << endl;	// �ڴ�
    cout << "HardDiskID: " << HardDiskID << endl;	// Ӳ��

    CString MachineID = BoardID + ProcessorID + MemoryID + HardDiskID;
    
    if(MachineID == "")
    {
		AfxMessageBox("���û����ȷ�İ�װ�����ļ����������������ṩ����ϵ");
    }
	char *ID_CODE = CreateKey(MachineID.GetBuffer());
	ID_CODE[10] = '\0';
	char *LICENSE_KEY = CreateKey(ID_CODE);

	if(strcmp(LICENSE_KEY,license) != 0)
	{
		char msg[200];
		sprintf_s(msg, 100, "��ʹ�õ����δͨ����֤���뽫�����û����ṩ������ʯ���ԿƼ���%s", ID_CODE);
		MessageBox(NULL, msg,"�����Ҫע��",MB_OK|MB_ICONEXCLAMATION);
        delete ID_CODE;
        delete LICENSE_KEY;
		return 0;
	}
    else
    {
        delete ID_CODE;
        delete LICENSE_KEY;
        return 1;
    }

}