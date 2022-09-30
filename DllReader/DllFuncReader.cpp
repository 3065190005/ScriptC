#include "DllFuncReader.h"
namespace Cervice {
	namespace Obj {

        DllFuncReader* DllFuncReader::m_instance = nullptr;
		int DllFuncReader::m_hasInit = 0;
		std::string DllFuncReader::m_Libs = ".\\libs\\";
		std::unordered_map<std::string, HINSTANCE> DllFuncReader::m_GlobalHandle;
		std::unordered_map<std::string, HINSTANCE> DllFuncReader::m_FuncTables;

        bool DllFuncReader::loadLib() {
            if (m_hasInit) return true;
            m_hasInit = true;

            std::string line;
            std::ifstream file(m_Libs + "config.cfg");
            if (!file.is_open()) {
                throw("file is not exits");
            }

            while (std::getline(file, line)) {
                HINSTANCE hdll;
                hdll = LoadLibraryA((m_Libs + line).c_str());

                if (hdll == nullptr) {
                    throw("handle " + line + " is not exits");
                }

                m_GlobalHandle[line] = hdll;

            }

            file.close();

            return true;
        }

        void DllFuncReader::releasedLib() {
            if (!m_hasInit) {
                return;
            }

            m_hasInit = false;

            for (auto& i : m_GlobalHandle) {
                FreeLibrary(i.second);
            }

            m_GlobalHandle.clear();
            m_FuncTables.clear();
        }

        funcPtr DllFuncReader::getFuncFromDll(std::string _func_name) {
            auto tables = m_FuncTables.find(_func_name);
            if (tables != m_FuncTables.end()) {
                return (funcPtr)GetProcAddress(tables->second, _func_name.c_str());
            }

            for (auto& i : m_GlobalHandle) {
                funcPtr ret = (funcPtr)GetProcAddress(i.second, _func_name.c_str());
                if (ret != nullptr) {
                    m_FuncTables[_func_name] = i.second;
                    return ret;
                }
            }

            return nullptr;
        }

        void DllFuncReader::callFunc(funcPtr func, std::vector<auto_c>* param, auto_c* ret) {
            func((void*)param, (void*)ret);
        }

        DllFuncReader* DllFuncReader::getInstance()
        {
            if (!m_instance) {
                static dllGc gc;
                m_instance = new DllFuncReader();
                m_instance->loadLib();
            }

            return m_instance;
        }

        DllFuncReader::dllGc::~dllGc() {
            if (m_instance) {
                m_instance->releasedLib();
                delete m_instance;
                m_instance = nullptr;
            }
        }
	}
}