<?php
require("lp-class/SafeSQL.class.php");
require("lp-class/SQLRs.class.php");
require("lp-class/MySQL.class.php");

$uname=$_GET["uname"];
$root="http://jybox.net/bbs/";
$conn=new MySQL();
$rs=$conn->SQL(SafeSQL("SELECT * FROM `jypw_members` WHERE `username`='%1'")->string($uname));
if($rs->read())
{
	$logo=$rs->icon;
	$logo=trim($logo);
	if($logo=="")
		$url=$root."images/face/none.gif";
	else
	{
		$t=explode("|", $logo);
		$logo=$t[0];
		$logo=trim($logo);
		if(@strtolower(@substr($logo,0,7))=="http://")
			$url=$logo;
		elseif($logo=="none.gif")
			$url=$root."images/face/none.gif";
		else
			$url=$root."attachment/upload/middle/".$logo;
	}
}
else
	$url=$root."images/face/none.gif";

//TODO,临时性的...以后改为重定向
header('content-Type:image/jpeg');
echo file_get_contents($url);
?>
