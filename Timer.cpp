#include <stdio.h>
#include "Timer.h"
#include "Utils.h"

Timer::Timer()
{
    start_time_ = 0;
    paused_time_ = 0;
    is_paused_ = false;
    is_started_ = false;
}

Timer::~Timer()
{

}

void Timer::start()
{
    is_started_ = true;
    is_paused_ = false;

    start_time_ = SDL_GetTicks(); 
}

void Timer::stop()
{
    is_paused_ = false;
    is_started_  = false;
}

void Timer::paused()
{
    if(is_started_ == true && is_paused_ == false) 
    {
        is_paused_ = true;
        paused_time_ = SDL_GetTicks() - start_time_;
    }
}

void Timer::unpaused()
{
    if(is_paused_ == true) 
    {
        is_paused_ = false;
        start_time_ = SDL_GetTicks() - paused_time_;
        paused_time_ = 0;
    }
}

int Timer::get_time()
{
    if(is_started_ == true) 
    {
        if(is_paused_ == true) 
        {
            return paused_time_;
        }
        else 
        {
            return SDL_GetTicks() - start_time_;
        }
    }
    return 0;
}

bool Timer::is_started()
{
    return is_started_;
}

bool Timer::is_paused()
{
    return is_paused_;
}