Param(
    [string]$Path = 'd:\Github\VST3 Developments\BLUR\EmbeddedFonts.h'
)

$backup = $Path + '.bak'
Copy-Item -Path $Path -Destination $backup -Force

$lines = [System.IO.File]::ReadAllLines($Path)
$start = -1
for ($i=0; $i -lt $lines.Length; $i++) {
    if ($lines[$i] -match 'static\s+const\s+unsigned\s+char\s+kFontSonoRegular\[\]\s*=\s*{') { $start = $i; break }
}
if ($start -eq -1) { Write-Host "START_NOT_FOUND"; exit 2 }

$end = -1
for ($i=$start+1; $i -lt $lines.Length; $i++) {
    if ($lines[$i] -match '^\s*};\s*$') { $end = $i; break }
}
if ($end -eq -1) { Write-Host "END_NOT_FOUND"; exit 3 }

$modified = $false
for ($i = $start+1; $i -lt $end; $i++) {
    $line = $lines[$i]
    $trim = $line.TrimEnd()
    if ($trim -eq '') { continue }
    $lastChar = $trim.Substring($trim.Length - 1, 1)
    if ($lastChar -ne ',') {
        if ($trim -match '0x[0-9A-Fa-f]{1,2}') {
            $lines[$i] = $trim + ','
            $modified = $true
        }
    }
}

if ($modified) {
    [System.IO.File]::WriteAllLines($Path, $lines, [System.Text.Encoding]::UTF8)
    Write-Host "FIXED"
} else {
    Write-Host "NO_CHANGES"
}
exit 0
