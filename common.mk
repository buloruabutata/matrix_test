target ?= riscv64-unknown-elf
march ?= rv64gv0p10zfh0p1
src ?= demo
debug := FmtFlag

ifdef flag
	debug:=$(flag),FmtFlag
endif

# 仿真结果输出到trace.out
ifdef trace
	debug:=$(debug),Exec --debug-file=trace.out
endif

CC=clang --target=$(target) \
		-march=$(march) \
		-menable-experimental-extensions \
		-mllvm -riscv-v-vector-bits-min=128 \

# 编译当前目录下的.c/cpp程序
run:
	mkdir -p build
	$(CC) -o build/$(src).elf $(src)
	$(target)-objdump -d build/$(src).elf > build/$(src).dump

# 编译build目录下的.s汇编程序
asm:
	mkdir -p build
	$(CC) -o build/$(src).elf -nostartfiles build/$(src)
	$(target)-objdump -d build/$(src).elf > build/$(src).dump

# 启用gem5仿真
gem5:
	# --cpu-type=TimingSimpleCPU
	# --cpu-type=MinorCPU --caches --l1i_size=32kB --l1d_size=64kB
	mkdir -p build
	../../riscv-ame-gem5/build/RISCV/gem5.opt --debug-flags=$(debug) ../../riscv-ame-gem5/configs/deprecated/example/se.py --cmd=build/$(src).elf | tee build/gem5.log

# 反汇编
dump:
	mkdir -p build
	$(target)-gcc $(src).cpp -o build/$(src).elf
	$(target)-objdump -d build/$(src).elf > build/$(src).dump

debug_help:
	../../riscv-ame-gem5/build/RISCV/gem5.opt --debug-help

log:
	code m5out/trace.out
