#include "LetManager.h"
#include "LetObject.h"
#include <algorithm>

namespace AutoMem {
	namespace Obj {

		LetObject::LetObject(LetObject&& mov) :
			m_letM(LetManager::getInstance())
		{
			released();
			m_block = mov.m_block;
			mov.m_block = nullptr;
			setType(mov.m_objType);
			mov.setType({ ObjT::null,ObjG::none });
			m_num_array = std::move(mov.m_num_array);
			m_str_array = std::move(mov.m_str_array);
			setAttribute(mov.Attribute);
		}

		LetObject::LetObject(const LetObject& cpy) :
			m_letM(LetManager::getInstance())
		{
			released();
			if (cpy.m_block) {
				cpy.m_block->ref++;
			}
			m_block = cpy.m_block;
			setType(cpy.m_objType);

			m_num_array = cpy.m_num_array;
			m_str_array = cpy.m_str_array,
			setAttribute(cpy.Attribute);
		}

		LetObject::LetObject(bool voi, bool type)
			:m_block(nullptr),
			m_letM(LetManager::getInstance()),
			Attribute(0)
		{
			OBJ_IF_CALL(voi, create(type));
		}

		LetObject::LetObject()
			:m_block(nullptr),
			m_letM(LetManager::getInstance()),
			Attribute(0)
		{
		}

		LetObject::~LetObject()
		{
			released();
		}

		void LetObject::create(bool type)
		{
			m_block = m_letM->malloc_L(type);
			setType({ ObjT::undef ,(type == StaticB ? ObjG::Static : ObjG::Dynamic) });
		}

		void LetObject::released()
		{
			OBJ_IF_CALL(m_block, m_letM->free_L(&m_block));
			setType({ ObjT::null ,ObjG::none });
			m_num_array.clear();
			m_str_array.clear();
		}

		bool LetObject::reference(LetObject* src, LetObject* des)
		{
			if (src == nullptr || des == nullptr) {
				return false;
			}

			while ((des->Attribute & (int)NatureType::ptr) == true)
			{ 
				des = (LetObject*)*(unsigned long long*)des->m_block->ptr;
			}

			src->m_letM->realloc_L(&src->m_block, 8);
			src->setType({ ObjT::undef,ObjG::Static });
			src->Attribute |= (nature)NatureType::ptr;
			unsigned long long ptr = (unsigned long long)des;
			*(longPtr)src->m_block->ptr = ptr;
			return true;
		}

		void LetObject::swap(LetObject& value)
		{
			LetObject temp;

			// this -> temp
			temp.Attribute = this->Attribute;
			temp.m_block = this->m_block;
			temp.m_letM = this->m_letM;
			temp.m_objType = this->m_objType;

			this->m_block = nullptr;
			this->m_letM = nullptr;
			

			for (auto& i : m_num_array)
				temp.m_num_array[i.first].swap(i.second);

			for (auto& i : m_str_array)
				temp.m_str_array[i.first].swap(i.second);

			this->m_num_array.clear();
			this->m_str_array.clear();

			// value -> this

			this->Attribute = value.Attribute;
			this->m_block = value.m_block;
			this->m_letM = value.m_letM;
			this->m_objType = value.m_objType;

			value.m_block = nullptr;
			value.m_letM = nullptr;

			for (auto& i : value.m_num_array)
				this->m_num_array[i.first].swap(i.second);
			for (auto& i : value.m_str_array)
				this->m_str_array[i.first].swap(i.second);
			
			value.m_num_array.clear();
			value.m_str_array.clear();

			// temp -> value
			value.Attribute = temp.Attribute;
			value.m_block = temp.m_block;
			value.m_letM = temp.m_letM;
			value.m_objType = temp.m_objType;

			temp.m_block = nullptr;
			temp.m_letM = nullptr;

			for (auto& i : temp.m_num_array)
				value.m_num_array[i.first].swap(i.second);

			for (auto& i : temp.m_str_array)
				value.m_str_array[i.first].swap(i.second);

			temp.m_num_array.clear();
			temp.m_str_array.clear();
		}

