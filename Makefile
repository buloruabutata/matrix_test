include common.mk

build_gem5:
	cd ../riscv-ame-gem5 && scons ./build/RISCV/gem5.$(gem5_type)

debug_help:
	../riscv-ame-gem5/build/RISCV/gem5.opt --debug-help

fix:
	# style.py from gem5/utils/style.py
	python style.py -m
