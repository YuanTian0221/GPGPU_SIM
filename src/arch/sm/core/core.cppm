// src/arch/sm/core
export module arch.sm.core;

import <memory>;
import <string>;
import <iostream>;

export namespace gpgpu::arch::sm::core {

// 硬件核心状态
enum class CoreState : uint8_t { // 节省内存
    IDLE,
    EXECUTING,
    STALLED,
    FINISHED
};

// 抽象基类：所有核心的通用接口
class CoreBase {
    public:
        virtual ~CoreBase() = default;
    
        // 纯虚函数：执行一个时钟周期
        virtual void cycle() = 0;
    
        // 虚函数：核心类型标识（可被子类覆盖）
        virtual std::string name() const { 
            return "BaseCore"; 
        }
    
        // 公共工具函数
        void log(const std::string& msg) const {
            std::cout << "[" << name() << "] " << msg << "\n";
        }
    
    protected:
        CoreState state_ = CoreState::IDLE;
        uint64_t cycles_ = 0; // 已执行周期数
    };

} // namespace gpgpu::arch::sm::core
