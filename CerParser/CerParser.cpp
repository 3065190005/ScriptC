#include "..\ErrorHandling\ErrorHandling.h"
#include "..\CerLexical\CerLexical.h"
#include "AST.h"
#include "CerParser.h"
#include "CerParserMacro.h"

extern struct CerTokClass::DebugInfo G_Global_Debug_Info;


ScriptC::Obj::CerParser::CerParser(CerLexical<char>* lexical) :
	m_lexical(lexical)
{
}

ScriptC::Obj::CerParser::~CerParser()
{

}

CerParser* ScriptC::Obj::CerParser::create(CerLexical<char>* lex)
{
	CerParser* ret = new CerParser(lex);
	if (lex == nullptr || ret == nullptr) {
		delete ret;
		ret = nullptr;
	}

	return ret;
}

AST* ScriptC::Obj::CerParser::expr()
{
	/*
	*	expr: singleKey | logicaloper_b
	*/

	AST* result = nullptr;
	result = singleKey();
	if (result != nullptr) {
		return result;
	}

	AST* index_ptr = nullptr;
	AST* expr_ptr = logicaloper_b();
	CerTokClass& tok = m_lexical->getCurrentToken();

	if (tok.getType() == CerTokType::LBRACKET || tok.getType() == CerTokType::DOT) {
		index_ptr = indexExpr();
	}

	result = new exprOp(expr_ptr, index_ptr);

	astlog(" - exprOp()\n");
	return result;
}

AST* ScriptC::Obj::CerParser::ifconditionExpr()
{
	/*
	* ifconditionExpr : KEY_IF expr COLON ExpressionBody
	*					(KEY_ELIF expr COLON ExpressionBody)*
	*					(KEY_ELSE COLON ExpressionBody)?
	*					KEY_END
	*/

	AST* result = nullptr;

	using _ifstruct = std::pair<AST*, std::vector<AST*>>;

	_ifstruct _if = { nullptr , {} };
	std::vector<_ifstruct> _elifs = {};
	_ifstruct _else = { nullptr , {} };

	takeEat(CerTokType::Key_If);
	_if.first = expr();
	takeEat(CerTokType::COLON);
	_if.second = ExpressionBody();

	CerTokClass& tok = m_lexical->getCurrentToken();
	while (tok.getType() == CerTokType::Key_Elif) {
		_ifstruct _elif;
		takeEat(CerTokType::Key_Elif);
		_elif.first = expr();
		takeEat(CerTokType::COLON);
		_elif.second = ExpressionBody();
		_elifs.emplace_back(_elif);
	}

	tok = m_lexical->getCurrentToken();
	if (tok.getType() == CerTokType::Key_Else) {
		takeEat(CerTokType::Key_Else);
		_else.first = new Num({ "true",CerTokType::Key_True });
		takeEat(CerTokType::COLON);
		_else.second = ExpressionBody();
	}

	takeEat(CerTokType::Key_End);

	result = new IfExpr(std::move(_if), std::move(_elifs), std::move(_else));

	return result;
}

AST* ScriptC::Obj::CerParser::whileconditionExpr() {
	/*
	* whileconditionExpr: KEY_WHILE expr COLON ExpressionBody KEY_END
	*/


	AST* result;

	takeEat(CerTokType::Key_While);
	CerTokClass& tok = m_lexical->getCurrentToken();
	AST* _expr = expr();
	takeEat(CerTokType::COLON);
	std::vector<AST*> _codes = ExpressionBody();
	takeEat(CerTokType::Key_End);

	result = new WhileExpr(std::move(_expr), std::move(_codes));

	return result;
}


AST* ScriptC::Obj::CerParser::forconditionExpr() {
	/*
	* forconditionExpr : KEY_FOR VAR_ID KEY_IN expr COLON ExpressionBody KEY_END
	*/
	CerTokClass _var;
	AST* _expr = nullptr;
	std::vector<AST*> _codes;
	AST* result = nullptr;

	takeEat(CerTokType::Key_For);
	CerTokClass& tok = m_lexical->getCurrentToken();
	CerTokClass::copy(_var, tok);
	takeEat(CerTokType::Var_Id);
	takeEat(CerTokType::Key_In);
	_expr = expr();
	takeEat(CerTokType::COLON);
	_codes = ExpressionBody();
	takeEat(CerTokType::Key_End);
	
	result = new ForExpr(_var, _expr, std::move(_codes));
	astlog(" - ForExpr()\n");

	return result;
}


