#include "..\AutoElement\LetManager.h"
#include "..\CerInterpreter\CommandCode.h"
#include "CerRunTime.h"
#include "CerCalcStack.h"
#include "CerStackFrame.h"
using namespace Cervice::Obj;

Cervice::Obj::CerStackFrame::CerStackFrame(std::string name)
{
	m_stackFrame_name = name;
}

CerCalcStack* Cervice::Obj::CerStackFrame::getCalcStack()
{
	return &m_calcStack;
}

std::list<auto_c>* Cervice::Obj::CerStackFrame::getDataStack()
{
	return &m_data_stack;
}

CerRunTime* Cervice::Obj::CerStackFrame::getRunTime()
{
	return &m_run_time;
}

std::string Cervice::Obj::CerStackFrame::getStackFrameName()
{
	return m_stackFrame_name;
}
