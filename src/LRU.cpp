//
// Created by 17305 on 2025/12/23.
//

// 输入
// ["LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"]
// [[2], [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]
// 输出
// [null, null, null, 1, null, -1, null, -1, 3, 4]
//
// 解释
// LRUCache lRUCache = new LRUCache(2);
// lRUCache.put(1, 1); // 缓存是 {1=1}
// lRUCache.put(2, 2); // 缓存是 {1=1, 2=2}
// lRUCache.get(1);    // 返回 1
// lRUCache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
// lRUCache.get(2);    // 返回 -1 (未找到)
// lRUCache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
// lRUCache.get(1);    // 返回 -1 (未找到)
// lRUCache.get(3);    // 返回 3
// lRUCache.get(4);    // 返回 4

#include <list>
#include <unordered_map>

#include "doctest/doctest.h"

// https://leetcode.cn/problems/lru-cache/solutions/2456294/tu-jie-yi-zhang-tu-miao-dong-lrupythonja-czgt/

// 最近最少使用策略
class LRUCache
{
private:
	int m_capacity;
	std::list<std::pair<int, int>> m_list;
	std::unordered_map<int, std::list<std::pair<int, int>>::iterator> m_umap;

public:
	explicit LRUCache(int capacity) : m_capacity(capacity) {}

	int get(int key)
	{
		// 在哈希表中查找key对应的迭代器
		auto val = m_umap.find(key);
		if (val == m_umap.end()) {
			return -1;  // 键不存在，返回-1
		}

		// 将访问的节点移动到链表头部（表示最近使用）
		m_list.splice(m_list.begin(), m_list, val->second);
		return val->second->second;  // 返回对应值
	}

	void put(int key, int value)
	{
		auto iter = m_umap.find(key);
		if (iter != m_umap.end()) {
			// 键已存在，更新值并将节点移到链表头部
			auto _iter = iter->second;
			_iter->second = value;  // 更新节点中的值
			m_list.splice(m_list.begin(), m_list, _iter); // 移动到链表最前面（最近使用）
			return;
		}

		// 键不存在，插入新节点到链表头部
		m_list.emplace_front(key, value);  // 在链表头部插入新键值对
		m_umap[key] = m_list.begin();      // 在哈希表中记录key到新节点的映射

		// 如果超出容量，删除最久未使用的节点（链表尾部）
		if (m_capacity < m_list.size()) {
			m_umap.erase(m_list.back().first);  // 从哈希表中删除对应的键
			m_list.pop_back();                  // 从链表中删除尾部节点
		}
	}
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */


TEST_CASE("LRU Cache") {}
