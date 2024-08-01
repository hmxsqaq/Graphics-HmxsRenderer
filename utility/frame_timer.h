#ifndef TIME_H
#define TIME_H

#include <chrono>

class FrameTimer {
public:
    FrameTimer();

    void StartFrame();
    void EndFrame();
    void Reset();

    [[nodiscard]] double GetFPS() const;
    [[nodiscard]] double GetAverageFrameTime() const;
    [[nodiscard]] double GetElapsedTime() const;

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
