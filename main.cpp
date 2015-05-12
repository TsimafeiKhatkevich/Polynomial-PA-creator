#include "common_lib.h"

int main(int argc, const char* argv[]) {
    TModelConfig cfg;
    cfg.ParseFromFile(argv[1]);
    std::ofstream output(argv[2]);

    // TODO: main staff

    output.close();

    return 0;
}
