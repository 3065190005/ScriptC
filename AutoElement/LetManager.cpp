#include "LetManager.h"
#include <algorithm>
#include <ctime>

using namespace Cervice::Obj;

LetManager* LetManager::m_instance = nullptr;
std::list<Block*>::iterator LetManager::void_ptr;

/*	**********
 *	构造函数
 *  **********/
LetManager::LetManager()
{
	m_init_success = false;
}

LetManager::~LetManager()
{
	destory();
}

/*	**********
 *	静态函数
 *  **********/
LetManager* LetManager::getInstance()
{
	static Gc gc;
	if (m_instance == nullptr) {
		m_instance = new LetManager();
		if (!m_instance || !m_instance->initialize()) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	return m_instance;
}

/*	**********
 *	共用函数
 *  **********/

void LetManager::realloc_L(Block** node, size_t lens)
{
	OBJ_IF_RETURN(node == nullptr);
	OBJ_IF_RETURN((*node) && (*node)->lens == lens);

	OBJ_IF_CALL(*node, free_L(node));

	*node = malloc_L(lens == 8);
	OBJ_IF_RETURN(lens == 8);

	Block * block = *node;
	size_t selfL = block->lens;

	bool change = false;
	while(true){
		if (selfL >= lens) {
			break;
		}

		if (selfL < 1024) {
			selfL *= 2;
		}
		else {
			selfL += 1024;
		}

		change = true;
	}

	if (change) {
		delete block->ptr;
		block->ptr = new char[selfL + 1]{ 0 };
		block->lens = selfL;
	}
}

Block* LetManager::malloc_L(bool type)
{
	auto& malloc_list = type == true ? m_Malloc_Blocks : m_Malloc_Blocks_D;
	auto& malloc_count = type == true ? m_malloc_count : m_malloc_count_d;

	auto& free_list = type == true ? m_Free_Blocks : m_Free_Blocks_D;
	auto& free_count = type == true ? m_free_count : m_free_count_d;

	if (free_list.empty()) {
		joinMemery();
	}

	Block * ele = *free_list.begin();
	free_list.pop_front();
	free_count--;

	malloc_list.emplace_front(ele);
	malloc_count++;

	ele->iter = malloc_list.begin();
	ele->ref++;

	return ele;
}

void LetManager::free_L(Block** _ptr)
{
	Block* ptr = *_ptr;
	ptr->ref--;

	if (ptr->ref != 0) {
		*_ptr = nullptr; return;
	}

	auto & malloc_list = ptr->lens == 8 ? m_Malloc_Blocks : m_Malloc_Blocks_D;
	auto & malloc_count = ptr->lens == 8 ? m_malloc_count : m_malloc_count_d;

	auto& free_list = ptr->lens == 8 ? m_Free_Blocks : m_Free_Blocks_D;
	auto& free_count = ptr->lens == 8 ? m_free_count : m_free_count_d;

	Ptr cur;
	rPtr end;
	Block* tmp;
	cur = ptr->iter;
	end = malloc_list.rbegin();

	tmp = *end;
	*end = *cur;
	*cur = tmp;

	(*cur)->iter = cur;
	
	Block* ele;
	ele = *malloc_list.rbegin();
	malloc_list.pop_back();
	malloc_count--;

	ele->iter = void_ptr;
	free_list.emplace_back(ele);
	free_count++;
	ele = nullptr;
	*_ptr = nullptr;
	return;
}

/*	**********
 *	内部函数
 *  **********/
bool LetManager::initialize()
{
	size_t blocks_max = MANAGER_BLOCKS_COUNT_MAX;
	size_t static_byte = MANAGER_STATIC_BYTE_SIZE;
	size_t dynamic_byte = MANAGER_DYNAMIC_BYTE_SIZE;

	if (m_init_success) { return true; }

	for (auto i = 0; i < blocks_max; i++) {
		Block* element = new Block{
			new char[static_byte] {0},
			0,
			static_byte,
			false,
			void_ptr
		};

		m_Free_Blocks.push_back(element);
	}

	for (auto i = 0; i < blocks_max; i++) {
		Block* element = new Block{
			new char[dynamic_byte+1] {0},
			0,
			dynamic_byte,
			false,
			void_ptr
		};

		m_Free_Blocks_D.push_back(element);
	}

	m_free_count = blocks_max;
	m_free_count_d = blocks_max;
	m_malloc_count = 0;
	m_malloc_count_d = 0;

	m_init_success = true;
	return true;
}

void LetManager::destory()
{
	std::for_each(m_Free_Blocks.begin(), m_Free_Blocks.end(), [&](Block* element) {
		delete element->ptr;
		element->ptr = nullptr;
		delete element;
		element = nullptr;
	});

	std::for_each(m_Malloc_Blocks.begin(), m_Malloc_Blocks.end(), [&](Block* element) {
		delete element->ptr;
		element->ptr = nullptr;
		delete element;
		element = nullptr;
	});

	std::for_each(m_Free_Blocks_D.begin(), m_Free_Blocks_D.end(), [&](Block* element) {
		delete element->ptr;
		element->ptr = nullptr;
		delete element;
		element = nullptr;
	});

	std::for_each(m_Malloc_Blocks_D.begin(), m_Malloc_Blocks_D.end(), [&](Block* element) {
		delete element->ptr;
		element->ptr = nullptr;
		delete element;
		element = nullptr;
	});

	m_Free_Blocks.clear();
	m_Malloc_Blocks.clear();
	m_Free_Blocks_D.clear();
	m_Malloc_Blocks_D.clear();

	m_init_success = false;
	return;
}

// Gc回收
LetManager::Gc::~Gc()
{
	if (m_instance && m_instance->m_init_success) {
		delete m_instance;
		m_instance = nullptr;
	}
}

bool Cervice::Obj::LetManager::joinMemery()
{

	size_t blocks_step = MANAGER_MEMERYS_SPACE_STEP;
	size_t static_byte = MANAGER_STATIC_BYTE_SIZE;
	size_t dynamic_byte = MANAGER_DYNAMIC_BYTE_SIZE;

	for (auto i = 0; i < blocks_step; i++) {
		Block* element = new Block{
			new char[static_byte] {0},
			0,
			static_byte,
			false,
			void_ptr
		};

		m_Free_Blocks.push_back(element);
	}

	for (auto i = 0; i < blocks_step; i++) {
		Block* element = new Block{
			new char[dynamic_byte + 1] {0},
			0,
			dynamic_byte,
			false,
			void_ptr
		};

		m_Free_Blocks_D.push_back(element);
	}

	m_free_count += blocks_step;
	m_free_count_d += blocks_step;

	return true;
}