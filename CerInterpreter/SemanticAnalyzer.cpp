#include "..\AutoElement\LetManager.h"
#include "..\AutoElement\LetObject.h"
#include "..\ErrorHandling\ErrorHandling.h"
#include "..\CerLexical\CerLexical.h"
#include "..\CerParser\AST.h"
#include "..\CerParser\CerParser.h"
#include "..\DllReader\DllFuncReader.h"
#include "NodeVisitor.h"
#include "SymbolClass.h"
#include "SemanticAnalyzer.h"

using namespace ScriptC::Obj;

SymbolTable SemanticAnalyzer::m_global_symbol_table(nullptr,"!console",0);
std::list<std::string> SemanticAnalyzer::m_global_file_table;

ScriptC::Obj::SemanticAnalyzer::SemanticAnalyzer() : 
	m_hasMain(true),
	m_rootAts(nullptr),
	m_symbol_table(&m_global_symbol_table)
{
	if(m_global_symbol_table.findSymbol(SYSTEM_FILE_NAME, SymbolType::VarSymbol,true) == SymbolTable::SymbolfindArea::noFind)
		m_global_symbol_table.pushSymbol(SymbolClass(SYSTEM_FILE_NAME, SymbolClass("let")));

	if (m_global_symbol_table.findSymbol(SYSTEM_MAIN_NAME, SymbolType::VarSymbol, true) == SymbolTable::SymbolfindArea::noFind)
		m_global_symbol_table.pushSymbol(SymbolClass(SYSTEM_MAIN_NAME, SymbolClass("let")));
}

ScriptC::Obj::SemanticAnalyzer::~SemanticAnalyzer()
{
}

SemanticAnalyzer* ScriptC::Obj::SemanticAnalyzer::create(AST* tree)
{
	SemanticAnalyzer* ret = new SemanticAnalyzer();
	ret->m_rootAts = tree;

	if (ret == nullptr || tree == nullptr) {
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool ScriptC::Obj::SemanticAnalyzer::scanner(auto_c* ret, AST* rootAts)
{
	AST* root = rootAts;
	if (root == nullptr) {
		root = m_rootAts;
	}

	return visit(root, ret, this);
}

bool ScriptC::Obj::SemanticAnalyzer::visit_indexExprOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::indexExprOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"getNode need indexExprOp");
	}

	indexExprOp* index = dynamic_cast<indexExprOp*>(node);
	auto vec = index->getIndexs();
	for (auto& i : vec) {
		ArrayVar* arr = dynamic_cast<ArrayVar*>(i);		
		if (arr && arr->gethasLeftIndex() && arr->getExpr().size() != 1) {
			m_errHis->setErrInfo(arr->getDebugInfo());
			m_errHis->throwErr(EType::Interpreter,"left Array only can take one element");
		}
		visit(i, ret, this);
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_exprOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::exprOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need exprOp");
	}

	exprOp* expr = dynamic_cast<exprOp*>(node);

	visit(expr->getExpr(), ret, this);

	if(expr->getHasIndex())
		visit(expr->getIndex(), ret, this);

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_BinOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::BinOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need BinOp");
	}

	BinOp* binop = dynamic_cast<BinOp*>(node);
	visit(binop->getLeft(), ret, this);
	visit(binop->getRight(), ret, this);
	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_IfExpr(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::IfExpr) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need BinOp");
	}

	IfExpr* binop = dynamic_cast<IfExpr*>(node);
	auto _if = binop->getIf();
	auto _elifs = binop->getElifs();
	auto _else = binop->getElse();

	visit(_if.first, ret, this);
	
	SymbolTable funcArea(m_symbol_table, "~if", m_symbol_table->getLevel() + 1);
	m_symbol_table = &funcArea;
	for (auto& i : _if.second) {
		visit(i, ret, this);
	}
	m_symbol_table = funcArea.getParent();

	for (auto& _elif : _elifs) {
		visit(_elif.first, ret, this);

		SymbolTable funcArea(m_symbol_table, "~elif", m_symbol_table->getLevel() + 1);
		m_symbol_table = &funcArea;
		for (auto& i : _elif.second) {
			visit(i, ret, this);
		}
		m_symbol_table = funcArea.getParent();
	}

	if (_else.first) {
		visit(_else.first, ret, this);
		SymbolTable funcArea(m_symbol_table, "~else", m_symbol_table->getLevel() + 1);
		m_symbol_table = &funcArea;
		for (auto& i : _else.second) {
			visit(i, ret, this);
		}
		m_symbol_table = funcArea.getParent();
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_WhileExpr(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::WhileExpr) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need WhileExpr");
	}

	WhileExpr* whileOp = dynamic_cast<WhileExpr*>(node);

	AST* expr = whileOp->getExpr();
	auto codes = whileOp->getCodes();

	visit(expr, ret, this);

	SymbolTable funcArea(m_symbol_table, "~while", m_symbol_table->getLevel() + 1);
	
	m_symbol_table = &funcArea;

	for (auto& i : codes) {
		visit(i, ret, this);
	}

	m_symbol_table = funcArea.getParent();

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_ForExpr(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ForExpr) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need ForExpr");
	}

	ForExpr* _forexpr = dynamic_cast<ForExpr*>(node);
	CerTokClass var = _forexpr->getVar();
	AST* expr = _forexpr->getExpr();
	auto codes = _forexpr->getCodes();

	visit(expr, ret, this);

	SymbolTable funcArea(m_symbol_table, "~while", m_symbol_table->getLevel() + 1);
	m_symbol_table = &funcArea;

	SymbolClass var_symbol(var.getCstr(), SymbolClass("let"));
	m_symbol_table->pushSymbol(var_symbol);

	for (auto& i : codes) {
		visit(i, ret, this);
	}

	m_symbol_table = funcArea.getParent();

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_UnaryOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::UnaryOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need UnaryOp");
	}

	UnaryOp* unaryOp = dynamic_cast<UnaryOp*>(node);
	return visit(unaryOp->getRight(), ret, this);

}

