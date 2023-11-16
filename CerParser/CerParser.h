#pragma once
#ifndef _SCRIPTC_CERPARSER_
#define _SCRIPTC_CERPARSER_

#include <vector>

namespace ScriptC {
	namespace Obj {

		template<typename T>
		class CerLexical;
		class CerTokClass;

		class AST;
		class Num;
		class BinOp;

		class CerParser
		{
		public:
			static CerParser* create(CerLexical<char>* lex);
			CerParser(CerLexical<char> * lexical);
			~CerParser();

		public:
			AST* Program();									// 程序开始
			AST* Body();									// 代码块
			AST* InterfaceDeclaration();					// 接口实现
			AST* InterfaceHeader();							// 接口定义
			AST* FunDeclaration();							// 函数实现
			AST* FunctionHeader(bool has_export = false);	// 函数定义
			AST* FileInclude();								// 文件包含
			std::vector<AST*> ExpressionBody();				// 语句主体
			AST* Assignment();								// 赋值(左值)运算
			AST* ReturnExpr();								// 计算(右值)运算
			AST* ifconditionExpr();							// if节点
			AST* whileconditionExpr();						// while节点
			AST* forconditionExpr();						// for节点
			AST* Empty();									// 空
			AST* indexExpr(bool left = false);				// 下标

			AST* expr();									// 计算(右值)运算
			AST* singleKey();								// 单关键字运算符
			AST* interNew();								// New 运算符
			AST* logicaloper_b();							// 逻辑运算符b
			AST* logicaloper_a();							// 逻辑运算符a
			AST* comparison_b();							// 比较运算符b
			AST* comparison_a();							// 比较运算符a
			AST* shiftoperator();							// 位移操作符
			AST* dualoperation();							// 加减运算
			AST* term();									// 乘除运算
			AST* factor();									// 运算体

			AST* String_const();			// 获取字符串常量
			AST* Integer_const();			// 获取整形常量或浮点常量节点
			AST* Function_Id();				// 获取函数节点
			AST* ArrayExpression();			// 获取数组节点
			AST* InterfaceExpression();		// 接口成员引用

		public:
			bool isExprBegin();
			bool isAssignBegin();

			// call this function to get AST*
			AST* parser();		// 运行分析

		public:
			void takeEat(CerTokType type);	// 吃掉指定type

		public:


		private:
			CerLexical<char>* m_lexical;
		};
	}
}

#endif

