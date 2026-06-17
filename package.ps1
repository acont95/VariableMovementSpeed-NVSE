param(
    [string]$SolutionDir,
    [string]$TargetDir,
    [string]$TargetName
)

$ErrorActionPreference = "Stop"

$distDir   = Join-Path $SolutionDir "dist"
$nvseOutDir   = Join-Path $distDir "nvse"
$pluginOutDir = Join-Path $nvseOutDir "plugins"
$configOutDir   = Join-Path $distDir "config"

$nvseSrcDir = Join-Path $SolutionDir "nvse"
$configSrcDir  = Join-Path $SolutionDir "config"

# Create folder structure
New-Item -ItemType Directory -Force -Path $pluginOutDir | Out-Null

# --- 1. Plugin ZIP (DLL + PDB) ---
Copy-Item -Path (Join-Path $TargetDir "$TargetName.dll") -Destination $pluginOutDir -Force
Copy-Item -Path (Join-Path $TargetDir "$TargetName.pdb") -Destination $pluginOutDir -Force

# --- 2. Script runner source and config  ---
Copy-Item -Path $nvseSrcDir/* -Destination $nvseOutDir -Force -Recurse
Copy-Item -Path $configSrcDir -Destination $distDir -Force -Recurse

Compress-Archive -Path $distDir/* -DestinationPath (Join-Path $distDir "$TargetName.zip") -Force

# Clear temp folder
Remove-Item -Path $nvseOutDir -Force -Recurse
Remove-Item -Path $configOutDir -Force -Recurse
