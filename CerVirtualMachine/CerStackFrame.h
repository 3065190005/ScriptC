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
			CerCalcStack m_calcStack;					// ������ջ
			std::list<auto_c> m_data_stack;				// �ռ�ջ
			CerRunTime m_run_time;						// ����ʱ
			std::string m_stackFrame_name;				// ջ֡����
			numberT m_frame_id;							// ջid
		};
	}
}


#endif

