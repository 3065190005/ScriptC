#include <iostream>
#include <fstream>
#include <chrono>
#include <list>
#include <memory>

#include "QuickLib/quicklib.h"
#include "AutoElement/LetManager.h"
#include "AutoElement/LetObject.h"
#include "CerLexical/CerLexical.h"
#include "CerParser/CerParser.h"
#include "CerParser/AST.h"
#include "DllReader/DllFuncReader.h"
#include "ErrorHandling/ErrorHandling.h"
#include "CerInterpreter/SemanticAnalyzer.h"
#include "CerInterpreter/CerInterpreter.h"
#include "CerVirtualMachine/CerStackSystem.h"
#include "CerVirtualMachine/CerVm.h"

#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

std::string G_TipsSymbol = "->: ";
std::string G_consoleTxt =
R"(ScriptC ��Last Update : 2023.12.10 | LetObject : v9_10) [console mode]
If you want to compile and run the code, type Enter twice.

)";

using namespace ScriptC::Obj;

// ��Դ�ļ�����
std::string G_SourcesFile;

// ��Դ�ļ���
std::string G_mainFile;

// ��Ĭ��·��
std::string G_Res_path = ".\\res\\";


// �������� debug
#ifdef _DEBUG
void TestCodeCall();
#endif // NDEBUG && _DEBUG

// �������� released
#ifdef NDEBUG
void ProgramerCall();
#endif

#ifdef _PACKAGE
void ScriptRun();
#endif

int CmdRunType();

int getCmdParam(int args, char** argv);

void InitConsole();

char* getExePath();

int main(int args , char** argv) {

	// InitConsole();
	std::string ret;

#ifdef _PACKAGE
	char* pValue;
	pValue = getExePath();
	if (pValue != NULL) {
		G_Res_path = pValue;
		G_Res_path += "\\res\\";
		delete[] pValue;
	}
	else {
		std::cout << "Can not find res folder";
		return 0;
	}
#endif

	if (args == 1) {
		DllFuncReader* manager = DllFuncReader::getInstance();
		auto func = manager->getFuncFromDll("__system___setArgv__");
		manager->callFunc(func, (std::vector<auto_c>*)argv, (auto_c*)&args);

		return CmdRunType();
	}

	if (getCmdParam(args, argv) != 0) {
		return 0;
	}
	
	args -= 2;
	argv += 2;

	DllFuncReader* manager = DllFuncReader::getInstance();
	auto func = manager->getFuncFromDll("__system___setArgv__");
	manager->callFunc(func, (std::vector<auto_c>*)argv, (auto_c*)&args);


#if defined _DEBUG
	TestCodeCall();
#elif defined NDEBUG && not defined _PACKAGE
	ProgramerCall();
#else
	ScriptRun();
	// ---
#endif
	
	// ::FreeConsole();
	return 0;
}
#ifdef _PACKAGE
void ScriptRun() {
	const char* update = "package update";

	std::string input;
	if (G_SourcesFile.empty())
		input = R"(return ;)";
	else
		input = std::move(G_SourcesFile);

	try {
		auto lexical = CerLexical<char>::create(input,G_mainFile);
		auto parser = CerParser::create(lexical);
		AST* astTree = parser->parser();
		delete lexical;
		delete parser;

		CerInterpreter::ByteCodes codeVec;
		auto_c ret(false, false), seman_ret(false, false), vm_ret(false, false);
		auto seman = SemanticAnalyzer::create(astTree);
		auto interpr = CerInterpreter::create(astTree, G_mainFile);
		seman->scanner(&seman_ret);
		codeVec = std::move(interpr->CompileCode(&ret, nullptr));

		delete seman;
		delete interpr;

		auto codeVm = CerVm::create(std::move(codeVec));
		codeVm->setCodeFile(G_mainFile);

		codeVm->runTime();
		delete astTree;
	}
	catch (std::string str) {
		auto errinst = ErrorHandling::getInstance();
		auto debug_list = errinst->getErrorList();
		std::cout << "Unexpected exit for the following reasons : \n" << std::endl;
		std::string space = " ";
		for (auto& debug_info : debug_list)
		{
			auto info = debug_info.second;
			printf("%s[error] in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n\n",
				space.c_str(), info.file.c_str(), info.column, info.row, info.character.c_str());
			space += " ";
		}
		std::cout << str << std::endl;
	}
	catch (const char* cstr) {
		std::string str{ cstr };
		auto errinst = ErrorHandling::getInstance();
		auto debug_list = errinst->getErrorList();
		std::cout << "Unexpected exit for the following reasons : \n" << std::endl;
		std::string space = " ";
		for (auto& debug_info : debug_list)
		{
			auto info = debug_info.second;
			printf("%s[error] in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n\n",
				space.c_str(), info.file.c_str(), info.column, info.row, info.character.c_str());
			space += " ";
		}
		std::cout << str << std::endl;
	}
	catch (...) {
		auto errinst = ErrorHandling::getInstance();
		auto debug_list = errinst->getErrorList();
		std::cout << "Unexpected exit for the following reasons : \n" << std::endl;
		std::string space = " ";
		for (auto& debug_info : debug_list)
		{
			auto info = debug_info.second;
			printf("%s[error] in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n\n",
				space.c_str(), info.file.c_str(), info.column, info.row, info.character.c_str());
			space += " ";
		}

		std::cout << "Crash : unknow error, please check C++ codes." << std::endl;
	}
}
#endif

