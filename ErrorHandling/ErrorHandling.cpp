#include "ErrorHandling.h"

Cervice::Obj::ErrorHandling* 
Cervice::Obj::ErrorHandling::m_errorHandling = nullptr;

Cervice::Obj::ErrorHandling* Cervice::Obj::ErrorHandling::getInstance()
{
	if (!m_errorHandling) {
		static Cervice::Obj::ErrorHandling::_errorHGc gc;
		m_errorHandling = new Cervice::Obj::ErrorHandling();
	}

	return m_errorHandling;
}

Cervice::Obj::ErrorHandling::ErrorHandling()
{
	m_type_str = { 
		"None",
		"Lexical",
		"Parser",
		"SemanticAnalyzer",
		"Interpreter",
		"Vm" };
}

Cervice::Obj::ErrorHandling::~ErrorHandling()
{
}

void Cervice::Obj::ErrorHandling::throwErr(ErrorHandling::errorType etype, std::string info)
{
	std::string type = m_type_str[(int)etype];
	std::string thr = type +" : "+ info;
	throw(thr);
}

void Cervice::Obj::ErrorHandling::setErrInfo(CerTokClass::DebugInfo info)
{
	m_debug = info;
}

CerTokClass::DebugInfo Cervice::Obj::ErrorHandling::getErrorInfo()
{
	return m_debug;
}

Cervice::Obj::ErrorHandling::_errorHGc::_errorHGc()
{

}

Cervice::Obj::ErrorHandling::_errorHGc::~_errorHGc()
{
	if (ErrorHandling::m_errorHandling) {
		delete m_errorHandling;
		m_errorHandling = nullptr;
	}
}
