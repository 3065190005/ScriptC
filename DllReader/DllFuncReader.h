#pragma once
#ifndef _SCRIPTC_DLL_DLLFUNCREADER_
#define _SCRIPTC_DLL_DLLFUNCREADER_

#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "../AutoElement/LetManager.h"
#include "../AutoElement/LetObject.h"

namespace ScriptC {
	namespace Obj {
		
		using namespace AutoMem::Obj;

		using funcPtr = void (*)(void*, void*);

		class DllFuncReader
		{
		public:
			static DllFuncReader* getInstance();
		private:
			DllFuncReader() {};
			~DllFuncReader() {};

		private:
			static DllFuncReader* m_instance; // 单例

		public:
			static int m_hasInit;		// 是否初始化
			static std::string m_Libs;	// dlls所在目录
			static std::unordered_map<std::string, HINSTANCE> m_GlobalHandle;	// dll文件名和对应实例指针
			static std::unordered_map<std::string, HINSTANCE> m_FuncTables;		// 函数名和对应函数所在dll实例指针


		protected:
			bool loadLib();				// 加载所有dll
			void releasedLib();			// 卸载dll

		public:
			funcPtr getFuncFromDll(std::string _func_name);							// 从dll获取函数指针
			void callFunc(funcPtr func, std::vector<auto_c>* param, auto_c* ret);	// 调用函数指针

		private:
			// single gc
			class dllGc {
			public:
				~dllGc();
			};
		};
	}
}

#endif

