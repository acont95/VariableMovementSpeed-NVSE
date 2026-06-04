param(
    [string]$SolutionDir,
    [string]$TargetDir,
    [string]$TargetName
)

$ErrorActionPreference = "Stop"

$distDir   = Join-Path $SolutionDir "dist"
$nvseDir   = Join-Path $distDir "nvse"
$pluginDir = Join-Path $nvseDir "plugins"

# Create folder structure
New-Item -ItemType Directory -Force -Path $pluginDir | Out-Null

# --- 1. Plugin ZIP (DLL + PDB) ---
Copy-Item -Path (Join-Path $TargetDir "$TargetName.dll") -Destination $pluginDir -Force
Copy-Item -Path (Join-Path $TargetDir "$TargetName.pdb") -Destination $pluginDir -Force

Compress-Archive -Path $nvseDir -DestinationPath (Join-Path $distDir "$TargetName.zip") -Force

# Clear temp folder
Remove-Item -Path $nvseDir -Force -Recurse
