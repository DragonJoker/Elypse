<?php
function GetPageArgument( $p_arg, $p_default )
{
	if ( !empty( $_REQUEST[$p_arg] ) )
	{
		if( strlen( $_REQUEST[$p_arg] ) > 0 )
		{
			return htmlspecialchars( $_REQUEST[$p_arg] );
		}
		else
		{
			return $_REQUEST[$p_arg];
		}
	}
	else
	{
		return $p_default;
	}
}
?>