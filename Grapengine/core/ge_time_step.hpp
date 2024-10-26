#ifndef GE_TIME_STEP_HPP
#define GE_TIME_STEP_HPP

namespace GE
{
  class TimeStep
  {
  public:
    explicit TimeStep(const u64 ns) : m_time_ns(ns) {}

    [[nodiscard]] f64 Secs() const
    {
      return static_cast<f64>(m_time_ns) * 1e-9;
    }

    [[nodiscard]] u64 u() const { return m_time_ns; }

  private:
    u64 m_time_ns;
  };
}

#endif // GE_TIME_STEP_HPP
