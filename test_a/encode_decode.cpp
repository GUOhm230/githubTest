# include "encode_decode.h"
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif



string my_privatemodel_p2;//上传至授权服务器的小部分模型
extern map<string, string> privateKey;
string keyFromApp = "";

// 曲谱-检测中英文识别模型
string music_det_salt_key = "4d8f19bc-826d-11ed-9792-0242ac110005";
string music_rec_ch_salt_key = "d12802f2-826d-11ed-9d97-0242c0a8000c";
string music_rec_fri_salt_key = "95d1595a-826e-11ed-8eba-0242c0a8000c";

// 曲谱-正反判断
string updown_pruned_v2_salt_key = "1e2180fa-8017-11ed-a726-0242ac110005";

// 曲谱-框选
string music_page_frame_salt_key = "e06ece98-825f-11ed-99e1-0242c0a8000c";

const string protectAlg_module = "{\"public_key_sign\": \"\", \"base64_pu\": \"\"}";

//2_step2_获取加密后的公钥信息
string _getPubkey(string function) {
    // // ---------------------------------生成公私钥并写入-----------------------------//
    // string pubkey = GeneratepubKey(function);

    // cout<<pubkey<<endl;
    // ofstream public_key_write;
    // public_key_write.open("test323/public_key_" + function + ".txt", std::ios::out | std::ios::app);
    // public_key_write<<pubkey<<endl;
    // public_key_write.close();
    // cout<<pubkey<<endl;

    // unsigned int in_len = pubkey.length();
    // const char *strpubkey = pubkey.c_str();

    // // ----------------------------------对公钥进行base_64加密并写入-----------------------//
    // string result64 = base64_encode(strpubkey, in_len);

    // ofstream base64_pu_key_write;
    // base64_pu_key_write.open("test323/base64_pu_" + function + ".txt", std::ios::out | std::ios::app);
    // base64_pu_key_write<<result64<<endl;
    // base64_pu_key_write.close();

    //===========================================================上下二选一===============================================================

    cout<<"// -------------------------------------读取现成的base_64加密字符串--------------------------//"<<endl;
    ifstream fin("test323/base64_pu_music_det.txt",ios::in);
    fin.seekg(0, ios_base::end);
    int model_lSize = fin.tellg();
    fin.seekg(0, std::ios::beg);
    char* buffer = (char*)malloc(sizeof(char) * model_lSize);
    fin.read((char*)buffer, sizeof(char) * model_lSize);
    fin.close();
    string result64 = buffer;
    cout<<result64<<endl;
    cout<<"// -------------------------------------按模型名称生成对应的salt---------------------------------------//"<<endl;
    // cout<<pubkey<<endl;
    string and64 = result64 + "&";
    string public_key_sign;
    //获取MD5值

    // 曲谱部分
    if (function == "music_det")
        public_key_sign = messageDigestFGet(and64, music_det_salt_key);
    else if (function == "music_rec_ch")
        public_key_sign = messageDigestFGet(and64, music_rec_ch_salt_key);
    else if (function == "music_rec_fri")
        public_key_sign = messageDigestFGet(and64, music_rec_fri_salt_key);
    else if (function == "music_page_frame")
        public_key_sign = messageDigestFGet(and64, music_page_frame_salt_key);
    else if (function == "updown_pruned_v2")
        public_key_sign = messageDigestFGet(and64, updown_pruned_v2_salt_key);

    // 结果存入json文件中。返回给前端
    Json::Value pubResult = getJson(protectAlg_module);
    pubResult["public_key_sign"] = public_key_sign;
    pubResult["base64_pu"] = result64;
    std::ostringstream os;
    string resultStr;
    Json::StreamWriterBuilder writerBuilder;
    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(pubResult, &os);
    resultStr = os.str();
    ofstream sign_write;
    sign_write.open("test323/sign_" + function + ".txt", std::ios::out | std::ios::app);
    sign_write<<resultStr<<endl;
    sign_write.close();
    return resultStr;
}

