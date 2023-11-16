#include "..\CerLexical\CerLexical.h"
#include "..\ErrorHandling\ErrorHandling.h"
#include "AST.h"
#include "CerParser.h"
#include "CerParserMacro.h"

/*****************
* class AST
* 基类
******************/
ScriptC::Obj::AST::AST() :
	m_ast_type(AstType::None)
{
	auto errHis = ErrorHandling::getInstance();
	m_info = errHis->getErrorInfo();
}

ScriptC::Obj::AST::~AST()
{
}


AST::AstType ScriptC::Obj::AST::getNodeType()
{
	return m_ast_type;
}

CerTokClass::DebugInfo ScriptC::Obj::AST::getDebugInfo()
{
	return m_info;
}

/***********************
* class Num : public AST
* 常量节点
************************/

ScriptC::Obj::Num::Num(CerTokClass tok):
	m_tok(tok)
{
	m_ast_type = AstNodeType::Num;
}

ScriptC::Obj::Num::~Num()
{
	astlog("\n~Num");
}

CerTokClass ScriptC::Obj::Num::getTok()
{
	return m_tok;
}
/*****************
* BreakOp :public AST
* 跳出关键字
*****************/

ScriptC::Obj::BreakOp::BreakOp(CerTokClass tok):
	m_tok(tok)
{
	m_ast_type = AstNodeType::BreakOp;
}

ScriptC::Obj::BreakOp::~BreakOp()
{
	astlog("\n~BreakOp");
}

CerTokClass ScriptC::Obj::BreakOp::getTok()
{
	return m_tok;
}


/*****************
* ContinueOp :public AST
* 继续关键字
*****************/

ScriptC::Obj::ContinueOp::ContinueOp(CerTokClass tok) :
	m_tok(tok)
{
	m_ast_type = AstNodeType::ContinueOp;
}

ScriptC::Obj::ContinueOp::~ContinueOp()
{
	astlog("\n~ContinueOp");
}

CerTokClass ScriptC::Obj::ContinueOp::getTok()
{
	return m_tok;
}

/*****************
* Var :public AST
* 变量节点
*****************/

ScriptC::Obj::Var::Var(CerTokClass tok)
{
	CerTokClass::copy(m_tok, tok);
	m_ast_type = AST::AstType::Var;
}

ScriptC::Obj::Var::~Var()
{
	astlog("\n~Var");
}

CerTokClass ScriptC::Obj::Var::getTok()
{
	return m_tok;
}

/*****************
* Str :public AST
* 字符串节点
*****************/

ScriptC::Obj::Str::Str(CerTokClass tok)
{
	CerTokClass::copy(m_tok, tok);
	m_ast_type = AST::AstType::Str;
}

ScriptC::Obj::Str::~Str()
{
	astlog("\n~Str");
}

CerTokClass ScriptC::Obj::Str::getTok()
{
	return m_tok;
}

/*****************
* IncludeFile :public AST
* 字符串节点
*****************/

ScriptC::Obj::IncludeFile::IncludeFile(CerTokClass file)
{
	CerTokClass::copy(m_file, file);
	m_ast_type = AST::AstType::IncludeFile;
}

ScriptC::Obj::IncludeFile::~IncludeFile()
{
	astlog("\n~IncludeFile");
}

CerTokClass ScriptC::Obj::IncludeFile::getFile()
{
	return m_file;
}

/*****************
* InterNew :public AST
* New节点
*****************/

ScriptC::Obj::InterNew::InterNew(CerTokClass name)
{
	CerTokClass::copy(m_inter_name, name);
	m_ast_type = AST::AstType::InterNew;
	m_call_init = false;
}

ScriptC::Obj::InterNew::InterNew(CerTokClass name, std::vector<AST*> params)
{
	CerTokClass::copy(m_inter_name, name);
	m_ast_type = AST::AstType::InterNew;
	m_params = std::move(params);
	m_call_init = true;
}

ScriptC::Obj::InterNew::~InterNew()
{
	astlog("\n~InterNew");
	if (m_call_init)
	{
		for (auto& i : m_params) 
		{
			delete i;
			i = nullptr;
		}
	}
}

CerTokClass ScriptC::Obj::InterNew::getName()
{
	return m_inter_name;
}

