#include "LetManager.h"
#include "LetObject.h"
#include <string>
#include <functional>
#include <algorithm>

using namespace Cervice::Obj;

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

// 常量赋值
LetObject* LetObject::operator<<(long long int num)
{
	return operator<<((long double)num);
}

LetObject* LetObject::operator<<(int num)
{
	return operator<<((long long int)num);
}

LetObject* LetObject::operator<<(long double num)
{
	thisPtr;

	OBJ_IF_CALL(!_this->m_block, _this->create(StaticB));
	_this->changeBlock(StaticB);

	auto self = _this->m_block->ptr;
	*(doublePtr)self = num;

	_this->setType({ ObjT::number, ObjG::Static });
	return _this;
}

LetObject* LetObject::operator<<(double num)
{
	return operator<<((long double)num);
}

LetObject* LetObject::operator<<(bool num)
{
	thisPtr;

	OBJ_IF_CALL(!_this->m_block, _this->create(StaticB));
	_this->changeBlock(StaticB);

	auto self = _this->m_block->ptr;
	*(boolPtr)self = num;

	_this->setType({ ObjT::boolean, ObjG::Static });
	return _this;
}

LetObject* LetObject::operator<<(const char* str)
{
	thisPtr;

	size_t lens = strnlen_s(str, UINT64_MAX);
	OBJ_IF_CALL(!_this->m_block, _this->create(DynamicB));

	_this->changeBlock(DynamicB);
	if (lens > _this->m_block->lens) {
		_this->m_letM->realloc_L(&_this->m_block, lens);
	}

	memcpy(_this->m_block->ptr, str, lens);

	_this->m_block->ptr[lens] = '\0';
	_this->m_block->ptr[_this->m_block->lens] = '\0';

	_this->setType({ ObjT::string, ObjG::Dynamic });
	return _this;
}

LetObject* LetObject::operator<<(std::string str)
{
	return operator<<(str.c_str());
}

// 变量赋值
LetObject* LetObject::operator=( LetObject* node)
{
	thisPtr;
	nodePtr;

	do 
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IFB_CALL(otheS.type == ObjT::null, _this->released());
		OBJ_IF_CALL(selfS.type == ObjT::null, _this->create(otheS.group == ObjG::Static ? true : false));

		if (node->m_block->lens > _this->m_block->lens) {
			_this->m_letM->realloc_L(&_this->m_block, node->m_block->lens);
		}

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		_this->m_num_array.clear();
		_this->m_str_array.clear();
		
		for (auto& i : node->m_num_array) {
			_this->m_num_array[i.first] = i.second;
		}

		for (auto& i : node->m_str_array) {
			_this->m_str_array[i.first] = i.second;
		}

		
		switch (otheS.type)
		{
		case LetObject::ObjT::undef:
			break;
		case LetObject::ObjT::boolean:
		case LetObject::ObjT::number:
			*(doublePtr)self = *(doublePtr)othe;
			break;
		case LetObject::ObjT::string: {
			size_t rlens = strnlen_s(othe, otheL);
			memcpy(self, othe, rlens);
			self[rlens] = '\0';
			self[_this->m_block->lens] = '\0';
		}
		default:
			break;
		}

		_this->setType(node->m_objType);

	}
	while (false);

	_this->Attribute = node->Attribute;
	return _this;
}

LetObject* LetObject::operator=( LetObject& node)
{
	return operator=(&node);
}

LetObject* LetObject::operator=(LetObject&& node)
{
	return operator=(&node);
}


LetObject& LetObject::operator[]( LetObject* node)
{
	thisPtr;
	nodePtr;

	if (node == nullptr || 
		node->m_objType.group == ObjG::none || 
		node->m_objType.type == ObjT::null || 
		node->m_objType.type == ObjT::undef) {
		throw("index only can take Number or String");
	}

	LetObject* ret = nullptr;

	auto selfS = _this->m_objType;
	OBJ_IF_CALL(selfS.type == ObjT::null, _this->create(StaticB));

	_this->m_letM->realloc_L(&_this->m_block, 8);

	auto nodeS = node->m_objType;
	if (nodeS.group == ObjG::Static) {
		ret = &_this->m_num_array[cast<numberT>(const_cast<LetObject*>(node))];
	}
	else if (nodeS.group == ObjG::Dynamic) {
		ret = &_this->m_str_array[std::move(cast<std::string>(const_cast<LetObject*>(node)))];
	}

	_this->setType({ ObjT::array,ObjG::Static });

	return *ret;
}