AST* ScriptC::Obj::CerParser::indexExpr(bool left)
{
	/*
	*	indexExpr  : (ArrayExpr | InterExpr)+
	*/

	std::vector<AST*> vec;
	int funcIndex = 0;
	int interIndex = 0;
	CerTokClass& tok = m_lexical->getCurrentToken();
	do {
		if (tok.getType() == CerTokType::LBRACKET) {
			// array
			ArrayVar* ele = dynamic_cast<ArrayVar*>(ArrayExpression());
			ele->setOnlyIndex(true);
			ele->setLeftIndex(true);
			vec.push_back(ele);
		}
		else if (tok.getType() == CerTokType::DOT) {
			// interface
			InterExprOp* ele = dynamic_cast<InterExprOp*>(InterfaceExpression());
			ele->setLeftIndex(true);
			if (ele->getPerson()->getNodeType() == AST::AstType::FuncCall)
			{
				funcIndex++;
				ele->setLeftIndex(false);
			}
			else
				interIndex++;
			vec.push_back(ele);
		}

		tok = m_lexical->getCurrentToken();
	} while (tok.getType() == CerTokType::LBRACKET || tok.getType() == CerTokType::DOT);
	
	/*
	* 2023.10.17
	* 左右值将在末尾或函数调用后进行拷贝，其他则都是引用
	* 2023.11.2
	* 将最后一个左值接口下表设置标记
	* 用来替换赋值操作的左值调用_attr跳过pop
	*/
	AST* back_ast = vec.back();
	if (back_ast->getNodeType() == AST::AstType::ArrayVar)
	{
		auto last_ast = dynamic_cast<ArrayVar*>(back_ast);
		last_ast->setLeftIndex(left);
	}
	else if (back_ast->getNodeType() == AST::AstType::InterExprOp)
	{
		auto last_ast = dynamic_cast<InterExprOp*>(back_ast);
		last_ast->setLeftIndex(left);
		last_ast->setLeftLast(true);
	}

	astlog(" - InterExprOp()\n");
	indexExprOp* result = new indexExprOp(std::move(vec));
	result->setFuncSize(funcIndex);
	result->setInterSize(interIndex);
	return result;
}



AST* ScriptC::Obj::CerParser::logicaloper_b()
{
	/*
	* logicaloper_b : logicaloper_a ((PlusDOR) logicaloper_a)*
	*/

	AST* result = nullptr;
	result = logicaloper_a();

	CerTokClass& tok = m_lexical->getCurrentToken();
	CerTokClass tokc("", CerTokType::None);
	CerTokClass::copy(tokc, tok);

	while (tok.getType() == CerTokType::PlusDOR)
	{
		CerTokClass::copy(tokc, tok);
		AST* result2 = nullptr;
		if (tok.getType() == CerTokType::PlusDOR)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusDOR);
			result2 = logicaloper_a();
		}

		result = new BinOp(result, tokc, result2);
	}
	return result;

}


AST* ScriptC::Obj::CerParser::logicaloper_a()
{
	/*
	* logicaloper_a : comparison_b ((PlusAND|PlusXOR|PlusOR|PlusDAND) comparison_b)*
	*/

	AST* result = nullptr;
	result = comparison_b();

	CerTokClass& tok = m_lexical->getCurrentToken();
	CerTokClass tokc("", CerTokType::None);
	CerTokClass::copy(tokc, tok);

	while (tok.getType() == CerTokType::PlusAND ||
		tok.getType() == CerTokType::PlusXOR ||
		tok.getType() == CerTokType::PlusOR ||
		tok.getType() == CerTokType::PlusDAND)
	{
		CerTokClass::copy(tokc, tok);
		AST* result2 = nullptr;
		if (tok.getType() == CerTokType::PlusAND)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusAND);
			result2 = comparison_b();
		}
		else if (tok.getType() == CerTokType::PlusXOR)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusXOR);
			result2 = comparison_b();
		}
		else if (tok.getType() == CerTokType::PlusOR)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusOR);
			result2 = comparison_b();
		}
		else if (tok.getType() == CerTokType::PlusDAND)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusDAND);
			result2 = comparison_b();
		}

		result = new BinOp(result, tokc, result2);
	}
	return result;
}

AST* ScriptC::Obj::CerParser::singleKey()
{
	/*
	* singleKey : (InterNew | KEY_BREAK | KEY_CONTINUE)
	*/

	AST* result = nullptr;

	CerTokClass& tok = m_lexical->getCurrentToken();
	CerTokClass peek = m_lexical->peekNextToken();

	auto tok_type = tok.getType();
	auto peek_type = peek.getType();

	if (tok_type == CerTokType::Key_New) {
		result = interNew();
	}
	else if (tok_type == CerTokType::Key_Break) {
		CerTokClass tokc;
		CerTokClass::copy(tokc, tok);
		takeEat(CerTokType::Key_Break);
		result = new BreakOp(tokc);
		astlog(" - BreakOp() \n\n");
	}
	else if (tok_type == CerTokType::Key_Continue) {
		CerTokClass tokc;
		CerTokClass::copy(tokc, tok);
		takeEat(CerTokType::Key_Continue);
		result = new ContinueOp(tokc);
		astlog(" - ContinueOp() \n\n");
	}

	return result;
}

