#include "common_lib.h"

void TModelConfig::ParseFromFile(const std::string& fileName) {
    std::ifstream fin(fileName.c_str());
    assert(fin.good());

    std::unordered_map<std::string, std::string> paramsMap;
    std::string line;
    while (std::getline(fin, line)) {
        std::string fieldName = line.substr(0, line.find_first_of(" \t="));
        std::string fieldVal = line.substr(line.find_last_of(" \t=") + 1, line.size());
        paramsMap[fieldName] = fieldVal;
    }

    auto it = paramsMap.find("start_size");
    if (it != paramsMap.end()) {
        NStartVertices = atoi(it->second.c_str());
    }

    it = paramsMap.find("steps");
    if (it != paramsMap.end()) {
        NSteps = atoi(it->second.c_str());
    }

    it = paramsMap.find("out_deg_half");
    if (it != paramsMap.end()) {
        OutDegree = 2 * atoi(it->second.c_str());
    }

    it = paramsMap.find("alpha");
    if (it != paramsMap.end()) {
        Alpha = atof(it->second.c_str());
    }

    it = paramsMap.find("beta");
    if (it != paramsMap.end()) {
        Beta = atof(it->second.c_str());
    }

    it = paramsMap.find("delta");
    if (it != paramsMap.end()) {
        Delta = atof(it->second.c_str());
    }

    assert(Alpha + Beta + Delta == 1.0);
}
