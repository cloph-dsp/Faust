import wave, os, glob, struct; files=glob.glob(r"D:\Github\VST3 Developments\MetallicSnare\MetalSnare-20260406T192740Z-3-001\MetalSnare\*.wav")[:10]; 
for f in files:
    try:
        w=wave.open(f,"r"); n=w.getnframes(); fs=w.getframerate(); sw=w.getsampwidth()
        d=w.readframes(n); w.close()
        print(f"File: {os.path.basename(f)}, Sample Rate: {fs}, Length: {n/fs:.2f}s, Peak: unknown (needs numpy)")
    except Exception as e: print(e)

