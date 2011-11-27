<?php
/*
**		--数据集类
**依赖:无
*/
//TODO(2011.9.11-Jybox)添加列举字段的函数
class SQLRs
{
	var $_rs;
	var $_row;
	var $_seek=0;
	function __construct($s=NULL)
	{
		if($s)
		{
			$this->_rs=$s;
			$this->_seek=0;
		}
	}
	function __get($name)
	{
		return $this->_row[$name];
	}
	function seek($s=0)
	{
		//注意，在调用该函数后应该手动调用read函数读取数据，否则通过__get取得的数据是过时的
		$this->_seek=$s;
		return @mysql_data_seek($this->_rs,$s);
	}
	function read()
	{
		$r=mysql_fetch_array($this->_rs);
		if($r)
		{
			$this->_row=$r;
			$this->_seek=$this->_seek+1;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	function num()
	{
		//上一版中该函数名为getNum，现在改名了，请注意
		return @mysql_num_rows($this->_rs);
	}
	function close()
	{	
		@mysql_free_result($this->_rsID);
		$this->_rs=NULL;
		$this->_row=NULL;
	}
	function __destruct()
	{
		$this->close();
	}
	function debug()
	{
		print_r($this);
	}
}
?>