bool ScriptC::Obj::SemanticAnalyzer::visit_Num(AST* node, autoPtr ret)
{
	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_Str(AST* node, autoPtr ret)
{
	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_Var(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::Var) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need Var");
	}

	Var* var_ast = dynamic_cast<Var*>(node);

	SymbolType type = SymbolType::None;
	std::string name = var_ast->getTok().getCstr();

	if (!m_symbol_table) {
		m_errHis->setErrInfo(var_ast->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"undefine symbol " + name);
	}

	symbArea finder = symbArea::noFind;
	if (var_ast->getNodeType() == AST::AstType::Var) {
		type = SymbolType::VarSymbol;
		finder = m_symbol_table->findSymbol(name, type, true);
	}

	if (finder == symbArea::noFind) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"undefine symbol " + name);
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_BreakOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::BreakOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need BreakOp");
	}

	BreakOp* var_ast = dynamic_cast<BreakOp*>(node);

	bool find = false;
	auto _symbolArea = m_symbol_table;
	while (_symbolArea->getName()[0] == '~') {
		if (_symbolArea->getName() == "~while") {
			find = true;
			break;
		}
		_symbolArea = _symbolArea->getParent();
	}

	if (!find) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"break can take only in Circulate code");
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_ContinueOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ContinueOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need ContinueOp");
	}

	ContinueOp* var_ast = dynamic_cast<ContinueOp*>(node);

	bool find = false;
	auto _symbolArea = m_symbol_table;
	while (_symbolArea->getName()[0] == '~') {
		if (_symbolArea->getName() == "~while") {
			find = true;
			break;
		}
		_symbolArea = _symbolArea->getParent();
	}

	if (!find) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"continue can take only in Circulate code");
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_InterNew(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::InterNew) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need InterNew");
	}

	InterNew* var_ast = dynamic_cast<InterNew*>(node);

	SymbolType type = SymbolType::None;
	std::string name = var_ast->getName().getCstr();

	if (!m_symbol_table) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"undefine symbol " + name);
	}

	symbArea finder = symbArea::noFind;
	if (var_ast->getNodeType() == AST::AstType::InterNew) {
		type = SymbolType::InterSymbol;
		finder = m_symbol_table->findSymbol(name, type, true);
	}

	if (finder == symbArea::noFind) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"undefine symbol " + name);
	}

	/*
	* 2023.11.13
	* 如果有调用_init函数 则进行检查
	*/

	bool is_init_call{ var_ast->getCallInit() };
	auto params{ var_ast->getParams() };
	
	while (is_init_call)
	{
		// 检查函数是否合法
		auto funParams = params;
		/*
		* 递归寻找父接口_init函数
		*/
		std::string error_info;
		
		while (true)
		{
			// 初始化变量
			error_info.clear();
			std::string func_name{ name + ":" + SPECIAL_FUNC_INIT };
			symbArea finder = m_symbol_table->findSymbol(name, SymbolType::InterSymbol, true);
			auto funcClass = m_symbol_table->getFuncSymbol(func_name, true);

			// 尝试检测_init 函数是否存在
			if (funcClass.getName() == "let") {
				error_info = "func is not define" + func_name;
			}

			// 强制检测参数是否正确 不添加重写功能
			else if (funcClass.getParams().size() != funParams.size()) {
				error_info = "can not take func params";
				name.clear();
			}

			// 通过测试
			else
			{
				var_ast->setInitInter(name);
				break;
			}

			// 未找到接口 且 调用检测失败
			if (finder == symbArea::noFind && !error_info.empty())
			{
				m_errHis->setErrInfo(var_ast->getDebugInfo());
				m_errHis->throwErr(EType::SemanticAnalyzer, error_info);
				return false;
			}
			// 获取父接口，尝试在父接口中寻找相关初始化函数
			else {
				auto inter = m_symbol_table->getInterSymbol(name, true);
				name = inter.getInterParent();
			}

			// 未找到父接口
			if (name.empty())
			{
				m_errHis->setErrInfo(var_ast->getDebugInfo());
				m_errHis->throwErr(EType::SemanticAnalyzer, error_info);
				return false;
			}
			else
				continue;
		}


		for (auto& varName : funParams) {
			visit(varName, ret, this);
		}


		break;
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_IncludeFile(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::IncludeFile) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need IncludeFile");
	}

	extern std::string G_Res_path;
	IncludeFile* include_ast = dynamic_cast<IncludeFile*>(node);

	std::string file = include_ast->getFile().getCstr();
	
	if (std::find(m_global_file_table.begin(), m_global_file_table.end(), file) != m_global_file_table.end()) {
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
		m_errHis->throwErr(EType::SemanticAnalyzer,"include file is not Exist");
	}
	
	std::string fileTxt,path;
	while (std::getline(isExist, path))
		fileTxt.append(std::move(path) + '\n');
	isExist.close();

	// file codeing
	m_global_file_table.push_back(file);

	auto lexical = CerLexical<char>::create(fileTxt, file);
	auto parser = CerParser::create(lexical);
	AST* astTree = parser->parser();
	delete lexical;
	delete parser;

	auto seman = SemanticAnalyzer::create(astTree);
	seman->m_hasMain = false;
	auto_c seman_ret(false, false);
	seman->m_symbol_table = m_symbol_table;
	seman->scanner(&seman_ret);

	/*
	* 2023.11.22
	* 添加新语法糖 require(string, var) new interface
	*/
	if (include_ast->hasCreateVar())
	{
		std::string inter_name, var_name;
		inter_name = include_ast->getInterface().getCstr();
		var_name = include_ast->getVar().getCstr();

		if (!m_symbol_table) {
			m_errHis->setErrInfo(node->getDebugInfo());
			m_errHis->throwErr(EType::SemanticAnalyzer, "undefine symbol " + inter_name);
		}

		symbArea inter_finder = symbArea::noFind;
		symbArea var_finder = symbArea::noFind;

		inter_finder = m_symbol_table->findSymbol(inter_name, SymbolType::InterSymbol, true);
		var_finder = m_symbol_table->findSymbol(var_name, SymbolType::VarSymbol, true);

		if (inter_finder == symbArea::noFind) {
			m_errHis->setErrInfo(node->getDebugInfo());
			m_errHis->throwErr(EType::SemanticAnalyzer, "undefine symbol " + inter_name);
		}

		if (var_finder != symbArea::noFind) {
			m_errHis->setErrInfo(node->getDebugInfo());
			m_errHis->throwErr(EType::SemanticAnalyzer, "symbom " + var_name + " was already define");
		}

		SymbolClass var_symbol(var_name, SymbolClass("let"));
		m_symbol_table->pushSymbol(var_symbol);
	}

	delete seman;
	delete astTree;

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_ArrayVar(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ArrayVar) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need ArrayVar");
	}

	ArrayVar* array_ast = dynamic_cast<ArrayVar*>(node);
	for (auto& i : array_ast->getExpr()) {
		visit(i, ret, this);
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_InterExprOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::InterExprOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need InterExprOp");
	}

	InterExprOp* array_ast = dynamic_cast<InterExprOp*>(node);
	AST* person = array_ast->getPerson();
	if (person->getNodeType() == AST::AstType::FuncCall) {
		FuncCall* func = dynamic_cast<FuncCall*>(person);

		// fixed 不检查捕获 变量 是否存在的bug
		std::string thisName = func->getthisName();
		auto finder = m_symbol_table->findSymbol(thisName, SymbolType::VarSymbol, true);
		if (finder == symbArea::noFind && !thisName.empty()) {
			m_errHis->setErrInfo(func->getDebugInfo());
			m_errHis->throwErr(EType::SemanticAnalyzer, "symbom " + thisName + " is not define");
		}


		auto param = func->getParams();
		for (auto& i : param) {
			visit(i, ret, this);
		}
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_AssignOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::AssignOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need AssignOp");
	}

	AssignOp* assign = dynamic_cast<AssignOp*>(node);

	SymbolType type = SymbolType::None;
	bool let_create = assign->getCreate();
	std::string name = assign->getLeft().getCstr();

	if (let_create == true) {
		auto index = dynamic_cast<indexExprOp*>(assign->getLeftIndex());
		if (index && index->getInterSize() > 0) {
			m_errHis->setErrInfo(index->getDebugInfo());
			m_errHis->throwErr(EType::SemanticAnalyzer,"symbom Cant create value " + name + " with person");
		}
	}

	visit(assign->getRight(), ret, this);

	if (!m_symbol_table) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"cant define symbol " + name);
	}

	if (assign->getLeftIndex() && assign->getLeft().getCstr() != "this") {
		visit(assign->getLeftIndex(), ret, this);
	}

	symbArea finder;
	type = SymbolType::VarSymbol;
	finder = m_symbol_table->findSymbol(name, type, true);

	if (finder == symbArea::Self && let_create) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"symbom " + name + " was already define");
	}

	if (finder == symbArea::noFind && !let_create) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"symbom " + name + " is undefine");
	}

	if (finder != symbArea::Self && let_create) {
		SymbolClass var_symbol(name, SymbolClass("let"));
		m_symbol_table->pushSymbol(var_symbol);
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_ReturnOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ReturnAst) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need ReturnAst");
	}

	ReturnAst* assign = dynamic_cast<ReturnAst*>(node);
	visit(assign->getExpr(), ret, this);
	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_YieldOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::YieldOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need YieldOp");
	}

	YieldOp* assign = dynamic_cast<YieldOp*>(node);
	if (m_symbol_table->getLevel() < 2)
	{
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "Yield OutSide Function");
		return true;
	}

	visit(assign->getExpr(), ret, this);
	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_ResumeOp(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ResumeOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need ResumeOp");
	}

	ResumeOp* assign = dynamic_cast<ResumeOp*>(node);
	visit(assign->getCoId(), ret, this);
	visit(assign->getExpr(), ret, this);
	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_FunDeclaration(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::FuncDelOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need FuncDelOp");
	}

	FuncDelOp* funcd = dynamic_cast<FuncDelOp*>(node);
	FuncHeader* funcH = dynamic_cast<FuncHeader*>(funcd->getHeader());
	std::string funcName = funcH->getFuncName();

	if (funcH->gethasExport()) {
		DllFuncReader* manager = DllFuncReader::getInstance();
		if (!manager || manager->getFuncFromDll(funcName) == nullptr) {
			m_errHis->setErrInfo(funcH->getDebugInfo());
			m_errHis->throwErr(EType::SemanticAnalyzer,"func " + funcName + " can not find with export");
		}
	}

	visit(funcd->getHeader(), ret, this);
	SymbolTable funcArea(m_symbol_table,funcH->getFuncName(),m_symbol_table->getLevel() + 1);
	m_symbol_table = &funcArea;
	
	for (auto& i : funcH->getParams()) {
		m_symbol_table->pushSymbol(SymbolClass(i.getCstr(), SymbolClass("let")));
	}

	m_symbol_table->pushSymbol(SymbolClass("this", SymbolClass("let")));

	for (auto& i : funcd->getCode()) {
		visit(i, ret, this);
	}

	m_symbol_table = funcArea.getParent();
	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_FunctionHeader(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::FuncHeader) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need FuncHeader");
	}

	FuncHeader* funcd = dynamic_cast<FuncHeader*>(node);
	
	symbArea finder;
	SymbolType type;
	std::string name;

	name = funcd->getFuncName();
	type = SymbolType::FuncSymbol;
	finder = m_symbol_table->findSymbol(name, type, true);
	if (finder != symbArea::noFind) {
		m_errHis->setErrInfo(funcd->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"symbom " + name + " was aleardy define");
	}

	std::vector<std::string> func_params;
	auto parmaVecCer = funcd->getParams();
	for (auto& i : parmaVecCer) {
		if (i.getCstr()== "this") {
			m_errHis->setErrInfo(funcd->getDebugInfo());
			m_errHis->throwErr(EType::SemanticAnalyzer,"this can not be param in FuncHeader");
		}

		func_params.push_back(i.getCstr());
	}

	/*
	* 2023.11.14
	* 将所有函数声明推送到
	* 全局符号表中
	*/

	SymbolClass symbol{ name,func_params };
	auto symbol_tab = m_symbol_table;

	while (symbol_tab->getLevel() > 1)
		symbol_tab = symbol_tab->getParent();
	
	symbol_tab->pushSymbol(symbol);
	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_FunctionCall(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::FuncCall) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need FuncCall");
	}

	FuncCall* funcd = dynamic_cast<FuncCall*>(node);

	auto funParams = funcd->getParams();
	auto funcClass = m_symbol_table->getFuncSymbol(funcd->getFuncName(), true);
	auto thisName = funcd->getthisName();

	if (funcClass.getName() == "let") {
		m_errHis->setErrInfo(funcd->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"func is not define " + funcd->getFuncName());
	}

	if (funcClass.getParams().size() != funParams.size()) {
		m_errHis->setErrInfo(funcd->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"can not take func params");
	}

	// fixed 不检查捕获 变量 是否存在的bug
	auto finder = m_symbol_table->findSymbol(thisName, SymbolType::VarSymbol, true);
	if (finder == symbArea::noFind && !thisName.empty()) {
		m_errHis->setErrInfo(funcd->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "symbom " + thisName + " is not define");
	}

	for (auto& varName : funParams) {
		visit(varName, ret, this);
	}

	return true;

}

