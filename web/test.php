<?php
$client = 'Vanilla';
$files = array('/config.zip', '/bin/assets.zip', '/bin/libraries.zip', '/bin/minecraft.jar', '/bin/natives.zip',);
foreach($files as $file){
	$fileNow = 'clients/'.$client.$file;
	if (!file_exists($fileNow)) {
		die('error');
	}
}
?>