#ifdef NDEBUG
void ProgramerCall() {
	const char* update = "released update";
	std::string input;

	if (G_SourcesFile.empty())
		input = R"(return ;)";
	else
		input = std::move(G_SourcesFile);

	auto lexical = CerLexical<char>::create(input,G_mainFile);
	auto parser = CerParser::create(lexical);
	AST* astTree = parser->parser();
	delete lexical;
	delete parser;


	CerInterpreter::ByteCodes codeVec;
	auto_c ret(false, false), seman_ret(false, false), vm_ret(false, false);
	auto seman = SemanticAnalyzer::create(astTree);
	auto interpr = CerInterpreter::create(astTree, G_mainFile);

	seman->scanner(&seman_ret);
	codeVec = std::move(interpr->CompileCode(&ret, nullptr));

	delete seman;
	delete interpr;

	auto codeVm = CerVm::create(std::move(codeVec));
	codeVm->setCodeFile(G_mainFile);

	std::chrono::steady_clock::time_point time_befor = std::chrono::steady_clock::now();
	std::cout << "Program Interpreter : stdout <<< \n\n";
	codeVm->runTime();
	std::chrono::steady_clock::time_point time_after = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(time_after - time_befor);
	std::cout << "\n\nProgram Return >>>" << std::endl;
	std::cout << " [result] \n";
	std::cout << std::endl << "[*] Program run over with " << time_span.count() << std::endl << std::endl;

	delete astTree;
}
#endif

#ifdef _DEBUG

