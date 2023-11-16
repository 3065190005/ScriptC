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
			AST* Program();									// ����ʼ
			AST* Body();									// �����
			AST* InterfaceDeclaration();					// �ӿ�ʵ��
			AST* InterfaceHeader();							// �ӿڶ���
			AST* FunDeclaration();							// ����ʵ��
			AST* FunctionHeader(bool has_export = false);	// ��������
			AST* FileInclude();								// �ļ�����
			std::vector<AST*> ExpressionBody();				// �������
			AST* Assignment();								// ��ֵ(��ֵ)����
			AST* ReturnExpr();								// ����(��ֵ)����
			AST* ifconditionExpr();							// if�ڵ�
			AST* whileconditionExpr();						// while�ڵ�
			AST* forconditionExpr();						// for�ڵ�
			AST* Empty();									// ��
			AST* indexExpr(bool left = false);				// �±�

			AST* expr();									// ����(��ֵ)����
			AST* singleKey();								// ���ؼ��������
			AST* interNew();								// New �����
			AST* logicaloper_b();							// �߼������b
			AST* logicaloper_a();							// �߼������a
			AST* comparison_b();							// �Ƚ������b
			AST* comparison_a();							// �Ƚ������a
			AST* shiftoperator();							// λ�Ʋ�����
			AST* dualoperation();							// �Ӽ�����
			AST* term();									// �˳�����
			AST* factor();									// ������

			AST* String_const();			// ��ȡ�ַ�������
			AST* Integer_const();			// ��ȡ���γ����򸡵㳣���ڵ�
			AST* Function_Id();				// ��ȡ�����ڵ�
			AST* ArrayExpression();			// ��ȡ����ڵ�
			AST* InterfaceExpression();		// �ӿڳ�Ա����

		public:
			bool isExprBegin();
			bool isAssignBegin();

			// call this function to get AST*
			AST* parser();		// ���з���

		public:
			void takeEat(CerTokType type);	// �Ե�ָ��type

		public:


		private:
			CerLexical<char>* m_lexical;
		};
	}
}

#endif

