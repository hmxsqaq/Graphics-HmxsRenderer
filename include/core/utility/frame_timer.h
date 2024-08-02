#ifndef TIME_H
#define TIME_H

#include <chrono>

class FrameTimer {
public:
    FrameTimer() : frame_count_(0), elapsed_time_(0.0), fps_(0.0) {
        start_time_ = Clock::now();
        last_frame_time_ = start_time_;
    }

    void Tick() {
        const TimePoint current_time = Clock::now();
        const Duration delta_time = current_time - last_frame_time_;
        last_frame_time_ = current_time;
        elapsed_time_ += delta_time.count();
        frame_count_++;
        if (elapsed_time_ >= 1.0) {
            fps_ = frame_count_ / elapsed_time_;
            frame_count_ = 0;
            elapsed_time_ = 0.0;
        }
    }

    [[nodiscard]] double fps() const { return fps_; }

private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::duration<double>;

    TimePoint start_time_;
    TimePoint last_frame_time_;
    int frame_count_;
    double elapsed_time_;
    double fps_;
};

#endif //TIME_H
