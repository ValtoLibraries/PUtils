#pragma once

#include <unordered_map>
#include <chrono>
#include <memory>
#include "SFML/Graphics.hpp"
#include "ViewItem.hpp"

namespace pse {
    //
    // Sprite class for SFML
    //
    class Sprite : public ViewItem {
        // Constructor
    public:
        Sprite(const std::string & texture, const sf::Vector2f & pos, const sf::Vector2f & size);

        // Destructor
    public:
        virtual ~Sprite() noexcept {}

        // ViewItem functions
    public:
        std::unique_ptr<ViewItem> copy() const noexcept override;

        void draw(sf::RenderWindow & window) noexcept override { window.draw(_sprite); }

        sf::Vector2f getSize() const noexcept override {
            return sf::Vector2f(_sprite.getLocalBounds().width, _sprite.getLocalBounds().height);
        }

        const sf::Drawable & getDrawable() noexcept override { return _sprite; }

        sf::Transformable & getTransformable() noexcept override { return _sprite; }

        // Texture getters and setters
    public:
        virtual void setTexture(const std::string & texture);

        const std::string & getTextureFile() const noexcept { return _textureFile; }

        // Attributes
    protected:
        sf::Sprite _sprite;
        std::string _textureFile;
        sf::Vector2f _size;

    private:
        static std::unordered_map<std::string, sf::Texture> textures;

        // Coplien
    public:
        Sprite(const Sprite & other) = delete;

        Sprite operator=(const Sprite & other) = delete;
    };
}
