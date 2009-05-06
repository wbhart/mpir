# Translation of gmpbench script 'runbench' for Windows PowerShell

$dir = "win32\release"

$multiply_args = (128,128),(512,512),(8192,8192),(131072,131072),(2097152,2097152)
$divide_args   = (8192,32),(8192,64),(8192,128),(8192,4096),(8192,8064),(131072,8192),(131072,65536),(8388608,4194304)
$rsa_args      = 512,1024,2048
$base_tests    = "multiply","divide"
$app_tests     = "rsa"
$tests         = "base","app"
"Running benchmarks" | Out-Host
$acc2 = 1
$n2=0
foreach ($cat in $tests)
{
	"  Category $cat" | Out-Host
	$yy = "`$"+$cat+"_tests"
	$xx = Invoke-Expression $yy
	$acc1 = 1
	$n1 = 0
	foreach ($t in $xx)
   	{
		"    Program $dir\$t" | Out-Host
		$yy = "`$"+$t+"_args"
		$args = Invoke-Expression $yy
		$acc = 1
		$n = 0
		for($i = 0; $i -lt $args.length; $i++)
		{
			if($args[$i].length -eq 2)
			{
				$parms = $args[$i][0]
				$parms = "$parms" + " " + $args[$i][1]
			}
			else
			{
				$parms = $args[$i]
			}
			$pcm = $parms -replace ' ', ','
	    	"      $t $parms" | Out-Host
			$out = Invoke-Expression ".\$dir\$t $parms" 
			foreach ($s in $out) { if($s -match "RESULT: [0-9]*") { $res = $s.Split(' ')[1] } }
			"      GMPbench.$cat.$t.$pcm result: $res" | Out-Host
			$acc = Invoke-Expression ".\$dir\gexpr -prec 10 `"$acc*$res`""
			$n++
		}
		$out = Invoke-Expression ".\$dir\gexpr `"$acc^(1/$n)`""
		"    GMPbench.$cat.$t result: $out" | Out-Host
		$acc1 = Invoke-Expression ".\$dir\gexpr -prec 10 `"$acc1*$acc^(1/$n)`""
		$n1++
	}
	$out = Invoke-Expression ".\$dir\gexpr `"$acc1^(1/$n1)`""
	"  GMPbench.$cat result: $out" | Out-Host
	$acc2 = Invoke-Expression ".\$dir\gexpr -round -prec 10 `"$acc2*$acc1^(1/$n1)`""
	$n2++
}
$out = Invoke-Expression ".\$dir\gexpr -round -prec 10 `"$acc2^(1/$n2)`""
"GMPbench result: $out" | Out-Host
