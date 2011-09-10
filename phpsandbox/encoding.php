<html>
	<head>
		<title>Learning PHP: Encoding  </title>
		
	</head>
	
	<body>
		
		<?php
			$url_page = 'php/created/page/url.php';
			$param1 = 'this is a string';
			$param2 = '"bad"/<>character$';
			$linktext="<Click> & you'll go to the second page"; 
		?>
		
		<?php
			//this gives you a clean link to use
			$url = "http://localhost/";
			$url .= rawurlencode($url_page);
			echo $url."<br />"; 
			$url .= "?param1=". urlencode($param1);
			echo $url."<br />";  
			$url .= "&param2=" .urlencode ($param2);
			echo $url."<br />"; 
			
			//htmlspecialchars escapes any html that might do bad things to your page
		?>
		
		<a href="<?php echo htmlspecialchars($url); ?>">
		<?php echo htmlspecialchars($linktext); ?>
		</a>
		
		<?php
			echo htmlspecialchars($url)."<br />"; 	
		?>
		
		
	</body>
	
</html>

http://localhost/php/created/page/url.php?param1=this+is+a+string&param2=