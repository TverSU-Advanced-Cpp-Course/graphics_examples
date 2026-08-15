# Сборка примеров.
#
# Использование:
#   .\build.ps1              собрать (conan install — только если пресета ещё нет)
#   .\build.ps1 -Install     перегенерировать зависимости и собрать
#   .\build.ps1 -Clean       снести build\ и собрать с нуля

param([switch]$Clean, [switch]$Install)

$ErrorActionPreference = 'Continue'
Set-Location $PSScriptRoot

[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new($false)
$env:VSLANG = 1033

if (-not (Get-Command conan -ErrorAction SilentlyContinue)) {
    Write-Error "conan не найден в PATH. Нужен Conan 2: pip install conan"
    exit 1
}

$presets = 'build\Release\generators\CMakePresets.json'

if ($Clean) {
    Remove-Item build -Recurse -Force -ErrorAction SilentlyContinue
}

if ($Clean -or $Install -or -not (Test-Path $presets)) {
    conan install . --build=missing
    if ($LASTEXITCODE -ne 0) {
        Write-Error "conan install упал — зависимости не собраны."
        exit 1
    }
}

cmd /c "call build\Release\generators\conanbuild.bat && cmake --preset conan-release && cmake --build --preset conan-release"

if ($LASTEXITCODE -ne 0) {
    Write-Error "Сборка упала."
    exit 1
}

Write-Host "Готово: $(Join-Path $PSScriptRoot 'build\Release')"
