function Get-Path
{
    param 
    (
        [string] $relativePath
    )

    return Join-Path $PSScriptRoot $relativePath
}

# build solution
C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe OpenGL/OpenGL.vcxproj /property:Configuration=Release | Out-Null # makes powershell wait for process to finish before moving on

# copy res folder into build path
Copy-Item (Get-Path "OpenGL\res") -Destination (Get-Path "bin\x64Release") -Force -Recurse

# Write-Output $currentPath
# Write-Output (Get-Absolute-Path "OpenGL\res")

$zipName = Read-Host -Prompt "Enter zip name e.g v0.1.0-alpha"

Compress-Archive -LiteralPath ((Get-Path "bin\x64Release\OpenGL.exe"),(Get-Path "bin\x64Release\res")) -DestinationPath (Get-Path "\bin\x64Release\$zipName.zip")