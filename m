verilator --cc --exe  --trace-params --trace-structs --trace-underscore \
    wrapper.sv \
	-exe test_main.cpp \
	test_utility.cpp

make -C obj_dir -f Vwrapper.mk

