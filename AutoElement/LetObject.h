#pragma once
#ifndef _AUTOMEM_LETOBJECT__
#define _AUTOMEM_LETOBJECT__

#include "letMacro.h"
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <iostream>
#include <filesystem>

namespace AutoMem {
	namespace Obj {

		namespace {
			bool StaticB = true;
			bool DynamicB = false;
		}

		class LetArrayMotion;
		class LetManager;
		class LetObject;

		using longPtr = long long int*;
		using doublePtr = long double*;
		using boolPtr = bool*;
		using nature = int;
		using numberT = long double;

		class BlockArray;

		/*************************
		* LetObject
		* 变量类 用户使用变量原子
		*************************/
		class LetObject
		{
		public: // 类型定义
			enum class ObjT	// 变量类型
			{
				null = 0,
				undef,
				boolean,
				number,
				string,
				array,
			};

			enum class ObjG { // 识别组
				none,
				Static,
				Dynamic
			};

			struct ObjS { // 块标识符
				ObjT type;
				ObjG group;
			};

		public:
			LetObject(LetObject&&);
			LetObject(const LetObject&);

			LetObject(bool voi, bool type);
			LetObject();

			~LetObject();
		private:

		private:
			void create(bool type);
			void released();

			void changeBlock(bool type);
			bool isLess(ObjT, ObjT);
			bool isLessEq(ObjT, ObjT);
			bool isSameAs(ObjT, ObjT);

		public:
			LetObject* operator<<(long long int);
			LetObject* operator<<(int);
			LetObject* operator<<(long double);
			LetObject* operator<<(double);
			LetObject* operator<<(bool);

			LetObject* operator<<(const char*);
			LetObject* operator<<(std::string);

			template<typename T>
			bool operator>>(T& type);

			template<>
			bool operator>><std::string>(std::string& val);

			std::unordered_map<numberT, BlockArray> getNumArray();
			std::unordered_map<std::string, BlockArray> getStrArray();
			std::unordered_map<numberT, BlockArray> * getNumArrayPtr();
			std::unordered_map<std::string, BlockArray>* getStrArrayPtr();
			std::vector<LetObject> getIndexs();

		public:

			// 功能运算符
			LetObject* operator=(LetObject*);
			LetObject& operator[](LetObject*);

			// 单目运算符
			LetObject operator!();
			LetObject operator~();

			// 算数运算符
			LetObject operator+(LetObject*);
			LetObject operator-(LetObject*);
			LetObject operator*(LetObject*);
			LetObject operator/(LetObject*);
			LetObject operator%(LetObject*);

			// 按位于运算符
			LetObject operator<<(LetObject*);
			LetObject operator>>(LetObject*);


			// 比较运算符
			LetObject operator>(LetObject*);
			LetObject operator<(LetObject*);
			LetObject operator>=(LetObject*);
			LetObject operator<=(LetObject*);
			LetObject operator!=(LetObject*);
			LetObject operator==(LetObject*);

			// 逻辑运算符
			LetObject operator&(LetObject*);
			LetObject operator^(LetObject*);
			LetObject operator|(LetObject*);
			LetObject operator&&(LetObject*);
			LetObject operator||(LetObject*);

		public:
			LetObject* operator=(LetObject&&);

			LetObject* operator=(LetObject&);

			LetObject& operator[](LetObject&&);

			LetObject& operator[](LetObject&);

			LetObject& operator[](const char* cstr);

			template<typename T>
			LetObject& operator[](T val);

			template<>
			LetObject& operator[]<std::string>(std::string val);

			LetObject operator+(LetObject&);
			LetObject operator+(LetObject&&);

			LetObject operator-(LetObject&);
			LetObject operator-(LetObject&&);

			LetObject operator*(LetObject&);
			LetObject operator*(LetObject&&);

			LetObject operator/(LetObject&);
			LetObject operator/(LetObject&&);

			LetObject operator%(LetObject&);
			LetObject operator%(LetObject&&);

			LetObject operator<<(LetObject&);
			LetObject operator<<(LetObject&&);

			LetObject operator>>(LetObject&);
			LetObject operator>>(LetObject&&);

			LetObject operator>(LetObject&);
			LetObject operator>(LetObject&&);

			LetObject operator<(LetObject&);
			LetObject operator<(LetObject&&);

			LetObject operator>=(LetObject&);
			LetObject operator>=(LetObject&&);

