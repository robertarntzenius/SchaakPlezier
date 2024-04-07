#include "bindings.h"

namespace py = pybind11;

PYBIND11_MODULE(wrappers, module) {
    bindEnums(module);
    bindStructs(module);
    bindMaps(module);
    bindBoard(module);
    bindGame(module);
    bindPlayer(module);
}
