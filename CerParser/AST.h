#pragma once
#ifndef _CERVER_ASTTREE_
#define _CERVER_ASTTREE_

#include <map>

#include "..\ErrorHandling\ErrorHandling.h"
#include "..\CerLexical\CerLexical.h"

namespace Cervice {
	namespace Obj {

		class AST;
		class Num;
		class CerTokClass;

		/*********************
		* AST ���ڵ�
		* ���г����﷨�� ����
		* ���ɳ����﷨��
		*********************/
		class AST
		{
		public:
			enum class AstType	// �ڵ�����
			{	
				None = 0,				// Ĭ���޽ڵ�
				Num,					// �����ڵ�
				Var,					// �����ڵ�
				Str,					// �ַ�������
				InterNew,				// new�ڵ�
				ArrayVar,				// ����ڵ�
				InterExprOp,			// �ӿڽڵ�
				indexExprOp,			// �±�ڵ�
				BinOp,					// ��Ԫ�����ڵ�
				UnaryOp,				// һԪ�����ڵ�
				exprOp,					// ���ʽ�ڵ�
				ProgramAst,				// ����ʼ�ڵ�
				BodyAst,				// �����ڵ�
				AssignOp,				// ��ֵ�����ڵ�
				EmptyAst,				// ����Ϊ�ڵ�
				FuncDelOp,				// ��������
				FuncHeader,				// ����ͷ
				InterfaceDecOp,			// �ӿ�ʵ��
				InterfaceHeaderOp,		// �ӿ�ͷ
				FuncCall,				// ִ�к���
				ReturnAst,				// ���غ���
				IfExpr,					// �����жϱ��ʽ
				WhileExpr,				// ����ѭ�����ʽ
				BreakOp,				// �����ؼ���
				ContinueOp,				// �����ؼ���
				ForExpr,				// �����ؼ���
				IncludeFile,			// ����Դ�ļ�
			};

		public:
			AST();
			virtual ~AST();
			
			AstType getNodeType();  // ��ȡ�ڵ�����

			CerTokClass::DebugInfo getDebugInfo();

		protected:
			CerTokClass::DebugInfo m_info;
			AstType m_ast_type;		// �ڵ�����
		};

		using AstNodeType = AST::AstType;

		/*****************
		* Num :public AST
		* �����ڵ�
		*****************/

		class Num :public AST 
		{
		public:
			Num(CerTokClass tok);
			~Num();

			CerTokClass getTok();

		private:
			CerTokClass m_tok;
		};

		/*****************
		* BreakOp :public AST
		* �����ؼ���
		*****************/
		class BreakOp :public AST
		{
		public:
			BreakOp(CerTokClass tok);
			~BreakOp();

			CerTokClass getTok();

		private:
			CerTokClass m_tok;
		};



		/*****************
		* ContinueOp :public AST
		* �����ؼ���
		*****************/
		class ContinueOp :public AST
		{
		public:
			ContinueOp(CerTokClass tok);
			~ContinueOp();

			CerTokClass getTok();

		private:
			CerTokClass m_tok;
		};

		/*****************
		* Var :public AST
		* �����ڵ�
		*****************/

		class Var :public AST
		{
		public:
			Var(CerTokClass tok);
			~Var();

			CerTokClass getTok();

		private:
			CerTokClass m_tok;
		};

		/*****************
		* Str :public AST
		* �ַ����ڵ�
		*****************/

		class Str :public AST
		{
		public:
			Str(CerTokClass tok);
			~Str();

			CerTokClass getTok();

		private:
			CerTokClass m_tok;
		};

		/*****************
		* IncludeFile :public AST
		* �ַ����ڵ�
		*****************/

		class IncludeFile :public AST
		{
		public:
			IncludeFile(CerTokClass file);
			~IncludeFile();

			CerTokClass getFile();

		private:
			CerTokClass m_file;
		};


		/*****************
		* InterNew :public AST
		* New�ڵ�
		*****************/

		class InterNew :public AST
		{
		public:
			InterNew(CerTokClass name);
			~InterNew();

			CerTokClass getName();

		private:
			CerTokClass m_inter_name;
		};

		/*****************
		* ArrayVar :public AST
		* ����ڵ�
		*****************/

		class ArrayVar :public AST
		{
		public:
			ArrayVar(std::vector<AST*> expr);
			~ArrayVar();

			std::vector<AST*> getExpr();
			void setOnlyIndex(bool isindex);
			bool gethasOnlyIndex();
			void setLeftIndex(bool isindex);
			bool gethasLeftIndex();

