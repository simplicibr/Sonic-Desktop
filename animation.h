#include<SFML/Graphics.hpp>

class Animation{
    public:
        Animation(sf::Texture* texture, sf::Vector2f imageCount, float switchTime);

        void Update(float row, float deltaTime);
        bool isfinished();

    public:
        sf::IntRect uvRect;

    private:
        sf::Vector2f imageCount;
        sf::Vector2f currentImage;

        float totalTime;
        float switchTime;
};