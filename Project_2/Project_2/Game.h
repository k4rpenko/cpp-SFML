#pragma once
#include  <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include <fstream>
#include "Timer.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
//Імпортуємо все для гри

using namespace std;


class Game
{
private:
    //Рендеримо вікно для гри
    sf::VideoMode winSize = {1280, 720, };
    sf::String winTitle = "Games";
    sf::RenderWindow win;
    sf::Image Icons;


    //Player
    //Анімації
    vector<sf::IntRect> animsFoxR;
    vector<sf::IntRect> animsFoxL;
    vector<sf::IntRect> animsFoxD;
    vector<sf::IntRect> animsFoxU;

    //Музика при хотьбі
    sf::SoundBuffer StringSoundBuffer; //Загрузка
    sf::Sound StringSound; //Витворення
    
    
    //Музика для Коіна
    sf::SoundBuffer StringSoundBufferCoins; //Загрузка
    sf::Sound StringSoundCoins; //Витворення 
    
    //Час анімації
    sf::Time foxFrameInter = sf::seconds(0.2);
    sf::Clock foxAniter;
    //Текстура і hitbox я його так називаю як з Unity 
    sf::Texture FoxTex;
    sf::Sprite FoxSprite;

    //Функції
    bool ShowFox = true; //Виведення ігрока
    int FrameIndex = 0; //Анімація ігрока
    int Money = 0; // Кількість монет
    //Спавн
    float spawnX = 50.0f; 
    float spawnY = 660.0f;
    //Сила гравітації або швидкість спуску у низ
    const float GRAVITY = 1000.0f;
    float FoxVelocityY = 0.0f;

    //Перевірка чи ігрок на землі і його швидкість
    bool isOnGround = false;
    float FoxSpead = 300;

    //Для рестарту гри
    sf::Clock restartClock;
    bool isRestarting = false;
    
    //Any
    sf::Texture Mono; 
    sf::Texture ImgMap;
    
    sf::Text* CoinsText;
    sf::Font font;

    sf::Text* TimerText;
    sf::Text* EndText;

    sf::Timer Timer;

    sf::RectangleShape boundaryTop;
    sf::RectangleShape boundaryLeft;
    sf::RectangleShape boundaryRight;
    vector<sf::RectangleShape> otherShapes;
    sf::Texture Coin;
    sf::Sprite CoinSprite;
    
    void HandleInput();
    void UpDate(float deltaTime);
    void Draw();
    void CheckCollisions();
    bool IsValidCoinPosition(const sf::Vector2f& position) const;
    void Jump();


public:
    Game();
    void Run();
};
