#include "bignum.h"

BigNum::BigNum()
{
	len = 1;
	data = NULL;
	dataAllocate(len);
	data[0] = 0;
	sign = 0;
	isnan = 0;
	type = 0;
}

BigNum::BigNum(const BigNum &oth)
{
	len = oth.len;
	sign = oth.sign;
	isnan = oth.isnan;
	type = 0;
	data = NULL;
	dataAllocate(len);
	memcpy(data, oth.data, sizeof(char) * len);
}

BigNum::BigNum(const int &a)
{
	std::ostringstream oss;
	oss << a;
	std::string s = oss.str();
	
	len = (int)s.length();
	sign = 0;
	isnan = 0;
	type = 0;
	data = NULL;
	if (s[0] == '-') {
		len--;
		sign = 1;
	}
	dataAllocate(len);
	for (int i = 0; i < len; i++)
		data[i] = s[sign + len - i - 1] - '0';
}

BigNum::BigNum(int _len, char *_data, bool _sign, bool _type)
{
	len = _len;
	sign = _sign;
	type = _type;
	data = NULL;
	dataAllocate(len);
	memcpy(data, _data, sizeof(char) * len);
}

BigNum::~BigNum()
{
	delete []data;
	data = NULL;
}

bool BigNum::isZero()
{
	if (len == 1 && data[0] == 0)
	{
		sign = 0;
		return 1;
	}
	return 0;
}

bool BigNum::isNaN()
{
	return isnan;
}

void BigNum::dataAllocate(int n)
{
	//len = n;
	if (data == NULL) {
		data = new char[n]();
		return;
	}
	delete[]data;
	data = new char[n]();
}

std::ostream & operator << (std::ostream &os, const BigNum &A)
{
	if (A.len == 0)
		return os;
	if (A.sign && !(A.len == 1 && A.data[0] == 0))
		os << '-';
	for (int i = A.len - 1; i >= 0; i--) 
		os << (int)(A.data[i]);
	return os;
}

std::istream & operator >> (std::istream &is, BigNum &A)
{
	std::string s;
	is >> s;
	if (s.empty()) {
		A.isnan = 1;
		return is;
	}
	std::string::iterator it = s.begin();
	A.len = (int)(s.end() - s.begin());
	A.sign = 0;
	A.type = 0;
	if ((*it) == '-') {
		A.sign = 1;
		A.len--;
		it++;
	}
	A.dataAllocate(A.len);
	for (int i = A.len - 1; it != s.end(); it++, i--)
		A.data[i] = (*it) - '0';
	
	A.isZero();
	return is;
}

bool BigNum::unsigned_less(const BigNum &A, const BigNum &B)
{
	if (A.len != B.len)
		return A.len < B.len;
	for (int i = A.len - 1; i >= 0; i--)
	{
		if (A.data[i] == B.data[i]) continue;
		return A.data[i] < B.data[i];
	}
	return 0;
}

bool BigNum::unsigned_greater(const BigNum &A, const BigNum &B)
{
	return unsigned_less(B, A);
}

BigNum BigNum::unsigned_add(const BigNum &A, const BigNum &B) 
{
	int mxlen = std::max(A.len, B.len) + 1;
	char *tmp = new char[mxlen]();
	for (int i = 0; i < mxlen - 1; i++)
	{
		char a = 0, b = 0;
		if (i < A.len) a = A.data[i];
		if (i < B.len) b = B.data[i];
		tmp[i] += a + b;
		tmp[i + 1] = tmp[i] / 10;
		tmp[i] %= 10;
	}
	
	int newlen = mxlen;
	if (tmp[mxlen - 1] == 0) newlen--;
	BigNum Ans(newlen, tmp);
	delete[]tmp;
	return Ans;
}

BigNum BigNum::unsigned_sub(const BigNum &A, const BigNum &B)
{
	assert(!(unsigned_less(A, B)));
	int mxlen = A.len;
	char *tmp = new char[mxlen]();
	for (int i = 0; i < A.len; i++) 
	{
		char a = 0, b = 0;
		if (i < A.len) a = A.data[i];
		if (i < B.len) b = B.data[i];

		tmp[i] += a - b;
		if (tmp[i] < 0) {
			tmp[i] += 10;
			tmp[i + 1] --;
		}
	}
	int newlen = mxlen;
	while (tmp[newlen - 1] == 0 && newlen > 1) newlen--;
	BigNum Ans(newlen, tmp);
	delete[]tmp;
	return Ans;
}

std::pair<BigNum, BigNum> BigNum::unsigned_div(const BigNum &A, const BigNum &B)
{
	BigNum quotient, remainder = A;
	char *ptr = new char[A.len]();
	BigNum tmp(A.len, ptr);
	BigNum M;
	for (int i = A.len; i > 0; i--)
	{
		tmp.len = i;
		for (int j = 9; j > 0; j--)
		{
			tmp.data[i - 1] = (char)j;
			M = tmp * B;
			if (!unsigned_less(remainder, M)) {
				quotient = unsigned_add(quotient, tmp);
				remainder = unsigned_sub(remainder, M);
				break;
			}
		}
	}
	delete[]ptr;
	return std::make_pair(quotient, remainder);
}

