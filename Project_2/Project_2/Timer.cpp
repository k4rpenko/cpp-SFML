#include "Timer.h"

sf::Timer::Timer() :duration(sf::Time::Zero), elapsedTime(sf::Time::Zero), isClockRunning(false) {}

void sf::Timer::setDuration(sf::Time time)
{
    duration = time;
}

void sf::Timer::start()
{
    isClockRunning = true;
    clock.restart();
}

void sf::Timer::stop()
{
    isClockRunning = false;
    elapsedTime += clock.getElapsedTime();
}

void sf::Timer::restart()
{
    elapsedTime = sf::Time::Zero;
    clock.restart();
    isClockRunning = true;
}

bool sf::Timer::isRunning() const
{
    return isRunning();
}

bool sf::Timer::isExpired() const
{
    return elapsedTime  + clock.getElapsedTime() >= duration;
}

sf::Time sf::Timer::getElapsedTime() const
{
    return elapsedTime  + (isClockRunning ? clock.getElapsedTime() : sf::Time::Zero);
}

sf::Time sf::Timer::getRemainingTime() const
{
    return max(sf::Time::Zero, duration - getElapsedTime());
}