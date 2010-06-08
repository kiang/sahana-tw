<?php
include_once $global['approot'] . '/inc/lib_form.inc';
function _shn_pref_config_form($errors = false, $updated = false) {
    global $conf;
    global $global;
    $sql_level1 = "SELECT incident_id, name FROM incident WHERE parent_id IS NULL";
    $sql_level2 = "SELECT  a.incident_id , a.name, a.parent_id, b.name as parent_name FROM incident a INNER JOIN incident b  ON a.parent_id = b.incident_id AND b.parent_id IS NULL";
    $sql_level3 = "SELECT a.incident_id, a.name, b.incident_id as parent_id, b.name as parent_name,c.name as grand_parent_name FROM incident a INNER JOIN incident b  ON a.parent_id = b.incident_id INNER JOIN incident c ON b.parent_id = c.incident_id AND c.parent_id IS NULL";
    $arr_level1 = $global['db']->GetAll($sql_level1);
    $arr_level2 = $global['db']->GetAll($sql_level2);
    $arr_level3 = $global['db']->GetAll($sql_level3);
    if (isset($_POST['ims_level']) && $_POST['ims_level'] == 'level2') {
        foreach($arr_level2 as $row) {
            $arr[$row['incident_id']] = $row['parent_name'] . ' --> ' . $row['name'];
        }
    } elseif (isset($_POST['ims_level']) && $_POST['ims_level'] == 'level3') {
        foreach($arr_level3 as $row) {
            $arr[$row['incident_id']] = $row['grand_parent_name'] . ' --> ' . $row['parent_name'] . ' --> ' . $row['name'];
        }
    } else {
        foreach($arr_level1 as $row) {
            $arr[$row['incident_id']] = $row['name'];
        }
    }
    if ($errors) display_errors();
    echo "<h2>" . _("Change default incident") . "</h2>";
    shn_form_fopen("ims_config", null, array('req_message' => false));
    if ($updated) {
        shn_form_fsopen("");
?><p align="center">Updated. </p>
<?php
        shn_form_fsclose();
    }
    shn_form_hidden(array('seq' => 'entry'));
    shn_form_fsopen(_("Default Incident"));
    shn_form_select(array('level1' => $conf['mod_admin_ims_level1'], 'level2' => $conf['mod_admin_ims_level2'], 'level3' => $conf['mod_admin_ims_level3']), _('Incident Type'), 'ims_level', 'onChange="submit(this)"');
    shn_form_select($arr, _('Select Incident'), 'ims_incident_id');
    shn_form_fsclose();
    shn_form_submit(_('Update'));
    shn_form_fclose();
}
function _shn_pref_config_validate() {
    $error_flag = false;
    //clean the post
    shn_tools_clean_post(&$local_post);
    //anything entered?
    if (empty($local_post)) {
        add_error(_("You have not completed the form"));
        return false;
    }
    if (!isset($local_post['ims_level'])) {
        add_error(_("Please Select the incident type"));
        $error_flag = true;
    }
    if (!isset($local_post['ims_incident_id'])) {
        add_error(_("Please select an item from 'Select Incident' box"));
        $error_flag = true;
    }
    if ($error_flag) return false;
    else {
        shn_user_pref_update('ims', 'incident_id', $local_post['ims_incident_id']);
        return true;
    }
}

function _shn_pref_openid_form($errors = false, $updated = false) {
    global $global;
    if ($errors) display_errors();
    $sql = "SELECT user_name FROM alt_logins WHERE p_uuid='{$_SESSION['user_id']}'";
    $res = $global['db']->Execute($sql);
    $openid = '';
    if(!empty($res->fields['user_name'])) {
        $openid = $res->fields['user_name'];
    }
    echo "<h2>" . _("Change OpenID") . "</h2>";
    shn_form_fopen("ch_openid", null, array('req_message' => false));
    shn_form_hidden(array('seq' => 'entry'));
    shn_form_fsopen(_("OpenID"));
    shn_form_text('', 'openid', 'size="50"', array(
        'value' => $openid
    ));
    shn_form_fsclose();
    shn_form_submit(_('Update'));
    shn_form_fclose();
}

function _shn_pref_openid_validate() {
    global $global;
    //clean the post
    shn_tools_clean_post(&$local_post);
    if(!empty($local_post['openid'])) {
        $openidCheck = $global['db']->Execute("SELECT p_uuid
                FROM alt_logins WHERE p_uuid != '{$_SESSION['user_id']}'
                AND user_name = '{$local_post['openid']}'");
        if(!empty($openidCheck->fields['p_uuid'])) {
            add_error(_('The OpenID had been used by another user.'));
            return false;
        }
        $sql = "SELECT user_name FROM alt_logins WHERE p_uuid='{$_SESSION['user_id']}'";
        $res = $global['db']->Execute($sql);
        if(!empty($res->fields['user_name'])) {
            if($res->fields['user_name'] != $local_post['openid']) {
                $global['db']->Execute('UPDATE alt_logins
                    SET user_name = \'' . $local_post['openid'] . '\'
                    WHERE p_uuid = \'' . $_SESSION['user_id'] . '\'');
            }
        } else {
            $global['db']->Execute('INSERT INTO alt_logins
                VALUES (\'' . $_SESSION['user_id'] . '\',
                    \'' . $local_post['openid'] . '\',
                    \'openid\')');
        }
    }
    add_confirmation(_("The OpenId was succesfully updated"));
    return true;
}