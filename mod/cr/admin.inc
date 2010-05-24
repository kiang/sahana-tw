<?php
/* $Id: admin.inc,v 1.11.16.5 2008/07/23 06:58:26 ravithb Exp $ */
/**Admin Module for CR
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
* @copyright  Lanka Software Foundation - http://www.opensource.lk
 */
include_once ($global['approot'] . '/inc/lib_form.inc');
require_once ('acl.inc');

function _shn_cr_adm_sub_menu() {
    shn_adm_menuopen('');
    shn_adm_menuitem("adm_forms", _("Configure Form Display"));
    shn_adm_menuitem("adm_opt_camp_type", _("Shelter Type"));
    shn_adm_menuclose();
}

function shn_cr_adm_default() {
    global $conf;
    shn_cr_adm_forms();
}

function shn_cr_adm_forms() {
    _shn_cr_adm_sub_menu();
    _shn_cr_adm_form_change();
}

function _shn_cr_adm_form_change() {
    global $global;
    shn_form_fopen('adm_form_commit', null, array("req" => false));
    shn_form_fsopen(_("Add Service / Facility"));
    shn_form_opt_select("opt_camp_service", _("Currently Available Services/Facilities"));
    echo "<br/>";
    $extra_opts['req'] = true;
    shn_form_text(_("Service Name"), "service_name", 'size="30"', $extra_opts);
    shn_form_text(_("Service Abbrevation [3 letter unique abbr. to store in the database]"), "service_abbr", 'size="3"', $extra_opts);
    shn_form_fsclose();
    shn_form_submit(_("Add Service"));
    shn_form_submit(_("Delete Service"), "name=delete");
    shn_form_fclose();
}

function shn_cr_adm_form_commit() {
    global $global;
    $db = $global['db'];
    $db_flag = true;
    if (isset($_POST['delete'])) {
        $sql = "DELETE FROM field_options WHERE field_name = 'opt_camp_service' AND option_code = '" . $_POST['opt_camp_service'] . "'";
        $res = $global['db']->Execute($sql);
        if ($res === false) {
            add_error(_('Unable to delete the service'));
        } else {
            add_confirmation(_('Service deleted successfuly'));
            $sql1 = "DELETE FROM camp_services WHERE opt_camp_service = '" . $_POST['opt_camps_service'] . "'";
            $res1 = $global['db']->Execute($sql1);
            if ($res1 === false) {
                add_error(_t('Unable to delete for assigned camp services'));
            }
            add_confirmation(_('Service assigned in camps deleted successfuly'));
        }
    } elseif (!empty($_POST['service_name'])) {
        if (!empty($_POST['service_abbr'])) {
            if (strlen($_POST['service_abbr']) == 3) {
                $q = "SELECT option_code FROM field_options";
                $res = $db->Execute($q);
                while (!$res->EOF) {
                    $value = $res->fields["option_code"];
                    if ($value == $_POST['service_abbr']) {
                        add_error(_("Abbrevation exist, please insert unique abbrevation"));
                        $db_flag = false;
                        break;
                    }
                    $res->MoveNext();
                }
                if ($db_flag) {
                    $q = "insert into field_options(field_name,option_code,option_description) values('opt_camp_service','{$_POST['service_abbr']}','{$_POST['service_name']}')";
                    $res = $db->Execute($q);
                    add_confirmation(_("Added values"));
                }
            } else add_error(_("Abbrevation should in 3 characters"));
        } else {
            //error: enter code
            add_error(_("Please fill in empty values"));
        }
    } else {
        // do not store new services
        add_warning(_("Nothing to add"));
    }
    shn_cr_adm_default();
}

function shn_cr_adm_form_setdivision() {
    global $global;
    $db = $global['db'];
    $q = "select * from config where confkey='division_type' && module_id='cr'";
    $res = $db->Execute($q);
    if ($res->EOF) {
        //insert first time
        $q = "INSERT INTO config(module_id,confkey,value) VALUES ('cr', 'division_type','{$_POST['opt_location_type']}')";
        $res = $db->Execute($q);
    } else {
        //do update
        $q = "update config set value='{$_POST['opt_location_type']}' where confkey='division_type' && module_id='cr'";
        $ref = $db->Execute($q);
    }
    shn_cr_adm_default();
}

