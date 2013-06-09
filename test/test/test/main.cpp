#include "opencv.hpp"
#include "libxml\tree.h"
#include "libxml\parser.h"

void main()
{
    xmlDocPtr doc; //�ļ�ָ��
   xmlNodePtr cur;//�ڵ�ָ��
   doc = xmlParseFile("b.xml");
   cur = xmlDocGetRootElement(doc);
   if(xmlStrcmp(cur->name, (const xmlChar*)"root") )
   {
     xmlFreeDoc(doc);
     return;
   }
   cur = cur->xmlChildrenNode;
   //parseNode(doc, cur);//����ڵ�
   //xmlSaveFormatFileEnc("tconfig.xml", doc, "UTF-8", 1 );//�����޸�
   xmlFreeDoc(doc); //�ͷ��ڴ�
   xmlCleanupParser();
   xmlMemoryDump();
   return;
	system("pause");
}
