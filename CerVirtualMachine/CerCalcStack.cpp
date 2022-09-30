#include "..\AutoElement\LetManager.h"
#include "..\AutoElement\LetObject.h"
#include "..\CerLexical\CerLexical.h"
#include "..\CerInterpreter\CommandCode.h"
#include "CerCalcStack.h"

using namespace Cervice::Obj;

Cervice::Obj::CerCalcStack::CerCalcStack()
{
}

Cervice::Obj::CerCalcStack::~CerCalcStack()
{
}

void Cervice::Obj::CerCalcStack::push_opera(auto_c obj)
{
    m_stack.emplace_back(obj);
}

auto_c Cervice::Obj::CerCalcStack::pop_opera()
{
    auto_c ret;
    ret = m_stack.back();
    m_stack.pop_back();
    return ret;
}

void Cervice::Obj::CerCalcStack::calc_opera(CodeType type)
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

void Cervice::Obj::CerCalcStack::BinOpCalc(CodeType type)
{
    auto_c left, right, result;
    left = m_stack.back();
    m_stack.pop_back();
    right = m_stack.back();
    m_stack.pop_back();

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

    m_stack.emplace_back(result);
}

void Cervice::Obj::CerCalcStack::UnaryCalc(CodeType type)
{
    auto_c left, result, unary;
    left = m_stack.back();
    m_stack.pop_back();

    numberT op;

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

    m_stack.emplace_back(result);
}

CerCalcStack* Cervice::Obj::CerCalcStack::create()
{
    CerCalcStack* ret = nullptr;
    ret = new CerCalcStack();
    if (!ret) {
        delete ret;
        ret = nullptr;
    }

    return ret;
}
