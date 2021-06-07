<?php
$username = $_POST['username'];
$trainId = $_POST['trainId'];
$date = substr($_POST['date'],5,5);
$from = $_POST['from'];
$to = $_POST['to'];
$number = $_POST['number'];
$queue = $_POST['queue'];
$command = "buy_ticket -u $username -i $trainId -d $date -f $from -t $to -n $number -q $queue";

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_connect($socket, '127.0.0.1', 10240);
socket_write($socket, $command);
$returnContent = socket_read($socket, 20480);
socket_close($socket);

echo $returnContent;