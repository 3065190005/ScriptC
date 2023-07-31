#include "..\AutoElement\LetManager.h"
#include "..\AutoElement\LetObject.h"
#include "CerRunTime.h"
using namespace ScriptC::Obj;

ScriptC::Obj::CerRunTime::CerRunTime()
{
}

ScriptC::Obj::CerRunTime::~CerRunTime()
{
}

std::map<std::string, auto_c>* ScriptC::Obj::CerRunTime::getVarMap()
{
	return &m_var_map;
}

void ScriptC::Obj::CerRunTime::setVarMapValue(std::string name, auto_c element)
{
	auto inter = m_var_map.find(name);
	if (inter == m_var_map.end()) {
		m_var_map.insert({ name,std::move(element) });
	}
	else {
		// 2023.7.10 ´íÎó´úÂëÐÞ¸´ let a = [1,2,3] ; a = "123" ³ö´í
		/*if (inter->second.getType() == LetObject::ObjT::array && inter->second.getAttribute() == 0) {
			inter->second = (inter->second + element);
		}else*/
		inter->second = element;
	}

}

auto_c ScriptC::Obj::CerRunTime::getVarMapValue(std::string name)
{
	return m_var_map[name];
}

autoPtr ScriptC::Obj::CerRunTime::getVarMapValuePtr(std::string name)
{
	return &m_var_map[name];
}

bool ScriptC::Obj::CerRunTime::findVarMap(std::string name)
{
	if (m_var_map.find(name) != m_var_map.end()) {
		return true;
	}
	return false;
}

void ScriptC::Obj::CerRunTime::setFuncMapValue(std::string name, FuncDescription des)
{
	auto inter = m_func_map.find(name);
	if (inter == m_func_map.end()) {
		m_func_map.insert({ name,std::move(des) });
	}
	else {
		inter->second = des;
	}
}

CerRunTime::FuncDescription ScriptC::Obj::CerRunTime::getFuncMapValue(std::string name)
{
	return m_func_map[name];
}

bool ScriptC::Obj::CerRunTime::findFuncMap(std::string name)
{
	if (m_func_map.find(name) != m_func_map.end()) {
		return true;
	}
	return false;
}

void ScriptC::Obj::CerRunTime::setInterMapValue(std::string name, InterProperties proper)
{
	auto inter = m_inter_map.find(name);
	if (inter == m_inter_map.end()) {
		m_inter_map.insert({ name,std::move(proper) });
	}
	else {
		inter->second = proper;
	}
}

CerRunTime::InterProperties ScriptC::Obj::CerRunTime::getInterMapValue(std::string name)
{
	return m_inter_map[name];
}

bool ScriptC::Obj::CerRunTime::findInterMap(std::string name)
{
	if (m_inter_map.find(name) != m_inter_map.end()) {
		return true;
	}
	return false;
}

void ScriptC::Obj::CerRunTime::insetInterVal(std::string cls , std::string name, auto_c var_default)
{
	auto iter = m_inter_map.find(cls);
	if (iter != m_inter_map.end()) {
		iter->second.var_people.insert({ name,std::move(var_default) });
	}
}

void ScriptC::Obj::CerRunTime::insetInterFunc(std::string name ,std::string func)
{
	auto iter = m_inter_map.find(name);
	if (iter != m_inter_map.end()) {
		iter->second.func_name.emplace_back(func);
	}
}
