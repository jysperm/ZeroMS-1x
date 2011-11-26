<?php 
/*
**		--载入文件	
**		该文件负责载入LightPHP的所有组件。一般情况下，使用LightPHP只需包含这一个文件
**		有关LightPHP的详情请参考包含在源码包里的"使用说明.pdf"
*/

//-------初始化设置，路径相对于该文件
//配置信息的默认来源方式(DB或FILE)
$cfgFrom="FILE";
//配置信息的位置(DB则指定表名；FILE则指定文件名)
$cfgFromName="lp-config.ini.php";
//$cfgFromName="lp_config";


/*
//-------建表SQL
SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
CREATE TABLE `lp_config` (
  `k` longtext NOT NULL,
  `v` longtext
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
INSERT INTO `lp_config` (`k`, `v`) VALUES
('ui_skin', 'default'),
('site_domain', '127.0.0.1'),
('site_name', 'LightPHP'),
('SafeSQLLog', ''),
('lp_ver', '2.0.1[3]-2011.9.20'),
('site_author', 'jybox');
*/

//-------引入类文件
require("lp-class/SafeSQL.class.php");
require("lp-class/MySQL.class.php");
require("lp-class/SQLRs.class.php");
require("lp-class/Options.class.php");
require("lp-class/PageUI.class.php");

//-------各回调函数
function defInfotip()
{
	return '该站点使用<a href="http://jybox.net/soft/LightPHP"><span class="BigO">LightPHP</span></a>构建';
}
function sqlCallback()
{
	header("location:http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']);
	exit();
}

//$page更名为$lpage,避免重名
$lpage=0;
uiType();
function defobCallBack($content)
{
	$lpage = new PageUI();
	return $lpage->obCallBack($content);
}
function uiType($typeName="Page")
{
	global $lpage;
	$typeName=strtoupper($typeName);
	if($typeName=="PAGE")
	{
		$lpage = new PageUI();
		$lpage->init();
		ob_start("defobCallBack");
	}
	elseif($typeName=="SERVER")
	{
		$lpage=NULL;
		ob_end_clean();
	}
}
?>
