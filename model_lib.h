#pragma once

#include <fstream>
#include <string>
#include <assert.h>
#include <vector>

#include "common_lib.h"

// accepted params: start_size, steps, out_deg_half, alpha, beta, delta
struct TModelConfig {
    uint32_t NStartVertices;
    uint32_t NSteps;
    uint32_t OutDegree;

    double Alpha;
    double Beta;
    double Delta;

    void ParseFromFile(const std::string& fileName);
};

class TPolynomialModelGenerator {
public:
    TPolynomialModelGenerator(const TModelConfig& cfg);

    bool TryNextStep();

    void WriteEdges(std::ostream& output) const;

private:
    typedef NMath::TIntPair TEdge;

private:
    const TModelConfig Cfg;
    std::map<double, NMath::TIntPair> CDF;

    size_t NVertices;
    std::vector<TEdge> EdgeList;
};
