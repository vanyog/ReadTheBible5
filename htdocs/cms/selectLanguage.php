<?php

// Функцията selectLanguage връща html кода на падащ списак за избор на език.
// Използва глобални променливи:
// $language - двебуквен индекст на избран език.
// (Индексът на избрания език се пази в бисквитка с продължителност 30 денонощия.)
// $languages - асоцииран масив с езици (виж conf/languages.php)

include_once("conf/languages.php");

function selectLanguage(){
global $language,$languages;
$r = '
<script type="text/javascript"><!--
function languageChanged(){
 s = document.getElementById("langSelect");
 i = s.selectedIndex;
 l = s.options[i].value;
 d = new Date();
 e = new Date( d.getTime() + 30*1000*60*60*24 );
 document.cookie = "language="+l+";expires="+e.toGMTString()+";path=/";
 if (!document.cookie)
    if (l=="bg") alert("Вашият браузър трябва да разрешава бисквитки, за да можете да смените езика.");
    else alert("Your browser must accept cookies to be able to change language.");
// alert("'.'http://'.$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF'].'?l="+l);
   window.location.href = "'.'http://'.$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF'].'?l="+l;
//   window.location.href = window.location.href;
// window.location.reload();
// history.go(0);
}
--></script>
<div id="language_selector" class="language_selector">
<select id="langSelect" onchange="languageChanged();">
';
foreach($languages as $k => $v){
  if ($k==$language) $s = ' selected'; else $s = ''; 
  $r .= '<option value="'.$k.'"'.$s." />$v\n";
}
return $r.'</select>
</div>';
}

// Връща избрания от посетителя език, като прави различни проверки

function getLanguage(){
global $languages,$default_language;
$l1 = substr($_SERVER['HTTP_ACCEPT_LANGUAGE'],0,2);
$l2 = $_COOKIE['language'];
$l3 = $_GET['l'];
$l = $default_language;
if ($l1) $l = $l1;
if ($l2) $l = $l2;
$ks = array_keys($languages);
if ($l3){ 
  $l = $l3;
//  if (in_array($l,$ks)) setcookie('language',$l,30*3600*24,'/');
}//  echo "| $l1 | $l2 | $l3 | $l |";// print_r(array_keys($languages));
if (in_array($l,$ks)) return $l;
else return $default_language;
}

?>