LetObject& LetObject::operator[]( LetObject& node)
{
	return operator[](&node);
}

LetObject& LetObject::operator[](LetObject&& node)
{
	return operator[](&node);
}

LetObject& LetObject::operator[](const char* cstr)
{
	std::string str;
	str.append(cstr);
	return operator[]<std::string>(str);
}

LetObject LetObject::operator+( LetObject& node)
{
	return operator+(&node);
}

LetObject Cervice::Obj::LetObject::operator!()
{
	LetObject ret(false, false);
	ret << false;

	thisPtr;

	do
	{
		auto selfS = _this->m_objType;

		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto selfST = selfS.type;
		auto self = _this->m_block->ptr;

		switch (selfS.type)
		{
		case LetObject::ObjT::boolean:
		{
			ret = _this;
		}
		break;
		case LetObject::ObjT::number:
		{
			if (*(doublePtr)self == 0.0) {
				ret << false;
			}
			else {
				ret << true;
			}
		}
		break;
		case LetObject::ObjT::string:
		{
			size_t lensS = strnlen_s(self, selfL);
			if (lensS != 0) {
				ret << true;
			}
			else {
				ret << false;
			}
		}
		break;
		case LetObject::ObjT::array:
		{
			if (_this->m_num_array.empty() && _this->m_str_array.empty()) {
				ret << false;
			}
			else {
				ret << true;
			}
		}
		break;
		default:
			throw("unknow value type ,its can not take ! operator");
			break;
		}

		ret.setType({ ObjT::boolean , ObjG::Static });

	} while (false);

	ret << !LetObject::cast<bool>(ret);

	return std::move(ret);
}

LetObject Cervice::Obj::LetObject::operator~()
{
	LetObject ret(false, false);
	ret << 0;

	thisPtr;

	do
	{
		auto selfS = _this->m_objType;

		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto selfST = selfS.type;
		auto self = _this->m_block->ptr;

		switch (selfS.type)
		{
		case LetObject::ObjT::boolean:
		case LetObject::ObjT::number:
		{
			long long int result = (long long int)(*(doublePtr)self);
			result = ~result;
			(*(doublePtr)ret.m_block->ptr) = (numberT)result;
		}
		break;
		default:
			throw("unknow value type ,its can not take ~ operator");
			break;
		}

		ret.setType(_this->m_objType);

	} while (false);

	return std::move(ret);
}

LetObject LetObject::operator+(LetObject&& node)
{
	return operator+(&node);
}

LetObject LetObject::operator+( LetObject* node)
{
	LetObject ret(false, false);
	
	thisPtr;
	nodePtr;

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型相加 数字 字符串 数组
		if (_this->isSameAs(selfST, otheST) ||
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				ret.create(StaticB);
				auto result = ret.m_block->ptr;
				*(doublePtr)result = *(doublePtr)self + *(doublePtr)othe;
			}
			break;
			case LetObject::ObjT::string:
			{
				size_t lensS = strnlen_s(self, selfL);
				size_t lensO = strnlen_s(othe, otheL);
				ret.create(DynamicB);
				if (lensS + lensO > ret.m_block->lens) {
					_this->m_letM->realloc_L(&ret.m_block, lensS + lensO);
				}
				auto result = ret.m_block->ptr;
				result = ret.m_block->ptr;
				memcpy(result, self, lensS);
				memcpy(result + lensS, othe, lensO);
				result[lensS + lensO] = '\0';
				result[ret.m_block->lens] = '\0';
			}
			break;
			case LetObject::ObjT::array:
			{
				ret.create(StaticB);
				for (auto& i : _this->m_num_array)
					ret.m_num_array[i.first] = i.second;
				for (auto& i : _this->m_str_array)
					ret.m_str_array[i.first] = i.second;
				for (auto& i : node->m_num_array)
					ret.m_num_array[i.first] = i.second;
				for (auto& i : node->m_str_array)
					ret.m_str_array[i.first] = i.second;
			}
			break;
			default:
				break;
			}
			ret.setType(_this->m_objType);
		}

		// 字符与数字相加
		else if (_this->isLess(otheST, ObjT::string) || _this->isLess(selfST, ObjT::string) &&
			(_this->isLess(otheST, ObjT::array) && _this->isLess(selfST, ObjT::array)))
		{
			Block* numB = isLess(selfST, ObjT::string) ? _this->m_block : node->m_block;
			Block* strB = isLess(selfST, ObjT::string) ?  node->m_block : _this->m_block;
			bool whoFirst =  (strB == m_block) ? true : false;

			std::string numC;
			numC = std::to_string(*(doublePtr)numB->ptr);

			size_t oneBT, twoBT;

			size_t offset = 0; auto iter = numC.rbegin();
			while (iter != numC.rend() && (*iter == '0' || *iter == '.')) {
				if (*iter == '.') {
					offset++; iter++;
					break;
				}
				offset++; iter++;
				continue;
			}

			numC = numC.substr(0,numC.size() - offset);
			oneBT = numC.length();

			twoBT = strnlen_s(strB->ptr, strB->lens);

			ret.create(DynamicB);
			if (oneBT + twoBT > ret.m_block->lens) {
				_this->m_letM->realloc_L(&ret.m_block, oneBT + twoBT);
			}
			
			auto result = ret.m_block->ptr;
			result = ret.m_block->ptr;
			if (whoFirst) {
				memcpy(result, strB->ptr, twoBT);
				memcpy(result + twoBT, numC.c_str(), oneBT);
			}
			else {
				memcpy(result, numC.c_str(), oneBT);
				memcpy(result + oneBT, strB->ptr, twoBT);
			}
			result[oneBT+ twoBT] = '\0';
			result[ret.m_block->lens] = '\0';

			ret.setType({ ObjT::string,ObjG::Dynamic });
		}
		else {
			throw("Object : cant take Add");
		}

	} while (false);

	return std::move(ret);
}

