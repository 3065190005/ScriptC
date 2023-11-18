#include "..\AutoElement\LetManager.h"
#include "..\AutoElement\LetObject.h"
#include "..\CerLexical\CerLexical.h"
#include "..\CerParser\AST.h"
#include "..\ErrorHandling\ErrorHandling.h"
#include "CerInterpreter.h"
#include "SemanticAnalyzer.h"


using namespace ScriptC::Obj;

ScriptC::Obj::NodeVisitor::NodeVisitor()
{
	m_errHis = ErrorHandling::getInstance();
}

ScriptC::Obj::NodeVisitor::~NodeVisitor()
{
}

// 节点，返回值，处理者 -> 编译器
bool ScriptC::Obj::NodeVisitor::visit(AST* node,  autoPtr ret,CerInterpreter* interpreter)
{
	switch (node->getNodeType())
	{
	case AstNodeType::Num:
		interpreter->visit_Num(node,ret);
		break;
	case AstNodeType::Str:
		interpreter->visit_Str(node, ret);
		break;
	case AstNodeType::Var:
		interpreter->visit_Var(node, ret);
		break;
	case AstNodeType::BreakOp:
		interpreter->visit_BreakOp(node, ret);
		break;
	case AstNodeType::ContinueOp:
		interpreter->visit_ContinueOp(node, ret);
		break;
	case AstNodeType::InterNew:
		interpreter->visit_InterNew(node, ret);
		break;
	case AstNodeType::ArrayVar:
		interpreter->visit_ArrayVar(node, ret);
		break;
	case AstNodeType::InterExprOp:
		interpreter->visit_InterExprOp(node, ret);
		break;
	case AstNodeType::BinOp:
		interpreter->visit_BinOp(node,ret);
		break;
	case AstNodeType::IfExpr:
		interpreter->visit_IfExpr(node, ret);
		break;
	case AstNodeType::WhileExpr:
		interpreter->visit_WhileExpr(node, ret);
		break;
	case AstNodeType::IncludeFile:
		interpreter->visit_IncludeFile(node, ret);
		break;
	case AstNodeType::ForExpr:
		interpreter->visit_ForExpr(node, ret);
		break;
	case AstNodeType::UnaryOp:
		interpreter->visit_UnaryOp(node, ret);
		break;
	case AstNodeType::AssignOp:
		interpreter->visit_AssignOp(node, ret);
		break;
	case AstNodeType::indexExprOp:
		interpreter->visit_indexExprOp(node, ret);
		break;
	case AstNodeType::exprOp:
		interpreter->visit_exprOp(node, ret);
		break;
	case AstNodeType::ProgramAst:
		interpreter->visit_Program(node, ret);
		break;
	case AstNodeType::BodyAst:
		interpreter->visit_Body(node, ret);
		break;
	case AstNodeType::InterfaceDecOp:
		interpreter->visit_InterDeclaration(node, ret);
		break;
	case AstNodeType::InterfaceHeaderOp:
		interpreter->visit_InterfaceHeader(node, ret);
		break;
	case AstNodeType::FuncDelOp:
		interpreter->visit_FunDeclaration(node, ret);
		break;
	case AstNodeType::FuncHeader:
		interpreter->visit_FunctionHeader(node, ret);
		break;
	case AstNodeType::FuncCall:
		interpreter->visit_FunctionCall(node, ret);
		break;
	case AstNodeType::ReturnAst:
		interpreter->visit_ReturnOp(node, ret);
		break;
	case AstNodeType::YieldOp:
		interpreter->visit_YieldOp(node, ret);
		break;
	case AstNodeType::ResumeOp:
		interpreter->visit_ResumeOp(node, ret);
		break;
	case AstNodeType::EmptyAst:
		interpreter->visit_Empty(node, ret);
		break;
	default:
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "Unknown AST node");
	}

	if (!ret || ret->getType() == LetObject::ObjT::null) {
		return false;
	}

	return true;
}

// 节点，返回值，处理者 -> 语义分析
bool ScriptC::Obj::NodeVisitor::visit(AST* node, autoPtr ret, SemanticAnalyzer* semantic)
{
	switch (node->getNodeType())
	{
	case AstNodeType::Num:
		semantic->visit_Num(node, ret);
		break;
	case AstNodeType::Str:
		semantic->visit_Str(node, ret);
		break;
	case AstNodeType::Var:
		semantic->visit_Var(node, ret);
		break;
	case AstNodeType::BreakOp:
		semantic->visit_BreakOp(node, ret);
		break;
	case AstNodeType::ContinueOp:
		semantic->visit_ContinueOp(node, ret);
		break;
	case AstNodeType::InterNew:
		semantic->visit_InterNew(node, ret);
		break;
	case AstNodeType::ArrayVar:
		semantic->visit_ArrayVar(node, ret);
		break;
	case AstNodeType::InterExprOp:
		semantic->visit_InterExprOp(node, ret);
		break;
	case AstNodeType::IncludeFile:
		semantic->visit_IncludeFile(node, ret);
		break;
	case AstNodeType::BinOp:
		semantic->visit_BinOp(node, ret);
		break;
	case AstNodeType::IfExpr:
		semantic->visit_IfExpr(node, ret);
		break;
	case AstNodeType::WhileExpr:
		semantic->visit_WhileExpr(node, ret);
		break;
	case AstNodeType::ForExpr:
		semantic->visit_ForExpr(node, ret);
		break;
	case AstNodeType::UnaryOp:
		semantic->visit_UnaryOp(node, ret);
		break;
	case AstNodeType::AssignOp:
		semantic->visit_AssignOp(node, ret);
		break;
	case AstNodeType::indexExprOp:
		semantic->visit_indexExprOp(node, ret);
		break;
	case AstNodeType::exprOp:
		semantic->visit_exprOp(node, ret);
		break;
	case AstNodeType::ReturnAst:
		semantic->visit_ReturnOp(node, ret);
		break;
	case AstNodeType::InterfaceDecOp:
		semantic->visit_InterDeclaration(node, ret);
		break;
	case AstNodeType::InterfaceHeaderOp:
		semantic->visit_InterfaceHeader(node, ret);
		break;
	case AstNodeType::FuncDelOp:
		semantic->visit_FunDeclaration(node, ret);
		break;
	case AstNodeType::FuncHeader:
		semantic->visit_FunctionHeader(node, ret);
		break;
	case AstNodeType::FuncCall:
		semantic->visit_FunctionCall(node, ret);
		break;
	case AstNodeType::ProgramAst:
		semantic->visit_Program(node, ret);
		break;
	case AstNodeType::BodyAst:
		semantic->visit_Body(node, ret);
		break;
	case AstNodeType::EmptyAst:
		semantic->visit_Empty(node, ret);
		break;
	case AstNodeType::YieldOp:
		semantic->visit_YieldOp(node, ret);
		break;
	case AstNodeType::ResumeOp:
		semantic->visit_ResumeOp(node, ret);
		break;
	default:
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "Unknown AST node");
		break;
	}

	if (ret->getType() == LetObject::ObjT::null) {
		return false;
	}

	return true;
}