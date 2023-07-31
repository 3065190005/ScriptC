#include "SymbolClass.h"
using namespace ScriptC::Obj;

/***************************
* class SymbolClass
* ·ûºÅÀà,»ù´¡·ûºÅ£¬±äÁ¿·ûºÅ
***************************/

// »ù´¡·ûºÅ
ScriptC::Obj::SymbolClass::SymbolClass(std::string name)
{
	m_symbol_name = name;
	m_symbol_type = SymbolType::None;
	if (name == "let")
	{
		m_symbol_type = SymbolType::BaseSymbol;
	}

	if (m_symbol_type == SymbolType::None)
		throw("can not take base symbol with " + name);
}


// ±äÁ¿·ûºÅ
ScriptC::Obj::SymbolClass::SymbolClass(std::string name, SymbolClass type)
{
	m_symbol_name = name;
	m_symbol_type = SymbolType::None;

	auto symbol = type.getType();
	if(symbol == SymbolType::BaseSymbol)
	{
		m_symbol_type = SymbolType::VarSymbol;
	}


	if (m_symbol_type == SymbolType::None)
		throw("can not take var symbol with " + name);
}

std::vector<std::string> ScriptC::Obj::SymbolClass::getParams()
{
	return m_symbol_params;
}


// º¯Êý·ûºÅ
ScriptC::Obj::SymbolClass::SymbolClass(std::string func_name, std::vector<std::string> params)
{
	m_symbol_name = func_name;
	m_symbol_type = SymbolType::FuncSymbol;
	m_symbol_params = params;

	if (m_symbol_type == SymbolType::None)
		throw("can not take function symbol with " + func_name);
}

// º¯Êý·ûºÅ
ScriptC::Obj::SymbolClass::SymbolClass(std::string inter_name, std::string inter_parent)
{
	m_symbol_name = inter_name;
	m_symbol_type = SymbolType::InterSymbol;
	m_symbol_inter_parent = inter_parent;

	if (m_symbol_type == SymbolType::None)
		throw("can not take function symbol with " + inter_name);
}

SymbolType ScriptC::Obj::SymbolClass::getType()
{
	return m_symbol_type;
}

std::string ScriptC::Obj::SymbolClass::getName()
{
	return m_symbol_name;
}


/***************************
* class SymbolTable
* ·ûºÅÀà,»ù´¡·ûºÅ£¬±äÁ¿·ûºÅ
***************************/

ScriptC::Obj::SymbolTable::SymbolTable(SymbolTable * parent , std::string symbol_name , size_t level) :
	m_parent(parent),
	m_symtab_name(symbol_name),
	m_symbol_level(level)
{
	m_tables.push_back(SymbolClass("let"));
}

ScriptC::Obj::SymbolTable::~SymbolTable()
{
	m_parent = nullptr;
}

SymbolTable::SymbolfindArea ScriptC::Obj::SymbolTable::findSymbol(std::string name, SymbolType type, bool recursion)
{
	for (auto& i : m_tables) {
		if (i.getName() == name && i.getType() == type) {
			return SymbolfindArea::Self;
		}
	}

	if (recursion && m_parent) {
		if (m_parent->findSymbol(name, type, recursion) != SymbolfindArea::noFind) {
			return SymbolfindArea::Parent;
		}
	}

	return SymbolfindArea::noFind;
}

void ScriptC::Obj::SymbolTable::pushSymbol(SymbolClass symbol)
{
	m_tables.push_back(symbol);
}

SymbolClass ScriptC::Obj::SymbolTable::getFuncSymbol(std::string name, bool recursion)
{
	for (auto& i : m_tables) {
		if (i.getName() == name && i.getType() == SymbolType::FuncSymbol) {
			return i;
		}
	}

	if (recursion && m_parent) {
		if (m_parent->findSymbol(name, SymbolType::FuncSymbol, recursion) != SymbolfindArea::noFind) {
			return m_parent->getFuncSymbol(name, recursion);
		}
	}
	auto ret = SymbolClass("let");
	return ret;
}

void ScriptC::Obj::SymbolTable::setParent(SymbolTable* parent) {
	m_parent = parent;
}

size_t ScriptC::Obj::SymbolTable::getLevel()
{
	return m_symbol_level;
}

SymbolTable* ScriptC::Obj::SymbolTable::getParent()
{
	return m_parent;
}

std::string ScriptC::Obj::SymbolTable::getName()
{
	return m_symtab_name;
}
