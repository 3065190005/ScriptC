#include "ErrorHandling.h"

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
}

CerTokClass::DebugInfo ScriptC::Obj::ErrorHandling::getErrorInfo()
{
	return m_debug;
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
