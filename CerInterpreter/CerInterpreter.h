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
		* ������ ���ʵ�֣�������ʵ�ִ���������
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
			ByteCodes& CompileCode(autoPtr ret, AST* rootAts = nullptr);	// ����ʵ�ֽӿ�
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
			bool visit_YieldOp(AST* node, autoPtr ret);			// ��������Э�̹��������
			bool visit_ResumeOp(AST* node, autoPtr ret);		// ��������Э�ָ̻�������
			bool visit_ReturnOp(AST* node, autoPtr ret);		// �������ɷ������������
			bool visit_FunDeclaration(AST* node, autoPtr ret);	// �������ɺ������������
			bool visit_FunctionHeader(AST* node, autoPtr ret);	// �������ɺ�������������
			bool visit_FunctionCall(AST* node, autoPtr ret);	// �������ɺ������ò�����
			bool visit_InterDeclaration(AST* node, autoPtr ret);// �������ɽӿڶ��������
			bool visit_InterfaceHeader(AST* node, autoPtr ret);	// �������ɽӿ�����������
			bool visit_Body(AST* node, autoPtr ret);			// �Դ����Ԫ�ؽ��������������
			bool visit_Program(AST* node, autoPtr ret);			// ����ʼ�ڵ㣬�Կ��������
			bool visit_Empty(AST* node, autoPtr ret);			// pass ����

			std::vector<CommandCode>* GetSelfVmCodes();

			void PushCode(CommandCode);

			AST::AstType analysExprOp(AST*);
			void printCode();

			std::map<std::string, int> getTables();

		private:
			ByteCodes m_vm_code;								// �����뼯��
			AST* m_rootAts;									// AST�ڵ�
			std::map<std::string,int> m_table_temp;			// ��ʶ����
			std::string m_file_name;						// ��ǰ���ڱ�����ļ���
		};
	}
}


#endif

