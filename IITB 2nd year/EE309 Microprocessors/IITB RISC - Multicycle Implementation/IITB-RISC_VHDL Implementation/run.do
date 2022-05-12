vsim work.processor
add wave -position insertpoint sim:/processor/*
force -freeze sim:/processor/clk 0 0, 1 {50 ps} -r 100
force -freeze sim:/processor/rst 1 0
add wave -position insertpoint  \
sim:/processor/controlport/Q
run
run
force -freeze sim:/processor/rst 0 0
run 1ns