<?php

error_reporting(0);

require_once("../../config/config.php");
require_once("../../core/lib/vendor/phpass/PasswordHash.php");

$dbh = mysqli_connect($config["esoTalk.database.host"], $config["esoTalk.database.user"], $config["esoTalk.database.password"], $config["esoTalk.database.dbName"]);

$uname = mysqli_real_escape_string($dbh, $_POST["uname"]);

$res = mysqli_query($dbh, "SELECT * FROM `et_member` WHERE `username`='{$uname}'");
$row = mysqli_fetch_assoc($res);

$result = [
    "uname" => $_POST["uname"],
    "listname" => $_POST["listname"],
    "clientver" => $_POST["clientver"],
    "clientname" => $_POST["clientname"]
];

$hasher = new PasswordHash(8, FALSE);
if($hasher->CheckPassword($_POST["pwd"], $row["password"]))
    $result["error"] = false;
else
    $result["error"] = true;

echo json_encode($result);

