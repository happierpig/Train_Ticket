<?php
$date = substr($_POST['date'], 5, 5);
$from = $_POST['from'];
$to = $_POST['to'];
$flag = $_POST['flag'];
$command = "query_ticket -s $from -t $to -d $date -p $flag";

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_connect($socket, '127.0.0.1', 10240);
socket_write($socket, $command);
$returnContent = socket_read($socket, 20480);
socket_close($socket);

echo $returnContent;