bool ScriptC::Obj::SemanticAnalyzer::visit_InterDeclaration(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::InterfaceDecOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need InterfaceDecOp");
	}

	InterfaceDecOp* interD = dynamic_cast<InterfaceDecOp*>(node);
	visit(interD->getHeader(), ret, this);

	InterfaceHeaderOp* InterH = dynamic_cast<InterfaceHeaderOp*>(interD->getHeader());
	SymbolTable funcArea(m_symbol_table, InterH->getName().getCstr(), m_symbol_table->getLevel() + 1);
	m_symbol_table = &funcArea;

	for (auto& i : interD->getDatas()) {
		if (i->getNodeType() == AST::AstType::FuncDelOp) {
			FuncDelOp* delOp = dynamic_cast<FuncDelOp*>(i);
			FuncHeader* header = dynamic_cast<FuncHeader*>(delOp->getHeader());
			header->setHasClass(true);
			if (!header || header->gethasExport()) {
				m_errHis->setErrInfo(header->getDebugInfo());
				m_errHis->throwErr(EType::SemanticAnalyzer,"on " + InterH->getName().getCstr()
					+ " interface can not define function with export");
			}
		}
		visit(i, ret, this);
	}

	m_symbol_table = funcArea.getParent();
	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_InterfaceHeader(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::InterfaceHeaderOp) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need InterfaceHeaderOp");
	}

	InterfaceHeaderOp* interH = dynamic_cast<InterfaceHeaderOp*>(node);

	symbArea finder;
	SymbolType type;
	std::string name , parent;

	name = interH->getName().getCstr();
	parent = interH->getParent().getCstr();
	type = SymbolType::InterSymbol;

	finder = m_symbol_table->findSymbol(name, type, true);
	if (finder != symbArea::noFind) {
		m_errHis->setErrInfo(interH->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer,"symbom " + name + " was aleardy define");
	}

	/*
	* 2023.10.23
	* 对要继承的父类进行判断
	*/
	finder = m_symbol_table->findSymbol(parent, type, true);
	if (!parent.empty() && finder == symbArea::noFind) {
		m_errHis->setErrInfo(interH->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "symbom " + parent + " was not define");
	}

	SymbolClass symbol(name, parent);
	m_symbol_table->pushSymbol(symbol);
	return true;

}

bool ScriptC::Obj::SemanticAnalyzer::visit_Body(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::BodyAst) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need BodyAst");
	}

	BodyAst* body = dynamic_cast<BodyAst*>(node);
	for (auto& i : body->getBlock()) {
		visit(i, ret, this);
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_Program(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::ProgramAst) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need ProgramAst");
	}

	if (m_hasMain) {
		SymbolTable funcArea(m_symbol_table, "!Program", m_symbol_table->getLevel() + 1);
		m_symbol_table = &funcArea;
		ProgramAst* prog = dynamic_cast<ProgramAst*>(node);
		visit(prog->getBody(), ret, this);
		m_symbol_table = funcArea.getParent();
	}
	else {
		ProgramAst* prog = dynamic_cast<ProgramAst*>(node);
		visit(prog->getBody(), ret, this);
	}

	return true;
}

bool ScriptC::Obj::SemanticAnalyzer::visit_Empty(AST* node, autoPtr ret)
{
	if (node->getNodeType() != AstNodeType::EmptyAst) {
		m_errHis->setErrInfo(node->getDebugInfo());
		m_errHis->throwErr(EType::SemanticAnalyzer, "getNode need EmptyAst");
	}

	return true;
}
