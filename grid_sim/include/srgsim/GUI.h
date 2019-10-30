#pragma once

#include "srgsim/world/World.h"
#include "srgsim/SRGEnums.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>

namespace srgsim
{
class GUI
{
public:
    GUI();
    ~GUI();
    void draw(srgsim::World* world);

private:
    sf::Sprite getSprite(Type type);
    void scaleSprite(const World* world);

    void handleSFMLEvents(const World* world);
    void calculateScale();
    void calculateSpriteSize(const World* world);

    uint32_t xResolution = 800;
    uint32_t yResolution = 800;
    uint32_t textureSize = 60;
    float scaleFactor = 0;
    float scaledSpriteSize = 0;

    sf::Sprite sprites[static_cast<int>(Type::Last)];
    sf::Texture* texture;
    sf::RenderWindow* window;

};
} // namespace srgsim