bool ScriptC::Obj::InterNew::getCallInit()
{
	return m_call_init;
}

std::vector<AST*> ScriptC::Obj::InterNew::getParams()
{
	return m_params;
}

void ScriptC::Obj::InterNew::setInitInter(std::string inter)
{
	m_init_inter = inter;
}

std::string ScriptC::Obj::InterNew::getInitInter()
{
	return m_init_inter;
}

/*****************
* ArrayVar :public AST
* 数组节点
*****************/

ScriptC::Obj::ArrayVar::ArrayVar(std::vector<AST*> expr)
{
	m_expr = expr;
	m_only_index = false;
	m_ast_type = AST::AstType::ArrayVar;
}

ScriptC::Obj::ArrayVar::~ArrayVar()
{
	for (auto& i : m_expr) {
		delete i;
		i = nullptr;
	}

	astlog("\n~ArrayVar");
}

std::vector<AST*> ScriptC::Obj::ArrayVar::getExpr()
{
	return m_expr;
}

void ScriptC::Obj::ArrayVar::setOnlyIndex(bool isindex)
{
	m_only_index = isindex;
}

bool ScriptC::Obj::ArrayVar::gethasOnlyIndex()
{
	return m_only_index;
}

void ScriptC::Obj::ArrayVar::setLeftIndex(bool isindex)
{
	m_left_index = isindex;
}

bool ScriptC::Obj::ArrayVar::gethasLeftIndex()
{
	return m_left_index;
}

/*****************
* InterExprOp :public AST
* 接口下标节点
*****************/

ScriptC::Obj::InterExprOp::InterExprOp(AST* person)
{
	m_person = person;
	m_ast_type = AST::AstType::InterExprOp;
	m_left_index = false;
	m_left_last = false;
}

ScriptC::Obj::InterExprOp::~InterExprOp()
{
	if (m_person) {
		delete m_person;
		m_person = nullptr;
	}

	astlog("\n~InterExprOp");
}

void ScriptC::Obj::InterExprOp::setLeftIndex(bool isindex)
{
	m_left_index = isindex;
}

void ScriptC::Obj::InterExprOp::setLeftLast(bool islast)
{
	m_left_last = islast;
}

bool ScriptC::Obj::InterExprOp::gethasLeftIndex()
{
	return m_left_index;
}

bool ScriptC::Obj::InterExprOp::gethasLeftLast()
{
	return m_left_last;
}

AST* ScriptC::Obj::InterExprOp::getPerson()
{
	return m_person;
}


/*****************
* indexExprOp :public AST
* 辅助表达式下标节点
*****************/

ScriptC::Obj::indexExprOp::indexExprOp(std::vector<AST*> indes):
	m_inte_size(0),
	m_func_size(0)
{
	m_ast_type = AST::AstType::indexExprOp;
	m_indexs = indes;
}

ScriptC::Obj::indexExprOp::~indexExprOp()
{
	for (auto& i : m_indexs) {
		delete i;
		i = nullptr;
	}
	astlog("\n~indexExprOp");
}

size_t ScriptC::Obj::indexExprOp::getFuncSize()
{
	return m_func_size;
}

size_t ScriptC::Obj::indexExprOp::getInterSize()
{
	return m_inte_size;
}

void ScriptC::Obj::indexExprOp::setFuncSize(size_t count)
{
	m_func_size = count;
}

void ScriptC::Obj::indexExprOp::setInterSize(size_t count)
{
	m_inte_size = count;
}

std::vector<AST*> ScriptC::Obj::indexExprOp::getIndexs()
{
	return m_indexs;
}


/*****************
* IfExpr :public AST
* 条件判断表达式
*****************/


ScriptC::Obj::IfExpr::IfExpr(std::pair<AST*, std::vector<AST*>> _if,
	std::vector<std::pair<AST*, std::vector<AST*>>> _elifs,
	std::pair<AST*, std::vector<AST*>> _else):
	m_if({ nullptr ,{} }),
	m_else({ nullptr ,{} })
{
	m_ast_type = AST::AstType::IfExpr;

	m_if = _if;
	m_elifs = _elifs;
	m_else = _else;
}

