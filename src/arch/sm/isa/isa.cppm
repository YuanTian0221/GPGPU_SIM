export module arch.sm.isa;

import <cstdint>;
import <vector>;
import <string_view>;

export namespace gpgpu::arch::sm::isa {
    // 指令操作码枚举
    enum class OpCode : uint8_t {
        MOV = 0x10,
        ADD = 0x20,
        MUL = 0x30,
        LD  = 0x40,
        ST  = 0x50,
        BRA = 0x60,
        // ... 其他操作码
    };

    // 指令操作数类型
    struct Operand {
        enum class Type { REG, IMM, MEM, PRED } type;
        uint32_t value;
        std::string_view name;  // 调试用符号名
    };

    // 中间表示(IR)指令
    class IRInstruction {
    public:
        IRInstruction(OpCode op, std::initializer_list<Operand> operands);
        
        // 符合InstructionType概念的方法
        void decode();
        bool execute();
        bool valid() const;
        
        // ISA特有方法
        OpCode opcode() const noexcept { return op_; }
        const auto& operands() const noexcept { return operands_; }
        uint32_t latency() const noexcept { return latency_; }

    private:
        OpCode op_;
        std::vector<Operand> operands_;
        uint32_t latency_ = 1;
        bool decoded_ = false;
    };

    // 寄存器别名系统
    struct RegisterAlias {
        uint32_t physical_reg;
        std::string_view name;
    };

    // 符合RegisterFileType概念的寄存器文件
    class RegisterFile {
    public:
        explicit RegisterFile(size_t size);
        
        // 必须实现的方法 (RegisterFileType概念)
        uint32_t read(uint32_t reg) const;
        void write(uint32_t reg, uint32_t value);
        size_t size() const noexcept { return regs_.size(); }
        
        // ISA扩展方法
        uint32_t read_by_name(std::string_view name) const;
        void bind_alias(std::string_view name, uint32_t phys_reg);

    private:
        std::vector<uint32_t> regs_;
        std::vector<RegisterAlias> aliases_;
    };

    // 指令编码器
    class InstructionEncoder {
    public:
        std::vector<uint8_t> encode(const IRInstruction& instr) const;
        IRInstruction decode(std::span<const uint8_t> bytes) const;
    };

    // 指令集特性查询
    struct ISAProperties {
        uint32_t warp_size;
        uint32_t max_registers;
        bool supports_fp64;
    };

    ISAProperties get_current_isa_properties() noexcept;
}