AST* ScriptC::Obj::CerParser::interNew()
{
	AST* result = nullptr;

	CerTokClass& tok = m_lexical->getCurrentToken();
	CerTokClass peek = m_lexical->peekNextToken();

	auto tok_type = tok.getType();
	auto peek_type = peek.getType();

	takeEat(CerTokType::Key_New);
	tok = m_lexical->getCurrentToken();

	CerTokClass tokc;
	CerTokClass::copy(tokc, tok);

	takeEat(CerTokType::Var_Id);

	/*
	* 2023.11.13
	* 添加 _init 初始化方法 new VarId 添加 括号用来启用特殊函数 _init
	*/

	tok = m_lexical->getCurrentToken();
	if (tok.getType() != CerTokType::LPARAM)
	{
		result = new InterNew(tokc);
		astlog(" - InterNew(" + tokc.getCstr() + ") \n\n");
		return result;
	}

	std::vector<AST*> params;

	takeEat(CerTokType::LPARAM);
	tok = m_lexical->getCurrentToken();

	if (tok.getType() != CerTokType::RPARAM) {
		AST* param = expr();
		params.push_back(param);

		tok = m_lexical->getCurrentToken();
		while (tok.getType() == CerTokType::COMMA) {
			takeEat(CerTokType::COMMA);
			param = expr();
			params.push_back(param);
			tok = m_lexical->getCurrentToken();
		}
	}

	takeEat(CerTokType::RPARAM);


	result = new InterNew(tokc, std::move(params));
	astlog(" - InterNew(" + tokc.getCstr() <<  " , params , size:" <<  params.size() << ") \n\n");
	return result;
}



AST* ScriptC::Obj::CerParser::comparison_b()
{
	/*
	* comparison_b : comparison_a ((PlusEQU|PlusNEQ) comparison_a)*
	*/
	AST* result = nullptr;
	result = comparison_a();

	CerTokClass& tok = m_lexical->getCurrentToken();
	CerTokClass tokc("", CerTokType::None);
	CerTokClass::copy(tokc, tok);

	while (tok.getType() == CerTokType::PlusEQU ||
		tok.getType() == CerTokType::PlusNEQ)
	{
		CerTokClass::copy(tokc, tok);
		AST* result2 = nullptr;
		if (tok.getType() == CerTokType::PlusEQU)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusEQU);
			result2 = comparison_a();
		}
		else if (tok.getType() == CerTokType::PlusNEQ)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusNEQ);
			result2 = comparison_a();
		}

		result = new BinOp(result, tokc, result2);
	}
	return result;

}

AST* ScriptC::Obj::CerParser::comparison_a()
{
	/*
	 * comparison_a : shiftoperator ((PlusGTR|PlusGEQ|PlusLSS|PlusLEQ) shiftoperator)*
	*/

	AST* result = nullptr;
	result = shiftoperator();

	CerTokClass& tok = m_lexical->getCurrentToken();
	CerTokClass tokc("", CerTokType::None);
	CerTokClass::copy(tokc, tok);

	while (tok.getType() == CerTokType::PlusGTR || 
		   tok.getType() == CerTokType::PlusGEQ || 
		   tok.getType() == CerTokType::PlusLSS || 
		   tok.getType() == CerTokType::PlusLEQ)
	{
		CerTokClass::copy(tokc, tok);
		AST* result2 = nullptr;
		if (tok.getType() == CerTokType::PlusGTR)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusGTR);
			result2 = shiftoperator();
		}
		else if (tok.getType() == CerTokType::PlusGEQ)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusGEQ);
			result2 = shiftoperator();
		}
		else if (tok.getType() == CerTokType::PlusLSS)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusLSS);
			result2 = shiftoperator();
		}
		else if (tok.getType() == CerTokType::PlusLEQ)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusLEQ);
			result2 = shiftoperator();
		}

		result = new BinOp(result, tokc, result2);
	}
	return result;
}

AST* ScriptC::Obj::CerParser::shiftoperator()
{
	/*
	 * shiftoperator: dualoperation((LeftO | RightO) dualoperation)*
	*/

	AST* result = nullptr;
	result = dualoperation();

	CerTokClass& tok = m_lexical->getCurrentToken();
	CerTokClass tokc("", CerTokType::None);
	CerTokClass::copy(tokc, tok);

	while (tok.getType() == CerTokType::PlusLeftO || tok.getType() == CerTokType::PlusRightO)
	{
		CerTokClass::copy(tokc, tok);
		AST* result2 = nullptr;
		if (tok.getType() == CerTokType::PlusLeftO)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusLeftO);
			result2 = dualoperation();
		}
		else if (tok.getType() == CerTokType::PlusRightO)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusRightO);
			result2 = dualoperation();
		}

		result = new BinOp(result, tokc, result2);
	}
	return result;
}

