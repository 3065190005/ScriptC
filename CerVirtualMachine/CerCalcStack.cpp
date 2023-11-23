#include "..\AutoElement\LetManager.h"
#include "..\AutoElement\LetObject.h"
#include "..\CerLexical\CerLexical.h"
#include "..\CerInterpreter\CommandCode.h"
#include "CerCalcStack.h"

using namespace ScriptC::Obj;

ScriptC::Obj::CerCalcStack::CerCalcStack()
{
}

ScriptC::Obj::CerCalcStack::~CerCalcStack()
{
}

void ScriptC::Obj::CerCalcStack::push_opera(auto_c obj)
{
    m_stack.emplace_back(obj);
}

auto_c ScriptC::Obj::CerCalcStack::pop_opera()
{
    auto_c ret;
    ret = m_stack.back();
    m_stack.pop_back();
    return ret;
}

void ScriptC::Obj::CerCalcStack::calc_opera(CodeType type)
{

    switch (type)
    {
    case CodeType::Add:
    case CodeType::Sub:
    case CodeType::Mul:
    case CodeType::Div:
    case CodeType::Mod:
    case CodeType::LeftO:
    case CodeType::RightO:
    case CodeType::Gtr:
    case CodeType::Geq:
    case CodeType::Lss:
    case CodeType::Leq:
    case CodeType::Equ:
    case CodeType::Neq:
    case CodeType::And:
    case CodeType::Xor:
    case CodeType::Or:
    case CodeType::DAnd:
    case CodeType::DOr:
        BinOpCalc(type);
        return;
    case CodeType::UnaryAdd:
    case CodeType::UnarySub:
    case CodeType::Not:
    case CodeType::Mat:
        UnaryCalc(type);
        return;
    default:
        break;
    }
}

void ScriptC::Obj::CerCalcStack::BinOpCalc(CodeType type)
{
    auto_c left, right, result;
    left = m_stack.back();
    m_stack.pop_back();
    right = m_stack.back();
    m_stack.pop_back();

	/*
	* 2023.11.23
	* 添加新的错误处理
	*/
    try
    {
        switch (type)
        {
        case CodeType::Add:
            result = left + right;
            break;
        case CodeType::Sub:
            result = left - right;
            break;
        case CodeType::Mul:
            result = left * right;
            break;
        case CodeType::Div:
            result = left / right;
            break;
        case CodeType::Mod:
            result = left % right;
            break;
        case CodeType::LeftO:
            result = left << right;
            break;
        case CodeType::RightO:
            result = left >> right;
            break;
        case CodeType::Gtr:
            result = left > right;
            break;
        case CodeType::Geq:
            result = left >= right;
            break;
        case CodeType::Lss:
            result = left < right;
            break;
        case CodeType::Leq:
            result = left <= right;
            break;
        case CodeType::Equ:
            result = left == right;
            break;
        case CodeType::Neq:
            result = left != right;
            break;
        case CodeType::And:
            result = left & right;
            break;
        case CodeType::Xor:
            result = left ^ right;
            break;
        case CodeType::Or:
            result = left | right;
            break;
        case CodeType::DAnd:
            result = left && right;
            break;
        case CodeType::DOr:
            result = left || right;
            break;
        }
    }
    catch(...)
    {
        LetTools tools;
        std::string str;
        str.append("Vm : Type ");
        str.append(getAutoCTypeStr(left.getType()));
        str.append(" and type ");
        str.append(getAutoCTypeStr(right.getType()));
        str.append(" cannot perform ");
        str.append(getBinOpStr(type));
        str.append(" operation");
        throw(str);
        return;
    }
    

    m_stack.emplace_back(result);
}

void ScriptC::Obj::CerCalcStack::UnaryCalc(CodeType type)
{
    auto_c left, result, unary;
    left = m_stack.back();
    m_stack.pop_back();

    numberT op;

	/*
	* 2023.11.23
	* 添加新的错误处理
	*/
    try
    {
        switch (type)
        {
        case CodeType::UnarySub:
        {
            op = -1.0;
            unary << op;
            result = left * unary;
            break;
        }
        case CodeType::UnaryAdd:
        {
            op = 1.0;
            unary << op;
            result = left * unary;
            break;
        }
        case CodeType::Not:
        {

            result = !left;
            break;
        }
        case CodeType::Mat:
        {

            result = ~left;
            break;
        }
        }
    }
    catch (...)
    {
        LetTools tools;
        std::string str;
        str.append("Vm : Type ");
        str.append(getAutoCTypeStr(left.getType()));
        str.append(" cannot perform ");
        str.append(getUnaryStr(type));
        str.append(" operation");
        throw(str);
        return;
    }
    

    m_stack.emplace_back(result);
}

std::string ScriptC::Obj::CerCalcStack::getBinOpStr(CodeType type)
{
    switch (type)
    {
    case CodeType::Add:
        return "+";
        break;
    case CodeType::Sub:
        return "-";
        break;
    case CodeType::Mul:
        return "*";
        break;
    case CodeType::Div:
        return "/";
        break;
    case CodeType::Mod:
        return "%";
        break;
    case CodeType::LeftO:
        return "<<";
        break;
    case CodeType::RightO:
        return ">>";
        break;
    case CodeType::Gtr:
        return ">";
        break;
    case CodeType::Geq:
        return ">=";
        break;
    case CodeType::Lss:
        return "<";
        break;
    case CodeType::Leq:
        return "<=";
        break;
    case CodeType::Equ:
        return "==";
        break;
    case CodeType::Neq:
        return "!=";
        break;
    case CodeType::And:
        return "&";
        break;
    case CodeType::Xor:
        return "^";
        break;
    case CodeType::Or:
        return "|";
        break;
    case CodeType::DAnd:
        return "&&";
        break;
    case CodeType::DOr:
        return "||";
        break;
    }

    throw("Vm: Error BinOp Str \n Please Check C++ Code\n");
    return "";
}

std::string ScriptC::Obj::CerCalcStack::getUnaryStr(CodeType type)
{
    switch (type)
    {
    case CodeType::UnarySub:
    {
        return "-";
    }
    case CodeType::UnaryAdd:
    {
        return "+";
    }
    case CodeType::Not:
    {

        return "!";
    }
    case CodeType::Mat:
    {
        return "~";
    }
    }


    throw("Vm: Error UnaryOp Str \n Please Check C++ Code\n");
    return "";
}

std::string ScriptC::Obj::CerCalcStack::getAutoCTypeStr(LetObject::ObjT type)
{
    switch (type)
    {
    case AutoMem::Obj::LetObject::ObjT::null:
        return "null";
        break;
    case AutoMem::Obj::LetObject::ObjT::undef:
        return "undef";
        break;
    case AutoMem::Obj::LetObject::ObjT::boolean:
        return "boolean";
        break;
    case AutoMem::Obj::LetObject::ObjT::number:
        return "number";
        break;
    case AutoMem::Obj::LetObject::ObjT::string:
        return "string";
        break;
    case AutoMem::Obj::LetObject::ObjT::array:
        return "array";
        break;
    default:
        break;
    }
}

CerCalcStack* ScriptC::Obj::CerCalcStack::create()
{
    CerCalcStack* ret = nullptr;
    ret = new CerCalcStack();
    if (!ret) {
        delete ret;
        ret = nullptr;
    }

    return ret;
}
