Add-Type -TypeDefinition @"
using System;
using System.Runtime.InteropServices;
public class VST3Loader {
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    public static extern IntPtr LoadLibraryW(string lpFileName);

    [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Ansi)]
    public static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);

    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool FreeLibrary(IntPtr hModule);

    public delegate IntPtr GetPluginFactoryDelegate();
}
"@

function Test-VST3([string]$path) {
    Write-Host ""
    Write-Host "=== Testing: $path ==="
    Write-Host "File size: $((Get-Item $path).Length) bytes"

    $h = [VST3Loader]::LoadLibraryW($path)
    if ($h -eq [IntPtr]::Zero) {
        $err = [System.Runtime.InteropServices.Marshal]::GetLastWin32Error()
        Write-Host "LoadLibraryW FAILED: Win32 error $err" -ForegroundColor Red
        return
    }
    Write-Host "LoadLibraryW OK, handle = 0x$($h.ToString('X'))"

    $factoryAddr = [VST3Loader]::GetProcAddress($h, "GetPluginFactory")
    if ($factoryAddr -eq [IntPtr]::Zero) {
        Write-Host "GetProcAddress GetPluginFactory FAILED" -ForegroundColor Red
    } else {
        Write-Host "GetPluginFactory address = 0x$($factoryAddr.ToString('X'))"
        $del = [VST3Loader+GetPluginFactoryDelegate]::CreateDelegate([VST3Loader+GetPluginFactoryDelegate], $factoryAddr)
        try {
            $factoryObj = $del.Invoke()
            Write-Host "factory() returned: 0x$($factoryObj.ToString('X'))"
            if ($factoryObj -ne [IntPtr]::Zero) {
                Write-Host "SUCCESS -- plugin loads" -ForegroundColor Green
            }
        } catch {
            Write-Host "factory() THREW: $($_.Exception.Message)" -ForegroundColor Red
        }
    }
    [VST3Loader]::FreeLibrary($h) | Out-Null
}

Test-VST3 "C:\Program Files\Common Files\VST3\Tuner.vst3"
Test-VST3 "C:\Program Files\Common Files\VST3\Freeze95.vst3"