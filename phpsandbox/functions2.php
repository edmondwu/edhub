<html>
	<head>
		<title>Learning PHP: Functions 2  </title>
		
	</head>
	
	<body>
		
		<?php
		
		//using globals to redefine $bar variable (which is global in a local setting)
		
		$bar = "outside"; 
		
		function foo() {
			global $bar; 
			$bar = "inside"; 
		}
		 foo(); 
		 echo $bar."<br />"; 
		
		?>
	
		<?php
		
		//using local variables, arguments and return values
		
		$globalbar = "outside"; 
		
		function foo2($barlocal) {
			$barlocal = "inside"; 
			return $barlocal; 
		}
		
		$newbar = foo2($globalbar); 
		echo $newbar; 
		
		?>
		
		<br />
		
		<?php 
			function paint($room="office", $color="red") {
					echo "The color of the {$room} is {$color}."; 
			}
			paint("blue");
		?>
		
	</body>
	
</html>