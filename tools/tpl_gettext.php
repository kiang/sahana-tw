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
    $offset = 0;
    $content = file_get_contents($file);
    while($pos = strpos($content, '_("', $offset)) {
        $length = strpos($content, '")', $pos) + 2 - $pos;
        $matches[] = substr($content, $pos, $length);
        $offset = $pos + $length;
    }
    while($pos = strpos($content, '_(\'', $offset)) {
        $length = strpos($content, '\')', $pos) + 2 - $pos;
        $matches[] = substr($content, $pos, $length);
        $offset = $pos + $length;
    }
    if(!empty($matches)) {
        $allMatches = array_merge($allMatches, $matches);
    }
}
if(!empty($allMatches)) {
    $languageContent = '<?php' . chr(10);
    foreach($allMatches AS $match) {
        $languageContent .= $match . ';' . chr(10);
    }
    file_put_contents('tpl_languages.php', $languageContent);
}