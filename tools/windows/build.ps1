cd build
rm -r *
cmake ../

$path = Get-Location
cd "C:\Program Files (x86)\MSBuild\15.0"


MSBuild (Join-Path $path OpenGL.sln) /property:Configuration=Release