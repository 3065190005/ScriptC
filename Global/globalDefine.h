#pragma once
#ifndef _SCRIPTC_GLOBAL_DEFINE_
#define _SCRIPTC_GLOBAL_DEFINE_

#define SPECIAL_FUNC_ATTR "_attr"	// Attr ���⺯����
#define SPECIAL_FUNC_GC "_gc"		// Gc ���⺯����
#define SPECIAL_FUNC_INIT "_init"	// Init ���⺯����
#define CLASS_NAME "_class_name"			// �ӿڱ�����Ĭ�Ͻӿڱ�����
#define CLASS_PARENT "_class_parent"		// �ӿڱ�����Ĭ�ϸ��ӿڱ�����
#define CLASS_AUTO_GC_CALL "_class_auto_gc_call"	// ���⺯��Gc�Ƿ������ñ�����
#define REQUIRE_FILE_PREFIX "%::"		// ������ require �����ļ����ļ�����ǰ׺����ͨ����������
#define SYSTEM_FILE_NAME "__FILE_NAME__" // ������� FILE_NAME �ı�����
#define SYSTEM_MAIN_NAME "__MAIN_NAME__" // ������� MAIN_NAME �ı�����
#define DEBUG_TRY_CATCH false	// debugģʽ�Ƿ����try�� catch

#ifdef _DEBUG
#define GlobalDebugOpend true
#else
#define GlobalDebugOpend false
#endif

#include <map>



// ȫ�ֱ�������
namespace ScriptC {
	namespace Obj {
		class GlobalManager
		{
		public:
			static size_t global_debug_depth;
		};

	}
}

#endif