<?php
$app = true;
require_once('connect.php');

function GetListFiles($folder,&$all_files){
    $fp=opendir($folder);
    while($cv_file=readdir($fp)) {
        if(is_file($folder."/".$cv_file)) {
            $all_files[]=$folder."/".$cv_file;
        }elseif($cv_file!="." && $cv_file!=".." && is_dir($folder."/".$cv_file)){
            GetListFiles($folder."/".$cv_file,$all_files);
        }
    }
    closedir($fp);
}
function dir_size($dirname) {
	$totalsize=0;
	if ($dirstream = @opendir($dirname)) {
	while (false !== ($filename = readdir($dirstream))) {
		if ($filename!="." && $filename!="..")
		{
			if (is_file($dirname."/".$filename))
			$totalsize+=filesize($dirname."/".$filename);
  
			if (is_dir($dirname."/".$filename))
			$totalsize+=dir_size($dirname."/".$filename);
			}
		}
	}
	closedir($dirstream);
	return $totalsize;
}
function get_zip_originalsize($filename) {
    $size = 0;
    $resource = zip_open($filename);
    while ($dir_resource = zip_read($resource)) {
        $size += zip_entry_filesize($dir_resource);
    }
    zip_close($resource);

    return $size;
}


$act = $_POST['act'];
switch($act){
	case 'checkClient':
		$client = $_POST['server'];
		$files = array('/config.zip', '/bin/assets.zip', '/bin/libraries.zip', '/bin/minecraft.jar', '/bin/natives.zip',);
		foreach($files as $file){
			$fileNow = 'clients/'.$client.$file;
			if (!file_exists($fileNow)) {
				die('error');
			}
		}
	break;
	case 'origZipsSize':
		if($_POST['files'] == "") die("error");
		$paths = explode(',',$_POST['files']);
		foreach($paths as $p){
			$file = 'clients/'.$_POST['server'].'/'.$p;
			echo $p.'-'.get_zip_originalsize($file).',';
		}
	break;
	case 'filesList':
		$path = 'clients/'.$_POST['server'];
		$all_files=array();
		
		GetListFiles($path,$all_files);
		$filesList = str_replace($path, "", implode(",", $all_files));
		exit($filesList);
	break;
	case 'filesSize':
		$files = explode(",", $_POST['files']);
		$totalSize = 0;
		foreach($files as $file){
			$fileOne = "clients/".$_POST['server'].$file;
			$totalSize = $totalSize + filesize($fileOne);
		}
		echo $totalSize;exit;
	break;
	case 'pathSizes':
		if($_POST['path'] == "") die("error");
		$dirs = explode("-", $_POST['path']);
		$ret = "";
		foreach($dirs as $d){
			$dir = "clients/".$_POST['server'].'/'.$d;
			$ret = $ret.$d."-".dir_size($dir).",";
		}
		echo $ret;exit;
	break;
	case 'fileSizes':
		if($_POST['file'] == "") die("error");
		$files = explode("-", $_POST['file']);
		$ret = "";
		foreach($files as $f){
			$file = "clients/".$_POST['server'].'/'.$f;
			$ret = $ret.$f."-".filesize($file).",";
		}
		echo $ret;exit;
	break;
}
?>