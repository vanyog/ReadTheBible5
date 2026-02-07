<script language="php">
include("cms/google_adsense_120x90.php");
include("cms/selectLanguage.php");

$language = getLanguage();

$http_index_left_menu = '
<div class="left_menu">
'.selectLanguage();

switch ($language){

case 'bg': $http_index_left_menu .= '
<p>
<a href="index.php#info">Обща Информация</a><br />
<a href="index.php#tim">Екип</a><br />
<a href="index.php#help">Помогнете</a><br />
<a href="index.php#download"><strong>Изтегляне (Download)</strong></a><br />
<a href="index.php#code">За програмисти</a><br />
<a href="index.php#versions">Версии</a><br />
<a href="index.php#maillists">Пощенски списъци</a>
<a href="help/help.html">Указания за ползване</a>
</p>
<p>Страници в sourceforge.net<br /></p>
<p><a href="http://sourceforge.net/projects/readthebible">Summary</a><br />
<a href="http://sourceforge.net/projects/readthebible/files">Files</a><br />
<a href="http://sourceforge.net/project/stats/index.php?group_id=292810&ugn=readthebible">Usage Statistics</a><br />
<a href="http://sourceforge.net/projects/readthebible/develop">Develop</a><br />
<a href="http://readthebible.svn.sourceforge.net/viewvc/readthebible/">Browse SVN</a><br />
<a href="http://sourceforge.net/mail/?group_id=292810">Mailing Lists</a><br />
<a href="http://sourceforge.net/p/readthebible/wiki/bg-Home">Wiki</a><br />
</p>';
break;

case 'en': $http_index_left_menu .= '
<p>
<a href="index.php#toc">Contents</a><br />
<a href="index.php#new">News</a><br />
<a href="index.php#info">General Information</a><br />
<a href="index.php#help">Contribute</a><br />
<a href="index.php#download"><strong>Download</strong></a><br />
<a href="index.php#code">For programmers</a><br />
<a href="index.php#versions">Versions</a><br />
<a href="index.php#maillists">Mailing Lists</a><br>
<a href="help/help.html">Online help (in Bulgarian)</a>
</p>
<p>In sourceforge.net<br /></p>
<p><a href="http://sourceforge.net/projects/readthebible">Summary</a><br />
<a href="http://sourceforge.net/projects/readthebible/files">Files</a><br />
<a href="http://sourceforge.net/project/stats/index.php?group_id=292810&ugn=readthebible">Usage Statistics</a><br />
<a href="http://sourceforge.net/projects/readthebible/develop">Develop</a><br />
<a href="http://readthebible.svn.sourceforge.net/viewvc/readthebible/">Browse SVN</a><br />
<a href="http://sourceforge.net/apps/mediawiki/readthebible/index.php?title=Main_Page">Wiki</a><br />
</p>';
break;
}

$http_index_left_menu .= $google_120_90.'
</div>
';

</script>
