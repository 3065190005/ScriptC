#pragma once
#ifndef _SCRIPTC_CERRUNTIME_
#define _SCRIPTC_CERRUNTIME_
#include <map>
#include <vector>
#include <string>

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;

		class CerRunTime
		{
		public:
			struct FuncDescription
			{
				bool hasExport = false;
				size_t addRess{0};
				size_t paramLen{0};
				std::vector<std::string> params;
				std::string fileName;
			};

			struct InterProperties {
				std::string name{};
				std::string parent{};
				std::map<std::string, auto_c> var_people;
				std::vector<std::string> func_name;
			};

			CerRunTime();
			~CerRunTime();

			std::map<std::string, auto_c>* getVarMap();
			void setVarMapValue(std::string name, auto_c element);
			void removeVarMapValue(std::string name);
			auto_c getVarMapValue(std::string name);
			autoPtr getVarMapValuePtr(std::string name);

			bool findVarMap(std::string name);

			void setFuncMapValue(std::string, FuncDescription);
			FuncDescription getFuncMapValue(std::string);
			bool findFuncMap(std::string);

			void setInterMapValue(std::string, InterProperties);
			InterProperties getInterMapValue(std::string);
			std::string getInterMapParent(std::string);
			bool findInterMap(std::string name);

			void insetInterVal(std::string cls, std::string name , auto_c var_default);
			void insetInterFunc(std::string name, std::string func);

		private:
			auto_c getParentValue(std::string name);

		private:
			std::map<std::string, auto_c> m_var_map;			// 变量映射表
			std::map<std::string, FuncDescription> m_func_map;	// 函数地址表
			std::map<std::string, InterProperties> m_inter_map;	// 接口属性表
		};

	}
}
#endif

