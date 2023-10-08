#include "..\AutoElement\LetManager.h"
#include "..\AutoElement\LetObject.h"
#include "..\CerLexical\CerLexical.h"
#include "..\CerParser\AST.h"
#include "..\CerParser\CerParser.h"
#include "..\DllReader\DllFuncReader.h"
#include "..\ErrorHandling\ErrorHandling.h"
#include "CerInterpreter.h"
#include "CerInterpreterMacro.h"
#include <sstream>

ScriptC::Obj::CerInterpreter::CerInterpreter() :
	m_rootAts(nullptr),
	m_vm_code({})
{
	if (m_table_temp["_FILE_NAME_"] != 1) {
		m_table_temp["_FILE_NAME_"] = 1;
	}
}

ScriptC::Obj::CerInterpreter::~CerInterpreter()
{
}

CerInterpreter* ScriptC::Obj::CerInterpreter::create(AST* tree)
{
	CerInterpreter* ret = new CerInterpreter();
	ret->m_rootAts = tree;
	
	if (ret == nullptr || tree == nullptr) {
		delete ret;
		ret = nullptr;
	}

	return ret;
}

std::vector<CommandCode>& ScriptC::Obj::CerInterpreter::CompileCode(autoPtr ret, AST* rootAts,bool isMerge)
{
	interlog("InterPreterCompileCode: >>>\n");

	AST* root = rootAts;
	if (root == nullptr) {
		root = m_rootAts;
	}

	visit(root, ret, this);

	if (isMerge) {
		fileAddressSet();
	}

	printCode();
	return m_vm_code;
}

void ScriptC::Obj::CerInterpreter::fileAddressSet()
{
	numberT size = m_vm_code.size();
	for (auto& i : m_include_file_sequence) {
		auto address = m_table_temp.find(i);
		size -= m_table_temp[i];
		m_table_temp[i] = size;
	}
}

