#include "Tests.h"
#include "FieaGameEngine/Timer.h"
#include "GameClock.h"
#include <functional>
#include <algorithm>
#include <thread>
#include <chrono>
#include <future>

#include <iostream>
#include <sstream>

//#include <cvmarkersobj.h>

namespace Fiea::Engine::Tests
{
    using namespace std::chrono_literals;
    //using namespace Concurrency::diagnostic;

    static void Wait(std::chrono::high_resolution_clock::duration dur)
    {
        auto time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::high_resolution_clock::now() - time;

        while (elapsed < dur)
        {
            elapsed = std::chrono::high_resolution_clock::now() - time;
        }
    }

    class Obj
    {
        static inline size_t _NextId = 1;

        size_t _Id{};
        size_t _Frame{};

    public:
        Obj() : _Id(_NextId++) {}

        void Update(GameTime::Millis millis, std::ostream& output)
        {
            output << "Obj #" << _Id << ": Updated #" << ++_Frame << std::endl;
            Wait(2ms);
        }

        void Render(std::ostream& output) const
        {
            output << "Obj #" << _Id << ": Rendering #" << _Frame << std::endl;
            Wait(1ms);
        }
    };

    TEST_CLASS(AsyncTest)
    {
    public:
        using timepoint = std::chrono::steady_clock::time_point;

        static const GameTime::Millis TARGET_MILLIS = 17;

        static const GameTime::Millis GAMELOOP_MILLIS = 33;
        static const GameTime::Millis RENDER_MILLIS = 17;
        static const GameTime::Millis DURATION_MILLIS = 10000;
        
        static void UpdateAll(std::vector<Obj>& objects, const GameTime& time, std::ostream& output)
        {
            output << std::endl << "Beginning Object Update @ " << time.Game() << "ms" << std::endl;
            std::for_each(objects.begin(), objects.end(), [&time, &output](auto& obj) { obj.Update(time.Frame(), output); });
        }

        static void GameLoop(std::vector<Obj>& objects, const GameClock& clock, std::future<bool>& doneReady)
        {
            GameTime time = clock.Current();

            // loop until signaled done
            while (doneReady.wait_for(0ms) != std::future_status::ready)
            {
                std::ostringstream output;
                while (clock.Elapsed(time) < GAMELOOP_MILLIS)
                {
                    std::this_thread::sleep_for(0ms);
                }
                clock.Update(time);

                UpdateAll(objects, time, output);
                LOG(output.str());
            }
        }

        static void RenderAll(const std::vector<Obj>& objects, std::ostream& output)
        {
            output << std::endl << "Beginning Object Render" << std::endl;
            std::for_each(objects.begin(), objects.end(), [&output](auto& obj) { obj.Render(output); });
        }

        static void RenderLoop(std::vector<Obj>& objects, const GameClock& clock, std::future<bool>& doneReady)
        {
            GameTime time = clock.Current();

            // loop until signaled done
            while (doneReady.wait_for(0ms) != std::future_status::ready)
            {
                std::ostringstream output;
                while (clock.Elapsed(time) < RENDER_MILLIS)
                {
                    // Sleep for 0 means that it will give up its thread to hardware and wait until it can get another free thread
                    std::this_thread::sleep_for(0ms);
                }
                clock.Update(time);

                RenderAll(objects, output);
                LOG(output.str());
            }
        }

        TEST_METHOD(Game)
        {
            timepoint now = std::chrono::steady_clock::now();
            const GameClock clock = GameClock([&now]() { return now; });

            const size_t NUM_OBJECTS = 15;
            std::vector<Obj> objects(NUM_OBJECTS);


            GameTime time = clock.Current();
            while (time.Game() < DURATION_MILLIS)
            {
                std::ostringstream updateOutput;
                std::ostringstream renderOutput;

                while (clock.Elapsed(time) < TARGET_MILLIS)
                {
                    now += 1ms;
                    //std::this_thread::sleep_for(0ms);
                }
                clock.Update(time);

                UpdateAll(objects, time, updateOutput);
                RenderAll(objects, renderOutput);

                LOG(updateOutput.str());
                LOG(renderOutput.str());
            }
        }

        TEST_METHOD(Threads)
        {
            timepoint now = std::chrono::steady_clock::now();
            const GameClock clock = GameClock([&now]() { return now; });

            const size_t NUM_OBJECTS = 15;
            std::vector<Obj> objects(NUM_OBJECTS);

            // create a facility for reporting done-ness
            std::promise<bool> done; // Promises that it will be set in the future but isnt now
            std::future doneReady = done.get_future(); // Tells us when done is populated

            // start two threads
            std::thread gameThread(GameLoop, std::ref(objects), std::ref(clock), std::ref(doneReady)); // We need to provide references to these objects or else it makes a copy by default which might be undesirable
            std::thread renderThread(RenderLoop, std::ref(objects), std::ref(clock), std::ref(doneReady));

            GameTime time = clock.Current();
            while (time.Game() < DURATION_MILLIS)
            {
                // bump the debug clock
                now += 100ns;
                clock.Update(time);

                // actually sleep so other threads can execute
                //std::this_thread::sleep_for(10ns);
            }
            
            // report done-ness
            done.set_value(true);

            // join threads to wait for safe exit
            gameThread.join();
            renderThread.join();
        }
    };
}