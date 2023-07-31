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

SFPtr ScriptC::Obj::CerStackSystem::PushNewSF(std::string name)
{
	m_stacks.emplace_back(CerStackFrame(name));
	return GetLastSF();
}

SFPtr ScriptC::Obj::CerStackSystem::PopNewSF()
{
	m_stacks.pop_back();
	return GetLastSF();
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
