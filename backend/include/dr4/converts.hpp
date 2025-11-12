#pragma once

#include "dr4/keycodes.hpp"
#include "dr4/mouse_buttons.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace dr4 {
namespace impl {
namespace detail {

sf::Mouse::Button
toSFML( dr4::MouseButtonType button );

dr4::MouseButtonType
fromSFML( sf::Mouse::Button button );

dr4::KeyMode
fromSFML( sf::Event::KeyEvent event );

dr4::KeyCode
fromSFML( sf::Keyboard::Key code );

} // namespace detail
} // namespace impl
} // namespace dr4
