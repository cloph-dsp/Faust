$rc = "C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\rc.exe"
Set-Location "D:\Github\VST3 Developments\iPlug2\Examples\IPlugEffect"
$out = "C:\Users\Razer\AppData\Local\Temp\main_res5.res"
$sdkInc = "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um"
$args = @(
    '/l0x0409'
    '/I', 'D:\Github\VST3 Developments\Tuner VST\'
    '/I', 'D:\Github\VST3 Developments\Tuner VST\resources\fonts'
    '/I', 'D:\Github\VST3 Developments\Tuner VST\resources\img'
    '/I', $sdkInc
    '/v'
    '/nologo'
    "/fo$out"
    'resources\main.rc'
)
& $rc @args
Write-Host "rc exit=$LASTEXITCODE"
Get-Item $out -ErrorAction SilentlyContinue | Format-Table Length,LastWriteTime