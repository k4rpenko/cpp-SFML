#pragma once
#include <SFML/System.hpp>
using namespace std;
namespace sf
{
    //Я писав його ще на початку в сінчі напевно, з блоганом
    class Timer
    {
    private:
        sf::Time duration;
        sf::Time elapsedTime;
        sf::Clock clock;
        bool isClockRunning;
    public:
        Timer();

        void setDuration(sf::Time time);
        void start();
        void stop();
        void restart();
        bool isRunning() const;
        bool isExpired() const;
        sf::Time getElapsedTime() const;
        sf::Time getRemainingTime() const;
    };
}
