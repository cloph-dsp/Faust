$root = Split-Path -Parent $PSScriptRoot
& 'C:\Program Files\Faust\bin\faust.exe' -lang cpp -fpga-mem-th 4 -ct 1 -cn GrungrFaustDSP -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0 -o "$root\DSP\GrungrFaustDSP.h" "$root\Grungr.dsp"
