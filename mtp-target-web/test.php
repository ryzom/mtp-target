<?php
include_once("stat_function.php");
include_once("stat_game.php");

	$uid = userName2Uid($user_login);
	//$uid = 5453;
	$cacheFileName = $cache_dir."/".$page.".html";
	
	/*
	if(isCacheFileUpToDate($cacheFileName))
	{
		include($cacheFileName);
		return;
	}
	*/
	
	$html_fp = fopen($cacheFileName, "wt");


	$requete = "SELECT user.Login,SUM(user_session.Score) as s FROM session,user_session,user WHERE session.Id=user_session.SessionId AND user_session.UId=user.UId AND TO_DAYS(NOW())-TO_DAYS(session.Date)=30 GROUP BY user_session.UId ORDER BY s DESC LIMIT 0,1 ;";
	$result=exec_game_db_requete($requete);
	if($line = mysql_fetch_array($result))
	{
		printf("%s %s<br>",$line[0],$line[1]);
	}


	fclose($html_fp);	  
	include($cacheFileName);	
?>
