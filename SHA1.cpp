#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <ctime>
using namespace std;

#define CircularShift(word, n) ((word) << (n)) | ((word) >> (32-(n))) // circular left shift operation

// initialized with a constant value
unsigned int H0 = 0x67452301; // A
unsigned int H1 = 0xEFCDAB89; // B
unsigned int H2 = 0x98BADCFE; // C
unsigned int H3 = 0x10325476; // D
unsigned int H4 = 0xC3D2E1F0; // E

// A sequence of constant word K(t)
unsigned int K[4] = { 0x5A827999, // 0 <= k < 20
					  0x6ED9EBA1, // 20 <= k < 40
					  0x8F1BBCDC, // 40 <= k < 60
					  0xCA62C1D6 }; // 60 <= k < 80

unsigned int W[80];	// word sequence

unsigned char SHA1IN[64]; // 512 bits message block

string ProcessingMessage();
unsigned char* HextoBits(const char*);
char DectoHex(int);

int main()
{
	ofstream file_in("file_in.mem");
	ofstream file_out("file_out.mem");
	srand((unsigned int)time(NULL));

	char string_tmp[129] = {0, };

	for (int j = 0; j < 128; j++) {
		int rd = rand() % 16;
		file_in << hex << rd;
		string_tmp[j] = DectoHex(rd);
		cout << string_tmp[j];
	}

	
	cout << endl << string_tmp << endl;
	string sha1(string_tmp);  cout << endl << sha1 << endl;
	string output;
	
	const char* SHA1IN_t;
	unsigned char* temporary;
	//cout << "input 512 bit : ";
	//cin >> sha1;
	SHA1IN_t = sha1.c_str();
	//cout << SHA1IN_t << endl;
	//printf("%02X\n", SHA1IN_t[1]);
	
	temporary = HextoBits(SHA1IN_t);
	memcpy(SHA1IN, temporary, sizeof(SHA1IN));

	//cout << "\noutput round" << endl;

	output = ProcessingMessage();

	file_out << output;

	file_out.close();
	file_in.close();

	return 0;
}


string ProcessingMessage(void)
{
	unsigned int temp;	// Temporary word value
	unsigned int A, B, C, D, E;	 // 32 bits word buffers
	stringstream hts0, hts1, hts2, hts3, hts4;
	string s;

	for (int t = 0; t < 16; t++) {
		W[t] = SHA1IN[t * 4] << 24;
		W[t] |= SHA1IN[t * 4 + 1] << 16;
		W[t] |= SHA1IN[t * 4 + 2] << 8;
		W[t] |= SHA1IN[t * 4 + 3];
		//printf("W[%d]%08X\n",t, W[t]);
	}

	for (int t = 16; t < 80; t++) {
		W[t] = CircularShift(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
		//printf("W[%d]%08X\n",t, W[t]);
	}

	A = H0;
	B = H1;
	C = H2;
	D = H3;
	E = H4;
	//temp = CircularShift(A, 5);
	//cout << hex << temp << endl;
	//printf("Round0 --> A: %X B: %X C: %X D: %X E: %X\n", A, B, C, D, E);

	for (int t = 0; t < 20; t++) {		// Round 1
		temp = CircularShift(A, 5);
		temp += (((B & C) | ((~B) & D)) + E + W[t] + K[0]);
		E = D;
		D = C;
		C = CircularShift(B, 30);
		B = A;
		A = temp;
		//printf("Round0%d --> A: %X B: %X C: %X D: %X E: %X\n", t, A, B, C, D, E);
	}

	//printf("Round1 --> A: %08X B: %08X C: %08X D: %08X E: %08X\n", A, B, C, D, E);

	for (int t = 20; t < 40; t++) {		// Round 2
		temp = CircularShift(A, 5);
		temp += ((B ^ C ^ D) + E + W[t] + K[1]);
		E = D;
		D = C;
		C = CircularShift(B, 30);
		B = A;
		A = temp;
	}

	//printf("Round2 --> A: %08X B: %08X C: %08X D: %08X E: %08X\n", A, B, C, D, E);

	for (int t = 40; t < 60; t++) {		// Round 3
		temp = CircularShift(A, 5);
		temp += (((B & C) | (B & D) | (C & D)) + E + W[t] + K[2]);
		//unsigned int temp2 = CircularShift(A, 5) + (((B & C) | (B & D) | (C & D)) + E + W[t] + K[2]);
		E = D;
		D = C;
		C = CircularShift(B, 30);
		B = A;
		A = temp;
	}

	//printf("Round3 --> A: %08X B: %08X C: %08X D: %08X E: %08X\n", A, B, C, D, E);

	for (int t = 60; t < 80; t++) {		// Round 4
		temp = CircularShift(A, 5);
		temp += ((B ^ C ^ D) + E + W[t] + K[3]);
		E = D;
		D = C;
		C = CircularShift(B, 30);
		B = A;
		A = temp;
	}

	//printf("Round4 --> A: %08X B: %08X C: %08X D: %08X E: %08X\n", A, B, C, D, E);

	H0 += A;
	H1 += B;
	H2 += C;
	H3 += D;
	H4 += E;

	hts0 << hex << H0;
	s = hts0.str();
	hts1 << hex << H1;
	s = s + hts1.str();
	hts2 << hex << H2;
	s = s + hts2.str();
	hts3 << hex << H3;
	s = s + hts3.str();
	hts4 << hex << H4;
	s = s + hts4.str();

	printf("\nresult: %08X%08X%08X%08X%08X\n", H0, H1, H2, H3, H4);
	cout << "result: " << s << endl;

	return s;
}

unsigned char* HextoBits(const char* hex_str) // String to bits
{
	char tmp[3];
	int hex_len = strlen(hex_str);
	unsigned char* bits = NULL;
	//cout << hex_len << endl;
	bits = (unsigned char*)malloc(hex_len / 2);

	for (int i = 0; i < (hex_len / 2); i++) {
		memcpy(tmp, hex_str + (i * 2), 2);
		tmp[2] = 0;
		//cout << tmp << endl;
		bits[i] = (unsigned char)strtoul(tmp, NULL, 16);
	}
	//cout << bits << endl;
	return bits;
}

char DectoHex(int dec)
{
	if (dec < 10)
		return (48 + dec);
	else
		return 65 + (dec - 10);
}