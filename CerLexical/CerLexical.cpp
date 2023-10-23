#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "CerLexical.h"
#include <codecvt>

ScriptC::Obj::CerTokClass::CerTokClass(std::string str, TokType type) :
	m_tok_cstr(std::move(str)),
	m_tok_type(type)
{
}

ScriptC::Obj::CerTokClass::CerTokClass(char cstr, TokType type):
	m_tok_type(type)
{
	std::string a("w");
	a[0] = cstr;

	m_tok_cstr = std::move(a);
}

ScriptC::Obj::CerTokClass::CerTokClass(std::wstring str, TokType type)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::string narrowStr = conv.to_bytes(str);

	m_tok_cstr = std::move(narrowStr);
}

ScriptC::Obj::CerTokClass::CerTokClass(wchar_t cstr, TokType type)
{
	std::wstring wstr(L"1");
	wstr[0] = cstr;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::string narrowStr = conv.to_bytes(wstr);

	m_tok_cstr = std::move(narrowStr);
}

ScriptC::Obj::CerTokClass::~CerTokClass()
{
}

void ScriptC::Obj::CerTokClass::copy(CerTokClass& des, CerTokClass& src)
{
	des.m_tok_cstr = src.m_tok_cstr;
	des.m_tok_type = src.m_tok_type;
}

CerTokType ScriptC::Obj::CerTokClass::getType()
{
	return this->m_tok_type;
}

std::string ScriptC::Obj::CerTokClass::getTypeName()
{
	switch ((int)m_tok_type)
	{
	case (int)TokType::Int_Const:return "Int_Const";
	case (int)TokType::Str_Const:return "Str_Const";
	case (int)TokType::float_Const:return "float_Const";
	case (int)TokType::Var_Id:return "Var_Id";

	case (int)TokType::PlusMat:return "PlusMat";		// ~
	case (int)TokType::PlusNot:return "PlusNot";		// !

	case (int)TokType::PlusAdd:return "PlusAdd";		// +
	case (int)TokType::PlusSub:return "PlusSub";		// -
	case (int)TokType::PlusMul:return "PlusMul";		// *
	case (int)TokType::PlusDiv:return "PlusDiv";		// /
	case (int)TokType::PlusMod:return "PlusMod";		// %

	case (int)TokType::PlusLeftO:return "PlusLeftO";
	case (int)TokType::PlusRightO:return "PlusRightO";

	case (int)TokType::PlusGTR:return "PlusGTR";	// >
	case (int)TokType::PlusGEQ:return "PlusGEQ";	// >=
	case (int)TokType::PlusLSS:return "PlusLSS";	// <
	case (int)TokType::PlusLEQ:return "PlusLEQ";	// <=

	case (int)TokType::PlusEQU:return "PlusEQU";	// ==
	case (int)TokType::PlusNEQ:return "PlusNEQ";	// !=

	case (int)TokType::PlusAND:return "PlusAND";	// &
	case (int)TokType::PlusXOR:return "PlusXOR";	// ^
	case (int)TokType::PlusOR:return "PlusOR";		// |
	case (int)TokType::PlusDAND:return "PlusDAND";	// &&

	case (int)TokType::PlusDOR:return "PlusDOR";	// ||

	case (int)TokType::PlusAssign:return "PlusAssign";	// =

	case (int)TokType::LBRACKET:return "LBRACKET";
	case (int)TokType::RBRACKET:return "RBRACKET";
	case (int)TokType::LBRACE:return "LBRACE";
	case (int)TokType::RBRACE:return "RBRACE";
	case (int)TokType::LPARAM:return "LPARAM";
	case (int)TokType::RPARAM:return "RPARAM";
	case (int)TokType::COLON:return "COLON";
	case (int)TokType::COMMA:return "COMMA";
	case (int)TokType::SEMI:return "SEMI";
	case (int)TokType::DOT:return ".";
	case (int)TokType::Key_Let:return "Key_Let";
	case (int)TokType::Key_Function:return "Key_Function";
	case (int)TokType::Key_Interface:return "Key_Interface";
	case (int)TokType::Key_End:return "Key_End";
	case (int)TokType::Key_Return:return "Key_Return";
	case (int)TokType::Key_New:return "Key_New";
	case (int)TokType::Key_False:return "Key_False";
	case (int)TokType::Key_True:return "Key_True";
	case (int)TokType::Key_Null:return "Key_Null";
	case (int)TokType::Key_Undef:return "Key_Undef";
	case (int)TokType::Key_If:return "Key_If";
	case (int)TokType::Key_Elif:return "Key_Elif";
	case (int)TokType::Key_Else:return "Key_Else";
	case (int)TokType::Key_While:return "Key_While";
	case (int)TokType::Key_Break:return "Key_Break";
	case (int)TokType::Key_Continue:return "Key_Continue";
	case (int)TokType::Key_For:return "Key_For";
	case (int)TokType::Key_In:return "Key_In";
	case (int)TokType::Key_Export:return "Key_Export";
	case (int)TokType::Key_InClude:return "Key_Require";
	case (int)TokType::Key_Inherits:return "Key_Inherits";

	case (int)TokType::Eof:return "Eof";
	default:
		return "Unknow";
	}
}

std::string ScriptC::Obj::CerTokClass::getCstr()
{
	return this->m_tok_cstr;
}

void ScriptC::Obj::CerTokClass::reCStr(std::string name)
{
	m_tok_cstr = name;
}

void ScriptC::Obj::CerTokClass::setDebugInfo(DebugInfo info)
{
	m_info = info;
}

ScriptC::Obj::CerTokClass::DebugInfo ScriptC::Obj::CerTokClass::getDebugInfo()
{
	return m_info;
}
