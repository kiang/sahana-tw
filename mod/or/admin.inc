<?php
/**Admin controller of the Organization Registry
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
include_once $global['approot'] . '/inc/lib_form.inc';
require_once ('acl.inc');

function _shn_or_adm_sub_menu() {
    shn_adm_menuopen('');
    shn_adm_menuitem('adm_opt_org_type', _('Organization Types'));
    shn_adm_menuitem('adm_opt_sector_type', _('Organization Sectors'));
    shn_adm_menuitem('adm_opt_org_sub_type', _('Relationship to the Holding Company'));
    shn_adm_menuclose();
}

function shn_or_adm_default() {
    global $conf;
    shn_or_adm_opt_org_type();
}

function shn_or_adm_opt_org_type() {
    _shn_or_adm_sub_menu();
    _shn_or_adm_field_options_form('opt_org_type');
}

function shn_or_adm_opt_sector_type() {
    _shn_or_adm_sub_menu();
    _shn_or_adm_field_options_form('opt_sector_type');
}

function shn_or_adm_opt_org_sub_type() {
    _shn_or_adm_sub_menu();
    _shn_or_adm_field_options_form('opt_org_sub_type');
}

function _shn_or_update_option($optioncode, $optiondescription) {
    global $global;
    $sql = "UPDATE field_options SET option_description='{$optiondescription}' WHERE option_code='{$optioncode}'";
    $global['db']->Execute($sql);
}

function _shn_or_insert_option($field_name, $optioncode, $optiondescription) {
    global $global;
    $sql = "INSERT INTO field_options (field_name,option_code,option_description) VALUES ('{$field_name}','{$optioncode}','{$optiondescription}')";
    $global['db']->Execute($sql);
    if ($sql) {
        add_confirmation(_($_POST['new_desc'] . ' was successfully added.'));
    }
    $_POST['new_code'] = $_POST['new_desc'] = null;
}

function _shn_or_delete_option($optioncode) {
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

function _shn_or_adm_field_options_form($type) {
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
                _shn_or_delete_option($code);
            } else {
                _shn_or_update_option($code, $desc);
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
        } else _shn_or_insert_option($type, $_POST['new_code'], $_POST['new_desc']);
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