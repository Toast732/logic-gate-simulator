#include "ticker.hpp"

#ifndef TICKER_CPP
#define TICKER_CPP

using tick_time = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;

// used for a precise sleep function, without a constant high cpu usage.
// from https://blog.bearcats.nl/accurate-sleep-function/
inline static void preciseSleep(double seconds) {
    using namespace std;
    using namespace std::chrono;

    static double estimate = 5e-3;
    static double mean = 5e-3;
    static double m2 = 0;
    static int64_t count = 1;

    while (seconds > estimate) {
        auto start = high_resolution_clock::now();
        this_thread::sleep_for(milliseconds(1));
        auto end = high_resolution_clock::now();

        double observed = (end - start).count() / 1e9;
        seconds -= observed;

        ++count;
        double delta = observed - mean;
        mean += delta / count;
        m2 += delta * (observed - mean);
        double stddev = sqrt(m2 / (count - 1));
        estimate = mean + stddev;
    }

    // spin lock
    auto start = high_resolution_clock::now();
    auto spinNs = int64_t(seconds * 1e9);
    auto delay = nanoseconds(spinNs);
        while (high_resolution_clock::now() - start < delay);
}

class Ticker {
public:
    double tps = 0.0f;
    Ticker() {};

    Ticker(double tps) {
        this->tps = tps;
        this->seconds_per_tick = 1 / tps;
    }

    // start ticking.
    void start() {

        // if we're already running.
        if (this->running) {
            return;
        }

        this->running = true;
        std::jthread new_thread(&Ticker::startTicker, this);

        this->ticker_thread = std::move(new_thread);
    }

    void stop() {
        this->running = false;
        this->ticker_thread.join();
    }

    // add a function to be called each tick.
    void addTargetFunction(void (*target_function)()) {
		this->tick_functions.push_back(target_function);
	}

    double calculateTPS(int seconds_smoothing) {
        // get the current time
        double current_time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

        double total_μspt = 0;
        int ticks_averaged = 0;

        // add up the difference between each tick time to the average tps, if they're within the seconds_smoothing
        for (int i = 1; i < this->tick_times.size() - 1; i++) {
            if (current_time - this->tick_times[i] < seconds_smoothing * 1000000) {
                total_μspt += (this->tick_times[i] - this->tick_times[i - 1]);
                ticks_averaged++;
			}
		}

        // return 0 if no ticks were averaged.
        if (ticks_averaged == 0) {
            return 0;
        }

        // divide by the number of ticks to get the average μs
        double average_μspt = total_μspt / ticks_averaged;

        // convert from microseconds to seconds
        double average_spt = average_μspt / 1000000;

        // convert from seconds per tick to ticks per second

        double average_tps = 1 / average_spt;

        return average_tps;
    }
    private:
        std::jthread ticker_thread;
        double seconds_per_tick = 0.0f;
        bool running = false;
        std::vector<double> tick_times;

        // list of functions to call each tick.
        std::vector<void (*)()> tick_functions;

        void addTickTime() {
            this->tick_times.push_back(std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).time_since_epoch().count());
        }

        void startTicker() {

            // while the ticker is running
            if (this->tps != -1) {
                while (this->running) {

                    addTickTime();

                    // sleep for the amount of time needed to hit desired tps
                    preciseSleep(this->seconds_per_tick);

                    // call the functions
                    (*this->tick_functions[0])();
                }
            }
            else {

                while (this->running) {

                    addTickTime();

                    // call the functions
                    (*this->tick_functions[0])();
                }
            }
        }
};

#endif