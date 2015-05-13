#include "model_lib.h"

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
    assert(NStartVertices);

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

//////////////////////////////// TPolynomialModelGenerator /////////////////////////////////////////////////

TPolynomialModelGenerator::TPolynomialModelGenerator(const TModelConfig& cfg)
    : Cfg(cfg)
    , NVertices(Cfg.NStartVertices)
{
    EdgeList.reserve(Cfg.OutDegree * (NVertices + Cfg.NSteps));

    // Init graph
    for (size_t i = 0; i < Cfg.OutDegree * NVertices; ++i) {
        size_t source = NMath::GetRandomIndex(NVertices);
        size_t dest = NMath::GetRandomIndex(NVertices);
        EdgeList.push_back(TEdge(source, dest));
    }

    // Init CDF
    NMath::TCachedBinomialCalcer binomCalcer;
    const uint32_t p = Cfg.OutDegree / 2;
    double totalValue = 0.0;

    for (uint32_t k = 0; k <= p; ++k) {
        const double firstMult = binomCalcer.Calc(p, k) * NMath::Power(Cfg.Beta, k);
        for (uint32_t l = k; l <= p; ++l) {
            const double secondMult = binomCalcer.Calc(p - k, l - k)
                    * NMath::Power(Cfg.Alpha, l - k)
                    * NMath::Power(Cfg.Delta, p - l);

            if (firstMult * secondMult == 0.0) {
                continue;
            }
            totalValue += firstMult * secondMult;
            CDF[totalValue] = NMath::TIntPair(k, 2 * l);
        }
    }
}

bool TPolynomialModelGenerator::TryNextStep() {
    if (NVertices >= Cfg.NSteps + Cfg.NStartVertices) {
        return false;
    }

    const NMath::TIntPair sampledPair = NMath::SampleValueFromCDF(CDF);
    const size_t curSize = EdgeList.size();
    // sampling k edges
    for (uint32_t i = 0; i < sampledPair.first; ++i) {
        const size_t edgeInd = NMath::GetRandomIndex(curSize);
        EdgeList.push_back(TEdge(NVertices, EdgeList[edgeInd].first));
        EdgeList.push_back(TEdge(NVertices, EdgeList[edgeInd].second));
    }
    // sampling l - 2k
    for (uint32_t i = 0; i < sampledPair.second - 2 * sampledPair.first; ++i) {
        const size_t edgeInd = NMath::GetRandomIndex(curSize);
        EdgeList.push_back(TEdge(NVertices, EdgeList[edgeInd].second));
    }

    ++NVertices;

    // sampling m - l
    for (uint32_t i = 0; i < Cfg.OutDegree - sampledPair.second; ++i) {
        const size_t vInd = NMath::GetRandomIndex(NVertices);
        EdgeList.push_back(TEdge(NVertices - 1, vInd));
    }

    return true;
}

void TPolynomialModelGenerator::WriteEdges(std::ostream& output) const {
    for (const auto& edge : EdgeList) {
        output << edge.first << " " << edge.second << std::endl;
    }
}
