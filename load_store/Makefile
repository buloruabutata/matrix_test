include ../common.mk

all: run

run:
	mkdir -p build
	clang --target=$(target) \
		-march=$(march) \
		-menable-experimental-extensions \
		-mllvm -riscv-v-vector-bits-min=128 \
		-o build/$(src).riscv $(src).cpp
	$(target)-objdump -d build/$(src).riscv > build/$(src).dump

gem5: run
	# --cpu-type=TimingSimpleCPU
	# --cpu-type=MinorCPU --caches --l1i_size=32kB --l1d_size=64kB
	mkdir -p build
	../../riscv-ame-gem5/build/RISCV/gem5.opt --debug-flags=$(debug) ../../riscv-ame-gem5/configs/deprecated/example/se.py --cmd=build/$(src).riscv | tee build/gem5.log
dump:
	mkdir -p build
	$(target)-gcc $(src).cpp -o build/$(src).elf
	$(target)-objdump -d build/$(src).elf > build/$(src).dump	

debug_help:
	../../riscv-ame-gem5/build/RISCV/gem5.opt --debug-help

log: 
	code m5out/trace.out