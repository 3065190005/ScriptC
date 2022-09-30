#include "..\CerLexical\CerLexical.h"
#include "..\ErrorHandling\ErrorHandling.h"
#include "AST.h"
#include "CerParser.h"
#include "CerParserMacro.h"

/*****************
* class AST
* 基类
******************/
Cervice::Obj::AST::AST() :
	m_ast_type(AstType::None)
{
	auto errHis = ErrorHandling::getInstance();
	m_info = errHis->getErrorInfo();
}

Cervice::Obj::AST::~AST()
{
}


AST::AstType Cervice::Obj::AST::getNodeType()
{
	return m_ast_type;
}

CerTokClass::DebugInfo Cervice::Obj::AST::getDebugInfo()
{
	return m_info;
}

/***********************
* class Num : public AST
* 常量节点
************************/

Cervice::Obj::Num::Num(CerTokClass tok):
	m_tok(tok)
{
	m_ast_type = AstNodeType::Num;
}

Cervice::Obj::Num::~Num()
{
	astlog("\n~Num");
}

CerTokClass Cervice::Obj::Num::getTok()
{
	return m_tok;
}
/*****************
* BreakOp :public AST
* 跳出关键字
*****************/

Cervice::Obj::BreakOp::BreakOp(CerTokClass tok):
	m_tok(tok)
{
	m_ast_type = AstNodeType::BreakOp;
}

Cervice::Obj::BreakOp::~BreakOp()
{
	astlog("\n~BreakOp");
}

CerTokClass Cervice::Obj::BreakOp::getTok()
{
	return m_tok;
}


/*****************
* ContinueOp :public AST
* 继续关键字
*****************/

Cervice::Obj::ContinueOp::ContinueOp(CerTokClass tok) :
	m_tok(tok)
{
	m_ast_type = AstNodeType::ContinueOp;
}

Cervice::Obj::ContinueOp::~ContinueOp()
{
	astlog("\n~ContinueOp");
}

CerTokClass Cervice::Obj::ContinueOp::getTok()
{
	return m_tok;
}

/*****************
* Var :public AST
* 变量节点
*****************/

Cervice::Obj::Var::Var(CerTokClass tok)
{
	CerTokClass::copy(m_tok, tok);
	m_ast_type = AST::AstType::Var;
}

Cervice::Obj::Var::~Var()
{
	astlog("\n~Var");
}

CerTokClass Cervice::Obj::Var::getTok()
{
	return m_tok;
}

/*****************
* Str :public AST
* 字符串节点
*****************/

Cervice::Obj::Str::Str(CerTokClass tok)
{
	CerTokClass::copy(m_tok, tok);
	m_ast_type = AST::AstType::Str;
}

Cervice::Obj::Str::~Str()
{
	astlog("\n~Str");
}

CerTokClass Cervice::Obj::Str::getTok()
{
	return m_tok;
}

/*****************
* IncludeFile :public AST
* 字符串节点
*****************/

Cervice::Obj::IncludeFile::IncludeFile(CerTokClass file)
{
	CerTokClass::copy(m_file, file);
	m_ast_type = AST::AstType::IncludeFile;
}

Cervice::Obj::IncludeFile::~IncludeFile()
{
	astlog("\n~IncludeFile");
}

CerTokClass Cervice::Obj::IncludeFile::getFile()
{
	return m_file;
}

/*****************
* InterNew :public AST
* New节点
*****************/

Cervice::Obj::InterNew::InterNew(CerTokClass name)
{
	CerTokClass::copy(m_inter_name, name);
	m_ast_type = AST::AstType::InterNew;
}

Cervice::Obj::InterNew::~InterNew()
{
	astlog("\n~InterNew");
}

CerTokClass Cervice::Obj::InterNew::getName()
{
	return m_inter_name;
}

/*****************
* ArrayVar :public AST
* 数组节点
*****************/

Cervice::Obj::ArrayVar::ArrayVar(std::vector<AST*> expr)
{
	m_expr = expr;
	m_only_index = false;
	m_ast_type = AST::AstType::ArrayVar;
}

Cervice::Obj::ArrayVar::~ArrayVar()
{
	for (auto& i : m_expr) {
		delete i;
		i = nullptr;
	}

	astlog("\n~ArrayVar");
}

std::vector<AST*> Cervice::Obj::ArrayVar::getExpr()
{
	return m_expr;
}

void Cervice::Obj::ArrayVar::setOnlyIndex(bool isindex)
{
	m_only_index = isindex;
}

bool Cervice::Obj::ArrayVar::gethasOnlyIndex()
{
	return m_only_index;
}

