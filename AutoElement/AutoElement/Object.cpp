#include "Object.h"
#include <sstream>
using namespace Cervice::Obj;

/**********************
* class Object
* 所有变量的基类
* 负责调度内存使用和回收
**********************/

unsigned long Object::m_MemoryLength = 0;
char* Object::m_GlobalMemery = nullptr;
std::vector<MemPoint>s*