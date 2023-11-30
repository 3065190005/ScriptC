#include "ErrorHandling.h"

#include <algorithm>

ScriptC::Obj::ErrorHandling* 
ScriptC::Obj::ErrorHandling::m_errorHandling = nullptr;

ScriptC::Obj::ErrorHandling* ScriptC::Obj::ErrorHandling::getInstance()
{
	if (!m_errorHandling) {
		static ScriptC::Obj::ErrorHandling::_errorHGc gc;
		m_errorHandling = new ScriptC::Obj::ErrorHandling();
	}

	return m_errorHandling;
}

ScriptC::Obj::ErrorHandling::ErrorHandling()
{
	m_type_str = { 
		"None",
		"Lexical",
		"Parser",
		"SemanticAnalyzer",
		"Interpreter",
		"Vm" };
}

ScriptC::Obj::ErrorHandling::~ErrorHandling()
{
}

void ScriptC::Obj::ErrorHandling::throwErr(ErrorHandling::errorType etype, std::string info)
{
	std::string type = m_type_str[(int)etype];
	std::string thr = type +" : "+ info;
	throw(thr);
}

void ScriptC::Obj::ErrorHandling::setErrInfo(CerTokClass::DebugInfo info)
{
	m_debug = info;
	updateDebugFrame(info);
}

CerTokClass::DebugInfo ScriptC::Obj::ErrorHandling::getErrorInfo()
{
	return m_debug;
}

ScriptC::Obj::ErrorHandling::DebugFrame
ScriptC::Obj::ErrorHandling::getErrorList()
{
	return m_debug_frame;
}

std::vector<ScriptC::Obj::ErrorHandling::DebugElement>
ScriptC::Obj::ErrorHandling::sortDebugFrame()
{
	std::vector<DebugElement> sort_vector;
	for (auto i : m_debug_frame)
		sort_vector.push_back(i);

	std::sort(sort_vector.begin(), sort_vector.end(),
		[&](DebugElement first, DebugElement second)
		{
			if (first.second.clock < second.second.clock)
				return true;
			return false;
		});

	return sort_vector;
}

void ScriptC::Obj::ErrorHandling::updateDebugFrame(CerTokClass::DebugInfo info)
{
	auto debug_vector = sortDebugFrame();

	auto finder = debug_vector.end();
	for (auto it = debug_vector.begin(); it != debug_vector.end(); it++)
	{
		if (it->second.file == info.file)
		{
			finder = it;
		}
	}

	if (finder == debug_vector.end())
	{
		m_debug_frame[info.file] = info;
		return;
	}
	
	removeUntileClock(finder->second.clock, debug_vector);

	m_debug_frame[info.file] = info;
}

bool ScriptC::Obj::ErrorHandling::removeUntileClock(
	size_t id, 
	std::vector<ScriptC::Obj::ErrorHandling::DebugElement> debug_vector)
{
	bool ret = false;

	for (auto riter = debug_vector.begin(); riter != debug_vector.end(); riter++)
	{
		if (id < riter->second.clock)
		{
			debug_vector.erase(riter);
			riter = debug_vector.begin();
		}
	}

	m_debug_frame.clear();
	for (auto& i : debug_vector)
		m_debug_frame[i.first] = i.second;

	return ret;
}

ScriptC::Obj::ErrorHandling::_errorHGc::_errorHGc()
{

}

ScriptC::Obj::ErrorHandling::_errorHGc::~_errorHGc()
{
	if (ErrorHandling::m_errorHandling) {
		delete m_errorHandling;
		m_errorHandling = nullptr;
	}
}
