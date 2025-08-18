target ?= riscv64-unknown-elf
march ?= rv64gv0p10zfh0p1
src ?= demo
debug := FmtFlag
cpu ?= AtomicSimpleCPU
str ?= MatRegs
gem5_type ?= opt

ifdef flag
	debug := $(flag),FmtFlag
endif

# 仿真结果输出到trace.out
ifdef trace
	debug := $(debug),Exec --debug-file=trace.out
endif

# 根据 cpu 的值设置 type
ifeq ($(cpu), timing)
    cpu_type := --cpu-type=TimingSimpleCPU
else ifeq ($(cpu), minor)
    cpu_type := --cpu-type=MinorCPU --caches --l1i_size=32kB --l1d_size=64kB
else
    cpu_type := --cpu-type=AtomicSimpleCPU
endif


CC=clang --target=$(target) \
		-march=$(march) \
		-menable-experimental-extensions \
		-mllvm -riscv-v-vector-bits-min=128 \

# 编译当前目录下的.c/cpp程序
c:
	mkdir -p build
	$(CC) -o build/$(src).elf $(src)
	$(target)-objdump -d build/$(src).elf > build/$(src).dump

# 编译build目录下的.s汇编程序
asm:
	mkdir -p build
	# -nostartfiles解决汇编函数_start找不到main的问题，-Wl,--no-relax禁用寄存器松弛，避免la使用未初始化的gp寻址
	# $(CC) -o build/$(src).elf -nostartfiles -static -Wl,--no-relax build/$(src)
	$(CC) -o build/$(src).elf -nostdlib -Wl,--no-relax build/$(src)
	$(target)-objdump -d build/$(src).elf > build/$(src).dump

# 启用gem5仿真
gem5_c: c
	mkdir -p build
	../../riscv-ame-gem5/build/RISCV/gem5.$(gem5_type) --debug-flags=$(debug) ../../riscv-ame-gem5/configs/deprecated/example/se.py --cmd=build/$(src).elf $(cpu_type) | tee build/gem5.log

gem5: asm
	mkdir -p build
	../../riscv-ame-gem5/build/RISCV/gem5.$(gem5_type) --debug-flags=$(debug) ../../riscv-ame-gem5/configs/deprecated/example/se.py --cmd=build/$(src).elf $(cpu_type) | tee build/gem5.log

sed:
	sed -n '/$(str)/p' m5out/trace.out > m5out/trace.sed
	code m5out/trace.sed

# 查看符号表
elf:
	riscv64-unknown-elf-nm build/$(src).elf

# 反汇编
dump:
	mkdir -p build
	$(target)-gcc $(src).cpp -o build/$(src).elf
	$(target)-objdump -d build/$(src).elf > build/$(src).dump



log:
	code m5out/trace.out