bool operator < (const BigNum &A, const BigNum &B)
{
	if (A.sign != B.sign) return A.sign;
	if (A.sign == 0) return BigNum::unsigned_less(A, B);
	return BigNum::unsigned_greater(A, B);
}

bool operator > (const BigNum &A, const BigNum &B)
{
	return B < A;
}

bool operator == (const BigNum &A, const BigNum &B)
{
	return A < B == B < A;
}

BigNum operator -(const BigNum &A)
{
	BigNum B = A;
	B.sign ^= 1;
	return B;
}

BigNum operator +(const BigNum &A)
{
	return A;
}

BigNum &BigNum::operator=(const BigNum &oth)
{
	len = oth.len;
	sign = oth.sign;
	isnan = oth.isnan;
	type = oth.type;
	dataAllocate(len);
	memcpy(data, oth.data, sizeof(char) * len);
	return *this;
}

BigNum operator + (const BigNum &A, const BigNum &B)
{
	BigNum Ans;
	if (A.sign == B.sign) {
		Ans = BigNum::unsigned_add(A, B);
		
		Ans.sign = A.sign;
		return Ans;
	}
	if (BigNum::unsigned_greater(A, B)) {
		Ans = BigNum::unsigned_sub(A, B);
		Ans.sign = A.sign;
	}
	else {
		Ans = BigNum::unsigned_sub(B, A);
		Ans.sign = B.sign;
	}
	return Ans;
}

BigNum operator - (const BigNum &A, const BigNum &B)
{
	BigNum Ans;
	if (A.sign != B.sign) {
		Ans = BigNum::unsigned_add(A, B);
		Ans.sign = A.sign;
		return Ans;
	}
	
	if (BigNum::unsigned_greater(A, B)) {
		Ans = BigNum::unsigned_sub(A, B);
		Ans.sign = A.sign;
	}
	else {
		Ans = BigNum::unsigned_sub(B, A);
		Ans.sign = !A.sign;
	}
	return Ans;
}

BigNum operator * (const BigNum &A, const BigNum &B)
{
	int mxlen = A.len + B.len;
	char *tmp = new char[mxlen]();
	for (int i = 0; i < A.len; i++)
		for (int j = 0; j < B.len; j++)
		{
			char a = A.data[i];
			char b = B.data[j];
			char c = a * b + tmp[i + j];
			tmp[i + j] = c % 10;
			tmp[i + j + 1] += c / 10;
		}


	int newlen = mxlen;
	while (tmp[newlen - 1] == 0 && newlen > 1) newlen--;
	BigNum Ans(newlen, tmp, A.sign ^ B.sign);
	delete[]tmp;
	return Ans;
}

BigNum operator / (const BigNum &A, const BigNum &B)
{
	assert(!(B.len == 1 && B.data[0] == 0));
	std::pair<BigNum, BigNum> tmp = BigNum::unsigned_div(A, B);
	tmp.first.sign = A.sign ^ B.sign;
	return tmp.first;
}

BigNum operator % (const BigNum &A, const BigNum &B)
{
	assert(A.sign == 0 && B.sign == 0 && !(B.len == 1 && B.data[0] == 0));
	std::pair<BigNum, BigNum> tmp = BigNum::unsigned_div(A, B);
	tmp.second.sign = 0;
	return tmp.second;
}

BigNum BigNum::pow_mod(BigNum A, BigNum B, const BigNum &mod)
{
	BigNum Ans, zero, two;
	Ans.data[0] = 1;
	two.data[0] = 2;
	A = A % mod;
	while (B > zero)
	{
		if (B.data[0] & 1)
			Ans = (Ans * A) % mod;
		A = (A * A) % mod;
		B = B / two;
	}
	return Ans;
}


void BigNum::readBin(std::istream &is)
{
	is >> *this;
	type = 1;
}

void BigNum::randBin(int n)
{
	assert(n > 0);
	srand((unsigned int)time(NULL));
	dataAllocate(n);
	sign = 0;
	type = 1;
	len = n;
	isnan = 0;

	data[n - 1] = 1;
	for (int i = n - 2; i >= 0; i--)
		data[i] = rand() & 1;
}

void BigNum::convertToDec()
{
	BigNum sum, tmp, two;
	tmp.data[0] = 1;
	two.data[0] = 2;

	for (int i = 0; i < len; i++, tmp = tmp * two) {
		if (data[i] == 0) continue;
		sum = sum + tmp;
	}
	*this = sum;
}

void BigNum::convertToBin()
{
	int mxlen = len << 2;
	char *tmp = new char[mxlen]();
	BigNum A = *this;
	BigNum two;
	two.data[0] = 2;
	int newlen = 0;
	while (!(A.len == 1 && A.data[0] == 0)) {
		std::pair<BigNum, BigNum> T = unsigned_div(A, two);
		if (T.second.data[0] == 1)
			tmp[newlen] = 1;
		A = T.first;
		newlen++;
	}
	while (tmp[newlen - 1] == 0 && newlen > 1) newlen--;
	*this = BigNum(newlen, tmp, sign, 1);
	delete[]tmp;
}

