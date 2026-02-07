<script language="php">
include("index_left_menu.php");

include("cms/f_html_file_contents.php");
include("cms/google_adsense_120x600.php");

$readme_file = "./readme-$language.html";
//echo "<p>$language</p>";

//$add_to_body_tag = ' onload="if (document.cookie && (document.cookie!=\'language=\'+llang)){ alert(\'reload\'); history.go(0); }"';

$html_content = $http_index_left_menu.'
<div id="main_content">
<p>
<a target="_blank" href="http://www.softsea.com/review/Read-the-Bible.html">
<img align="middle" src="http://www.softsea.com/images/awards-5stars-2010.gif" border="0" hspace="5px" alt="Link to SoftSea.com">
</a> 
<a target="_blank" href="http://www.geardownload.com/hobby/cd-bible.html">
<img align="middle" src="http://www.geardownload.com/images/5stars.jpg" border="0" hspace="5px" alt="Link to SoftSea.com">
</a> 
</p>'. html_file_contents($readme_file).'</div>

<div id="right_content">'.$google_120_600.'</div>

';

include("cms/html_build_page.php");
</script>
