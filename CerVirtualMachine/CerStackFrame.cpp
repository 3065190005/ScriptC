#include "..\AutoElement\LetManager.h"
#include "..\CerInterpreter\CommandCode.h"
#include "CerRunTime.h"
#include "CerCalcStack.h"
#include "CerStackFrame.h"
using namespace ScriptC::Obj;

ScriptC::Obj::CerStackFrame::CerStackFrame(std::string name, numberT id)
{
	m_stackFrame_name = name;
	m_frame_id = id;
}

CerCalcStack* ScriptC::Obj::CerStackFrame::getCalcStack()
{
	return &m_calcStack;
}

std::list<auto_c>* ScriptC::Obj::CerStackFrame::getDataStack()
{
	return &m_data_stack;
}

CerRunTime* ScriptC::Obj::CerStackFrame::getRunTime()
{
	return &m_run_time;
}

std::string ScriptC::Obj::CerStackFrame::getStackFrameName()
{
	return m_stackFrame_name;
}

numberT ScriptC::Obj::CerStackFrame::getStackFrameId()
{
	return m_frame_id;
}
