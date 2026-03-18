import("stdfaust.lib");

// ========================================
// QUANTUM DISTORTION PLUGIN v2.0
// Processamento probabilístico de áudio aprimorado
// ========================================

// --- PARÂMETROS DE CONTROLE ---
collapse_rate = hslider("h:Quantum/Collapse Rate (Hz) [style:knob]", 4, 0.1, 16, 0.1);
quantum_chaos = hslider("h:Quantum/Chaos Amount [style:knob]", 0.5, 0, 1, 0.01);
transition_time = hslider("h:Quantum/Transition Time (ms) [style:knob]", 50, 1, 500, 1) : si.smoo;

// Probabilidades para cada estado
prob_bitcrush = hslider("h:Probabilities/State 1: Bitcrush [style:knob]", 0.25, 0, 1, 0.01);
prob_distortion = hslider("h:Probabilities/State 2: Distortion [style:knob]", 0.25, 0, 1, 0.01);
prob_ringmod = hslider("h:Probabilities/State 3: Ring Mod [style:knob]", 0.25, 0, 1, 0.01);
prob_granular = hslider("h:Probabilities/State 4: Granular [style:knob]", 0.25, 0, 1, 0.01);

// Controles de intensidade por estado
intensity_bitcrush = hslider("h:Intensity/Bitcrush Depth [style:knob]", 0.5, 0, 1, 0.01) : si.smoo;
intensity_distortion = hslider("h:Intensity/Distortion Drive [style:knob]", 0.5, 0, 1, 0.01) : si.smoo;
intensity_ringmod = hslider("h:Intensity/Ring Mod Freq [style:knob]", 0.5, 0, 1, 0.01) : si.smoo;
intensity_granular = hslider("h:Intensity/Granular Size [style:knob]", 0.5, 0, 1, 0.01) : si.smoo;

// Controles globais
freeze = checkbox("h:Control/Freeze State");
manual_state = hslider("h:Control/Manual State [style:knob]", 1, 1, 4, 1);
dry_wet = hslider("h:Control/Dry-Wet [style:knob]", 1.0, 0, 1, 0.01) : si.smoo;
output_gain = hslider("h:Control/Output Gain (dB) [style:knob]", 0, -12, 12, 0.1) : ba.db2linear : si.smoo;

// Filtro pós-processamento
filter_freq = hslider("h:Filter/Lowpass Freq (Hz) [style:knob]", 18000, 500, 20000, 10) : si.smoo;
filter_enable = checkbox("h:Filter/Enable Filter");

// Display do estado atual
state_display = hbargraph("h:Monitor/Current State", 1, 4);
random_display = hbargraph("h:Monitor/Random Value", 0, 1);

// --- GERADOR DE NÚMEROS PSEUDO-ALEATÓRIOS MELHORADO ---
// LCG simples e confiável
noise_state = (+(12345) ~ *(1103515245)) : +(12345);
random = noise_state : abs : float : /(2147483647.0);

// --- COLAPSO DE FUNÇÃO DE ONDA ---
collapse_system = state_machine ~ _
with {
    state_machine(prev_state) = new_state
    with {
        // Inicialização
        current = ba.if(prev_state < 1, 1, ba.if(prev_state > 4, 1, prev_state));
        
        // Gerador de variação lenta para o collapse rate
        lfo_rate = os.osc(0.1 + quantum_chaos * 0.3);  // LFO muito lento
        rate_variation = lfo_rate * 0.3;  // ±30% de variação
        modulated_rate = collapse_rate * (1.0 + rate_variation * quantum_chaos);
        
        // Contador independente com rate variável
        samples_per_collapse = int(ma.SR / max(0.1, modulated_rate));
        counter = (+(1) ~ _) : %(samples_per_collapse);
        trigger = (counter == 0);
        
        // Geração de random value
        rand_raw = random;
        
        // Aplicar chaos
        rand_val = rand_raw * quantum_chaos + (1.0 - quantum_chaos) * 0.25;
        
        // Normalizar probabilidades
        prob_sum = prob_bitcrush + prob_distortion + prob_ringmod + prob_granular;
        safe_sum = max(0.001, prob_sum);
        
        // Calcular thresholds cumulativos
        t1 = prob_bitcrush / safe_sum;
        t2 = t1 + (prob_distortion / safe_sum);
        t3 = t2 + (prob_ringmod / safe_sum);
        
        // Seleção do novo estado com lógica explícita
        next_state = ba.if(rand_val < t1, 1,
                     ba.if(rand_val < t2, 2,
                     ba.if(rand_val < t3, 3, 4)));
        
        // Atualizar apenas quando trigger e não congelado
        should_update = trigger & (freeze == 0);
        candidate = ba.if(should_update, next_state, current);
        
        // Estado final
        new_state = ba.if(freeze, int(manual_state), candidate);
    };
};

