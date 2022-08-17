<html>
<body>

<?php

$dbname = 'django_db_v1';
$dbuser = 'root';  
$dbpass = ''; 
$dbhost = 'localhost'; 

$connect = @mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

if(!$connect){
	echo "Error: " . mysqli_connect_error();
	exit();
}

echo "Connection Success!<br><br>";

$spo2 = $_GET["spo2"];
$bpm = $_GET["bpm"]; 


$query = "INSERT INTO  chartapp_patient (name,spo2, bpm,state_id) VALUES ('new_test','$spo2', '$bpm','1')";
$result = mysqli_query($connect,$query);

echo "Insertion Success!<br>";

?>
</body>
</html>