		private:
			std::vector<AST*> m_expr;
			bool m_only_index;
			bool m_left_index;
		};

		/*****************
		* InterExprOp :public AST
		* �ӿ��±�ڵ�
		*****************/

		class InterExprOp :public AST
		{
		public:
			InterExprOp(AST* person);
			~InterExprOp();

			void setLeftIndex(bool isindex);
			bool gethasLeftIndex();

			AST* getPerson();

		private:
			AST* m_person;
			bool m_left_index;
		};

		/*****************
		* indexExprOp :public AST
		* �������ʽ�±�ڵ�
		*****************/

		class indexExprOp :public AST
		{
		public:
			indexExprOp(std::vector<AST*> indes);
			~indexExprOp();

			size_t getFuncSize();
			size_t getInterSize();

			void setFuncSize(size_t);
			void setInterSize(size_t);

			std::vector<AST*> getIndexs();
		private:
			std::vector<AST*> m_indexs;
			size_t m_func_size;
			size_t m_inte_size;
		};

		/*****************
		* IfExpr :public AST
		* �����жϱ��ʽ
		*****************/

		class IfExpr :public AST
		{
		public:
			IfExpr(std::pair<AST*, std::vector<AST*>> _if , 
				std::vector<std::pair<AST*, std::vector<AST*>>> _elifs ,
				std::pair<AST*, std::vector<AST*>> _else);
			~IfExpr();
			
			std::pair<AST*, std::vector<AST*>> getIf();						// ��� if �ڵ�
			std::vector<std::pair<AST*, std::vector<AST*>>> getElifs();		// ��� elif ���нڵ�
			std::pair<AST*, std::vector<AST*>> getElse();					// ��� else �ڵ�

		private:
			std::pair<AST*,std::vector<AST*>> m_if;
			std::vector<std::pair<AST*, std::vector<AST*>>> m_elifs;
			std::pair<AST*, std::vector<AST*>> m_else;
		};


		/*****************
		* WhileExpr :public AST
		* ����ѭ�����ʽ
		*****************/

		class WhileExpr :public AST
		{
		public:
			WhileExpr(AST* _expr , std::vector<AST*> _codebody);
			~WhileExpr();

			AST* getExpr();							// ��� expr �ڵ�
			std::vector<AST*> getCodes();			// ��� code ���нڵ�

		private:
			AST* m_expr;
			std::vector<AST*> m_codes;
		};


		/*****************
		* ForExpr :public AST
		* ����ѭ�����ʽ
		*****************/

		class ForExpr :public AST
		{
		public:
			ForExpr(CerTokClass _var,AST* _expr, std::vector<AST*> _codebody);
			~ForExpr();

			CerTokClass getVar();					// ��� for ����
			AST* getExpr();							// ��� expr �ڵ�
			std::vector<AST*> getCodes();			// ��� code ���нڵ�

		private:
			CerTokClass m_var;
			AST* m_expr;
			std::vector<AST*> m_codes;
		};

		/********************
		* BinOp : public AST
		* ��Ԫ�����ڵ�
		*********************/

		class BinOp : public AST 
		{
		public:
			BinOp(AST* ,CerTokClass,AST*);
			~BinOp();

			AST* getLeft();
			CerTokClass getTok();
			AST* getRight();

		private:
			AST* m_left;		// ������ڵ�
			AST* m_right;		// �Ҳ����ڵ�
			CerTokClass m_op;	// ��������
		};

		/********************
		* UnaryOp : public AST
		* һԪ�����ڵ�
		*********************/

		class UnaryOp : public AST
		{
		public:
			UnaryOp(CerTokClass, AST*);
			~UnaryOp();

			CerTokClass getOpType();
			AST* getRight();

		private:
			AST* m_right;			// �Ҳ����ڵ�
			CerTokClass m_op;	// ��������
		};

		/********************
		* exprOp : public AST
		* ���ʽ�ڵ�
		*********************/

		class exprOp : public AST
		{
		public:
			exprOp(AST* expr,AST* index);
			~exprOp();

			AST* getExpr();
			AST* getIndex();
			bool getHasIndex();
			bool getHasLeft();

		private:
			AST* m_expr;			// ���ʽ
			AST* m_index;			// �±�����
			bool m_has_index;		// �Ƿ����±�
			bool m_has_left;		// �Ƿ���ֵ
		};

