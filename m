verilator --cc --exe  --trace-fst --trace-params --trace-structs --trace-underscore \
    calc.sv -exe test_calc.cpp
make -C obj_dir -f Vcalc.mk

