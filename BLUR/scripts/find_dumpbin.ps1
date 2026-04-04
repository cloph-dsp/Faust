$roots = @(
  'C:\Program Files\Microsoft Visual Studio\2022\BuildTools',
  'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools',
  'C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools',
  'C:\Program Files\Microsoft Visual Studio\2019\BuildTools'
)
foreach ($root in $roots) {
  if (Test-Path $root) {
    try {
      $found = Get-ChildItem -Path $root -Recurse -Filter dumpbin.exe -ErrorAction SilentlyContinue -Force | Select-Object -First 1
      if ($found) { Write-Output $found.FullName; break }
    } catch {
      # ignore
    }
  }
}