LetObject LetObject::operator-( LetObject& node)
{
	return operator-(&node);
}

LetObject LetObject::operator-(LetObject&& node)
{
	return operator-(&node);
}

LetObject LetObject::operator-( LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型相减 数字
		if (_this->isSameAs(selfST, otheST) &&
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				ret.create(StaticB);
				auto result = ret.m_block->ptr;
				*(doublePtr)result = *(doublePtr)self - *(doublePtr)othe;
			}
			break;
			default:
				break;
			}
			ret.setType(_this->m_objType);
		}
		else {
			throw("Object : cant take Sub");
		}

	} while (false);

	return std::move(ret);
}

LetObject LetObject::operator*(LetObject&& node)
{
	return operator*(&node);
}

LetObject LetObject::operator*( LetObject& node)
{
	return operator*(&node);
}

LetObject LetObject::operator*( LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型相乘 数字
		if (_this->isSameAs(selfST, otheST) &&
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				ret.create(StaticB);
				auto result = ret.m_block->ptr;
				*(doublePtr)result = *(doublePtr)self * *(doublePtr)othe;
			}
			break;
			default:
				break;
			}
			ret.setType(_this->m_objType);
		}
		else {
			throw("Object : cant take Mul");
		}

	} while (false);

	return std::move(ret);
}

LetObject LetObject::operator/( LetObject& node)
{
	return operator/(&node);
}

LetObject LetObject::operator/(LetObject&& node)
{
	return operator/(&node);
}

LetObject LetObject::operator/( LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型相乘 数字
		if (_this->isSameAs(selfST, otheST) &&
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				ret.create(StaticB);
				auto result = ret.m_block->ptr;
				OBJ_IFB_CALL(*(doublePtr)self == 0.0, ret.released());
				*(doublePtr)result = *(doublePtr)self / *(doublePtr)othe;
			}
			break;
			default:
				break;
			}
			ret.setType(_this->m_objType);
		}
		else {
			throw("Object : cant take Div");
		}

	} while (false);

	return std::move(ret);
}

// %

LetObject Cervice::Obj::LetObject::operator%(LetObject&&node)
{
	return operator%(&node);
}

LetObject Cervice::Obj::LetObject::operator%(LetObject& node)
{
	return operator%(&node);
}


LetObject Cervice::Obj::LetObject::operator%(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 数字 mod
		if (_this->isSameAs(selfST, otheST) &&
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				ret.create(StaticB);
				auto result = ret.m_block->ptr;
				OBJ_IFB_CALL(*(doublePtr)self == 0.0, ret.released());
				*(doublePtr)result = fmodl(*(doublePtr)self, *(doublePtr)othe);
			}
			break;
			default:
				break;
			}
			ret.setType(_this->m_objType);
		}
		else {
			throw("Object : cant take Mod");
		}

	} while (false);

	return std::move(ret);
}

// <<

