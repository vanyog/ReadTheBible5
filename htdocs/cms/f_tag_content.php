<script language="php">

function tag_content($t,$s){
$t1 = "<$t";
$i1 = strpos($s,$t1);
if ($i1!==false){
  $i1 = strpos($s,">",$i1+1);
  $t2 = "</$t>";
  $i2 = strpos($s,$t2,$i1+1);
  return substr($s,$i1+1,$i2-$i1-1);
}
else return "";
}

</script>
