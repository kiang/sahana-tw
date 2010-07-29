<?PHP
/**
 *
 * Sahana authentication form generator library.
 *
 *
 * PHP version 4 and 5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @package    framework
 * @subpackage security
 * @author     Ravindra De Silva <ravindra@opensource.lk><ravidesilva@iee.org>
 * @copyright  Lanka Software Foundation - http://www.opensource.lk
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 *
 */
global $global;
include_once $global['approot'] . 'inc/lib_security/lib_auth.inc';
include_once $global['approot'] . "/inc/lib_errors.inc";
include_once $global['approot'] . "/inc/lib_validate.inc";
/**
 * Generates a form to delete a user
 * @return void
 * @access public
 */
function shn_auth_form_user_del($title = "Delete Usert", $mod = "admin", $act = "del_user_cr", $error = false) {
?>
<?php
    if ($error) display_errors();
?> 
<h3>Remove Users</h3>              
<div id="formcontainer">
<?php
    shn_form_fopen($act, $mod);
    shn_form_fsopen(_("Select User"));
    $extra_opts['req'] = true;
    $users = shn_auth_user_list(true, false);
    shn_form_multi_select('users', $users, "Users", null, $extra_opts);
    //$extra_opts['value']=$_SESSION['user'];
    shn_form_fsclose();
?>
<br />

<?php
    //create the submit button
    shn_form_submit(_("Remove user"));
?>

<br />
<?php
    //close the form
    shn_form_fclose();
?>				     
</div>
<?php
}
/**
 * Generates a form to add an user
 * @return void
 * @access public
 */
function shn_auth_form_signup($title = "Create Your Account", $mod = "pref", $act = "signup_cr", $error = false) {
    global $global;
    if ($error) display_errors();
?>               
<h3>SignUp for a SAHANA account</h3> 
<div id="formcontainer">
<?php
    shn_form_fopen($act, $mod);
    shn_form_fsopen(_("Account Details"));
    $extra_opts['req'] = true;
    //$extra_opts['value']=$_SESSION['user'];
    shn_form_text(_("Full Name "), 'account_name', 'size="30"', null);
    shn_form_text(_("User Name "), 'user_name', 'size="30"', $extra_opts);
    if (isset($_POST['password']) || isset($_POST['re_password'])) {
        unset($_POST['password']);
        unset($_POST['re_password']);
    }
    shn_form_password("Password for Login", "password", null, $extra_opts);
    shn_form_password("Confirm Password", "re_password", null, $extra_opts);
    shn_form_fsclose();
    $extra_opts['req'] = true;
    shn_form_fsopen(_("OpenId Login Details"));
?><div class="info"><?php echo _("You can use an OpenID instead of a username/password for login if you like."); ?> </div><br /><?
    shn_form_text(_("OpenID "), 'openid', 'size="30"', null);
    shn_form_fsclose();
    shn_form_fsopen(_("Just a couple of more Details to verify that you are not a bot"));
?>
<br />
<img src="index.php?stream=image&mod=pref&act=captcha" />
<br />
<?php
    //create the submit button
    if (isset($_POST['captcha'])) {
        unset($_POST['captcha']);
    }
    shn_form_text(_("Enter the text in the above image "), 'captcha', 'size="30"', null);
    shn_form_fsclose();
    shn_form_submit(_("Submit"));
?>

<br />
<?php
    //close the form
    shn_form_fclose();
?>				     
</div>
<?php
    // end of form
    
}
/**
 * Generates a form to add an user
 * @return void
 * @access public
 */