void TestCodeCall() {

std::string input =
R"(//--- debug
let result = 123;
let abcdef = 456;

result = abcdef % result;

function getNumB():
	return 2.5;
end

require ("io");


require("os", os) new StdOs;
// require("os");
// let io = new StdIo; 


let io = new StdIo;

interface Class {
	let num = 114514.1919810;
	let str = "Hello double world";

	// ���ⷽ�� (��������)
	function _gc():
		io.print("\n gc called " + this.str + "\n");
	end

	function getNum():
		return 1.63;
	end
	function getNumB():
		return "Hello Class getFuncB()\n";
	end

	function PrintNum():
		io.println(this.num);
	end

	function subNum():
		this.num = "This Change!";
	end
}

interface SubClass override Class{
	let num = 1000;
	
	function subNum():
		this.num = "Sub Class Overide This Change!";
	end

	function _init(value):
		this.num = value;
	end

	function _attr(info, name):
		for i in info:
			io.println(i);
		end

		if (info.type == "function"):
			for i in name:
				io.println(i);
			end
		else:
			io.println(name);
		end

	end
}

function getNum():
	return 1; 
end 

function test() : 
	return 0;
end

let str = "Hello World!";
let assignValue = 123;
let space = test();
let array = getNum();
array[0] = [0,1,1,2,3,0,7];


let cls = new Class;
cls.num[0] = !cls.getNum();

if (str == "Hello World") :
	cls.num[0] = 1;
elif (assignValue == 456) :
	cls.num[0] = 2;
else:
	cls.num[0] = 3;
end


let whileIndex = 0;
while (whileIndex < 10):
	cls.num[0] = cls.num[0] * 2;
	whileIndex = whileIndex + 1;
	if(whileIndex < 5):
		continue;
	elif(whileIndex >= 5):
		break;
	end
end

let forResu = 0;
whileIndex = [0,1,2,3,4,5,6,7,8,9];
for i in whileIndex:
	forResu = forResu + i;
end


let maps = 0;
maps[0] = "*********\n";
maps[1] = "****x****\n";
maps[2] = "*********\n";

io.print(maps);

cls.num = 11111;
cls.str = "Hello World";
cls.PrintNum()<cls>;
cls.subNum();
cls.PrintNum()<cls>;
io.println("\n\n --- sub class called ---\n");

let subcls = new SubClass;
subcls.PrintNum()<subcls>;
subcls.subNum();
subcls.PrintNum()<subcls>;

io.println("\n\n --- sub class init ---\n");
let init_cls = new SubClass("init change !");
io.println(init_cls.num);

io.println("\n\n --- sub class attr --- \n");

init_cls.getAAA("Abc");
let space_s = init_cls.numbert;
init_cls.getAAA("CCC") = "value";
init_cls.numbert = "numberT value";

// ����ע��
io.println("program over");


io.println("\n\n --- function yield Test --- \n");

function yieldFunc(param):
	io.println("yieldFunc " + param);
	param = co_yield(param + " co_yield");
	io.println(param);
	return param + " return";
end

let co_yield_value = yieldFunc("This is Param");
io.println(co_yield_value[1]);
co_yield_value = co_resume(co_yield_value[0], co_yield_value[1] + " co_resume");
io.println(co_yield_value);


io.println("\n\n --- function yield this Test --- \n");

function yieldFuncThis():
	io.println("yieldFuncThis " + this);
	this = "yieldFuncThis" + this;
	this = co_yield(this + " co_yield");
	io.println(this);
	return this + " return";
end

let this_value = "This is this Value";
let co_yield_value_this = yieldFuncThis()<this_value>;
io.println(co_yield_value_this[1]);
this_value = co_resume(co_yield_value_this[0], co_yield_value_this[1] + " co_resume");
io.println(this_value);


io.println("\n\n --- interface co_yield Test --- \n");
interface co_yieldClass{
	let str = "Default";
	let child = "Child";
	function yieldFunc():
		let value = "yeildFunc call";
		this.str = this.str + " co_yield";

		io.println(value);
		io.println(this.str);

		value = co_yield(value + " co_yield");

		io.println(value);
		io.println(this.str);

		return value + " Return";
	end
}

let co_yield_cls = new co_yieldClass;
let value = co_yield_cls.yieldFunc();
io.println(value[1]);
io.println(co_yield_cls.str);

co_yield_cls.str = co_yield_cls.str + " co_resume";
value = co_resume(value[0], value[1] + "Resume");

io.println(value);

io.println("\n\n --- Right interface co_yield Test --- \n");
function getRight():
	return new co_yieldClass;
end

value = getRight().yieldFunc();
io.println(value[1]);
value = co_resume(value[0], value[1] + "Resume");
io.println(value);

io.println("\n\n --- co_yield With Require --- \n");

io.println("main Called");
require("example\\yield_file");
io.println("main co_yield info Ret " + info[1]);
info = co_resume(info[0],info[1]);
io.println("main function return info " + info);


io.println("\n\n --- co_yield Sell Buy Apples --- \n");

function sell(max):
	let count = co_yield(max);
	while(count >= 0):
		io.println("Sell Apple: " + count);
		count = co_yield(count);
	end
end

function buy(max):
	let count = sell(max);
	while(count[1] >= 0):
		count = co_resume(count[0], count[1]);
		io.println("Buy Apple: " + count[1]);
		count[1] = count[1] - 1;
	end
end

let person = buy(5);

io.println("\n\n --- FILE_NAME && MAIN_NAME --- \n");


io.println(__FILE_NAME__);
io.println(__MAIN_NAME__);
require("example\\console_var");
io.println(__FILE_NAME__);
io.println(__MAIN_NAME__);

os.system("pause");
return forResu;
)";


	std::cout << "Test Code �� \n";
	AST* astTree = nullptr;
	auto ta = true;
	auto fa = false;

	auto_c param1, param2;

	// lexic, paser, seman, inter, vmret, print
	bool control[] = { ta,		ta,	   ta,	  ta,	ta,	   fa };
	G_mainFile = "sources.sc";

