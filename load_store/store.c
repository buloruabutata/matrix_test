#include <riscv_matrix.h>
#include <stdint.h>
#include <stdio.h>

 void getMtype(uint64_t *csr_data, uint64_t *reg_data) {
    __asm__ volatile("csrr %0, 0xc40" : "=r"(*csr_data));
    __asm__ volatile("mv %0, x5" : "=r"(*reg_data));
 }
 void getMtilem(uint64_t *csr_data, uint64_t *reg_data) {
    __asm__ volatile("csrr %0, 0xC41" : "=r"(*csr_data));
    __asm__ volatile("mv %0, x4" : "=r"(*reg_data));
}
void getMtilen(uint64_t *csr_data, uint64_t *reg_data) {
    __asm__ volatile("csrr %0, 0xC42" : "=r"(*csr_data));
    __asm__ volatile("mv %0, x4" : "=r"(*reg_data));
}
void getMtilek(uint64_t *csr_data, uint64_t *reg_data) {
    __asm__ volatile("csrr %0, 0xC43" : "=r"(*csr_data));
    __asm__ volatile("mv %0, x4" : "=r"(*reg_data));
}
int main() {

    uint32_t csr_temp = 0x00000000;
    __asm__ volatile("mv x6, %0" : : "r"(csr_temp) : );
    __asm__ volatile("msettype x5, x6");
    uint64_t csr_mtype;
    uint64_t x5;
    getMtype(&csr_mtype, &x5);
    printf("After msettype:\n");
    printf("  CSR[c40] = 0x%lx\n", csr_mtype);
    printf("  x5       = 0x%lx\n", x5);

    //msettilem设置
    uint64_t mtilem_init, x4_initm;
    getMtilem(&mtilem_init, &x4_initm);

    __asm__ volatile("msettilemi x4, 0x8");
    uint64_t mtilem_msettilemi, x4_msettilemi;
    getMtilem(&mtilem_msettilemi, &x4_msettilemi);
    printf("After msettilemi:\n");
    printf("  CSR[c40] = 0x%lx\n", mtilem_msettilemi);
    printf("  x5       = 0x%lx\n", x4_msettilemi);

    //msettilen设置
    uint64_t mtilen_init, x4_initn;
    getMtilen(&mtilen_init, &x4_initn);

    __asm__ volatile("msettileni x4, 0x8");
    uint64_t mtilen_msettileni, x4_msettileni;
    getMtilen(&mtilen_msettileni, &x4_msettileni);
    printf("After msettileni:\n");
    printf("  CSR[c40] = 0x%lx\n", mtilen_msettileni);
    printf("  x5       = 0x%lx\n", x4_msettileni);

    //msettilek设置
    uint64_t mtilek_init, x4_initk;
    getMtilek(&mtilek_init, &x4_initk);

    __asm__ volatile("msettileki x4, 0x8");
    uint64_t mtilek_msettileki, x4_msettileki;
    getMtilek(&mtilek_msettileki, &x4_msettileki);
    printf("After msettileki:\n");
    printf("  CSR[c40] = 0x%lx\n", mtilek_msettileki);
    printf("  x5       = 0x%lx\n", x4_msettileki);

    //load数据
    const uint8_t ls_i8_src1[8*8]={
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8
    };
    const uint32_t ls_i8_src2[8*2]={
    1,2,
    1,2,
    1,2,
    1,2,
    1,2,
    1,2,
    1,2,
    1,2
    };
    const uint16_t ls_i8_src3[8*8]={
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8
    };
    const uint32_t ls_i8_src4[8*8]={
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8
    };
    size_t stride=8;
    uint8_t dest[8*8] = {0}; // 初始化为0
    // muint8_t ms1=mla_m(ls_i8_src1, stride);
    // muint8_t ms2=mlb_m(ls_i8_src2, stride);
    __asm__ volatile("mv x1, %0" : : "r"(ls_i8_src1) : );
    __asm__ volatile("mv x5, %0" : : "r"(stride) : );

    // __asm__ volatile("mlae8.m tr0, (x1), x5");
    // __asm__ volatile("mlate8.m tr1, (x1), x5");
    // __asm__ volatile("mlbe8.m tr2, (x1), x5");
    // __asm__ volatile("mlbte8.m tr3, (x1), x5");
    // __asm__ volatile("mlce8.m acc4, (x1), x5");
    // __asm__ volatile("mlcte8.m acc5, (x1), x5");
    // __asm__ volatile("mltre8.m tr6, (x1), x5");

    // store指令测试 - 将tr0中的数据存储到dest
    __asm__ volatile("mv x1, %0" : : "r"(dest) : );
    __asm__ volatile("msae8.m tr0, (x1), x5");
    // __asm__ volatile("msae8.m tr0, (x1), x5");
    // __asm__ volatile("msae8.m tr0, (x1), x5");
    // __asm__ volatile("msae8.m tr0, (x1), x5");

    printf("\nStore results:\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%5d ", dest[i*8 + j]);
        }
        printf("\n");
    }

    // // 验证结果是否正确
    int error = 0;
    // for (int i = 0; i < 8*8; i++) {
    //     if (dest[i] != ls_i8_src3[i]) {
    //         printf("Error at index %d: expected %d, got %d\n",
    //             i, ls_i8_src3[i], dest[i]);
    //         error = 1;
    //     }
    // }
    // 使用二维形式
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (dest[i*8 + j] != ls_i8_src1[i*8 + j]) {
                printf("Error at [%d][%d]: expected %d, got %d\n",
                    i, j, ls_i8_src1[i*8 + j], dest[i*8 + j]);
                error = 1;
            }
        }
    }

    if (!error) {
        printf("Store test passed!\n");
    } else {
        printf("Store test failed!\n");
    }

    // 测试输出
    printf("Hello RISC-V!\n");

    // msa_m(ms1,ls_i8_src1, stride);
    // msa_m(ms2,ls_i8_src2, stride);

    return 0;
}