ScriptC::Obj::IfExpr::~IfExpr()
{
	if (m_if.first) {
		delete m_if.first;
		for (auto& i : m_if.second) {
			delete i;
		}
	}

	for (auto& i : m_elifs) {
		delete i.first;
		for (auto& j : i.second) {
			delete j;
		}
	}

	if (m_else.first) {
		for (auto& i : m_else.second) {
			delete i;
		}
	}

	astlog("\n~IfExpr");
}

std::pair<AST*, std::vector<AST*>> ScriptC::Obj::IfExpr::getIf()
{
	return m_if;
}

std::vector<std::pair<AST*, std::vector<AST*>>> ScriptC::Obj::IfExpr::getElifs()
{
	return m_elifs;
}

std::pair<AST*, std::vector<AST*>> ScriptC::Obj::IfExpr::getElse()
{
	return m_else;
}

/*****************
* WhileExpr :public AST
* 条件循环表达式
*****************/

ScriptC::Obj::WhileExpr::WhileExpr(AST* _expr, std::vector<AST*> _codebody):
	m_expr(_expr),
	m_codes(_codebody)
{
	m_ast_type = AST::AstType::WhileExpr;
}

ScriptC::Obj::WhileExpr::~WhileExpr()
{
	if (m_expr) {
		delete m_expr;
	}

	for (auto& i : m_codes) {
		delete i;
	}

	astlog("\n~WhileExpr");
}

AST* ScriptC::Obj::WhileExpr::getExpr()
{
	return m_expr;
}

std::vector<AST*> ScriptC::Obj::WhileExpr::getCodes()
{
	return m_codes;
}


/*****************
* ForExpr :public AST
* 数组循环表达式
*****************/

ScriptC::Obj::ForExpr::ForExpr(CerTokClass _var, AST* _expr, std::vector<AST*> _codebody)
{
	CerTokClass::copy(m_var,_var);
	m_expr = _expr;
	m_codes = _codebody;

	m_ast_type = AST::AstType::ForExpr;
}

ScriptC::Obj::ForExpr::~ForExpr()
{
	if (m_expr) {
		delete m_expr;
	}

	for (auto& i : m_codes) {
		delete i;
	}

	astlog("\n~WhileExpr");
}

CerTokClass ScriptC::Obj::ForExpr::getVar()
{
	return m_var;
}

AST* ScriptC::Obj::ForExpr::getExpr()
{
	return m_expr;
}

std::vector<AST*> ScriptC::Obj::ForExpr::getCodes()
{
	return m_codes;
}


/*************************
* class BinOp : public AST
* 二元操作符节点
*************************/

ScriptC::Obj::BinOp::BinOp(AST* left, CerTokClass op, AST* right) :
	m_left(left),
	m_right(right),
	m_op(op)
{
	m_ast_type = AstNodeType::BinOp;
}

ScriptC::Obj::BinOp::~BinOp()
{
	astlog("\n~BinOp");
	if (m_left) {
		delete m_left;
		m_left = nullptr;
	}

	if (m_right) {
		delete m_right;
		m_right = nullptr;
	}
}

AST* ScriptC::Obj::BinOp::getLeft()
{
	return m_left;
}

CerTokClass ScriptC::Obj::BinOp::getTok()
{
	return m_op;
}

AST* ScriptC::Obj::BinOp::getRight()
{
	return m_right;
}

/********************
* UnaryOp : public AST
* 一元操作节点
*********************/

ScriptC::Obj::UnaryOp::UnaryOp(CerTokClass op, AST* right):
	m_right(right),
	m_op(op)
{
	m_ast_type = AstNodeType::UnaryOp;
}

ScriptC::Obj::UnaryOp::~UnaryOp()
{
	astlog("\n~UnaryOp");
	delete m_right;
	m_right = nullptr;
}

CerTokClass ScriptC::Obj::UnaryOp::getOpType()
{
	return m_op;
}

AST* ScriptC::Obj::UnaryOp::getRight()
{
	return m_right;
}

/********************
* exprOp : public AST
* 表达式节点
*********************/

