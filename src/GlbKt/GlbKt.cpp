// GlbKt.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "GlbKt.h"
#include "IDtest.h"
#include <fstream>

using namespace std;

GLBKT_API void glbKt(void)
{
    fstream file;
    file.open("license.dat");
    char BiosID[256];
    file >> BiosID; 

    ID_test(BiosID);

	return;
}


