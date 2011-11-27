<?php
/*
**		--键值对应类
**		该类提供了一种键值对应的储存机制，用于保存LightPHP的设置信息，也可供用户使用
**		该类支持数据库和INI文件两种储存方法
*/
class Options
{
	var $_conn=NULL;
	var $_from=NULL;
	var $_safe=1;
	function __construct($cf=NULL,$f=NULL,$safe=1)
	{
		$this->_safe=$safe;
		global $cfgFrom;
		$cf=strtoupper($cf);
		if($cf==NULL)
		{
			$cf=$cfgFrom;
		}
		if($cf=="DB")
		{
			$this->_conn=new MySQL();
		}
		if($f==NULL)
		{
			global $cfgFromName;
			$f=$cfgFromName;
		}
		$this->_from=$f;
	}
	function __get($kname)
	{
		if($this->_conn)
		{
			$rs=$this->_conn->SQL(SafeSQL("SELECT * FROM `%1` WHERE `k`='%2'")->string($this->_from)->string($kname));
			if($rs->getNum())
			{
				$rs->read();
				return $rs->v;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			$rs=parse_ini_file(dirname(__FILE__)."/../{$this->_from}");
			return str_replace("\\n","\n",$rs[$kname]);
		}
	}
	function __set($kname,$value)
	{
		if($this->_conn)
		{
			$rs=$this->_conn->SQL(SafeSQL("SELECT * FROM `%1` WHERE `k`='%2'")->string($this->_from)->string($kname));
			if($rs->getNum())
			{
				$this->_conn->SQL(SafeSQL("UPDATE `%1` SET `v`='%2' WHERE `k`='%3'")->string($this->_from)->text($value)->string($kname));
			}
			else
			{
				$this->_conn->SQL(SafeSQL("INSERT INTO `%1` (`k`,`v`) VALUES('%2','%3'")->string($this->_from)->string($kname)->text($value));
			}
		}
		else
		{
			$rs=parse_ini_file(dirname(__FILE__)."/../{$this->_from}");
			$rs[$kname]=$value;
			$out="[lp-config]\n";
			foreach($rs as $k => $v)
			{
				$out=$out.$k."=".str_replace("\n","\\n",$v)."\n";
			}
			if($this->_safe)
			{
				$out=";<?php /*\n".$out.";*/ ?>\n";
			}
			file_put_contents(dirname(__FILE__)."/../{$this->_from}",$out);
		}
	}
	function debug()
	{
		print_r($this);
	}
}
?>
