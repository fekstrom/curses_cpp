#include "curses_cpp/curses.hpp"

#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include <iostream>

class TestRunListener : public Catch::EventListenerBase
{
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(Catch::TestRunInfo const&) override
    {
        std::cout << "Built with Curses version: " << curses::CursesVersion() << "\n";
    }
};
CATCH_REGISTER_LISTENER(TestRunListener)
