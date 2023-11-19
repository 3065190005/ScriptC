#include "..\AutoElement\LetManager.h"
#include "..\CerInterpreter\CommandCode.h"
#include "CerCalcStack.h"
#include "CerRunTime.h"
#include "CerStackFrame.h"
#include "CerStackSystem.h"

using namespace ScriptC::Obj;

SFPtr ScriptC::Obj::CerStackSystem::GetLastSF()
{
	SFPtr ret = m_stacks.begin();
	if (ret != m_stacks.end()) {
		std::advance(ret, m_stacks.size() - 1);
	}
	return ret;
}

SFPtr ScriptC::Obj::CerStackSystem::GetLastFuncSf()
{
	auto riter = GetLastSF();
	while (riter->getStackFrameName()[0] == '~')
		riter--;

	return riter;
}

SFPtr ScriptC::Obj::CerStackSystem::PushNewSF(std::string name)
{
	m_stacks.emplace_back(CerStackFrame(name, GetOnlyId()));
	return GetLastSF();
}

SFPtr ScriptC::Obj::CerStackSystem::PopNewSF()
{
	ClearOnlyId(m_stacks.rbegin()->getStackFrameId());
	m_stacks.pop_back();
	return GetLastSF();
}

SFPtr ScriptC::Obj::CerStackSystem::GetBaseSF()
{
	SFPtr ret = m_stacks.begin();
	return ret;
}

bool ScriptC::Obj::CerStackSystem::SaveLastSF(numberT id, numberT eip, std::string file)
{
	auto back_element = m_stacks.back();
	m_stacks.pop_back();

	m_bakcup_stacks[id].emplace_back(std::move(back_element));
	m_bk_stack_eip[id].emplace_back(eip);
	m_bk_stack_file_name[id].emplace_back(file);
	return true;
}

numberT ScriptC::Obj::CerStackSystem::LoadSfOnce(numberT id, std::string& ret_file)
{
	auto finder = m_bakcup_stacks.find(id);
	if (finder == m_bakcup_stacks.end())
		return -2;

	/* 恢复父栈的this 本体 */
	auto this_finder = m_bk_stack_this_ref.find(id);
	if (this_finder != m_bk_stack_this_ref.end())
	{
		if (GetLastSF()->getStackFrameName()[0] != '~')
		{
			GetLastSF()->getDataStack()->emplace_back(std::move(this_finder->second));
			m_bk_stack_this_ref.erase(id);
		}
	}

	/* 恢复栈 */
	m_stacks.emplace_back(finder->second.back());
	finder->second.pop_back();

	if (finder->second.empty())
	{
		m_bakcup_stacks.erase(id);
	}

	/* 恢复栈的执行位置eip */
	numberT ret = m_bk_stack_eip[id].back();
	m_bk_stack_eip[id].pop_back();
	if (m_bk_stack_eip[id].empty())
		m_bk_stack_eip.erase(m_bk_stack_eip.find(id));

	/* 获取恢复栈的文件位置 */
	ret_file = m_bk_stack_file_name[id].back();
	m_bk_stack_file_name[id].pop_back();
	if (m_bk_stack_file_name[id].empty())
		m_bk_stack_file_name.erase(m_bk_stack_file_name.find(id));

	return ret;
}

void ScriptC::Obj::CerStackSystem::SetBkStackThisRef(numberT id, auto_c ref)
{
	m_bk_stack_this_ref.insert({ id,std::move(ref) });
}

void ScriptC::Obj::CerStackSystem::ClearBkStackThisRef(numberT id)
{
	m_bk_stack_this_ref.erase(id);
}

numberT ScriptC::Obj::CerStackSystem::GetOnlyId()
{
	std::random_device rd;
	std::mt19937 mt{ rd() };
	
	while (true)
	{
		numberT id = mt();
		auto beg_iter = m_stack_id.begin();
		auto end_iter = m_stack_id.end();

		auto find_iter = std::find(beg_iter, end_iter, id);
		if (find_iter == end_iter)
		{
			m_stack_id.emplace_back(id);
			return id;
		}

		continue;
	}

	throw("GetOnlyId cannot obtain more numberT\n Please Check C++ Code");
	return -1;
}

void ScriptC::Obj::CerStackSystem::ClearOnlyId(numberT id)
{
	auto beg_iter = m_stack_id.begin();
	auto end_iter = m_stack_id.end();
	auto find_iter = std::find(beg_iter, end_iter, id);
	if (find_iter != end_iter)
	{
		m_stack_id.erase(find_iter);
	}
}

