#pragma once
#ifndef _CERVER_ERRORHANDING_
#define _CERVER_ERRORHANDING_

#include <string>
#include <vector>

#include "..\CerLexical\CerLexical.h"

namespace Cervice {
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
