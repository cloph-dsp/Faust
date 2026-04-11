import("stdfaust.lib");
crossover = fi.crossover3LR4(220.0, 2200.0);
process(left_in, right_in) = left_in : crossover : _,_,_;
