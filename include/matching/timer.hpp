#pragma once

#include <chrono>

namespace matching {

class Timer {
private:

    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint t1_{};
    TimePoint t2_{};
    double delta_{0.0};   // Duración del último intervalo (segundos)
    double elapsed_{0.0}; // Tiempo acumulado total (segundos)

public:
    Timer() = default;

    // Reinicia el acumulador de tiempo
    void init() noexcept {
        delta_ = 0.0;
        elapsed_ = 0.0;
    }

    // Marca el inicio del reloj
    void startclock() noexcept {
        t1_ = Clock::now();
    }

    // Detiene el reloj y calcula la diferencia de tiempo
    void stopclock() noexcept {
        t2_ = Clock::now();
        const std::chrono::duration<double> duration = t2_ - t1_;
        delta_ = duration.count();
    }

    // Detiene el reloj y suma el intervalo al total acumulado
    void increment() noexcept {
        stopclock();
        elapsed_ += delta_;
    }

    [[nodiscard]] double get_delta() const noexcept { return delta_; }
    [[nodiscard]] double get_elapsed() const noexcept { return elapsed_; }
};

} // namespace matching