bool ScriptC::Obj::CerInterpreter::visit_indexExprOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::indexExprOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter,"getNode need indexExprOp");
	}

	indexExprOp* index = dynamic_cast<indexExprOp*>(node);
	auto vec = index->getIndexs();
	for (auto& i : vec) {
		visit(i, ret, this);
	}

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_exprOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::exprOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need exprOp");
	}

	exprOp* expr = dynamic_cast<exprOp*>(node);
	
	visit(expr->getExpr(), ret, this);

	if (expr->getHasIndex())
		visit(expr->getIndex(), ret, this);

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_BinOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::BinOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need BinOp");
	}

	BinOp* binop = dynamic_cast<BinOp*>(node);

	CerTokType type;

	visit(binop->getLeft(),ret, this);

	visit(binop->getRight(), ret, this);

	type = binop->getTok().getType();
	
	CommandCode code(CodeType::Push);
	switch (type)
	{
	case CerTokType::PlusAdd:
		code = CommandCode(CodeType::Add);
		break;
	case CerTokType::PlusSub:
		code = CommandCode(CodeType::Sub);
		break;
	case CerTokType::PlusMul:
		code = CommandCode(CodeType::Mul);
		break;
	case CerTokType::PlusDiv:
		code = CommandCode(CodeType::Div);
		break;
	case CerTokType::PlusMod:
		code = CommandCode(CodeType::Mod);
		break;
	case CerTokType::PlusLeftO:
		code = CommandCode(CodeType::LeftO);
		break;
	case CerTokType::PlusRightO:
		code = CommandCode(CodeType::RightO);
		break;
	case CerTokType::PlusGTR:
		code = CommandCode(CodeType::Gtr);
		break;
	case CerTokType::PlusGEQ:
		code = CommandCode(CodeType::Geq);
		break;
	case CerTokType::PlusLSS:
		code = CommandCode(CodeType::Lss);
		break;
	case CerTokType::PlusLEQ:
		code = CommandCode(CodeType::Leq);
		break;
	case CerTokType::PlusEQU:
		code = CommandCode(CodeType::Equ);
		break;
	case CerTokType::PlusNEQ:
		code = CommandCode(CodeType::Neq);
		break;
	case CerTokType::PlusAND:
		code = CommandCode(CodeType::And);
		break;
	case CerTokType::PlusXOR:
		code = CommandCode(CodeType::Xor);
		break;
	case CerTokType::PlusOR:
		code = CommandCode(CodeType::Or);
		break;
	case CerTokType::PlusDAND:
		code = CommandCode(CodeType::DAnd);
		break;
	case CerTokType::PlusDOR:
		code = CommandCode(CodeType::DOr);
		break;
	default:
		code = CommandCode(CodeType::Pass);
		break;
	}

	m_errHis->setErrInfo(binop->getDebugInfo());
	PushCode(std::move(code));
	
	if (code.getCodeType() == CodeType::Pass) {
		return false;
	}

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_IfExpr(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::IfExpr) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need IfExpr");
	}

	numberT allJump = 0;
	numberT oneJump = 0;
	numberT twoJump = 0;

	IfExpr* ifexpr = dynamic_cast<IfExpr*>(node);

	std::vector<std::pair<numberT, numberT>> replaces;

	auto _if = ifexpr->getIf();
	auto _elifs = ifexpr->getElifs();
	auto _else = ifexpr->getElse();

	auto_c param1(false, false);
	CodeParams params;

	visit(_if.first, ret, this);

	params.insert({ "param1",std::move(param1) });
	params.insert({ "param2",std::move(param1) });
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

	oneJump = m_vm_code.size();

	param1 << "~if";
	params.insert({ "param1",std::move(param1) });
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::PushS, std::move(params))));

	for (auto& codeBody : _if.second) {
		visit(codeBody, ret, this);
	}
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::PopS, params)));

	params.insert({ "param1",std::move(param1) });
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

	twoJump = m_vm_code.size();

	replaces.push_back({ oneJump,twoJump });
	replaces.push_back({ twoJump ,-1 });
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::Pass, params)));


	for (auto& elifIf : _elifs) {

		visit(elifIf.first, ret, this);

		params.insert({ "param1",std::move(param1) });
		params.insert({ "param2",std::move(param1) });
		m_errHis->setErrInfo(node->getDebugInfo());
		PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

		oneJump = m_vm_code.size();

		param1 << "~elif";
		params.insert({ "param1",std::move(param1) });
		PushCode(std::move(CommandCode(CodeType::PushS, std::move(params))));

		for (auto& codeBody : elifIf.second) {
			visit(codeBody, ret, this);
		}
		m_errHis->setErrInfo(node->getDebugInfo());
		PushCode(std::move(CommandCode(CodeType::PopS, params)));

		params.insert({ "param1",std::move(param1) });
		PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

		twoJump = m_vm_code.size();

		replaces.push_back({ oneJump,twoJump });
		replaces.push_back({ twoJump ,-1 });

		PushCode(std::move(CommandCode(CodeType::Pass, params)));
	}

	if (_else.first) {
		visit(_else.first, ret, this);

		params.insert({ "param1",std::move(param1) });
		params.insert({ "param2",std::move(param1) });
		m_errHis->setErrInfo(node->getDebugInfo());
		PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

		oneJump = m_vm_code.size();

		param1 << "~else";
		params.insert({ "param1",std::move(param1) });
		PushCode(std::move(CommandCode(CodeType::PushS, std::move(params))));

		for (auto& codeBody : _else.second) {
			visit(codeBody, ret, this);
		}
		m_errHis->setErrInfo(node->getDebugInfo());
		PushCode(std::move(CommandCode(CodeType::PopS, params)));

		params.insert({ "param1",std::move(param1) });
		PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

		twoJump = m_vm_code.size();

		replaces.push_back({ oneJump,twoJump });
		replaces.push_back({ twoJump ,-1 });

		PushCode(std::move(CommandCode(CodeType::Pass, params)));
	}

	allJump = m_vm_code.size();

	for (auto& i : replaces) {
		auto ptr = (m_vm_code.at(i.first - 1).getCodeParams());
		numberT jumper = -1;
		if (i.second != -1) {
			jumper = i.second - i.first + 1;
			ptr->find("param1")->second << jumper;
		}
		else {
			jumper = allJump - i.first;
			ptr->find("param1")->second << jumper;
		}
	}

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_WhileExpr(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::WhileExpr) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need WhileExpr");
	}

	WhileExpr* _whileExpr = dynamic_cast<WhileExpr*>(node);
	AST* _expr = _whileExpr->getExpr();
	auto _codes = _whileExpr->getCodes();
	
	numberT beg_end = 0;
	numberT end_eip = 0;
	numberT beg_eip = 0;

	auto_c param1(false, false);
	CodeParams params;

	// push ~while
	param1 << "~while";
	params.insert({ "param1",std::move(param1) });
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::PushS, std::move(params))));

	// Push beg(0):end(1)
	param1 << 0;
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	beg_end = m_vm_code.size() - 1;

	// pass (beg
	PushCode(std::move(CommandCode(CodeType::Pass, std::move(params))));
	beg_eip = m_vm_code.size() - 1;

	// expr
	visit(_expr, ret, this);

	// jmp param2 param3(beg)
	param1 << 1;
	params.insert({ "param3",std::move(param1) });
	params.insert({ "param2",std::move(param1) });
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

	// PushS param1: ~loop
	param1 << "~loop";
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::PushS, std::move(params))));

	// ...codes
	for (auto& i : _codes) {
		visit(i, ret, this);
	}

	// popS (~loop
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::PopS, std::move(params))));

	// jmp param3(end)
	param1 << 0;
	params.insert({ "param3",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

	// pass (end
	PushCode(std::move(CommandCode(CodeType::Pass, std::move(params))));
	end_eip = m_vm_code.size() - 1;

	// popS (~while
	PushCode(std::move(CommandCode(CodeType::PopS, std::move(params))));

	std::string str;
	std::stringstream ss;
	ss << beg_eip;
	ss << ":";
	ss << end_eip;
	ss >> str;
	m_vm_code.at(beg_end).getCodeParams()->find("param1")->second << str;
	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_ForExpr(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ForExpr) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need ForExpr");
	}

	ForExpr* _forexpr = dynamic_cast<ForExpr*>(node);
	AST* _expr = _forexpr->getExpr();
	auto _codes = _forexpr->getCodes();
	std::string var_name = _forexpr->getVar().getCstr();

	numberT beg_end = 0;
	numberT end_eip = 0;
	numberT beg_eip = 0;

	auto_c param1(false, false);
	CodeParams params;

	// push ~while
	param1 << "~while";
	params.insert({ "param1",std::move(param1) });
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::PushS, std::move(params))));

	// push 0
	param1 << 0;
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	// pop ~index
	param1 << "~index";
	params.insert({ "param1",std::move(param1) });
	params.insert({ "param2",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Pop, std::move(params))));

	// push expr
	visit(_expr, ret, this);

	// pop 	~arr
	param1 << "~arr";
	params.insert({ "param1",std::move(param1) });
	params.insert({ "param2",std::move(param1) });
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::Pop, std::move(params))));

	// lens ~arr
	param1 << "~arr";
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Lens, std::move(params))));

	// pop ~arrInd
	param1 << "~arrInd";
	params.insert({ "param1",std::move(param1) });
	params.insert({ "param2",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Pop, std::move(params))));

	// Push beg(0):end(1)
	param1 << 0;
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	beg_end = m_vm_code.size() - 1;

	// pass (beg
	PushCode(std::move(CommandCode(CodeType::Pass, std::move(params))));
	beg_eip = m_vm_code.size() - 1;

	// push ~arrInd[~index]
	param1 << "~arrInd";
	params.insert({ "param1",std::move(param1) });
	param1 << "value";
	params.insert({ "param2",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	param1 << "~index";
	params.insert({ "param1",std::move(param1) });
	param1 << "value";
	params.insert({ "param2",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	params.insert({ "param1",std::move(param1) });
	param1 << "array";
	params.insert({ "param2",std::move(param1) });
	param1 << ",0:(i)";
	params.insert({ "param3",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));
	
	// push KEY_null;
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	// neq
	PushCode(std::move(CommandCode(CodeType::Neq, std::move(params))));

	// jmp param2 param3(beg)
	param1 << 1;
	params.insert({ "param3",std::move(param1) });
	params.insert({ "param2",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

	// PushS param1: ~loop
	param1 << "~loop";
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::PushS, std::move(params))));

	// push ~arr[~arrInd[~index]];
	param1 << "~arr";
	params.insert({ "param1",std::move(param1) });
	param1 << "value";
	params.insert({ "param2",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	param1 << "~arrInd";
	params.insert({ "param1",std::move(param1) });
	param1 << "value";
	params.insert({ "param2",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	param1 << "~index";
	params.insert({ "param1",std::move(param1) });
	param1 << "value";
	params.insert({ "param2",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	params.insert({ "param1",std::move(param1) });
	param1 << "array";
	params.insert({ "param2",std::move(param1) });
	param1 << ",0:(i)";
	params.insert({ "param3",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	params.insert({ "param1",std::move(param1) });
	param1 << "array";
	params.insert({ "param2",std::move(param1) });
	param1 << ",0:(i)";
	params.insert({ "param3",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	// pop forvar
	param1 << var_name;
	params.insert({ "param1",std::move(param1) });
	params.insert({ "param2",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Pop, std::move(params))));
	m_table_temp[var_name] = 1;

	//push ~index;
	param1 << "~index";
	params.insert({ "param1",std::move(param1) });
	param1 << "value";
	params.insert({ "param2",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	//push 1;
	param1 << 1;
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

	//Add
	PushCode(std::move(CommandCode(CodeType::Add, std::move(params))));

	//Pop ~index;
	param1 << "~index";
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Pop, std::move(params))));

	// ...codes
	for (auto& i : _codes) {
		visit(i, ret, this);
	}

	// popS (~loop
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::PopS, std::move(params))));

	// jmp param3(end)
	param1 << 0;
	params.insert({ "param3",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

	// pass (end
	PushCode(std::move(CommandCode(CodeType::Pass, std::move(params))));
	end_eip = m_vm_code.size() - 1;

	// popS (~while
	PushCode(std::move(CommandCode(CodeType::PopS, std::move(params))));

	std::string str;
	std::stringstream ss;
	ss << beg_eip;
	ss << ":";
	ss << end_eip;
	ss >> str;
	m_vm_code.at(beg_end).getCodeParams()->find("param1")->second << str;
	return true;


	/*
	PushS param1:~while
        
        push 0
        pop ~index;

        push expr;
        pop ~arr;

        lens ~arr;
        pop ~arrInd

        Push beg(0):end(1)
        pass(beg

        push ~arrInd[~index]
        push KEY_null;
        neq;

        jmp param2 param3(beg)
            PushS param1:~loop

            push ~arr[~arrInd[~index]];
            pop i

            ...codes

            PopS (~loop
            jmp param3(end)

        pass(end
        PopS (~while
	*/
}

bool ScriptC::Obj::CerInterpreter::visit_UnaryOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::UnaryOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need UnaryOp");
	}

	UnaryOp* unaryOp = dynamic_cast<UnaryOp*>(node);

	auto_c rightEle(false,false);
	visit(unaryOp->getRight(), ret, this);

	m_errHis->setErrInfo(node->getDebugInfo());
	if (unaryOp->getOpType().getType() == CerTokType::PlusAdd) {
		CommandCode opear2(CodeType::UnaryAdd);
		PushCode(std::move(opear2));
	}

	else if (unaryOp->getOpType().getType() == CerTokType::PlusSub) {
		CommandCode opear2(CodeType::UnarySub);
		PushCode(std::move(opear2));
	}

	else if (unaryOp->getOpType().getType() == CerTokType::PlusNot) {
		CommandCode opear2(CodeType::Not);
		PushCode(std::move(opear2));
	}

	else if (unaryOp->getOpType().getType() == CerTokType::PlusMat) {
		CommandCode opear2(CodeType::Mat);
		PushCode(std::move(opear2));
	}
	
	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_Num(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::Num) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need Num");
	}

	Num* number = dynamic_cast<Num*>(node);

	std::stringstream ss;
	numberT num = 0;
	numberT double_num = 0;
	CerTokClass tok = number->getTok();

	m_errHis->setErrInfo(node->getDebugInfo());
	if (tok.getType() == CerTokType::Key_False) {
		auto_c param(false, false);
		param << false;
		CodeParams param2;
		param2.insert({ "param1",std::move(param) });
		CommandCode opear1(CodeType::Push, param2);
		PushCode(std::move(opear1));
	}
	else if (tok.getType() == CerTokType::Key_True) {
		auto_c param(false, false);
		param << true;
		CodeParams param2;
		param2.insert({ "param1",std::move(param) });
		CommandCode opear1(CodeType::Push, param2);
		PushCode(std::move(opear1));
	}
	else if (tok.getType() == CerTokType::Key_Null) {
		auto_c param(false, false);
		CodeParams param2;
		param2.insert({ "param1",std::move(param) });
		CommandCode opear1(CodeType::Push, param2);
		PushCode(std::move(opear1));
	}
	else if (tok.getType() == CerTokType::Key_Undef) {
		auto_c param(true, true);
		CodeParams param2;
		param2.insert({ "param1",std::move(param) });
		CommandCode opear1(CodeType::Push, param2);
		PushCode(std::move(opear1));
	}
	else if (tok.getType() == CerTokType::Int_Const) {
		ss << tok.getCstr();
		ss >> num;
		auto_c param(false,false);
		param << num;
		CodeParams param2;
		param2.insert({ "param1",std::move(param) });
		CommandCode opear1(CodeType::Push, param2);
		PushCode(std::move(opear1));

	}
	else if (tok.getType() == CerTokType::float_Const) {
		ss << tok.getCstr();
		ss >> double_num;
		auto_c param;
		param << double_num;
		CodeParams param2;
		param2.insert({ "param1",std::move(param) });
		CommandCode opear1(CodeType::Push, param2);
		PushCode(std::move(opear1));
	}
	else {
		m_errHis->setErrInfo(tok.getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "class Num : unknow Element Type for this class");
	}
	ss.str("");
	ss.clear();

	if (!ret || ret->getType() == LetObject::ObjT::null) {
		return false;
	}

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_Str(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::Str) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need Num");
	}

	Str* str = dynamic_cast<Str*>(node);
	auto_c param(false, false);
	param << str->getTok().getCstr();
	CodeParams param2;
	param2.insert({ "param1",std::move(param) });
	CommandCode opear1(CodeType::Push, param2);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear1));
	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_Var(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::Var) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need Var");
	}

	Var* var_ast = dynamic_cast<Var*>(node);
	std::string var_name;
	var_name = var_ast->getTok().getCstr();
	CerTokClass tok = var_ast->getTok();

	m_errHis->setErrInfo(node->getDebugInfo());

	if (tok.getType() == CerTokType::Var_Id) {
		if (m_table_temp.find(var_name) != m_table_temp.end()) {
			auto_c space, param;
			space << "value";
			param << var_name;

			CodeParams param2;
			param2.insert({ "param1",std::move(param) });
			param2.insert({ "param2" ,std::move(space) });
			CommandCode opear1(CodeType::Push, param2);
			PushCode(std::move(opear1));
		}

		if (!ret || ret->getType() == LetObject::ObjT::null) {
			return false;
		}
	}
	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_BreakOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::BreakOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need BreakOp");
	}

	BreakOp* var_ast = dynamic_cast<BreakOp*>(node);

	auto_c param1(false, false);
	CodeParams params;

	m_errHis->setErrInfo(node->getDebugInfo());

	// PopS param1
	param1 << "~while";
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::PopS, std::move(params))));

	param1 << 1;
	params.insert({ "param3",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_ContinueOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ContinueOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need ContinueOp");
	}

	ContinueOp* var_ast = dynamic_cast<ContinueOp*>(node);

	auto_c param1(false, false);
	CodeParams params;

	m_errHis->setErrInfo(node->getDebugInfo());

	// PopS param1
	param1 << "~while";
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::PopS, std::move(params))));

	param1 << 0;
	params.insert({ "param3",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_InterNew(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::InterNew) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need InterNew");
	}

	InterNew* var_ast = dynamic_cast<InterNew*>(node);
	std::string var_name;
	var_name = var_ast->getName().getCstr();
	CerTokClass tok = var_ast->getName();

	m_errHis->setErrInfo(node->getDebugInfo());
	if (tok.getType() == CerTokType::Var_Id) {
		if (m_table_temp.find(var_name) != m_table_temp.end()) {
			auto_c space, param;
			space << "interface";
			param << var_name;

			CodeParams param2;
			param2.insert({ "param1",std::move(param) });
			param2.insert({ "param2" ,std::move(space) });
			CommandCode opear1(CodeType::Push, param2);
			PushCode(std::move(opear1));
		}
	}

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_IncludeFile(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::IncludeFile) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need IncludeFile");
	}

	extern std::string G_Res_path;
	IncludeFile* include_ast = dynamic_cast<IncludeFile*>(node);

	std::string file = include_ast->getFile().getCstr();
	std::string prefix = "%::";
	if (m_table_temp.find(prefix + file) != m_table_temp.end()) {
		return true;
	}

	std::string customPath = file + ".sc";
	std::string filePath = G_Res_path + file + ".sc";
	std::ifstream isExist(filePath);
	if (!isExist) {
		isExist.open(customPath);
	}
	if (!isExist) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter,"include file is not Exist");
	}


	std::string fileTxt, path;
	while (std::getline(isExist, path))
		fileTxt.append(std::move(path) + '\n');
	isExist.close();

	auto_c param1(false, false);
	CodeParams params;
	param1 << file;
	params.insert({ "param1",std::move(param1) });
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::Inc, std::move(params))));

	// file codeing
	m_table_temp[prefix + file] = 1;
	m_include_file_sequence.push_back(prefix + file);
	auto lexical = CerLexical<char>::create(fileTxt, file);
	auto parser = CerParser::create(lexical);
	AST* astTree = parser->parser();
	delete lexical;
	delete parser;

	auto interpr = CerInterpreter::create(astTree);
	
	interpr->m_table_temp = m_table_temp;
	std::vector<CommandCode> codeVec;
	codeVec = std::move(interpr->CompileCode(ret));
	m_table_temp[prefix + file] = interpr->m_self_size + 1;
	m_table_temp.insert(interpr->m_table_temp.begin(), interpr->m_table_temp.end());
	
	m_include_file.insert({ file,std::move(codeVec) });

	auto offset = m_include_file_sequence.begin();

	for (auto inc = m_include_file_sequence.begin(); inc != m_include_file_sequence.end(); inc++)
	{
		if ((*inc) == (prefix + file)) {
			offset = inc;
		}
	}

	m_include_file_sequence.insert(offset,
		interpr->m_include_file_sequence.begin(),
		interpr->m_include_file_sequence.end()
	);

	delete interpr;
	delete astTree;

	return true;

}

