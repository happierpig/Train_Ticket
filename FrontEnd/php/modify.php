<?php
$curuser = $_POST['curuser'];
$target = $_POST['target'];
$username = $_POST['username'];
$email = $_POST['email'];
$password = $_POST['password'];
$privilege = $_POST['privilege'];

$command = "modify_profile -c $curuser -u $target -p $password -n $username -m $email -g $privilege";

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_connect($socket, '127.0.0.1', 10240);
socket_write($socket, $command);
$returnContent = socket_read($socket, 20480);
socket_close($socket);

echo $returnContent;