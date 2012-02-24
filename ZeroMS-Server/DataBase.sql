CREATE TABLE IF NOT EXISTS `error_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `time` int(10) unsigned NOT NULL,
  `uname` varchar(255) NOT NULL,
  `info` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
CREATE TABLE IF NOT EXISTS `group` (
  `gid` int(11) NOT NULL AUTO_INCREMENT,
  `groupname` varchar(255) NOT NULL,
  `caption` varchar(255) NOT NULL,
  `master` varchar(255) NOT NULL,
  `regtime` int(11) unsigned NOT NULL,
  `website` text NOT NULL,
  `info` text NOT NULL,
  `avater` varchar(32) NOT NULL,
  `istemp` tinyint(1) NOT NULL,
  PRIMARY KEY (`gid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
CREATE TABLE IF NOT EXISTS `group_member` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `groupname` varchar(255) NOT NULL,
  `uname` varchar(255) NOT NULL,
  `isadmin` tinyint(1) NOT NULL,
  `isdeny` tinyint(1) NOT NULL,
  `regtime` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
CREATE TABLE IF NOT EXISTS `image_cache` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `md5` varchar(32) NOT NULL,
  `user` varchar(255) NOT NULL,
  `lasttime` int(10) unsigned NOT NULL,
  `isexist` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
CREATE TABLE IF NOT EXISTS `image_sign` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `md5` varchar(32) NOT NULL,
  `user` varchar(255) NOT NULL,
  `time` int(10) unsigned NOT NULL,
  `issign` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
CREATE TABLE IF NOT EXISTS `log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `time` int(10) unsigned NOT NULL,
  `uname` varchar(255) NOT NULL,
  `info` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
CREATE TABLE IF NOT EXISTS `msg_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `time` int(10) unsigned NOT NULL,
  `uname` varchar(255) NOT NULL,
  `user` varchar(255) NOT NULL,
  `msg` text NOT NULL,
  `issend` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
CREATE TABLE IF NOT EXISTS `user` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `uname` varchar(255) NOT NULL,
  `pwd` text NOT NULL,
  `lastlogintime` int(10) unsigned NOT NULL,
  `lastloginip` text NOT NULL,
  `regtime` int(11) unsigned NOT NULL,
  `onlinetime` int(11) NOT NULL,
  `website` text NOT NULL,
  `info` text NOT NULL,
  `email` varchar(255) NOT NULL,
  `avatar` varchar(32) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
CREATE TABLE IF NOT EXISTS `userlist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `uanme` varchar(255) NOT NULL,
  `user` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
CREATE TABLE IF NOT EXISTS `userrequest` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `time` int(11) unsigned NOT NULL,
  `uname` varchar(255) NOT NULL,
  `user` varchar(255) NOT NULL,
  `group` varchar(255) NOT NULL,
  `msg` text NOT NULL,
  `ishandle` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

