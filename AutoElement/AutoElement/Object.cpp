#include "Object.h"
#include <sstream>
using namespace Cervice::Obj;

/**********************
* class Object
* ���б����Ļ���
* ��������ڴ�ʹ�úͻ���
**********************/

unsigned long Object::m_MemoryLength = 0;
char* Object::m_GlobalMemery = nullptr;
std::vector<MemPoint>s*