		bool LetObject::isRef(LetObject* value)
		{
			if (value == nullptr)
				return false;

			auto attribute = this->Attribute;
			if (!((int)NatureType::ptr & attribute))
				return false;
			
			auto block_ptr = this->m_block->ptr;
			auto value_ptr = (long long)(value);
			auto this_ptr  = *(longPtr)this->m_block->ptr;
			if (value_ptr == this_ptr)
				return true;
			else
				return false;
		}

		LetObject::ObjT LetObject::getType()
		{
			return m_objType.type;
		}

		LetObject::ObjG LetObject::getGroup()
		{
			return m_objType.group;
		}

		LetObject::ObjS LetObject::getStruct()
		{
			return m_objType;
		}

		nature LetObject::getAttribute()
		{
			thisPtr;
			return _this->Attribute;
		}

		void LetObject::setAttribute(nature att)
		{
			Attribute = att;
		}

		nature LetObject::getSelfAttribute()
		{
			return Attribute;
		}

		void LetObject::setType(ObjS typ)
		{
			m_objType = typ;
		}


		void LetObject::changeBlock(bool type)
		{
			ObjG selfType = m_objType.group;
			ObjG targetType = type == StaticB ? ObjG::Static : ObjG::Dynamic;

			if (selfType == targetType) {
				return;
			}

			if (selfType == ObjG::none) {
				create(type);
				return;
			}

			released();
			create(type);
			return;
		}

		bool LetObject::isLess(ObjT node, ObjT targ)
		{
			return node < targ;
		}

		bool LetObject::isLessEq(ObjT node, ObjT targ)
		{
			return node <= targ;
		}

		bool LetObject::isSameAs(ObjT node, ObjT targ)
		{
			return node == targ;
		}

	}
}

/*************************
* LetTools
* 工具类 变量相关工具
*************************/

