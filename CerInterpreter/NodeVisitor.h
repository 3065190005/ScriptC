#pragma once
#ifndef _SCRIPTC_NODEVISITOR_
#define _SCRIPTC_NODEVISITOR_

#include "QuickLib/quicklib.h"
#include "..\AutoElement\LetManager.h"

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;

		class AST;
		class Object;
		class CerInterpreter;
		class SemanticAnalyzer;
		class ErrorHandling;

		/*
		* �����������
		*/
		class NodeVisitor
		{
		public:
			NodeVisitor();
			~NodeVisitor();


		public:	// ʵ�ֽӿڵ��� ���ڵ㣬����ֵ�������ߣ�
			bool visit(AST* node, autoPtr ret,CerInterpreter* interpreter);
			bool visit(AST* node, autoPtr ret, SemanticAnalyzer* semantic);

		protected:
			class ErrorHandling * m_errHis;
		};
	}
}

#endif

