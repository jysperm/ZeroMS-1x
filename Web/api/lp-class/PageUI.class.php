<?php
/*
**		--页面框架类	
**		该文件负责实现了页面框架
*/
class PageUI
{
	static $_config;
	function init()
	{
		//初始化是必须的，否则在遍历配置数组的时候会找不到这个健名
		PageUI::$_config["dirdepth"]=0;
		PageUI::$_config["title"]="";
		PageUI::$_config["header"]="";
		PageUI::$_config["keywords"]="";
		PageUI::$_config["description"]="";
		PageUI::$_config["infotip"]=NULL;
	}
	function __get($kname)
	{
		return PageUI::$_config[$kname];
	}
	function __set($kname,$value)
	{
		PageUI::$_config[$kname]=$value;
	}
	function addHeader($h)
	{
		PageUI::$_config["header"].=$h;
	}
	static function obCallBack($content)
	{
		$dirdepth=PageUI::$_config["dirdepth"];
		unset(PageUI::$_config["dirdepth"]);
		if(PageUI::$_config["infotip"]==NULL)
		{
			PageUI::$_config["infotip"]=defInfotip();
		}
		$o=new Options();
		$out=file_get_contents(dirname(__FILE__)."/../lp-style/$o->ui_theme/frame.htm.php");
		$menu=file_get_contents(dirname(__FILE__)."/../lp-style/menu.htm.php");					
		$footer=file_get_contents(dirname(__FILE__)."/../lp-style/footer.htm.php");				
		$out=str_ireplace('<!--$~menu-->', $menu,$out);
		$out=str_ireplace('<!--$~footer-->', $footer,$out);
		$out=str_ireplace('<!--$~body-->', $content,$out);
		foreach(PageUI::$_config as $kname => $value)
		{
			$out=str_replace('<!--$~'.$kname.'-->', $value, $out);
		}
		$cfg=parse_ini_file(dirname(__FILE__)."/../{$o->_from}");
		foreach($cfg as $kname => $value)
		{
			$out=str_replace('<!--$~cfg:'.$kname.'-->', $value, $out);
		}
		$out=str_ireplace('<!--$~dirdepth-->', str_repeat("../", $dirdepth),$out);
		return $out;
	}
}
?>
