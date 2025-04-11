// test_offset.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include<stdio.h>
#include<stddef.h>
//����һ���ṹ��

struct S
{
	char a1;
	char a2;
	int b;
	char c;
	short d;
};

#pragma pack(1) // ��ʾ������Ĵ��붼����n���ֽڶ���
struct SPack
{
	char a1;
	char a2;
	int b;
	char c;
	short d;
};
#pragma pack() // ȡ������n���ֽڶ��룬�Ƕ�#pragma pack(n)�ķ������

int main()
{
	//offsetof���ʹ��
	size_t ret = offsetof(struct S, c);
	// printf("%zd\n", ret);
	printf("c in S at %d\n", (int)ret);
	ret = offsetof(struct SPack, c);
	printf("c in SPack at %d\n", (int)ret);

	return 0;
}