AST* ScriptC::Obj::CerParser::dualoperation()
{
	/*
	* dualoperation : term ((ADD|SUB|MOD) term)*
	*/
	AST* result = nullptr;
	result = term();

	CerTokClass& tok = m_lexical->getCurrentToken();
	CerTokClass tokc("", CerTokType::None);
	CerTokClass::copy(tokc, tok);

	while (tok.getType() == CerTokType::PlusAdd || tok.getType() == CerTokType::PlusSub || tok.getType() == CerTokType::PlusMod)
	{
		CerTokClass::copy(tokc, tok);
		AST* result2 = nullptr;
		if (tok.getType() == CerTokType::PlusAdd)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusAdd);
			result2 = term();
		}
		else if (tok.getType() == CerTokType::PlusSub)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusSub);
			result2 = term();
		}
		else if (tok.getType() == CerTokType::PlusMod)
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusMod);
			result2 = term();
		}

		result = new BinOp(result, tokc, result2);
	}
	return result;
}

AST* ScriptC::Obj::CerParser::term()
{
	/*
	* term : factor ((MUL|DIV) factor)*
	*/
	AST* result = nullptr;
	result = factor();

	CerTokClass& tok = m_lexical->getCurrentToken();
	CerTokClass tokc("",CerTokType::None);
	CerTokClass::copy(tokc, tok);

	while (tok.getType() == CerTokType::PlusMul || tok.getType() == CerTokType::PlusDiv)
	{
		CerTokClass::copy(tokc, tok);
		AST* result2 = nullptr;
		if (tok.getType() == CerTokType::PlusMul) 
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusMul);
			 result2 = factor();
		}
		else if (tok.getType() == CerTokType::PlusDiv) 
		{
			astlog(" - op(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::PlusDiv);
			result2 = factor();
		}
		result = new BinOp(result, tokc, result2);
	}

	return result;
}

AST* ScriptC::Obj::CerParser::factor()
{
	/*
	*	factor	:
	*			Integer_const
	*			| String_const
	*			| LPARAM logicaloper_b RPARAM
	*			| PlusAdd  factor
	*			| PlusSub  factor
	*			| PlusNot  factor
	*			| PlusMat  factor
	*			| VAR_ID
	*			| Function_Id
	*			| ArrayExpr
	*			| Key_True
	*			| Key_False
	*			| Key_Null
	*			| Key_Undef
	*			(indexExpr)?
	*/

	AST* result = nullptr;
	CerTokClass& tok = m_lexical->getCurrentToken();
	auto tok_type = tok.getType();
	auto peek_type = m_lexical->peekNextToken().getType();

	// expr
	if (tok_type == CerTokType::Int_Const || 
		tok_type == CerTokType::float_Const)
	{
		result = Integer_const();
	}
	else if (tok_type == CerTokType::Str_Const) {
		result = String_const();
	}
	else if (tok_type == CerTokType::LPARAM) {
		takeEat(CerTokType::LPARAM);
		result = logicaloper_b();
		takeEat(CerTokType::RPARAM);
	}
	else if (tok_type == CerTokType::PlusNot) {
		CerTokClass tokc;
		CerTokClass::copy(tokc, tok);
		takeEat(CerTokType::PlusNot);
		result = factor();
		result = new UnaryOp(tokc, result);
	}
	else if (tok_type == CerTokType::PlusMat) {
		CerTokClass tokc;
		CerTokClass::copy(tokc, tok);
		takeEat(CerTokType::PlusMat);
		result = factor();
		result = new UnaryOp(tokc, result);
	}
	else if (tok_type == CerTokType::PlusAdd) {
		CerTokClass tokc;
		CerTokClass::copy(tokc, tok);
		takeEat(CerTokType::PlusAdd);
		result = factor();
		result = new UnaryOp(tokc, result);
	}
	else if (tok_type == CerTokType::PlusSub) {
		CerTokClass tokc;
		CerTokClass::copy(tokc, tok);
		takeEat(CerTokType::PlusSub);
		result = factor();
		result = new UnaryOp(tokc, result);
	}
	else if (tok_type == CerTokType::Key_False) {
		result = new Num(tok);
		takeEat(CerTokType::Key_False);
		return result;
	}
	else if (tok_type == CerTokType::Key_True) {
		result = new Num(tok);
		takeEat(CerTokType::Key_True);
		return result;
	}
	else if (tok_type == CerTokType::Key_Null) {
		result = new Num(tok);
		takeEat(CerTokType::Key_Null);
		return result;
	}
	else if (tok_type == CerTokType::Key_Undef) {
		result = new Num(tok);
		takeEat(CerTokType::Key_Undef);
		return result;
	}
	else if (tok_type == CerTokType::LBRACKET) {
		result = ArrayExpression();
	}
	else if (tok_type == CerTokType::Var_Id) {
		if (peek_type == CerTokType::LPARAM) {
			result = Function_Id();
		}
		else {
			CerTokClass tokc;
			CerTokClass::copy(tokc, tok);
			astlog(" - Var(" + tok.getCstr() + " , " + tok.getTypeName() + ")\n\n");
			takeEat(CerTokType::Var_Id);
			result = new Var(tokc);
		}
	}
	else {
		auto errHin = ErrorHandling::getInstance();
		errHin->setErrInfo(tok.getDebugInfo());
		errHin->throwErr(EType::Parser,"unknow factor " + tok.getCstr());
	}

	tok = m_lexical->getCurrentToken();
	if (tok.getType() == CerTokType::LBRACKET || tok.getType() == CerTokType::DOT) {
		AST* index_ptr = indexExpr();
		return new exprOp(result, index_ptr);
	}
	else {
		return result;
	}
}

