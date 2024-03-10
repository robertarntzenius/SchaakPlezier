#include "bindings.h"

namespace py = pybind11;

PYBIND11_MODULE(wrappers, module) {
    bindEnums(module);
    bindStructs(module);
    bindBoard(module);
}
