#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <numeric>

class FrameTimer {
public:
    FrameTimer() {
        start_time_ = Clock::now();
        last_frame_time_ = start_time_;
    }

    void StartFrame() {
        last_frame_time_ = Clock::now();
    }

    void EndFrame() {
        const TimePoint current_time = Clock::now();
        const Duration frame_duration = current_time - last_frame_time_;
        const double frame_time = frame_duration.count();
        frame_times_.push_back(frame_time);
        if (frame_times_.size() > max_frame_count_) {
            frame_times_.erase(frame_times_.begin());
        }
    }

    void Reset() {
        start_time_ = Clock::now();
        last_frame_time_ = start_time_;
        frame_times_.clear();
    }

    [[nodiscard]] double GetFPS() const {
        if (frame_times_.empty()) return 0.0;
        const double average_frame_time = GetAverageFrameTime();
        if (average_frame_time == 0.0) return 0.0;
        return 1.0 / average_frame_time;
    }

    [[nodiscard]] double GetAverageFrameTime() const {
        if (frame_times_.empty()) return 0.0;
        return std::accumulate(frame_times_.begin(), frame_times_.end(), 0.0) / static_cast<double>(frame_times_.size());
    }

    [[nodiscard]] double GetElapsedTime() const {
        const Duration elapsed_time = Clock::now() - start_time_;
        return elapsed_time.count();
    }

    void SetMaxFrameCount(const size_t max_frame_count) { max_frame_count_ = max_frame_count; }
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::duration<double>;

    TimePoint start_time_;
    TimePoint last_frame_time_;
    std::vector<double> frame_times_;
    size_t max_frame_count_ = 100;
};

#endif //TIME_H
