<?php
$app = true;
require_once('connect.php');

$act = $_POST['act'];
switch($act){
case "auth":
	$plogin = mysql_real_escape_string(trim(htmlspecialchars($_POST['login'])));
	$ppass = mysql_real_escape_string(trim($_POST['password']));
	
	if(!empty($plogin) && !empty($ppass)){
		$query = mysql_query("SELECT * FROM `$table_db` WHERE `$login_db` = '$plogin' AND `$password_db` = '$ppass'") or die('queryError');
		$res = mysql_fetch_array($query);
		
		$hash = $res[$password_db];
		$session = rand(999999, 9999999999);
		$query = mysql_query("UPDATE `$table_db` SET `$session_db` = '$session' WHERE `$login_db` = '$plogin'") or die("sqlError");
		
		if($ppass == $hash){
			exit($hash.'<>'.$plogin.'<>'.$session);
		}else die("Bad Login");
		
	}else die("LoginPassError");
break;
case "reg":	
	$rlogin = mysql_real_escape_string(htmlspecialchars(trim($_POST['login'])));
	$rpassword = mysql_real_escape_string(trim($_POST['password']));
	$ip = $_SERVER['REMOTE_ADDR'];
	
	$query = mysql_query("SELECT * FROM `$table_db` WHERE `$login_db` = '$rlogin' AND `$password_db` = '$rpassword'") or die('queryError1');
	$res = mysql_fetch_array($query);
	if(!empty($res['login'])) die('userExists');
	
	$query = mysql_query("INSERT INTO `$table_db` (`$login_db`,`$password_db`,`$ip_db`) VALUES('$rlogin', '$rpassword', '$ip')") or die('queryError2');
	exit('regComplete');
break;
}
?>