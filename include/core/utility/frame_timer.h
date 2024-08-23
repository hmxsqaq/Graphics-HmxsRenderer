#ifndef TIME_H
#define TIME_H

#include <chrono>

class FrameTimer {
public:
    FrameTimer() : start_time_(Clock::now()), last_frame_time_(start_time_){
    }

    void Tick() {
        const TimePoint current_time = Clock::now();
        delta_time_ = current_time - last_frame_time_;
        last_frame_time_ = current_time;
        elapsed_time_ += delta_time_.count();
        frame_count_++;
        if (elapsed_time_ >= 1.0) {
            fps_ = frame_count_ / elapsed_time_;
            frame_count_ = 0;
            elapsed_time_ = 0.0;
        }
    }

    [[nodiscard]] double fps() const { return fps_; }
    [[nodiscard]] double delta_time() const { return delta_time_.count(); }
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::duration<double>;

    TimePoint start_time_;
    TimePoint last_frame_time_;
    Duration delta_time_ = Duration::zero();
    int frame_count_ = 0;
    double elapsed_time_ = 0;
    double fps_ = 0;
};

#endif //TIME_H
