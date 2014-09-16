<?php
$act = $_POST['act'];
switch($act){
	case 'filesList':
		$path = 'clients/' + $_POST['server'];
	break;
}
?>