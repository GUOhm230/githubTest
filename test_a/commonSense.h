#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "json.h"

using namespace std;

Json::Value getJson(const string& info);
vector <int> Greedy(vector <int>& outValue);
string read_json(string jsonFile);
