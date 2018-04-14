function MyMake([string]$name, $flag) {
    $gccFlags = @(
        "-std=c11",
        "-Wall",
        "-Wextra",
        "-Wfloat-equal",
        "-Wshadow",
        "-Wunreachable-code",
        # "-save-temps",
        "-masm=intel",
        "-fopenmp",
        $( -join ($name, ".c")),
        "..\include\pcg-c-basic-0.9\pcg_basic.o",
        "-o",
        $( -join ($name, ".exe")),
        "-O3"
    )
    # Write-Host "gcc" $gccFlags $flag
    & "gcc" $gccFlags $flag
}