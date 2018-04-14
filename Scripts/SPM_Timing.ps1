
# Roda os 3 programas de Gauss $Samples vezes
function All_Gauss ([int]$fatores, [int]$Iteracoes, [int]$Samples = 1, [int]$modo = 0) {
    <#
    .SYNOPSIS
      Runs the given script block and returns the execution duration.
      Discovered on StackOverflow. http://stackoverflow.com/questions/3513650/timing-a-commands-execution-in-powershell
      
    .EXAMPLE
      Measure-Command2 { ping -n 1 google.com }
    #>
    $timings_S = @()
    $timings_P = @()
    $timings_M = @()
    $cont = 0
    do {
        $sw = New-Object Diagnostics.Stopwatch
        $timings_S += (Measure-Command {& .\S_Gauss.exe $fatores $Iteracoes}).TotalSeconds
        $cont++
    }
    while ($cont -lt $Samples)
    do {
        $sw = New-Object Diagnostics.Stopwatch
        $timings_P += (Measure-Command {& .\P_Gauss.exe $fatores $Iteracoes}).TotalSeconds
        $cont++
    }
    while ($cont -lt $Samples)
    do {
        $sw = New-Object Diagnostics.Stopwatch
        $timings_M += (Measure-Command {& .\M_Gauss.exe $fatores $Iteracoes}).TotalSeconds
        $cont++
    }
    while ($cont -lt $Samples)
      
    $stats_S = $timings_S | Measure-Object -Average -Minimum -Maximum
    $stats_P = $timings_P | Measure-Object -Average -Minimum -Maximum
    $stats_M = $timings_M | Measure-Object -Average -Minimum -Maximum
    
    switch ($modo) {
        1 {
            Write-Host "Tempo maximo para S_Gauss: $($stats_S.Maximum) ms"
            Write-Host "Tempo maximo para P_Gauss: $($stats_P.Maximum) ms"
            Write-Host "Tempo maximo para M_Gauss: $($stats_M.Maximum) ms"
        }
        2 {
            Write-Host "Tempo maximo para S_Gauss: $($stats_S.Maximum) ms"
            Write-Host "Tempo maximo para P_Gauss: $($stats_P.Maximum) ms"
            Write-Host "Tempo maximo para M_Gauss: $($stats_M.Maximum) ms"
        }
        3 {
            Write-Host "Tempo maximo para S_Gauss: $($stats_S.Maximum) ms"
            Write-Host "Tempo maximo para P_Gauss: $($stats_P.Maximum) ms"
            Write-Host "Tempo maximo para M_Gauss: $($stats_M.Maximum) ms"
        }
        Default {
            Write-Host "Tempo medio para S_Gauss: $($stats_S.Average) s"
            Write-Host "Tempo medio para P_Gauss: $($stats_P.Average) s"
            Write-Host "Tempo medio para M_Gauss: $($stats_M.Average) s"
        }
    }
}

# Roda os 3 programas de Fractal $Samples vezes
function All_Fractal ([int]$hPixel, [int]$wPixel, [int]$Iteracoes, [int]$threadCount, [int]$Samples = 1, [int]$modo = 0) {
    $cont = 0
    $timings_P = @()
    do {
        $timings_P += (Measure-Command {& .\P_Fractal.exe $hPixel $wPixel $Iteracoes $threadCount}).TotalSeconds
        $cont++
    }
    while ($cont -lt $Samples)

    $stats_P = $timings_P | Measure-Object -Average -Minimum -Maximum
    
    switch ($modo) {
        1 {
            Write-Host "Tempo maximo para S_Fractal: $($stats_S.Maximum) ms"
            Write-Host "Tempo maximo para P_Fractal: $($stats_P.Maximum) ms"
            Write-Host "Tempo maximo para M_Fractal: $($stats_M.Maximum) ms"
        }
        2 {
            Write-Host "Tempo maximo para S_Fractal: $($stats_S.Maximum) ms"
            Write-Host "Tempo maximo para P_Fractal: $($stats_P.Maximum) ms"
            Write-Host "Tempo maximo para M_Fractal: $($stats_M.Maximum) ms"
        }
        3 {
            Write-Host "Tempo maximo para S_Fractal: $($stats_S.Maximum) ms"
            Write-Host "Tempo maximo para P_Fractal: $($stats_P.Maximum) ms"
            Write-Host "Tempo maximo para M_Fractal: $($stats_M.Maximum) ms"
        }
        Default {
            Write-Host "Tempo medio para P_Fractal: $($stats_P.Average) s"
        }
    }
    $stats_P
}
