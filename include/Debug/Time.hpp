#ifndef PULSAR_TIME_INCLUDED
#define PULSAR_TIME_INCLUDED

#include "setup.hpp"
using Pulsar::Decimal;

namespace Time {
    struct Clock {
        struct Result {
            Decimal asMilli;
            Decimal asSec;
        };

        std::chrono::steady_clock::time_point time;

        Clock(): time(std::chrono::high_resolution_clock::now()) {

        }

        Decimal getSec() {
            auto t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> msm = t2 - time;
            return msm.count() / 1000.0;
        }

        Decimal getMilli() {
            auto t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> msm = t2 - time;
            return msm.count();
        }

        Decimal reset() {
            auto t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> msm = t2 - time;
            time = t2;
            return msm.count();
        }
    };
}


#endif