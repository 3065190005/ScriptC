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

using namespace Cervice::Obj;

// 静态动态内存大小
size_t G_memerySetNumber = 0;

// 主源文件代码
std::string G_SourcesFile;

// 命令行是否已使用内存设置
bool G_memerySetParam = false;

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

struct CerTokClass::DebugInfo G_Global_Debug_Info;

int main(int args , char** argv) {
	if (args == 1) {
		DllFuncReader* manager = DllFuncReader::getInstance();
		auto func = manager->getFuncFromDll("__system___setArgv__");
		manager->callFunc(func, (std::vector<auto_c>*)argv, (auto_c*)&args);

		return CmdRunType();
	}

	if (getCmdParam(args, argv) != 0) {
		return 0;
	}

	HWND hwnd = GetForegroundWindow();
	if (hwnd) {
		ShowWindow(hwnd, SW_SHOW);
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

	return 0;
}
#ifdef _PACKAGE
void ScriptRun() {
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

		std::vector<CommandCode> codeVec;
		auto_c ret(false, false), seman_ret(false, false), vm_ret(false, false);
		auto seman = SemanticAnalyzer::create(astTree);
		auto interpr = CerInterpreter::create(astTree);
		seman->scanner(&seman_ret);
		codeVec = std::move(interpr->CompileCode(&ret, nullptr, true));
		
		for (auto& i : interpr->getTables()) {
			if (i.first.find("%::") != i.first.npos) {
				std::string file;
				file = i.first.substr(3);
				CerVm::m_CodeBaseAddress.insert({ file + ".sc",i.second + 1 });
			}
		}

		delete seman;
		delete interpr;

		auto codeVm = CerVm::create(std::move(codeVec));
		codeVm->m_CodeBaseAddress[G_mainFile] = 1;
		codeVm->setBaseAddress(G_mainFile);

		codeVm->runTime();
		delete astTree;
	}
	catch (std::string str) {
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


	std::vector<CommandCode> codeVec;
	auto_c ret(false, false), seman_ret(false, false), vm_ret(false, false);
	auto seman = SemanticAnalyzer::create(astTree);
	auto interpr = CerInterpreter::create(astTree);

	seman->scanner(&seman_ret);
	codeVec = std::move(interpr->CompileCode(&ret, nullptr, true));
	for (auto& i : interpr->getTables()) {
		if (i.first.find("%::") != i.first.npos) {
			std::string file;
			file = i.first.substr(3);
			CerVm::m_CodeBaseAddress.insert({ file + ".sc",i.second + 1 });
		}
	};

	delete seman;
	delete interpr;

	auto codeVm = CerVm::create(std::move(codeVec));
	codeVm->m_CodeBaseAddress[G_mainFile] = 1;
	codeVm->setBaseAddress(G_mainFile);

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
	std::string input = R"(
	include "os";
	include "io";
	include "window";

	let os = new StdOs;
	let io = new StdIo;
	let window = new StdWindow;
	
	window.hideControl();
	let ret = window.msgBox("Title","txt",1);
	io.print(ret);

	ret = window.editBox("Title","txt");
	io.print(ret);

	ret = window.bowserBox();
	io.print(ret);

	ret = window.htmlBox("title1","<h>123</h>");
	io.print(ret);

	ret = window.urlBox("title2","www.baidu.com");
	io.print(ret);

	window.showControl();
	os.system("pause");
	return;
	
	// ---
)";

	
	std::cout << "Test Code : " << std::endl;
	AST* astTree = nullptr;
	auto ta = true;
	auto fa = false;

	auto_c param1,param2;

					// lexic, paser, seman, inter, vmret, print
	bool control[] = { ta,		ta,	   ta,	  ta,	ta,	   ta };
	G_mainFile = "sources.sc";
	if (control[0]) {
		auto lexical = CerLexical<char>::create(input, "sources");
		if (control[1]) {
			auto parser = CerParser::create(lexical);
			astTree = parser->parser();

			std::vector<CommandCode> codeVec;

			auto_c ret(false, false), seman_ret(false, false), vm_ret(false, false);

			if (control[2]) {
				auto seman = SemanticAnalyzer::create(astTree);
				seman->scanner(&seman_ret);

				if (control[3]) {
					std::map<std::string, int> import_file;
					auto interpr = CerInterpreter::create(astTree);
					codeVec = std::move(interpr->CompileCode(&ret, nullptr, true));

					for (auto &i : interpr->getTables()) {
						if (i.first.find("%::") != i.first.npos) {
							std::string file;
							file = i.first.substr(3);
							CerVm::m_CodeBaseAddress.insert({ file+".sc",i.second+1 });
						}
					};

					if (control[4]) {
						auto codeVm = CerVm::create(std::move(codeVec));
						codeVm->m_CodeBaseAddress[G_mainFile] = 1;
						codeVm->setBaseAddress(G_mainFile);

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

std::string G_TipsSymbol = "->: ";
std::string G_consoleTxt = 
R"(ScriptC （Last Update : 2022.9.30 | LetObject : v9_1) [console mode]
If you want to compile and run the code, type Enter twice.

)";

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

		std::vector<CommandCode> codeVec;

		auto_c ret(false, false), seman_ret(false, false), vm_ret(false, false);

		auto seman = SemanticAnalyzer::create(astTree);
		seman->scanner(&seman_ret);
		std::map<std::string, int> import_file;
		auto interpr = CerInterpreter::create(astTree);
		codeVec = std::move(interpr->CompileCode(&ret, nullptr, true));
		for (auto& i : interpr->getTables()) {
			if (i.first.find("%::") != i.first.npos) {
				std::string file;
				file = i.first.substr(3);
				CerVm::m_CodeBaseAddress.insert({ file + ".sc",i.second + 1 });
			}
		};

		auto codeVm = CerVm::create(std::move(codeVec));
		codeVm->m_CodeBaseAddress[G_mainFile] = 1;
		codeVm->setBaseAddress(G_mainFile);
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