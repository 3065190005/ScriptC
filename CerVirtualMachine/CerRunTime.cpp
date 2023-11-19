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
		// 2023.7.10 错误代码修复 let a = [1,2,3] ; a = "123" 出错
		/*if (inter->second.getType() == LetObject::ObjT::array && inter->second.getAttribute() == 0) {
			inter->second = (inter->second + element);
		}else*/
		/*
		* 2023.11.19
		* 修改 直接赋值 为 Swap函数
		* Fixed: 当element或 inter->second 的指针为空指针时会报错
		* 之后将会在用户层面进行保存处理
		*/
		LetTools tools;
		tools.Swap(inter->second, element);
	}

}

void ScriptC::Obj::CerRunTime::removeVarMapValue(std::string name)
{
	auto inter = m_var_map.find(name);
	if (inter != m_var_map.end()) {
		m_var_map.erase(name);
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
	/*
	* 2023.10.23
	* 实现继承关系，获取父类的所有成员
	*/
	auto parent = m_inter_map.find(proper.parent);
	if (parent != m_inter_map.end())
	{
		proper.var_people = parent->second.var_people;
		proper.func_name = parent->second.func_name;
	}

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

/*
* 2023.10.23
* 获取相关类的父类名
*/
std::string ScriptC::Obj::CerRunTime::getInterMapParent(std::string name)
{
	return m_inter_map[name].parent;
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
		/*
		* 2023.10.23
		* 修复父类继承insert无法覆盖的问题
		*/
		iter->second.var_people[name] = std::move(var_default);
	}
}

void ScriptC::Obj::CerRunTime::insetInterFunc(std::string name ,std::string func)
{
	auto iter = m_inter_map.find(name);
	if (iter != m_inter_map.end()) {
		iter->second.func_name.emplace_back(func);
	}
}