// model_p2解密
bool decode_authorization(string &encodedInfo, string function) {
    my_privatemodel_p2.clear();
    my_privatemodel_p2.shrink_to_fit();
    // step1: 授权服务器传过来的encodeInfo首先base64解密获取服务器生成的aes密钥以及模型信息，该密钥用以加密model—p2
    Json::Value encode_info_json = getJson(encodedInfo);
    string key_decoded = base64_decode(encode_info_json["en_params_aes_key"].asString());

    string aes_data_de64 = base64_decode(encode_info_json["enc_aes_data"].asString());
    cout<<"step1 over"<<endl;

    // string myprivatekey = privateKey.at(function); //提交前端代码，用map储存了当前的私钥
    // ----------------------------------------私钥解密测试，因此要读取私钥-------------------------------------------
    ifstream fin("test323/pri_key.txt",ios::in);
    fin.seekg(0, ios_base::end);
    int model_lSize = fin.tellg();
    fin.seekg(0, std::ios::beg);
    char* buffer = (char*)malloc(sizeof(char) * model_lSize);
    fin.read((char*)buffer, sizeof(char) * model_lSize);
    fin.close();
    string myprivatekey = buffer;
    // cout<<"-=------------------------------------------------------"<<endl;
    // cout<<myprivatekey<<endl;
    // cout<<"-=------------------------------------------------------"<<endl;

    // ----------------------------------------------私钥读取完成--------------------------------------------------------------

    if (myprivatekey.length() == 0)
        return false;

    unsigned char aes_keys[4098] = {};
    cout<<"pri_key read over"<<endl;

    // step2: 服务器生成的密钥是经过公钥加密的，因此本处要先对该密钥进行私钥解密。获取的aes密钥用于进行模型信息的解密
    int key_length = private_decrypt((unsigned char *) key_decoded.c_str(), key_decoded.length(),
                                     (unsigned char *) myprivatekey.c_str(), aes_keys);
    
    // ofstream aes_keys_write;
    // aes_keys_write.open("test323/aes_keys" + function + ".txt", std::ios::out | std::ios::app);
    // aes_keys_write<<aes_keys<<endl;
    // aes_keys_write.close();
    // // cout<<aes_keys<<endl;
    // cout<<"aes_key:"<<aes_keys<<endl;
    if (key_length <= 0)
        return false;
    cout<<"step3 over"<<endl;

    // step3: 使用解密后的服务器aes密钥对服务器传输过来经base_64解码后的字符串aes_data_de64进行解密
    const int &KEY_SIZE = key_length;
    unsigned char inbuf[4096];
    unsigned char outbuf[4096];
    AES_KEY decrypt_key = {};
    size_t offset = 0;
    unsigned char *user_key = (unsigned char *) malloc(sizeof(unsigned char) * KEY_SIZE);
    string aes_save;
    for (int i = 0; i < KEY_SIZE; i++){
        user_key[i] = aes_keys[i];
        aes_save.push_back(aes_keys[i]);
    }
    cout<<"-----------------------------------aes解密-----------------------------------------"<<endl;
    string decode_json_result;
    cout<<aes_save<<endl;
    cout<<"-----------------------------------aes解密-----------------------------------------"<<endl;
    AES_set_decrypt_key(user_key, KEY_SIZE * 8, &decrypt_key);
    cout<<"AES_set_decrypt_key 运行完成"<<endl;
    int model_length = aes_data_de64.length();
    int read_times = model_length / 4096;
    if (model_length % 4096 > 0)
        read_times++;

    for (int ri = 0; ri < read_times - 1; ri++) {
        for (int bi = 0; bi < 4096; bi++)
            inbuf[bi] = aes_data_de64[ri * 4096 + bi];

        offset = 0;
        for (int i = 0; i < 4096 / AES_BLOCK_SIZE; ++i) {
            AES_ecb_encrypt(inbuf + offset, outbuf + offset, &decrypt_key, AES_DECRYPT);
            offset += AES_BLOCK_SIZE;
        }

        vector<unsigned char> vctBuf(outbuf, outbuf + 4096);
        string strBuf(vctBuf.begin(), vctBuf.end());
        decode_json_result.append(strBuf);
    }
    size_t len = model_length % 4096;
    int outlen = 0;
    for (int bi = 0; bi < model_length % 4096; bi++)
        inbuf[bi] = aes_data_de64[(read_times - 1) * 4096 + bi];
    for (int bi = model_length % 4096; bi < 4096; bi++)
        inbuf[bi] = 0;

    offset = 0;
    for (int i = 0; i < len / AES_BLOCK_SIZE; ++i) {
        AES_ecb_encrypt(inbuf + offset, outbuf + offset, &decrypt_key, AES_DECRYPT);
        offset += AES_BLOCK_SIZE;
    }
    outlen = len;
    int padding = (int) outbuf[outlen - 1];
    for (int j = 0; j < padding; ++j) {
        if ((int) outbuf[outlen - j - 1] != padding) {
            padding = 0;
            break;
        }
    }
    outlen -= padding;
    vector<unsigned char> vctBuf(outbuf, outbuf + outlen);
    string strBuf(vctBuf.begin(), vctBuf.end());
    decode_json_result.append(strBuf);
    free(user_key);


    Json::Value decode_result = getJson(decode_json_result);
    cout<<"step3 over"<<endl;

    // step4: 获取解密字符串
    my_privatemodel_p2 = base64_decode(decode_result["params"].asString());
    // authorization_id = base64_decode(decode_result["authorization_id"].asString()).c_str();
    // md5_key = base64_decode(decode_result["md5_key"].asString());

    if (my_privatemodel_p2.length() > 0)
        return true;
    else
        return false;
}


