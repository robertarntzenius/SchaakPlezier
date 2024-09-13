#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "player.h"
#include "playerfactory.h"

namespace py = pybind11;


void bindPlayer(py::module& m) {
    py::class_<Player, std::unique_ptr<Player>> pyPlayer(m, "Player");

    pyPlayer.def("decideOnMove", &Player::decideOnMove);

    pyPlayer.def("getPlayerType", &Player::getPlayerType);

    m.def("makePlayer", [](const std::string& playerTypeString) { return PlayerFactory::makePlayer(playerTypeString); });

    py::enum_<PlayerType>(m, "PlayerType")
        .value("Human", PlayerType::Human)
        .value("Random", PlayerType::Random)
        .value("MinMax", PlayerType::MinMax)
        .value("MonteCarlo", PlayerType::MonteCarlo)
        .value("AlphaBeta", PlayerType::AlphaBeta);
}
