<?php

$uploaddir = '/home/ace/cvs/mtp-target/user_texture/';
$uploadfilename = strtolower($_FILES['userfile']['name']);
$uploadfile = $uploaddir . $uploadfilename;
$maxfilesize = 300000; // some tga are bigger because they add some dumb extra infos

if($_FILES['userfile']['size'] > $maxfilesize)
{
   die("FAILED! The file you tried to upload is too big (can't be more than ".maxfilesize." bytes)");
}

if(file_exists($uploadfile) || $uploadfilename == "ping_ball_blue.tga")
{
   die("FAILED! The file you tried to upload already exists, please, use another name");
}

if(!ereg("ping_ball_([[:alnum:]]+).tga", $uploadfilename, $regs))
{
   die("FAILED! The file you tried to upload doesn't follow the file name rulez (for example: ping_ball_ryzom.tga), please, use another name");
}

if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile))
{
   system("convert ".$uploaddir."ping_ball_".$regs[1].".tga ".$uploaddir."ping_ball_".$regs[1].".jpg");

   echo "<p>The file '".$uploadfilename."' was uploaded. Don't forget  to add the following line in your mtp_target.cfg: ";
   echo '<pre>EntityTexture = "'.$regs[1].'";</pre></p>';
}
else
{
   die("FAILED! Possible file upload attack!");
}

?>
