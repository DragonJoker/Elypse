<?php
require_once( 'argument.php' );

if( !empty( $_REQUEST['action'] ) )
{
	$action = GetPageArgument( 'action', '' );

	if( empty( $_SESSION['action'] ) || $_SESSION['action'] !== $action )
	{
		$_SESSION['action'] = $action;
	}
}
else if ( isset( $_SESSION['action'] ) )
{
	unset( $_SESSION['action'] );
}

if( !empty( $_REQUEST['gamename'] ) )
{
	$gamename = GetPageArgument( 'gamename', '' );

	if( empty( $_SESSION['gamename'] ) || $_SESSION['gamename'] !== $gamename )
	{
		$_SESSION['gamename'] = $gamename;
	}
}
else if ( isset( $_SESSION['gamename'] ) )
{
	unset( $_SESSION['gamename'] );
}

if ( $_SESSION['action'] == 'confirm' )
{
	echo( 'ok' );
}
else if ( $_SESSION['action'] == 'info' )
{
	echo( 'unlimited free' );
}
?>