		/********************
		* ProgramAst : public AST
		* ����ʼ�ڵ�
		*********************/

		class ProgramAst : public AST
		{
		public:
			ProgramAst(AST* body);
			~ProgramAst();

			AST* getBody();

		private:
			AST* m_body;	// ÿ�����򼯺�
		};


		/********************
		* BodyAst : public AST
		* ���򼯺�Body
		*********************/

		class BodyAst : public AST
		{
		public:
			BodyAst(std::vector<AST*>& block);
			~BodyAst();

			std::vector<AST*> getBlock();

		private:
			std::vector<AST*> m_block;	// ÿ�����򼯺�
		};

		/********************
		* AssignOp : public AST
		* ��ֵ������
		*********************/

		class AssignOp : public AST
		{
		public:
			AssignOp(CerTokClass, AST*,AST*, bool);
			~AssignOp();

			CerTokClass getLeft();
			AST* getRight();
			AST* getLeftIndex();
			bool getCreate();

			void setLeftName(std::string);

		private:
			CerTokClass m_left_var;		// ������
			AST* m_left_index;			// ��ֵ�±�
			AST* m_right_expr;			// ��ֵexpr
			bool m_create;				// �Ƿ�ʹ��let�ֶ�

		};

		/********************
		* FuncHeader : public AST
		* ���������ڵ�
		*********************/

		class FuncHeader : public AST
		{
		public:
			FuncHeader(CerTokClass, std::vector<CerTokClass>, bool hasexport = false);
			~FuncHeader();

			CerTokClass getTok();
			std::vector<CerTokClass> getParams();
			std::string getFuncName();
			void setHasClass(bool);
			bool gethasClass();

			void setFuncName(std::string);
			bool gethasExport();

		private:
			bool m_hasClass;
			bool m_hasExport;
			std::vector<CerTokClass> m_params;	// ��������
			std::string m_func_name;			// ������
			CerTokClass m_func_tok;				// ���溯��tok

		};

		/********************
		* FuncCall : public AST
		* ���������ڵ�
		*********************/

		class FuncCall : public AST
		{
		public:
			FuncCall(CerTokClass, std::vector<AST*>);
			~FuncCall();

			CerTokClass getTok();
			std::vector<AST*> getParams();
			std::string getFuncName();
			void setthisName(std::string name);
			std::string getthisName();

		private:
			std::string m_this_name;
			std::vector<AST*> m_params;	// ��������
			std::string m_func_name;			// ������
			CerTokClass m_func_tok;				// ���溯��tok

		};

		/********************
		* FuncDelOp : public AST
		* ����ʵ�ֽڵ�
		*********************/

		class FuncDelOp : public AST
		{
		public:
			FuncDelOp(AST* header , std::vector<AST*> codes);
			~FuncDelOp();

			AST* getHeader();
			std::vector<AST*> getCode();

		private:
			AST* m_func_header;				// ���������ڵ�
			std::vector<AST*> m_func_codes;	// ʵ�ִ���

		};

		/********************
		* InterfaceDecOp : public AST
		* �ӿ������ڵ�
		*********************/

		class InterfaceDecOp : public AST
		{
		public:
			InterfaceDecOp(AST* header, std::vector<AST*> datas);
			~InterfaceDecOp();

			AST* getHeader();
			std::vector<AST*> getDatas();

		private:
			AST* m_interface_header;				// �ӿ������ڵ�
			std::vector<AST*> m_interface_data;		// �ӿ��ڲ�����
		};

		/********************
		* InterfaceHeaderOp : public AST
		* �ӿ������ڵ�
		*********************/

		class InterfaceHeaderOp : public AST
		{
		public:
			InterfaceHeaderOp(CerTokClass name, CerTokClass parent);
			~InterfaceHeaderOp();

			CerTokClass getName();
			CerTokClass getParent();

		private:
			CerTokClass m_name;				// �ӿ������ڵ�
			CerTokClass m_parent;			// �ӿ��ڲ�����

		};

		/********************
		* ReturnAst : public AST
		* ����ʼ�ڵ�
		*********************/

		class ReturnAst : public AST
		{
		public:
			ReturnAst(AST* expr);
			~ReturnAst();

			AST* getExpr();

		private:
			AST* m_expr;	// ���ر��ʽ
		};

		/********************
		* Empty : public AST
		* ��Pass
		*********************/
		class EmptyAst : public AST {
		public:
			EmptyAst();
			~EmptyAst();
		};
	}
}


#endif

