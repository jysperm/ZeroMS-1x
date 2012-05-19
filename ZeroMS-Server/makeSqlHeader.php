<pre>
<?php
  $conn=mysql_connect("127.0.0.1","root","newpasswd");
  mysql_query("SET NAMES utf8",$conn);
  mysql_select_db("zeroms_m",$conn);
  
  $tables=array("user","group","userlist","group_member","userrequest","msg_log");
  
  foreach($tables as $table)
  {    
?>
class <?php echo S($table);?>

{
public:
    <?php echo S($table);?>():_isEmpty(true)
    {
        
    }
    <?php echo S($table);?>(QSqlQuery *query):
<?php
    $rs=mysql_query("SHOW COLUMNS FROM  `{$table}`",$conn);
    $i=-1;
    while($row=mysql_fetch_array($rs))
    {
      $i++;
?>
        <?php echo S($row["Field"],1);?>(query->value(<?php echo $i;?>).to<?php echo TT($row["Type"]);?>()),
<?php
    }
?>
        _isEmpty(true){}

<?php
    $rs=mysql_query("SHOW COLUMNS FROM  `{$table}`",$conn);
    while($row=mysql_fetch_array($rs))
    {
?>
    <?php echo T($row["Type"]);?> <?php echo S($row["Field"],1);?>;
<?php
    }
?>

    static QString _table()
    {
        return "<?php echo $table;?>";
    }
    
    QVector&lt;QPair&lt;QString,QString&gt; &gt; _values()
    {
        QVector&lt;QPair&lt;QString,QString&gt; &gt; _result_;
<?php
    $rs=mysql_query("SHOW COLUMNS FROM  `{$table}`",$conn);
    while($row=mysql_fetch_array($rs))
    {
      if(T($row["Type"])=="QString")
      {
?>
        _result_.append(qMakePair("<?php echo S($row["Field"],1);?>",<?php echo S($row["Field"],1);?>));
<?php
      }
      else
      {
?>
        _result_.append(qMakePair("<?php echo S($row["Field"],1);?>",QString::number(<?php echo S($row["Field"],1);?>)));
<?php
      }
    }
?>
        return _result_;
    }
    
    bool _isEmpty;
};

<?php
  }
?>





<?php
  }
  
  function S($string,$n=0)
  {
    $string=str_replace("userlist","UserList",$string);
    $string=str_replace("user","User",$string);
    if($n==1)
      $string=str_replace("User","user",$string);
    $string=str_replace("group","Group",$string);
    $string=str_replace("lastlogintime","lastLoginTime",$string);
    $string=str_replace("lastloginip","lastLoginIp",$string);
    $string=str_replace("regtime","regTime",$string);
    $string=str_replace("onlinetime","onlineTime",$string);
    $string=str_replace("group","Group",$string);
    $string=str_replace("handletime","handleTime",$string);
    $string=str_replace("ishandle","isHandle",$string);
    $string=str_replace("user_request","UserRequest",$string);
    $string=str_replace("isdeny","isDeny",$string);
    $string=str_replace("isadmin","isAdmin",$string);
    $string=str_replace("issign","isSign",$string);
    $string=str_replace("signtime","signTime",$string);
    $string=str_replace("msg_log","MsgLog",$string);
    
    $string=str_replace("Groupname","groupname",$string);
    $string=str_replace("Group_member","GroupMember",$string);
    
    return $string;
  }
  
  function T($string)
  {
    if(stristr($string,"tinyint"))
    {
      return "bool";
    }
    if(stristr($string,"int"))
    {
      if(stristr($string,"unsigned"))
        return "unsigned int";
      return "int";
    }
    if(stristr($string,"varchar"))
    {
      return "QString";
    }
    if(stristr($string,"text"))
    {
      return "QString";
    }
    return "UUUUUU";
  }
  
  function TT($string)
  {
    if(stristr($string,"tinyint"))
    {
      return "Bool";
    }
    if(stristr($string,"int"))
    {
      if(stristr($string,"unsigned"))
        return "UInt";
      return "Int";
    }
    if(stristr($string,"varchar"))
    {
      return "String";
    }
    if(stristr($string,"text"))
    {
      return "String";
    }
    return "UUUUUU";
  }

?>
</pre>
