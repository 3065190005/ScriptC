#include <map>
#include <typeinfo>
#include "CerLexical.h"
#include "..\ErrorHandling\ErrorHandling.h"
#include "CerLexicalMacro.h"
using namespace Cervice::Obj;

template<typename T>
CerLexical<T>* Cervice::Obj::CerLexical<T>::create(std::basic_string<T> code,std::string filename)
{
	code = kstring::StringReplace(code, "\r\n", "\n");
	toklog("Sources Txt >>>\n");
	toklog(" - " << code.c_str() << std::endl;);

	CerLexical<T>* lex = new CerLexical<T>(std::move(code));
	lex->m_row = 0;
	lex->m_column = 1;
	lex->m_file_name = filename;

	if (!lex->initTokenVector()) {
		delete lex;
		lex = nullptr;
		goto retGoto;
	}

retGoto:
	return lex;
}

template<typename T>
Cervice::Obj::CerLexical<T>::CerLexical(std::basic_string<T> code):
	m_current_tok("",CerTokType::None)
{
	m_sources_index = 0;
	m_tokens_index = 1;
	m_sources_txt = std::move(code);
}

template<typename T>
Cervice::Obj::CerLexical<T>::~CerLexical()
{
}

template<typename T>
void Cervice::Obj::CerLexical<T>::JumpExegesis()
{
	do {
		T byte_char = m_sources_txt.at(m_sources_index);
		if (!isColumnJump(byte_char)) 
		{
			resourcesIndexAdvance(1);
			m_row = 0;
			m_column++;
			continue;
		}

		break;
	} while (m_sources_index < m_sources_txt.size());
}

template<typename T>
bool Cervice::Obj::CerLexical<T>::isNumber(T byte)
{
	bool wide_char = false;
	std::string name(typeid(byte).name());
	if (name == "wchar_t") {
		return byte >= L'0' && byte <= L'9';
	}
	else if (name == "char") {
		return byte >= '0' && byte <= '9';
	}

	return false;
}

template<typename T>
bool Cervice::Obj::CerLexical<T>::isCharacter(T byte)
{
	if ((byte >= 'a' && byte <= 'z') ||
		(byte >= L'a' && byte <= L'z'))
		return true;

	if ((byte >= 'A' && byte <= 'Z') ||
		(byte >= L'A' && byte <= L'Z'))
		return true;

	return false;
}

template<typename T>
bool Cervice::Obj::CerLexical<T>::isJumpChar(T byte)
{
	if (isRowJump(byte)||
		isColumnJump(byte))
		return true;

	return false;
}

template<typename T>
bool Cervice::Obj::CerLexical<T>::isRowJump(T byte)
{
	if (byte == '\t' ||
		byte == ' ')
		return true;

	if (byte == L'\t' ||
		byte == L' ')
		return true;

	return false;
}

template<typename T>
bool Cervice::Obj::CerLexical<T>::isColumnJump(T byte)
{
	if (byte == '\r' ||
		byte == '\n')
		return true;

	if (byte == L'\r' ||
		byte == L'\n')
		return true;

	return false;
}

template<typename T>
bool Cervice::Obj::CerLexical<T>::isDoubleChar(T byte1,T byte2)
{
	auto index = m_sources_index;
	if (m_sources_index+1 >= m_sources_txt.size())
		return false;
	unsigned int column = 0;
	T char1 = m_sources_txt.at(index);
	while (++index != m_sources_txt.size()) {
		if (isJumpChar(m_sources_txt.at(index))) {
			if (isColumnJump(m_sources_txt.at(index)))
				column++;
			continue;
		}
		else {
			break;
		}
	}

	if (index == m_sources_txt.size()) {
		return false;
	}

	T char2 = m_sources_txt.at(index);

	if (char1 == byte1 && char2 == byte2) {
		m_column += column;
		m_row++;
		m_sources_index = index;
		return true;
	}

	return false;
}

template<typename T>
void Cervice::Obj::CerLexical<T>::pushToks(CerTokClass tok)
{
	tok.setDebugInfo({ m_row,m_column ,tok.getCstr(),m_file_name });
	printTokInfo(tok.getDebugInfo());
	toklog(" - TOKEN(");toklog(tok.getCstr()); 
	toklog(" , "); toklog(tok.getTypeName());
	toklog(")");
	toklog("\n\n");
	m_tokens_vec.push_back(std::move(tok));
}

