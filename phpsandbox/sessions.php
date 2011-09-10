<?php
	session_start();  //tells php to get the id or session file if not assign one
	// it will provide a PHPSESSID  or session ID which is very long which makes if difficult
?>

<html>
	<head>
		<title>Learning PHP: Sessions</title>
		
	</head>
	
	<body>
	
	<h2> SESSIONS </h2>
	<br />
	
	<?php
		$_SESSION['first_name']="edmond";
		$_SESSION['last_name']="wu";
	?> 
	
	<?php
		$name = $_SESSION['first_name']." ".$_SESSION['last_name'];
		echo $name;
	?>
	
		
	</body>
	
</html>