function shn_cr_adm_opt_camp_type() {
    _shn_cr_adm_sub_menu();
    _shn_cr_adm_field_options_form('opt_camp_type');
}

function _shn_cr_update_option($optioncode, $optiondescription) {
    global $global;
    $sql = "UPDATE field_options SET option_description='{$optiondescription}' WHERE option_code='{$optioncode}'";
    $global['db']->Execute($sql);
}

function _shn_cr_insert_option($field_name, $optioncode, $optiondescription) {
    global $global;
    $sql = "INSERT INTO field_options (field_name,option_code,option_description) VALUES ('{$field_name}','{$optioncode}','{$optiondescription}')";
    $global['db']->Execute($sql);
    if ($sql) {
        add_confirmation(_($_POST['new_desc'] . ' was successfully added.'));
    }
    $_POST['new_code'] = $_POST['new_desc'] = null;
}

function _shn_cr_delete_option($optioncode) {
    global $global;
    $sql = "DELETE FROM field_options WHERE option_code='{$optioncode}'";
    $global['db']->Execute($sql);
    add_confirmation(_('Field was successfully deleted.'));
}

function is_optioncode_exists($optioncode) {
    $found = false;
    global $global;
    $sql = "SELECT * FROM field_options WHERE option_code='{$optioncode}'";
    $res = $global['db']->Execute($sql);
    while (!$res->EOF) {
        $code = $res->fields['option_code'];
        $found = true;
        $res->MoveNext();
    }
    return $found;
}

function _shn_cr_adm_field_options_form($type) {
    global $global;
    global $conf;
    $button_type = isset($_POST['button_name']) ? $_POST['button_name'] : '';
    if ($button_type == _('Update')) {
        $sql = "SELECT field_name, option_code, option_description  FROM field_options WHERE field_name = '{$type}'";
        $sel = $global['db']->GetAll($sql);
        $count = 0;
        foreach ($sel as $row) {
            $opcode = $row['option_code'];
            $fname = $row['field_name'];
            $code = $_POST[$opcode];
            $desc = isset($_POST[$count]) ? $_POST[$count] : '';
            ++$count;
            $todelete = isset($_POST[$count]) ? $_POST[$count] : '';
            ++$count;
            if ($todelete == 'on') {
                _shn_cr_delete_option($code);
            } else {
                _shn_cr_update_option($code, $desc);
            }
        }
    } else if ($button_type == _('AddNew')) {
        if (empty($_POST['new_code'])) {
            add_error(_("Please enter the code"));
            display_errors();
        } else if(empty($_POST['new_desc'])) {
            add_error(_("Please  enter the description"));
            display_errors();
        } else if (is_optioncode_exists($_POST['new_code'])) {
            add_error(_("Code already exists"));
            display_errors();
        } else _shn_cr_insert_option($type, $_POST['new_code'], $_POST['new_desc']);
    }
    $sql = "SELECT field_name, option_code, option_description  FROM field_options WHERE field_name = '{$type}'";
    $arr = $global['db']->GetAll($sql);
    if (!empty($arr)) {
        shn_form_fopen('adm_' . $type, null);
        shn_form_fsopen(_('Update'));
        shn_form_hidden(array('seq' => 'update'));
        ?>
<table>
    <thead>
        <td><strong><?php echo _('Code'); ?></strong></td>
        <td width="100%"><strong><?php echo _('Description'); ?></strong></td>
        <td width="100%"><strong><?php echo _('Delete'); ?></strong></td>
    </thead>
    <tbody>
        <?php
        $count = 0;
        foreach ($arr as $row) {
            ?>
        <tr>
            <td><input type="text" READONLY name="<?=$row['option_code']; ?>"
                       value="<?=$row['option_code']; ?>"  />
            </td>
            <td><input type="text" name="<?=$count++;?>"
                       value="<?=$row['option_description']; ?>" />
            </td>
            <td><input type="checkbox" name="<?=$count++;?>" />
            </td>
        </tr>
            <?php
        }
        ?>
    </tbody>
</table>
        <?php
        shn_form_submit(_('Update'), "name='button_name'");
        shn_form_fsclose();
    }
    shn_form_fsopen(_('Add New '));
    shn_form_text(_("Code"), 'new_code', null, array('req' => true));
    shn_form_text(_("Description"), 'new_desc', null, array('req' => true));
    shn_form_submit(_('AddNew'), "name='button_name'");
    shn_form_fclose();
}