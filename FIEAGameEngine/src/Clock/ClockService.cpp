#include "FIEAGameEngine/ClockService.h"
#include <chrono>
#include <cstdint>

namespace Fiea::Engine::Clock
{
    ClockService::ClockService() : _CurrentTime(GetSystemTimeInSeconds()){}

    uint32_t ClockService::Time()
    {
        return _CurrentTime;
    }

    uint32_t ClockService::Update(uint32_t newTime)
    {
        uint32_t previousTime = _CurrentTime;
        if (newTime == 0)
        {
            _CurrentTime = GetSystemTimeInSeconds();
        }
        else
        {
            _CurrentTime = newTime;
        }
        return previousTime;
    }

    uint32_t ClockService::GetSystemTimeInSeconds() {
        using namespace std::chrono;
        return static_cast<uint32_t>(
            duration_cast<seconds>(system_clock::now().time_since_epoch()).count());
    }
}