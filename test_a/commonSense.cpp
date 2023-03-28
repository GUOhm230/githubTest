#include "commonSense.h"


Json::Value getJson(const string& info)
{
	bool res;
	Json::Value root;
	Json::CharReaderBuilder builder;
	JSONCPP_STRING errs;

	std::unique_ptr<Json::CharReader> const jsonReader(builder.newCharReader());
	res = jsonReader->parse(info.c_str(), info.c_str() + info.length(), &root, &errs);

	if (res == 0)std::cout << "RES = 0" << std::endl;
	if (!res || !errs.empty()) {
		std::cout << "parseJson err. " << errs << std::endl;
	}
	return root;
}


string read_json(string jsonFile) {
	Json::Value root;
	std::ifstream ifs;
	ifs.open(jsonFile);
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	JSONCPP_STRING errs;
	if (!parseFromStream(builder, ifs, &root, &errs)) {
		std::cout << errs << std::endl;
		return "";
	}
	ifs.close();
	std::ostringstream os;
	string textStr;
	Json::StreamWriterBuilder writerBuilder;
	writerBuilder.settings_["emitUTF8"] = true;//Config emitUTF8
	std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
	jsonWriter->write(root, &os);
	textStr = os.str();

	return textStr;
}

vector <int> Greedy(vector <int>& outValue)
{
	vector <int> result;
	for (int i = 0; i < outValue.size(); i++) {
		if (outValue[i] == 0)
			;
		else if (result.size() == 0)
			result.push_back(outValue[i]);
		else if (outValue[i] != outValue[i - 1])
			result.push_back(outValue[i]);
	}
	vector <int> final_result;
	for (int i = 0; i < result.size(); i++) {
		if (result[i] != 74)
			final_result.push_back(result[i]);
	}
	return final_result;
}