function shn_auth_form_user_add($title = "Create Your Account", $mod = "admin", $act = "add_user_cr", $error = false) {
    global $global;
    if ($error) display_errors();
?>               
<h3>Add Users</h3> 
<div id="formcontainer">
<?php
    shn_form_fopen($act, $mod);
    shn_form_fsopen(_("Account Details"));
    $extra_opts['req'] = true;
    //$extra_opts['value']=$_SESSION['user'];
    shn_form_text(_("Full Name "), 'account_name', 'size="30"', $extra_opts);
    shn_form_text(_("User Name "), 'user_name', 'size="30"', $extra_opts);
    shn_form_password("Password for Login", "password", null, $extra_opts);
    shn_form_password("Confirm Password", "re_password", null, $extra_opts);
    shn_form_fsclose();
    $roles = _shn_acl_get_roles(null, false);
    shn_form_fsopen(_("Select Role"));
    shn_form_select($roles, "Roles", 'roles', null, $extra_opts);
    shn_form_fsclose();
    $extra_opts['req'] = true;
    shn_form_fsopen(_("OpenId Login Details"));
?><div class="info"><?php echo _("Option to use an OpenID instead of a username/password if you'd like."); ?> </div><br /><?
    shn_form_text(_("OpenID "), 'openid', 'size="30"', null);
    shn_form_fsclose();
?>
<br />

<?php
    //create the submit button
    shn_form_submit(_("Submit"));
?>

<br />
<?php
    //close the form
    shn_form_fclose();
?>				     
</div>
<?php
    // end of form
    
}
/**
 * Generates a form to change the password
 * @return void
 * @access public
 */
function shn_auth_form_ch_pwd($error = false) {
?>
<center><h2><?php echo _('Change Password'); ?></h2></center>
<?php
    if ($error) display_errors();
?>             
<h3><?php echo _('Change Passwords'); ?></h3>
<div id="formcontainer">
<?php
    $act = "ch_passwd_cr";
    $mod = "pref";
?>
	<h4><?php echo _('Password must adhere to following rules:'); ?></h4>
    1. <?php echo _('Minimum length is 8.'); ?><br />
    2. <?php echo _('At least one letter in the expression.'); ?><br />
    3. <?php echo _('At least one character is numeric or special.'); ?><br />
    4. <?php echo _('Cannot be any of the past four (4) passwords.'); ?><br />
    5. <?php echo _('Passwords cannot be changed more frequently than once per day.'); ?><br />
	
	<br />
	<?php
    shn_form_fopen($act, $mod);
    shn_form_fsopen(_('Old and New Password'));
    $extra_opts['req'] = true;
    //$extra_opts['value']=$_SESSION['user'];
    shn_form_password(_('Old Password'), "old_password", null, $extra_opts);
    shn_form_password(_('New Password'), "password", null, $extra_opts);
    shn_form_password(_('Confirm New Password'), "re_password", null, $extra_opts);
    $user_id = $_SESSION["user_id"];
    shn_form_hidden(array('user' => $user_id));
    shn_form_fsclose();
?>
<br />

<?php
    //create the submit button
    shn_form_submit(_('Change Password'));
?>

<br />
<?php
    //close the form
    shn_form_fclose();
?>				     
</div>
<?php
    // end of form
    
}
function _shn_admin_ch_pwd_cr() {
    global $global;
    $db = $global["db"];
    $VARCHAR = 100;
    //for the moment return true
    //  list($error,$password)=(shn_validate_password($_POST['password']))?array($error,$_POST['password']):array(true,NULL);
    if ((null == $_POST["old_password"]) or (is_null($_POST["old_password"]))) {
        $error = true;
        add_error(_("Old password cannot be empty"));
    } else {
        $old_password = trim($_POST['old_password']);
    }
    if ((null == $_POST["password"]) or (is_null($_POST["password"]))) {
        $error = true;
        add_error(_("New password cannot be empty"));
    } else {
        $password = trim($_POST['password']);
    }
    if ((null == $_POST["re_password"]) or (is_null($_POST["re_password"]))) {
        $error = true;
        add_error(_("Confirm password cannot be empty"));
    } else {
        $re_password = trim($_POST['re_password']);
    }
    if (!($password == $re_password)) {
        $error = true;
        add_error(_("Password and Confirm Password should match"));
    }
    if (!empty($error)) {
        return $error;
    }
    $user_id = trim($_POST['user']);
    $error = shn_change_password($user_id, $old_password, $password);
    if (!empty($error)) {
        return $error;
    }
    shn_admin_ch_pwd($error);
}
/**
 * Generates a form to change the password
 * @return void
 * @access public
 */
