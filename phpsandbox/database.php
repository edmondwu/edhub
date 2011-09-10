<?php 
// 1. Create a database connection
$connection = mysql_connect("localhost", "root", "");
if (!$connection) {
	die("Database connection failed:" . mysqlerror());
}

// 2. Select a database to use
$db_select = mysql_select_db("widget", $connection);
if (!$db_select) {
	die("Database selection failed" . mysql_error()); 
}

?>

<html>
	<head>
		<title>Databases </title>
		
	</head>
	
	<body>
		
		<?php
			// 3. Perform a database query
			$result = mysql_query("SELECT * FROM subjects", $connection);
			if ($result) 
				echo "Query Performed"; 
           else 
				die ("Database query failed: " . mysql_error());
			
			// 4. User returned data
			while ($row = mysql_fetch_array($result)) {
				echo $row["menu_name"]."-".$row["id"]."<br />";
			}
		?>
		
		
		
	</body>
	
</html>
