#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>

using namespace std;

void swap(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}

int S[256];

void KSA(string &key)
{
	
	for (int i = 0; i < 256; i++)
	{

		S[i] = i;
	}
	int j = 0;
	for (int i = 1; i < 256; i++)
	{
		j = (j + S[i] + key[i%key.length()]) % 256;
			swap(S[i], S[j]);
	}

}

//counters are stored outside so multiple calls of PRGA will be different
int i = 0;
int j = 0;
int k;
int PRGA()
{
	i = (i + 1) % 256;
	j = (j + S[i]) % 256;
	swap(S[i], S[j]);
	k = S[(S[i] + S[j]) % 256];
	return k;
}

void main()
{
	string text;
	string plaintext;
	string ciphertext;
	string decryption;

	fstream x;
	string key;

	int c = 0;
	x.open("Plaintext.txt");
	
	// opening the Plaintext.txt
	if (x.is_open())
	{
		while (!x.eof())
		{
			getline(x, text);
			plaintext += text;
		}
	}
	x.close();

	cout << "Original Text: " << plaintext << endl << endl;

	//ask user for key to encrypt
	cout << "Enter your encryption key: ";
	cin >> key;
	cout << endl;

	KSA(key);

	for (int z = 0; z < 3072; z++)
	{
		PRGA();
	}
	
	//using xor for each byte of plaintext with PRGA
	while (c < plaintext.length())
	{
		ciphertext += plaintext[c] ^ PRGA();
		c++;
	}
	cout << "This is the ciphertext: " << ciphertext << endl;

	//save encrypted text
	x.open("Encryption.txt", fstream::out);
	x << ciphertext << endl;
	x.close();

	cout << endl << endl;

	//ask user for key to decrypt
	cout << "Enter your decryption key: ";
	cin >> key;
	cout << endl;
	KSA(key);

	//resetting the counters to PRGA
	i = 0; j = 0;
	for (int z = 0; z < 3072; z++)
	{
		PRGA();
	}
	
	c = 0;
	while (c < ciphertext.length())
	{
		decryption += ciphertext[c] ^ PRGA();
		c++;
	}
	cout << "This is the decrytion: " << decryption << endl;

	//save decrypted text
	x.open("Decryption.txt", fstream::out);
	x << decryption << endl;
	x.close();

	system("pause");
}