#pragma once
#ifndef _SCRIPTC_ERRORHANDING_
#define _SCRIPTC_ERRORHANDING_

#include <string>
#include <vector>

#include "..\CerLexical\CerLexical.h"

namespace ScriptC {
	namespace Obj {

		class ErrorHandling
		{
		public:
			enum class errorType {
				None = 0,
				Lexical,
				Parser,
				SemanticAnalyzer,
				Interpreter,
				Vm,
				System,
			};

		public:
			static ErrorHandling* getInstance();

		private:
			ErrorHandling();
			~ErrorHandling();

		public:
			void throwErr(ErrorHandling::errorType , std::string);
			void setErrInfo(CerTokClass::DebugInfo info);
			CerTokClass::DebugInfo getErrorInfo();

		private:
			static ErrorHandling* m_errorHandling;
			std::vector<std::string> m_type_str;
			CerTokClass::DebugInfo m_debug;

		private:
			class _errorHGc {
			public:
				_errorHGc();
				~_errorHGc();
			};
		};

		using EType = ErrorHandling::errorType;
	}
}

#endif
