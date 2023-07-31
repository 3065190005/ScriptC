#include "LetManager.h"
#include "LetObject.h"

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