LetObject Cervice::Obj::LetObject::operator<<(LetObject&& node)
{
	return operator<<(&node);
}

LetObject Cervice::Obj::LetObject::operator<<(LetObject& node)
{
	return operator<<(&node);
}

LetObject Cervice::Obj::LetObject::operator<<(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 数字 左位移
		if (_this->isSameAs(selfST, otheST) &&
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				ret.create(StaticB);
				auto result = ret.m_block->ptr;
				long long int one = (long long int)*(doublePtr)self;
				long long int two = (long long int)*(doublePtr)othe;
				long long int three = one << two;
				*(doublePtr)result = (numberT)three;
			}
			break;
			default:
				break;
			}
			ret.setType(_this->m_objType);
		}
		else {
			throw("Object : cant take <<");
		}

	} while (false);

	return std::move(ret);
}

// >>

LetObject Cervice::Obj::LetObject::operator>>(LetObject&& node)
{
	return operator>>(&node);
}

LetObject Cervice::Obj::LetObject::operator>>(LetObject& node)
{
	return operator>>(&node);
}

LetObject Cervice::Obj::LetObject::operator>>(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 数字 左位移
		if (_this->isSameAs(selfST, otheST) &&
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				ret.create(StaticB);
				auto result = ret.m_block->ptr;
				long long int one = (long long int) * (doublePtr)self;
				long long int two = (long long int) * (doublePtr)othe;
				long long int three = one >> two;
				*(doublePtr)result = (numberT)three;
			}
			break;
			default:
				break;
			}
			ret.setType(_this->m_objType);
		}
		else {
			throw("Object : cant take >>");
		}

	} while (false);

	return std::move(ret);
}

// >

LetObject Cervice::Obj::LetObject::operator>(LetObject&& node)
{
	return operator>(&node);
}

LetObject Cervice::Obj::LetObject::operator>(LetObject& node)
{
	return operator>(&node);
}

LetObject Cervice::Obj::LetObject::operator>(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);
	ret << false;

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型 数字 字符 >
		if (_this->isSameAs(selfST, otheST) ||
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				auto result = ret.m_block->ptr;
				numberT one = * (doublePtr)self;
				numberT two = * (doublePtr)othe;

				if (selfST == ObjT::boolean) {
					one = (numberT) * (boolPtr)self;
				}
				if (otheST == ObjT::boolean) {
					two = (numberT) * (boolPtr)othe;
				}

				if (one > two) {
					ret << true;
				}
			}
			break;
			case LetObject::ObjT::string:
			{
				size_t lensS = strnlen_s(self, selfL);
				size_t lensO = strnlen_s(othe, otheL);

				size_t strlens = lensS > lensO ? lensO : lensS;

				for (size_t index = 0; index < strlens; index++) {
					if (self[index] > othe[index]) {
						ret << true;
						goto brk;
					}
					else if(self[index] < othe[index]){
						goto brk;
					}
					continue;
				}

				if (lensS > lensO) {
					ret << true;
					break;
				}
			}
			break;
			default:
				break;
			}
		brk:
			ret.setType({ ObjT::boolean,ObjG::Static });
		}
		else {
			throw("Object : cant take >");
		}

	} while (false);

	return std::move(ret);
}

// <

LetObject Cervice::Obj::LetObject::operator<(LetObject&& node)
{
	return operator<(&node);
}

LetObject Cervice::Obj::LetObject::operator<(LetObject& node)
{
	return operator<(&node);
}

LetObject Cervice::Obj::LetObject::operator<(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);
	ret << false;

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型 数字 字符 <
		if (_this->isSameAs(selfST, otheST) ||
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				auto result = ret.m_block->ptr;
				numberT one = *(doublePtr)self;
				numberT two = *(doublePtr)othe;

				if (selfST == ObjT::boolean) {
					one = (numberT) * (boolPtr)self;
				}
				if (otheST == ObjT::boolean) {
					two = (numberT) * (boolPtr)othe;
				}

				if (one < two) {
					ret << true;
				}
			}
			break;
			case LetObject::ObjT::string:
			{
				size_t lensS = strnlen_s(self, selfL);
				size_t lensO = strnlen_s(othe, otheL);

				size_t strlens = lensS > lensO ? lensO : lensS;

				for (size_t index = 0; index < strlens; index++) {
					if (self[index] < othe[index]) {
						ret << true;
						goto brk;
					}
					else if (self[index] > othe[index]) {
						goto brk;
					}
					continue;
				}

				if (lensS < lensO) {
					ret << true;
					break;
				}
			}
			break;
			default:
				break;
			}
		brk:
			ret.setType({ ObjT::boolean,ObjG::Static });
		}
		else {
			throw("Object : cant take <");
		}

	} while (false);

	return std::move(ret);
}