bool ScriptC::Obj::CerStackSystem::HasOnlyId(numberT id)
{
	auto end_iter = m_bakcup_stacks.end();
	if (m_bakcup_stacks.find(id) != end_iter)
	{
		return true;
	}
	return false;
}

void ScriptC::Obj::CerStackSystem::setVarMapValue(std::string name, auto_c element, bool recursion)
{
	int state = 0;

	auto getFuncLev = [](char chr)->int {
		if (chr == '!') return 3;
		else if (chr == '~') return 1;
		else return 2;
	};

	state = getFuncLev(GetLastSF()->getStackFrameName()[0]);

	if (recursion == false) {
		GetLastSF()->getRunTime()->setVarMapValue(name, std::move(element));
		return;
	}
	for (auto i = m_stacks.rbegin(); i != m_stacks.rend(); i++) {

		if (state == 2) {
			if (state == getFuncLev(i->getStackFrameName()[0]) && 
				GetLastSF()->getStackFrameName() != i->getStackFrameName()) {
				continue;
			}
		}
		else if (state == 1) {
			if (state > getFuncLev(i->getStackFrameName()[0])) {
				continue;
			}
		}

		state = getFuncLev(i->getStackFrameName()[0]);

		if (i->getRunTime()->findVarMap(name)) {
			i->getRunTime()->setVarMapValue(name, std::move(element));
			return;
		}
	}

	GetLastSF()->getRunTime()->setVarMapValue(name, std::move(element));
}

auto_c ScriptC::Obj::CerStackSystem::getVarMapValue(std::string name, bool recursion)
{
	int state = 0;

	auto getFuncLev = [](char chr)->int {
		if (chr == '!') return 3;
		else if (chr == '~') return 1;
		else return 2;
	};

	state = getFuncLev(GetLastSF()->getStackFrameName()[0]);

	if (recursion == false) {
		return GetLastSF()->getRunTime()->getVarMapValue(name);
	}

	for (auto i = m_stacks.rbegin(); i != m_stacks.rend(); i++) {
		if (state == 2) {
			if (state == getFuncLev(i->getStackFrameName()[0]) && 
				GetLastSF()->getStackFrameName() != i->getStackFrameName()) {
				continue;
			}
		}
		else if (state == 1) {
			if (state > getFuncLev(i->getStackFrameName()[0])) {
				continue;
			}
		}

		state = getFuncLev(i->getStackFrameName()[0]);

		if (i->getRunTime()->findVarMap(name)) {
			return i->getRunTime()->getVarMapValue(name);
		}
	}

	return auto_c(false, false);
}

autoPtr ScriptC::Obj::CerStackSystem::getVarMapValuePtr(std::string name, bool recursion)
{
	int state = 0;

	auto getFuncLev = [](char chr)->int {
		if (chr == '!') return 3;
		else if (chr == '~') return 1;
		else return 2;
	};

	state = getFuncLev(GetLastSF()->getStackFrameName()[0]);

	if (recursion == false) {
		return GetLastSF()->getRunTime()->getVarMapValuePtr(name);
	}

	for (auto i = m_stacks.rbegin(); i != m_stacks.rend(); i++) {

		if (state == 2) {
			if (state == getFuncLev(i->getStackFrameName()[0]) && 
				GetLastSF()->getStackFrameName() != i->getStackFrameName()) {
				continue;
			}
		}
		else if(state == 1) {
			if (state > getFuncLev(i->getStackFrameName()[0])) {
				continue;
			}
		}

		state = getFuncLev(i->getStackFrameName()[0]);

		if (i->getRunTime()->findVarMap(name)) {
			return i->getRunTime()->getVarMapValuePtr(name);
		}
	}

	return nullptr;
}

struct CerRunTime::InterProperties ScriptC::Obj::CerStackSystem::getInterMapValue(std::string name, bool recursion)
{
	if (recursion == false) {
		return GetLastSF()->getRunTime()->getInterMapValue(name);
	}

	for (auto i = m_stacks.rbegin(); i != m_stacks.rend(); i++) {
		if (i->getRunTime()->findInterMap(name)) {
			return i->getRunTime()->getInterMapValue(name);
		}
	}

	return {};
}

CerRunTime::FuncDescription ScriptC::Obj::CerStackSystem::getFuncMapAddress(std::string name)
{
	for (auto i = m_stacks.rbegin(); i != m_stacks.rend(); i++) {
		if (i->getRunTime()->findFuncMap(name)) {
			return i->getRunTime()->getFuncMapValue(name);
		}
	}

	return {};
}