void Cervice::Obj::ArrayVar::setLeftIndex(bool isindex)
{
	m_left_index = isindex;
}

bool Cervice::Obj::ArrayVar::gethasLeftIndex()
{
	return m_left_index;
}

/*****************
* InterExprOp :public AST
* 接口下标节点
*****************/

Cervice::Obj::InterExprOp::InterExprOp(AST* person)
{
	m_person = person;
	m_ast_type = AST::AstType::InterExprOp;
}

Cervice::Obj::InterExprOp::~InterExprOp()
{
	if (m_person) {
		delete m_person;
		m_person = nullptr;
	}

	astlog("\n~InterExprOp");
}

void Cervice::Obj::InterExprOp::setLeftIndex(bool isindex)
{
	m_left_index = isindex;
}

bool Cervice::Obj::InterExprOp::gethasLeftIndex()
{
	return m_left_index;
}

AST* Cervice::Obj::InterExprOp::getPerson()
{
	return m_person;
}


/*****************
* indexExprOp :public AST
* 辅助表达式下标节点
*****************/

Cervice::Obj::indexExprOp::indexExprOp(std::vector<AST*> indes):
	m_inte_size(0),
	m_func_size(0)
{
	m_ast_type = AST::AstType::indexExprOp;
	m_indexs = indes;
}

Cervice::Obj::indexExprOp::~indexExprOp()
{
	for (auto& i : m_indexs) {
		delete i;
		i = nullptr;
	}
	astlog("\n~indexExprOp");
}

size_t Cervice::Obj::indexExprOp::getFuncSize()
{
	return m_func_size;
}

size_t Cervice::Obj::indexExprOp::getInterSize()
{
	return m_inte_size;
}

void Cervice::Obj::indexExprOp::setFuncSize(size_t count)
{
	m_func_size = count;
}

void Cervice::Obj::indexExprOp::setInterSize(size_t count)
{
	m_inte_size = count;
}

std::vector<AST*> Cervice::Obj::indexExprOp::getIndexs()
{
	return m_indexs;
}


/*****************
* IfExpr :public AST
* 条件判断表达式
*****************/


