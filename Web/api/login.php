<?php
require("lp-class/SafeSQL.class.php");
require("lp-class/SQLRs.class.php");
require("lp-class/MySQL.class.php");

$pkey="0-ms-0.1";

if($_GET["mode"]=="post")
{
    switch($_POST["do"])
    {
        case "login":
            $conn=new MySQL();
            $rs=$conn->SQL(SafeSQL("SELECT * FROM `jypw_members` WHERE `username`='%1'")->string($_POST["uname"]));
            $rs->read();
            $time=$_POST["time"];
            $time=$time-($time%10);
            if($_GET["time"]=="yes")
                $spwd=md5(md5($time).md5($_POST["uname"]).($rs->password));
            else
                $spwd=md5(md5($_POST["uname"]).md5($rs->password));
            $dpwd=md5($pkey.$dpwd);
            if($_POST["pwd"]==$dpwd)
            {
                echo "1\n";
            }
            else
            {
                echo "0\n";
            }
            echo $_POST["uname"]."\n";
            echo $_POST["listname"]."\n";
            echo $_POST["clientver"]."\n";
            echo $_POST["clientname"]."\n";
            break;
    }
}
?>