// >=

LetObject Cervice::Obj::LetObject::operator>=(LetObject&& node)
{
	return operator>=(&node);
}

LetObject Cervice::Obj::LetObject::operator>=(LetObject& node)
{
	return operator>=(&node);
}

LetObject Cervice::Obj::LetObject::operator>=(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);
	ret << false;

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		if ((otheS.type == ObjT::null && selfS.type == ObjT::null) ||
			(otheS.type == ObjT::undef && selfS.type == ObjT::undef)) {

			ret << true;
			return std::move(ret);
		}

		if ((otheS.type == ObjT::null || selfS.type == ObjT::null) ||
			(otheS.type == ObjT::undef || selfS.type == ObjT::undef)) {

			return std::move(ret);
		}

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型 数字 字符 <=
		if (_this->isSameAs(selfST, otheST) ||
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				auto result = ret.m_block->ptr;
				numberT one = *(doublePtr)self;
				numberT two = *(doublePtr)othe;

				if (selfST == ObjT::boolean) {
					one = (numberT) * (boolPtr)self;
				}
				if (otheST == ObjT::boolean) {
					two = (numberT) * (boolPtr)othe;
				}

				if (one >= two) {
					ret << true;
				}
			}
			break;
			case LetObject::ObjT::string:
			{
				size_t lensS = strnlen_s(self, selfL);
				size_t lensO = strnlen_s(othe, otheL);

				size_t strlens = lensS > lensO ? lensO : lensS;

				for (size_t index = 0; index < strlens; index++) {
					if (self[index] > othe[index]) {
						ret << true;
						goto brk;
					}
					else if (self[index] < othe[index]) {
						goto brk;
					}
					continue;
				}

				if (lensS >= lensO) {
					ret << true;
					break;
				}
			}
			break;
			default:
				break;
			}
		brk:
			ret.setType({ ObjT::boolean,ObjG::Static });
		}
		else {
			throw("Object : cant take >=");
		}

	} while (false);

	return std::move(ret);
}

// <=

LetObject Cervice::Obj::LetObject::operator<=(LetObject&& node)
{
	return operator<=(&node);
}

LetObject Cervice::Obj::LetObject::operator<=(LetObject& node)
{
	return operator<=(&node);
}

LetObject Cervice::Obj::LetObject::operator<=(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);
	ret << false;

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		if ((otheS.type == ObjT::null && selfS.type == ObjT::null) ||
			(otheS.type == ObjT::undef && selfS.type == ObjT::undef)) {

			ret << true;
			return std::move(ret);
		}

		if ((otheS.type == ObjT::null || selfS.type == ObjT::null) ||
			(otheS.type == ObjT::undef || selfS.type == ObjT::undef)) {

			return std::move(ret);
		}

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型 数字 字符 <=
		if (_this->isSameAs(selfST, otheST) ||
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				auto result = ret.m_block->ptr;
				numberT one = *(doublePtr)self;
				numberT two = *(doublePtr)othe;

				if (selfST == ObjT::boolean) {
					one = (numberT) * (boolPtr)self;
				}
				if (otheST == ObjT::boolean) {
					two = (numberT) * (boolPtr)othe;
				}

				if (one <= two) {
					ret << true;
				}
			}
			break;
			case LetObject::ObjT::string:
			{
				size_t lensS = strnlen_s(self, selfL);
				size_t lensO = strnlen_s(othe, otheL);

				size_t strlens = lensS > lensO ? lensO : lensS;

				for (size_t index = 0; index < strlens; index++) {
					if (self[index] < othe[index]) {
						ret << true;
						goto brk;
					}
					else if (self[index] > othe[index]) {
						goto brk;
					}
					continue;
				}

				if (lensS <= lensO) {
					ret << true;
					break;
				}
			}
			break;
			default:
				break;
			}
		brk:
			ret.setType({ ObjT::boolean,ObjG::Static });
		}
		else {
			throw("Object : cant take <=");
		}

	} while (false);

	return std::move(ret);
}

// !=

LetObject Cervice::Obj::LetObject::operator!=(LetObject&& node)
{
	return operator!=(&node);
}

