#pragma once
#include "FIEAGameEngine/Types.h"
#include "FIEAGameEngine/ServiceMgr.h"

namespace Fiea::Engine::Clock
{
    struct ITimeProvider
    {
        FIEA_SERVICE_INTERFACE
        virtual uint32_t Time() = 0;
    };

    struct ITimeUpdater : public ITimeProvider
    {
        FIEA_SERVICE_INTERFACE
            virtual uint32_t Update(uint32_t newTime = 0) = 0;
    };

    class ClockService : public ITimeUpdater
    {
    private:
        FIEA_SERVICE_BIND2(ClockService, ITimeProvider, ITimeUpdater)

        uint32_t _CurrentTime;

    public:
        ClockService();
        static uint32_t GetSystemTimeInSeconds();

        virtual uint32_t Time() override;
        virtual uint32_t Update(uint32_t newTime = 0) override;
    };
}