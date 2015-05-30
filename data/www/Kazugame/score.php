<?php
require_once( 'argument.php' );

if( !empty( $_REQUEST['score'] ) )
{
	$score = intval( GetPageArgument( 'score', 0 ) );

	if( empty( $_SESSION['score'] ) || $_SESSION['score'] !== $score )
	{
		$_SESSION['score'] = $score;
	}
}
else if ( isset( $_SESSION['score'] ) )
{
	unset( $_SESSION['score'] );
}

if( !empty( $_REQUEST['name'] ) )
{
	$name = GetPageArgument( 'name', '' );

	if( empty( $_SESSION['name'] ) || $_SESSION['name'] !== $name )
	{
		$_SESSION['name'] = $name;
	}
}
else if ( isset( $_SESSION['name'] ) )
{
	unset( $_SESSION['name'] );
}

if( !empty( $_REQUEST['submit'] ) )
{
	$submit = true;

	if( empty( $_SESSION['submit'] ) || $_SESSION['submit'] !== $submit )
	{
		$_SESSION['submit'] = $submit;
	}
}
else if ( isset( $_SESSION['submit'] ) )
{
	unset( $_SESSION['submit'] );
}

if ( !isset( $_SESSION['submit'] ) )
{
	echo( 'coin|glop<br>1000|2000' );
}
?>