template<typename T>
CerTokType Cervice::Obj::CerLexical<T>::isKeyWord(std::string str)
{
	CerTokType ret = CerTokType::Var_Id;
	if (str == "let")
		ret = CerTokType::Key_Let;
	else if (str == "interface")
		ret = CerTokType::Key_Interface;
	else if (str == "function")
		ret = CerTokType::Key_Function;
	else if (str == "end")
		ret = CerTokType::Key_End;
	else if (str == "return")
		ret = CerTokType::Key_Return;
	else if (str == "new")
		ret = CerTokType::Key_New;
	else if (str == "false")
		ret = CerTokType::Key_False;
	else if (str == "true")
		ret = CerTokType::Key_True;
	else if (str == "null")
		ret = CerTokType::Key_Null;
	else if (str == "undef")
		ret = CerTokType::Key_Undef;
	else if (str == "if")
		ret = CerTokType::Key_If;
	else if (str == "elif")
		ret = CerTokType::Key_Elif;
	else if (str == "else")
		ret = CerTokType::Key_Else;
	else if (str == "while")
		ret = CerTokType::Key_While;
	else if (str == "break")
		ret = CerTokType::Key_Break;
	else if (str == "continue")
		ret = CerTokType::Key_Continue;
	else if (str == "for")
		ret = CerTokType::Key_For;
	else if (str == "in")
		ret = CerTokType::Key_In;
	else if (str == "export")
		ret = CerTokType::Key_Export;
	else if (str == "require")
		ret = CerTokType::Key_InClude;

	return ret;
}

template<typename T>
std::basic_string<T> Cervice::Obj::CerLexical<T>::getIntegerConst(bool &isfloat)
{
	short isdot = 0;
	std::basic_string<T> ret;
	do {
		T byte_char = m_sources_txt.at(m_sources_index);
		if (isNumber(byte_char)) {
			ret += byte_char;
			resourcesIndexAdvance(1);
			continue;
		}
		else if (isdot == 0 && (byte_char == '.' || byte_char == L'.')) {
			isdot++;
			ret += byte_char;
			resourcesIndexAdvance(1);
			continue;
		}
		else {
			break;
		}
	} while (m_sources_index < m_sources_txt.size());

	if (isdot > 0)
		isfloat = true;
	else
		isfloat = false;

	bool has = true;
	kstring::stringTo<long double>(ret,&has);

	if (!has) {
		auto errHis = ErrorHandling::getInstance();
		errHis->setErrInfo({ m_row, m_column, ret, m_file_name });
		errHis->throwErr(ErrorHandling::errorType::Lexical, "number is biger than max memery");
	}

	return ret;
}

template<typename T>
std::basic_string<T> Cervice::Obj::CerLexical<T>::getVarIdConst()
{
	std::basic_string<T> ret;
	do {
		T byte_char = m_sources_txt.at(m_sources_index);
		if (isCharacter(byte_char)) {
			ret += byte_char;
			resourcesIndexAdvance(1);
			continue;
		}
		else if ((byte_char == '_' || byte_char == L'_')) {
			ret += byte_char;
			resourcesIndexAdvance(1);
			continue;
		}
		else if ((isNumber(byte_char))) {
			ret += byte_char;
			resourcesIndexAdvance(1);
			continue;
		}
		else {
			break;
		}
	} while (m_sources_index < m_sources_txt.size());

	if (ret.length() > MACRO_VAR_MAX_LEN) {
		auto errHis = ErrorHandling::getInstance();
		errHis->setErrInfo({ m_row, m_column, ret, m_file_name });
		errHis->throwErr(ErrorHandling::errorType::Lexical, "value can not longer than 255");
	}

	return ret;
}

template<typename T>
std::basic_string<T> Cervice::Obj::CerLexical<T>::getStrConst()
{
	std::map<char, char> formant = {
		{'a','\a'},
		{'b','\b'},
		{'f','\f'},
		{'n','\n'},
		{'r','\r'},
		{'t','\t'},
		{'v','\v'},
		{'\"','\"'}
	};

	int state = 0;
	char chr = '\0';
	bool isChar = false;
	size_t lens = m_sources_txt.size() - m_sources_index;
	std::string result;
	auto lamb = [&](char c) {
		resourcesIndexAdvance(1);
		if (isColumnJump(c)) {
			m_column++;
			m_row = 0;
		}
	};
	for (; m_sources_index < m_sources_txt.size();lamb(m_sources_txt.at(m_sources_index)))
	{
		chr = m_sources_txt.at(m_sources_index);
		if (state == 0)
		{
			if (chr == '\"') {
				state = 1;
				continue;
			}

		}
		else if (state == 1)
		{
			if (isChar) {
				if (formant.find(chr) != formant.end())
					result.insert(result.end(), formant[chr]);
				else
					result.insert(result.end(), chr);
				isChar = false;
				continue;
			}

			if (chr == '\"') {
				state = 2;
				continue;
			}

			if (chr == '\\') {
				isChar = true;
				continue;
			}

			result.insert(result.end(), chr);
		}
		else if (state == 2)
		{
			break;
		}
	}

	return result;
}