AST* ScriptC::Obj::CerParser::Assignment()
{
	/*
	*	Assignment : KEY_LET Var_Id PlusAssign expr 
	*			   | Var_Id (indexExpr)* PlusAssign expr 
	*/

	bool create = false;
	auto tok = m_lexical->getCurrentToken();
	create = tok.getType() == CerTokType::Key_Let;
	if (create == true) {
		takeEat(CerTokType::Key_Let);
	}
	tok = m_lexical->getCurrentToken();
	CerTokClass tokc;
	CerTokClass::copy(tokc, tok);

	takeEat(CerTokType::Var_Id);
	AST* index = nullptr;
	tok = m_lexical->getCurrentToken();
	if (create == false && (tok.getType() == CerTokType::LBRACKET || tok.getType() == CerTokType::DOT)) {
		index = indexExpr(true);
	}
	takeEat(CerTokType::PlusAssign);
	AST* right = expr();

	AssignOp* ret = new AssignOp(tokc, right,index, create);
	astlog(" - AssignOp(" << tokc.getTypeName()<<" , expr)\n\n");
	return ret;
}

AST* ScriptC::Obj::CerParser::Body()
{
	/*
		Body	: InterfaceDeclaration
				| ExpressionBody
				| FunDeclaration
				| FileInclude
				| (Body | Empty)

	*/
	std::vector<AST*> vec;
	do {
		AST* ast = nullptr;
		auto tok = m_lexical->getCurrentToken();
		if (tok.getType() == CerTokType::Key_Function) {
			ast = FunDeclaration();
			vec.push_back(ast);
			continue;
		}

		if (tok.getType() == CerTokType::Key_Interface) {
			ast = InterfaceDeclaration();
			vec.push_back(ast);
			continue;
		}

		if (tok.getType() == CerTokType::Key_InClude) {
			ast = FileInclude();
			vec.push_back(ast);
			continue;
		}

		auto exprVec = ExpressionBody();
		if (exprVec.at(0)->getNodeType() != AST::AstType::EmptyAst) {
			vec.insert(vec.end(), exprVec.begin(), exprVec.end());
			exprVec.clear();
			continue;
		}

		ast = Empty();
		vec.push_back(ast);
		break;
	} while (true);

	astlog(" - Body(" << vec.size() << ")\n\n");

	BodyAst* ret = new BodyAst(vec);
	return ret;
}

std::vector<AST*> ScriptC::Obj::CerParser::ExpressionBody()
{
	/*
	*ExpressionBody	: Assignment SEMI
	*				| expr	SEMI
	*				| ReturnExpr SEMI
	*				| ifconditionExpr
	*				| whileconditionExpr
	*				| forconditionExpr
	*				| (ExpressionBody | Empty)
	*/
	std::vector<AST*> vec;
	do {
		AST* ast = nullptr;
		auto tok = m_lexical->getCurrentToken();
		auto peek_tok = m_lexical->peekNextToken();

		do {
			tok = m_lexical->getCurrentToken();
			if (tok.getType() == CerTokType::SEMI) {
				takeEat(CerTokType::SEMI);
				continue;
			}
			break;
		} while (true);

		if (isAssignBegin() || tok.getType() == CerTokType::Key_Let)
		{
			// Assignment;
			ast = Assignment();
			vec.push_back(ast);
		}
		else if (isExprBegin()) {
			// expr
			ast = expr();
			vec.push_back(ast);
		}
		else if (tok.getType() == CerTokType::Key_Return) {
			// return expr
			ast = ReturnExpr();
			vec.push_back(ast);
		}
		else if (tok.getType() == CerTokType::Key_If) {
			// if ...
			ast = ifconditionExpr();
			vec.push_back(ast);

			do {
				tok = m_lexical->getCurrentToken();
				if (tok.getType() == CerTokType::SEMI) {
					takeEat(CerTokType::SEMI);
					continue;
				}
				break;
			} while (true);

			continue;
		}
		else if (tok.getType() == CerTokType::Key_While) {
			// while ...
			ast = whileconditionExpr();
			vec.push_back(ast);

			do {
				tok = m_lexical->getCurrentToken();
				if (tok.getType() == CerTokType::SEMI) {
					takeEat(CerTokType::SEMI);
					continue;
				}
				break;
			} while (true);

			continue;
		}
		else if (tok.getType() == CerTokType::Key_For) {
			// for ...
			ast = forconditionExpr();
			vec.push_back(ast);

			do {
				tok = m_lexical->getCurrentToken();
				if (tok.getType() == CerTokType::SEMI) {
					takeEat(CerTokType::SEMI);
					continue;
				}
				break;
			} while (true);

			continue;
		}
		else
		{
			ast = Empty();
			vec.push_back(ast);
			do {
				tok = m_lexical->getCurrentToken();
				if (tok.getType() == CerTokType::SEMI) {
					takeEat(CerTokType::SEMI);
					continue;
				}
				break;
			} while (true);
			break;
		}

		takeEat(CerTokType::SEMI);

		do {
			tok = m_lexical->getCurrentToken();
			if (tok.getType() == CerTokType::SEMI) {
				takeEat(CerTokType::SEMI);
				continue;
			}
			break;
		} while (true);
	} while (true);

	if (vec.size() > 1) {
		astlog(" - ExpressionBody (" << vec.size() << ")\n\n");
	}

	return vec;
}



