import("stdfaust.lib");
xover_lo = 220.0;
xover_hi = 2200.0;
process(left_in, right_in) = (out_l, out_r)
with {
	(low_l, mid_l, high_l) = left_in : fi.crossover3LR4(xover_lo, xover_hi);
	(low_r, mid_r, high_r) = right_in : fi.crossover3LR4(xover_lo, xover_hi);
	out_l = low_l;
	out_r = low_r;
};
