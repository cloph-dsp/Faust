import("stdfaust.lib");
xover_lo = 220.0;
xover_hi = 2200.0;
(low_l, mid_l, high_l) = _,_ : fi.crossover3LR4(xover_lo, xover_hi);
(low_r, mid_r, high_r) = _,_ : fi.crossover3LR4(xover_lo, xover_hi);
process(left_in, right_in) = low_l, low_r;
