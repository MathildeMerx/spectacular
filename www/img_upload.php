<?php
/* ---------------------------------------------------------------
 *
 * FILENAME     : img_upload.php
 * CREATION     : 29/01/2021
 * AUTHOR       : Luca Jakomulski
 * PURPOSE      : 
 * 
 * --------------------------------------------------------------- */

if (isset($_FILES["image"]) && $_FILES["image"]["error"] == 0) {

  $img_name = uniqid() . ".png";
  $img_path = "../img/" . $img_name;

  $file = fopen("../files/lastid", "r");
  $last_id = rtrim(fgets($file, 200), "\r\n");
  fclose($file);

  $file = fopen("../files/match", "a");
  fwrite($file, $last_id . ":" . $img_name . "\n");
  fclose($file);

  echo $last_id . ":" . $img_name;

  move_uploaded_file($_FILES["image"]["tmp_name"], $img_path);

  echo "<p>Your image has been uploaded.</p>";
  echo "<p><a href=\"index.html\">Add an image.</a></p>";
  
}
else {
  echo "<p>An error occured when uploading your image (error code : ". $_FILES["image"]["error"] . ").</p>";
  echo "<p><a href=\"index.html\">Please retry.</a></p>";
}


?>