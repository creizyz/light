#pragma once

#include <chrono>

enum class precision_t
{
  milliseconds = 1000,
  microseconds = 1000000,
  nanoseconds  = 1000000000
};

/**
 * @brief Simple timer class used for benchmarking
 */
class Timer
{
public:

  /**
   * @brief Constructs a new Timer object
   */
  Timer();

  /**
   * @brief Resets start time
   */
  void reset();

  /**
   * @brief Returns elapsed time since timer
   *        creation or last reset.
   * 
   * @return double elapsed time in seconds
   */
  double elapsed() const;

private:
  using clock_t = std::chrono::high_resolution_clock;
  std::chrono::time_point<clock_t> beg;
};

/**
 * @brief Benchmark the execution time of
 *        a function over multiple executions.
 * 
 * @param function function to benchmark
 * @param precision time precision required
 * @param n number of executions for the function
 * @return long elapsed time in the precision required
 */
template <typename Function>
long benchmark(Function function, const precision_t precision, const size_t n)
{
  Timer timer;
  double elapsed = 0;
  for (size_t i = 0; i < n; i++)
  {
    timer.reset();
    function();
    elapsed += timer.elapsed();
  }
  return (elapsed * (long)precision) / (long) n;
}