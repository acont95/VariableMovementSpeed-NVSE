param(
    [string]$SolutionDir,
    [string]$TargetDir,
    [string]$TargetName
)

$ErrorActionPreference = "Stop"

$distDir   = Join-Path $SolutionDir "dist"
$nvseDir   = Join-Path $distDir "nvse"
$pluginDir = Join-Path $nvseDir "plugins"

$nvseSrcDir = Join-Path $SolutionDir "nvse"
$configDir  = Join-Path $SolutionDir "config"

# Create folder structure
New-Item -ItemType Directory -Force -Path $pluginDir | Out-Null

# --- 1. Plugin ZIP (DLL + PDB) ---
Copy-Item -Path (Join-Path $TargetDir "$TargetName.dll") -Destination $pluginDir -Force
Copy-Item -Path (Join-Path $TargetDir "$TargetName.pdb") -Destination $pluginDir -Force

# --- 2. Script runner source and config  ---
Copy-Item -Path $nvseSrcDir/* -Destination $nvseDir -Force -Recurse
Copy-Item -Path $configDir -Destination $distDir -Force -Recurse

Compress-Archive -Path $distDir/* -DestinationPath (Join-Path $distDir "$TargetName.zip") -Force

# Clear temp folder
Remove-Item -Path $nvseDir -Force -Recurse