template<typename T>
bool Cervice::Obj::CerLexical<T>::initTokenVector()
{
	toklog("TokenPrint >>>\n");
	T sour_byte;
	while (m_sources_index < m_sources_txt.size()) 
	{
		CerTokClass tok("", CerTokType::None);
		sour_byte = m_sources_txt.at(m_sources_index);

		// É¾³ý×¢ÊÍ
		if (isDoubleChar('/', '/') || isDoubleChar(L'/', L'/')) {
			JumpExegesis();
			continue;
		}

		// ÅÐ¶ÏÊÇ·ñ×Ö·û´®
		if (sour_byte == '\"') {
			tok = CerTokClass(getStrConst(), CerTokType::Str_Const);
			pushToks(std::move(tok));
			continue;
		}

		// Ìø¹ý¿Õ¸ñµÈ
		if (isJumpChar(sour_byte)) {
			resourcesIndexAdvance(1);
			if (isColumnJump(sour_byte)) {
				m_column++;
				m_row = 0;
			}
			else {
				if (m_row == 1) {
					m_row--;
				}
			}
			continue;
		}
		
		// ÅÐ¶ÏÊÇ·ñÃû³Æ
		if (isCharacter(sour_byte) || sour_byte == '_' || sour_byte == L'_')
		{
			auto var_name = getVarIdConst();
			tok = CerTokClass(var_name, isKeyWord(var_name));
			pushToks(std::move(tok));
			continue;

		}
		
		// ÅÐ¶ÏÊÇ·ñÊý×Ö
		if (isNumber(sour_byte)) 
		{
			bool isfloat = false;
			auto number = getIntegerConst(isfloat);
			if (isfloat)
				tok = CerTokClass(number, CerTokType::float_Const);
			else
				tok = CerTokClass(number, CerTokType::Int_Const);
			pushToks(std::move(tok));
			continue;
		}

		// ÅÐ¶ÏË«×Ö·û·ûºÅ
		if (isDoubleChar('<', '<') || isDoubleChar(L'<', L'<')) {
			tok = CerTokClass('<', CerTokType::PlusLeftO);
		}
		else if (isDoubleChar('>', '>') || isDoubleChar(L'>', L'>')) {
			tok = CerTokClass('<', CerTokType::PlusRightO);
		}
		else if (isDoubleChar('>', '=') || isDoubleChar(L'>', L'=')) {
			tok = CerTokClass('=', CerTokType::PlusGEQ);
		}
		else if (isDoubleChar('<', '=') || isDoubleChar(L'<', L'=')) {
			tok = CerTokClass('=', CerTokType::PlusLEQ);
		}
		else if (isDoubleChar('=', '=') || isDoubleChar(L'=', L'=')) {
			tok = CerTokClass('=', CerTokType::PlusEQU);
		}
		else if (isDoubleChar('!', '=') || isDoubleChar(L'!', L'=')) {
			tok = CerTokClass('=', CerTokType::PlusNEQ);
		}
		else if (isDoubleChar('&', '&') || isDoubleChar(L'&', L'&')) {
			tok = CerTokClass('&', CerTokType::PlusDAND);
		}
		else if (isDoubleChar('|', '|') || isDoubleChar(L'|', L'|')) {
			tok = CerTokClass('|', CerTokType::PlusDOR);
		}

		// ÅÐ¶Ïµ¥×Ö·û·ûºÅ
		else if (sour_byte == ';' || sour_byte == L';') {
			tok = CerTokClass(sour_byte, CerTokType::SEMI);
		}
		else if (sour_byte == '.' || sour_byte == L'.') {
			tok = CerTokClass(sour_byte, CerTokType::DOT);
		}
		else if (sour_byte == ':' || sour_byte == L':') {
			tok = CerTokClass(sour_byte, CerTokType::COLON);
		}
		else if (sour_byte == ',' || sour_byte == L',') {
			tok = CerTokClass(sour_byte, CerTokType::COMMA);
		}
		else if (sour_byte == '{' || sour_byte == L'{') {
			tok = CerTokClass(sour_byte, CerTokType::LBRACE);
		}
		else if (sour_byte == '}' || sour_byte == L'}') {
			tok = CerTokClass(sour_byte, CerTokType::RBRACE);
		}
		else if (sour_byte == '[' || sour_byte == L'[') {
			tok = CerTokClass(sour_byte, CerTokType::LBRACKET);
		}
		else if (sour_byte == ']' || sour_byte == L']') {
			tok = CerTokClass(sour_byte, CerTokType::RBRACKET);
		}
		else if (sour_byte == '(' || sour_byte == L'(') {
			tok = CerTokClass(sour_byte, CerTokType::LPARAM);
		}
		else if (sour_byte == ')' || sour_byte == L')') {
			tok = CerTokClass(sour_byte, CerTokType::RPARAM);
		}

		else if (sour_byte == '!' || sour_byte == L'!') {
			tok = CerTokClass(sour_byte, CerTokType::PlusNot);
		}
		else if (sour_byte == '~' || sour_byte == L'~') {
			tok = CerTokClass(sour_byte, CerTokType::PlusMat);
		}

		else if (sour_byte == '+' || sour_byte == L'+') {
			tok = CerTokClass(sour_byte, CerTokType::PlusAdd);
		}
		else if (sour_byte == '-' || sour_byte == L'-') {
			tok = CerTokClass(sour_byte, CerTokType::PlusSub);
		}
		else if (sour_byte == '*' || sour_byte == L'*') {
			tok = CerTokClass(sour_byte, CerTokType::PlusMul);
		}
		else if (sour_byte == '/' || sour_byte == L'/') {
			tok = CerTokClass(sour_byte, CerTokType::PlusDiv);
		}
		else if (sour_byte == '%' || sour_byte == L'%') {
			tok = CerTokClass(sour_byte, CerTokType::PlusMod);
		}

		else if (sour_byte == '>' || sour_byte == L'>') {
			tok = CerTokClass(sour_byte, CerTokType::PlusGTR);
		}
		else if (sour_byte == '<' || sour_byte == L'<') {
			tok = CerTokClass(sour_byte, CerTokType::PlusLSS);
		}

		else if (sour_byte == '&' || sour_byte == L'&') {
			tok = CerTokClass(sour_byte, CerTokType::PlusAND);
		}
		else if (sour_byte == '^' || sour_byte == L'^') {
			tok = CerTokClass(sour_byte, CerTokType::PlusXOR);
		}
		else if (sour_byte == '|' || sour_byte == L'|') {
			tok = CerTokClass(sour_byte, CerTokType::PlusOR);
		}

		else if (sour_byte == '=' || sour_byte == L'=') {
			tok = CerTokClass(sour_byte, CerTokType::PlusAssign);
		}


		if (tok.getType() != CerTokType::None) {
			resourcesIndexAdvance(1);
			pushToks(std::move(tok));
			continue;
		}

		return false;
	}
	
	T ret(0);
	pushToks(std::move(CerTokClass(ret,CerTokType::Eof)));
	m_current_tok = m_tokens_vec.at(0);
	return true;
}

