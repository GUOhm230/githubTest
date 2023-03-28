#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>

#include "rsa_ed.h"
#include "base64_ed.h"
#include "json.h"
#include "commonSense.h"
#include "messageDigestFive.h"
#include <map>

using namespace std;


extern string my_privatemodel_p2;

string _getPubkey(string model_name);
bool decode_authorization(string& encodedInfo, string function);
char* modelCombine(string model_p1_path,size_t &model_size);
bool encryptStep1(string randStr, string md5App);