AST* ScriptC::Obj::CerParser::FunDeclaration()
{
   /*
	* FunDeclaration: KEY_FUNCTION FunctionHeader COLON ExpressionBody KEY_END
	*				| KEY_FUNCTION KEY_EXPORT FunctionHeader KEY_END
	*/

	AST* ast = nullptr;
	auto tok = m_lexical->getCurrentToken();
	auto peek_tok = m_lexical->peekNextToken();

	AST* header;
	std::vector<AST*> codes;
	FuncDelOp* ret = nullptr;
	takeEat(CerTokType::Key_Function);
	tok = m_lexical->getCurrentToken();
	if (tok.getType() == CerTokType::Key_Export) {
		takeEat(CerTokType::Key_Export);
		header = FunctionHeader(true);
		takeEat(CerTokType::Key_End);
		ret = new FuncDelOp(header, codes);
		astlog(" - FunDeclaration export \n\n");
	}
	else {
		header = FunctionHeader();
		takeEat(CerTokType::COLON);
		codes = ExpressionBody();
		takeEat(CerTokType::Key_End);

		ret = new FuncDelOp(header, codes);
		astlog(" - FunDeclaration \n\n");
	}

	return ret;
}

AST* ScriptC::Obj::CerParser::InterfaceDeclaration()
{
	/*
	* InterfaceDeclaration	: KEY_INTERFACE InterfaceHeader LBRACE (FunDeclaration | Assignment SEMI)* RBRACE
	*/

	takeEat(CerTokType::Key_Interface);
	
	AST* header = InterfaceHeader();
	std::vector<AST*> datas;

	takeEat(CerTokType::LBRACE);

	auto tok = m_lexical->getCurrentToken();
	while (true) {
		tok = m_lexical->getCurrentToken();
		if (tok.getType() == CerTokType::Key_Function) {
			AST* func = FunDeclaration();
			
			FuncDelOp* funcD = dynamic_cast<FuncDelOp*>(func);
			FuncHeader* funcH = dynamic_cast<FuncHeader*>(funcD->getHeader());
			InterfaceHeaderOp* InterH = dynamic_cast<InterfaceHeaderOp*>(header);
			std::string name = InterH->getName().getCstr() + ":" + funcH->getFuncName();
			funcH->setFuncName(name);

			datas.push_back(func);
			continue;
		}
		else if (tok.getType() == CerTokType::Key_Let){
			AST* assign = Assignment();

			InterfaceHeaderOp* InterH = dynamic_cast<InterfaceHeaderOp*>(header);
			AssignOp* assignAst = dynamic_cast<AssignOp*>(assign);
			assignAst->setLeftName(InterH->getName().getCstr() + ":" + assignAst->getLeft().getCstr());


			takeEat(CerTokType::SEMI);
			datas.push_back(assign);
			continue;
		}
		break;
	}
	takeEat(CerTokType::RBRACE);

	AST* interdec = new InterfaceDecOp(header, std::move(datas));
	astlog(" - InterfaceDecOp \n\n");
	return interdec;
}

AST* ScriptC::Obj::CerParser::InterfaceHeader() {
	/*
	* InterfaceHeader	: VAR_ID (Key_INHERITS VAR_ID)?
	*/

	AST* ast = nullptr;
	auto tok = m_lexical->getCurrentToken();
	std::string name, parent;
	takeEat(CerTokType::Var_Id);
	name = tok.getCstr();
	
	tok = m_lexical->getCurrentToken();
	if (tok.getType() == CerTokType::Key_Inherits)
	{
		takeEat(CerTokType::Key_Inherits);
		auto tok = m_lexical->getCurrentToken();
		takeEat(CerTokType::Var_Id);
		parent = tok.getCstr();
	}
	AST* ret = new InterfaceHeaderOp(name, parent);
	astlog(" - InterfaceHeaderOp (" + name + " , " + parent  + ") \n\n");
	return ret;
}

