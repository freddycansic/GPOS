$currentPath = Get-Location
function Get-Absolute-Path
{
    param 
    (
        [string] $relativePath
    )


    return Join-Path $currentPath $relativePath
}

# build solution
C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe OpenGL.sln /property:Configuration=Release | Out-Null # makes powershell wait for process to finish before moving on

# copy res folder into build path
Copy-Item (Get-Absolute-Path "OpenGL\res") -Destination (Get-Absolute-Path "bin\x64Release") -Force -Recurse

$zipName = Read-Host -Prompt "Enter zip name e.g v0.1.0-alpha"

Compress-Archive -LiteralPath ((Get-Absolute-Path "bin\x64Release\OpenGL.exe"),(Get-Absolute-Path "bin\x64Release\res")) -DestinationPath (Get-Absolute-Path "\bin\x64Release\$zipName.zip")