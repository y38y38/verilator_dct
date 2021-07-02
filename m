verilator --cc --exe  --trace-fst --trace-params --trace-structs --trace-underscore \
    dct.sv -exe test_dct.cpp
make -C obj_dir -f Vdct.mk

