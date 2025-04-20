#define SFML_STATIC
#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <cstdint>
#include "animation.h"
#include <string>

std::uint16_t WIN_WIDTH = sf::VideoMode::getDesktopMode().width;
std::uint16_t WIN_HEIGHT = sf::VideoMode::getDesktopMode().height;
std::map<std::string, sf::Texture> textures;

void loadTexture(std::string key, std::string filepath){
    textures[key].loadFromFile(filepath);
}

void enableTransparency(sf::RenderWindow& window){
    HWND hwnd = window.getSystemHandle(); //window handler
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(130, 151, 181), 0, LWA_COLORKEY);
}

sf::RectangleShape loadSonicRect(){
    sf::RectangleShape Sonic(sf::Vector2f(140.0f,140.0f));
    Sonic.setOrigin(140/2, 140/2);
    Sonic.setPosition(WIN_WIDTH-100, WIN_HEIGHT-70);

    return Sonic;
}

void flip(sf::RectangleShape& sprite, int isFlipped){
    if(isFlipped == 1){
        sprite.setScale(1.f, 1.f);
    }else{
        sprite.setScale(-1.f, 1.f);
    }
}

void playAnim(Animation& animation, sf::RectangleShape& Sonic, float deltaTime){
    animation.Update(0.0f, deltaTime);
    Sonic.setTextureRect(animation.uvRect);
}

void run(sf::RectangleShape& character, float deltaTime, float moveSPD){
    character.move({moveSPD*deltaTime, 0.f});
}

void getFaster(float& moveSPD, bool& isFaster, float maxSPD, float deltaTime){
    if(isFaster == false){
        isFaster = true;
        if(moveSPD > 0 && moveSPD < maxSPD){
            moveSPD += 100 * deltaTime;
        }else if(moveSPD < 0 && moveSPD > maxSPD*-1){
            moveSPD -= 100 * deltaTime;
        }
    }
}

void getSlower(float& moveSPD, bool isFaster){
    if(isFaster == true){
        isFaster = false;
        if(moveSPD > 400){
            moveSPD = 400;
        }else if(moveSPD < -400){
            moveSPD = -400;
        }
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Desktop Sonic", sf::Style::None);

    //window.setFramerateLimit(144);

    sf::Event event;
    enableTransparency(window);
    SetWindowPos(window.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    SetWindowLong(window.getSystemHandle(), GWL_EXSTYLE, 
    GetWindowLong(window.getSystemHandle(), GWL_EXSTYLE) | WS_EX_TRANSPARENT);

    sf::RectangleShape Sonic = loadSonicRect();
    
    loadTexture("sonic_idle_1", "./sprites/sonic_idle_1.png");
    loadTexture("sonic_idle_2", "./sprites/sonic_idle_2.png");
    loadTexture("sonic_run_1", "./sprites/sonic_run_1.png");
    loadTexture("sonic_speed_up", "./sprites/sonic_speed_up.png");
    loadTexture("sonic_full_speed", "./sprites/sonic_full_speed.png");

    Animation animationIdle1(&textures["sonic_idle_1"], sf::Vector2f(3.f, 1.f), 0.15f);
    Animation animationIdle2(&textures["sonic_idle_2"], sf::Vector2f(15.8f, 1.f), 0.15f);
    Animation animationRun1(&textures["sonic_run_1"], sf::Vector2f(8.f, 1.f), 0.15f);
    Animation animationSpeedUP(&textures["sonic_speed_up"], sf::Vector2f(8.f, 1.f), 0.07f);
    Animation animationFullSpeed(&textures["sonic_full_speed"], sf::Vector2f(4.f, 1.f), 0.03f);

    enum States {IDLING, IDLING_END, WALKING, RUNNING, JUMPING, SPEED_UP, FULL_SPEED};
    enum States state;

    float moveSPD = -400;
    float maxSPD = 2000;

    Sonic.setTexture(&textures["sonic_idle_2"]);

    Animation* currentAnimation = &animationIdle2;

    float deltaTime = 0.0f;

    sf::Clock clock;
    sf::Clock clockRun;
    int isFlipped = -1;
    bool isFaster = false;

    flip(Sonic, isFlipped);

    while(window.isOpen()){
        SetWindowPos(window.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        srand(time(0));
        
        deltaTime = clock.restart().asSeconds();

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed) window.close();
        }

        //correr a cada 10 segundos
        if(clockRun.getElapsedTime().asSeconds() >= (float)(rand()%10+2)){
            state = RUNNING;
        }
        if(clockRun.getElapsedTime().asSeconds() >= (float)(rand()%50+5)){
            state = IDLING;
            isFaster = true;
            getSlower(moveSPD, isFaster);
            //moveSPD *= -1;
            clockRun.restart();
        }
        //if(clockRun.getElapsedTime().asSeconds() >= 7.0f){
            isFaster = false;
            getFaster(moveSPD, isFaster, maxSPD, deltaTime);
            if(moveSPD > 0){
                if(moveSPD > 1200 && moveSPD < maxSPD){
                    state = SPEED_UP;
                }else if(moveSPD >= maxSPD){
                    state = FULL_SPEED;
                }

            }else if(moveSPD < 0){
                if(moveSPD < -1200 && moveSPD > maxSPD*-1){
                    state = SPEED_UP;
                }else if(moveSPD <= maxSPD*-1){
                    state = FULL_SPEED;
                }
                
            }
        //}

        //std::cout << Sonic.getPosition().x << std::endl;

        if(Sonic.getPosition().x < WIN_WIDTH-(WIN_WIDTH-100)){
            if(Sonic.getPosition().x < WIN_WIDTH-(WIN_WIDTH-101)){
                Sonic.setPosition(Sonic.getPosition().x + 2, Sonic.getPosition().y);
            }
            isFlipped *= -1;
            flip(Sonic, isFlipped);
            moveSPD *= -1;
        }else if(Sonic.getPosition().x > WIN_WIDTH-100){
            if(Sonic.getPosition().x > WIN_WIDTH-99){
                Sonic.setPosition(Sonic.getPosition().x - 2, Sonic.getPosition().y);
            }
            isFlipped *= -1;
            flip(Sonic, isFlipped);
            moveSPD *= -1;
        }
        
        switch(state){
            case IDLING:
                Sonic.setTexture(&textures["sonic_idle_2"]);
                currentAnimation = &animationIdle2;
                break;

            case IDLING_END:
                break;
            
            case WALKING:
                break;

            case RUNNING:
                run(Sonic, deltaTime, moveSPD);
                Sonic.setTexture(&textures["sonic_run_1"]);
                currentAnimation = &animationRun1;
                break;
            
            case SPEED_UP:
                run(Sonic, deltaTime, moveSPD);
                Sonic.setTexture(&textures["sonic_speed_up"]);
                currentAnimation = &animationSpeedUP;
                break;
            case FULL_SPEED:
                run(Sonic, deltaTime, moveSPD);
                Sonic.setTexture(&textures["sonic_full_speed"]);
                currentAnimation = &animationFullSpeed;
                break;

            default:
            break;
        }

        playAnim(*currentAnimation, Sonic, deltaTime);

        window.clear(sf::Color(130, 151, 181, 255));
        window.draw(Sonic);
        window.display();
    }
}