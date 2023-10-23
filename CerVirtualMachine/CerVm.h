#pragma once
#ifndef _SCRIPTC_CERVM_
#define _SCRIPTC_CERVM_

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;

		class CommandCode;
		class CerStackSystem;

		/************************************
		* class CerVm
		* ��������ʵ�֣�������command code
		*************************************/
		
		class CerVm
		{
		public:
			using VectorStr = std::vector<std::string>;

		public:
			CerVm();
			~CerVm();

		public:
			bool initVm();
			void Command();
			void runTime();
		
		private:
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
			void CodeCallFunc(std::string _funcName, std::vector<auto_c> _params, std::string _thisName);
			void CodePopNewSf();

		public:
			void printRunCode();		// ��ӡ��ǰ���� debug
			bool isRunOver();			// �����Ƿ�ִ�����
			bool CmpCodeType(CommandCode::CommandCodeType type1 , CommandCode::CommandCodeType type2);	// �����ֽ��������Ƿ�ƥ��
			bool CmpCurrentType(CommandCode::CommandCodeType type);				// ƥ�䵱ǰ��ִ�е��ֽ����Ƿ�Ϊ type
			VectorStr isCallGc();		// ���ص�ǰջ������ �ӿڱ����� (������this)
			VectorStr isCallGcR();		// ���ص�ǰջ�����ǵ����� �ӿڱ����� (������this)
			bool GcCallBack();			// �Ƿ���ýӿڵ���������
			std::string findClassFunc(std::string className, std::string funcName);// �ݹ��ȡ��ĺ�����

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