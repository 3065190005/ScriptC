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
			None = 0,		// ��ʼ
			BaseSymbol,		// ��������
			VarSymbol,		// ��������
			FuncSymbol,		// ��������
			InterSymbol,	// �ӿڷ���
		};

		/***************************
		* class SymbolClass
		* ������,�������ţ���������
		***************************/

		class SymbolClass
		{
		public:
			SymbolClass(std::string name);									// ������������
			SymbolClass(std::string name, SymbolClass type);				// ������������
			SymbolClass(std::string func_name, std::vector<std::string>);	// ������������
			SymbolClass(std::string inter_name, std::string parent);		// ������������

			SymbolType getType();
			std::string getName();
			std::vector<std::string> getParams();

		private:
			SymbolType m_symbol_type;		// ����
			std::string m_symbol_name;		// ����

			std::vector<std::string>m_symbol_params;	// ����
			std::string m_symbol_inter_parent;			// ���ӿ�
		};

		/***************************
		* class SymbolTable
		* ���ű����վֲ�����
		***************************/

		class SymbolTable {
		public:
			SymbolTable(SymbolTable* parent, std::string symbol_name,size_t level);
			~SymbolTable();

			// ��Χ
			enum class SymbolfindArea {
				noFind,	// δ�ҵ�
				Self,	// ��ǰ��
				Parent	// ����
			};

			// ���ҷ��� �������� �� �������� �� �Ƿ�ݹ���ң�
			SymbolfindArea findSymbol(std::string name, SymbolType type, bool recursion);
			SymbolClass getFuncSymbol(std::string name, bool recursion);

			// ��ӷ��� ���ű���
			void pushSymbol(SymbolClass symbol);

			// ���ø��ڵ�
			void setParent(SymbolTable *);
			SymbolTable* getParent();
			size_t getLevel();
			std::string getName();

		private:
			SymbolTable* m_parent;				// ����
			std::vector<SymbolClass> m_tables;	// ��ǰ�����з���
			std::string m_symtab_name;			// ���ű�����
			size_t m_symbol_level;				// ���ŵ�ǰ�㼶
		};

		using symbArea = SymbolTable::SymbolfindArea;
	}
}

#endif