AST* ScriptC::Obj::CerParser::FunctionHeader(bool has_export)
{
	/*
	* FuncHeader: VAR_ID LPARAM(VAR_ID(COMMA VAR_ID)*)* RPARAM
	*/

	AST* ast = nullptr;
	auto tok = m_lexical->getCurrentToken();
	auto peek_tok = m_lexical->peekNextToken();

	CerTokClass func_name;
	std::vector<CerTokClass> func_params;

	takeEat(CerTokType::Var_Id);
	CerTokClass::copy(func_name, tok);
	takeEat(CerTokType::LPARAM);
	tok = m_lexical->getCurrentToken();
	if (tok.getType() == CerTokType::Var_Id) {
		takeEat(CerTokType::Var_Id);
		CerTokClass tokc;
		CerTokClass::copy(tokc, tok);
		func_params.push_back(tokc);

		tok = m_lexical->getCurrentToken();
		while (tok.getType() == CerTokType::COMMA) {
			takeEat(CerTokType::COMMA);
			tok = m_lexical->getCurrentToken();
			takeEat(CerTokType::Var_Id);
			CerTokClass tokc;
			CerTokClass::copy(tokc, tok);
			func_params.push_back(tokc);
			tok = m_lexical->getCurrentToken();
		}
	}

	takeEat(CerTokType::RPARAM);

	astlog(" - FuncHeader (" << func_name.getCstr() << " " << func_params.size() << ")\n\n");
	std::reverse(std::begin(func_params), std::end(func_params));
	FuncHeader* funcH = new FuncHeader(func_name, func_params, has_export);
	return funcH;
}

AST* ScriptC::Obj::CerParser::Program()
{
	/*
	* Program : Body Eof
	*/

	astlog(" - Program()\n\n");
	AST* body = Body();
	ProgramAst* ret = new ProgramAst(body);
	takeEat(CerTokType::Eof);
	return ret;
}

AST* ScriptC::Obj::CerParser::Function_Id()
{
	/*
	*  Function_Id : VAR_ID LPARAM (expr (COMMA expr)*)* RPARAM (PlusLSS VAR_ID PlusGTR)
	*/
	AST* ast = nullptr;
	auto tok = m_lexical->getCurrentToken();
	auto peek_tok = m_lexical->peekNextToken();
	std::string this_name;

	CerTokClass func_name;
	std::vector<AST*> func_params;

	takeEat(CerTokType::Var_Id);
	CerTokClass::copy(func_name, tok);
	takeEat(CerTokType::LPARAM);
	tok = m_lexical->getCurrentToken();
	if (tok.getType() != CerTokType::RPARAM) {
		AST* param = expr();
		func_params.push_back(param);

		tok = m_lexical->getCurrentToken();
		while (tok.getType() == CerTokType::COMMA) {
			takeEat(CerTokType::COMMA);
			param = expr();
			func_params.push_back(param);
			tok = m_lexical->getCurrentToken();
		}
	}

	takeEat(CerTokType::RPARAM);

	tok = m_lexical->getCurrentToken();
	if (tok.getType() == CerTokType::PlusLSS) {
		takeEat(CerTokType::PlusLSS);
		tok = m_lexical->getCurrentToken();
		this_name = tok.getCstr();
		takeEat(CerTokType::Var_Id);
		takeEat(CerTokType::PlusGTR);
	}

	astlog(" - FuncCall (" << func_name.getCstr() << " " << func_params.size() << ")\n\n");

	FuncCall* funcC = new FuncCall(func_name, func_params);
	funcC->setthisName(this_name);
	return funcC;
}

AST* ScriptC::Obj::CerParser::ArrayExpression()
{
	/*
	* ArrayExpression: LBRACKET(expr(COMMA expr)*)* RBRACKET
	*/

	takeEat(CerTokType::LBRACKET);

	auto tok = m_lexical->getCurrentToken();
	auto peek_tok = m_lexical->peekNextToken();
	std::vector<AST*> vec;

	for(bool dot = true ; dot ;)
	{
		tok = m_lexical->getCurrentToken();
		peek_tok = m_lexical->peekNextToken();

		dot = false;
		if (isExprBegin()) {
			AST* param = expr();
			vec.push_back(param);

			tok = m_lexical->getCurrentToken();
			peek_tok = m_lexical->peekNextToken();

			if (tok.getType() == CerTokType::COMMA) {
				takeEat(CerTokType::COMMA);
				dot = true;
			}
		}
	}

	takeEat(CerTokType::RBRACKET);

	AST* result = new ArrayVar(std::move(vec));
	return result;
}

AST* ScriptC::Obj::CerParser::InterfaceExpression() 
{
	/*
	 *	InterExpr: DOT(VAR_ID | Function_Id);
	 */
	AST* result = nullptr;

	takeEat(CerTokType::DOT);
	auto tok = m_lexical->getCurrentToken();
	auto peek_tok = m_lexical->peekNextToken();
	if (tok.getType() == CerTokType::Var_Id) {
		if (peek_tok.getType() == CerTokType::LPARAM) {
			result = Function_Id();
		}
		else {
			takeEat(CerTokType::Var_Id);
			result = new Var(tok);
		}
	}

	return new InterExprOp(result);
}

