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
		* ������������жϵ�ǰ�����Ƿ�����ڱ���
		* ��������Ϊ�±��ŷ��ż��ж����õķ����Ƿ���ںϷ�
		************************************************/

		class SemanticAnalyzer : public NodeVisitor
		{
		public:
			SemanticAnalyzer();
			~SemanticAnalyzer();

			static SemanticAnalyzer* create(AST* tree);		// �﷨����

		public:
			bool scanner(auto_c * ret , AST* rootAts = nullptr);		// ��ʼ����
		private:
			static SymbolTable m_global_symbol_table;			// ϵͳ������
			static std::list<std::string> m_global_file_table;	// �ļ������ռ�

		public:
			bool visit_indexExprOp(AST* node, autoPtr ret);		// ���������±����������
			bool visit_exprOp(AST* node, autoPtr ret);			// ���������ܱ��ʽ������
			bool visit_BinOp(AST* node, autoPtr ret);			// �������ɶ�Ԫ���������
			bool visit_IfExpr(AST* node, autoPtr ret);			// ���������������������
			bool visit_WhileExpr(AST* node, autoPtr ret);		// ��������ѭ�����������
			bool visit_ForExpr(AST* node, autoPtr ret);			// �������ѭ�����������
			bool visit_UnaryOp(AST* node, autoPtr ret);			// ��������һԪ���������
			bool visit_Num(AST* node, autoPtr ret);				// �������ɳ������������
			bool visit_Str(AST* node, autoPtr ret);				// ���������ִ����������
			bool visit_Var(AST* node, autoPtr ret);				// �������ɱ������������
			bool visit_BreakOp(AST* node, autoPtr ret);			// ���������������������
			bool visit_ContinueOp(AST* node, autoPtr ret);		// �������ɼ������������
			bool visit_InterNew(AST* node, autoPtr ret);		// �������ɽӿ�ʵ��������
			bool visit_IncludeFile(AST* node, autoPtr ret);		// �����ļ�����ʵ��������
			bool visit_ArrayVar(AST* node, autoPtr ret);		// ���������������������
			bool visit_InterExprOp(AST* node, autoPtr ret);		// ����ӿڳ�Ա���������
			bool visit_AssignOp(AST* node, autoPtr ret);		// �������ɸ�ֵ���������
			bool visit_ReturnOp(AST* node, autoPtr ret);		// �������ɷ������������
			bool visit_YieldOp(AST* node, autoPtr ret);			// ��������Э�̹��������
			bool visit_ResumeOp(AST* node, autoPtr ret);		// ��������Э�ָ̻�������
			bool visit_FunDeclaration(AST* node, autoPtr ret);	// �������ɺ������������
			bool visit_FunctionHeader(AST* node, autoPtr ret);	// �������ɺ�������������
			bool visit_FunctionCall(AST* node, autoPtr ret);	// �������ɺ������ò�����
			bool visit_InterDeclaration(AST* node, autoPtr ret);// �������ɽӿڶ��������
			bool visit_InterfaceHeader(AST* node, autoPtr ret);	// �������ɽӿ�����������
			bool visit_Body(AST* node, autoPtr ret);			// �Դ����Ԫ�ؽ��������������
			bool visit_Program(AST* node, autoPtr ret);			// ����ʼ�ڵ㣬�Կ��������
			bool visit_Empty(AST* node, autoPtr ret);			// pass ����

		private:
			bool m_hasMain;
			AST* m_rootAts;
			SymbolTable * m_symbol_table;
		};
	}
}

#endif

