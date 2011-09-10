<html>
	<head>
		<title>Learning PHP: Process Form Page </title>
		
	</head>
	
	<body>
		
		<?php
			$username = $_POST['username'];
			$password = $_POST['password'];
			
			echo "Your username is:".$username."<br />";
			echo "Your password is:".$password;
			
			echo "<br />Your username is: {$username} <br /> Your password is: {$password}"; 
			
		?>
		
	</body>
	
</html>