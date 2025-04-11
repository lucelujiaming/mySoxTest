// test_offset.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include<stdio.h>
#include<stddef.h>
//定义一个结构体

struct S
{
	char a1;
	char a2;
	int b;
	char c;
	short d;
};

#pragma pack(1) // 表示它后面的代码都按照n个字节对齐
struct SPack
{
	char a1;
	char a2;
	int b;
	char c;
	short d;
};
#pragma pack() // 取消按照n个字节对齐，是对#pragma pack(n)的反向操作

int main()
{
	//offsetof宏的使用
	size_t ret = offsetof(struct S, c);
	// printf("%zd\n", ret);
	printf("c in S at %d\n", (int)ret);
	ret = offsetof(struct SPack, c);
	printf("c in SPack at %d\n", (int)ret);

	return 0;
}