current_state = collapse_system;
debug_random = random;

// --- EFEITOS POR ESTADO MELHORADOS ---

// Estado 1: Bitcrusher com sample rate reduction
bitcrusher(x) = crushed
with {
    // Bit depth reduction
    bits = max(1, 16 - (intensity_bitcrush * 14));
    step = 1.0 / pow(2, bits);
    bit_reduced = floor(x / step + 0.5) * step;
    
    // Sample rate reduction
    sample_reduce_factor = 1 + int(intensity_bitcrush * 31);
    sample_hold = (+(1) : %(sample_reduce_factor)) ~ _ : ==(0);
    crushed = bit_reduced : ba.sAndH(sample_hold);
};

// Estado 2: Distorsão waveshaping assimétrica
distortion_fx(x) = distorted
with {
    drive = 1 + (intensity_distortion * 49);
    // Waveshaping assimétrico para harmonicos mais ricos
    positive = max(0, x) : pow(_, 0.7 + intensity_distortion * 0.3);
    negative = min(0, x) : abs : pow(_, 0.8 + intensity_distortion * 0.2) : *(-1);
    shaped = (positive + negative) * drive;
    distorted = ma.tanh(shaped) : *(0.95);
};

// Estado 3: Ring Modulator com feedback
ringmod(x) = modulated
with {
    mod_freq = 50 + (intensity_ringmod * 950);
    // Carrier com feedback para textura mais complexa
    carrier_fb = (os.osc(mod_freq) + _) ~ *(0.3 * intensity_ringmod);
    // Mix entre ring mod puro e com feedback
    ring_pure = x * os.osc(mod_freq);
    ring_complex = x * carrier_fb;
    modulated = (ring_pure * (1 - intensity_ringmod * 0.5) + ring_complex * intensity_ringmod * 0.5) : *(0.85);
};

// Estado 4: Granular multi-tap com pitch shift
granular(x) = granulated
with {
    grain_size = 0.005 + (intensity_granular * 0.095);
    delay_samples = grain_size * ma.SR;
    max_delay = int(0.15 * ma.SR);
    
    // Multi-tap delays com diferentes tamanhos de grain
    tap1 = de.delay(max_delay, int(delay_samples), x);
    tap2 = de.delay(max_delay, int(delay_samples * 1.5), x);
    tap3 = de.delay(max_delay, int(delay_samples * 2.0), x);
    
    // Envelopes de janela para cada tap
    rate1 = 1.0 / (grain_size * 2);
    rate2 = 1.0 / (grain_size * 3);
    rate3 = 1.0 / (grain_size * 4);
    
    env1 = os.osc(rate1) : *(0.5) : +(0.5) : pow(_, 2);
    env2 = os.osc(rate2) : *(0.5) : +(0.5) : pow(_, 2);
    env3 = os.osc(rate3) : *(0.5) : +(0.5) : pow(_, 2);
    
    // Mix dos grains com envelopes
    grains = tap1 * env1 * 0.4 + tap2 * env2 * 0.3 + tap3 * env3 * 0.3;
    
    granulated = (grains + x * 0.3) : *(0.9);
};

// --- ROTEADOR DE ESTADOS ---
quantum_processor(x) = output
with {
    // Attach dos displays aqui onde são realmente usados
    state = current_state <: attach(_, state_display);
    rand_viz = debug_random <: attach(_, random_display);
    
    // Processa cada estado
    s1 = bitcrusher(x);
    s2 = distortion_fx(x);
    s3 = ringmod(x);
    s4 = granular(x);
    
    // Seleção direta do estado (garante que state está entre 1-4)
    safe_state = max(1, min(4, int(state)));
    selected = ba.selectn(4, safe_state - 1, s1, s2, s3, s4);
    
    // Suavização suave de transições
    transition_smooth = si.smooth(ba.tau2pole(0.005), selected);
    
    output = transition_smooth : fi.dcblocker;
};

// --- FILTRO PÓS-PROCESSAMENTO ---
post_filter(x) = filtered
with {
    lpf = fi.lowpass(4, filter_freq, x);
    filtered = ba.if(filter_enable, lpf, x);
};

// --- LIMITADOR SUAVE ---
soft_limiter(x) = limited
with {
    threshold = 0.9;
    limited = ba.if(abs(x) > threshold,
                    ma.signum(x) * (threshold + (abs(x) - threshold) * 0.2),
                    x);
};

// --- PROCESSAMENTO FINAL ESTÉREO ---
process = si.bus(2) : par(i, 2, channel)
with {
    channel(x) = x <: quantum_processor * dry_wet, x * (1 - dry_wet) 
                 :> _ : post_filter : soft_limiter : *(output_gain);
};