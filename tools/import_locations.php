<?php
$APPROOT = realpath(dirname(__FILE__)) . '/../';
$global['approot'] = $APPROOT;
$global['previous'] = false;
require_once ($APPROOT . 'inc/lib_config.inc');
require ($APPROOT . 'conf/sysconf.inc');
require_once ($APPROOT . 'inc/handler_db.inc');
//print_r($global['db']);
$db = $global['db'];

$fileToImport = $global['approot'] . 'tools/location_lists_taiwan.csv';
$fh = fopen($fileToImport, 'r');
$levels = array(
    2 => array(),
    3 => array(),
    4 => array(),
);
$number = 24;
$sql = 'INSERT INTO location (loc_uuid, parent_id, opt_location_type, name)
    VALUES (\'%s\',\'%s\',\'%s\',\'%s\')';
while($line = fgetcsv($fh, 4096)) {
    if(!is_array($line) || count($line) != 3) {
        continue;
    } else {
        if(!isset($levels[2][$line[0]])) {
            $db->Execute(sprintf($sql, 's5belc-' . $number, 's5belc-23', '2', $line[0]));
            $levels[2][$line[0]] = 's5belc-' . $number;
            ++$number;
        }
        if(!isset($levels[3][$line[1]])) {
            $db->Execute(sprintf($sql, 's5belc-' . $number, $levels[2][$line[0]], '3', $line[1]));
            $levels[3][$line[1]] = 's5belc-' . $number;
            ++$number;
        }
        if(!isset($levels[4][$line[2]])) {
            $db->Execute(sprintf($sql, 's5belc-' . $number, $levels[3][$line[1]], '4', $line[2]));
            $levels[4][$line[2]] = 's5belc-' . $number;
            ++$number;
        }
    }
}