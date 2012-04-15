#include "OServerDataBaseHeader.h"

//该文件内容由 makeSqlHeader.php 脚本自动生成
//用于为OServerDataBase,提供数据库结构的相关信息

using namespace OSDB;

const QString User::_uid="uid";
const QString User::_uname="uname";
const QString User::_pwd="pwd";
const QString User::_lastLoginTime="lastlogintime";
const QString User::_lastLoginIp="lastloginip";
const QString User::_regTime="regtime";
const QString User::_onlineTime="onlinetime";
const QString User::_website="website";
const QString User::_info="info";
const QString User::_email="email";
const QString User::_avatar="avatar";

const QString Group::_gid="gid";
const QString Group::_groupname="groupname";
const QString Group::_caption="caption";
const QString Group::_master="master";
const QString Group::_regTime="regtime";
const QString Group::_website="website";
const QString Group::_info="info";
const QString Group::_avatar="avatar";

const QString UserList::_id="id";
const QString UserList::_uname="uname";
const QString UserList::_user="user";

const QString GroupMember::_id="id";
const QString GroupMember::_groupname="groupname";
const QString GroupMember::_uname="uname";
const QString GroupMember::_isAdmin="isadmin";
const QString GroupMember::_isDeny="isdeny";
const QString GroupMember::_regTime="regtime";

const QString UserRequest::_id="id";
const QString UserRequest::_time="time";
const QString UserRequest::_uname="uname";
const QString UserRequest::_user="user";
const QString UserRequest::_invitation="invitation";
const QString UserRequest::_msg="msg";
const QString UserRequest::_isHandle="ishandle";
const QString UserRequest::_handleTime="handletime";
const QString UserRequest::_result="result";

const QString MsgLog::_id="id";
const QString MsgLog::_time="time";
const QString MsgLog::_signTime="signtime";
const QString MsgLog::_uname="uname";
const QString MsgLog::_user="user";
const QString MsgLog::_msg="msg";
const QString MsgLog::_isSign="issign";
