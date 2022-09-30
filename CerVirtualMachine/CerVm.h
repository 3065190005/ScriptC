#pragma once
#ifndef _CERVER_CERVM_
#define _CERVER_CERVM_

namespace Cervice {
	namespace Obj {

		class CommandCode;
		class CerStackSystem;

		/************************************
		* class CerVm
		* ��������ʵ�֣�������command code
		*************************************/
		
		class CerVm
		{
		public:
			CerVm();
			~CerVm();

		public:
			bool initVm();
			void Command();
			void runTime();
			auto_c runTime(std::string result);
		
		public:
			void VmInter();
			void VmFunc();
			void VmCall();
			void VmLeave();
			void VmPush();
			void VmJmp();
			void VmPushS();
			void VmPopS();
			void VmLens();
			void VmInc();
			auto_c VmPop();
			auto_c VmAdd();
			auto_c VmSub();
			auto_c VmMul();
			auto_c VmDiv();
			auto_c VmMod();
			auto_c VmNot();
			auto_c VmMat();
			auto_c VmLeftO();
			auto_c VmRightO();
			auto_c VmGtr();
			auto_c VmGeq();
			auto_c VmLss();
			auto_c VmLeq();
			auto_c VmEqu();
			auto_c VmNeq();
			auto_c VmAnd();
			auto_c VmXor();
			auto_c VmOr();
			auto_c VmDAnd();
			auto_c VmDOr();
			auto_c VmUnaryAdd();
			auto_c VmUnarySub();
			auto_c VmPass();

		public:
			bool takeEat(CommandCode::CommandCodeType type);
			void advance();

		public:
			static CerVm* create(std::vector<CommandCode> codes);
			static std::map<std::string, size_t> m_CodeBaseAddress;

			void setBaseAddress(std::string str);
			size_t getBaseAddress();

		private: 
			bool m_isinit;								// �Ƿ��ʼ��
			std::vector<CommandCode> m_command_codes;	// ����code
			CommandCode m_current_cmd_code;				// ��ǰcode
			size_t m_command_codes_index;				// ��һ��code�±�
			CerStackSystem m_stacks;					// ջϵͳ
			std::string m_BaseAddress;
		};
	}
}

#endif