#if DEBUG_TRY_CATCH==true
	try
	{
#endif // DEBUG_TRY_CATCH
		if (control[0]) {
			auto lexical = CerLexical<char>::create(input, "sources");
			if (control[1]) {
				auto parser = CerParser::create(lexical);
				astTree = parser->parser();

				CerInterpreter::ByteCodes codeVec;

				auto_c ret(false, false), seman_ret(false, false), vm_ret(false, false);

				if (control[2]) {
					auto seman = SemanticAnalyzer::create(astTree);
					seman->scanner(&seman_ret);

					if (control[3]) {
						std::map<std::string, int> import_file;
						auto interpr = CerInterpreter::create(astTree, G_mainFile);
						codeVec = std::move(interpr->CompileCode(&ret, nullptr));

						if (control[4]) {
							auto codeVm = CerVm::create(std::move(codeVec));
							codeVm->setCodeFile(G_mainFile);

							codeVm->runTime();

							if (control[5]) {
								auto_c index(false, false);
								std::cout << std::endl << std::endl;
							}
						}
					}
				}
			}
		}

#if DEBUG_TRY_CATCH==true
	}
	catch (std::string str) {
		auto errinst = ErrorHandling::getInstance();
		auto debug_list = errinst->getErrorList();
		std::cout << "Unexpected exit for the following reasons : \n" << std::endl;
		std::string space = " ";
		for (auto& debug_info : debug_list)
		{
			auto info = debug_info.second;
			printf("%s[error] in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n\n",
				space.c_str(), info.file.c_str(), info.column, info.row, info.character.c_str());
			space += " ";
		}
		std::cout << str << std::endl;
	}
	catch (const char* cstr) {
		std::string str{ cstr };
		auto errinst = ErrorHandling::getInstance();
		auto debug_list = errinst->getErrorList();
		std::cout << "Unexpected exit for the following reasons : \n" << std::endl;
		std::string space = " ";
		for (auto& debug_info : debug_list)
		{
			auto info = debug_info.second;
			printf("%s[error] in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n\n",
				space.c_str(), info.file.c_str(), info.column, info.row, info.character.c_str());
			space += " ";
		}
		std::cout << str << std::endl;
	}
	catch (...) {
		auto errinst = ErrorHandling::getInstance();
		auto debug_list = errinst->getErrorList();
		std::cout << "Unexpected exit for the following reasons : \n" << std::endl;
		std::string space = " ";
		for (auto& debug_info : debug_list)
		{
			auto info = debug_info.second;
			printf("%s[error] in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n\n",
				space.c_str(), info.file.c_str(), info.column, info.row, info.character.c_str());
			space += " ";
		}

		std::cout << "Crash : unknow error, please check C++ codes." << std::endl;
	}
#endif // DEBUG_TRY_CATCH


	delete astTree;
	return;
}


#endif // _DEBUG || NDEBUG

int getCmdParam(int args, char** argv) {
	if (args >= 2) {
		std::string str = argv[1];
		std::ifstream file_ptr;
		std::ostringstream ss;
		file_ptr.open(str, std::ios::in | std::ios::binary);
		G_mainFile = str;
		
		if (!file_ptr.is_open()) {
			std::cout << "can't open and read file with �� " << G_mainFile << " ��" << std::endl;
			file_ptr.close();
			return -1;
		}

		ss << file_ptr.rdbuf();
		std::string contents(ss.str());
		file_ptr.close();
		G_SourcesFile = std::move(contents);
	}

	return 0;
}

