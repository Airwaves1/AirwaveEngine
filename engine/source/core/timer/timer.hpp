#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cmath>
#include <numeric>

namespace Airwave {

class Timer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using Duration = Clock::duration;
    using TimePoint = Clock::time_point;

    struct Record {
        std::vector<Duration> durations;
        TimePoint start;

        void begin() { start = Clock::now(); }
        void end() { 
            auto end = Clock::now();
            durations.push_back(end - start); 
        }
        
        // 统计信息
        double average() const {
            if(durations.empty()) return 0;
            return total() / durations.size();
        }
        
        double total() const {
            return std::chrono::duration<double>(std::reduce(
                durations.begin(), durations.end(), Duration{}
            )).count();
        }
        
        double stddev() const {
            const double avg = average();
            double variance = 0;
            for(auto d : durations) {
                double diff = std::chrono::duration<double>(d).count() - avg;
                variance += diff * diff;
            }
            return std::sqrt(variance / durations.size());
        }
    };

    // 开始新记录
    static void BeginSession() { currentSession = {}; }

    // 开始测量阶段
    static void BeginStage(const std::string& name) {
        currentSession[name].begin();
    }

    // 结束测量阶段
    static void EndStage(const std::string& name) {
        currentSession[name].end();
    }

    // 输出详细报告
    static void PrintReport() {
        std::cout << "\n=== Performance Report ===\n";
        for(const auto& [name, record] : currentSession) {
            std::cout << "[" << name << "]\n"
                      << "  Samples: " << record.durations.size() << "\n"
                      << "  Total:   " << record.total() << "s | " << record.total() * 1000 << "ms\n"
                      << "  Avg:     " << record.average() << "s | " << record.average() * 1000 << "ms\n"
                      << "  StdDev:  " << record.stddev() << "s | " << record.stddev() * 1000 << "ms\n";
        }
    }

private:
    static inline std::unordered_map<std::string, Record> currentSession;
};

// 自动作用域计时器
class ScopeTimer {
public:
    ScopeTimer(const std::string& name) : stageName(name) {
        Timer::BeginStage(stageName);
    }

    ~ScopeTimer() {
        Timer::EndStage(stageName);
    }

private:
    std::string stageName;
};

} // namespace Airwave

#define PROFILE_SCOPE(name) \
    Airwave::ScopeTimer ANONYMOUS_VARIABLE(profile_scope_)(name)

#define ANONYMOUS_VARIABLE(base) \
    CONCAT(base, __LINE__)
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)