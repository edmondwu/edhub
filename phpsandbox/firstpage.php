<html>
	<head>
		<title>Learning PHP: First Page  </title>
		
	</head>
	
	<body>
		
		<?php
			$linktext="<Click> & you'll go to the second page"; 
		?>
		
		<a href="secondpage.php?name=<?php echo urlencode("edmond wu")?>&id=1">
		
		<?php echo htmlspecialchars($linktext); ?>
		
		</a>
		
		<?php
		
		
		?>
		
	</body>
	
</html>