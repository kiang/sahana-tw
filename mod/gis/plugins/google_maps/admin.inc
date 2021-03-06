<?php
/* $id$ */
/**Admin Module for Google Maps
*
* PHP version 4 and 5
*
* LICENSE: This source file is subject to LGPL license
* that is available through the world-wide-web at the following URI:
* http://www.gnu.org/copyleft/lesser.html
*
* @package    Sahana - http://sahana.sourceforge.net
* @author   	Mifan Careem <mifan@opensource.lk>
* @copyright  Lanka Software Foundation - http://www.opensource.lk
*
*/
include_once ($global['approot'] . '/inc/lib_form.inc');
function shn_plugin_default() {
?>
<h2><?php echo _("Google Maps Admin Page") ?></h2>	
<?php
    _shn_gis_gm_form();
}
function _shn_gis_gm_form() {
    global $global;
    $db = $global['db'];
    //shn_form_fopen(abc);
    // @todo: using small hack at the moment to bypass front controller. correct later
    
?>
<!-- 	<div id="formcontainer"> -->
	<!--<form method="POST" action="index.php?mod=gis&act=adm_default&submod=google_maps&seq=commit" id="formset">-->
	
<?php
    //Show stored value
    $query = "select value from config where confkey='mod_gis_google_key' and module_id='gis'";
    $res = $db->Execute($query);
    if (isset($res->EOF) && !$res->EOF) {
        $value = $res->fields['value'];
    }
    shn_form_fopen("adm_default&submod=google_maps&seq=commit", null, array('req' => false));
    shn_form_fsopen(_("Information"));
    echo _("<p>Google Maps requires a unique API Key that is linked to the domain name of the server that this instance of Sahana is installed on. This API Key needs to be entered in the field below</p>");
    echo _("<p>Obtain your key from: <a href='http://www.google.com/apis/maps/signup.html' target=_blank>Sign Up for the Google Maps API</a>.<br />More information: <a href='http://code.google.com/apis/maps' target=_blank>What is the Google Maps API?</a>.");
    shn_form_fsclose();
    shn_form_fsopen(_("Configure Google Maps"));
    shn_form_text(_("Enter Google Maps Key"), "gmap_key", 'size="40"', array('value'=> isset($value) ? $value : ''));
    shn_form_fsclose();
    shn_form_submit(_("Done"));
    shn_form_fclose();
}
function shn_plugin_commit() {
    global $global;
    $db = $global['db'];
    if (!null == ($_POST['gmap_key'])) {
        add_confirmation(_("Changes were saved"));
        $query = "select * from config where confkey='mod_gis_google_key' and module_id='gis'";
        $res = $db->Execute($query);
        if (!$res->EOF) {
            $gm = "UPDATE config SET value = '{$_POST['gmap_key']}' WHERE module_id = 'gis' AND confkey = 'mod_gis_google_key'";
            $db->Execute($gm);
        } else {
            $insert = "insert into config (module_id,confkey,value) values ('gis','mod_gis_google_key','{$_POST['gmap_key']}')";
            //echo $insert;
            $db->Execute($insert);
        }
    }
}
?>
