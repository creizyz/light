#include "core/utils/timer.hpp"

using second_t = std::chrono::duration<double, std::ratio<1>>;

Timer::Timer()
  : beg(clock_t::now())
{ }

void Timer::reset()
{
  beg = clock_t::now();
}

double Timer::elapsed() const
{
  return std::chrono::duration_cast<second_t>(clock_t::now() - beg).count();
}