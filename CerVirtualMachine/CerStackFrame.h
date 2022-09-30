#pragma once
#ifndef _CERVER_CERSTACKFRAME_
#define _CERVER_CERSTACKFRAME_

#include <list>

namespace Cervice {
	namespace Obj {

		class CerStackFrame;
		class CerCalcStack;
		class CerRunTime;

		class CerStackFrame
		{
		public:
			CerStackFrame(std::string);

			CerCalcStack* getCalcStack();
			std::list<auto_c>* getDataStack();
			CerRunTime* getRunTime();

			std::string getStackFrameName();
			
		private:
			CerCalcStack m_calcStack;					// 操作数栈
			std::list<auto_c> m_data_stack;			// 空间栈
			CerRunTime m_run_time;						// 运行时
			std::string m_stackFrame_name;				// 栈帧名称
		};
	}
}


#endif

