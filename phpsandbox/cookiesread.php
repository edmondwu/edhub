<?php setcookie('test', 0, time()-(60*60*24*7)); ?>

<html>
	<head>
		<title>Learning PHP: Cookies READ</title>
		
	</head>
	
	<body>
	
	<h2> READING a cooke named "test"</h2>
	<br />
	
	<?php
		$var1 = 0; 
		if (isset($_COOKIE['test'])) {
			$var1=$_COOKIE['test'];
		}
		echo $var1; 
	?> 
	
		
	</body>
	
</html>

