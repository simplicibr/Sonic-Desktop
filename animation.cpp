#include "animation.h"

Animation::Animation(sf::Texture* texture, sf::Vector2f imageCount, float switchTime){
    this->imageCount = imageCount;
    this->switchTime = switchTime;
    totalTime = 0.0f;
    currentImage.x = 0;

    uvRect.width = texture->getSize().x/float(imageCount.x);
    uvRect.height = texture->getSize().y/float(imageCount.y);
}

void Animation::Update(float row, float deltaTime){
    currentImage.y = row;
    totalTime += deltaTime;

    if(totalTime >= switchTime){
        totalTime -= switchTime;
        currentImage.x++;

        if(currentImage.x >= imageCount.x){
            currentImage.x = 0;
        }
    }

    uvRect.left = currentImage.x * uvRect.width;
    uvRect.top = currentImage.y * uvRect.height;
}

bool Animation::isfinished(){
    return currentImage.x >= imageCount.x - 1;
}