bool ScriptC::Obj::CerInterpreter::visit_ArrayVar(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ArrayVar) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need ArrayVar");
	}

	ArrayVar* array_ast = dynamic_cast<ArrayVar*>(node);

	auto array_ast_params = array_ast->getExpr();

	int index = 0;
	std::string param3Str;

	auto_c pushArray;
	auto_c pushIndex;
	auto_c stepIndex;

	pushIndex << 0;
	stepIndex << 1;
	numberT unKnowIndex = 0;

	for (auto& i : array_ast_params) {
		visit(i, ret, this);

		if (array_ast->gethasOnlyIndex()) {
			std::string param3StrPath;
			std::stringstream ss;
			
			if (!param3Str.empty()) 
			{
				ss << ",";
				ss >> param3StrPath;
			}

			if(array_ast->gethasLeftIndex())
				param3StrPath += "0:(s)";
			else
				param3StrPath += "0:(i)";

			param3Str += param3StrPath;
		}
		else if (analysExprOp(i) == AST::AstType::Num) {
			pushArray[pushIndex] = m_vm_code.back().getCodeParams()->find("param1")->second;
			m_vm_code.pop_back();
			pushIndex >> unKnowIndex;
			pushIndex = pushIndex + stepIndex;
		}
		else if (analysExprOp(i) == AST::AstType::Var) {
			std::string param3StrPath;
			std::stringstream ss;
			pushArray[pushIndex] = m_vm_code.back().getCodeParams()->find("param1")->second;
			m_vm_code.pop_back();
			pushIndex >> unKnowIndex;
			pushIndex = pushIndex + stepIndex;

			if(!param3Str.empty())
				ss << ",";

			ss << unKnowIndex;
			ss >> param3StrPath;
			param3StrPath +=":(v)";
			param3Str += param3StrPath;
		}
		else {
			std::string param3StrPath;
			std::stringstream ss;
			pushIndex >> unKnowIndex;
			pushIndex = pushIndex + stepIndex;
			if (!param3Str.empty())
				ss << ",";
			ss << unKnowIndex;
			ss >> param3StrPath;
			param3StrPath += ":(u)";
			param3Str += param3StrPath;
		}
	}

	CodeParams param1;
	auto_c space,param3C;	
	space << "array";
	param3C << param3Str;

	param1.insert({ "param1", std::move(pushArray) });
	param1.insert({ "param2", std::move(space) });
	param1.insert({ "param3", std::move(param3C) });
	CommandCode opear1(CodeType::Push, std::move(param1));
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear1));

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_InterExprOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::InterExprOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need InterExprOp");
	}

	InterExprOp* inter = dynamic_cast<InterExprOp*>(node);

	CodeParams param1;

	auto_c pushTarget,space, param3C;
	if (inter->gethasLeftIndex()) {
		pushTarget << "(u)";
	}
	else {
		pushTarget << "(Ru)";
	}
	space << "interface";
	auto var = dynamic_cast<Var*>(inter->getPerson());
	if (var) {
		param3C << var->getTok().getCstr();
	}
	else {
		visit(inter->getPerson(), ret, this);
		auto code = m_vm_code.back();
		m_vm_code.pop_back();
		std::string funcName;
		(*code.getCodeParams())["param1"] >> funcName;
		funcName = "(u)" + funcName;
		(*code.getCodeParams())["param1"] << funcName;
		m_vm_code.emplace_back(code);
		param3C << "(u)";
	}
	param1.insert({ "param1", std::move(pushTarget) });
	param1.insert({ "param2", std::move(space) });
	param1.insert({ "param3", std::move(param3C) });
	m_errHis->setErrInfo(node->getDebugInfo());
	CommandCode opear1(CodeType::Push, std::move(param1));
	PushCode(std::move(opear1));
	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_AssignOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::AssignOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need AssignOp");
	}

	auto_c obj, right_ret;
	AssignOp* assign = dynamic_cast<AssignOp*>(node);
	std::string var_name;
	var_name = assign->getLeft().getCstr();
	AST* right_expr = assign->getRight();
	obj << var_name;

	visit(right_expr, ret, this);
	if (assign->getLeftIndex()) {
		auto interTest = dynamic_cast<indexExprOp*>(assign->getLeftIndex());
		if (interTest) 
		{
			obj << "(u)";

			auto_c space, param,param3;
			space << "value";
			param << var_name;
			param3 << "ptr";

			CodeParams param2;
			param2.insert({ "param1",std::move(param) });
			param2.insert({ "param2" ,std::move(space) });
			param2.insert({ "param3" ,std::move(space) });
			CommandCode opear1(CodeType::Push, param2);
			m_errHis->setErrInfo(assign->getLeftIndex()->getDebugInfo());
			PushCode(std::move(opear1));
		}

		visit(assign->getLeftIndex(), ret, this);
	}

	CodeParams param1;
	param1.insert({ "param1",std::move(obj) });
	if (assign->getCreate() == true) {
		auto_c space;
		param1.insert({ "param2",std::move(space) });
	}
	CommandCode opear1(CodeType::Pop, param1);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear1));
	m_table_temp[var_name] = 1;

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_ReturnOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ReturnAst) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need ReturnAst");
	}

	ReturnAst* assign = dynamic_cast<ReturnAst*>(node);
	CodeParams param2;
	
	auto_c right_ret;
	visit(assign->getExpr(), ret, this);

	if (analysExprOp(assign->getExpr()) != AST::AstType::EmptyAst) {
		auto_c space;
		param2.insert({ "param1" ,std::move(space) });
	}
	CommandCode opear2(CodeType::Leave, param2);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear2));
	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_FunDeclaration(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::FuncDelOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need FuncDelOp");
	}

	FuncDelOp* funcd = dynamic_cast<FuncDelOp*>(node);
	FuncHeader* funcH = dynamic_cast<FuncHeader*>(funcd->getHeader());
	std::string funcName = funcH->getFuncName();

	visit(funcH, ret, this);
	size_t last, now;
	
	last = m_vm_code.size();

	for (auto& i : funcH->getParams()) {
		m_table_temp[i.getCstr()] = 1;
	}

	if (funcH->gethasExport()) {
		return true;
	}

	if (funcH->gethasClass()) {
		m_table_temp["this"] = 1;
	}

	for (auto& i : funcd->getCode())
		visit(i, ret, this);

	now = m_vm_code.size() + 1;

	numberT ptr = (numberT)(now - last);
	auto_c blockLen;
	blockLen << ptr;
	m_vm_code.at(last-1).insertCodeParams("BlockLen", std::move(blockLen));

	CodeParams param2;
	CommandCode opear2(CodeType::Leave, param2);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear2));

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_FunctionHeader(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::FuncHeader) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need FuncHeader");
	}

	FuncHeader* funcH = dynamic_cast<FuncHeader*>(node);

	auto_c funcName, funcParamsSize;
	std::vector<auto_c> funcParams;

	funcName << funcH->getFuncName();
	funcParamsSize << (numberT)funcH->getParams().size();

	CodeParams param1;
	param1.insert({ "param1",std::move(funcName) });
	param1.insert({ "param2",std::move(funcParamsSize) });
	if (funcH->gethasExport()) {
		param1.insert({ "param3",std::move(funcParamsSize) });
	}

	int count = 4;
	std::stringstream ss;
	for (auto& i : funcH->getParams()) {
		auto_c param;
		std::string paramstr;
		ss << count;
		ss >> paramstr;
		paramstr = "param" + paramstr;
		ss.str("");
		ss.clear();
		param << i.getCstr();
		param1.insert({ paramstr , std::move(param) });
		count++;
	}

	CommandCode opear1(CodeType::Func, param1);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear1));
	m_table_temp[funcH->getFuncName()] = 1;
	
	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_FunctionCall(AST* node, autoPtr ret)
{

	if (node->getNodeType() != AstNodeType::FuncCall) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need FuncCall");
	}

	FuncCall* funcH = dynamic_cast<FuncCall*>(node);

	auto_c funcName, funcParamsSize;
	std::vector<auto_c> funcParams;

	funcName << funcH->getFuncName();
	funcParamsSize << (numberT)funcH->getParams().size();
	std::string str = funcH->getthisName();
	auto_c funcThis;
	CodeParams param1;
	param1.insert({ "param1",std::move(funcName) });
	param1.insert({ "param2",std::move(funcParamsSize) });
	if (!str.empty()) {
		funcThis << str;
		param1.insert({ "param3",std::move(funcThis) });
	}

	for (auto& i : funcH->getParams()) {
		auto_c param;
		visit(i, ret, this);
	}

	CommandCode opear1(CodeType::Call, param1);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear1));
	m_table_temp[funcH->getFuncName()] = 1;

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_InterDeclaration(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::InterfaceDecOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need InterfaceDecOp");
	}

	InterfaceDecOp* interD = dynamic_cast<InterfaceDecOp*>(node);
	InterfaceHeaderOp * interH = dynamic_cast<InterfaceHeaderOp*>(interD->getHeader());
	visit(interH, ret, this);
	for (auto& i : interD->getDatas()) {
		visit(i, ret, this);
	}

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_InterfaceHeader(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::InterfaceHeaderOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need InterfaceHeaderOp");
	}

	InterfaceHeaderOp* interH = dynamic_cast<InterfaceHeaderOp*>(node);
	CodeParams param1;
	
	auto_c childName, parentName;
	childName << interH->getName().getCstr();
	param1.insert({ "param1",std::move(childName) });
	if (!interH->getParent().getCstr().empty()) {
		parentName << interH->getParent().getCstr();
	}
	param1.insert({ "param2",std::move(parentName) });

	m_table_temp[interH->getName().getCstr()] = 1;
	CommandCode opear1(CodeType::Inter, param1);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear1));
	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_Body(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::BodyAst) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need BodyAst");
	}

	BodyAst* body = dynamic_cast<BodyAst*>(node);
	for (auto& i : body->getBlock()) {
		visit(i, ret, this);
	}
	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_Program(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ProgramAst)
		m_errHis->throwErr(EType::Interpreter,"getNode need ProgramAst");

	ProgramAst* prog = dynamic_cast<ProgramAst*>(node);
	visit(prog->getBody(), ret, this);

	CommandCode opear3(CodeType::Leave);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear3));

	
	CodeParams param2;
	numberT size = (numberT)m_vm_code.size() + 1;
	auto_c param;
	m_self_size = size;
	for (auto& i : m_include_file) {
		size += i.second.size();
	}
	param << size;
	param2.insert({ "param1",std::move(param) });
	CommandCode opear2(CodeType::Push, param2);
	if (size == 0) {
		m_errHis->setErrInfo(node->getDebugInfo());
		PushCode(opear2);
		return true;
	}
	m_vm_code.insert(m_vm_code.begin(), opear2);

	CodeParams param4;
	CommandCode opear4(CodeType::Pass, param4);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(opear4);

	for (auto j = m_include_file_sequence.rbegin(); j != m_include_file_sequence.rend(); j ++) {
		auto i = m_include_file.find(j->substr(3));
		if (i != m_include_file.end()) {
			m_vm_code.insert(m_vm_code.end(), i->second.begin(), i->second.end());
		}
	}

	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_Empty(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::EmptyAst) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need EmptyAst");
	}

	EmptyAst* pass = dynamic_cast<EmptyAst*>(node);

	CommandCode opear2(CodeType::Pass);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear2));
	return true;
}

