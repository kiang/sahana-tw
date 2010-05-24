<?php
function shn_or_adm_acl_install() {
    global $global;
    $db = $global["db"];
    $q = "SELECT value FROM config WHERE module_id='or' AND confkey='acl_base'";
    $res_acl = $db->Execute($q);
    if (!$res_acl->EOF && $res_acl->fields[0] == 'installed') {
        $msg = "ACL Base for Organization Registry is already installed";
    }
    include_once $global['approot'] . 'inc/lib_security/acl_api.inc';
    include_once $global['approot'] . 'inc/lib_security/acl.inc';
    //  include_once $global['approot'].'inc/lib_security/authenticate.inc';
    $acl = new SahanaACL(NULL);
    /** start of or specifc ACL entries
     *
     */
    // add a module named "or"
    $res = shn_acl_add_module("or", "organization reg");
    /** action groups **/
    // add an action group named "deny_admin" under the module "or"
    $res = shn_acl_add_action_group("or", "deny_admin", "deny admin group");
    $res = shn_acl_add_action("or", "deny_admin", "shn_or_acl_setup", "Display ACL configuration info");
    $res = shn_acl_add_action("or", "deny_admin", "shn_or_adm_default", "Display Admin page");
    $res = shn_acl_add_action("or", "deny_admin", "shn_or_adm_or_type", "Display Admin Or type page");
    $res = shn_acl_add_action("or", "deny_admin", "shn_or_adm_sector", "Display Admin Or sector page");
    $res = shn_acl_add_action("or", "deny_admin", "shn_or_adm_location_range", "Display Admin Or location range page");
    $res = shn_acl_add_action("or", "deny_admin", "shn_or_adm_location", "Display Admin Or location page");
    //deny permission for 'deny_admin' action group with in 'or' to 'guest' role
    $res = shn_acl_add_perms_action_group_role('guest', 'or', 'deny_admin');
    $res = shn_acl_add_perms_action_group_role('user', 'or', 'deny_admin');
    // add an action group named "deny_user" under the module "or"
    $res = shn_acl_add_action_group("or", "deny_user", "deny user group");
    //add an action name 'shn_or_reg_org"  under above action groups
    $res = shn_acl_add_action("or", "deny_user", "shn_or_reg_org", "Display Organization Registration form");
    $res = shn_acl_add_action("or", "deny_user", "shn_or_reg_org_cr", "Save Organization Registration info");
    $res = shn_acl_add_action("or", "deny_user", "shn_or_reg_operation", "Save Operation Registration form");
    $res = shn_acl_add_action("or", "deny_user", "shn_or_reg_operation_cr", "Save Operation Registration form");
    $res = shn_acl_add_action("or", "deny_user", "_shn_or_edit_org", "edit Org Registration form");
    $res = shn_acl_add_action("or", "deny_user", "_shn_or_del_org", "delete Org Registration form");
    $res = shn_acl_add_perms_action_group_role('guest', 'or', 'deny_user');
    if ($res_acl->EOF) {
        $q = "INSERT INTO config VALUES('or','acl_base','installed')";
    } else {
        $q = "UPDATE config set value='installed' WHERE module_id='or' AND confkey='acl_base'";
    }
    $res = $db->Execute($q);
    $msg = _("ACL Base for Organization Registry was succesfully installed");
?>
    <div id="result_msg">
       <?php echo $msg; ?>
    </div>
    <br />
<?php
}