			LetObject operator<=(LetObject&);
			LetObject operator<=(LetObject&&);

			LetObject operator!=(LetObject&);
			LetObject operator!=(LetObject&&);

			LetObject operator==(LetObject&);
			LetObject operator==(LetObject&&);

			LetObject operator&(LetObject&);
			LetObject operator&(LetObject&&);

			LetObject operator^(LetObject&);
			LetObject operator^(LetObject&&);

			LetObject operator|(LetObject&);
			LetObject operator|(LetObject&&);

			LetObject operator&&(LetObject&);
			LetObject operator&&(LetObject&&);

			LetObject operator||(LetObject&);
			LetObject operator||(LetObject&&);

		public:
			template<typename castT>
			static castT cast(LetObject&);

			template<typename castT>
			static castT cast(LetObject*);

			static bool reference(LetObject* src, LetObject* des);

			template<>
			static std::string cast<std::string>(LetObject&);

			template<>
			static std::string cast<std::string>(LetObject*);

			void swap(LetObject&);

			bool isRef(LetObject* value);

			char* block();
			size_t lens();

		public:

			ObjT getType();
			ObjG getGroup();
			ObjS getStruct();

			nature getAttribute();
			void setAttribute(nature);
			nature getSelfAttribute();

		private:
			void setType(ObjS typ);

		private:
			Block* m_block = nullptr;					// 内存块
			LetManager* m_letM = nullptr;				// 管理类
			ObjS m_objType = { ObjT::null,ObjG::none };	// 类型

			std::unordered_map<numberT, BlockArray> m_num_array;					// 数字下标
			std::unordered_map<std::string, BlockArray> m_str_array;				// 数组下标

			friend class LetArrayMotion;
		private:
			nature Attribute = 0;

		};

		class BlockArray : public LetObject {
		public: BlockArray() :LetObject(false, false) {}
		};

		using auto_c = LetObject;
		using autoPtr = auto_c*;

		/*************************
		* LetTools
		* 工具类 变量相关工具
		*************************/
		class LetTools
		{
		public:
			LetTools() {};
			~LetTools() {};

			enum class Operator
			{
				None = 0,
				Has,
				Type,
				Group,
				Nature,
				SelfNature,
			};

		public:
			void print(auto_c& value);														// 打印
			void println(auto_c& value);													// 打印带回车
			bool AutoCmp(std::pair<Operator,std::string> condition, auto_c& target);		// 属性对比
			void Swap(auto_c& value1, auto_c& value2);										// 通过move 交换两个数值
			bool IsRef(auto_c& ref_value, auto_c *value);									// 检测 ref_vlaue 是否引用 value
			bool ToBin(auto_c& ref_value, std::string& bin_str, bool has_header = true);	// 编译为16进制
			bool FromBin(auto_c& ref_value, std::string& bin_str, bool has_header = true);	// 16进制转换为变量
			bool SaveToBin(auto_c& ref_value, std::filesystem::path);						// 编译的16进制保存到文件bin中
			bool ReadFromBin(auto_c& ref_value, std::filesystem::path);						// 从文件bin中解析变量

		private:
			bool compileFileHeader(std::string&);				// 编译文件头
			bool compileData(std::string&, auto_c&);			// 编译数据内容
			bool compileDataObjS(std::string&, auto_c&);		// 数据块标识
			bool compileDataAttribute(std::string&, auto_c&);	// 数据块属性
			bool compileDataNumAry(std::string&, auto_c&);		// 数字数组
			bool compileDataStrAry(std::string&, auto_c&);		// 字符数组
			bool compileDataValue(std::string&, auto_c&);		// 变量值

			bool analysisFileHeader(std::string&);				// 解析文件头
			size_t analysisFileSize(std::string&);				// 解析文件大小
			bool analysisData(std::string&, auto_c&);			// 解析文件大小
			bool analysisDataObjS(std::string&, auto_c&);		// 数据块标识
			bool analysisDataAttribute(std::string&, auto_c&);	// 数据块属性
			bool analysisDataNumAry(std::string&, auto_c&);		// 数字数组
			bool analysisDataStrAry(std::string&, auto_c&);		// 字符数组
			bool analysisDataValue(std::string&, auto_c&);		// 获取变量值
		};

		using ToolsCond = std::pair<LetTools::Operator, std::string>;

#include "LetObject.tpp"

	}
}

#endif
