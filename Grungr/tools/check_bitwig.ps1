Get-Process | Where-Object { $_.ProcessName -in @('Bitwig Studio','BitwigAudioEngine-X64-AVX2','BitwigPluginHost-X64-AVX2','BitwigStudioApp') } | Select-Object ProcessName,Id | Format-Table -AutoSize
