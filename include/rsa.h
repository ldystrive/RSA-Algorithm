#ifndef RSA_H_
#define RSA_H_

#include "bignum.h"

class RSA {
	
public:
	
	//���ɶ����Ƴ���Ϊn�Ĵ�����
	static BigNum Gen_prime(int n);
	
	// ��������
	static bool millerRabinTest(BigNum A);

	//��ʼ�������ɹ�Կ��˽Կ
	void init();
	
	//��չŷ������㷨����˽Կ
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
