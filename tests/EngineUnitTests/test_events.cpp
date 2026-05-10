#include "events/ge_event.hpp"

#include <gtest/gtest.h>

#if defined(GE_CLANG_COMPILER)
  #pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

using namespace GE;

TEST(Event, WindowCloseData)
{
  Event close_ev{ EvType::WINDOW_CLOSE };
  EXPECT_EQ(close_ev.GetType(), EvType::WINDOW_CLOSE);
}

TEST(Event, WindowCloseDataDispatcher)
{
  Event close_ev{ EvType::WINDOW_CLOSE };

  bool dispatched = false;
  close_ev //
    .When(EvType::WINDOW_CLOSE)
    .Then([&](const EvData& /*ev*/) { dispatched = true; });

  EXPECT_TRUE(dispatched);
  EXPECT_TRUE(close_ev.IsHandled());
}

TEST(Event, KeyPressData)
{
  Event key_press_ev{ EvType::KEY_PRESS, KeyCode::A };
  EXPECT_EQ(key_press_ev.GetType(), EvType::KEY_PRESS);
}

TEST(Event, KeyPressDataDispatcher)
{
  Event key_press_ev{ EvType::KEY_PRESS, KeyCode::A };

  key_press_ev //
    .When(EvType::KEY_PRESS)
    .ThenWithRes(
      [](const EvData& ev) -> bool
      {
        const auto& key = *std::get_if<KeyCode>(&ev);
        return key == KeyCode::A;
      });

  EXPECT_TRUE(key_press_ev.IsHandled());
}

TEST(Event, KeyReleaseData)
{
  Event key_rel_ev{ EvType::KEY_RELEASE, KeyCode::B };
  EXPECT_EQ(key_rel_ev.GetType(), EvType::KEY_RELEASE);
}

TEST(Event, KeyReleaseDataDispatcher)
{
  Event key_rel_ev{ EvType::KEY_RELEASE, KeyCode::B };

  key_rel_ev //
    .When(EvType::KEY_RELEASE)
    .ThenWithRes(
      [](const EvData& ev)
      {
        const auto& key = *std::get_if<KeyCode>(&ev);
        return key == KeyCode::B;
      });
  EXPECT_TRUE(key_rel_ev.IsHandled());
}

TEST(Event, MouseButtonPressData)
{
  Event mouse_press{ EvType::MOUSE_BUTTON_PRESSED, KeyCode::MOUSE_BT_LEFT };
  EXPECT_EQ(mouse_press.GetType(), EvType::MOUSE_BUTTON_PRESSED);
}

TEST(Event, MouseButtonPressDataDispatcher)
{
  Event mouse_press{ EvType::MOUSE_BUTTON_PRESSED, KeyCode::MOUSE_BT_LEFT };

  mouse_press //
    .When(EvType::MOUSE_BUTTON_PRESSED)
    .ThenWithRes(
      [](const EvData& ev)
      {
        const auto& key = *std::get_if<KeyCode>(&ev);
        return key == KeyCode::MOUSE_BT_LEFT;
      });
  EXPECT_TRUE(mouse_press.IsHandled());
}

TEST(Event, MouseButtonReleaseData)
{
  Event mouse_rel{ EvType::MOUSE_BUTTON_RELEASE, KeyCode::MOUSE_BT_RIGHT };
  EXPECT_EQ(mouse_rel.GetType(), EvType::MOUSE_BUTTON_RELEASE);
}

TEST(Event, MouseButtonRelDataDispatcher)
{
  Event mouse_rel{ EvType::MOUSE_BUTTON_RELEASE, KeyCode::MOUSE_BT_RIGHT };

  mouse_rel //
    .When(EvType::MOUSE_BUTTON_RELEASE)
    .ThenWithRes(
      [](const EvData& ev)
      {
        const auto& key = *std::get_if<KeyCode>(&ev);

        return key == KeyCode::MOUSE_BT_RIGHT;
      });
  EXPECT_TRUE(mouse_rel.IsHandled());
}

TEST(Event, MouseMoveData)
{
  Event mouse_move{ EvType::MOUSE_MOVE, std::make_pair(1.0f, 2.0f) };
  EXPECT_EQ(mouse_move.GetType(), EvType::MOUSE_MOVE);
}

TEST(Event, MouseMoveDataDispatcher)
{
  Event mouse_move{ EvType::MOUSE_MOVE, std::make_pair(1.0f, 2.0f) };

  mouse_move //
    .When(EvType::MOUSE_MOVE)
    .ThenWithRes(
      [](const EvData& ev)
      {
        const auto& [x, y] = *std::get_if<MousePairData>(&ev);
        return x > 0.0f && y > 0.0f;
      });
  EXPECT_TRUE(mouse_move.IsHandled());
}

TEST(Event, MouseScrollData)
{
  Event mouse_scroll{ EvType::MOUSE_SCROLL, std::make_pair(0.5f, -0.5f) };
  EXPECT_EQ(mouse_scroll.GetType(), EvType::MOUSE_SCROLL);
}

TEST(Event, MouseScrollDataDispatcher)
{
  Event mouse_scroll{ EvType::MOUSE_SCROLL, std::make_pair(0.5f, -0.5f) };

  mouse_scroll //
    .When(EvType::MOUSE_SCROLL)
    .ThenWithRes(
      [](const EvData& ev)
      {
        const auto& [x, y] = *std::get_if<MousePairData>(&ev);
        return x > 0.0f && y < 0.0f;
      });
  EXPECT_TRUE(mouse_scroll.IsHandled());
}
