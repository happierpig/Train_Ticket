<?php
$username = $_POST['username'];
$password = $_POST['password'];
$command = "login -u $username -p $password";

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_connect($socket, '127.0.0.1', 10240);
socket_write($socket, $command);
$returnContent = socket_read($socket, 10240);
socket_close($socket);

echo $returnContent;
