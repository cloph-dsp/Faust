$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
$proj = "D:\Github\VST3 Developments\DataBend\projects\DataBend-vst3.vcxproj"
$SolutionDir = "D:\Github\VST3 Developments\DataBend"

$psi = New-Object System.Diagnostics.ProcessStartInfo
$psi.FileName = $msbuild
$psi.Arguments = "`"$proj`" /p:Configuration=Release /p:Platform=x64 /p:SolutionDir=`"$SolutionDir`\\`" /t:Build /v:minimal /nologo"
$psi.UseShellExecute = $false
$psi.RedirectStandardOutput = $true
$psi.RedirectStandardError = $true

$proc = [System.Diagnostics.Process]::Start($psi)
$stdout = $proc.StandardOutput.ReadToEnd()
$stderr = $proc.StandardError.ReadToEnd()
$proc.WaitForExit()
Write-Output $stdout
if ($stderr) { Write-Output "STDERR: $stderr" }
Write-Output "EXIT:$($proc.ExitCode)"