LetObject Cervice::Obj::LetObject::operator!=(LetObject& node)
{
	return operator!=(&node);
}

LetObject Cervice::Obj::LetObject::operator!=(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);
	ret << false;

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		if ((otheS.type == ObjT::null && selfS.type == ObjT::null) ||
			(otheS.type == ObjT::undef && selfS.type == ObjT::undef)) {

			return std::move(ret);
		}

		if ((otheS.type == ObjT::null || selfS.type == ObjT::null) ||
			(otheS.type == ObjT::undef || selfS.type == ObjT::undef)) {
			ret << true;
			return std::move(ret);
		}

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型 数字 字符 !=
		if (_this->isSameAs(selfST, otheST) ||
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				auto result = ret.m_block->ptr;
				numberT one = *(doublePtr)self;
				numberT two = *(doublePtr)othe;

				if (selfST == ObjT::boolean) {
					one = (numberT) * (boolPtr)self;
				}
				if (otheST == ObjT::boolean) {
					two = (numberT) * (boolPtr)othe;
				}

				if (one != two) {
					ret << true;
				}
			}
			break;
			case LetObject::ObjT::string:
			{
				if (strcmp(self, othe) != 0) {
					ret << true;
				}
			}
			break;
			default:
				break;
			}
			ret.setType({ ObjT::boolean,ObjG::Static });
		}
		else {
			throw("Object : cant take !=");
		}

	} while (false);

	return std::move(ret);
}

// ==

LetObject Cervice::Obj::LetObject::operator==(LetObject&& node)
{
	return operator==(&node);
}

LetObject Cervice::Obj::LetObject::operator==(LetObject& node)
{
	return operator==(&node);
}

LetObject Cervice::Obj::LetObject::operator==(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);
	ret << false;

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		if ((otheS.type == ObjT::null && selfS.type == ObjT::null)||
			(otheS.type == ObjT::undef && selfS.type == ObjT::undef)) {

			ret << true;
			return std::move(ret);
		}

		if ((otheS.type == ObjT::null || selfS.type == ObjT::null) ||
			(otheS.type == ObjT::undef || selfS.type == ObjT::undef)) {

			return std::move(ret);
		}

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型 数字 字符 ==
		if (_this->isSameAs(selfST, otheST) ||
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				auto result = ret.m_block->ptr;
				numberT one = *(doublePtr)self;
				numberT two = *(doublePtr)othe;

				if (selfST == ObjT::boolean) {
					one = (numberT) * (boolPtr)self;
				}
				if (otheST == ObjT::boolean) {
					two = (numberT) * (boolPtr)othe;
				}

				if (one == two) {
					ret << true;
				}
			}
			break;
			case LetObject::ObjT::string:
			{
				if (strcmp(self, othe) == 0) {
					ret << true;
				}
			}
			break;
			default:
				break;
			}
			ret.setType({ ObjT::boolean,ObjG::Static });
		}
		else {
			throw("Object : cant take !=");
		}

	} while (false);

	return std::move(ret);
}

// &

LetObject Cervice::Obj::LetObject::operator&(LetObject&& node)
{
	return operator&(&node);
}

LetObject Cervice::Obj::LetObject::operator&(LetObject& node)
{
	return operator&(&node);
}


LetObject Cervice::Obj::LetObject::operator&(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 数字 &
		if (_this->isSameAs(selfST, otheST) &&
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				ret.create(StaticB);
				auto result = ret.m_block->ptr;

				long long one, two;
				one = ((long long int)(*(doublePtr)self));
				two = ((long long int)(*(doublePtr)othe));

				if (selfST == ObjT::boolean) {
					one = (*(longPtr)self);
				}
				if (otheST == ObjT::boolean) {
					two = (*(longPtr)othe);
				}

				long long int num = one & two;

				*(doublePtr)result = (numberT)num;
			}
			break;
			default:
				break;
			}
			ret.setType(_this->m_objType);
		}
		else {
			throw("Object : cant take &");
		}

	} while (false);

	return std::move(ret);
}

// ^

LetObject Cervice::Obj::LetObject::operator^(LetObject&& node)
{
	return operator^(&node);
}

LetObject Cervice::Obj::LetObject::operator^(LetObject& node)
{
	return operator^(&node);
}