Cervice::Obj::IfExpr::IfExpr(std::pair<AST*, std::vector<AST*>> _if,
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

Cervice::Obj::IfExpr::~IfExpr()
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

std::pair<AST*, std::vector<AST*>> Cervice::Obj::IfExpr::getIf()
{
	return m_if;
}

std::vector<std::pair<AST*, std::vector<AST*>>> Cervice::Obj::IfExpr::getElifs()
{
	return m_elifs;
}

std::pair<AST*, std::vector<AST*>> Cervice::Obj::IfExpr::getElse()
{
	return m_else;
}

/*****************
* WhileExpr :public AST
* 条件循环表达式
*****************/

Cervice::Obj::WhileExpr::WhileExpr(AST* _expr, std::vector<AST*> _codebody):
	m_expr(_expr),
	m_codes(_codebody)
{
	m_ast_type = AST::AstType::WhileExpr;
}

Cervice::Obj::WhileExpr::~WhileExpr()
{
	if (m_expr) {
		delete m_expr;
	}

	for (auto& i : m_codes) {
		delete i;
	}

	astlog("\n~WhileExpr");
}

AST* Cervice::Obj::WhileExpr::getExpr()
{
	return m_expr;
}

std::vector<AST*> Cervice::Obj::WhileExpr::getCodes()
{
	return m_codes;
}


/*****************
* ForExpr :public AST
* 数组循环表达式
*****************/

Cervice::Obj::ForExpr::ForExpr(CerTokClass _var, AST* _expr, std::vector<AST*> _codebody)
{
	CerTokClass::copy(m_var,_var);
	m_expr = _expr;
	m_codes = _codebody;

	m_ast_type = AST::AstType::ForExpr;
}

Cervice::Obj::ForExpr::~ForExpr()
{
	if (m_expr) {
		delete m_expr;
	}

	for (auto& i : m_codes) {
		delete i;
	}

	astlog("\n~WhileExpr");
}

CerTokClass Cervice::Obj::ForExpr::getVar()
{
	return m_var;
}

AST* Cervice::Obj::ForExpr::getExpr()
{
	return m_expr;
}

std::vector<AST*> Cervice::Obj::ForExpr::getCodes()
{
	return m_codes;
}


/*************************
* class BinOp : public AST
* 二元操作符节点
*************************/

Cervice::Obj::BinOp::BinOp(AST* left, CerTokClass op, AST* right) :
	m_left(left),
	m_right(right),
	m_op(op)
{
	m_ast_type = AstNodeType::BinOp;
}

Cervice::Obj::BinOp::~BinOp()
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

AST* Cervice::Obj::BinOp::getLeft()
{
	return m_left;
}

CerTokClass Cervice::Obj::BinOp::getTok()
{
	return m_op;
}

AST* Cervice::Obj::BinOp::getRight()
{
	return m_right;
}

/********************
* UnaryOp : public AST
* 一元操作节点
*********************/

Cervice::Obj::UnaryOp::UnaryOp(CerTokClass op, AST* right):
	m_right(right),
	m_op(op)
{
	m_ast_type = AstNodeType::UnaryOp;
}

Cervice::Obj::UnaryOp::~UnaryOp()
{
	astlog("\n~UnaryOp");
	delete m_right;
	m_right = nullptr;
}

CerTokClass Cervice::Obj::UnaryOp::getOpType()
{
	return m_op;
}

AST* Cervice::Obj::UnaryOp::getRight()
{
	return m_right;
}

/********************
* exprOp : public AST
* 表达式节点
*********************/

Cervice::Obj::exprOp::exprOp(AST* expr, AST* index):
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

Cervice::Obj::exprOp::~exprOp()
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

AST* Cervice::Obj::exprOp::getExpr()
{
	return m_expr;
}

AST* Cervice::Obj::exprOp::getIndex()
{
	return m_index;
}

bool Cervice::Obj::exprOp::getHasIndex()
{
	return m_has_index;
}

bool Cervice::Obj::exprOp::getHasLeft()
{
	return m_has_left;
}



/********************
* ProgramAst : public AST
* 程序开始节点
*********************/

Cervice::Obj::ProgramAst::ProgramAst(AST* body)
{
	m_body = body;
	m_ast_type = AST::AstType::ProgramAst;
}

Cervice::Obj::ProgramAst::~ProgramAst()
{
	astlog("\n~ProgramAst");
	delete m_body;
	m_body = nullptr;
}

AST* Cervice::Obj::ProgramAst::getBody()
{
	return m_body;
}


/********************
* BodyAst : public AST
* 程序集合Body
*********************/

Cervice::Obj::BodyAst::BodyAst(std::vector<AST*> &block)
{
	m_ast_type = AST::AstType::BodyAst;
	m_block = std::move(block);
}

Cervice::Obj::BodyAst::~BodyAst()
{
	astlog("\n~BodyAst");
	for (auto& i : m_block) {
		delete i;
		i = nullptr;
	}
}

std::vector<AST*> Cervice::Obj::BodyAst::getBlock()
{
	return m_block;
}


/********************
* AssignOp : public AST
* 赋值操作数
*********************/

Cervice::Obj::AssignOp::AssignOp(CerTokClass left, AST* right,AST* m_idnex,bool create)
{
	m_create = create;
	m_left_var = left;
	m_left_index = m_idnex;
	m_right_expr = right;
	m_ast_type = AST::AstType::AssignOp;
}

Cervice::Obj::AssignOp::~AssignOp()
{
	astlog("\n~AssignOp");
	delete m_right_expr;
	m_right_expr = nullptr;
	if (m_left_index) {
		delete m_left_index;
	}
}

CerTokClass Cervice::Obj::AssignOp::getLeft()
{
	return m_left_var;
}

AST* Cervice::Obj::AssignOp::getRight()
{
	return m_right_expr;
}

AST* Cervice::Obj::AssignOp::getLeftIndex()
{
	return m_left_index;
}

bool Cervice::Obj::AssignOp::getCreate()
{
	return m_create;
}

void Cervice::Obj::AssignOp::setLeftName(std::string name)
{
	m_left_var.reCStr(name);
}



/********************
* FuncHeader : public AST
* 函数声明节点
*********************/
Cervice::Obj::FuncHeader::FuncHeader(CerTokClass tok, std::vector<CerTokClass> params, bool hasexport)
{
	m_hasClass = false;
	m_params = params;
	m_func_tok = tok;
	m_func_name = tok.getCstr();
	m_hasExport = hasexport;
	m_ast_type = AST::AstType::FuncHeader;
}

Cervice::Obj::FuncHeader::~FuncHeader()
{
	astlog("\n~FuncHeader");
}

CerTokClass Cervice::Obj::FuncHeader::getTok()
{
	return m_func_tok;
}

std::vector<CerTokClass> Cervice::Obj::FuncHeader::getParams()
{
	return m_params;
}

std::string Cervice::Obj::FuncHeader::getFuncName()
{
	return m_func_name;
}

void Cervice::Obj::FuncHeader::setHasClass(bool has)
{
	m_hasClass = has;
}

bool Cervice::Obj::FuncHeader::gethasClass()
{
	return m_hasClass;
}

void Cervice::Obj::FuncHeader::setFuncName(std::string name)
{
	m_func_name = name;
}

bool Cervice::Obj::FuncHeader::gethasExport()
{
	return m_hasExport;
}

/********************
* FuncCall : public AST
* 函数声明节点
*********************/

Cervice::Obj::FuncCall::FuncCall(CerTokClass tok, std::vector<AST*> params)
{
	m_params = params;
	m_func_tok = tok;
	m_func_name = tok.getCstr();

	m_ast_type = AST::AstType::FuncCall;
}

Cervice::Obj::FuncCall::~FuncCall()
{
	astlog("\n~FuncCall");
	for (auto& i : m_params) {
		delete i;
		i = nullptr;
	}
}

CerTokClass Cervice::Obj::FuncCall::getTok()
{
	return m_func_tok;
}

std::vector<AST*> Cervice::Obj::FuncCall::getParams()
{
	return m_params;
}

std::string Cervice::Obj::FuncCall::getFuncName()
{
	return m_func_name;
}

void Cervice::Obj::FuncCall::setthisName(std::string name)
{
	m_this_name = name;
}

std::string Cervice::Obj::FuncCall::getthisName()
{
	return m_this_name;
}



/********************
* FuncDelOp : public AST
* 函数实现节点
*********************/

Cervice::Obj::FuncDelOp::FuncDelOp(AST* header, std::vector<AST*> codes)
{
	m_func_header = header;
	m_func_codes = codes;
	m_ast_type = AST::AstType::FuncDelOp;
}

Cervice::Obj::FuncDelOp::~FuncDelOp()
{
	delete m_func_header;
	m_func_header = nullptr;
	for (auto& i : m_func_codes) {
		delete i;
		i = nullptr;
	}
	astlog("\n~FuncDelOp");
}

AST* Cervice::Obj::FuncDelOp::getHeader()
{
	return m_func_header;
}

std::vector<AST*> Cervice::Obj::FuncDelOp::getCode()
{
	return m_func_codes;
}

/********************
* ReturnAst : public AST
* 程序开始节点
*********************/


Cervice::Obj::ReturnAst::ReturnAst(AST* expr)
{
	m_expr = expr;
	m_ast_type = AST::AstType::ReturnAst;
}

Cervice::Obj::ReturnAst::~ReturnAst()
{
	delete m_expr;
	astlog("\n~ReturnAst");
}

AST* Cervice::Obj::ReturnAst::getExpr()
{
	return m_expr;
}


/********************
* Empty : public AST
* 空Pass
*********************/

Cervice::Obj::EmptyAst::EmptyAst()
{
	m_ast_type = AST::AstType::EmptyAst;
}

Cervice::Obj::EmptyAst::~EmptyAst()
{
}

/********************
* InterfaceDecOp : public AST
* 接口声明节点
*********************/

Cervice::Obj::InterfaceDecOp::InterfaceDecOp(AST* header, std::vector<AST*> datas)
{
	m_interface_header = header;
	m_interface_data = datas;

	m_ast_type = AST::AstType::InterfaceDecOp;
}

Cervice::Obj::InterfaceDecOp::~InterfaceDecOp()
{

	delete m_interface_header;
	for (auto& i : m_interface_data) {
		delete i;
		i = nullptr;
	}

	astlog("\n~InterfaceDecOp");
}

AST* Cervice::Obj::InterfaceDecOp::getHeader()
{
	return m_interface_header;
}

std::vector<AST*> Cervice::Obj::InterfaceDecOp::getDatas()
{
	return m_interface_data;
}

/********************
* InterfaceHeaderOp : public AST
* 接口声明节点
*********************/

Cervice::Obj::InterfaceHeaderOp::InterfaceHeaderOp(CerTokClass name, CerTokClass parent)
{
	m_name = name;
	m_parent = parent;

	m_ast_type = AST::AstType::InterfaceHeaderOp;
}

Cervice::Obj::InterfaceHeaderOp::~InterfaceHeaderOp()
{
	astlog("\n~InterfaceHeaderOp");
}

CerTokClass Cervice::Obj::InterfaceHeaderOp::getName()
{
	return m_name;
}

CerTokClass Cervice::Obj::InterfaceHeaderOp::getParent()
{
	return m_parent;
}


