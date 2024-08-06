#pragma once
#include "bound.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Constantable : public Bound, public sf::Drawable {};