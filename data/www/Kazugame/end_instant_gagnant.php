<?php
require_once( argument.php )

if( !empty( $_REQUEST['win'] ) )
{
	$win = intval( GetPageArgument( 'win', '' ) );

	if( empty( $_SESSION['win'] ) || $_SESSION['win'] !== $win )
	{
		$_SESSION['win'] = $win;
	}
}
else if ( isset( $_SESSION['win'] ) )
{
	unset( $_SESSION['win'] );
}

if( !empty( $_REQUEST['id_instant'] ) )
{
	$id_instant = intval( GetPageArgument( 'id_instant', '' ) );

	if( empty( $_SESSION['id_instant'] ) || $_SESSION['id_instant'] !== $id_instant )
	{
		$_SESSION['id_instant'] = $id_instant;
	}
}
else if ( isset( $_SESSION['id_instant'] ) )
{
	unset( $_SESSION['id_instant'] );
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
?>