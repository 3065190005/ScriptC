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
	if (m_table_temp[SYSTEM_FILE_NAME] != 1) {
		m_table_temp[SYSTEM_FILE_NAME] = 1;
	}

	if (m_table_temp[SYSTEM_MAIN_NAME] != 1) {
		m_table_temp[SYSTEM_MAIN_NAME] = 1;
	}
}

ScriptC::Obj::CerInterpreter::~CerInterpreter()
{
}

CerInterpreter* ScriptC::Obj::CerInterpreter::create(AST* tree, std::string fileName)
{
	CerInterpreter* ret = new CerInterpreter();
	if (ret == nullptr || tree == nullptr) {
		delete ret;
		ret = nullptr;
		return ret;
	}

	ret->m_rootAts = tree;
	ret->m_file_name = fileName;

	return ret;
}

ScriptC::Obj::CerInterpreter::ByteCodes& 
ScriptC::Obj::CerInterpreter::CompileCode(autoPtr ret, AST* rootAts)
{
	interlog("InterPreterCompileCode: >>>\n");

	AST* root = rootAts;
	if (root == nullptr) {
		root = m_rootAts;
	}

	visit(root, ret, this);

	printCode();
	return m_vm_code;
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

	/*
	* 2023.10.17
	* bug ： 只有当下表为左值时才会用引用，右值全是拷贝复制 VmPush (u)和(Ru)不会进入LetObject::reference
	* fix ： 当仅为变量作为右值 且 拥有下表时 改为引用方式
	*/

	if (expr->getHasIndex())
	{
		if (expr->getExpr()->getNodeType() == AstNodeType::Var)
			GetSelfVmCodes()->back().insertCodeParams("param3", auto_c());
		visit(expr->getIndex(), ret, this);
	}

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

	oneJump = GetSelfVmCodes()->size();

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

	twoJump = GetSelfVmCodes()->size();

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

		oneJump = GetSelfVmCodes()->size();

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

		twoJump = GetSelfVmCodes()->size();

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

		oneJump = GetSelfVmCodes()->size();

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

		twoJump = GetSelfVmCodes()->size();

		replaces.push_back({ oneJump,twoJump });
		replaces.push_back({ twoJump ,-1 });

		PushCode(std::move(CommandCode(CodeType::Pass, params)));
	}

	allJump = GetSelfVmCodes()->size();

	for (auto& i : replaces) {
		auto ptr = (GetSelfVmCodes()->at(i.first - 1).getCodeParams());
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

	beg_end = GetSelfVmCodes()->size() - 1;

	// pass (beg
	PushCode(std::move(CommandCode(CodeType::Pass, std::move(params))));
	beg_eip = GetSelfVmCodes()->size() - 1;

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
	end_eip = GetSelfVmCodes()->size() - 1;

	// popS (~while
	PushCode(std::move(CommandCode(CodeType::PopS, std::move(params))));

	std::string str;
	std::stringstream ss;
	ss << beg_eip;
	ss << ":";
	ss << end_eip;
	ss >> str;
	GetSelfVmCodes()->at(beg_end).getCodeParams()->find("param1")->second << str;
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

	beg_end = GetSelfVmCodes()->size() - 1;

	// pass (beg
	PushCode(std::move(CommandCode(CodeType::Pass, std::move(params))));
	beg_eip = GetSelfVmCodes()->size() - 1;

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

	/*
	* 2023.12.10
	* []下标将通过AryIndex字节码进行处理
	* 不再通过 push 进行推送
	* 提高效率
	*/
	param1 << "right";
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::AryIndex, std::move(params))));
	
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

	/*
	* 2023.12.10
	* []下标将通过AryIndex字节码进行处理
	* 不再通过 push 进行推送
	* 提高效率
	*/

	param1 << "right";
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::AryIndex, std::move(params))));

	param1 << "right";
	params.insert({ "param1",std::move(param1) });
	PushCode(std::move(CommandCode(CodeType::AryIndex, std::move(params))));

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
	end_eip = GetSelfVmCodes()->size() - 1;

	// popS (~while
	PushCode(std::move(CommandCode(CodeType::PopS, std::move(params))));

	std::string str;
	std::stringstream ss;
	ss << beg_eip;
	ss << ":";
	ss << end_eip;
	ss >> str;
	GetSelfVmCodes()->at(beg_end).getCodeParams()->find("param1")->second << str;
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

	/*
	* 2023.11.13
	* 拦截Pop，添加在pop之前调用函数调用并将返回值返回给接下来要执行的pop函数
	*/

	bool is_call_init{ var_ast->getCallInit() };
	auto func_params{ var_ast->getParams() };
	
	while (is_call_init)
	{
		CodeParams params;
		auto_c param;

		for (auto& varName : func_params) {
			visit(varName, ret, this);
		}

		param << (std::string{ var_ast->getInitInter() + ":"} + SPECIAL_FUNC_INIT);
		params.insert({ "param1",std::move(param) });

		param << (numberT)func_params.size();
		params.insert({ "param2",std::move(param) });

		PushCode(std::move(CommandCode(CodeType::Call, std::move(params))));
		break;
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

	/* 检测文件是否已包含 */
	if (m_table_temp.find(REQUIRE_FILE_PREFIX + file) != m_table_temp.end()) {
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

	/* 读取文件 */
	std::string fileTxt, path;
	while (std::getline(isExist, path))
		fileTxt.append(std::move(path) + '\n');
	isExist.close();

	/* 生成Inc字节码 */
	auto_c param1(false, false);
	CodeParams params;
	param1 << file;
	params.insert({ "param1",std::move(param1) });
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(CommandCode(CodeType::Inc, std::move(params))));

	/* 设置文件包含标识 */
	m_table_temp[REQUIRE_FILE_PREFIX + file] = 1;

	/* 编译要包含的文件 */
	auto lexical = CerLexical<char>::create(fileTxt, file);
	auto parser = CerParser::create(lexical);
	AST* astTree = parser->parser();
	delete lexical;
	delete parser;

	auto interpr = CerInterpreter::create(astTree, customPath);
	
	/* 子文件继承父文件的标识 */
	interpr->m_table_temp = m_table_temp;

	/* 生成子文件的字节码 */
	ScriptC::Obj::CerInterpreter::ByteCodes codeVec;
	codeVec = std::move(interpr->CompileCode(ret));

	m_vm_code.merge(std::move(codeVec));
	m_table_temp = interpr->m_table_temp;

	delete interpr;
	delete astTree;

	/*
	* 2023.11.22
	* require 添加Pop 丢弃require return的无效值
	* require 新增初始化语法糖
	*/
	CodeParams param2;
	CommandCode opear1(CodeType::Pop, param2);
	PushCode(std::move(opear1));

	
	if (include_ast->hasCreateVar())
	{
		std::string var_name, inter_name;
		var_name = include_ast->getVar().getCstr();
		inter_name = include_ast->getInterface().getCstr();
		m_errHis->setErrInfo(node->getDebugInfo());

		auto_c space, param;
		space << "interface";
		param << inter_name;

		CodeParams param2;
		param2.insert({ "param1",std::move(param) });
		param2.insert({ "param2" ,std::move(space) });
		PushCode(std::move(CommandCode(CodeType::Push, std::move(param2))));

		param << var_name;
		param2.insert({ "param1",std::move(param) });
		param2.insert({ "param2" ,std::move(space) });
		PushCode(std::move(CommandCode(CodeType::Pop, std::move(param2))));

		m_table_temp[var_name] = 1;
	}

	return true;

}

