#pragma once
#ifndef _SCRIPTC_SEMANTICANALYZER_
#define _SCRIPTC_SEMANTICANALYZER_

#include <vector>
#include <list>

#include "NodeVisitor.h"

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;

		class CommandCode;
		class NodeVisitor;
		class CerTokClass;
		class SymbolTable;

		/************************************************
		* class SemanticAnalyzer : public NodeVisitor
		* 语义分析器，判断当前符号是否存在于表中
		* 以名字作为下标存放符号及判断引用的符号是否存在合法
		************************************************/

		class SemanticAnalyzer : public NodeVisitor
		{
		public:
			SemanticAnalyzer();
			~SemanticAnalyzer();

			static SemanticAnalyzer* create(AST* tree);		// 语法解析

		public:
			bool scanner(auto_c * ret , AST* rootAts = nullptr);		// 开始分析
		private:
			static SymbolTable m_global_symbol_table;			// 系统变量域
			static std::list<std::string> m_global_file_table;	// 文件包含空间

		public:
			bool visit_indexExprOp(AST* node, autoPtr ret);		// 编译生成下标运算操作码
			bool visit_exprOp(AST* node, autoPtr ret);			// 编译生成总表达式操作码
			bool visit_BinOp(AST* node, autoPtr ret);			// 编译生成二元运算操作码
			bool visit_IfExpr(AST* node, autoPtr ret);			// 编译生成条件运算操作码
			bool visit_WhileExpr(AST* node, autoPtr ret);		// 编译条件循环运算操作码
			bool visit_ForExpr(AST* node, autoPtr ret);			// 编译变量循环运算操作码
			bool visit_UnaryOp(AST* node, autoPtr ret);			// 编译生成一元运算操作码
			bool visit_Num(AST* node, autoPtr ret);				// 编译生成常量运算操作码
			bool visit_Str(AST* node, autoPtr ret);				// 编译生成字串运算操作码
			bool visit_Var(AST* node, autoPtr ret);				// 编译生成变量运算操作码
			bool visit_BreakOp(AST* node, autoPtr ret);			// 编译生成跳出运算操作码
			bool visit_ContinueOp(AST* node, autoPtr ret);		// 编译生成继续运算操作码
			bool visit_InterNew(AST* node, autoPtr ret);		// 编译生成接口实例操作码
			bool visit_IncludeFile(AST* node, autoPtr ret);		// 编译文件包含实例操作码
			bool visit_ArrayVar(AST* node, autoPtr ret);		// 编译数组生成运算操作码
			bool visit_InterExprOp(AST* node, autoPtr ret);		// 编译接口成员运算操作码
			bool visit_AssignOp(AST* node, autoPtr ret);		// 编译生成赋值运算操作码
			bool visit_ReturnOp(AST* node, autoPtr ret);		// 编译生成返回运算操作码
			bool visit_YieldOp(AST* node, autoPtr ret);			// 编译生成协程挂起操作码
			bool visit_ResumeOp(AST* node, autoPtr ret);		// 编译生成协程恢复操作码
			bool visit_FunDeclaration(AST* node, autoPtr ret);	// 编译生成函数定义操作码
			bool visit_FunctionHeader(AST* node, autoPtr ret);	// 编译生成函数声明操作码
			bool visit_FunctionCall(AST* node, autoPtr ret);	// 编译生成函数调用操作码
			bool visit_InterDeclaration(AST* node, autoPtr ret);// 编译生成接口定义操作码
			bool visit_InterfaceHeader(AST* node, autoPtr ret);	// 编译生成接口声明操作码
			bool visit_Body(AST* node, autoPtr ret);			// 对代码块元素进行逐个编译生成
			bool visit_Program(AST* node, autoPtr ret);			// 程序开始节点，对块编译生成
			bool visit_Empty(AST* node, autoPtr ret);			// pass 生成

		private:
			bool m_hasMain;
			AST* m_rootAts;
			SymbolTable * m_symbol_table;
		};
	}
}

#endif

