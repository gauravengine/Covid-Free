<?php
class chartapp_patient{
 public $link='';
 function __construct($spo2, $bpm){
  $this->connect();
  $this->storeInDB($spo2, $bpm);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'django_db_v1') or die('Cannot select the DB');
 }
 
 function storeInDB($spo2, $bpm){
  $query = "insert into chartapp_patient set bpm='".$bpm."', spo2='".$spo2."', name='nsut', state_id='1' ";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }
 
}
if($_GET['spo2'] != '' and  $_GET['bpm'] != ''){
 $chartapp_patient=new chartapp_patient($_GET['spo2'],$_GET['bpm']);
}
?>