bool ScriptC::Obj::CerInterpreter::visit_ArrayVar(AST* node, autoPtr ret)
{
	/*
	* 2023.12.10
	* 两种数组表达式
	* 第一种， 作为数组形式 [1,2,3,4,5, index, getfunc()]
	*	(v) 表示当前值是一个变量名需要获取
	*	(u) 表示最近一个栈值
	* 第二种， 作为下表形式 var[1]
	*	"left" 表示以引用的方式获取
	*	 "right" 表示以值的方式获取
	*	
	* 
	*/
	if (node->getNodeType() != AstNodeType::ArrayVar) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need ArrayVar");
	}

	ArrayVar* array_ast = dynamic_cast<ArrayVar*>(node);

	bool only_index_mode = array_ast->gethasOnlyIndex();
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

		/*
		* 以下标方式进行解析
		*/
		if (only_index_mode) {
			std::string param3StrPath;
			std::stringstream ss;
			
			if (!param3Str.empty()) 
			{
				ss << ",";
				ss >> param3StrPath;
			}

			if(array_ast->gethasLeftIndex())
				param3StrPath += "left";
			else
				param3StrPath += "right";

			param3Str += param3StrPath;
		}

		/*
		* 以数组方式进行解析
		*/
		else if (analysExprOp(i) == AST::AstType::Num) {
			pushArray[pushIndex] = GetSelfVmCodes()->back().getCodeParams()->find("param1")->second;
			GetSelfVmCodes()->pop_back();
			pushIndex >> unKnowIndex;
			pushIndex = pushIndex + stepIndex;
		}
		else if (analysExprOp(i) == AST::AstType::Var) {
			std::string param3StrPath;
			std::stringstream ss;
			pushArray[pushIndex] = GetSelfVmCodes()->back().getCodeParams()->find("param1")->second;
			GetSelfVmCodes()->pop_back();
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

	/*
	* 2023.12.10
	* 下标字节码修改为 AryIndex
	* 数组标识保持不变
	*/
	if (!only_index_mode)
	{
		CodeParams param1;
		auto_c space, param3C;
		space << "array";
		param3C << param3Str;

		param1.insert({ "param1", std::move(pushArray) });
		param1.insert({ "param2", std::move(space) });
		param1.insert({ "param3", std::move(param3C) });
		CommandCode opear1(CodeType::Push, std::move(param1));
		m_errHis->setErrInfo(node->getDebugInfo());
		PushCode(std::move(opear1));
	}
	else
	{
		auto_c temp;
		CodeParams param1;
		if (array_ast->gethasLeftIndex())
			temp << "left";
		else
			temp << "right";

		param1.insert({ "param1", std::move(temp) });
		CommandCode opear1(CodeType::AryIndex, std::move(param1));
		m_errHis->setErrInfo(node->getDebugInfo());
		PushCode(std::move(opear1));
	}

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
		param3C << "(u)";
		FuncCall* func_call = dynamic_cast<FuncCall*>(inter->getPerson());

		visit(inter->getPerson(), ret, this);
		auto code = GetSelfVmCodes()->back();
		GetSelfVmCodes()->pop_back();


		std::string funcName;
		(*code.getCodeParams())["param1"] >> funcName;
		funcName = "(u)" + funcName;
		(*code.getCodeParams())["param1"] << funcName;

		/*2023.11.11
		* 添加函数跳转和函数调用
		* 实现特殊变量 _attr
		*/

		{
			CodeParams params;
			auto_c param;
			bool is_last = inter->gethasLeftLast() && inter->gethasLeftIndex();

			GetSelfVmCodes()->emplace_back(code);

			/*
			* 如果是左值最后一位
			* 将赋值当做参数传递给 attr
			*/

			if (is_last)
			{
				(*GetSelfVmCodes()->rbegin()->getCodeParams())["param5"] = auto_c();
				/*
				* Pop
				*/
				PushCode(std::move(CommandCode(CodeType::Pop, std::move(params))));


				/*
				* Jmp param:2
				*/
				param << numberT(2);
				params.insert({ "param1",std::move(param) });
				PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));
			}
		}
	}
	param1.insert({ "param1", std::move(pushTarget) });
	param1.insert({ "param2", std::move(space) });
	param1.insert({ "param3", std::move(param3C) });
	m_errHis->setErrInfo(node->getDebugInfo());


	

	CommandCode opear1(CodeType::Push, std::move(param1));

	/*
	* 2023.10.17
	* 函数返回值将不在进行变量转换，为返回this指针进行修改
	* 变量赋值运算等不受影响
	*/
	if (inter->getPerson()->getNodeType() != Obj::AstNodeType::FuncCall)
	{
		/*
		* 2023.11.2
		* 添加跳转和函数调用
		* 实现特殊变量 _attr
		*/
		if (var != nullptr)
		{

			/*
			* Jmp param1:4 param2:$ param4:1 param5:<value>
			*/
			CodeParams params;
			auto_c param;

			param << numberT(5);
			params.insert({ "param1",std::move(param) });

			param << "$";
			params.insert({ "param2",std::move(param) });

			param << numberT(1);
			params.insert({ "param4",std::move(param) });


			param << var->getTok().getCstr();
			params.insert({ "param5",std::move(param) });


			m_errHis->setErrInfo(node->getDebugInfo());
			PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));


			/*
			*  Swap param1:0 ,param2:2
			*/
			if (inter->gethasLeftLast() && inter->gethasLeftIndex())
			{
				param << numberT(0);
				params.insert({ "param1",std::move(param) });

				param << numberT(1);
				params.insert({ "param2",std::move(param) });
				PushCode(std::move(CommandCode(CodeType::Swap, std::move(params))));
			}


			/*
			* Push param:null
			*/
			else
			{
				param = auto_c();
				params.insert({ "param1",std::move(param) });
				PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));
			}

			/*
			*  Push , param1:<[name,type]>
			*/

			param["name"] << var->getTok().getCstr();
			param["type"] << "member";

			params.insert({ "param1",std::move(param) });
			PushCode(std::move(CommandCode(CodeType::Push, std::move(params))));

			
			/*
			* Call param1:(u):_attr , param2:2
			*/

			param << (std::string("(u)") + SPECIAL_FUNC_ATTR);
			params.insert({ "param1",std::move(param) });

			param << numberT(2);
			params.insert({ "param2",std::move(param) });

			param << auto_c();
			params.insert({ "param6",std::move(param) });
			PushCode(std::move(CommandCode(CodeType::Call, std::move(params))));

			/*
			*  Jmp , param1:2:
			*/
			param << numberT{ 2 };
			if (inter->gethasLeftLast() && inter->gethasLeftIndex())
				param << numberT{ 3 };

			params.insert({ "param1",std::move(param) });
			PushCode(std::move(CommandCode(CodeType::Jmp, std::move(params))));

		}
		// ---

		PushCode(std::move(opear1));
	}

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

