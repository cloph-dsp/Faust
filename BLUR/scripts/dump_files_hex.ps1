$paths = @(
 'C:\Program Files\Common Files\VST3\SpectralProtocol.vst3',
 'd:\Github\VST3 Developments\BLUR\iPlug2\Examples\IPlugEffect\build-win\vst3\x64\Release\SpectralProtocol.vst3'
)
foreach ($p in $paths) {
    if (Test-Path $p) {
        Write-Output "FILE: $p"
        try {
            $b = [System.IO.File]::ReadAllBytes($p)
            $len = [Math]::Min(64, $b.Length)
            $hex = ($b[0..($len-1)] | ForEach-Object { '{0:X2}' -f $_ }) -join ' '
            Write-Output $hex
        } catch {
            Write-Output "ERROR reading $p : $_"
        }
    } else {
        Write-Output "MISSING: $p"
    }
}