void InitConsole()
{
	HWND hwnd = GetConsoleWindow();
	if (hwnd)
		return;

	::AllocConsole();

	// std::cout, std::clog, std::cerr, std::cin
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();

	// std::wcout, std::wclog, std::wcerr, std::wcin
	HANDLE hConOut = CreateFile(L"CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hConIn = CreateFile(L"CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	SetStdHandle(STD_INPUT_HANDLE, hConIn);
	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();
	std::wcin.clear();

}

char* getExePath()
{
	char* path = new char[MAX_PATH] {0};
	HMODULE hm = GetModuleHandle(NULL);
	if (!::GetModuleFileNameA(hm, path, MAX_PATH) || !::PathRemoveFileSpecA(path))
	{
		delete[] path;
		path = NULL;
		return NULL;
	}
	return path;
}

int CmdRunType()
{
	std::cout << G_consoleTxt;
	int count = 0;
	std::string path;
	std::string sources;
	while (true) {
		std::cout << G_TipsSymbol;
		std::getline(std::cin, path);
		if (path.empty()) {
			if (++count == 1) {
				break;
			}
		}
		else {
			count = 0;
			sources.append(std::move(path + "\n"));
		}
	}

	std::string input = sources;
	if (input.empty()) {
		input = "return;";
	}

	AST* astTree = nullptr;

	try {
		G_mainFile = "^console.sc";
		auto lexical = CerLexical<char>::create(input, "");
		auto parser = CerParser::create(lexical);
		astTree = parser->parser();

		CerInterpreter::ByteCodes codeVec;

		auto_c ret(false, false), seman_ret(false, false), vm_ret(false, false);

		auto seman = SemanticAnalyzer::create(astTree);
		seman->scanner(&seman_ret);
		std::map<std::string, int> import_file;
		auto interpr = CerInterpreter::create(astTree,G_mainFile);
		codeVec = std::move(interpr->CompileCode(&ret, nullptr));

		auto codeVm = CerVm::create(std::move(codeVec));
		codeVm->setCodeFile(G_mainFile);
		codeVm->runTime();

		delete astTree;
		delete lexical;
		delete parser;
		delete seman;
		delete interpr;
		delete codeVm;

	}
	catch (std::string str) {
		if (astTree != nullptr) {
			delete astTree;
		}

		auto errinst = ErrorHandling::getInstance();
		auto debug_list = errinst->getErrorList();
		std::cout << "Unexpected exit for the following reasons : \n" << std::endl;
		std::string space = " ";
		for (auto& debug_info : debug_list)
		{
			auto info = debug_info.second;
			printf("%s[error] in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n\n",
				space.c_str(), info.file.c_str(), info.column, info.row, info.character.c_str());
			space += " ";
		}
		std::cout << str << std::endl;
	}
	catch (const char* cstr) {
		if (astTree != nullptr) {
			delete astTree;
		}

		std::string str{ cstr };
		auto errinst = ErrorHandling::getInstance();
		auto debug_list = errinst->getErrorList();
		std::cout << "Unexpected exit for the following reasons : \n" << std::endl;
		std::string space = " ";
		for (auto& debug_info : debug_list)
		{
			auto info = debug_info.second;
			printf("%s[error] in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n\n",
				space.c_str(), info.file.c_str(), info.column, info.row, info.character.c_str());
			space += " ";
		}
		std::cout << str << std::endl;
	}
	catch (...) {
		if (astTree != nullptr) {
			delete astTree;
		}

		auto errinst = ErrorHandling::getInstance();
		auto debug_list = errinst->getErrorList();
		std::cout << "Unexpected exit for the following reasons : \n" << std::endl;
		std::string space = " ";
		for (auto& debug_info : debug_list)
		{
			auto info = debug_info.second;
			printf("%s[error] in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n\n",
				space.c_str(), info.file.c_str(), info.column, info.row, info.character.c_str());
			space += " ";
		}

		std::cout << "Crash : unknow error, please check C++ codes." << std::endl;
	}

	return 0;
}