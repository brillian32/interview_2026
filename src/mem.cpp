//
// Created by 17305 on 2025/12/24.
//

void *my_memcpy(void *dest, const void *src, size_t n) {
	// 必须转换为 char* (或 uint8_t*) 才能按字节操作
	char *d = (char *)dest;
	const char *s = (const char *)src;

	// 保存 dest 的首地址用于返回
	void *ret = dest;

	while (n--) {
		*d++ = *s++;
	}

	return ret;
}

void *my_memmove(void *dest, const void *src, size_t n) {
	char *d = (char *)dest;
	const char *s = (const char *)src;

	if (d == s) {
		return dest; // 相同地址无需拷贝
	}

	// 判断重叠情况
	// 如果 dest 在 src 后面，且 dest 在 src 的拷贝范围内 (dest < s + n)
	// 则必须从后向前拷贝，防止源数据被覆盖
	if (d > s && d < s + n) {
		// 指针移到末尾
		d = d + n - 1;
		s = s + n - 1;

		// 从后向前拷贝
		while (n--) {
			*d-- = *s--;
		}
	} else {
		// dest 在 src 前面，或者完全不重叠
		// 从前向后拷贝 (类似 memcpy)
		while (n--) {
			*d++ = *s++;
		}
	}

	return dest;
}