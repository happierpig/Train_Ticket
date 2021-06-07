
<?php
$curUsername = $_POST['curUsername'];
$username = $_POST['username'];
$email = $_POST['email'];
$password = $_POST['password'];

$command = "add_user -c $curUsername -u $username -p $password -n $username -m $email -g 1";

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_connect($socket, '127.0.0.1', 10240);
socket_write($socket,  $command);
$returnContent = socket_read($socket, 20480);
socket_close($socket);

echo $returnContent;