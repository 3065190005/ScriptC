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

			using DebugElement = std::pair<std::string, CerTokClass::DebugInfo>;
			using DebugFrame = std::map<std::string, CerTokClass::DebugInfo>;

		private:
			ErrorHandling();
			~ErrorHandling();

		public:
			void throwErr(ErrorHandling::errorType , std::string);
			void setErrInfo(CerTokClass::DebugInfo info);
			CerTokClass::DebugInfo getErrorInfo();
			DebugFrame getErrorList();

			std::vector<DebugElement> sortDebugFrame();
			void updateDebugFrame(CerTokClass::DebugInfo info);
			bool removeUntileClock(size_t id, std::vector<DebugElement>);

		private:
			static ErrorHandling* m_errorHandling;
			std::vector<std::string> m_type_str;
			CerTokClass::DebugInfo m_debug;
			DebugFrame m_debug_frame;

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
