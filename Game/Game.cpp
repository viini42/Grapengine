#include "Game.hpp"

#include "MainLayer.hpp"

#include <core/ge_entry_point.hpp>

Game::Game() : GE::Application("F1 Game", { 640, 480 }, "") {
  auto main_layer = GE::MakeRef<MainLayer>();
  AddLayer(main_layer);
}

Game::~Game() {}

GE::Ptr<GE::Application> CreateApplication()
{
  return GE::MakeRef<Game>();
}