function shn_auth_form_reset_pwd($error = false) {
    if ($error) display_errors();
?>  
<h3>Reset Passwords</h3>              
<div id="formcontainer">
<?php
    $act = "reset_pwd_cr";
    $mod = "admin";
?>
	<h4>Password must adhere to following rules:</h4>
    1. Minimum length is 8.<br />
    2. At least one letter in the expression.<br />
    3. At least one character is numeric or special.<br />
	
	<br />
	<?php
    shn_form_fopen($act, $mod);
    $extra_opts['req'] = true;
    $users = shn_auth_user_list(true);
    shn_form_fsopen(_("Select User"));
    shn_form_select($users, "Users", 'user', null, $extra_opts);
    shn_form_fsclose();
    shn_form_fsopen(_("Old and New Password"));
    $extra_opts['req'] = true;
    shn_form_password("New Password", "password", null, $extra_opts);
    shn_form_password("Confirm New Password", "re_password", null, $extra_opts);
    shn_form_fsclose();
?>
<br />

<?php
    //create the submit button
    shn_form_submit(_("Reset Password"));
?>

<br />
<?php
    //close the form
    shn_form_fclose();
?>				     
</div>
<?php
    // end of form
    
}
function _shn_admin_reset_pwd_cr() {
    global $global;
    $db = $global["db"];
    $VARCHAR = 100;
    if ((null == $_POST["password"]) or (is_null($_POST["password"]))) {
        $error = true;
        add_error(_("New password cannot be empty"));
    } else {
        $password = trim($_POST['password']);
    }
    if ((null == $_POST["re_password"]) or (is_null($_POST["re_password"]))) {
        $error = true;
        add_error(_("Confirm password cannot be empty"));
    } else {
        $re_password = trim($_POST['re_password']);
    }
    if (!($password == $re_password)) {
        $error = true;
        add_error(_("Password and Confirm Password should match"));
    }
    if (!empty($error)) {
        return $error;
    }
    $user_id = trim($_POST['user']);
    if (shn_force_change_password($user_id, $password) == true) {
        return;
    }
    add_confirmation(_("The Password was succesfully updated"));
    // shn_admin_reset_pwd($done);
    
}

function shn_auth_form_edit_openid($error = false) {
    global $global;
    $db = $global['db'];
    $q = "SELECT U.p_uuid, U.user_name, AL.user_name AS openid
        FROM users AS U
        LEFT JOIN alt_logins AS AL ON AL.p_uuid = U.p_uuid
            AND AL.type = 'openid'
        ORDER BY U.user_name";
    $res = $db->Execute($q);
    $users = array();
    while (!$res->EOF) {
        $users[$res->fields["p_uuid"]] = $res->fields["user_name"] . ' - '
                . $res->fields["openid"];
        $res->MoveNext();
    }
    if ($error) display_errors();
    echo '<h3>' . _('Edit OpenID') . '</h3>';
    $act = "edit_openid_cr";
    $mod = "admin";
    shn_form_fopen($act, $mod);
    $extra_opts['req'] = true;
    shn_form_fsopen(_("Select User"));
    shn_form_select($users, "Users", 'user', null, $extra_opts);
    shn_form_fsclose();
    shn_form_fsopen(_("New OpenID"));
    shn_form_text('', 'openid', 'size="50"');
    shn_form_fsclose();
    echo '<br />';
    shn_form_submit(_("Save OpenID"));
    shn_form_fclose();
}
function _shn_admin_edit_openid_cr() {
    global $global;
    $db = $global["db"];
    if(!empty($_POST['openid'])) {
        $openidCheck = $global['db']->Execute("SELECT p_uuid
                FROM alt_logins WHERE p_uuid != '{$_POST['user']}'
                AND user_name = '{$_POST['openid']}'");
        if(!empty($openidCheck->fields['p_uuid'])) {
            add_error(_('The OpenID had been used by another user.'));
            return false;
        }
        $sql = "SELECT user_name FROM alt_logins WHERE p_uuid='{$_POST['user']}'";
        $res = $global['db']->Execute($sql);
        if(!empty($res->fields['user_name'])) {
            if($res->fields['user_name'] != $_POST['openid']) {
                $global['db']->Execute('UPDATE alt_logins
                    SET user_name = \'' . $_POST['openid'] . '\'
                    WHERE p_uuid = \'' . $_POST['user'] . '\'');
            }
        } else {
            $global['db']->Execute('INSERT INTO alt_logins
                VALUES (\'' . $_POST['user'] . '\',
                    \'' . $_POST['openid'] . '\',
                    \'openid\')');
        }
    }
    add_confirmation(_("The OpenID was succesfully updated"));
    return true;
}