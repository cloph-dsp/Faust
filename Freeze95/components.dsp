// Partial DSP components
// Base random generators
rnd_base = (+(111) ~ *(1103515245)) : +(12345) : %(2147483647);
rnd1 = rnd_base : %(2147483647) : /(2147483647.0);
rnd2 = (rnd_base : +(1)) : %(2147483647) : /(2147483647.0);
rnd3 = (rnd_base : +(2)) : %(2147483647) : /(2147483647.0);
rnd4 = (rnd_base : +(3)) : %(2147483647) : /(2147483647.0);
rnd5 = (rnd_base : +(4)) : %(2147483647) : /(2147483647.0);

// Simplified band processing
low_l = left_in : fi.lowpass(2, 220.0);
mid_l = left_in : fi.bandpass(2, 220.0, 2200.0);
high_l = left_in : fi.highpass(2, 2200.0);

low_r = right_in : fi.lowpass(2, 220.0);
mid_r = right_in : fi.bandpass(2, 220.0, 2200.0);
high_r = right_in : fi.highpass(2, 2200.0);

// Simplified auto-makeup gain
intelligent_makeup(ref_l, ref_r, proc_l, proc_r) = proc_l * 0.95, proc_r * 0.95;