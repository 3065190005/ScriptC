#pragma once
#ifndef _CERLEXICAL_SCRIPTC_
#define _CERLEXICAL_SCRIPTC_

#include <iostream>
#include <vector>

#include "QuickLib/quicklib.h"

namespace ScriptC {
	namespace Obj {
		class ErrorHandling;

		// token 类
		class CerTokClass 
		{
		public:
			// 报错信息
			struct DebugInfo {
				unsigned int row = 0;		// 行
				unsigned int column = 0;	// 列
				std::string character;		// 单词
				std::string file;
			};

			// token 类型
			enum class TokType {
				None = 0,
				Int_Const,		// 整形常量
				Str_Const,		// 字符串常量
				float_Const,	// 浮点常量
				Var_Id,			// 英文名

				PlusNot,		// ! 单目非运算符 非
				PlusMat,		// ~ 单目非运算符 取反

				PlusAdd,		// +
				PlusSub,		// -
				PlusMul,		// *
				PlusDiv,		// /
				PlusMod,		// %

				PlusLeftO,		// << 左移
				PlusRightO,		// >> 左移

				PlusGTR,		// >  大于
				PlusGEQ,		// >= 大于等于
				PlusLSS,		// <  小于
				PlusLEQ,		// <= 小于等于

				PlusEQU,		// == 恒等
				PlusNEQ,		// != 不等

				PlusAND,		// & 按位与
				PlusXOR,		// ^ 按位与异或
				PlusOR,			// | 按位或
				PlusDAND,		// && 逻辑与

				PlusDOR,		// || 逻辑或

				PlusAssign,		// = 赋值

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

				Key_Let,		// let 关键字
				Key_Interface,	// interface 关键字
				Key_Function,	// function 声明
				Key_End,		// end 关键字
				Key_Return,		// return 关键字
				Key_New,		// new 关键字
				Key_False,		// false 关键字
				Key_True,		// true 关键字
				Key_Null,		// null 关键字
				Key_Undef,		// undef 关键字
				Key_If,			// if 关键字
				Key_Elif,		// elif 关键字
				Key_Else,		// else 关键字
				Key_While,		// while 关键字
				Key_Break,		// break 关键字
				Key_Continue,	// continue 关键字
				Key_For,		// for 关键字
				Key_In,			// in 关键字
				Key_Export,		// export 关键字
				Key_Require,	// require 关键字
				Key_Override,	// override 关键字

				Key_Yield,		// co_yield
				Key_Resume,		// co_resume

				Eof,			// 文件结束符
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
			std::string m_tok_cstr;		// token 文本字符串
			TokType m_tok_type;			// token 类型
			struct DebugInfo m_info;	// 错误处理info信息
		};

		// 设置别名
		using CerTokType = CerTokClass::TokType;

		template<typename T>	// code type (utf-8 or Ascii)
		class CerLexical
		{
		public:
			static CerLexical * create(std::basic_string<T> code,std::string);	// 创建分词器

		public:
			CerLexical(std::basic_string<T> code);			// 以源文件进行创建
			~CerLexical();

			void JumpExegesis();				// 无视注释
			bool isNumber(T byte);				// 判断字符是否数字 0 ~ 9
			bool isCharacter(T byte);			// 判断字符是否字母 a~z A~Z
			bool isJumpChar(T byte);			// 跳过空格 回车
			bool isRowJump(T byte);				// 跳过/t ' '
			bool isColumnJump(T byte);			// 跳过 /r /n
			bool isDoubleChar(T byte1,T byte2);	// 判断双符号
			void pushToks(CerTokClass tok);		// 添加token
			CerTokType isKeyWord(std::string str);	// 判断是否keyWord

			std::basic_string<T> getIntegerConst(bool& isfloat);	// 获取数字(包括小数
			std::basic_string<T> getVarIdConst();					// 获取英文名称(包括下划线
			std::basic_string<T> getStrConst();						// 获取字符串名称

			bool initTokenVector();				// 初始化所有 tokens

			bool advance(int jump = 1);			// 增进Token容器下标

			void resourcesIndexAdvance(int i = 1);

			void printTokInfo(struct CerTokClass::DebugInfo);

		public: // CerrParser
			CerTokClass getNextToken();					// 返回当前token current指向下一个
			CerTokClass peekNextToken(int step = 0);	// 返回下step个token
			CerTokClass& getCurrentToken();				// 获取当前current

			void setFileName(std::string file);
			std::string getFileName();

		private:
			size_t m_sources_index;								// 源文件当前值
			std::basic_string<T> m_sources_txt;					// 源文件数据
			std::vector<CerTokClass> m_tokens_vec;				// tokens存档容器
			CerTokClass m_current_tok;							// tokens当前值
			size_t m_tokens_index;								// tokens容器指针

			std::string m_file_name;							// 当前文件名
			unsigned int m_row;									// 行
			unsigned int m_column;								// 列
		};

	}
}

#include "CerLexical.tpp"

#endif

