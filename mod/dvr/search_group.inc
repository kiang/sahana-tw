<script language='JavaScript'>
function confirmdelete(name)
{
return confirm('This group with the group head '+name+' and all individuals belongs to that group will be permanently deleted');
}
</script>
<?php
/* $Id: search_group.inc,v 1.5.16.4 2008/03/10 05:29:56 ravithb Exp $ */
/**
 *
 * Search groups and individuals in disaster victims registry
 *
 * PHP version 4 and 5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author     Isuru Samaraweera <isuru@opensource.lk>
 * @copyright  Lanka Software Foundation - http://www.opensource.lk
 * @package    module
 * @subpackage dvr
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 *
 */
function shn_dvr_search_default() {
    shn_form_fopen("search_group", null, array('req_message' => false));
?>
    <?php
    shn_form_hidden(array('seq' => 'search'));
    shn_form_fsopen(_("General Search"));
    print '<div class="info">';
    print _("Please select a group that you are going to search");
    print '<br />';
    print '<br />';
    print '</div>';
    print '<br />';
    $extra_opts['help'] = _("Please Select the group type");
    $extra_opts['req'] = false;
    $extra_opts['value'] = isset($_SESSION['dvr']['person']['typ_of_grp']) ? $_SESSION['dvr']['person']['typ_of_grp'] : '';
    $extra_opts['sort'] = 'option_code';
    shn_form_opt_select('opt_group_type', _("Type of Group"), '', $extra_opts);
    shn_form_text(_('Any Card Number'), 'card', 'size="30"');
    shn_form_text(_('Any Name'), 'name', 'size="30"');
    shn_form_submit(_('Next'));
    shn_form_fsclose();
?>
 <!--<a href="index.php?mod=dvr&act=search_group&seq=advanced">Advanced Search</a>-->
 <?php
    shn_form_fclose();
}
function shn_dvr_group_search_results() {
    global $global;
    $group = $_POST['opt_group_type'];
    $card = $_POST['card'];
    $name = $_POST['name'];
    $search_q1 = null;
    include_once ($global['approot'] . '/mod/dvr/data_access.inc');
    include_once ($global['approot'] . '/mod/dvr/lib.inc');
    $results = _shn_dvr_get_group_search_results($group, $card, $name);
?>
	<br />
	
	
	<br />
    <div id ="result">
	<table>
        <thead>
        		
            <td><?php echo _("Group Type") ?></td>
            <td><?php echo _("Group Head Name") ?></td>
            <td><?php echo _("Family Name") ?></td>
           <!-- <td><?php echo _("Local Name") ?></td>-->
          	<td><?php echo _("NIC") ?></td>
            <td><?php echo _("Address") ?></td>
            <td></td>
            <td></td>
        </thead>
        <?php
    while (!$results->EOF) {
?>
        
        <tr>
        
        <td><?=_shn_dvr_getoptiondescription($results->fields['group_type']) ?></td>
            
            <td><?=$results->fields['full_name'] ?></td>
            <td><?=$results->fields['family_name'] ?></td>
           <!-- <td><?=$results->fields['local_name'] ?></td>-->
          	<td><?=$results->fields['serial'] ?></td>
            <td><?=$results->fields['address'] ?></td>
            
            <td><a href='index.php?mod=dvr&act=edit_group&groupid=<?=$results->fields['group_id'] ?>'>Edit</a></td>
 <td><a href='index.php?mod=dvr&act=delete_group&groupid=<?=$results->fields['group_id'] ?>' onClick="javascript: return confirmdelete('<?=$results->fields['full_name'] ?>')"><?php echo _('Delete'); ?></a></td>
        
        </tr>
 <?php
        $results->MoveNext();
    }
?>
        </table>
        </div>
        
        <?php
    //print 'ok'	;
    
}
//advanced search to be completed
function shn_dvr_advanced_search_group() {
    shn_form_fopen("search", null);
    shn_form_fsopen(_('Advanced Search'));
    shn_form_fsopen(_("General Search"));
    print '<div class="info">';
    print _("Please select a group that you are going to search");
    print '<br />';
    print '<br />';
    print '</div>';
    print '<br />';
    $typ_of_grp = array();
    array_push($typ_of_grp, '-- Select Category --');
    array_reverse($typ_of_grp);
    $typ_of_grp[1] = "Families";
    $typ_of_grp[2] = "Passengers";
    $typ_of_grp[3] = "Tourists";
    $typ_of_grp[4] = "Other";
    $extra_opts['value'] = $typ_of_grp;
    $extra_opts['help'] = _("Enter the National Identity Card Number if available. (Optional)");
    $extra_opts['req'] = false;
    shn_form_select($typ_of_grp, _("Type Of Group"), 'typ_of_grp', "onChange='makeRequest(\"stream.php?mod=rms&amp;act=req_new&amp;seq=ax_sub_cat\",this.value)'", $extra_opts);
    shn_form_fsopen(_('Identity'));
    shn_form_text(_('Identity'), 'idcard', 'size="30"');
    shn_form_checkbox(_('Identity Card'), 'idcard_chk');
    shn_form_checkbox(_('Passport Number'), 'pass_chk');
    shn_form_checkbox(_(v), 'drv_chk');
    shn_form_fsclose();
    shn_form_fsopen(_('Name'));
    shn_form_text(_('Name'), 'name', 'size="30"');
    shn_form_checkbox(_('Full Name'), 'fullname_chk');
    shn_form_checkbox(_('Family Name'), 'familyname_chk');
    shn_form_checkbox(_('Local Name'), 'localname_chk');
    shn_form_fsclose();
    shn_form_submit(_('Next'));
    shn_form_fsclose();
?>
    
 <a href="index.php?mod=dvr&act=search_group">Simple Search</a>
    
    <?php
    shn_form_fclose();
}
?>