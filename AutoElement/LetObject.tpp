#include "LetManager.h"
#include "LetObject.h"

#define thisPtr \
LetObject* _this = this;\
while (true) {\
	if ((_this->Attribute & (int)NatureType::ptr) == true) {\
		_this = (LetObject*)*(unsigned long long*)_this->m_block->ptr;\
		continue;\
	}\
	else {\
		break;\
	}\
}

#define nodePtr \
while((node->Attribute & (int)NatureType::ptr) == true)\
{ node = (LetObject*)*(unsigned long long*)node->m_block->ptr;}

#define _this _this

using namespace AutoMem::Obj;

template<typename castT>
inline castT LetObject::cast(LetObject&node)
{
	return cast<castT>(&node);
}

template<typename castT>
inline castT LetObject::cast(LetObject* node)
{
	nodePtr;

	castT ret;
	do
	{
		OBJ_IF_BREAK(!node->m_block);
		ret = *(castT*)node->m_block->ptr;
	} while (false);

	return ret;
}

template<>
inline std::string LetObject::cast<std::string>(LetObject& node) {
	return cast<std::string>(&node);
}

template<>
inline std::string LetObject::cast<std::string>(LetObject* node) {
	nodePtr;

	std::string ret;
	do
	{
		OBJ_IF_BREAK(node->m_objType.type != ObjT::string);
		ret.append(node->m_block->ptr);
	} while (false);

	return ret;
}

template<typename T>
inline LetObject& LetObject::operator[](T val)
{
	thisPtr;

	auto selfS = _this->m_objType;
	OBJ_IF_CALL(selfS.type == ObjT::null, _this->create(StaticB));
	_this->m_letM->realloc_L(&_this->m_block, 8);

	LetObject* ret = nullptr;
	ret = &_this->m_num_array[(double)val];

	_this->setType({ ObjT::array,ObjG::Static });
	return *ret;
}

template<>
inline LetObject& LetObject::operator[]<std::string>(std::string val)
{
	thisPtr;

	auto selfS = _this->m_objType;
	OBJ_IF_CALL(selfS.type == ObjT::null, _this->create(StaticB));
	_this->m_letM->realloc_L(&_this->m_block, 8);

	LetObject* ret = nullptr;
	ret = &_this->m_str_array[val];

	_this->setType({ ObjT::array,ObjG::Static });
	return *ret;
}

template<typename T>
inline bool LetObject::operator>>(T& type) {

	thisPtr;
	if (_this->m_objType.group != ObjG::none) {
		type = *(T*)_this->m_block->ptr;
		return true;
	}

	return false;
}

template<>
inline bool LetObject::operator>><std::string>(std::string& val)
{
	thisPtr;
	val.clear();
	if (_this->m_objType.group == ObjG::Dynamic) {
		val.append(_this->m_block->ptr);
		return true;
	}

	return false;
}