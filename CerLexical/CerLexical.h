#pragma once
#ifndef _CERLEXICAL_SCRIPTC_
#define _CERLEXICAL_SCRIPTC_

#include <iostream>
#include <vector>

#include "QuickLib/quicklib.h"

namespace ScriptC {
	namespace Obj {
		class ErrorHandling;

		// token ��
		class CerTokClass 
		{
		public:
			// ������Ϣ
			struct DebugInfo {
				unsigned int row = 0;		// ��
				unsigned int column = 0;	// ��
				std::string character;		// ����
				std::string file;
			};

			// token ����
			enum class TokType {
				None = 0,
				Int_Const,		// ���γ���
				Str_Const,		// �ַ�������
				float_Const,	// ���㳣��
				Var_Id,			// Ӣ����

				PlusNot,		// ! ��Ŀ������� ��
				PlusMat,		// ~ ��Ŀ������� ȡ��

				PlusAdd,		// +
				PlusSub,		// -
				PlusMul,		// *
				PlusDiv,		// /
				PlusMod,		// %

				PlusLeftO,		// << ����
				PlusRightO,		// >> ����

				PlusGTR,		// >  ����
				PlusGEQ,		// >= ���ڵ���
				PlusLSS,		// <  С��
				PlusLEQ,		// <= С�ڵ���

				PlusEQU,		// == ���
				PlusNEQ,		// != ����

				PlusAND,		// & ��λ��
				PlusXOR,		// ^ ��λ�����
				PlusOR,			// | ��λ��
				PlusDAND,		// && �߼���

				PlusDOR,		// || �߼���

				PlusAssign,		// = ��ֵ

				LBRACE,			// {
				RBRACE,			// }
				LBRACKET,		// [
				RBRACKET,		// ]
				LPARAM,			// (
				RPARAM,			// )

				COLON,			// :
				COMMA,			// ,
				SEMI,			// ;
				DOT,			// .

				Key_Let,		// let �ؼ���
				Key_Interface,	// interface �ؼ���
				Key_Function,	// function ����
				Key_End,		// end �ؼ���
				Key_Return,		// return �ؼ���
				Key_New,		// new �ؼ���
				Key_False,		// false �ؼ���
				Key_True,		// true �ؼ���
				Key_Null,		// null �ؼ���
				Key_Undef,		// undef �ؼ���
				Key_If,			// if �ؼ���
				Key_Elif,		// elif �ؼ���
				Key_Else,		// else �ؼ���
				Key_While,		// while �ؼ���
				Key_Break,		// break �ؼ���
				Key_Continue,	// continue �ؼ���
				Key_For,		// for �ؼ���
				Key_In,			// in �ؼ���
				Key_Export,		// export �ؼ���
				Key_Require,	// require �ؼ���
				Key_Override,	// override �ؼ���

				Key_Yield,		// co_yield
				Key_Resume,		// co_resume

				Eof,			// �ļ�������
			};

		public:
			CerTokClass(std::string str = "",TokType type = TokType::None);
			CerTokClass(char cstr, TokType type = TokType::None);

			CerTokClass(std::wstring str, TokType type = TokType::None);
			CerTokClass(wchar_t cstr , TokType type = TokType::None);
			~CerTokClass();

			static void copy(CerTokClass&, CerTokClass&);

			TokType getType();
			std::string getTypeName();
			std::string getCstr();
			void reCStr(std::string);

			void setDebugInfo(struct DebugInfo);
			CerTokClass::DebugInfo getDebugInfo();

		private:
			std::string m_tok_cstr;		// token �ı��ַ���
			TokType m_tok_type;			// token ����
			struct DebugInfo m_info;	// ������info��Ϣ
		};

		// ���ñ���
		using CerTokType = CerTokClass::TokType;

		template<typename T>	// code type (utf-8 or Ascii)
		class CerLexical
		{
		public:
			static CerLexical * create(std::basic_string<T> code,std::string);	// �����ִ���

		public:
			CerLexical(std::basic_string<T> code);			// ��Դ�ļ����д���
			~CerLexical();

			void JumpExegesis();				// ����ע��
			bool isNumber(T byte);				// �ж��ַ��Ƿ����� 0 ~ 9
			bool isCharacter(T byte);			// �ж��ַ��Ƿ���ĸ a~z A~Z
			bool isJumpChar(T byte);			// �����ո� �س�
			bool isRowJump(T byte);				// ����/t ' '
			bool isColumnJump(T byte);			// ���� /r /n
			bool isDoubleChar(T byte1,T byte2);	// �ж�˫����
			void pushToks(CerTokClass tok);		// ���token
			CerTokType isKeyWord(std::string str);	// �ж��Ƿ�keyWord

			std::basic_string<T> getIntegerConst(bool& isfloat);	// ��ȡ����(����С��
			std::basic_string<T> getVarIdConst();					// ��ȡӢ������(�����»���
			std::basic_string<T> getStrConst();						// ��ȡ�ַ�������

			bool initTokenVector();				// ��ʼ������ tokens

			bool advance(int jump = 1);			// ����Token�����±�

			void resourcesIndexAdvance(int i = 1);

			void printTokInfo(struct CerTokClass::DebugInfo);

		public: // CerrParser
			CerTokClass getNextToken();					// ���ص�ǰtoken currentָ����һ��
			CerTokClass peekNextToken(int step = 0);	// ������step��token
			CerTokClass& getCurrentToken();				// ��ȡ��ǰcurrent

			void setFileName(std::string file);
			std::string getFileName();

		private:
			size_t m_sources_index;								// Դ�ļ���ǰֵ
			std::basic_string<T> m_sources_txt;					// Դ�ļ�����
			std::vector<CerTokClass> m_tokens_vec;				// tokens�浵����
			CerTokClass m_current_tok;							// tokens��ǰֵ
			size_t m_tokens_index;								// tokens����ָ��

			std::string m_file_name;							// ��ǰ�ļ���
			unsigned int m_row;									// ��
			unsigned int m_column;								// ��
		};

	}
}

#include "CerLexical.tpp"

#endif

