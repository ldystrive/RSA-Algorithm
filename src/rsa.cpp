#include "rsa.h"
#include "bignum.h"

//先用这6个数测试， 然后再用一些随机数测试
static int primeTestNum[] = { 2, 3, 5, 7, 61, 24251 };

bool RSA::millerRabinTest(BigNum A)
{
	BigNum one(1);
	BigNum two(2);
	BigNum zero(0);

	if (A == two) return true;
	if (A < two || (A % two == zero)) return false;
	if (A.len == 1) {
		int t = A.data[0];
		if (t == 2 || t == 3 || t == 5 || t == 7) return true;
		return false;
	}

	BigNum N1 = A - one;
	BigNum M = N1;
	int k = 0;
	while (M % two == zero) {
		k++;
		M = M / two;
	}
	N1.convertToBin();
	int len = N1.len;
	N1.convertToDec();
		
	BigNum a;
	for (int i = 0; i < MRTestTimes; i++)
	{
		if (i < 6) a = BigNum(primeTestNum[i]);
		else if (len < 20) a.randBin(len - 1);
		else a.randBin(19);

		a.convertToDec();
		BigNum x = BigNum::pow_mod(a, M, A);
		BigNum y;
		for (int j = 0; j < k; j++)
		{
			y = (x * x) % A;
			if (y == one && !(x == one) && !(x == A - one))
				return false;
			x = y;
		}
		if (!(y == one)) return false;
	}
	return true;
}

BigNum RSA::Gen_prime(int n)
{
	BigNum A;
	while (true) {
		A.randBin(n);
		A.data[0] = 1;
		A.convertToDec();
		if (millerRabinTest(A)) break;
	}
	return A;
}

BigNum RSA::exgcd(BigNum m, BigNum n)
{
	//BigNum X, Y;



	
	if (n == BigNum(0)) {
		return BigNum(1);
	}

	BigNum a, a1, b, b1, c, d, q, r, t;
	a1 = b = BigNum(1);
	a = b1 = BigNum(0);
	c = m; d = n;
	
	q = c / d; r = c % d;
	while (!(r == BigNum(0))) {
		c = d;
		d = r;
		t = a1;
		a1 = a;
		a = t - q * a;
		t = b1;
		b1 = b;
		b = t - q * b;

		q = c / d;
		r = c % d;
	}
	return a;

	
}

BigNum RSA::exgcd(BigNum A, BigNum B, BigNum &X, BigNum &Y)
{
	if (B == BigNum(0)) {
		X = BigNum(1);
		Y = BigNum(0);
		return A;
	}
	
	BigNum r = exgcd(B, A % B, X, Y);
	BigNum t = X;
	X = Y;
	Y = t - A / B * Y;
	return r;
}


void RSA::init() 
{
	E = BigNum(65537);
	P = Gen_prime(RSA_bits / 2);
	Q = Gen_prime(RSA_bits / 2);
	while(P == Q) Q = Gen_prime(RSA_bits / 2);

	N = P * Q;
	phiN = (P - BigNum(1)) * (Q - BigNum(1));
	BigNum X, Y;
	//exgcd(E, phiN, X, Y);
	D = exgcd(E, phiN);
	//D = X;
	D = (D + phiN) % phiN;
}

BigNum RSA::encode(BigNum M)
{
	return BigNum::pow_mod(M, E, N);
}

BigNum RSA::decode(BigNum C)
{
	return BigNum::pow_mod(C, D, N);
}