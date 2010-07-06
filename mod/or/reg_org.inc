<?php
/**Registration related forms for Organizations of the Organization Registry
*
* PHP version 4 and 5
*
* LICENSE: This source file is subject to LGPL license
* that is available through the world-wide-web at the following URI:
* http://www.gnu.org/copyleft/lesser.html
*
* @author     Ravindra De Silva <ravindra@opensource.lk><ravidesilva@iee.org>
* @copyright  Lanka Software Foundation - http://www.opensource.lk
* @package    sahana
* @subpackage or
*/
global $global;
include_once $global['approot'] . "/inc/lib_form.inc";
include_once $global['approot'] . "/inc/lib_errors.inc";
require_once $global['approot'] . "/inc/lib_location.inc";
include_once $global['approot'] . "/inc/lib_security/lib_acl.inc";
include_once "lib_or.inc";
function _shn_or_regform_org($error = false) {
    global $global, $conf;
    include $global['approot'] . "/mod/gis/gis_fns.inc";
    $db = $global['db'];
?>
<h2><?php echo _('Organization Registration') ?></h2>
<?php
    /*    if($error)
    display_errors();*/
?>  
<div id="formcontainer">
<?php
    shn_form_fopen("reg_org_confirm", null, array('req' => false));
    shn_form_hidden(array('seq' => 'img'));
    shn_form_fsopen(_("Primary Details"));
    $extra_opts['req'] = true;
?><div class="info"><?php echo _("These are the basic details of an organization. Organization name is required , but the Registration number is optional"); ?> </div><br /><?php
    if (!empty($error)) {
        $extra_opts['value'] = $_SESSION['org']['name'];
    }
    shn_form_text(_("Organization Name : "), 'org_name', 'size="40"', $extra_opts);
    $extra_opts['req'] = false;
    if (!empty($error)) {
        $extra_opts['value'] = $_SESSION['org']['reg_no'];
    }
    $extra_opts['help'] = _("If your organization is registered under the government, please enter your Company Registration Number");
    shn_form_text(_("Registration Number(if any) : "), 'reg_no', 'size="40"', $extra_opts);
    shn_form_fsclose();
    shn_form_fsopen(_("Organization Type"));
    if (!empty($error)) {
        _shn_or_display_org_type($error, false, $_SESSION['org']['type']);
    } else {
        _shn_or_display_org_type($error, false);
    }
    shn_form_fsclose();
    shn_form_fsopen(_("Organization Sector"));
    if (!empty($error)) {
        _shn_or_display_sector($error, $_SESSION['org']['sector']);
    } else {
        _shn_or_display_sector($error);
    }
    echo '<div class="info">' . _('Other services : ') . '</div>';
    $res = $db->Execute("SELECT option_code, option_description
    FROM field_options WHERE field_name='opt_org_service'");
    while(!$res->EOF) {
        echo '<div style="float:left; margin: 20px;">';
        echo '<input type="checkbox" name="services[]" value="' . $res->fields['option_code'] . '"';
        if(isset($_SESSION['org']['services']) && in_array($res->fields['option_code'], $_SESSION['org']['services'])) {
            echo ' checked="checked"';
        }
        echo '>';
        echo $res->fields['option_description'];
        echo '</div>';
        $res->MoveNext();
    }
    shn_form_fsclose();
    // base location
    //  $parent=_shn_or_get_start_loc();
    // $parent="fw1hlc-7";
    // 	$range= shn_or_get_loc_range();
    // 	$range=array('start'=>2,'end'=>3);
    // 	$extra_opts["camps"]=true;
    // 	$extra_opts["org"]=true;
    $extra_opts['help'] = (_("Select the location based at.  This is hierarchical. For example if you are based  in the country 'Sri lanka' ,district 'Colombo' ,and village 'Dehiwala',  start by selecting the country. Once you select the country all the divisions of that country will be shown in the next select box.  In this case all districts in 'Sri lanka' will appear . From those select a district, which inturn will show the village list in the next select box. Select 'dehiwala' and it is done! Please note that the levels can be customized, hence it does not need to always be 'country'->'district'->'village'. It is configurable from the 'administration' section.")); //display help icon only once
    shn_form_fsopen(_("Base Location"));
    //print $_SESSION['org']['loc'];
    if (!empty($error)) {
        shn_location(null, $_SESSION['org']['loc'], null, $extra_opts);
    } else {
        shn_location(null, null, null, $extra_opts);
    }
    $extra_opts['value'] = false;
    $extra_opts['req'] = false;
    //unset($extra_opts);
    $extra_opts['help'] = (_("In addition to the hierarchical location , you may specify the address of the organization here. "));
    shn_form_text(_("Address"), 'loc_txt', 'size="40"', $extra_opts);
    shn_form_fsclose();
    shn_gis_add_marker_map_form(_("Area Map"), $nameOfPoint = "", $button_submit_opts = null, $extra_opts = array(
        'gps_x' => isset($_SESSION['org']['loc_x']) ? $_SESSION['org']['loc_x'] : '',
        'gps_y' => isset($_SESSION['org']['loc_y']) ? $_SESSION['org']['loc_y'] : '',
    ));
    // Contact infomation
    shn_form_fsopen(_("Contact Information"));
    _shn_or_display_contact_person($error, false, null);
    shn_form_fsclose();
    shn_form_fsopen(_("Facilities Available"));
    _shn_or_display_org_facilities($error);
    shn_form_fsclose();
?>
<br />
<center>
<?php
    //create the submit button
    shn_form_submit(_("Next"));
?>
</center>
<br />
<?php
    //close the form
    shn_form_hidden(array('org_type' => '1'));
    shn_form_fclose();
?>
<script type="text/javascript">
//<![CDATA[
    $(function() {
        var selectedStack = [];
        $('#optSectorType').change(function() {
            var values = $(this).val();
            if(values.length <= 3) {
                selectedStack = values;
            } else {
                alert('<?php echo _('You could only select 3 options in this field.'); ?>');
                $(this).val(selectedStack);
            }
        });
    });
//]]>
</script>
</div>
<?php
    // end of form
    
}

function _shn_or_regconfirm_org() {
    global $global, $conf;
    $db = $global['db'];
?>
<h2><?php echo _('Confirm Registration Details of ') . $_SESSION['org']['name'] ?></h2>
<div id="formcontainer">
<?php
    if ($_POST['action'] == 'confirm_edit') {
        shn_form_fopen("view_org_submit", null, array('req_message' => false, 'name' => "confirm_reg"));
    } else if ($_SESSION['org']['parent_id'] == null) {
        shn_form_fopen("reg_org_cr", null, array('req_message' => false, 'name' => "confirm_reg"));
    } else {
        shn_form_fopen("reg_operation_cr", null, array('req_message' => false, 'name' => "confirm_reg"));
    }
    shn_form_fsopen(_('Primary Details'));
    $q = "select o_uuid from org_main where name='{$_SESSION['org']['name']}'";
    $res = $db->Execute($q);
    if (($res == NULL) || ($res->EOF)) {
    } else {
        // do not show the warning when confirm edit.
        if ($_POST['action'] != 'confirm_edit') {
            $msg = _("An Organization by the name ") . $_SESSION['org']['name'] . _(" exists already. May be you should change the name?");
            add_warning($msg);
        }
    }
    shn_form_label(_("Organization Name : "), $_SESSION['org']['name']);
    if ($_SESSION['org']['parent_id'] == null) {
        shn_form_label(_("Registration Number(if any) : "), $_SESSION['org']['reg_no']);
        $q = "SELECT option_description FROM field_options WHERE field_name='opt_org_type' and option_code='{$_SESSION['org']['type']}'";
    } else {
        $q = "SELECT option_description FROM field_options WHERE field_name='opt_org_sub_type' and option_code='{$_SESSION['org']['type']}'";
    }
    $res = $db->Execute($q);
    shn_form_label(_("Organization Type"), $res->fields["option_description"]);
    $count = 1;
    foreach($_SESSION['org']['sector'] as $i) {
        $q = "SELECT option_description FROM field_options WHERE field_name='opt_sector_type' and option_code='{$i}'";
        $res = $db->Execute($q);
        $sector = $sector . " " . $count . "." . $res->fields["option_description"];
        $count++;
    }
    shn_form_label(_("Organization Sector"), $sector);
    $services = '';
    $q = "SELECT option_description FROM field_options
    WHERE field_name='opt_org_service'
    AND option_code IN ('" . implode('\', \'', $_SESSION['org']['services']) . "')";
    $res = $db->Execute($q);
    while(!$res->EOF) {
        $services .= $res->fields["option_description"] . ', ';
        $res->MoveNext();
    }
    shn_form_label(_("Other services : "), $services);
    shn_form_fsclose();
    /*
    // base location
    $parent=_shn_or_get_start_loc();
    $range= shn_or_get_loc_range();
    */
    shn_form_fsopen(_("Base Location"));
    $location = _shn_org_location_string($_SESSION['org']['loc']);
    $map_location = _("Latitude= ") . $_SESSION['org']['loc_y'] . _(" Longitude= ") . $_SESSION['org']['loc_x'];
    shn_form_label(_("Organization Location"), $location);
    shn_form_label(_("Organization Location Address"), $_SESSION['org']['loc_txt']);
    shn_form_label(_("Map Location"), $map_location);
    shn_form_fsclose();
    // Contact infomation
    shn_form_fsopen(_("Contact Information"));
    _shn_or_display_contact_person($error, true, null);
    shn_form_fsclose();
    shn_form_fsopen(_("Facilities Avaliable"));
    _shn_or_display_org_facilities($error, true);
    shn_form_fsclose();
?>
<br />
<center>
<?php
    //create the submit button
    $extra_opts['br'] = false;
    shn_form_button(_("Save"), "onClick='change_action(\"save\")'", $extra_opts);
    shn_form_button(_("Edit Details"), "onClick='change_action(\"edit\")'", $extra_opts);
    shn_form_hidden(array('action' => '0'));
    _shn_or_action_change_javascript("confirm_reg", "action");
?>
</center>
<br />
<?php
    //close the form
    shn_form_fclose();
?>				     
</div>
<?php
    // end of form
    
}
function _shn_or_regform_operation($error = false, $parent_org = null) {
    global $global, $conf;
    include $global['approot'] . "/mod/gis/gis_fns.inc";
    $db = $global['db'];
    $q = "SELECT name FROM org_main WHERE o_uuid='{$parent_org}'";
    $res = $db->Execute($q);
?>
<h2><?php echo _("Add an Operation/Branch under the Organization ") . $res->fields["name"] ?> </h2>
<div id="note">
<?php echo _("Your Organization might be having branches or carrying out relief operations in this disaster. Then information of those operations is usefull "); ?>
</div>
               
<div id="formcontainer">
<?php
    shn_form_fopen("reg_org_confirm", null, array('req' => false));
    shn_form_hidden(array('seq' => 'img'));
    shn_form_fsopen(_('Holding Company'));
    _shn_or_display_orgs(false, false, $parent_org);
    shn_form_fsclose();
    shn_form_fsopen(_('Primary Details'));
    $extra_opts['req'] = true;
    shn_form_text(_("Operation/Branch Name : "), 'org_name', 'size="50"', $extra_opts);
    shn_form_fsclose();
    shn_form_fsopen(_("Relationship to the Holding Company"));
    _shn_or_display_org_sub_type($error);
    shn_form_fsclose();
    shn_form_fsopen(_("Organization Sector"));
    _shn_or_display_sector($error);
    echo '<div class="info">' . _('Other services : ') . '</div>';
    $res = $db->Execute("SELECT option_code, option_description
    FROM field_options WHERE field_name='opt_org_service'");
    while(!$res->EOF) {
        echo '<div style="float:left; margin: 20px;">';
        echo '<input type="checkbox" name="services[]" value="' . $res->fields['option_code'] . '"';
        if(isset($_SESSION['org']['services']) && in_array($res->fields['option_code'], $_SESSION['org']['services'])) {
            echo ' checked="checked"';
        }
        echo '>';
        echo $res->fields['option_description'];
        echo '</div>';
        $res->MoveNext();
    }
    shn_form_fsclose();
    // base location
    shn_form_fsopen(_("Base Location"));
    //$parent=_shn_or_get_start_loc();
    //$range= shn_or_get_loc_range();
    if (!empty($error)) {
        shn_location(null, $_SESSION['org']['loc'], null);
    }
    else {
        shn_location(null, $_POST[$range["end"]], null);
    }
    $extra_opts['help'] = (_("In addition to the hierarchical location , you may specify the address of the organization here. "));
    if (!empty($error)) {
        $extra_opts['value'] = $_SESSION['org']['loc_txt'];
        $extra_opts['br'] = true;
    }
    shn_form_text(_("Address"), 'loc_txt', 'size="40"', $extra_opts);
    shn_form_fsclose();
    shn_gis_add_marker_map_form(_("Area Map"), $nameOfPoint = "", $button_submit_opts = null, $extra_opts = array(
        'gps_x' => isset($_SESSION['org']['loc_x']) ? $_SESSION['org']['loc_x'] : '',
        'gps_y' => isset($_SESSION['org']['loc_y']) ? $_SESSION['org']['loc_y'] : '',
    ));
    shn_form_fsopen(_("Contact Information"));
    _shn_or_display_contact_person($error, false, null);
    shn_form_fsclose();
    shn_form_fsopen(_("Facilities Avaliable"));
    _shn_or_display_org_facilities($error);
    shn_form_hidden(array('org_type' => '0'));
    shn_form_fsclose();
?>
<br />
<center>
<?php
    //create the submit button
    shn_form_submit(_("Next"));
?>
</center>
<br />
<?php
    //close the form
    shn_form_fclose();
?>				     
</div>
<?php
    // end of form
    
}
?>