<script language="php">

$html_title = 'Компютърна Библия 5';

$html_header = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
   <title>'.$html_title.'</title>
   <meta http-equiv="Content-Type" content="text/html; charset=windows-1251">
   <meta http-equiv="Pragma" content="no-cache">
   <meta http-equiv="Expires" content="-1">
   <link href="/style.css" rel="stylesheet" type="text/css" />
</head>

<body'.$add_to_body_tag.'>
';

$html_footer = '
</body>
</html>
';

//header('Cache-control: no-cache');
echo $html_header.$html_content.$html_footer;

</script>
