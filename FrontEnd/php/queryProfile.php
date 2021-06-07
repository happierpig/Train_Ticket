<?php
$curuser = $_POST['curuser'];
$target = $_POST['targetuser'];
$command = "query_profile -c $curuser -u $target";

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_connect($socket, '127.0.0.1', 10240);
socket_write($socket, $command);
$returnContent = socket_read($socket, 20480);
socket_close($socket);

echo $returnContent;