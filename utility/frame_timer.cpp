#include "frame_timer.h"
#include <numeric>

FrameTimer::FrameTimer() {
    start_time_ = Clock::now();
    last_frame_time_ = start_time_;
}

void FrameTimer::StartFrame() {
    last_frame_time_ = Clock::now();
}

void FrameTimer::EndFrame() {
    const TimePoint current_time = Clock::now();
    const Duration frame_duration = current_time - last_frame_time_;
    const double frame_time = frame_duration.count();
    frame_times_.push_back(frame_time);
    if (frame_times_.size() > max_frame_count_) {
        frame_times_.erase(frame_times_.begin());
    }
}

void FrameTimer::Reset() {
    start_time_ = Clock::now();
    last_frame_time_ = start_time_;
    frame_times_.clear();
}

double FrameTimer::GetFPS() const {
    if (frame_times_.empty()) return 0.0;
    const double average_frame_time = GetAverageFrameTime();
    if (average_frame_time == 0.0) return 0.0;
    return 1.0 / average_frame_time;
}

double FrameTimer::GetAverageFrameTime() const {
    if (frame_times_.empty()) return 0.0;
    return std::accumulate(frame_times_.begin(), frame_times_.end(), 0.0) / static_cast<double>(frame_times_.size());
}

double FrameTimer::GetElapsedTime() const {
    const Duration elapsed_time = Clock::now() - start_time_;
    return elapsed_time.count();
}
