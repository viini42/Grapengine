#ifndef GRAPENGINE_MAINLAYER_HPP
#define GRAPENGINE_MAINLAYER_HPP

#include <grapengine.hpp>

class MainLayer : public GE::Layer
{
public:
  MainLayer();

  ~MainLayer() override;
  void OnAttach() override;
  void OnUpdate(GE::TimeStep step) override;
  void OnEvent(GE::Event& event) override;

private:
  GE::Ptr<GE::Scene> m_scene;
};

#endif // GRAPENGINE_MAINLAYER_HPP