bool ScriptC::Obj::CerParser::isExprBegin()
{
	auto tok = m_lexical->getCurrentToken();
	auto peek_tok = m_lexical->peekNextToken();

	if (tok.getType() == CerTokType::Int_Const ||
		tok.getType() == CerTokType::float_Const ||
		tok.getType() == CerTokType::Str_Const ||
		tok.getType() == CerTokType::Key_True ||
		tok.getType() == CerTokType::Key_False ||
		tok.getType() == CerTokType::Key_Null ||
		tok.getType() == CerTokType::Key_Undef ||
		tok.getType() == CerTokType::Key_New ||
		tok.getType() == CerTokType::Key_Break||
		tok.getType() == CerTokType::Key_Continue)
	{
		// expr	1 ... | 2.2....
		return true;
	}
	else if (tok.getType() == CerTokType::LPARAM)
	{
		// expr (...)
		return true;
	}
	else if (tok.getType() == CerTokType::PlusSub || tok.getType() == CerTokType::PlusAdd)
	{
		// expr - .... | + ...
		return true;
	}
	else if (tok.getType() == CerTokType::Var_Id && 
		peek_tok.getType() != CerTokType::PlusAssign)
	{
		// expr var + .... | var - ....
		return true;
	}
	else if (tok.getType() == CerTokType::Var_Id &&
		peek_tok.getType() == CerTokType::LPARAM)
	{
		// expr function
		return true;
	}
	else if (tok.getType() == CerTokType::LBRACKET)
	{
		// expr [...]
		return true;
	}

	return false;
}

bool ScriptC::Obj::CerParser::isAssignBegin()
{
	bool ret = false;
	auto tok = m_lexical->getCurrentToken();
	int count = 0;
	auto peek_tok = m_lexical->peekNextToken(count);
	if (tok.getType() == CerTokType::Var_Id) {
		do {
			peek_tok = m_lexical->peekNextToken(count);
			if (peek_tok.getType() == CerTokType::SEMI)
				break;
			if (peek_tok.getType() == CerTokType::Eof)
				break;

			if (peek_tok.getType() == CerTokType::PlusAssign) {
				ret = true;
				break;
			}
		} while (++count);
	}

	return ret;
}

AST* ScriptC::Obj::CerParser::Integer_const()
{
	/*
	* Integer_const	: int 
	*				| float
	*/
	auto tok = m_lexical->getCurrentToken();
	AST* result = nullptr;

	switch (tok.getType()) 
	{
	case CerTokType::Int_Const:
		result = new Num(tok);
		takeEat(CerTokType::Int_Const);
		break;
	case CerTokType::float_Const:
		result = new Num(tok);
		takeEat(CerTokType::float_Const);
		break;
	}
	astlog(" - Num(" + tok.getCstr() + " , " + tok.getTypeName() +")\n\n");
	return result;
}

AST* ScriptC::Obj::CerParser::String_const()
{
	/*
	* String 
	*/

	auto tok = m_lexical->getCurrentToken();
	CerTokClass ctok;
	CerTokClass::copy(ctok, tok);
	takeEat(CerTokType::Str_Const);
	astlog(" - Str(" + tok.getCstr() + ")\n\n");
	return new Str(ctok);
}

AST* ScriptC::Obj::CerParser::Empty()
{
	/*
	* Empty : null
	*/
	
	EmptyAst* pass = new EmptyAst();
	return pass;
}

AST* ScriptC::Obj::CerParser::parser()
{
	astlog("AST Print >>>\n");

	AST* ret = Program();
	return ret;
}

AST* ScriptC::Obj::CerParser::ReturnExpr()
{
	AST* return_expr = nullptr;
	takeEat(CerTokType::Key_Return);
	auto tok = m_lexical->getCurrentToken();
	if (tok.getType() != CerTokType::SEMI)
		return_expr = expr();
	else
		return_expr = Empty();
	
	ReturnAst* ret = new ReturnAst(return_expr);

	if(return_expr->getNodeType() == AST::AstType::EmptyAst)
		astlog(" - ReturnAst \n\n");
	else
		astlog(" - ReturnAst expr \n\n");
	return ret;
}

AST* ScriptC::Obj::CerParser::FileInclude()
{
	/*
	*	FileInclude: KEY_INCLUDE LPARAM String_const RPARAM SEMI
	*/

	/*
	* 2023.10.11 
	* 修改 将 require "str"; 改为 require (str);
	*/

	IncludeFile* include_file = nullptr;
	takeEat(CerTokType::Key_InClude);
	takeEat(CerTokType::LPARAM);
	auto tok = this->m_lexical->getCurrentToken();
	include_file = new IncludeFile(tok);
	takeEat(CerTokType::Str_Const);
	takeEat(CerTokType::RPARAM);
	takeEat(CerTokType::SEMI);
	return include_file;
}

void ScriptC::Obj::CerParser::takeEat(CerTokType type)
{
	auto errHis = ErrorHandling::getInstance();
	auto tok = this->m_lexical->getCurrentToken();
	errHis->setErrInfo(tok.getDebugInfo());
	if (tok.getType() == type) {
		m_lexical->getNextToken();
		return;
	}

	CerTokClass tokc("", type);
	errHis->throwErr(EType::Parser, "token need " + tokc.getTypeName() + " but is " + tok.getTypeName());
}

