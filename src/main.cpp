#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fstream>

#include "bignum.h"
#include "des.h"
#include "rsa.h"

using namespace std;

RSA rsa;

// DES 秘钥
unsigned long long K;
BigNum DES_K;

// DES秘钥通过RSA加密后的串
BigNum RSAEncodedK;
BigNum RSADecodedK;

char in_text[10010];
char ans_text[10010];
int text_len;


template <typename T>
void input(T &t)
{
	char c;
	cin.get(c);
	if (c != '\n') {
		cin.putback(c);
		cin >> t;
		cin.get();
	}
}

BigNum ull2BigNum(unsigned long long A)
{
	ostringstream os;
	os << A;
	string s = os.str();
	BigNum Ans;
	Ans.len = (int)s.length();
	Ans.dataAllocate(Ans.len);
	for (int i = 0; i < s.length(); i++)
		Ans.data[i] = s[s.length() - i - 1] - '0';
	return Ans;
}

void init()
{
	cout << "RSA生成秘钥中..." << endl;
	cout << "(可能会比较慢，需要20s左右)" << endl;

	rsa.init();
	
	cout << "p: " << rsa.P << endl;
	cout << "q: " << rsa.Q << endl;
	cout << "n: " << rsa.N << endl;
	cout << "d: " << rsa.D << endl;
	cout << "e: " << rsa.E << endl;
	cout << "RSA init. done\n" << endl;	

	K = 1383827165325090801ULL;
	cout << "请输入DES秘钥 (default:1383827165325090801):";	
	input<unsigned long long>(K);
	DES_K = ull2BigNum(K);	
}

void des_cbc(BigNum Text, int len, BigNum Key, BigNum &Ans, unsigned long long coder(const unsigned long long &, const unsigned long long &), int mode)
{
	unsigned long long num = 0;
	unsigned long long temp = 0;
	unsigned long long ullkey = 0;
	int tot = 0;
	
	if (Key.type == 0) {
		// 十进制
		for (int i = Key.len - 1; i >= 0; i--)
			ullkey = ullkey * 10 + Key.data[i];
	}
	else { 
		// 二进制
		for (int i = Key.len - 1; i >= 0; i--)
			ullkey = ullkey << 1 | (unsigned long long) Key.data[i];
	}

	memset(in_text, 0, sizeof(in_text));
	for (int i = 0; i < Text.len; i++)
		in_text[i] = Text.data[i];



	for (int i = 0; i < len; i += 64) {

		num = 0;
		for (int j = 0; j < 64; j++)
			num = num << 1 | (unsigned long long)in_text[i + j];

		// encode, 先与前一段结果异或，再加密
		if (mode == 0)
			num ^= temp;

		unsigned long long ans = coder(num, ullkey);

		// decode，先解密，再与前一段结果异或
		if (mode == 1)
			ans ^= temp;

		// 
		for (int j = 0; j < 64; j++)
			ans_text[tot++] = ((ans >> (63 - j)) & 1);

		if (mode == 0) temp = ans;
		if (mode == 1) temp = num;
	}

	Ans = BigNum(tot, ans_text, 0, 1);
}

BigNum encode(BigNum Key, string M_path, BigNum &encodedKey)
{
	cout << "************************ENCODE************************" << endl;
	cout << "请输入要加密的.txt文件路径 (default:" << M_path << "):";
	input<string>(M_path);
	ifstream is(M_path);
	BigNum M, C;
	is >> M;
	M.type = 1;

	cout << "加密前明文:" << endl;
	cout << M << endl;
	cout << "--------------------------------------------------------" << endl;

	// cbc模式的DES加密
	text_len = ((M.len - 1) / 64 + 1) * 64;
	des_cbc(M, text_len, Key, C, DES::encode, 0);

	cout << "加密后密文:" << endl;
	cout << C << endl;

	encodedKey = rsa.encode(Key);
	return C;
}

BigNum decode(BigNum encodedKey, string C_path, BigNum &Key)
{
	cout << "************************DECODE************************" << endl;
	cout << "请输入要解密的.txt文件路径 (default:" << C_path << "):";
	input<string>(C_path);
	ifstream is(C_path);
	BigNum M, C;
	is >> C;
	C.type = 1;
	
	cout << "解密前密文:" << endl;
	cout << C << endl;
	cout << "--------------------------------------------------------" << endl;

	//先利用RSA解密出DES的秘钥
	Key = rsa.decode(encodedKey);

	//cbc模式的DES解密
	text_len = ((C.len - 1) / 64 + 1) * 64;
	des_cbc(C, text_len, Key, M, DES::decode, 1);
	
	cout << "解密后明文:" << endl;
	cout << M << endl;

	return M;
}

int main()
{
	init();
	cout << endl;

	// 加密 
	BigNum C = encode(DES_K, string("testData/input_plaintext.txt"), RSAEncodedK);
	cout << "RSA加密后的DES秘钥:" << endl; 
	cout << RSAEncodedK << endl;
	cout << endl;

	// 解密
	BigNum M = decode(RSAEncodedK, string("testData/input_ciphertext.txt"), RSADecodedK);
	cout << "RSA解密后的DES秘钥:" << endl;
	cout << RSADecodedK << endl;

	system("pause");
	return 0;
}
