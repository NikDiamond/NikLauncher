<?php
if($app != true) die('403 Forbid_dbden');
	ini_set('error_reporting', E_ALL);
	ini_set('display_errors', 1);
	ini_set('display_startup_errors', 1);
	
	$base = 'niklauncher';   //База данных
	$charset = 'utf8'; //Кодировка вывода из базы
	$host = 'localhost'; //Хост
	$user = 'root';		 //Имя пользователя
	$pass = '';			 //Пароль
	
	$table_db = 'users';
	$id_db = 'id';
	$ip_db = 'ip';
	$login_db = 'login';
	$password_db = 'pass';
	$session_db = 'session';
	
	$version = '0.01';
	if($_SERVER['HTTP_USER_AGENT'] != $version) die("wrongClientVersion");

	$db = @mysql_connect($host,$user,$pass) or die('Ошибка базы данных: connect.php'); //Подключение к базе
	mysql_select_db($base,$db);
	mysql_query("SET NAMES '$charset'");
?>