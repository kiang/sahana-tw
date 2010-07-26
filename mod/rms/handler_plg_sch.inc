<?php
/**
 * The Sahana Resquest Management System
 *
 * PHP version 4 and 5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author       Sudheera R. Fernando <sudheera@opensource.lk>
 * @copyright    Lanka Software Foundation - http://www.opensource.lk
 * @package      sahana
 * @subpackage   rms
 * @tutorial
 * @license      http://www.gnu.org/copyleft/lesser.html GNU Lesser General
 * Public License (LGPL)
 */
_shn_rms_print_hedder(_("Search for Pledges"));
function _shn_rms_sch_getkeywords() {
    $categories = array();
    $cat_arr = _shn_rms_get_categories();
    array_push($categories, _('All'));
    foreach ($cat_arr as $cat) {
        $categories[$cat['ct_uuid']] = $cat['name'];
    }
    //---------------------------------------------------------------------------------
    $status = array();
    $status['all'] = _("All");
    $status['not confirmed'] = _("Not Confirmed");
    $status['confirmed'] = _("Confirmed");
    $status['delivered'] = _("Delivered");
    $status['discarded'] = _("Discarded");
    //-----------------------------------------------------------------------------------
    $date = array();
    //---------------------------------------------------------------------------------
    shn_form_fopen("plg_sch", null, array('req_message' => false));
    shn_form_fsopen(_('Select Criteria'));
    shn_form_hidden(array('seq' => 'get_keywords'));
    shn_form_select($categories, _("Category"), 'category', null, array('br' => false));
    print '<br/>';
    print '<br/>';
    shn_form_select($status, _("Status"), 'status', null, array('br' => false));
    print '<br/>';
    print '<br/>';
    shn_form_fsclose();
    shn_form_submit(_("Search"));
    shn_form_fclose();
}
function _shn_rms_shn_get_results() {
    global $global;
    $db = $global['db'];
    $paging = false;
    $ctgr = $_SESSION['rms_plg_sch']['category'];
    $stts = $_SESSION['rms_plg_sch']['status'];
    $cat_qry2 = "SELECT ct.ct_uuid, rpm.status, rpm.inventory, rpm.quantity,
                plg.plg_uuid, plg.donor_name, plg.plg_date FROM ct_catalogue as ct
                 JOIN rms_plg_item as rpm ON rpm.item_uuid = ct.ct_uuid 
                 JOIN rms_pledge as plg ON plg.plg_uuid=rpm.plg_uuid
                 WHERE ct.final_flag = '1'";
    if(!empty($ctgr)) {
        $cat_qry = "SELECT serial FROM ct_catalogue WHERE ct_uuid='" . $_SESSION['rms_plg_sch']['category'] . "'";
        $res1 = $db->Execute($cat_qry);
        $cat_serial = $res1->fields['serial'];
        $cat_qry2.= " AND ct.serial LIKE '" . $cat_serial . "%' ";
    }
    if ($stts != 'all') {
        $cat_qry2.= " AND rpm.status='$stts' ";
    }
    $cat_qry2 .= ' ORDER BY plg.plg_date DESC';
    $headers = array();
    include_once $global['approot'] . "/inc/lib_paging.inc";
    $res2 = shn_paging_get_report($cat_qry2, '10', NULL, array('post' => true, 'return' => true));
    return $res2;
}
function _shn_rms_sch_results() {
    $_SESSION['rms_plg_schres'] = null;
    $res = _shn_rms_shn_get_results();
    $tbl_arr = array();
    $tbl_hd_arr = array();
    if (!$res->EOF) {
        if ($_SESSION['isadmin'] == TRUE) {
            $tbl_hd_arr[0] = array(_("Donor"), _("Category"), _("Item"), _("Quantity"), _("Date"), _("status"), _("Inventory"));
        } else {
            $tbl_hd_arr[0] = array(_("Category"), _("Item"), _("Quantity"), _("Date"), _("status"), _("Inventory"));
        }
        $view_url = '<a href=index.php?mod=rms&amp;act=plg_sch&amp;seq=view_plg&amp;plg_id=' . $res->fields['plg_uuid'] . '>' . _("Details") . '</a>';
        $i = 0;
        while (!$res->EOF) {
            if ($_SESSION['isadmin'] == TRUE) {
                $tbl_arr[$i] = array($res->fields['donor_name'], _shn_rms_get_cat_name(_shn_rms_get_parent_catalogue($res->fields['ct_uuid'])), _shn_rms_get_cat_name($res->fields['ct_uuid']), $res->fields['quantity'], $res->fields['plg_date'], $res->fields['status'], _shn_rms_get_inventory_name($res->fields['inventory']));
            } else {
                $tbl_arr[$i] = array(_shn_rms_get_cat_name(_shn_rms_get_parent_catalogue($res->fields['ct_uuid'])), _shn_rms_get_cat_name($res->fields['ct_uuid']), $res->fields['quantity'], $res->fields['plg_date'], $res->fields['status'], _shn_rms_get_inventory_name($res->fields['inventory']));
            }
            $res->MoveNext();
            $i++;
        }
        print '<br />';
        shn_html_table($tbl_arr, $tbl_hd_arr, null, array('class' => 'wide'));
    } else {
        print '<br />';
        _shn_html_print_alert(_("There are no results that match your search criteria."));
    }
}
function _shn_rms_get_donors_array() {
    global $global;
    $db = $global['db'];
    $sql = "SELECT DISTINCT donor_uuid FROM rms_pledge";
    $rec = $db->Execute($sql);
    $donor = array();
    $i = 0;
    if ($rec === false) {
        $msg = $global['db']->ErrorMsg();
        add_error($msg . '<br />' . $sql);
        display_errors();
        return false;
    } elseif (1 <= $rec->RecordCount()) {
        While (!$rec->EOF) {
            $donor[$i] = $rec->fields['donor_uuid'];
            $rec->MoveNext();
            $i++;
        }
        return $donor;
    }
}
$seq = isset($_REQUEST['seq']) ? $_REQUEST['seq'] : '';
switch ($seq) {
    case '':
        $_SESSION['isadmin'] = _shn_rms_get_user_group();
        _shn_rms_sch_getkeywords();
    break;
    case 'get_keywords':
        $_SESSION['rms_plg_sch']['category'] = $_REQUEST['category'];
        $_SESSION['rms_plg_sch']['status'] = $_REQUEST['status'];
        _shn_rms_sch_getkeywords();
        _shn_rms_sch_results();
    break;
    case 'view_plg':
        $plg_id = $_REQUEST['plg_id'];
        _shn_rms_view_pledge($plg_id);
        $keywords = $_REQUEST['keywords'];
        print '<br />[ <a href="index.php?mod=rms">' . _("RMS Home") . '</a> ] :: [ <a href="index.php?mod=rms&amp;act=plg_sch">' . _("Back to Pledge Search") . '</a> ]';
    break;
}
?>