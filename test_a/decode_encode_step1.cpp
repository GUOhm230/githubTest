#include "decode_encode.h"
  
// 加密函数
bool encrypt_file(string infile, string outfile, const char* key, int key_size)
{
	unsigned char inbuf[4096];
	unsigned char outbuf[4096];
	unsigned char user_key[KEY_SIZE];
	AES_KEY encrypt_key = {};
	size_t offset = 0;
	size_t len = 0;
	int outlen = 0;
	strncpy((char*)user_key, key, KEY_SIZE);

	std::ifstream in(infile, std::ios::binary);
	std::ofstream out(outfile, std::ios::binary);
	if (in.fail() || out.fail())
		return false;

	AES_set_encrypt_key(user_key, KEY_SIZE * 8, &encrypt_key);

	while (true)
	{
		len = sizeof(inbuf) - AES_BLOCK_SIZE;

		in.read((char*)inbuf, len);
		len = in.gcount();
		if (len == 0)
			break;

		outlen = len;
		if (in.eof())  // file eof need padding
		{
			outlen = (outlen / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
			memset(inbuf + len, (outlen - len), outlen - len);  // padding
		}

		offset = 0;
		for (int i = 0; i < outlen / AES_BLOCK_SIZE; ++i)
		{
			AES_ecb_encrypt(inbuf + offset, outbuf + offset, &encrypt_key, AES_ENCRYPT);
			offset += AES_BLOCK_SIZE;
		}

		cout <<"outlen: " << outlen << " len: "<< len<<" AES_BLOCK_SIZE: "<< AES_BLOCK_SIZE<<endl;

		out.write((char*)outbuf, outlen);
	}

	in.close();
	out.flush();
	out.close();
	return true;
}

//解密函数
char* decrypt_file(string mnnPath, size_t &model_size)
{
	unsigned char inbuf[4096];
	unsigned char outbuf[4096];
	unsigned char user_key[KEY_SIZE];
	AES_KEY decrypt_key = {};
	model_size = 0;
	size_t offset = 0;
	size_t len = 0;
	int outlen = 0;

	strncpy((char*)user_key, key, KEY_SIZE);

	std::ifstream in(mnnPath, std::ios::binary);
	// std::ofstream out(outfile, std::ios::binary);//保存至缓存中，不再入sdk卡
	if (in.fail())
		return NULL;

	in.seekg(0, ios_base::end);
	int lSize = in.tellg();
	in.seekg(0, std::ios::beg);

	//创建缓存，储存解密后的二进制模型
	char* buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL) {
		fputs("Memory error", stderr);
		exit(2);
	}	


	AES_set_decrypt_key(user_key, KEY_SIZE * 8, &decrypt_key);
	while (true)
	{
		in.read((char*)inbuf, sizeof(inbuf));
		len = in.gcount();
		if (len == 0)
			break;

		offset = 0;
		for (int i = 0; i < len / AES_BLOCK_SIZE; ++i)
		{
			AES_ecb_encrypt(inbuf + offset, outbuf + offset, &decrypt_key, AES_DECRYPT);
			offset += AES_BLOCK_SIZE;
		}

		outlen = len;

		if (in.eof())  // parser padding
		{
			int padding = (int)outbuf[outlen - 1];
			for (int j = 0; j < padding; ++j)
			{
				if ((int)outbuf[outlen - j - 1] != padding)
				{
					padding = 0;
					break;
				}
			}
			outlen -= padding;
		}

		for (size_t i = model_size; i < model_size + outlen; i++) {
			if (i >= lSize) {
				return NULL;
			}
			buffer[i] = outbuf[i - model_size];
		}
		model_size += outlen;
	}
	cout<<"模型所占内存大小："<<model_size<<endl;

	cout << endl;

	in.close();
	return buffer;
}


//  int main(){
//          // ----------------------------------mnn加密测试---------------------------
//          std::string infile = "/home/lq4/SSDProject/workHandover/5_updownJudgement/pruning/model/resnet18_080_resize_classes3_model/resnetPruned_80Sparse_resizeModel_dynamic.mnn";
//          std::string outfile = "/home/lq4/SSDProject/workHandover/5_updownJudgement/pruning/model/resnet18_080_resize_classes3_model/resnetPruned_80Sparse_resizeModel_dynamic.key";
//          std::string outfile2 = "/home/lq4/SSDProject/md5_code/resnetPruned_80Sparse_model_dynamic_key_decode.mnn";
//          // char key[]="M!n@c#5$4%3^2&1*";
//          bool flag = encrypt_file(infile, outfile, key, 17);

//          // char *truebuffer = decrypt_file(outfile);
//          // if(truebuffer != NULL){
//          // 	cout<<"内存存储成功"<<endl;
//          // }
//          // else{
//          // 	cout<<"模型储存有误"<<endl;
//          // }
//          // 计算二进制文件的长度
//          // std::ifstream in_database_file(outfile, std::ios::binary);
//          // if (!in_database_file.is_open())
//          // {
//          // 	std::cout << "Fail to open database file." << std::endl;
//          // 	//throw std::exception("Error!");
//          // 	throw std::exception();
//          // }
//          // in_database_file.seekg(0, std::ios::end);
//          // size_t filesize=in_database_file.tellg();
//          // cout<<"filesize="<<filesize<<endl;
//          return 0;
//  }