LetObject Cervice::Obj::LetObject::operator^(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 数字 ^
		if (_this->isSameAs(selfST, otheST) &&
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				ret.create(StaticB);
				auto result = ret.m_block->ptr;

				long long one, two;
				one = ((long long int)(*(doublePtr)self));
				two = ((long long int)(*(doublePtr)othe));

				if (selfST == ObjT::boolean) {
					one = (*(longPtr)self);
				}
				if (otheST == ObjT::boolean) {
					two = (*(longPtr)othe);
				}

				long long int num = one ^ two;

				*(doublePtr)result = (numberT)num;
			}
			break;
			default:
				break;
			}
			ret.setType(_this->m_objType);
		}
		else {
			throw("Object : cant take ^");
		}

	} while (false);

	return std::move(ret);
}

// |

LetObject Cervice::Obj::LetObject::operator|(LetObject&& node)
{
	return operator|(&node);
}

LetObject Cervice::Obj::LetObject::operator|(LetObject& node)
{
	return operator|(&node);
}

LetObject Cervice::Obj::LetObject::operator|(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 数字 |
		if (_this->isSameAs(selfST, otheST) &&
			(_this->isLess(otheST, ObjT::string) && _this->isLess(selfST, ObjT::string))
			)
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			case LetObject::ObjT::number:
			{
				ret.create(StaticB);
				auto result = ret.m_block->ptr;

				long long one, two;
				one = ((long long int)(*(doublePtr)self));
				two = ((long long int)(*(doublePtr)othe));

				if (selfST == ObjT::boolean) {
					one = (*(longPtr)self);
				}
				if (otheST == ObjT::boolean) {
					two = (*(longPtr)othe);
				}

				long long int num = one | two;

				*(doublePtr)result = (numberT)num;
			}
			break;
			default:
				break;
			}
			ret.setType(_this->m_objType);
		}
		else {
			throw("Object : cant take |");
		}

	} while (false);

	return std::move(ret);
}

// &&

LetObject Cervice::Obj::LetObject::operator&&(LetObject&& node)
{
	return operator&&(&node);
}

LetObject Cervice::Obj::LetObject::operator&&(LetObject& node)
{
	return operator&&(&node);
}

LetObject Cervice::Obj::LetObject::operator&&(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);
	ret << false;


	LetObject bovl1,bovl2;
	bovl1 << false;
	bovl2 << false;

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		OBJ_IF_BREAK(otheS.type == ObjT::null || otheS.type == ObjT::undef);
		OBJ_IF_BREAK(selfS.type == ObjT::null || selfS.type == ObjT::undef);

		auto selfL = _this->m_block->lens;
		auto otheL = node->m_block->lens;

		auto selfST = selfS.type;
		auto otheST = otheS.type;

		auto self = _this->m_block->ptr;
		auto othe = node->m_block->ptr;

		// 同类型 数字 字符 ==
		if ((_this->isLessEq(otheST, ObjT::array) && _this->isLessEq(selfST, ObjT::array)))
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			{
				bovl1 << *(boolPtr)othe;
			}
			break;
			case LetObject::ObjT::number:
			{
				numberT one = *(doublePtr)self;
				if (one != 0.0) {
					bovl1 << true;
				}
			}
			break;
			case LetObject::ObjT::string:
			{
				if (strnlen_s(self, selfL) != 0) {
					bovl1 << true;
				}
			}
			break;
			case LetObject::ObjT::array:
			{
				if (!_this->m_num_array.empty() || !_this->m_str_array.empty()) {
					bovl1 << true;
				}
			}
			break;
			default:
				break;
			}

			switch (otheS.type)
			{
			case LetObject::ObjT::boolean:
			{
				bovl2 << *(boolPtr)othe;
			}
			break;
			case LetObject::ObjT::number:
			{
				numberT two = *(doublePtr)othe;
				if (two != 0.0) {
					bovl2 << true;
				}
			}
			break;
			case LetObject::ObjT::string:
			{
				if (strnlen_s(othe, otheL) != 0) {
					bovl2 << true;
				}
			}
			break;
			case LetObject::ObjT::array:
			{
				if (!node->m_num_array.empty() || !node->m_str_array.empty()) {
					bovl2 << true;
				}
			}
			break;
			default:
				break;
			}

			if (cast<bool>(bovl1) && cast<bool>(bovl2)) {
				ret << true;
			}

			ret.setType({ ObjT::boolean,ObjG::Static });
		}
		else {
			throw("Object : cant take &&");
		}

	} while (false);

	return std::move(ret);
}

// &&

LetObject Cervice::Obj::LetObject::operator||(LetObject&& node)
{
	return operator||(&node);
}

