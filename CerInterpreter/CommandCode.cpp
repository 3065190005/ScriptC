#include "..\AutoElement\LetManager.h"
#include "..\AutoElement\LetObject.h"
#include "CommandCode.h"

using namespace ScriptC::Obj;

ScriptC::Obj::CommandCode::CommandCode(CommandCodeType type, std::map<std::string, auto_c> params):
	m_type(type),
	m_params(params)
{

}

ScriptC::Obj::CommandCode::~CommandCode()
{
}

CommandCode::CommandCodeType ScriptC::Obj::CommandCode::getCodeType()
{
	return m_type;
}

void ScriptC::Obj::CommandCode::insertCodeParams(std::string name, auto_c value)
{
	m_params.insert({ name,std::move(value) });
}

std::map<std::string, auto_c>* ScriptC::Obj::CommandCode::getCodeParams()
{
	return &m_params;
}

void ScriptC::Obj::CommandCode::setDebugInfo(CerTokClass::DebugInfo info)
{
	m_info = info;
}

CerTokClass::DebugInfo ScriptC::Obj::CommandCode::getDebugInfo()
{
	return m_info;
}

std::string ScriptC::Obj::CommandCode::getCodeTypeStr()
{
	std::string ret;
	switch (m_type)
	{
	case ScriptC::Obj::CommandCode::CommandCodeType::Pop:
		ret = "Pop";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Inter:
		ret = "Inter";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Func:
		ret = "Func";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Call:
		ret = "Call";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Jmp:
		ret = "Jmp";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::PopS:
		ret = "Pops";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::PushS:
		ret = "PushS";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Lens:
		ret = "Lens";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Leave:
		ret = "Leave";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Yield:
		ret = "Yield";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Resume:
		ret = "Resume";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::UnaryAdd:
		ret = "UnaryAdd";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::UnarySub:
		ret = "UnarySub";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Pass:
		ret = "Pass";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Push:
		ret = "Push";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Add:
		ret = "Add";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::AryIndex:
		ret = "AryIndex";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Sub:
		ret = "Sub";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Mul:
		ret = "Mul";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Div:
		ret = "Div";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Mod:
		ret = "Mod";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Mat:
		ret = "Mat";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Not:
		ret = "Not";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::LeftO:
		ret = "LeftOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::RightO:
		ret = "RightOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Gtr:
		ret = "GtrOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Geq:
		ret = "GeqOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Lss:
		ret = "LssOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Leq:
		ret = "LeqOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Equ:
		ret = "EquOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Neq:
		ret = "NeqOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::And:
		ret = "AndOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Xor:
		ret = "XorOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Or:
		ret = "orOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::DAnd:
		ret = "DAndOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::DOr:
		ret = "DOrOperator";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Inc:
		ret = "Inc";
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Swap:
		ret = "Swap";
		break;
	default:
		break;
	}

	return ret;
}
