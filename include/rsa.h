#ifndef RSA_H_
#define RSA_H_

#include "bignum.h"

class RSA {
	
public:
	
	//生成二进制长度为n的大质数
	static BigNum Gen_prime(int n);
	
	// 素数测试
	static bool millerRabinTest(BigNum A);

	//初始化，生成公钥和私钥
	void init();
	
	//扩展欧几里德算法，求私钥
	BigNum exgcd(BigNum M, BigNum N);
	BigNum exgcd(BigNum M, BigNum N, BigNum &X, BigNum &Y);

	BigNum encode(BigNum M);
	BigNum decode(BigNum C);

	BigNum P, Q, N;
	BigNum E, D;
	BigNum phiN;

	// 128bits RSA
	const static int RSA_bits = 128;

	//Miller Rabin Test times
	const static int MRTestTimes = 15;
};


#endif // RSA_H_
