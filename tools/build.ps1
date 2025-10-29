$files = Get-ChildItem -Path source -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }

Write-Output "Compiling with files:"
$files | ForEach-Object { Write-Output "  $_" }

$cmd = "g++ -std=c++17 -Iinclude $files tools/card_demo.cpp -O0 -g -o tools/card_demo.exe"
Write-Output "Running: $cmd"
# Execute command
& g++ -std=c++17 -Iinclude $files tools/card_demo.cpp -O0 -g -o tools/card_demo.exe
$rc = $LASTEXITCODE
Write-Output "g++ exit code: $rc"
if ($rc -ne 0) { exit $rc } else { Write-Output "Build OK" }