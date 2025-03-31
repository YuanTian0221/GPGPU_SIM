// src/arch/sm/clock
export module arch.sm.clock;

import arch.sm.clock;
import <vector>;
import <memory>;

export namespace gpgpu::arch::sm::clock {

class Clock {
public:
    // 注册核心
    void add_core(std::shared_ptr<Core> core) {
        cores_.push_back(core);
    }

    // 运行指定周期数
    void tick(uint64_t cycles = 1) {
        for (uint64_t i = 0; i < cycles; ++i) {
            for (auto& core : cores_) {
                core->cycle();
            }
            ++global_cycles_;
        }
    }

    uint64_t global_cycles() const { return global_cycles_; }

private:
    std::vector<std::shared_ptr<Core>> cores_;
    uint64_t global_cycles_ = 0;
};

} // namespace gpgpu::arch::sm::clock