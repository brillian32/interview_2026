//
// Created by 17305 on 2025/12/18.
//

#include <doctest/doctest.h>
#include <iostream>

/**
 * Definition for singly-linked list.
 */
struct ListNode
{
	int val;
	ListNode *next;
	ListNode() : val(0), next(nullptr) {}
	explicit ListNode(const int x) : val(x), next(nullptr) {}
	ListNode(const int x, ListNode *next) : val(x), next(next) {}
};

class Solution
{
public:
	// 双指针实现
	static ListNode *reverseList(ListNode *head)
	{
		ListNode *pre = nullptr;
		auto cur = head;
		while (cur != nullptr) {
			const auto tmp = cur->next;
			cur->next = pre;
			pre = cur;
			cur = tmp;
		}
		return pre;
	}

	// 递归方法实现
	static ListNode *reverseListV2(ListNode *head) { return rescur(head, nullptr); }

	static ListNode *rescur(ListNode *cur, ListNode *pre)
	{
		if (cur == nullptr) {
			return pre;
		}
		const auto tmp = cur->next;
		cur->next = pre;
		return rescur(tmp, cur);
	}
};

// https://leetcode.cn/problems/reverse-linked-list/?envType=study-plan-v2&envId=top-100-liked
TEST_CASE("reverse List")
{
	auto head = new ListNode(1);
	head->next = new ListNode(2);
	head->next->next = new ListNode(3);
	auto cur = head;
	while (cur) {
		std::cout << cur->val << std::endl;
		cur = cur->next;
	}

	auto newH = Solution::reverseList(head);
	cur = newH;
	std::cout << "反转后链表：" << std::endl;
	while (cur) {
		std::cout << cur->val << std::endl;
		cur = cur->next;
	}
}
