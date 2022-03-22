#include <string.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <memory>
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
					  0xCA62C1D6}; // 60 <= k < 80

unsigned int W[80];	// word sequence

unsigned char SHA1IN[64]; // 512 bits message block
unsigned int SHA1OUT[40]; // 160 bits output

void ProcessingMessage();
unsigned char* HextoBits(char*);

int main()
{
	string sha1;
	char SHA1IN_t[128];
	unsigned char* temporary;
	cout << "input 512 bit : ";
	cin >> sha1;
	strcpy(SHA1IN_t, sha1.c_str());
	cout << SHA1IN_t << endl;
	//printf("%d\n", SHA1IN_t[0]);
	//temporary = HextoBits(SHA1IN_t);
	//memcpy(SHA1IN, temporary, sizeof(SHA1IN));
	cout << SHA1IN << endl;

	cout << "\noutput round" << endl;

	ProcessingMessage();

	return 0;
}


void ProcessingMessage(void)
{
	unsigned int temp;	// Temporary word value
	unsigned int A, B, C, D, E;	 // 32 bits word buffers

	for (int t = 0; t < 15; t++) {
		W[t] = SHA1IN[t * 4] << 24;
		W[t] |= SHA1IN[t * 4 + 1] << 16;	
		W[t] |= SHA1IN[t * 4 + 2] << 8;
		W[t] |= SHA1IN[t * 4 + 3];
		//cout << hex << W[t] << endl;
	}

	for (int t = 16; t < 80; t++) {
		W[t] = CircularShift(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
	}

	A = H0;
	B = H1;
	C = H2;
	D = H3;
	E = H4;

	for (int t = 0; t < 20; t++) {		// Round 1
		temp = CircularShift(A, 5) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
		E = D;
		D = C;
		C = CircularShift(B, 30);
		B = A;
		A = temp;
	}

	cout << hex << "Round1 --> A: " << A << " B: " << B << " C : " << C << " D : " << D << " E : " << E << endl;

	for (int t = 20; t < 40; t++) {		// Round 2
		temp = CircularShift(A, 5) + (B ^ C ^ D) + E + W[t] + K[1];
		E = D;
		D = C;
		C = CircularShift(B, 30);
		B = A;
		A = temp;
	}

	cout << hex << "Round2 --> A: " << A << " B: " << B << " C : " << C << " D : " << D << " E : " << E << endl;

	for (int t = 40; t < 60; t++) {		// Round 3
		temp = CircularShift(A, 5) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
		E = D;
		D = C;
		C = CircularShift(B, 30);
		B = A;
		A = temp;
	}

	cout << "Round3 --> A: " << A << " B: " << B << " C : " << C << " D : " << D << " E : " << E << endl;

	for (int t = 60; t < 80; t++) {		// Round 4
		temp = CircularShift(A, 5) + (B ^ C ^ D) + E + W[t] + K[3];
		E = D;
		D = C;
		C = CircularShift(B, 30);
		B = A;
		A = temp;
	}

	cout << hex << "Round4 --> A: " << A << " B: " << B << " C : " << C << " D : " << D << " E : " << E << endl;

	H0 += A;
	H1 += B;
	H2 += C;
	H3 += D;
	H4 += E;

	cout << hex << "\nresult: " << H0 << H1 << H2 << H3 << H4 << endl;
}

unsigned char* HextoBits(char* hex_str) // String to bits
{
	char tmp[3];
	int hex_len = strlen(hex_str);
	unsigned char *bits = NULL;

	for (int i = 0; i < hex_len / 2; i++) {
		memcpy(tmp, hex_str + (i * 2), 2);
		tmp[2] = 0;
		bits[i] = (unsigned char)strtoul(tmp, NULL, 16);
	}

	return bits;
}