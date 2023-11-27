
#include "LetManager.h"
#include "LetObject.h"
#include <algorithm>
#include <fstream>
#include <sstream>

/*************************
* LetTools
* 工具类 变量相关工具
*************************/

#define BINHEADER 0xE4B883E5A49CE8AF9A

void AutoMem::Obj::LetTools::print(auto_c& value)
{
	if (value.getType() == LetObject::ObjT::array) {
		for (auto& i : value.getStrArray()) {
			if (i.second.getType() == LetObject::ObjT::array) {
				print(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::number) {
				std::cout << LetObject::cast<long double>(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::boolean) {
				std::cout << std::boolalpha << LetObject::cast<bool>(i.second);
			}
			else {
				std::cout << LetObject::cast<std::string>(i.second);
			}
		}

		for (auto& i : value.getNumArray()) {
			if (i.second.getType() == LetObject::ObjT::array) {
				print(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::boolean) {
				std::cout << std::boolalpha << i.first << " " << LetObject::cast<bool>(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::number) {
				std::cout << LetObject::cast<long double>(i.second);
			}
			else {
				std::cout << LetObject::cast<std::string>(i.second);
			}
		}
	}
	else if (value.getType() == LetObject::ObjT::number) {
		std::cout << LetObject::cast<long double>(value);
	}
	else if (value.getType() == LetObject::ObjT::string) {
		std::cout << LetObject::cast<std::string>(value);
	}
	else if (value.getType() == LetObject::ObjT::boolean) {
		std::cout << std::boolalpha << LetObject::cast<bool>(value);
	}
}



void AutoMem::Obj::LetTools::println(auto_c& value)
{
	if (value.getType() == LetObject::ObjT::array) {
		for (auto& i : value.getStrArray()) {
			if (i.second.getType() == LetObject::ObjT::array) {
				print(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::number) {
				std::cout << LetObject::cast<long double>(i.second) << std::endl;
			}
			else if (i.second.getType() == LetObject::ObjT::boolean) {
				std::cout << std::boolalpha << LetObject::cast<bool>(i.second) << std::endl;
			}
			else {
				std::cout << LetObject::cast<std::string>(i.second) << std::endl;
			}
		}

		for (auto& i : value.getNumArray()) {
			if (i.second.getType() == LetObject::ObjT::array) {
				print(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::boolean) {
				std::cout << std::boolalpha << i.first << " " << LetObject::cast<bool>(i.second);
			}
			else if (i.second.getType() == LetObject::ObjT::number) {
				std::cout << LetObject::cast<long double>(i.second) << std::endl;
			}
			else {
				std::cout << LetObject::cast<std::string>(i.second) << std::endl;
			}
		}
	}
	else if (value.getType() == LetObject::ObjT::number) {
		std::cout << LetObject::cast<long double>(value) << std::endl;
	}
	else if (value.getType() == LetObject::ObjT::string) {
		std::cout << LetObject::cast<std::string>(value) << std::endl;
	}
	else if (value.getType() == LetObject::ObjT::boolean) {
		std::cout << std::boolalpha << LetObject::cast<bool>(value) << std::endl;
	}
}

bool AutoMem::Obj::LetTools::AutoCmp(std::pair<Operator, std::string> condition, auto_c& target)
{
	if (condition.second.empty())
		return false;

	std::function<bool()> cond_lamb = [&]() -> bool
		{
			bool return_bool = false;

			switch (condition.first)
			{
			case AutoMem::Obj::LetTools::Operator::Has:
			{
				auto_c value_auto;
				std::string value = condition.second;
				value_auto << value;
				auto letobject_v = target.getIndexs();

				for (auto& i : letobject_v)
				{
					auto_c cmp_value;
					cmp_value = value_auto == i;
					if (LetObject::cast<bool>(cmp_value))
					{
						return_bool = true;
						break;
					}
				}

				return return_bool;
			}
			case AutoMem::Obj::LetTools::Operator::Type:
			{
				std::string type_str = condition.second;

				std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::tolower);

				if (type_str == "null")
					return_bool = target.getType() == LetObject::ObjT::null;
				else if (type_str == "undef")
					return_bool = target.getType() == LetObject::ObjT::undef;
				else if (type_str == "boolean")
					return_bool = target.getType() == LetObject::ObjT::boolean;
				else if (type_str == "number")
					return_bool = target.getType() == LetObject::ObjT::number;
				else if (type_str == "string")
					return_bool = target.getType() == LetObject::ObjT::string;
				else if (type_str == "array")
					return_bool = target.getType() == LetObject::ObjT::array;


				return return_bool;
			}
			case AutoMem::Obj::LetTools::Operator::Group:
			{
				std::string type_str = condition.second;
				std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::tolower);

				if (type_str == "dynamic")
					return_bool = target.getGroup() == LetObject::ObjG::Dynamic;
				else if (type_str == "static")
					return_bool = target.getGroup() == LetObject::ObjG::Static;


				return return_bool;
			}
			case AutoMem::Obj::LetTools::Operator::Nature:
			{
				std::string type_str = condition.second;
				std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::tolower);

				if (type_str == "ptr")
					return_bool = (target.getAttribute() & (int)NatureType::ptr);
				else if (type_str == "cls")
					return_bool = (target.getAttribute() & (int)NatureType::cls);


				return return_bool;
			}
			case AutoMem::Obj::LetTools::Operator::SelfNature:
			{
				std::string type_str = condition.second;
				std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::tolower);

				if (type_str == "ptr")
					return_bool = (target.getSelfAttribute() & (int)NatureType::ptr);
				else if (type_str == "cls")
					return_bool = (target.getSelfAttribute() & (int)NatureType::cls);


				return return_bool;
			}

			case AutoMem::Obj::LetTools::Operator::None:
			default:
				return false;
			}

			return return_bool;
		};

	return cond_lamb();
}

void AutoMem::Obj::LetTools::Swap(auto_c& value1, auto_c& value2)
{
	value1.swap(value2);
}

bool AutoMem::Obj::LetTools::IsRef(auto_c& ref_value, auto_c* value)
{
	return ref_value.isRef(value);
}

bool AutoMem::Obj::LetTools::ToBin(auto_c& ref_value, std::string& bin_str, bool has_header)
{
	bin_str.clear();

	bool ret = true;
	try
	{
		if (has_header)
			ret &= compileFileHeader(bin_str);

		std::string data_str;
		ret &= compileData(data_str, ref_value);

		// 文件长度
		size_t file_size = data_str.size();
		bin_str.append((const char*)(&file_size), sizeof(size_t));
		bin_str.append(data_str);
	}
	catch (std::string str)
	{
		bin_str.clear();
		bin_str = str;
		ret = false;
	}


	return ret;
}

bool AutoMem::Obj::LetTools::FromBin(auto_c& ref_value, std::string& bin_str, bool has_header)
{
	bool ret = true;
	try
	{
		size_t file_size = 0;
		if (has_header)
			ret &= analysisFileHeader(bin_str);
		file_size = analysisFileSize(bin_str);

		auto_c var;
		std::string data;
		data = bin_str.substr(0, file_size);
		bin_str = bin_str.substr(file_size, bin_str.size());
		if (data.size() != file_size)
			throw("File Data Size Error");

		analysisData(data, var);
		ref_value = var;

	}
	catch (std::string str)
	{
		ret = false;;
	}

	return ret;

}

bool AutoMem::Obj::LetTools::SaveToBin(auto_c& ref_value, std::filesystem::path path)
{
	std::string bin_str;
	bool ret = false;
	try
	{
		ret = ToBin(ref_value, bin_str);
	}
	catch (...)
	{
		ret = false;
	}

	if (ret)
	{
		std::fstream fs;
		fs.open(path.c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::in | std::ios_base::trunc);
		if (fs.is_open())
		{
			fs.clear();
			fs.write(bin_str.c_str(), bin_str.size());
		}
		else
		{
			ret = false;
		}
		fs.close();
	}

	return ret;
}

bool AutoMem::Obj::LetTools::ReadFromBin(auto_c& ref_value, std::filesystem::path path)
{
	auto_c bin_var;
	std::ifstream fin(path.c_str()); 
	if (!fin.is_open())
		return false;

	std::stringstream buffer;           
	buffer << fin.rdbuf();       
	std::string bin_str(buffer.str()); 

	bool ret = false;
	try
	{
		ret = FromBin(bin_var, bin_str);
	}
	catch (...)
	{
		ret = false;
	}

	if (ret)
	{
		ref_value = bin_var;
	}

	return ret;
}

std::string AutoMem::Obj::LetTools::ObjTToStr(LetObject::ObjT objt)
{
	std::string ret;
	switch (objt)
	{
	case AutoMem::Obj::LetObject::ObjT::null:
		ret = "null";
		break;
	case AutoMem::Obj::LetObject::ObjT::undef:
		ret = "undef";
		break;
	case AutoMem::Obj::LetObject::ObjT::boolean:
		ret = "boolean";
		break;
	case AutoMem::Obj::LetObject::ObjT::number:
		ret = "number";
		break;
	case AutoMem::Obj::LetObject::ObjT::string:
		ret = "string";
		break;
	case AutoMem::Obj::LetObject::ObjT::array:
		ret = "array";
		break;
	default:
		ret = "";
		break;
	}

	return ret;
}

std::string AutoMem::Obj::LetTools::ObjGToStr(LetObject::ObjG objg)
{
	std::string ret;
	switch (objg)
	{
	case AutoMem::Obj::LetObject::ObjG::none:
		ret = "none";
		break;
	case AutoMem::Obj::LetObject::ObjG::Static:
		ret = "static";
		break;
	case AutoMem::Obj::LetObject::ObjG::Dynamic:
		ret = "dynamic";
		break;
	default:
		ret = "";
		break;
	}

	return ret;
}

AutoMem::Obj::LetObject::ObjT AutoMem::Obj::LetTools::StrToObjT(std::string objt_str)
{
	for (size_t i = 0; i < objt_str.size(); i++)
	{
		if (objt_str[i] >= 'a' && objt_str[i] <= 'z')
		{
			objt_str[i] -= 32;
		}
	}

	AutoMem::Obj::LetObject::ObjT ret{ LetObject::ObjT::null };

	if (objt_str == "NULL")
		ret = LetObject::ObjT::null;
	if (objt_str == "UNDEF")
		ret = LetObject::ObjT::undef;
	if (objt_str == "BOOLEAN" || objt_str == "BOOL")
		ret = LetObject::ObjT::boolean;
	if (objt_str == "NUMBER")
		ret = LetObject::ObjT::number;
	if (objt_str == "STRING" || objt_str == "STR")
		ret = LetObject::ObjT::string;
	if (objt_str == "ARRAY")
		ret = LetObject::ObjT::array;

	return ret;
}

AutoMem::Obj::LetObject::ObjG AutoMem::Obj::LetTools::StrToObjG(std::string objg_str)
{
	for (size_t i = 0; i < objg_str.size(); i++)
	{
		if (objg_str[i] >= 'a' && objg_str[i] <= 'z')
		{
			objg_str[i] -= 32;
		}
	}

	AutoMem::Obj::LetObject::ObjG ret{LetObject::ObjG::none};

	if (objg_str == "NONE")
		ret = LetObject::ObjG::none;
	if (objg_str == "DYNAMIC")
		ret = LetObject::ObjG::Dynamic;
	if (objg_str == "STATIC")
		ret = LetObject::ObjG::Static;

	return ret;
}

bool AutoMem::Obj::LetTools::compileFileHeader(std::string& bin_str)
{
	char header_chars[9]{ 0xE4, 0xB8 ,0x83,0xE5,0xA4,0x9C,0xE8,0xAF,0x9A };
	bin_str.append(header_chars, 9);
	return true;
}

bool AutoMem::Obj::LetTools::compileData(std::string& bin_str, auto_c& ref_value)
{

	compileDataObjS(bin_str, ref_value);
	compileDataAttribute(bin_str, ref_value);
	
	switch (ref_value.getType())
	{
	case LetObject::ObjT::array:
		compileDataNumAry(bin_str, ref_value);
		compileDataStrAry(bin_str, ref_value);
		break;
	default:
		compileDataValue(bin_str, ref_value);
		break;
	}

	return true;
}

bool AutoMem::Obj::LetTools::compileDataObjS(std::string& bin_str, auto_c& ref_value)
{
	// 数据块标识
	auto objs = ref_value.getStruct();
	bin_str.append((const char*)(&objs.type), sizeof(objs.type));
	bin_str.append((const char*)(&objs.group), sizeof(objs.group));
	return true;
}

bool AutoMem::Obj::LetTools::compileDataAttribute(std::string& bin_str, auto_c& ref_value)
{
	// 数据块标识
	auto attribute = ref_value.getAttribute();
	bin_str.append((const char*)(&attribute), sizeof(attribute));
	return true;
}

bool AutoMem::Obj::LetTools::compileDataNumAry(std::string& bin_str, auto_c& ref_value)
{
	// 数组数量
	size_t array_size = ref_value.getNumArrayPtr()->size();
	bin_str.append((const char*)(&array_size), sizeof(size_t));
	
	auto ary_ptr = ref_value.getNumArrayPtr();
	
	for (auto it = ary_ptr->begin(); it != ary_ptr->end(); it++)
	{
		// 数字下表
		bin_str.append((const char*)(&it->first), sizeof(numberT));

		// 数据内容
		compileData(bin_str, it->second);
	}

	return true;
}

bool AutoMem::Obj::LetTools::compileDataStrAry(std::string& bin_str, auto_c& ref_value)
{
	size_t array_size = ref_value.getStrArrayPtr()->size();
	bin_str.append((const char*)(&array_size), sizeof(size_t));

	auto ary_ptr = ref_value.getStrArrayPtr();

	for (auto it = ary_ptr->begin(); it != ary_ptr->end(); it++)
	{
		// 字符长度
		size_t str_len = it->first.length();
		bin_str.append((const char*)(&str_len), sizeof(size_t));

		// 字符值
		bin_str.append(it->first);

		// 数据内容
		compileData(bin_str, it->second);
	}

	return true;
}

bool AutoMem::Obj::LetTools::compileDataValue(std::string& bin_str, auto_c& ref_value)
{
	// 变量长度
	size_t block_lens = ref_value.lens();
	bin_str.append((const char*)(&block_lens), sizeof(size_t));
	// 变量值
	char* block_ptr = ref_value.block();
	bin_str.append(block_ptr, block_lens);
	return true;
}

bool AutoMem::Obj::LetTools::analysisFileHeader(std::string& bin_str)
{
	char header_chars[9]{ 0xE4, 0xB8 ,0x83,0xE5,0xA4,0x9C,0xE8,0xAF,0x9A };
	std::string header = bin_str.substr(0, 9);
	for (auto i = 0; i < 9; i++)
	{
		if (header[i] != header_chars[i])
			throw("Analysis File Header Error");
	}
	
	bin_str = bin_str.substr(9, bin_str.size());
	return true;
}

size_t AutoMem::Obj::LetTools::analysisFileSize(std::string& bin_str)
{
	std::string size = bin_str.substr(0, sizeof(size_t));
	if (size.size() != sizeof(size_t))
		throw("Analysis File Size Error");
	bin_str = bin_str.substr(sizeof(size_t), bin_str.size());

	size_t* size_ptr = (size_t*)size.c_str();
	return *size_ptr;
}

bool AutoMem::Obj::LetTools::analysisData(std::string& bin_str, auto_c& ref_value)
{
	analysisDataObjS(bin_str, ref_value);
	analysisDataAttribute(bin_str, ref_value);

	switch (ref_value.getType())
	{
	case LetObject::ObjT::array:
		analysisDataNumAry(bin_str, ref_value);
		analysisDataStrAry(bin_str, ref_value);
		break;
	default:
		analysisDataValue(bin_str, ref_value);
		break;
	}

	return true;
}

bool AutoMem::Obj::LetTools::analysisDataObjS(std::string& bin_str, auto_c& ref_value)
{
	// 数据块标识
	LetObject::ObjS objs;
	
	std::string objt_str = bin_str.substr(0, sizeof(objs.type));
	objs.type = *(LetObject::ObjT*)objt_str.c_str();
	bin_str = bin_str.substr(sizeof(objs.type), bin_str.size());

	std::string objg_str = bin_str.substr(0, sizeof(objs.group));
	objs.group = *(LetObject::ObjG*)objg_str.c_str();
	bin_str = bin_str.substr(sizeof(objs.group), bin_str.size());

	if (objs.group == LetObject::ObjG::Dynamic)
	{
		ref_value << "";
	}
	else
	{
		switch (objs.type)
		{
		case LetObject::ObjT::null:
			ref_value = auto_c();
			break;
		case LetObject::ObjT::undef:
			ref_value = auto_c(true, true);
			break;
		case LetObject::ObjT::boolean:
			ref_value << false;
			break;
		case LetObject::ObjT::number:
			ref_value << numberT{ 0 };
			break;
		case LetObject::ObjT::array:
			ref_value[0] << numberT{ 0 };
			ref_value.getNumArrayPtr()->clear();
			break;
		default:
			break;
		}
	}

	return true;
}

bool AutoMem::Obj::LetTools::analysisDataAttribute(std::string& bin_str, auto_c& ref_value)
{
	std::string attribute_str = bin_str.substr(0, sizeof(nature));
	nature attribute = *(nature*)attribute_str.c_str();
	bin_str = bin_str.substr(sizeof(nature), bin_str.size());
	ref_value.setAttribute(attribute);
	return true;
}

bool AutoMem::Obj::LetTools::analysisDataNumAry(std::string& bin_str, auto_c& ref_value)
{
	// 数组数量
	std::string array_size_str = bin_str.substr(0, sizeof(size_t));
	size_t array_size = *(size_t*)array_size_str.c_str();
	bin_str = bin_str.substr(sizeof(size_t), bin_str.size());

	auto ary_ptr = ref_value.getNumArrayPtr();

	for (size_t i = 0 ; i < array_size ; i ++)
	{
		// 数字下表
		std::string index_str = bin_str.substr(0, sizeof(numberT));
		numberT index_number = *(numberT*)index_str.c_str();
		bin_str = bin_str.substr(sizeof(numberT), bin_str.size());

		// 数据内容
		auto_c param;
		analysisData(bin_str, param);

		ref_value[index_number] = param;
	}

	return true;
}

bool AutoMem::Obj::LetTools::analysisDataStrAry(std::string& bin_str, auto_c& ref_value)
{
	std::string array_size_str = bin_str.substr(0, sizeof(size_t));
	size_t array_size = *(size_t*)array_size_str.c_str();
	bin_str = bin_str.substr(sizeof(size_t), bin_str.size());

	for (size_t i = 0 ; i < array_size ; i ++)
	{
		// 字符长度
		std::string str_len_c = bin_str.substr(0, sizeof(size_t));
		size_t str_len = *(size_t*)str_len_c.c_str();
		bin_str = bin_str.substr(sizeof(size_t), bin_str.size());

		// 字符值
		std::string str_index = bin_str.substr(0, str_len);
		bin_str = bin_str.substr(str_len, bin_str.size());
		

		// 数据内容
		auto_c param;
		analysisData(bin_str, param);

		ref_value[str_index] = param;
	}

	return true;
}

bool AutoMem::Obj::LetTools::analysisDataValue(std::string& bin_str, auto_c& ref_value)
{
	// 变量长度
	std::string block_lens = bin_str.substr(0, sizeof(size_t));
	size_t lens = *(size_t*)block_lens.c_str();
	bin_str = bin_str.substr(sizeof(size_t), bin_str.size());

	// 变量值
	std::string block_value = bin_str.substr(0, lens);
	bin_str = bin_str.substr(lens, bin_str.size());


	if (ref_value.getGroup() == LetObject::ObjG::Dynamic)
	{
		ref_value << block_value;
	}
	else
	{
		if (ref_value.getType() == LetObject::ObjT::null)
			return true;
		else
			memcpy(ref_value.block(), block_value.c_str(), lens);
	}

	return true;
}
