#include "common_lib.h"
#include "model_lib.h"

int main(int argc, const char* argv[]) {
    assert(argc >= 2);
    TModelConfig cfg;
    cfg.ParseFromFile(argv[1]);

    TPolynomialModelGenerator modelGenerator(cfg);

    while (modelGenerator.TryNextStep()) {
    }

    if (argc == 2) {
        modelGenerator.WriteEdges(std::cout);
    } else {
        std::ofstream output(argv[2]);
        modelGenerator.WriteEdges(output);
        output.close();
    }
    return 0;
}
