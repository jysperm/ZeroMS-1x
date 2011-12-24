<?php
require("lp-class/SafeSQL.class.php");
require("lp-class/SQLRs.class.php");
require("lp-class/MySQL.class.php");

$uname=$_GET["uname"];
$root="http://jybox.net/bbs/";
$conn=new MySQL();
$rs=$conn->SQL(SafeSQL("SELECT * FROM `jypw_members` WHERE `username`='%1'")->string($uname));
if($rs->read())
	header("Location: {$root}u.php?a=info&uid={$rs->uid}");
?>
