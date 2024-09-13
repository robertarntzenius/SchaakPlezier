#include "pyBoard.h"
#include "pyDefinitions.h"
#include "pyPlayer.h"


namespace py = pybind11;

PYBIND11_MODULE(wrappers, module) {
    bindEnums(module);
    bindStructs(module);
    bindBoard(module);
    bindPlayer(module);
}
