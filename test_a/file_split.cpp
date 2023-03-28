/*
将模型文件进行拆分
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <cstring>
#include <sstream>
#include <string> 

using namespace std;


void file_string(std::string model_path){
    unsigned char inbuf[4096];
	// unsigned char outbuf[4096];
	size_t offset = 0;
	size_t len = 0;
	int outlen = 0;

    std::ifstream in(model_path, std::ios::binary);
    if (in.fail())
		return;
    in.seekg(0, ios_base::end);
    int model_lSize = in.tellg();
    in.seekg(0, std::ios::beg);


	char* buffer = (char*)malloc(sizeof(char) * model_lSize);
	if (buffer == NULL) {
		fputs("Memory error", stderr);
        free(buffer);
		exit(EXIT_FAILURE);
	}

    in.read((char*)buffer, sizeof(char) * model_lSize);
    cout<<"模型字节数："<<model_lSize<<"\t"<<in.gcount()<<endl;

    in.close();
    string outfile = "test.guo";
    std::ofstream out(outfile, std::ios::binary);
    // out.write((char*)buffer, model_lSize);
    char* outbuf = (char*)malloc(sizeof(char) * (1024*5));
    char* outbuf = (char*)malloc(sizeof(char) * (1024*5));


    for(int i = 0; i < 1024*5; i++){
        outbuf[i] = buffer[i];
    }
    for(int j = 5*1024; j < model_lSize; j++){

    }
    // cout<<"单个字符占用"<<strlen(outbuf[0])<<endl;
    out.write((char*)outbuf, 1024*5);


}

int main(){
    string model_path = "page_sfv2_6641_v2_sim.key";
    file_string(model_path);
    return 0;
}