template<typename T>
bool Cervice::Obj::CerLexical<T>::advance(int jump)
{
	if (m_tokens_index < m_tokens_vec.size()) {
		m_tokens_index++;
		return true;
	}

	return false;
}

template<typename T>
void Cervice::Obj::CerLexical<T>::resourcesIndexAdvance(int i)
{
	m_sources_index += i;
	m_row += i;
}

template<typename T>
void Cervice::Obj::CerLexical<T>::printTokInfo(CerTokClass::DebugInfo info)
{
	toklog("[i]bugInfo info("); toklog(info.file);
	toklog(" , "); toklog(info.character);
	toklog(" , "); toklog(info.row);
	toklog(" , "); toklog(info.column);
	toklog(" )\n");
}

template<typename T>
CerTokClass Cervice::Obj::CerLexical<T>::getNextToken()
{
	if (m_tokens_index == m_tokens_vec.size()) {
		return CerTokClass("", CerTokType::Eof);
	}
	m_current_tok = m_tokens_vec.at(m_tokens_index);
	CerTokClass retTok = m_current_tok;
	if (m_tokens_index < m_tokens_vec.size()) {
		advance();
	}

	return retTok;
}

template<typename T>
CerTokClass Cervice::Obj::CerLexical<T>::peekNextToken(int step)
{
	if (m_tokens_index+step >= m_tokens_vec.size()) {
		return CerTokClass("", CerTokType::Eof);
	}
	return m_tokens_vec.at(m_tokens_index + step);
}

template<typename T>
CerTokClass& Cervice::Obj::CerLexical<T>::getCurrentToken()
{
	return m_current_tok;
}

template<typename T>
void Cervice::Obj::CerLexical<T>::setFileName(std::string file)
{
	m_file_name = file;
}

template<typename T>
std::string Cervice::Obj::CerLexical<T>::getFileName()
{
	return m_file_name;
}