//前端传进的model_p1即模型拆分后较大的部分并用base_64解密获得aes密钥再解密服务器传过来的model_p2。二者拼接成完整的model
char *modelCombine(string model_p1_path, size_t &model_size) {
    //模型较大部分
    char *path1 = (char *) model_p1_path.c_str();

    std::ifstream in1(path1, std::ios::binary);
    if (in1.fail())
        return nullptr;
    in1.seekg(0, ios_base::end);
    int lSize1 = in1.tellg();
    int p2_length = my_privatemodel_p2.length();
    in1.seekg(0, std::ios::beg);
    //完整的model字符串
    char *buffer = (char *) malloc(sizeof(char) * lSize1 + sizeof(char) * p2_length);
    char *buffer1 = (char *) malloc(sizeof(char) * lSize1);

    if (buffer == NULL) {
        fputs("Memory error", stderr);
        free(buffer);
        return nullptr;
        exit(2);
    }
    in1.read((char *) buffer1, sizeof(char) * lSize1);
    in1.close();
    //模型整合，获取buffer
    for (int j = 0; j < p2_length; j++)
        buffer[j] = my_privatemodel_p2[j];
    for (int i = 0; i < lSize1; i++)
        buffer[i + p2_length] = buffer1[i];
    model_size = in1.gcount() + size_t(p2_length);
    string outfile = "music_rec_fri_completeModel";
    std::ofstream out(outfile, std::ios::binary);
    out.write((char *) buffer, lSize1 + p2_length);
    cout << model_size << endl;
    // free(buffer);
    return buffer;
}

// 第一步加密判定：md5校正.
bool encryptStep1(string randStr, string md5App){
    string md5So = messageDigestFGet(keyFromApp, randStr);
    int temp = md5App.compare(md5So);
    bool flag;
    temp == 0?flag = true:flag = false;
    // flag=true;
    if(!flag){
        cout<<"md5校验不通过"<<endl;
        return false;
    }
    return true;
}

int main(){
    // ------------step2:提交部分模型，收到反馈后，获得pub_key.txt以及私钥----------------
    // string av = _getPubkey("music_det"); //music_det, music_rec_ch, music_rec_fri, updown_pruned_v2
    // cout<<"step1 done!"<<endl;
    // ------------step4:提交部分模型，收到反馈后，获得pub_key.txt以及私钥----------------
    /*
    3.27修改：原来的encodedInfo是一个字典，因此读一次就行。现在是字典的嵌套，因此，需要反复的读取
    */
    // string encodedInfo = "{\"enc_aes_data\": \"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\", \"en_params_aes_key\": \"fqxm12j3R+ecyPPvU1T7gQvCZzXhsEjDgQ5glfdIsmkvNCvxmRjxjRUdKyn0UyI3AStIbjxLc++ADZqeQFMrkSb8EbY9keqXFW1Ca8Cb3N+Vvh3H3OyhZVfo7tdR90s0+gt/a0Ryq1lt8lPRayFSTT0zssm5JMX/RLaOTD8wn2FtSftq7AmIssKLk0U17a9Nsui5fNzEatBTubv8c5srdrsAWYKfTjQ4i0WfDcWYLTmDc71n1pqYtqc7Vl2C9dtvGkrgdAjNV7nGD5QZt4sLCM6fuO5HkZi9qIpzvkiMSWiPQeG7m0rcEADyyUND6hOmSHj5KOjt2K0FzSane9CcxQ==\"}";
	// json文件读取测试
    // Json::Reader read_builder;
	// Json::Value root;
	// if (read_builder.parse(encodedInfo, root))
	// {
	// 	string name = root["enc_aes_data"].asString();
	// 	string age = root["en_params_aes_key"].asString();
	// 	// string sex = root["sex"].asString();
    //     cout<<"-------------------------------------------------------------"<<endl;
	// 	cout << name + "," << age <<  endl;
    // }
    ifstream fin("test323/encodeInf.txt",ios::in);
    fin.seekg(0, ios_base::end);
    int model_lSize = fin.tellg();
    fin.seekg(0, std::ios::beg);
    char* buffer = (char*)malloc(sizeof(char) * model_lSize);
    fin.read((char*)buffer, sizeof(char) * model_lSize);
    fin.close();
    string encodedInfo = buffer;
    // cout<<"================================================================"<<endl;
    // cout<<encodedInfo<<endl;
    // cout<<"================================================================"<<endl;

    decode_authorization(encodedInfo, "music_det");
    cout<<my_privatemodel_p2.length()<<endl;

//    char* buffer = modelCombine("./music/music_rec_fri_submit2frontEnd");
//    else cout<<"授权服务器数据解析错误"<<endl;

//     // cout<<authorization_id.c_str()<<endl;
//     // cout<<md5_key.c_str()<<endl;
   return 0;
}