void AutoMem::Obj::LetTools::print(auto_c& value)
{
	if (value.getType() == LetObject::ObjT::array) {
		for (auto& i : value.getStrArray()) {
			if (i.second.getType() == LetObject::ObjT::array) {
				print(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::number) {
				std::cout << LetObject::cast<long double>(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::boolean) {
				std::cout << std::boolalpha << LetObject::cast<bool>(i.second);
			}
			else {
				std::cout << LetObject::cast<std::string>(i.second);
			}
		}

		for (auto& i : value.getNumArray()) {
			if (i.second.getType() == LetObject::ObjT::array) {
				print(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::boolean) {
				std::cout << std::boolalpha << i.first << " " << LetObject::cast<bool>(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::number) {
				std::cout << LetObject::cast<long double>(i.second);
			}
			else {
				std::cout << LetObject::cast<std::string>(i.second);
			}
		}
	}
	else if (value.getType() == LetObject::ObjT::number) {
		std::cout << LetObject::cast<long double>(value);
	}
	else if (value.getType() == LetObject::ObjT::string) {
		std::cout << LetObject::cast<std::string>(value);
	}
	else if (value.getType() == LetObject::ObjT::boolean) {
		std::cout << std::boolalpha << LetObject::cast<bool>(value);
	}
}



void AutoMem::Obj::LetTools::println(auto_c& value)
{
	if (value.getType() == LetObject::ObjT::array) {
		for (auto& i : value.getStrArray()) {
			if (i.second.getType() == LetObject::ObjT::array) {
				print(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::number) {
				std::cout << LetObject::cast<long double>(i.second) << std::endl;
			}
			else if (i.second.getType() == LetObject::ObjT::boolean) {
				std::cout << std::boolalpha << LetObject::cast<bool>(i.second) << std::endl;
			}
			else {
				std::cout << LetObject::cast<std::string>(i.second) << std::endl;
			}
		}

		for (auto& i : value.getNumArray()) {
			if (i.second.getType() == LetObject::ObjT::array) {
				print(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::boolean) {
				std::cout << std::boolalpha << i.first << " " << LetObject::cast<bool>(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::number) {
				std::cout << LetObject::cast<long double>(i.second) << std::endl;
			}
			else {
				std::cout << LetObject::cast<std::string>(i.second) << std::endl;
			}
		}
	}
	else if (value.getType() == LetObject::ObjT::number) {
		std::cout << LetObject::cast<long double>(value) << std::endl;
	}
	else if (value.getType() == LetObject::ObjT::string) {
		std::cout << LetObject::cast<std::string>(value) << std::endl;
	}
	else if (value.getType() == LetObject::ObjT::boolean) {
		std::cout << std::boolalpha << LetObject::cast<bool>(value) << std::endl;
	}
}

bool AutoMem::Obj::LetTools::AutoCmp(std::pair<Operator, std::string> condition, auto_c& target)
{
	if (condition.second.empty())
		return false;

	std::function<bool()> cond_lamb = [&]() -> bool
		{
			bool return_bool = false;

			switch (condition.first)
			{
			case AutoMem::Obj::LetTools::Operator::Has:
			{
				auto_c value_auto;
				std::string value = condition.second;
				value_auto << value;
				auto letobject_v = target.getIndexs();

				for (auto& i : letobject_v)
				{
					auto_c cmp_value;
					cmp_value = value_auto == i;
					if (LetObject::cast<bool>(cmp_value))
					{
						return_bool = true;
						break;
					}
				}

				return return_bool;
			}
			case AutoMem::Obj::LetTools::Operator::Type:
			{
				std::string type_str = condition.second;
				
				std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::tolower);

				if (type_str == "null")
					return_bool = target.getType() == LetObject::ObjT::null;
				else if (type_str == "undef")
					return_bool = target.getType() == LetObject::ObjT::undef;
				else if (type_str == "boolean")
					return_bool = target.getType() == LetObject::ObjT::boolean;
				else if (type_str == "number")
					return_bool = target.getType() == LetObject::ObjT::number;
				else if (type_str == "string")
					return_bool = target.getType() == LetObject::ObjT::string;
				else if (type_str == "array")
					return_bool = target.getType() == LetObject::ObjT::array;
				

				return return_bool;
			}
			case AutoMem::Obj::LetTools::Operator::Group:
			{
				std::string type_str = condition.second;
				std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::tolower);

				if (type_str == "dynamic")
					return_bool = target.getGroup() == LetObject::ObjG::Dynamic;
				else if (type_str == "static")
					return_bool = target.getGroup() == LetObject::ObjG::Static;


				return return_bool;
			}
			case AutoMem::Obj::LetTools::Operator::Nature:
			{
				std::string type_str = condition.second;
				std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::tolower);

				if (type_str == "ptr")
					return_bool = (target.getAttribute() & (int)NatureType::ptr);
				else if (type_str == "cls")
					return_bool = (target.getAttribute() & (int)NatureType::cls);


				return return_bool;
			}
			case AutoMem::Obj::LetTools::Operator::SelfNature:
			{
				std::string type_str = condition.second;
				std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::tolower);

				if (type_str == "ptr")
					return_bool = (target.getSelfAttribute() & (int)NatureType::ptr);
				else if (type_str == "cls")
					return_bool = (target.getSelfAttribute() & (int)NatureType::cls);


				return return_bool;
			}

			case AutoMem::Obj::LetTools::Operator::None:
			default:
				return false;
			}

			return return_bool;
		};

	return cond_lamb();
}

void AutoMem::Obj::LetTools::Swap(auto_c& value1, auto_c& value2)
{
	value1.swap(value2);
}

bool AutoMem::Obj::LetTools::IsRef(auto_c& ref_value, auto_c* value)
{
	return ref_value.isRef(value);
}
