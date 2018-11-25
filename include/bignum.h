#ifndef BIGNUM_H_
#define BIGNUM_H_

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <ctime>

class BigNum 
{
public:
	BigNum();
	BigNum(const BigNum &oth);
	BigNum(const int &a);
	BigNum(int _len, char *_data, bool _sign=0, bool _type=0);

	~BigNum();

	// Dec only
	friend std::istream & operator >> (std::istream &is, BigNum & A);
	friend std::ostream & operator << (std::ostream &os, const BigNum & A);

	friend BigNum operator + (const BigNum &A, const BigNum &B);
	friend BigNum operator - (const BigNum &A, const BigNum &B);
	friend BigNum operator * (const BigNum &A, const BigNum &B);
	friend BigNum operator / (const BigNum &A, const BigNum &B);
	friend BigNum operator % (const BigNum &A, const BigNum &B);
	
	friend bool operator > (const BigNum &A, const BigNum &B);
	friend bool operator < (const BigNum &A, const BigNum &B);
	friend bool operator == (const BigNum &A, const BigNum &B);
	BigNum &operator =(const BigNum &oth);

	friend BigNum operator -(const BigNum &A);
	friend BigNum operator +(const BigNum &A);


	static bool unsigned_less(const BigNum &A, const BigNum &B);
	static bool unsigned_greater(const BigNum &A, const BigNum &B);
	static BigNum unsigned_add(const BigNum &A, const BigNum &B);
	static BigNum unsigned_sub(const BigNum &A, const BigNum &B);
	static std::pair<BigNum, BigNum> unsigned_div(const BigNum &A, const BigNum &B);
	static BigNum pow_mod(BigNum A, BigNum B, const BigNum &mod);

	void dataAllocate(int n);
	bool isZero();
	bool isNaN();
	// 以上所有函数只支持十进制，若二进制需要计算，需要先转成十进制

	//随机生成二进制长度为n的大数
	void randBin(int n);
	void convertToBin();
	void convertToDec();
	void readBin(std::istream &is);


	int len;

	//0: positive  1:  negative
	bool sign;

	//not a number
	bool isnan;

	//0: Dec     1: Bin
	bool type;

	char *data = NULL;
};
#endif // BIGNUM_H_