ScriptC::Obj::exprOp::exprOp(AST* expr, AST* index):
	m_expr(expr),
	m_index(index),
	m_has_index(false),
	m_has_left(false)
{

	if (m_index) {
		m_has_index = true;
	}

	m_ast_type = AstNodeType::exprOp;
}

ScriptC::Obj::exprOp::~exprOp()
{
	if (m_expr) {
		delete m_expr;
		m_expr = nullptr;
	}

	if (m_index) {
		delete m_index;
		m_index = nullptr;
	}

	astlog("\n~exprOp");
}

AST* ScriptC::Obj::exprOp::getExpr()
{
	return m_expr;
}

AST* ScriptC::Obj::exprOp::getIndex()
{
	return m_index;
}

bool ScriptC::Obj::exprOp::getHasIndex()
{
	return m_has_index;
}

bool ScriptC::Obj::exprOp::getHasLeft()
{
	return m_has_left;
}



/********************
* ProgramAst : public AST
* 程序开始节点
*********************/

ScriptC::Obj::ProgramAst::ProgramAst(AST* body)
{
	m_body = body;
	m_ast_type = AST::AstType::ProgramAst;
}

ScriptC::Obj::ProgramAst::~ProgramAst()
{
	astlog("\n~ProgramAst");
	delete m_body;
	m_body = nullptr;
}

AST* ScriptC::Obj::ProgramAst::getBody()
{
	return m_body;
}


/********************
* BodyAst : public AST
* 程序集合Body
*********************/

ScriptC::Obj::BodyAst::BodyAst(std::vector<AST*> &block)
{
	m_ast_type = AST::AstType::BodyAst;
	m_block = std::move(block);
}

ScriptC::Obj::BodyAst::~BodyAst()
{
	astlog("\n~BodyAst");
	for (auto& i : m_block) {
		delete i;
		i = nullptr;
	}
}

std::vector<AST*> ScriptC::Obj::BodyAst::getBlock()
{
	return m_block;
}


/********************
* AssignOp : public AST
* 赋值操作数
*********************/

ScriptC::Obj::AssignOp::AssignOp(CerTokClass left, AST* right,AST* m_idnex,bool create)
{
	m_create = create;
	m_left_var = left;
	m_left_index = m_idnex;
	m_right_expr = right;
	m_ast_type = AST::AstType::AssignOp;
}

ScriptC::Obj::AssignOp::~AssignOp()
{
	astlog("\n~AssignOp");
	delete m_right_expr;
	m_right_expr = nullptr;
	if (m_left_index) {
		delete m_left_index;
	}
}

CerTokClass ScriptC::Obj::AssignOp::getLeft()
{
	return m_left_var;
}

AST* ScriptC::Obj::AssignOp::getRight()
{
	return m_right_expr;
}

AST* ScriptC::Obj::AssignOp::getLeftIndex()
{
	return m_left_index;
}

bool ScriptC::Obj::AssignOp::getCreate()
{
	return m_create;
}

void ScriptC::Obj::AssignOp::setLeftName(std::string name)
{
	m_left_var.reCStr(name);
}



/********************
* FuncHeader : public AST
* 函数声明节点
*********************/
ScriptC::Obj::FuncHeader::FuncHeader(CerTokClass tok, std::vector<CerTokClass> params, bool hasexport)
{
	m_hasClass = false;
	m_params = params;
	m_func_tok = tok;
	m_func_name = tok.getCstr();
	m_hasExport = hasexport;
	m_ast_type = AST::AstType::FuncHeader;
}

ScriptC::Obj::FuncHeader::~FuncHeader()
{
	astlog("\n~FuncHeader");
}

CerTokClass ScriptC::Obj::FuncHeader::getTok()
{
	return m_func_tok;
}

std::vector<CerTokClass> ScriptC::Obj::FuncHeader::getParams()
{
	return m_params;
}

std::string ScriptC::Obj::FuncHeader::getFuncName()
{
	return m_func_name;
}

void ScriptC::Obj::FuncHeader::setHasClass(bool has)
{
	m_hasClass = has;
}

bool ScriptC::Obj::FuncHeader::gethasClass()
{
	return m_hasClass;
}

void ScriptC::Obj::FuncHeader::setFuncName(std::string name)
{
	m_func_name = name;
}

