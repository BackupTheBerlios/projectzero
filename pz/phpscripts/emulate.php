<?php

// opties zetten die nodig zijn voor het goed functioneren van ons prog
// note: eens onderzoeken wat nodig is
error_reporting(E_ALL);

function userErrorHandler ($errno, $errmsg, $filename, $linenum, $vars)
{
	$errortype = array (
		1   =>  "Error",
		2   =>  "Warning",
		4   =>  "Parsing Error",
		8   =>  "Notice",
		16  =>  "Core Error",
		32  =>  "Core Warning",
		64  =>  "Compile Error",
		128 =>  "Compile Warning",
		256 =>  "User Error",
		512 =>  "User Warning",
		1024=>  "User Notice"
		);
	$user_errors = array(E_USER_ERROR, E_USER_WARNING, E_USER_NOTICE);
    
	echo "<error num=\"$errno\" type=\"$errortype\" errmsg=\"$errmsg\" "
	     . "file=\"$filename\" line=\"$linenum\">";

	// XXX what is this? :)
	/*if (in_array($errno, $user_errors))
        $err .= "\t<vartrace>".wddx_serialize_value($vars,"Variables")
		. "</vartrace>\n";*/

	// exit(); ?
}

$my_error_handler = set_error_handler("userErrorHandler");

// $argv can be turned of > $_SERVER[] should always work
$myargs = $_SERVER['argv'];
if (count($myargs) < 3)
{
	echo "<error errmsg=\"Not enough arguments given to emulate.php.\" "
	   . "type=\"EmulateError\">";
	exit();
}

include($myargs[2]);
$uneven = (count($myargs)-3) % 2;
if ($uneven)
{
	echo "<error errmsg=\"Arguments for scripts should come two by "
	   . "two.\" type=\"EmulateError\">";
	exit();
}
$options = array();
for($i=3;$i<count($myargs);$i+=2)
{
	$options[$myargs[$i]] = $myargs[$i+1];
}
switch($myargs[1])
{
	case "GetOutput":
		GetOutput($options);
		break;
	case "GetPreview":
		GetPreview($options);
		break;
	case "GetOptions":
		$newoptions = GetOptions();
		// print out the options ...
		break;
}

?>

