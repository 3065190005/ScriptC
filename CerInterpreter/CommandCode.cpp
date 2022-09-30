#include "..\AutoElement\LetManager.h"
#include "..\AutoElement\LetObject.h"
#include "CommandCode.h"

using namespace Cervice::Obj;

Cervice::Obj::CommandCode::CommandCode(CommandCodeType type, std::map<std::string, auto_c> params):
	m_type(type),
	m_params(params)
{

}

Cervice::Obj::CommandCode::~CommandCode()
{
}

CommandCode::CommandCodeType Cervice::Obj::CommandCode::getCodeType()
{
	return m_type;
}

void Cervice::Obj::CommandCode::insertCodeParams(std::string name, auto_c value)
{
	m_params.insert({ name,std::move(value) });
}

std::map<std::string, auto_c>* Cervice::Obj::CommandCode::getCodeParams()
{
	return &m_params;
}

void Cervice::Obj::CommandCode::setDebugInfo(CerTokClass::DebugInfo info)
{
	m_info = info;
}

CerTokClass::DebugInfo Cervice::Obj::CommandCode::getDebugInfo()
{
	return m_info;
}

std::string Cervice::Obj::CommandCode::getCodeTypeStr()
{
	std::string ret;
	switch (m_type)
	{
	case Cervice::Obj::CommandCode::CommandCodeType::Pop:
		ret = "Pop";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Inter:
		ret = "Inter";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Func:
		ret = "Func";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Call:
		ret = "Call";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Jmp:
		ret = "Jmp";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::PopS:
		ret = "Pops";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::PushS:
		ret = "PushS";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Lens:
		ret = "Lens";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Leave:
		ret = "Leave";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::UnaryAdd:
		ret = "UnaryAdd";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::UnarySub:
		ret = "UnarySub";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Pass:
		ret = "Pass";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Push:
		ret = "Push";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Add:
		ret = "Add";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Sub:
		ret = "Sub";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Mul:
		ret = "Mul";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Div:
		ret = "Div";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Mod:
		ret = "Mod";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Mat:
		ret = "Mat";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Not:
		ret = "Not";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::LeftO:
		ret = "LeftOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::RightO:
		ret = "RightOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Gtr:
		ret = "GtrOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Geq:
		ret = "GeqOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Lss:
		ret = "LssOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Leq:
		ret = "LeqOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Equ:
		ret = "EquOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Neq:
		ret = "NeqOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::And:
		ret = "AndOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Xor:
		ret = "XorOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Or:
		ret = "orOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::DAnd:
		ret = "DAndOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::DOr:
		ret = "DOrOperator";
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Inc:
		ret = "Inc";
		break;
	default:
		break;
	}

	return ret;
}