bool ScriptC::Obj::CerInterpreter::visit_YieldOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::YieldOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need YieldOp");
	}

	YieldOp* assign = dynamic_cast<YieldOp*>(node);
	CodeParams param2;

	visit(assign->getExpr(), ret, this);

	CommandCode opear2(CodeType::Yield, param2);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear2));
	return true;
}

bool ScriptC::Obj::CerInterpreter::visit_ResumeOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ResumeOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::Interpreter, "getNode need ResumeOp");
	}

	ResumeOp* assign = dynamic_cast<ResumeOp*>(node);
	CodeParams param2;

	visit(assign->getExpr(), ret, this);
	visit(assign->getCoId(), ret, this);

	CommandCode opear2(CodeType::Resume, param2);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(std::move(opear2));
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
	
	last = GetSelfVmCodes()->size();

	for (auto& i : funcH->getParams()) {
		m_table_temp[i.getCstr()] = 1;
	}

	if (funcH->gethasExport()) {
		return true;
	}

	/*
	* 2023.11.30
	* 修复 require 包含的文件缺少this定义信息
	*/
	m_table_temp["this"] = 1;

	for (auto& i : funcd->getCode())
		visit(i, ret, this);

	now = GetSelfVmCodes()->size() + 1;

	numberT ptr = (numberT)(now - last);
	auto_c blockLen;
	blockLen << ptr;
	GetSelfVmCodes()->at(last-1).insertCodeParams("BlockLen", std::move(blockLen));

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
	numberT size = (numberT)GetSelfVmCodes()->size() + 1;
	auto_c param;
	param << size;
	param2.insert({ "param1",std::move(param) });
	CommandCode opear2(CodeType::Push, param2);
	if (size == 0) {
		m_errHis->setErrInfo(node->getDebugInfo());
		PushCode(opear2);
		return true;
	}

	GetSelfVmCodes()->insert(GetSelfVmCodes()->begin(), opear2);

	CodeParams param4;
	CommandCode opear4(CodeType::Pass, param4);
	m_errHis->setErrInfo(node->getDebugInfo());
	PushCode(opear4);

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

std::vector<CommandCode>* ScriptC::Obj::CerInterpreter::GetSelfVmCodes()
{
	auto finder = m_vm_code.find(m_file_name);
	if (finder == m_vm_code.end())
	{
		m_errHis->throwErr(EType::Interpreter, "GetSelfVmCodes Finder == m_vm_code.end() \n Please Check C++ Code");
		return nullptr;
	}

	auto ret = &(finder->second);
	return ret;
}

void ScriptC::Obj::CerInterpreter::PushCode(CommandCode code)
{
	code.setDebugInfo(m_errHis->getErrorInfo());
	m_vm_code[m_file_name].push_back(code);
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
	for (auto code = GetSelfVmCodes()->begin(); code != GetSelfVmCodes()->end(); code++,column ++) 
	{
#if DebuInterLog && GlobalDebugOpend
		std::cout << m_file_name << " : " << column;
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
