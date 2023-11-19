#pragma once
#ifndef _SCRIPTC_CERINTERPRETER_
#define _SCRIPTC_CERINTERPRETER_

#include "CommandCode.h"
#include "..\CerParser\AST.h"
#include "NodeVisitor.h"
#include <map>
#include <vector>

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;
		
		class CommandCode;
		class NodeVisitor;
		class CerTokClass;
		class AST;

		/*******************************************
		* class CerInterpreter : public NodeVisitor
		* 编译器 相关实现，基础并实现处理者内容
		********************************************/
		class CerInterpreter : public NodeVisitor
		{
		public:
			using ByteCodes = std::map<std::string, std::vector<CommandCode>>;

		public:
			CerInterpreter();
			~CerInterpreter();

			static CerInterpreter* create(AST* tree, std::string fileName);
		public:
			ByteCodes& CompileCode(autoPtr ret, AST* rootAts = nullptr);	// 对外实现接口
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
			bool visit_YieldOp(AST* node, autoPtr ret);			// 编译生成协程挂起操作码
			bool visit_ResumeOp(AST* node, autoPtr ret);		// 编译生成协程恢复操作码
			bool visit_ReturnOp(AST* node, autoPtr ret);		// 编译生成返回运算操作码
			bool visit_FunDeclaration(AST* node, autoPtr ret);	// 编译生成函数定义操作码
			bool visit_FunctionHeader(AST* node, autoPtr ret);	// 编译生成函数声明操作码
			bool visit_FunctionCall(AST* node, autoPtr ret);	// 编译生成函数调用操作码
			bool visit_InterDeclaration(AST* node, autoPtr ret);// 编译生成接口定义操作码
			bool visit_InterfaceHeader(AST* node, autoPtr ret);	// 编译生成接口声明操作码
			bool visit_Body(AST* node, autoPtr ret);			// 对代码块元素进行逐个编译生成
			bool visit_Program(AST* node, autoPtr ret);			// 程序开始节点，对块编译生成
			bool visit_Empty(AST* node, autoPtr ret);			// pass 生成

			std::vector<CommandCode>* GetSelfVmCodes();

			void PushCode(CommandCode);

			AST::AstType analysExprOp(AST*);
			void printCode();

			std::map<std::string, int> getTables();

		private:
			ByteCodes m_vm_code;								// 操作码集合
			AST* m_rootAts;									// AST节点
			std::map<std::string,int> m_table_temp;			// 标识符号
			std::string m_file_name;						// 当前正在编译的文件名
		};
	}
}


#endif

