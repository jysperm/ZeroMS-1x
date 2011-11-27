<?php
/*
**		--安全SQL类	
**依赖:Options SQLRs MySQL
*/
//该类提供了一系列机制以保护SQL查询的安全性，防止SQL注入

//无论如何，单引号都会被转义，这是最基本的安全措施，除此之外，还通过一系列的成员函数来限定数据的类型的取值范围
function SafeSQL($s)
{
	//好像PHP并不支持某些C++语法，所以要增加这样一个函数，以直接用类名而不是new来创建类
	return new SafeSQL($s);
}
//下面是四种处理异常数据的方法
define("LPSQL_DIE",1);		//停止执行
//下面两个未完全实现，不推荐使用
define("LPSQL_LOG_DIE",2);		//记录注入信息(将记录到配置文件的SafeSQLLog一项中)，并停止执行
define("LPSQL_LOG_CALLBACK",3);		//记录注入信息(将记录到配置文件的SafeSQLLog一项中)，并调用回调函数
define("LPSQL_CALLBACK",4);	//执行名为sqlCallback的回调函数(比如可以在回调函数里刷新页面等等)
class SafeSQL
{
	//如何处理异常数据，从上面的常量中取值
	static $err;
	//SQL语句
	var $sql;
	//转义序列
	var $z;
	function esc()
	{
		//转义
		$zz=1;
		while(!(strrpos($this->sql,str_repeat("\n",$zz))===false))
			$zz++;
		$this->sql=str_replace("%%",str_repeat("\n", $zz),$this->sql);
		$this->z=$zz;
	}
	function unesc()
	{
		//反转义
		$this->sql=str_replace(str_repeat("\n", $this->z),"%%",$this->sql);
	}
	function __construct($s)
	{
		$this->sql=$s;
	}
	function int($n,$min="n",$max="n")
	{
		//合法性判断
		preg_match("/^[0-9]*$/",$n,$r);
		if(!$r[0])
		{
			$this->err("int数据异常：$n");
			return;
		}
		if($min!="n")
			if(!($n>$min))
			{
				$this->err("int数据异常：$n");
				return;
			}
		if($max!="n")
			if(!($n<$max))
			{
				$this->err("int数据异常：$n");
				return;
			}
		$this->esc();
		//执行替换
		//TODO(2011.9.11-Jybox)暂时只支持10个占位符
		$i=0;
		while(strrpos($this->sql,"%$i")===false)
		{
			if($i>10)
				break;
			$i++;
		}
		$this->sql=str_replace("%$i", $n, $this->sql);
		$this->unesc();
		//返回值
		return $this;
	}
	function double($n,$min="n",$max="n")
	{
		//合法性判断
		preg_match("/^-?([1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0)$/",$n,$r);
		if(!$r[0])
		{
			$this->err("double数据异常：$n");
			return;
		}
		if($min!="n")
			if(!($n>$min))
			{
				$this->err("double数据异常：$n");
				return;
			}
		if($max!="n")
			if(!($n<$max))
			{
				$this->err("double数据异常：$n");
				return;
			}
		$this->esc();
		//执行替换
		$i=0;
		while(strrpos($this->sql,"%$i")===false)
		{
			if($i>10)
				break;
			$i++;
		}
		$this->sql=str_replace("%$i", $n, $this->sql);
		$this->unesc();
		//返回值
		return $this;
	}
	function string($n,$c=1,$u=1,$d=0)
	{
		//三个可选参数分别是是否支持中文，是否支持下划线，是否支持小数点
		
		//合法性判断
		preg_match($c?"/^[\x80-\xffA-Za-z0-9\._]*$/":"/^[A-Za-z0-9\._]*$/",$n,$r);
		if(!$r[0])
		{
			$this->err("string数据异常：$n");
			return;
		}
		if($u)
			if(!(strrpos($n,"_")===false))
			{
				$this->err("string数据异常：$n");
				return;
			}
		if($d)
			if(!(strrpos($n,".")===false))
			{
				$this->err("string数据异常：$n");
				return;
			}
		$this->esc();
		//执行替换
		$i=0;
		while(strrpos($this->sql,"%$i")===false)
		{
			if($i>10)
				break;
			$i++;
		}
		$this->sql=str_replace("%$i", $n, $this->sql);
		$this->unesc();
		//返回值
		return $this;
	}
	function text($n)
	{
		//该函数几乎不会对参数进行什么处理(所以当你不想让SafeSQL处理参数的时候就可以用它)，只是会对一些特殊符号进行转义，比如单引号
		//注意，是转义不是删除，转义对SQL执行结果没有任何影响
		$n=mysql_escape_string($n);
		$this->esc();
		//执行替换
		$i=0;
		while(strrpos($this->sql,"%$i")===false)
		{
			if($i>10)
				break;
			$i++;
		}
		$this->sql=str_replace("%$i", $n, $this->sql);
		$this->unesc();
		//返回值
		return $this;
	}
	function rx($n,$rx)
	{
		//该函数将使用用户提供的正则表达式来检查参数，注意正则表达式两端都应该有斜杠
		
		//合法性判断
		preg_match($rx,$n,$r);
		if(!$r[0])
		{
			$this->err("rx：$n($rx)");
			return;
		}
		$n=mysql_escape_string($n);
		$this->esc();
		//执行替换
		$i=0;
		while(strrpos($this->sql,"%$i")===false)
		{
			if($i>10)
				break;
			$i++;
		}
		$this->sql=str_replace("%$i", $n, $this->sql);
		$this->unesc();
		//返回值
		return $this;
	}
	function get()
	{
		return str_replace("%%", "%", $this->sql);
	}
	function err($s="")
	{
		switch(SafeSQL::$err)
		{
			case LPSQL_REPOT:
				echo $s;
				break;
			//下面两个未完全实现，不推荐使用
			case LPSQL_LOG_DIE:
			case LPSQL_LOG_CALLBACK:
				$o=new Options;
				$o->SafeSQLLog=$o->SafeSQLLog.$s."\n";
				if(SafeSQL::$err==LPSQL_LOG_DIE)
					die($s);
			case LPSQL_CALLBACK:
				sqlCallback();
				break;
			case LPSQL_DIE:
			default:
				die($s);
		}
	}
	function debug()
	{
		print_r($this);
	}
}
?>
