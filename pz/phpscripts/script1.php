<?php

function GetOutput($options)
{
	for($i=0;$i<20;$i++)
	{
		echo("bla");
	}
	echo("\n");
	echo "My first option: " . $options["myfirstoption"];
}

?>
