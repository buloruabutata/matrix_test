// #include <stdint.h>
// #include <stdio.h>

// int main() {
//     uint8_t input[8][16] = {0};

//     for(int i = 0; i < 8; i++) {
//         for(int j = 0; j < 16; j++) {
//             input[i][j] = i * 10 + j;
//         }
//     }

//     __asm__ volatile(
//         // "msetsew t0, e8\n"
//         "msettilemi t0, 0x8\n"
//         "msettileki t0, 0x8\n"
//         "msettileni t0, 0x8\n"
//         "li t1, 8\n"
//         "mv t0, %0\n"
//         // 0000_0100_0110_0010_1000_0000_1111_0111
//         "mlae8.m tr1, (t0), t1\n"
//         // ".word 0x046280F7\n"
//         :
//         : "r"(input)
//         : "t0", "t1"
//     );

//     return 0;
// }

#include <stdint.h>
#include <stdio.h>
#include <riscv_matrix.h>
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

    uint32_t csr_temp = 0x00000001;
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

    __asm__ volatile("msettileni x4, 0x4");
    uint64_t mtilen_msettileni, x4_msettileni;
    getMtilen(&mtilen_msettileni, &x4_msettileni);
    printf("After msettileni:\n");
    printf("  CSR[c40] = 0x%lx\n", mtilen_msettileni);
    printf("  x5       = 0x%lx\n", x4_msettileni);

    //msettilek设置
    uint64_t mtilek_init, x4_initk;
    getMtilek(&mtilek_init, &x4_initk);

    __asm__ volatile("msettileki x4, 0x4");
    uint64_t mtilek_msettileki, x4_msettileki;
    getMtilek(&mtilek_msettileki, &x4_msettileki);
    printf("After msettileki:\n");
    printf("  CSR[c40] = 0x%lx\n", mtilek_msettileki);
    printf("  x5       = 0x%lx\n", x4_msettileki);

    //load数据
    const uint8_t ls_i8_src1[8*16]={
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
    1,2,3,4,5,6,7,8,
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
    // muint8_t ms1=mla_m(ls_i8_src1, stride);
    // muint8_t ms2=mlb_m(ls_i8_src2, stride);
    __asm__ volatile("mv x1, %0" : : "r"(ls_i8_src1) : );
    __asm__ volatile("mv x5, %0" : : "r"(stride) : );

    // __asm__ volatile("mlae16.m tr0, (x1), x5");
    // __asm__ volatile("mlate16.m tr1, (x1), x5");
    // __asm__ volatile("mlbe16.m tr2, (x1), x5");
    // __asm__ volatile("mlbte16.m tr3, (x1), x5");
    // __asm__ volatile("mlce16.m acc4, (x1), x5");
    // __asm__ volatile("mlcte16.m acc5, (x1), x5");
    __asm__ volatile("mltre8.m tr6, (x1), x5");

    printf("CSR[c41] (mtilem) = 0x%lx\n", mtilem_init);
    printf("  x4       = 0x%lx\n", x4_initm);
    printf("After msettilemi:\n");
    printf("CSR[c41] (mtilem) = 0x%lx\n", mtilem_msettilemi);
    printf("  x4       = 0x%lx\n", x4_msettilemi);

    // printf("CSR[c42] (mtilen) = 0x%lx\n", mtilen_init);
    // printf("  x4       = 0x%lx\n", x4_initn);
    // printf("After msettileni:\n");
    // printf("CSR[c42] (mtilen) = 0x%lx\n", mtilen_msettileni);
    // printf("  x4       = 0x%lx\n", x4_msettileni);

    // printf("CSR[c43] (mtilek) = 0x%lx\n", mtilek_init);
    // printf("  x4       = 0x%lx\n", x4_initk);
    // printf("After msettileki:\n");
    // printf("CSR[c43] (mtilek) = 0x%lx\n", mtilek_msettileki);
    // printf("  x4       = 0x%lx\n", x4_msettileki);


    // msa_m(ms1,ls_i8_src1, stride);
    // msa_m(ms2,ls_i8_src2, stride);

    return 0;
}
