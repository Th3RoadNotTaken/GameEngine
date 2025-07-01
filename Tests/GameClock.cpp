#include "GameClock.h"

using namespace std::chrono;

namespace Fiea::Engine
{
    GameClock::GameClock(NowFunc now) : _Now(now)
    {
        // if this was not overridden in the member initialization, use the provided
        //  system clock, by default (typical case)
        if (_Now == nullptr) {
            _Now = high_resolution_clock::now;
        }
        assert(_Now != nullptr);

        _StartTime = _Now();
    }

    GameTime GameClock::Current() const {
        GameTime time;
        time._Start = time._Start = _StartTime;
        time._Current = _Now();
        return time;
    }

    GameTime::Millis GameClock::Elapsed(const GameTime& time) const
    {
        return duration_cast<milliseconds>(_Now() - time._Last).count();
    }

    void GameClock::Update(GameTime& time) const
    {
        time._Last = time._Current;
        time._Current = _Now();
    }
}