void ScriptC::Obj::CerInterpreter::PushCode(CommandCode code)
{
	code.setDebugInfo(m_errHis->getErrorInfo());
	m_vm_code.push_back(code);
	//interlog(" - CommandCode(" + code.getCodeTypeStr());
	//for (auto& i : *(code.getCodeParams())) {
	//	interlog(" , " + i.first);
	//}
	//interlog(")\n\n");
}

AST::AstType ScriptC::Obj::CerInterpreter::analysExprOp(AST* node)
{
	if (node->getNodeType() == AST::AstType::exprOp) {
		exprOp* result = dynamic_cast<exprOp*>(node);
		return analysExprOp(result->getExpr());
	}
	return node->getNodeType();
}

void ScriptC::Obj::CerInterpreter::printCode()
{
	AutoMem::Obj::LetTools tools;
	auto column = 1;
	for (auto code = m_vm_code.begin(); code != m_vm_code.end(); code++,column ++) 
	{
#if DebuInterLog && GlobalDebugOpend
		std::cout << column;
		interlog(" - CommandCode(" + code->getCodeTypeStr());
		for (auto& i : *(code->getCodeParams())) {
			interlog(" , " + i.first + ":");

			tools.print(i.second);
	}
		interlog(")\n\n");
#endif
	}
}

std::map<std::string, int> ScriptC::Obj::CerInterpreter::getTables()
{
	return m_table_temp;
}
