<?php
/*
 * Execute the commands:
 * php tpl_gettext.php
 * xgettext -o ../res/locale/sahana.pot -j tpl_languages.php
 */
$files = `find ../ -type f -iname "*.tpl.php"`;
$files = explode(chr(10), $files);
$allMatches = array();
foreach($files AS $file) {
    if(empty($file)) continue;
    $matches = array();
    $content = file_get_contents($file);
    preg_match_all('/_\([\'"][^\)]*[\'"]\)/', $content, $matches);
    if(!empty($matches)) {
        $allMatches = array_merge($allMatches, $matches[0]);
    }
}
if(!empty($allMatches)) {
    $languageContent = '<?php' . chr(10);
    foreach($allMatches AS $match) {
        $languageContent .= $match . ';' . chr(10);
    }
    file_put_contents('tpl_languages.php', $languageContent);
}