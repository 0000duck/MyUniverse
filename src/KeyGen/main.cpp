
#include <stdio.h>
#include "../Ktmfc/md5.h"
#include <Windows.h>

void main()
{
	printf("������10λ���û���:");
	char s[100];
	do
	{
		gets_s(s,100);
	}
	while(strlen(s) != 10);

	printf("\n���֤����Ϊ��%s\n",MD5String(s));
	system("pause");

}