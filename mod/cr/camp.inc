<?php
/* $Id: camp.inc,v 1.52.4.22 2008/07/23 07:09:39 ravithb Exp $ */
/**Camp library for  CR
*
* PHP version 4 and 5
*
* LICENSE: This source file is subject to LGPL license
* that is available through the world-wide-web at the following URI:
* http://www.gnu.org/copyleft/lesser.html
*
* @package    Sahana - http://sahana.sourceforge.net
* @author   Chathra Hendehewa <chathra@opensource.lk>
* @author   Mifan Careem <mifan@opensource.lk>
* @edited by Isuru Samaraweera <isuru@opensource.lk>
* @copyright  Lanka Software Foundation - http://www.opensource.lk
*/
include_once ($global['approot'] . '/inc/lib_form.inc');
include_once ($global['approot'] . '/inc/lib_validate.inc');
include_once ($global['approot'] . '/inc/lib_errors.inc');
function _shn_cr_cadd_start($errors = false) {
    if ($errors) display_errors();
    global $global;
    global $conf;
    $db = $global['db'];
?>
<h2 align="center"><?php echo _("Register New ") . $conf['mod_cr_alt'] ?></h2>
<?php
    shn_form_fopen('acmp');
    shn_form_fsopen(_("General Details"));
    shn_form_hidden(array('seq' => 'chk'));
    shn_form_text(_("Shelter Name"), "camp_name", 'size="50"', array('req' => true));
    shn_form_opt_select("opt_camp_type", _("Shelter Type"));
?>

<!--<label>Division</label>
<select name="opt_location_type" >-->

<?php
    //-----------------------------------------
    shn_form_textarea(_("Address"), "camp_address");
    shn_form_fsclose();
    //-------------------------
    global $global;
    include $global['approot'] . "/inc/lib_location.inc";
    global $range;
    shn_form_fsopen(_("Location Details"));
    shn_location($range);
    shn_form_fsclose();
    shn_form_fsopen(_("Map Location"));
    echo '<div class=\'message information\'>';
    echo _('Click on the button "Update the location" will update the map location if you did manually modify the values of position.');
    echo "</div>";
    shn_form_text(_("Latitude : "), 'gps_x', 'size="40"');
    shn_form_text(_("Longitude : "), 'gps_y', 'size="40"');
    shn_form_button(_('Update the location'));
    shn_form_fsclose();
    if ($conf['gis']) {
        $extra_opts['marker'] = 'single';
        shn_form_fsopen($name_of_map);
        $plugin_name = $conf['mod_gis_dflt'];
        include $global['approot'] . "/mod/gis/plugins/" . "$plugin_name" . "/handler_" . "$plugin_name" . ".inc";
        echo "<div class='message information'>";
        echo _("You can move around and enlarge/reduce the size of the area you want using
the scale on upper left corner of the map. By clicking on a map you can
mark your location with a pointer.");
        echo "</div>";
        show_add_marker_map($nameOfPoint, $extra_opts);
        shn_form_fsclose();
    }
    //-------------------------
    shn_form_fsopen(_("Contact Person Details"));
    shn_form_text(_("Full Name"), "camp_contact_name", 'size="60"', array('help' => 'Enter Details of the person in charge of the Shelter'));
    shn_form_text(_("Phone Number"), "camp_contact_number", 'size="60"');
    shn_form_text(_("Mobile Number"), "contact_mob", 'size="60"');
    shn_form_text(_("Occupation"), "occupation", 'size="60"', array('help' => 'Official Occupation of Shelter Administrator'));
    shn_form_fsclose();
    shn_form_fsopen(_("Shelter Population"));
    shn_form_text(_("Capacity"), "capacity", 'size="10"');
    shn_form_text(_("Men"), "men", 'size="10"');
    shn_form_text(_("Women"), "women", 'size="10"');
    shn_form_text(_("Children( 1 ~ 12 years old )"), "children", 'size="10"');
    echo '<hr />';
    shn_form_text(_("Infected Count"), "family", 'size="10"');
    shn_form_date(_('Update time'), 'modified');
    shn_form_fsclose();
    /*
    $hospital_help=_("Check this if you want to enter hospital information related to this {$conf['mod_cr_alt']}");
    shn_form_fsopen(_("Hospital Metadata"));
    shn_form_checkbox(_("{$conf['mod_cr_alt']} is of type hospital"),"hospital",null,array('help'=>$hospital_help));
    shn_form_fsclose();
    */
    shn_form_submit(_("Next"));
    shn_form_fclose();
}
function _shn_cr_hos_bed($errors = false) {
    if ($errors) display_errors;
    $_SESSION['opt_camp_service'] = $_POST['opt_camp_service'];
    $_SESSION['camp_comments'] = $_POST['comments'];
    global $global;
    global $conf;
    shn_form_fopen(acmp);
?>
	<h1><?php echo _("Hospital Capabilities") ?></h1>
<?php
    shn_form_fsopen(_("Number of Beds"));
    shn_form_hidden(array('seq' => 'commit'));
    shn_form_text(_("ICU"), "icu", 'size="5"');
    shn_form_text(_("Medicine"), "med", 'size="5"');
    shn_form_text(_("Surgery"), "sur", 'size="5"');
    shn_form_text(_("Paediatrics"), "paed", 'size="5"');
    shn_form_text(_("Obstetrics"), "obs", 'size="5"');
    shn_form_fsclose();
    shn_form_submit(_("Next"));
}
function _shn_cr_cadd_chk($errors = false) {
    if ($errors) display_errors;
    global $global, $conf;
    $_SESSION['camp_name'] = $_POST['camp_name'];
    $_SESSION['opt_camp_type'] = $_POST['opt_camp_type'];
    $_SESSION['opt_location_type'] = $_POST['opt_location_type'];
    $_SESSION['camp_address'] = $_POST['camp_address'];
    $_SESSION['camp_contact_name'] = $_POST['camp_contact_name'];
    $_SESSION['camp_contact_number'] = $_POST['camp_contact_number'];
    if ($_POST['family'] == null) {
        $_SESSION['camp_family'] = 0;
    } else {
        $_SESSION['camp_family'] = $_POST['family'];
    }
    if ($_POST['men'] == null) {
        $_SESSION['camp_men'] = 0;
    } else {
        $_SESSION['camp_men'] = $_POST['men'];
    }
    if ($_POST['women'] == null) {
        $_SESSION['camp_women'] = 0;
    } else {
        $_SESSION['camp_women'] = $_POST['women'];
    }
    if ($_POST['children'] == null) {
        $_SESSION['camp_children'] = 0;
    } else {
        $_SESSION['camp_children'] = $_POST['children'];
    }
    $_SESSION['camp_total'] = $_SESSION['camp_men']
            + $_SESSION['camp_women'] + $_SESSION['camp_children'];
    if ($_POST['capacity'] == null) {
        $_SESSION['camp_capacity'] = 0;
    } else {
        $_SESSION['camp_capacity'] = $_POST['capacity'];
    }
    if (empty($_POST['modified'])) {
        $_SESSION['camp_modified'] = '';
    } else {
        $_SESSION['camp_modified'] = $_POST['modified'];
    }
    $_SESSION['contact_mob'] = $_POST['contact_mob'];
    $_SESSION['occupation'] = $_POST['occupation'];
    $_SESSION['cr_1'] = $_POST['loc_select_1'];
    $_SESSION['cr_2'] = $_POST['loc_select_2'];
    $_SESSION['cr_3'] = $_POST['loc_select_3'];
    $_SESSION['cr_4'] = $_POST['loc_select_4'];
    $_SESSION['cr_5'] = $_POST['loc_select_5'];
    if (isset($_POST['hospital'])) {
        $_SESSION['hospital'] = true;
    }
    $_SESSION['gis_loc_x'] = $_POST['gps_x'];
    $_SESSION['gis_loc_y'] = $_POST['gps_y'];
?>
<h2><?php echo _("Shelter Checklist") ?></h2>
<?php
    //var_dump($_SESSION);
    shn_form_fopen("acmp", '', array('req_message' => false));
    shn_form_fsopen(_("Services / Facilities Available"));
    if (isset($_SESSION['hospital'])) {
        shn_form_hidden(array('seq' => 'hos'));
    } else {
        shn_form_hidden(array('seq' => 'confir'));
    }
    shn_form_opt_checkbox("opt_camp_service");
    //shn_form_textarea(_("Other Facilities"),"comments");
    shn_form_fsclose();
    shn_form_submit(_("Next"));
    shn_form_fclose();
}
function _shn_cr_cadd_commit() {
    global $global;
    if (isset($_POST['finish'])) {
        //write to database
        /*if(!isset($_SESSION['hospital'])){
        $_SESSION['opt_camp_service']=$_POST['opt_camp_service'];
        $_SESSION['camp_comments']=$_POST['comments'];
        }*/
        if (isset($_SESSION['hospital'])) {
            $_SESSION['icu'] = $_POST['icu'];
            $_SESSION['med'] = $_POST['med'];
            $_SESSION['sur'] = $_POST['sur'];
            $_SESSION['paed'] = $_POST['paed'];
            $_SESSION['obs'] = $_POST['obs'];
        }
        //foreach($_SESSION['opt_camp_service'] as $a=>$b){
        //echo "value is $b";
        //}
        //echo microtime(true);
        //var_dump($_SESSION);
        _shn_cr_cadd_commit_db();
    } elseif (isset($_POST['edit'])) {
        _shn_cr_cedt_add();
    }
}
function _shn_cr_create_cid($val) {
    global $global;
    include_once ($global['approot'] . '/inc/lib_uuid.inc');
    $id = shn_create_uuid($val);
    //echo "ID IS: $id";
    return $id;
}
/**
 * @todo: change variables to lat=y. lon=x
 */
function _shn_cr_validate_gps() {
    clean_errors();
    $error_flag = false;
    // latitude
    if (!(null == ($_POST['gps_y']))) {
        if (!shn_valid_y_coordinate(trim($_POST['gps_y']))) {
            //add_error(_("Coordinates invalid: should be between -180 & 180"));
            $error_flag = true;
        }
    }
    //longitude
    if (!(null == ($_POST['gps_x']))) {
        if (!shn_valid_x_coordinate(trim($_POST['gps_x']))) {
            //add_error(_("Coordinates invalid: should be between -180 & 180"));
            $error_flag = true;
        }
    }
    return $error_flag;
}
function _shn_cr_validate_error() {
    clean_errors();
    $error_flag = false;
    if (null == ($_POST['camp_name'])) {
        add_error(_("Please enter the shelter name"));
        $error_flag = true;
    }
    //echo $_POST['family'];
    if (!(null == ($_POST['family']))) {
        if (!is_numeric(trim($_POST['family']))) {
            add_error(_("The infected count is not a valid number"));
            $error_flag = true;
        } else if (!positive_number($_POST['family'])) {
            add_error(_("The infected count cannot be negative"));
            $error_flag = true;
        }
    }
    /*
    if(!(null == ($_POST['camp_contact_number']))){
    if(!shn_valid_phone(trim($_POST['camp_contact_number']))){
    add_error(_("The Phone Number is not valid"));
    $error_flag=true;
    }
    }
    
    if(!(null == ($_POST['contact_mob']))){
    if(!shn_valid_phone(trim($_POST['contact_mob']))){
    add_error(_("The Mobile Phone Number is not valid"));
    $error_flag=true;
    }
    }
    */
    if (!(null == ($_POST['capacity']))) {
        if (!is_numeric(trim($_POST['capacity']))) {
            add_error(_("The capacity is not a valid number"));
            $error_flag = true;
        } else if (!positive_number($_POST['capacity'])) {
            add_error(_("The capacity cannot be negative"));
            $error_flag = true;
        }
    }
    if (!(null == ($_POST['total']))) {
        if (!is_numeric(trim($_POST['total']))) {
            add_error(_("The total count is not a valid number"));
            $error_flag = true;
        } else if (!positive_number($_POST['total'])) {
            add_error(_("The total count cannot be negative"));
            $error_flag = true;
        }
    }
    if (!(null == ($_POST['men']))) {
        if (!is_numeric(trim($_POST['men']))) {
            add_error(_("The men count is not a valid number"));
            $error_flag = true;
        } else if (!positive_number($_POST['men'])) {
            add_error(_("The men count cannot be negative"));
            $error_flag = true;
        }
    }
    if (!(null == ($_POST['women']))) {
        if (!is_numeric(trim($_POST['women']))) {
            add_error(_("The women count is not a valid number"));
            $error_flag = true;
        } else if (!positive_number($_POST['women'])) {
            add_error(_("The women count cannot be negative"));
            $error_flag = true;
        }
    }
    if (!(null == ($_POST['children']))) {
        if (!is_numeric(trim($_POST['children']))) {
            add_error(_("The children count is not a valid number"));
            $error_flag = true;
        } else if (!positive_number($_POST['children'])) {
            add_error(_("The children count cannot be negative"));
            $error_flag = true;
        }
    }
    if ($_POST['children'] != null || $_POST['women'] != null || $_POST['men'] != null) {
        $totalcount = $_POST['children'] + $_POST['women'] + $_POST['men'];
        if ($_POST['family'] > $totalcount) {
            add_error(_('The Infected count should be less or equal than the total count'));
            $error_flag = true;
        }
        if($_POST['capacity'] < $totalcount) {
            add_error(_('The capacity should be greater or equal than the total count'));
            $error_flag = true;
        }
    }
    return $error_flag;
}
function positive_number($nr) {
    if (ereg("^[0-9]+$", $nr) && $nr >= 0) {
        return true;
    } else {
        return false;
    }
}
function _shn_cr_bed_validate_error() {
    //if(isset($_SESSION['hospital'])){
    clean_errors();
    $error_flag = false;
    if (!(null == ($_POST['icu']))) {
        if (!is_numeric(trim($_POST['icu']))) {
            add_error(_("The ICU beds is not a valid number"));
            $error_flag = true;
        }
    }
    if (!(null == ($_POST['med']))) {
        if (!is_numeric(trim($_POST['med']))) {
            add_error(_("The medicine beds is not a valid number"));
            $error_flag = true;
        }
    }
    if (!(null == ($_POST['sur']))) {
        if (!is_numeric(trim($_POST['sur']))) {
            add_error(_("The surgery beds is not a valid number"));
            $error_flag = true;
        }
    }
    if (!(null == ($_POST['paed']))) {
        if (!is_numeric(trim($_POST['paed']))) {
            add_error(_("The paediatrics Beds is not a valid number"));
            $error_flag = true;
        }
    }
    if (!(null == ($_POST['obs']))) {
        if (!is_numeric(trim($_POST['obs']))) {
            add_error(_("The obstetrics Beds is not a valid number"));
            $error_flag = true;
        }
    }
    return $error_flag;
}
function _shn_cr_get_locid() {
    if ($_SESSION['cr_5'] != null) return $_SESSION['cr_5'];
    else if ($_SESSION['cr_4'] != null) return $_SESSION['cr_4'];
    else if ($_SESSION['cr_3'] != null) return $_SESSION['cr_3'];
    else if ($_SESSION['cr_2'] != null) return $_SESSION['cr_2'];
    else return $_SESSION['cr_1'];
}
/**
 * Database inserts for Sheter Registry
 * @access private
 */
function _shn_cr_cadd_commit_db() {
    global $global, $conf;
    include_once ($global['approot'] . '/inc/lib_uuid.inc');
    $db = $global['db'];
    $uid = shn_create_uuid('c');
    $puid = shn_create_uuid('p');
    shn_db_insert_phonetic('camp_name', $uid);
    $loc_id = _shn_cr_get_locid();
    //enter into camp table
    $q = "INSERT INTO camp_general(c_uuid,name,location_id,opt_camp_type,address,capacity) VALUES('{$uid}','{$_SESSION['camp_name']}','{$loc_id}','{$_SESSION['opt_camp_type']}','{$_SESSION['camp_address']}','{$_SESSION['camp_capacity']}')";
    $res = $db->Execute($q);
    //enter into camp_reg table
    $q = "INSERT INTO camp_reg (c_uuid,admin_name,admin_no,men,women,family,
    children,total,modified) VALUES ('{$uid}',
    '{$_SESSION['camp_contact_name']}','{$_SESSION['camp_contact_number']}',
    '{$_SESSION['camp_men']}','{$_SESSION['camp_women']}',
    '{$_SESSION['camp_family']}','{$_SESSION['camp_children']}',
    '{$_SESSION['camp_total']}', '{$_SESSION['camp_modified']}')";
    $res = $db->Execute($q);
    // insert into camp admin
    $qa = "INSERT INTO camp_admin(c_uuid,contact_puuid) VALUES ('{$uid}','{$puid}')";
    $resa = $db->Execute($qa);
    // insert into person_details
    $qb = "INSERT INTO person_details(p_uuid,occupation) VALUES('{$puid}','{$_SESSION['occupation']}')";
    $resb = $db->Execute($qb);
    // insert into admin contact
    $qc = "INSERT INTO contact(pgoc_uuid,opt_contact_type,contact_value) VALUES ('{$puid}','cmob','{$_SESSION['contact_mob']}')";
    $resc = $db->Execute($qc);
    if ($conf['gis']) {
        include $global['approot'] . "/mod/gis/gis_fns.inc";
        shn_gis_dbinsert($uid, $loc_id, $conf['mod_gis'], $_SESSION['gis_loc_x'], $_SESSION['gis_loc_y'], NULL);
    }
    /*insert services into camp_services table
    *currently add checked values.
    */
    if (isset($_SESSION['opt_camp_service'])) {
        foreach($_SESSION['opt_camp_service'] as $a => $b) {
            //currently add checked values.
            $q = "insert into camp_services(c_uuid,opt_camp_service,value) values('{$uid}','{$b}',1)";
            $res = $db->Execute($q);
        }
    }
    add_confirmation(_("You have succesfully registered shelter ") . $_SESSION["camp_name"]);
?>
	<br />
	<div id="note" style="text-align: left;">
            <div><?php echo _('Add Shelter'); ?>:<?php
            echo sprintf(_('Please click %s to add another shelter.'),
                    '<a href="index.php?mod=cr&act=acmp"><b>' . _('Add Shelter') . '</b></a>');
            ?></div>
            <div><?php echo _('Edit Shelter'); ?>:<?php
            echo sprintf(_('To edit current shelter, please do a search with %s or %s first to find the shelter you would like to edit.'),
                    '<a href="index.php?mod=cr&act=srch"><b>' . _('Search by Name') . '</b></a>',
                    '<a href="index.php?mod=cr&act=srch_address"><b>' . _('Search by address') . '</b></a>');
            ?></div>
	</div>
<br/>
<?php
    unset($_SESSION['hospital']);
}
//code to add confirmation page
function _shn_cr_cadd_confir($errors = false) {
    if ($errors) display_errors;
    global $global;
    global $conf;
    $db = $global['db'];
?>
    <h2><?php echo _('Confirm Registration Details of ') . $_SESSION['camp_name'] ?></h2>
    <?php
    shn_form_fopen("acmp", null, array('req_message' => false, 'name' => ""));
    shn_form_fsopen(_('General Details'));
    shn_form_label(_("Shelter Name") . " : ", $_SESSION['camp_name']);
    $q = "select option_description from field_options where field_name='opt_camp_type' and option_code='" . $_SESSION['opt_camp_type'] . "'";
    $ref = $db->Execute($q);
    shn_form_label(_("Shelter Type") . " : ", $ref->fields[0]);
    shn_form_label(_("Address") . " : ", $_SESSION['camp_address']);
    shn_form_fsclose();
    shn_form_fsopen(_('Location Details'));
    $location = _shn_cr_location_string($_SESSION['cr']['loc']);
    shn_form_label(_("Location : "), $location);
    shn_form_fsclose();
    shn_form_fsopen(_('Contact Person Details'));
    shn_form_label(_("Full Name") . " : ", $_SESSION['camp_contact_name']);
    shn_form_label(_("Phone Number") . " : ", $_SESSION['camp_contact_number']);
    shn_form_label(_("Mobile Number") . " : ", $_SESSION['contact_mob']);
    shn_form_label(_("Occupation") . " : ", $_SESSION['occupation']);
    shn_form_fsclose();
    shn_form_fsopen(_('Shelter Population'));
    shn_form_label(_("Capacity") . " : ", $_SESSION['camp_capacity']);
    shn_form_label(_("Men") . " : ", $_SESSION['camp_men']);
    shn_form_label(_("Women") . " : ", $_SESSION['camp_women']);
    shn_form_label(_("Children( 1 ~ 12 years old )") . " : ", $_SESSION['camp_children']);
    shn_form_label(_("Total Count") . " : ", $_SESSION['camp_total']);
    echo '<hr />';
    shn_form_label(_("Infected Count") . " : ", $_SESSION['camp_family']);
    shn_form_label(_('Update time') . " : ", $_SESSION['camp_modified']);
    shn_form_hidden(array('seq' => 'commit'));
    shn_form_fsclose();
    shn_form_fsopen(_('GPS Coordinates'));
    shn_form_label(_("Latitude") . " : ", $_SESSION['gis_loc_y']);
    shn_form_label(_("Longitude") . " : ", $_SESSION['gis_loc_x']);
    shn_form_fsclose();
    shn_form_fsopen(_('Services / Facilities Available'));
    if (!isset($_SESSION['hospital'])) {
        $_SESSION['opt_camp_service'] = $_POST['opt_camp_service'];
        $_SESSION['camp_comments'] = $_POST['comments'];
    }
    $a = "select option_code,option_description from field_options where field_name='opt_camp_service'";
    $ref = $db->Execute($a);
    while (!$ref->EOF) {
        $val = $ref->fields[0];
        $name = $ref->fields[1];
        $flag = false;
        if (isset($_SESSION['opt_camp_service'])) {
            foreach($_SESSION['opt_camp_service'] as $a => $b) {
                if ($val == $b) {
                    shn_form_label(_lc($name) . " : ", _('Available'));
                    $flag = true;
                }
            }
        }
        if (!$flag) {
            shn_form_label(_lc($name) . " : ", _('Not Available'));
        }
?>
		<!--  </tr> -->
		<?php
        $ref->MoveNext();
    }
    shn_form_fsclose();
    echo '<center>';
    shn_form_submit(_("Finish"), 'name="finish"');
    shn_form_submit(_("Edit"), 'name="edit"');
    echo '</center>';
    shn_form_fclose();
}
function _shn_cr_cedt_add($errors = false) {
    if ($errors) display_errors();
    global $global;
    $db = $global['db'];
?>
<h2><center><?php echo _("Register New Shelter") ?></center></h2>
	<?php
    shn_form_fopen('acmp');
    shn_form_fsopen(_("General Details"));
    shn_form_hidden(array('seq' => 'gis'));
    shn_form_text(_("Shelter Name"), "camp_name", 'size="50"', array('req' => true, 'value' => $_SESSION['camp_name']));
    shn_form_opt_select("opt_camp_type", _("Shelter Type"), null, array('value' => $_SESSION['opt_camp_type']));
    shn_form_textarea(_("Address"), "camp_address", null, array('value' => $_SESSION['camp_address']));
    shn_form_fsclose();
?>

	<?php
    global $global;
    include $global['approot'] . "/inc/lib_location.inc";
    global $range;
    shn_form_fsopen(_("Location Details"));
    shn_location($range, $_SESSION['cr']['loc']);
    shn_form_fsclose();
    //-----------------------------------------
    shn_form_fsopen(_("Contact Person Details"));
    shn_form_text(_("Full Name"), "camp_contact_name", 'size="60"', array('value' => $_SESSION['camp_contact_name']));
    shn_form_text(_("Phone Number"), "camp_contact_number", 'size="60"', array('value' => $_SESSION['camp_contact_number']));
    shn_form_text(_("Mobile Number"), "contact_mob", 'size="60"', array('value' => $_SESSION['camp_contact_mob']));
    shn_form_text(_("Occupation"), "occupation", 'size="60"', array('value' => $_SESSION['camp_occupation']));
    shn_form_fsclose();
    shn_form_fsopen(_("Shelter Population"));
    shn_form_text(_("Capacity"), "capacity", 'size="10"', array('value' => $_SESSION['camp_capacity']));
    shn_form_text(_("Men"), "men", 'size="10"', array('value' => $_SESSION['camp_men']));
    shn_form_text(_("Women"), "women", 'size="10"', array('value' => $_SESSION['camp_women']));
    shn_form_text(_("Children( 1 ~ 12 years old )"), "children", 'size="10"', array('value' => $_SESSION['camp_children']));
    echo '<hr />';
    shn_form_text(_("Infected Count"), "family", 'size="10"', array('value' => $_SESSION['camp_family']));
    shn_form_date(_('Update time'), 'modified', array(
        'value' => $_SESSION['camp_modified']
    ));
    shn_form_fsclose();
    shn_form_submit(_("Next"));
    shn_form_fclose();
}
function _shn_cr_location_string($loc) {
    global $global;
    $db = $global["db"];
    $loc_arr = array();
    include $global['approot'] . "/inc/lib_location.inc";
    shn_get_parents($loc, $loc_arr);
    if ($loc_arr[0] == 'unknown') {
        //shn_form_label(_("Organization Location"),"Unknown");
        
    } else {
        $max = count($loc_arr) - 1;
        array_pop($loc_arr);
        for ($count = 0;$count < $max;$count++) {
            $x = array_pop($loc_arr);
            $q = "select name from location where loc_uuid='{$x}'";
            $res = $db->Execute($q);
            if ($count == 0) {
                $location = $location . $res->fields["name"];
            } else {
                $location = $location . " -->" . $res->fields["name"];
            }
        }
    }
    return $location;
}
?>
