#pragma once
#ifndef _CERVER_NODEVISITOR_
#define _CERVER_NODEVISITOR_

#include "QuickLib/quicklib.h"
#include "..\AutoElement\LetManager.h"

namespace Cervice {
	namespace Obj {

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

