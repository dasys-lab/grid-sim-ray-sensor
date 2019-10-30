#include "srgsim/GUI.h"

#include "srgsim/SRGEnums.h"
#include "srgsim/world/Cell.h"
#include "srgsim/world/Door.h"
#include "srgsim/world/Object.h"
#include "srgsim/world/ServiceRobot.h"

#include <FileSystem.h>
#include <iostream>

//#define GUI_DEBUG

namespace srgsim
{
GUI::GUI()
{
    std::string textureFile = essentials::FileSystem::getSelfPath() + "/textures/textures.png";
    std::cout << "[GUI] Info: loading textureFile '" << textureFile << "'" << std::endl;
    this->texture = new sf::Texture();
    if (!this->texture->loadFromFile(textureFile)) {
        std::cerr << "[GUI] Coudn't load the texture file " << textureFile << std::endl;
    }
    this->texture->setSmooth(true);
    this->texture->setRepeated(true);
    this->window = new sf::RenderWindow(sf::VideoMode(xResolution, yResolution), "Grid Simulator GUI");
    this->window->setActive(false);
    for (int i = 0; i != static_cast<int>(Type::Last); i++) {
        sf::Sprite sprite;
        sprite.setTexture(*this->texture);
        Type type = static_cast<Type>(i);
        switch (type) {
        case Type::Wall:
            sprite.setTextureRect(sf::IntRect(textureSize * 2, 0, textureSize, textureSize));
            break;
        case Type::DoorOpen:
            sprite.setTextureRect(sf::IntRect(textureSize * 3, 0, textureSize, textureSize));
            break;
        case Type::DoorClosed:
            sprite.setTextureRect(sf::IntRect(textureSize * 3, textureSize, textureSize, textureSize));
            break;
        case Type::Floor:
            sprite.setTextureRect(sf::IntRect(textureSize * 2, textureSize, textureSize, textureSize));
            break;
        case Type::Unknown:
            sprite.setTextureRect(sf::IntRect(textureSize, textureSize, textureSize, textureSize));
            break;
        case Type::Robot:
            sprite.setTextureRect(sf::IntRect(0, textureSize * 2, textureSize, textureSize));
            break;
        case Type::CupBlue:
            sprite.setTextureRect(sf::IntRect(textureSize, textureSize * 3, textureSize, textureSize));
            break;
        case Type::CupRed:
            sprite.setTextureRect(sf::IntRect(0, textureSize * 3, textureSize, textureSize));
            break;
        case Type::CupYellow:
            sprite.setTextureRect(sf::IntRect(textureSize * 2, textureSize * 3, textureSize, textureSize));
            break;
        default:
            sprite.setTextureRect(sf::IntRect(0, 0, textureSize, textureSize));
            std::cout << "[GUI] Unknown cell type " << static_cast<int>(type) << std::endl;
        }
        sprites[static_cast<int>(type)] = sprite;
    }
}

GUI::~GUI()
{
    delete this->window;
    delete this->texture;
}

void GUI::draw(World* world)
{
    this->window->setActive(true);

    handleSFMLEvents(world);

    this->window->clear();

    {
        std::recursive_mutex& dataMutex = world->getDataMutex();
        std::lock_guard<std::recursive_mutex> guard(dataMutex);
        for (auto &pair : world->getGrid()) {
            // background sprite
            sf::Sprite sprite = getSprite(pair.second->type);
            sprite.setPosition(pair.second->coordinate.x * scaledSpriteSize,
                               pair.second->coordinate.y * scaledSpriteSize);
            this->window->draw(sprite);

            // object sprites
            for (Object *object : pair.second->getObjects()) {
                sf::Sprite sprite;
                switch (object->getType()) {
                    case Type::Door:
                        if (static_cast<class Door *>(object)->isOpen()) {
                            sprite = getSprite(Type::DoorOpen);
                        } else {
                            sprite = getSprite(Type::DoorClosed);
                        }
                        break;
                    default:
                        sprite = getSprite(object->getType());
                }
                sprite.setPosition(object->getCell()->coordinate.x * scaledSpriteSize,
                                   object->getCell()->coordinate.y * scaledSpriteSize);
                this->window->draw(sprite);

                if (ServiceRobot *robot = dynamic_cast<ServiceRobot *>(object)) {
                    if (Object *carriedObject = robot->getCarriedObject()) {
                        sprite = getSprite(carriedObject->getType());
                        sprite.setPosition((robot->getCell()->coordinate.x * scaledSpriteSize) + scaledSpriteSize / 2,
                                           (robot->getCell()->coordinate.y * scaledSpriteSize) + scaledSpriteSize / 2);
                        sprite.setScale(0.25, 0.25);
                        this->window->draw(sprite);
                    }
                }
#ifdef GUI_DEBUG
                std::cout << "GUI: Placing object of Type " << object->getType() << " at (" << object->getCell()->coordinate.x << ", "
                          << object->getCell()->coordinate.y << ")" << std::endl;
#endif
            }
        }

        // for debug purposes
        for (Perception perception : world->getMarkers()) {
            sf::Sprite sprite;
            sprite = getSprite(perception.type);
            sprite.setPosition((perception.x * scaledSpriteSize) + scaledSpriteSize / 4,
                               (perception.y * scaledSpriteSize) + scaledSpriteSize / 4);
            sprite.setScale(0.25, 0.25);
            this->window->draw(sprite);
        }
        world->getMarkers().clear();
    }
    this->window->display();
}

void GUI::handleSFMLEvents(const World* world)
{
    sf::Event event;

    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        } else if (event.type == sf::Event::Resized) {
            window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            scaleSprite(world);
        }
    }
}

void GUI::scaleSprite(const World* world)
{
    calculateSpriteSize(world);
    calculateScale();
    for (auto& sprite : sprites) {
        sprite.setScale(scaleFactor, scaleFactor);
    }
}

void GUI::calculateScale()
{
    scaleFactor = scaledSpriteSize / float(textureSize);
}

void GUI::calculateSpriteSize(const World* world)
{
    auto sizeX = float(window->getSize().x) / float(world->getSizeX());
    auto sizeY = float(window->getSize().y) / float(world->getSizeY());

    if (sizeX < sizeY) {
        scaledSpriteSize = sizeX;
    } else {
        scaledSpriteSize = sizeY;
    }
}

sf::Sprite GUI::getSprite(Type type)
{
    return sprites[static_cast<int>(type)];
}
} // namespace srgsim
