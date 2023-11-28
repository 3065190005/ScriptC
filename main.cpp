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
R"(ScriptC （Last Update : 2023.11.22 | LetObject : v9_7) [console mode]
If you want to compile and run the code, type Enter twice.

)";

using namespace ScriptC::Obj;

// 主源文件代码
std::string G_SourcesFile;

// 主源文件名
std::string G_mainFile;

// 库默认路径
std::string G_Res_path = ".\\res\\";


// 开发调试 debug
#ifdef _DEBUG
void TestCodeCall();
#endif // NDEBUG && _DEBUG

// 开发调试 released
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
		auto errHis = ErrorHandling::getInstance();
		auto info = errHis->getErrorInfo();
		printf("in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n", info.file.c_str(), info.column, info.row, info.character.c_str());
		std::cout << str << std::endl;
	}
	catch (const char* cstr) {
		std::string str{ cstr };
		auto errHis = ErrorHandling::getInstance();
		auto info = errHis->getErrorInfo();
		printf("in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n", info.file.c_str(), info.column, info.row, info.character.c_str());
		std::cout << str << std::endl;
	}
	catch (...) {

		auto errHis = ErrorHandling::getInstance();
		auto info = errHis->getErrorInfo();
		printf("in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n", info.file.c_str(), info.column, info.row, info.character.c_str());
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
	require ("io");
	function log(val):
		let io = new StdIo;
		io.println(val);
	end

	require ("time");
	let tm = new StdTime;
	
	log(tm.time());
	log(tm.timeAsM());
	log(tm.clock());
	log(tm.clockAsM());
	log(tm.cast("120",tm.min));
	log(tm.castAsM(120000,tm.sec));
	let schedule = tm.join();
	log(schedule);
	log(tm.over(schedule));
	log(tm.overAsM(schedule));
	log(tm.date());
	log(tm.dateAsM());
	log(tm.toDate(1664374474));
	log(tm.toDateAsM(1664374474789));

	return 0;
)";


	std::cout << "Test Code ： \n";
	AST* astTree = nullptr;
	auto ta = true;
	auto fa = false;

	auto_c param1,param2;

					// lexic, paser, seman, inter, vmret, print
	bool control[] = { ta,		ta,	   ta,	  ta,	ta,	   fa };
	G_mainFile = "sources.sc";
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
			std::cout << "can't open and read file with “ " << G_mainFile << " ”" << std::endl;
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

		auto errHis = ErrorHandling::getInstance();
		auto info = errHis->getErrorInfo();
		printf("in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n", info.file.c_str(), info.column, info.row, info.character.c_str());
		std::cout << str << std::endl;
	}
	catch (...) {
		if (astTree != nullptr) {
			delete astTree;
		}

		auto errHis = ErrorHandling::getInstance();
		auto info = errHis->getErrorInfo();
		printf("in file \"%s\" column \"%d\" row \"%d\" : \"%s\"\n", info.file.c_str(), info.column, info.row, info.character.c_str());
		std::cout << "Crash : unknow error, please check C++ codes." << std::endl;
	}

	return 0;
}