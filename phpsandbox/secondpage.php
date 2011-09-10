<html>
	<head>
		<title>Learning PHP: First Page  </title>
		
	</head>
	
	<body>
		
		<?php
			
			print_r($_GET); // prints values that is gotten in array
			$id = $_GET['id'];
			$name = $_GET['name'];
			echo "<br />";
			echo $id.": ".$name; 
		
		?>
		
		<br />
		
		<?php
		
			$string = "kevin skoglund"; 
			echo urlencode($string); 
			echo "<br />"; 
			echo rawurlencode($string); 
		?>
		
	</body>
	
</html>