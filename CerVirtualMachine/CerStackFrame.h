#pragma once
#ifndef _SCRIPTC_CERSTACKFRAME_
#define _SCRIPTC_CERSTACKFRAME_

#include <list>

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;

		class CerStackFrame;
		class CerCalcStack;
		class CerRunTime;

		class CerStackFrame
		{
		public:
			CerStackFrame(std::string,numberT);

			CerCalcStack* getCalcStack();
			std::list<auto_c>* getDataStack();
			CerRunTime* getRunTime();

			std::string getStackFrameName();
			numberT getStackFrameId();
			
		private:
			CerCalcStack m_calcStack;					// 操作数栈
			std::list<auto_c> m_data_stack;				// 空间栈
			CerRunTime m_run_time;						// 运行时
			std::string m_stackFrame_name;				// 栈帧名称
			numberT m_frame_id;							// 栈id
		};
	}
}


#endif

