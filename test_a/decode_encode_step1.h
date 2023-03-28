#ifndef _OPENSSL_DECODE_ENCODE
#define _OPENSSL_DECODE_ENCODE

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <cstring>
#include <sstream>
#include <string>   

#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>

using namespace std;


const char key[17]="M!n@c#5$4%3^2&1*";

const int KEY_SIZE = 16;   //  128bit

#define LOG(x) std::cout << (x) << std::endl;

#define PUB_KEY_FILE "pubkey.pem" 
#define PRI_KEY_FILE "prikey.pem"    

// 加密函数
bool encrypt_file(string infile, string outfile, const char* key=key, int key_size=17);

//解密函数
char* decrypt_file(string mnnPath, size_t &model_size);

#endif