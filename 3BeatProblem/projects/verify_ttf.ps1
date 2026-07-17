$path = 'D:\Github\VST3 Developments\3BeatProblem\build-win\vst3\x64\Release\3BeatProblem.vst3'
$bytes = [System.IO.File]::ReadAllBytes($path)
$len = $bytes.Length
Write-Host "File: $len bytes"
# TrueType magic: 0x00010000 (4 bytes LE) at start of any TTF file
# volunmo.ttf is 25756 bytes; if embedded, find a 4-byte run 00 01 00 00
$ttfMagic = [byte[]](0x00, 0x01, 0x00, 0x00)
$idx = -1
for ($i = 0; $i -le $len - 4; $i++) {
  if ($bytes[$i] -eq 0x00 -and $bytes[$i+1] -eq 0x01 -and $bytes[$i+2] -eq 0x00 -and $bytes[$i+3] -eq 0x00) {
    # TTF must have a valid table directory at offset 12; sample the next 4 bytes as uint32
    if ($i + 16 -le $len) {
      $numTables = [BitConverter]::ToUInt16($bytes, $i + 4)
      if ($numTables -gt 0 -and $numTables -lt 64) {
        $idx = $i
        Write-Host ("TTF magic at offset {0}, numTables={1}" -f $i, $numTables)
        break
      }
    }
  }
}
if ($idx -lt 0) {
  Write-Host "No valid TTF signature found in .vst3 binary"
} else {
  Write-Host ("TTF embedded at offset {0}" -f $idx)
}
