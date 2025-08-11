#include <stdint.h>
#include <stdio.h>

// 此例程仅用于测试M扩展中相关指令能否正确设置对应寄存器
// 两个辅助函数仅get*用于运行过程中保存指令运行结果

// csrr csr read，将csr的值中的值读入寄存器中，csrr rd csr，x[rd]=Csrs[csr]
// "=r"后面的变量代表输出操作数，汇编代码会向变量中写入值
// "r"后面的变量代表输入操作数，汇编代码会读取变量中的值
// %0，0是汇编代码后方变量的索引

#define MTYPE 0xc40
#define MTILEM 0xc41
#define MTILEN 0xc42
#define MTILEK 0xc43

void print_log(uint64_t *csr_data, uint64_t *reg_data) {
    printf("  CSR         = 0x%lx\n", *csr_data);
    printf("  reg_data    = 0x%lx\n", *reg_data);
}

void init_test(uint64_t *csr_data, uint64_t *reg_data) {
    if (csr_data != NULL) {
        *csr_data = 0;
    }
    if (reg_data != NULL) {
        *reg_data = 0;
    }
    printf("Init:\n");
    print_log(csr_data, reg_data);
}

void getMcsr(uint64_t *csr_data, uint64_t *reg_data, uint32_t csr_index) {
    switch (csr_index)
    {
        case MTYPE:
            // 读取Csrs[0xc40]中的值，写入csr_data变量中
            __asm__ volatile("csrr %0, 0xc40" : "=r"(*csr_data));
            break;

        case MTILEM:
            __asm__ volatile("csrr %0, 0xc41" : "=r"(*csr_data));
            break;
        
        case MTILEN:
            __asm__ volatile("csrr %0, 0xc42" : "=r"(*csr_data));
            break;
        
        case MTILEK:
            __asm__ volatile("csrr %0, 0xc43" : "=r"(*csr_data));
            break;
        
        default:
            break;
    }
    // 读取t0寄存器中的值，写入reg_data变量中
    __asm__ volatile("mv %0, t0" : "=r"(*reg_data));
    printf("%x:\n", csr_index);
    print_log(csr_data, reg_data);
}

int main() {
    uint64_t csr_init, reg_init, imm;

    init_test(&csr_init, &reg_init);
    getMcsr(&csr_init, &reg_init, MTYPE);
    // e16为 16bit element的缩写，表示矩阵中每个元素占用16比特
    // 根据指令的机器码0x0200c2f7 -> 24~15位是imm，得到e16为0_0000_0000_1
    // 内联汇编，第一个冒号后写输出操作数，第二个冒号写输入操作数，第三个冒号写被修改的寄存器（用于告知编译器该寄存器会被指令修改）
    imm = 0x12345678;
    __asm__ volatile("mv t1, %0" : : "r"(imm) : "t1");
    __asm__ volatile("msettype t0, t1");
    getMcsr(&csr_init, &reg_init, MTYPE);
    __asm__ volatile("msettypei t0, e16" : : : "t0");
    // if use riscv64-unknown-elf-gcc
    // __asm__ volatile("msettypei t0, 1" : : : "t0");
    getMcsr(&csr_init, &reg_init, MTYPE);

    // 0x021062f7 -> 0_0001_0000_0，ba对应mtype寄存器中的mba字段（1bit），此命令用于将mtype寄存器的mba值1
    __asm__ volatile("msettypehi t0, ba" : : : "t0");
    getMcsr(&csr_init, &reg_init, MTYPE);
    
    // 设置mtype中的msew字段，等价于msettypei x5, e16
    __asm__ volatile("msetsew t0, 1" : : : "t0");
    getMcsr(&csr_init, &reg_init, MTYPE);
    
    __asm__ volatile("msetint4 t0, 0" : : : "t0");
    getMcsr(&csr_init, &reg_init, MTYPE);
   
    // mtile*用于表示矩阵尺寸，包含mtilem，mtilen，mtilek，对于矩阵A*B=C，后缀m、n、k分别表示A_rows、A_cols\B_rows、B_cols
    // 每次测试新的CSR时，将临时变量csr_init与reg_init清空
    init_test(&csr_init, &reg_init);
    // 读取CSR的初始值到csr_init，读取t0寄存器中的值到reg_init
    getMcsr(&csr_init, &reg_init, MTILEM);
    imm = 0xA;
    // 将立即数赋值给t1寄存器
    __asm__ volatile("mv t1, %0" : : "r"(imm) : "t1");
    // 将t1寄存器（立即数）中的值赋值给CSR，并且CSR原本的值保存到t0寄存器
    __asm__ volatile("msettilem t0, t1");
    // 再次读取CSR的值
    getMcsr(&csr_init, &reg_init, MTILEM);
    __asm__ volatile("msettilemi t0, 0x3ff");
    getMcsr(&csr_init, &reg_init, MTILEM);
    
    init_test(&csr_init, &reg_init);
    getMcsr(&csr_init, &reg_init, MTILEN);
    imm = 0xffffffff;
    __asm__ volatile("mv t1, %0" : : "r"(imm) : "t1");
    __asm__ volatile("msettilen t0, t1");
    getMcsr(&csr_init, &reg_init, MTILEN);
    __asm__ volatile("msettileni t0, 0x3f0");
    getMcsr(&csr_init, &reg_init, MTILEM);
    
    init_test(&csr_init, &reg_init);
    getMcsr(&csr_init, &reg_init, MTILEK);
    imm = 0x11111111;
    __asm__ volatile("mv t1, %0" : : "r"(imm) : "t1");
    __asm__ volatile("msettilek t0, t1");
    getMcsr(&csr_init, &reg_init, MTILEK);
    __asm__ volatile("msettileki t0, 0x300");
    getMcsr(&csr_init, &reg_init, MTILEK);

    return 0;
}