bool ScriptC::Obj::FuncHeader::gethasExport()
{
	return m_hasExport;
}

/********************
* FuncCall : public AST
* 函数声明节点
*********************/

ScriptC::Obj::FuncCall::FuncCall(CerTokClass tok, std::vector<AST*> params)
{
	m_params = params;
	m_func_tok = tok;
	m_func_name = tok.getCstr();

	m_ast_type = AST::AstType::FuncCall;
}

ScriptC::Obj::FuncCall::~FuncCall()
{
	astlog("\n~FuncCall");
	for (auto& i : m_params) {
		delete i;
		i = nullptr;
	}
}

CerTokClass ScriptC::Obj::FuncCall::getTok()
{
	return m_func_tok;
}

std::vector<AST*> ScriptC::Obj::FuncCall::getParams()
{
	return m_params;
}

std::string ScriptC::Obj::FuncCall::getFuncName()
{
	return m_func_name;
}

void ScriptC::Obj::FuncCall::setthisName(std::string name)
{
	m_this_name = name;
}

std::string ScriptC::Obj::FuncCall::getthisName()
{
	return m_this_name;
}



/********************
* FuncDelOp : public AST
* 函数实现节点
*********************/

ScriptC::Obj::FuncDelOp::FuncDelOp(AST* header, std::vector<AST*> codes)
{
	m_func_header = header;
	m_func_codes = codes;
	m_ast_type = AST::AstType::FuncDelOp;
}

ScriptC::Obj::FuncDelOp::~FuncDelOp()
{
	delete m_func_header;
	m_func_header = nullptr;
	for (auto& i : m_func_codes) {
		delete i;
		i = nullptr;
	}
	astlog("\n~FuncDelOp");
}

AST* ScriptC::Obj::FuncDelOp::getHeader()
{
	return m_func_header;
}

std::vector<AST*> ScriptC::Obj::FuncDelOp::getCode()
{
	return m_func_codes;
}

/********************
* ReturnAst : public AST
* 程序开始节点
*********************/


ScriptC::Obj::ReturnAst::ReturnAst(AST* expr)
{
	m_expr = expr;
	m_ast_type = AST::AstType::ReturnAst;
}

ScriptC::Obj::ReturnAst::~ReturnAst()
{
	delete m_expr;
	astlog("\n~ReturnAst");
}

AST* ScriptC::Obj::ReturnAst::getExpr()
{
	return m_expr;
}


/********************
* Empty : public AST
* 空Pass
*********************/

ScriptC::Obj::EmptyAst::EmptyAst()
{
	m_ast_type = AST::AstType::EmptyAst;
}

ScriptC::Obj::EmptyAst::~EmptyAst()
{
}

/********************
* InterfaceDecOp : public AST
* 接口声明节点
*********************/

ScriptC::Obj::InterfaceDecOp::InterfaceDecOp(AST* header, std::vector<AST*> datas)
{
	m_interface_header = header;
	m_interface_data = datas;

	m_ast_type = AST::AstType::InterfaceDecOp;
}

ScriptC::Obj::InterfaceDecOp::~InterfaceDecOp()
{

	delete m_interface_header;
	for (auto& i : m_interface_data) {
		delete i;
		i = nullptr;
	}

	astlog("\n~InterfaceDecOp");
}

AST* ScriptC::Obj::InterfaceDecOp::getHeader()
{
	return m_interface_header;
}

std::vector<AST*> ScriptC::Obj::InterfaceDecOp::getDatas()
{
	return m_interface_data;
}

/********************
* InterfaceHeaderOp : public AST
* 接口声明节点
*********************/

ScriptC::Obj::InterfaceHeaderOp::InterfaceHeaderOp(CerTokClass name, CerTokClass parent)
{
	m_name = name;
	m_parent = parent;

	m_ast_type = AST::AstType::InterfaceHeaderOp;
}

ScriptC::Obj::InterfaceHeaderOp::~InterfaceHeaderOp()
{
	astlog("\n~InterfaceHeaderOp");
}

CerTokClass ScriptC::Obj::InterfaceHeaderOp::getName()
{
	return m_name;
}

CerTokClass ScriptC::Obj::InterfaceHeaderOp::getParent()
{
	return m_parent;
}


