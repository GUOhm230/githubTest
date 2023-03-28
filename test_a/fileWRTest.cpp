#include <fstream>
#include <string>
#include <iostream>
using namespace std;

int main(){
    // ofstream fout;
    // fout.open("pri_key.txt",ios::out);
    // fout<<"郭辉铭，李玉，张华"<<endl;
    // fout.close();
    ifstream fin("public_key_music_rec_fri.txt",ios::in);
    fin.seekg(0, ios_base::end);
    int model_lSize = fin.tellg();
    fin.seekg(0, std::ios::beg);
    char* buffer = (char*)malloc(sizeof(char) * model_lSize);
    fin.read((char*)buffer, sizeof(char) * model_lSize);
    fin.close();
    string s = buffer;
    cout<<s<<endl;

    return 0;
}

