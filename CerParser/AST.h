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
		* AST 根节点
		* 所有抽象语法树 基类
		* 生成抽象语法树
		*********************/
		class AST
		{
		public:
			enum class AstType	// 节点类型
			{	
				None = 0,				// 默认无节点
				Num,					// 常量节点
				Var,					// 变量节点
				Str,					// 字符串变量
				InterNew,				// new节点
				ArrayVar,				// 数组节点
				InterExprOp,			// 接口节点
				indexExprOp,			// 下标节点
				BinOp,					// 二元操作节点
				UnaryOp,				// 一元操作节点
				exprOp,					// 表达式节点
				ProgramAst,				// 程序开始节点
				BodyAst,				// 代码块节点
				AssignOp,				// 赋值操作节点
				EmptyAst,				// 无行为节点
				FuncDelOp,				// 函数声明
				FuncHeader,				// 函数头
				InterfaceDecOp,			// 接口实现
				InterfaceHeaderOp,		// 接口头
				FuncCall,				// 执行函数
				ReturnAst,				// 返回函数
				IfExpr,					// 条件判断表达式
				WhileExpr,				// 条件循环表达式
				BreakOp,				// 跳出关键字
				ContinueOp,				// 继续关键字
				ForExpr,				// 继续关键字
				IncludeFile,			// 导入源文件
			};

		public:
			AST();
			virtual ~AST();
			
			AstType getNodeType();  // 获取节点类型

			CerTokClass::DebugInfo getDebugInfo();

		protected:
			CerTokClass::DebugInfo m_info;
			AstType m_ast_type;		// 节点类型
		};

		using AstNodeType = AST::AstType;

		/*****************
		* Num :public AST
		* 常量节点
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
		* 跳出关键字
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
		* 继续关键字
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
		* 变量节点
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
		* 字符串节点
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
		* 字符串节点
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
		* New节点
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
		* 数组节点
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
		* 接口下标节点
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
		* 辅助表达式下标节点
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
		* 条件判断表达式
		*****************/

		class IfExpr :public AST
		{
		public:
			IfExpr(std::pair<AST*, std::vector<AST*>> _if , 
				std::vector<std::pair<AST*, std::vector<AST*>>> _elifs ,
				std::pair<AST*, std::vector<AST*>> _else);
			~IfExpr();
			
			std::pair<AST*, std::vector<AST*>> getIf();						// 获得 if 节点
			std::vector<std::pair<AST*, std::vector<AST*>>> getElifs();		// 获得 elif 所有节点
			std::pair<AST*, std::vector<AST*>> getElse();					// 获得 else 节点

		private:
			std::pair<AST*,std::vector<AST*>> m_if;
			std::vector<std::pair<AST*, std::vector<AST*>>> m_elifs;
			std::pair<AST*, std::vector<AST*>> m_else;
		};


		/*****************
		* WhileExpr :public AST
		* 条件循环表达式
		*****************/

		class WhileExpr :public AST
		{
		public:
			WhileExpr(AST* _expr , std::vector<AST*> _codebody);
			~WhileExpr();

			AST* getExpr();							// 获得 expr 节点
			std::vector<AST*> getCodes();			// 获得 code 所有节点

		private:
			AST* m_expr;
			std::vector<AST*> m_codes;
		};


		/*****************
		* ForExpr :public AST
		* 数组循环表达式
		*****************/

		class ForExpr :public AST
		{
		public:
			ForExpr(CerTokClass _var,AST* _expr, std::vector<AST*> _codebody);
			~ForExpr();

			CerTokClass getVar();					// 获得 for 变量
			AST* getExpr();							// 获得 expr 节点
			std::vector<AST*> getCodes();			// 获得 code 所有节点

		private:
			CerTokClass m_var;
			AST* m_expr;
			std::vector<AST*> m_codes;
		};

		/********************
		* BinOp : public AST
		* 二元操作节点
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
			AST* m_left;		// 左操作节点
			AST* m_right;		// 右操作节点
			CerTokClass m_op;	// 操作符号
		};

		/********************
		* UnaryOp : public AST
		* 一元操作节点
		*********************/

		class UnaryOp : public AST
		{
		public:
			UnaryOp(CerTokClass, AST*);
			~UnaryOp();

			CerTokClass getOpType();
			AST* getRight();

		private:
			AST* m_right;			// 右操作节点
			CerTokClass m_op;	// 操作符号
		};

		/********************
		* exprOp : public AST
		* 表达式节点
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
			AST* m_expr;			// 表达式
			AST* m_index;			// 下标索引
			bool m_has_index;		// 是否有下标
			bool m_has_left;		// 是否左赋值
		};

		/********************
		* ProgramAst : public AST
		* 程序开始节点
		*********************/

		class ProgramAst : public AST
		{
		public:
			ProgramAst(AST* body);
			~ProgramAst();

			AST* getBody();

		private:
			AST* m_body;	// 每条程序集合
		};


		/********************
		* BodyAst : public AST
		* 程序集合Body
		*********************/

		class BodyAst : public AST
		{
		public:
			BodyAst(std::vector<AST*>& block);
			~BodyAst();

			std::vector<AST*> getBlock();

		private:
			std::vector<AST*> m_block;	// 每条程序集合
		};

		/********************
		* AssignOp : public AST
		* 赋值操作数
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
			CerTokClass m_left_var;		// 变量名
			AST* m_left_index;			// 左值下标
			AST* m_right_expr;			// 赋值expr
			bool m_create;				// 是否使用let字段

		};

		/********************
		* FuncHeader : public AST
		* 函数声明节点
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
			std::vector<CerTokClass> m_params;	// 函数参数
			std::string m_func_name;			// 函数名
			CerTokClass m_func_tok;				// 保存函数tok

		};

		/********************
		* FuncCall : public AST
		* 函数声明节点
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
			std::vector<AST*> m_params;	// 函数参数
			std::string m_func_name;			// 函数名
			CerTokClass m_func_tok;				// 保存函数tok

		};

		/********************
		* FuncDelOp : public AST
		* 函数实现节点
		*********************/

		class FuncDelOp : public AST
		{
		public:
			FuncDelOp(AST* header , std::vector<AST*> codes);
			~FuncDelOp();

			AST* getHeader();
			std::vector<AST*> getCode();

		private:
			AST* m_func_header;				// 函数声明节点
			std::vector<AST*> m_func_codes;	// 实现代码

		};

		/********************
		* InterfaceDecOp : public AST
		* 接口声明节点
		*********************/

		class InterfaceDecOp : public AST
		{
		public:
			InterfaceDecOp(AST* header, std::vector<AST*> datas);
			~InterfaceDecOp();

			AST* getHeader();
			std::vector<AST*> getDatas();

		private:
			AST* m_interface_header;				// 接口声明节点
			std::vector<AST*> m_interface_data;		// 接口内部代码
		};

		/********************
		* InterfaceHeaderOp : public AST
		* 接口声明节点
		*********************/

		class InterfaceHeaderOp : public AST
		{
		public:
			InterfaceHeaderOp(CerTokClass name, CerTokClass parent);
			~InterfaceHeaderOp();

			CerTokClass getName();
			CerTokClass getParent();

		private:
			CerTokClass m_name;				// 接口声明节点
			CerTokClass m_parent;			// 接口内部代码

		};

		/********************
		* ReturnAst : public AST
		* 程序开始节点
		*********************/

		class ReturnAst : public AST
		{
		public:
			ReturnAst(AST* expr);
			~ReturnAst();

			AST* getExpr();

		private:
			AST* m_expr;	// 返回表达式
		};

		/********************
		* Empty : public AST
		* 空Pass
		*********************/
		class EmptyAst : public AST {
		public:
			EmptyAst();
			~EmptyAst();
		};
	}
}


#endif

