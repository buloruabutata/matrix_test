target ?= riscv64-unknown-elf
march ?= rv64gv0p10zfh0p1
src ?= demo
debug := FmtFlag

ifdef flag
	debug:=$(flag),FmtFlag
endif

ifdef trace
	debug:=$(debug),Exec --debug-file=trace.out
endif

