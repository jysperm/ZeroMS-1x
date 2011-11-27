<?php
/*
**		--数据库连接类
**依赖:SQLRs
*/
class MySQL
{
	//配置信息数组
	var $cfgs;
	//数据库连接对象
	var $c=0;
	//是否使用了持久性连接
	var $pc;
	function __construct($h=0,$n=0,$u=0,$p=0,$c=0,$o=0)
	{
		if(!$h)
		{
			$this->cfgs=parse_ini_file("dbCfg.ini.php");
		}
		else
		{
			$this->cfgs["host"]=$h;
			$this->cfgs["dbname"]=$n;
			$this->cfgs["user"]=$u;
			$this->cfgs["pwd"]=$p;
			$this->cfgs["charset"]=$c;
			$this->cfgs["collate"]=$o;
		}
	}
	function open($p=0)
	{
		//参数表示是否使用持久性连接
		//根据我的理解，持久性连接会在PHP程序结束后被继续保存很长一段时间(当然，也要占内存的)
		//所以在需要反复连接同一数据库的时候比较有用
		//注意：使用持久性连接的时候，MySQL类不会自动的关闭连接，需要您在需要的时候手动调用close()函数
		if(!$this->ping())
		{
			$this->pc=$p;
			if($p)
			{
				$this->c=mysql_pconnect($this->cfgs["host"],$this->cfgs["user"],$this->cfgs["pwd"])
					or die("无法连接到数据库");
			}
			else
			{
				$this->c=mysql_connect($this->cfgs["host"],$this->cfgs["user"],$this->cfgs["pwd"])
					or die("无法连接到数据库");
			}
			mysql_query("SET NAMES ".$this->cfgs["charset"]);
			$l = mysql_select_db($this->cfgs["dbname"],$this->c) or die("打开数据库失败");
		}
	}
	function ping()
	{
		//该函数用来检查数据连接是否还存在，相比与上一版直接检查$this->c要更为准确
		return @mysql_ping($this->c);
	}
	function SQL($s,$r=1)
	{
		if(@get_class($s)=="SafeSQL")
			$s=$s->get();
		if(!$this->ping())
			$this->open();
		if($r)
			return new SQLRs(mysql_query($s, $this->c));
		else
			return mysql_query($s, $this->c);
	}
	function reStart()
	{
		$this->close();
		$this->open($this->pc);
	}
	function dbs()
	{
		//该函数返回一个数组，包含了这个数据连接中所有的数据库名
		$r=mysql_list_dbs($this->c);
		$s=array();
		while($a=mysql_fetch_row($r))
			$s=array_merge($s,$a);
		return $s;
	}
	function tables()
	{
		//该函数返回一个数组，包含了这个数据连接中所有的表名
		$r=mysql_list_tables($this->c);
		$s=array();
		while($a=mysql_fetch_row($r))
			$s=array_merge($s,$a);
		return $s;
	}
	function close()
	{
		if($this->ping())
		{
			@mysql_close($this->c);
			$this->c=0;
		}
	}
	function __destruct()
	{
		if(!$this->pc)
			$this->close();
	}
	function debug()
	{
		print_r($this);
	}
}
?>
