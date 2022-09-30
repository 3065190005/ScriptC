#pragma once
#ifndef _CERVER_SYMBOLCLASS_
#define _CERVER_SYMBOLCLASS_

#include <iostream>
#include <vector>

namespace Cervice {
	namespace Obj {

		class SymbolClass;
		class SymbolTable;

		enum class SymbolType {
			None = 0,		// 初始
			BaseSymbol,		// 基础符号
			VarSymbol,		// 变量符号
			FuncSymbol,		// 函数符号
			InterSymbol,	// 接口符号
		};

		/***************************
		* class SymbolClass
		* 符号类,基础符号，变量符号
		***************************/

		class SymbolClass
		{
		public:
			SymbolClass(std::string name);									// 基础符号声明
			SymbolClass(std::string name, SymbolClass type);				// 变量符号声明
			SymbolClass(std::string func_name, std::vector<std::string>);	// 函数符号声明
			SymbolClass(std::string inter_name, std::string parent);		// 函数符号声明

			SymbolType getType();
			std::string getName();
			std::vector<std::string> getParams();

		private:
			SymbolType m_symbol_type;		// 类型
			std::string m_symbol_name;		// 名字

			std::vector<std::string>m_symbol_params;	// 参数
			std::string m_symbol_inter_parent;			// 父接口
		};

		/***************************
		* class SymbolTable
		* 符号表，掌握局部符号
		***************************/

		class SymbolTable {
		public:
			SymbolTable(SymbolTable* parent, std::string symbol_name,size_t level);
			~SymbolTable();

			// 范围
			enum class SymbolfindArea {
				noFind,	// 未找到
				Self,	// 当前域
				Parent	// 父域
			};

			// 查找符号 （符号名 ， 符号类型 ， 是否递归查找）
			SymbolfindArea findSymbol(std::string name, SymbolType type, bool recursion);
			SymbolClass getFuncSymbol(std::string name, bool recursion);

			// 添加符号 符号本体
			void pushSymbol(SymbolClass symbol);

			// 设置父节点
			void setParent(SymbolTable *);
			SymbolTable* getParent();
			size_t getLevel();
			std::string getName();

		private:
			SymbolTable* m_parent;				// 父表
			std::vector<SymbolClass> m_tables;	// 当前表所有符号
			std::string m_symtab_name;			// 符号表名字
			size_t m_symbol_level;				// 符号当前层级
		};

		using symbArea = SymbolTable::SymbolfindArea;
	}
}

#endif

