#include <iostream>

extern "C"
{
	void __stdcall printerror(int code)
	{
		switch (code)
		{
		case 0:
		{
			std::cout << "ERROR 168: Division by zero is forbidden and will not be calculated\n";
			break;
		}
		case 1:
		{
			std::cout << "ERROR 169: Total length of two strings exceedes 255, so concatenation is cancelled\n";
			break;
		}
		case 2:
		{
			std::cout << "ERROR 1: Memory allocation error, returning source string\n";
			break;
		}
		}
	}
	int __stdcall stringcompare(char* left, char* right)
	{
		int len1 = (unsigned char)left[0], len2 = (unsigned char)right[0];
		int len = (len1 < len2) ? len1 : len2;
		for (int i = 0; i < len; i++)
		{
			if (left[i + 1] != right[i + 1]) return ((unsigned)left[i + 1] < (unsigned)right[i + 1] ? -1 : 1);
		}
		if (len1 == len2) return 0;
		else if (len1 > len2) return 1;
		else return -1;
	}
	int __stdcall userstringcompare(char* left, char* right)
	{
		int len1 = (unsigned char)left[0], len2 = (unsigned char)right[0];
		int len = (len1 < len2) ? len1 : len2;
		for (int i = 0; i < len; i++)
		{
			if (left[i + 1] != right[i + 1]) return (left[i + 1] < right[i + 1] ? -1 : 1);
		}
		if (len1 == len2) return 0;
		else if (len1 > len2) return 1;
		else return -1;
	}

	int __stdcall tonumber(char* str)
	{
		for (int i = 0; i < (unsigned char)str[0]; i++)
		{
			if (str[i + 1] < '0' || str[i + 1] > '9') return -1;
		}
		int result = 0;
		for (int i = 0; i < (unsigned char)str[0]; i++)
		{
			result *= 10;
			result += (str[i + 1] - '0');
		}
		return result;
	}

	int __stdcall indexof(char* where, char* what)
	{
		int length = (unsigned char)where[0];
		if ((unsigned char)what[0] < 1 || length < 0) return -1;
		for (int i = 0; i < length - (unsigned char)what[0] + 1; i++)
		{
			if (where[i + 1] == what[1])
			{
				for (int j = 1; j < (unsigned char)what[0]; j++)
				{
					if (where[i + 1 + j] != what[j + 1]) break;
					if (j == (unsigned char)what[0] - 1) return i;
				}
			}
		}
		return -1;
	}
	void __stdcall printnum(int n)
	{
		std::cout << n << std::endl;
	}
	void __stdcall printstr(char* s)
	{
		for (int i = 0; i < (unsigned char)s[0]; i++)
		{
			std::cout << s[i + 1];
		}
		std::cout << '\n';
	}
	char* __stdcall strconcat(char* s1, char* s2)
	{
		if ((unsigned char)s1[0] + (unsigned char)s2[0] > 255)
		{
			printerror(1);
			return s1;
		}
		char* ret = (char*)malloc(256);
		if (ret == 0)
		{
			printerror(2);
			return s1;
		}
		memset(ret, 0, 256);
		for (int i = 0; i < (unsigned char)s1[0]; i++) ret[i + 1] = s1[i + 1];
		for (int i = 0; i < (unsigned char)s2[0]; i++) ret[i + 1 + (unsigned char)s1[0]] = s2[i + 1];
		ret[0] = (unsigned char)s1[0] + (unsigned char)s2[0];
		return ret;
	}
	char* __stdcall sumsymbol(char* c1, char* c2)
	{
		if (c1[0] == 0) return c2;
		if (c2[0] == 0) return c1;
		char* ret = (char*)malloc(2);
		if (ret == 0)
		{
			printerror(2);
			return c1;
		}
		ret[0] = 1;
		ret[1] = (unsigned char)c1[1] + (unsigned char)c2[1];
		return ret;
	}
	void __stdcall stringcopy(char* dest, char* src)
	{
		for (int i = 0; i < (unsigned char)src[0] + 1; i++)
		{
			dest[i] = src[i];
		}
	}
	int __stdcall stringlength(char* s)
	{
		return s[0];
	}
}