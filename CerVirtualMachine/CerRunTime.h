#pragma once
#ifndef _CERVER_CERRUNTIME_
#define _CERVER_CERRUNTIME_
#include <map>
#include <vector>
#include <string>

namespace Cervice {
	namespace Obj {

		class CerRunTime
		{
		public:
			struct FuncDescription
			{
				bool hasExport = false;
				size_t addRess;
				size_t paramLen;
				std::vector<std::string> params;
			};

			struct InterProperties {
				std::string name;
				std::string parent;
				std::map<std::string, auto_c> var_people;
				std::vector<std::string> func_name;
			};

			CerRunTime();
			~CerRunTime();

			std::map<std::string, auto_c>* getVarMap();
			void setVarMapValue(std::string name, auto_c element);
			auto_c getVarMapValue(std::string name);
			autoPtr getVarMapValuePtr(std::string name);

			bool findVarMap(std::string name);

			void setFuncMapValue(std::string, FuncDescription);
			FuncDescription getFuncMapValue(std::string);
			bool findFuncMap(std::string);

			void setInterMapValue(std::string, InterProperties);
			InterProperties getInterMapValue(std::string);
			bool findInterMap(std::string name);

			void insetInterVal(std::string cls, std::string name , auto_c var_default);
			void insetInterFunc(std::string name, std::string func);

		private:
			std::map<std::string, auto_c> m_var_map;			// 变量映射表
			std::map<std::string, FuncDescription> m_func_map;	// 函数地址表
			std::map<std::string, InterProperties> m_inter_map;	// 接口属性表
		};

	}
}
#endif