LetObject Cervice::Obj::LetObject::operator||(LetObject& node)
{
	return operator||(&node);
}

LetObject Cervice::Obj::LetObject::operator||(LetObject* node)
{
	thisPtr;
	nodePtr;

	LetObject ret(false, false);
	ret << false;


	LetObject bovl1, bovl2;
	bovl1 << false;
	bovl2 << false;

	do
	{
		auto selfS = _this->m_objType;
		auto otheS = node->m_objType;

		if ((otheS.type == ObjT::null && selfS.type == ObjT::null) ||
			(otheS.type == ObjT::undef && selfS.type == ObjT::undef)) {
			break;
		}

		size_t selfL = 0;
		size_t otheL = 0;
		char* self = nullptr;
		char* othe = nullptr;
		auto selfST = selfS.type;
		auto otheST = otheS.type;

		if (!(selfS.type == ObjT::null || selfS.type == ObjT::undef)) {
			selfL = _this->m_block->lens;
			self = _this->m_block->ptr;
		}

		if (!(otheS.type == ObjT::null || otheS.type == ObjT::undef)) {
			otheL = node->m_block->lens;
			othe = node->m_block->ptr;
		}

		// 同类型 数字 字符 ==
		if ((_this->isLessEq(otheST, ObjT::array) && _this->isLessEq(selfST, ObjT::array)))
		{
			switch (selfS.type)
			{
			case LetObject::ObjT::boolean:
			{
				bovl1 << *(boolPtr)othe;
			}
			break;
			case LetObject::ObjT::number:
			{
				numberT one = *(doublePtr)self;
				if (one != 0.0) {
					bovl1 << true;
				}
			}
			break;
			case LetObject::ObjT::string:
			{
				if (strnlen_s(self, selfL) != 0) {
					bovl1 << true;
				}
			}
			break;
			case LetObject::ObjT::array:
			{
				if (!_this->m_num_array.empty() || !_this->m_str_array.empty()) {
					bovl1 << true;
				}
			}
			break;
			default:
				break;
			}

			switch (otheS.type)
			{
			case LetObject::ObjT::boolean:
			{
				bovl2 << *(boolPtr)othe;
			}
			break;
			case LetObject::ObjT::number:
			{
				numberT two = *(doublePtr)othe;
				if (two != 0.0) {
					bovl2 << true;
				}
			}
			break;
			case LetObject::ObjT::string:
			{
				if (strnlen_s(othe, otheL) != 0) {
					bovl2 << true;
				}
			}
			break;
			case LetObject::ObjT::array:
			{
				if (!node->m_num_array.empty() || !node->m_str_array.empty()) {
					bovl2 << true;
				}
			}
			break;
			default:
				break;
			}

			if (cast<bool>(bovl1) || cast<bool>(bovl2)) {
				ret << true;
			}

			ret.setType({ ObjT::boolean,ObjG::Static });
		}
		else {
			throw("Object : cant take ||");
		}

	} while (false);

	return std::move(ret);
}


std::unordered_map<numberT, BlockArray> LetObject::getNumArray()
{
	thisPtr;
	return _this->m_num_array;
}

std::unordered_map<std::string, BlockArray> LetObject::getStrArray()
{
	thisPtr;
	return _this->m_str_array;
}

std::unordered_map<numberT, BlockArray>* LetObject::getNumArrayPtr()
{
	thisPtr;
	return &(_this->m_num_array);
}

std::unordered_map<std::string, BlockArray>* LetObject::getStrArrayPtr()
{
	thisPtr;
	return &(_this->m_str_array);
}

std::vector<LetObject> LetObject::getIndexs()
{
	thisPtr;
	std::vector<LetObject> ret;

	if (_this->getType() == ObjT::array) {

		std::vector<numberT> vecN;
		for (auto & i : _this->m_num_array) {
			vecN.emplace_back((numberT)i.first);
		}

		std::vector<std::string> vecS;
		for (auto& i : _this->m_str_array) {
			vecS.emplace_back(i.first);
		}

		std::sort(vecN.begin(), vecN.end());
		std::sort(vecS.begin(), vecS.end());

		for (auto& i : vecN) {
			auto_c val;
			val << i;
			ret.emplace_back(std::move(val));
		}

		for (auto& i : vecS) {
			auto_c val;
			val << i;
			ret.emplace_back(std::move(val));
		}

		return std::move(ret);
	}

	return ret;
}