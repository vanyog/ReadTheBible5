<script language="php">
include_once("f_tag_content.php");

function html_file_contents($fn){
return tag_content('body',file_get_contents($fn));
}

</script>
