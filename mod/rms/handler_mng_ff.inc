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
 * @author		Sudheera R. Fernando <sudheera@opensource.lk>
 * @copyright    Lanka Software Foundation - http://www.opensource.lk
 * @package		sahana
 * @subpackage	rms
 * @tutorial
 * @license		http://www.gnu.org/copyleft/lesser.html GNU Lesser General
 * Public License (LGPL)
 */
//global $global;
_shn_rms_print_hedder(_("Fulfil Requests"));
function _shn_mng_set_close($req_id) {
    global $global;
    $sql = "UPDATE rms_request SET status = 'closed' WHERE req_uuid = '$req_id'";
    $res = $global['db']->Execute($sql);
    if ($res === false) {
        $msg = $global['db']->ErrorMsg();
        add_error($msg . ' <br> ' . $sql);
        display_errors();
        return false;
    } else {
        add_confirmation("Close request successfully");
        echo '<br />[ <a href="index.php?mod=rms">' . _('RMS Home') . '</a> ] :: [ <a href="index.php?mod=rms&amp;act=req_ls">' . _('Fullfil Other Request') . '</a> ]';
    }
}
function _shn_rms_mng_change_status($req_id) {
    shn_form_fopen("mng_ff", null, array('req_message' => false));
    shn_form_fsopen(_("Change the status"));
    _shn_html_print_alert(_("You have completly fulfill the request, Do you want to close the request?"));
    shn_form_hidden(array('seq' => 'chg_stat'));
    shn_form_submit(_("Close"), 'close');
    shn_form_fsclose();
    shn_form_fclose();
}
function _shn_rms_mng_fulfill_check($req_id) {
    global $global;
    $sql = "SELECT item_uuid,quantity FROM rms_req_item WHERE req_uuid = '$req_id'";
    $res = $global['db']->Execute($sql);
    if ($res === false) {
        $msg = $global['db']->ErrorMsg();
        add_error($msg . ' <br> ' . $sql);
        display_errors();
        return false;
    } else {
        if ($res->RecordCount() <= 0) {
            return false;
        }
        $res_array = $res->GetArray();
        $bool = true;
        foreach($res_array as $itm) {
            $sql1 = "SELECT SUM(quantity) as qty FROM rms_fulfil WHERE req_uuid = '$req_id' AND item_uuid = '$itm[0]'";
            $result = $global['db']->Execute($sql1);
            if ($result === false) {
                $msg = $global['db']->ErrorMsg();
                add_error($msg . ' <br> ' . $sql);
                display_errors();
                return false;
            } else {
                if ($itm[1] > $result->fields['qty']) {
                    $bool = false;
                    return false;
                }
            }
        }
        return $bool;
    }
}
function _shn_rms_mng_request() {
    $_SESSION['rms_req_id'] = null;
    print '<div id="req_id">';
    print '<div class="info">';
    //print _("Request / Pledge Matching ");
    print _("Select a Request from the ") . '<a href="index.php?mod=rms&act=req_ls">' . _("request list") . '</a>';
    print '<br />';
    print '</div>';
    //    shn_form_fopen("mng_ff",null, array('req_message'=>false));
    //    shn_form_hidden(array('seq'=>'get_req'));
    //    shn_form_fsopen(_("Request"));
    //    shn_form_text(_("Request ID"),'req_id',null,array('br'=>false));
    //    shn_form_submit(_("Next"));
    //    shn_form_fsclose();
    //    shn_form_fclose();
    print '</div>';
}
function _shn_rms_mng_getrequest($req_id) {
    $req = _shn_rms_view_request($req_id);
    if ($req === false) {
        add_error(_("Please Enter a valid Request ID"));
        display_errors();
        _shn_rms_mng_request();
        return false;
    }
    print '<br />';
    shn_html_table_open(array('class' => 'wide'));
    shn_html_tbody_open();
    shn_html_tr_open();
    shn_html_td_open();
    shn_form_fopen("req_ls", null, array('req_message' => false));
    shn_form_hidden(array('seq' => ''));
    shn_form_submit(_("Back"));
    shn_form_fclose();
    shn_html_td_close();
    shn_html_td_open(array('align' => 'right'));
    shn_form_fopen("mng_ff", null, array('req_message' => false));
    shn_form_hidden(array('seq' => 'sel_itm'));
    shn_form_submit(_("Next"));
    shn_form_fclose();
    shn_html_tr_close();
    shn_html_tbody_close();
    shn_html_table_close();
}
function _shn_rms_mng_select_item() {
    $req_id = $_SESSION['rms_req_id'];
    print '<div class="info">';
    print _("This shows the current fulfillment status of the request, ");
    print _("Please select an item from the request to be fulfilled.");
    print '<br />';
    print '</div>';
    _shn_rms_view_reqff($req_id);
}
function _shn_rms_mng_select_pledge() {
    global $global;
    $_SESSION['rms_plg_id'] = null;
    $itm = $_SESSION['rms_req_item'];
?>
<div id="result"> 
 
    <table>
    <caption><?php echo _('Items from inventories'); ?></caption>
    <thead>
        <td><?php echo _("Item") ?></td>
        <td><?php echo _("Amount") ?></td>
        <td><?php echo _("Inventory") ?></td>
        <td><?php echo _('Status') ?></td>
        <td><?php echo _('Location') ?></td>
        <td><?php echo _("Action") ?></td>
    </thead>
    <tbody>
    <?php
    $db = $global['db'];
    $sql = "SELECT item.item_id AS item_id, item.amount AS amount,
    item.state AS state, inventory.inv_uuid AS inv_uuid,
    inventory.inventory_name AS inventory_name
    FROM ims_item_records as item, ims_inventory_records AS inventory
    WHERE item.inv_id=inventory.inv_uuid
    AND item.catalog_id='$itm' AND item.state='available' AND item.amount!=0;";
    // print $sql;
    $res = $db->Execute($sql);
    while (!$res == NULL && !$res->EOF) {
        $sql2 = "SELECT name FROM ct_catalogue WHERE ct_uuid='$itm';";
        $res2 = $global['db']->Execute($sql2);
        $item_name = $res2->fields['name'];
        $amount = $res->fields['amount'];
        $inventory_name = $res->fields['inventory_name'];
        $state = $res->fields['state'];
        $inv_id = $res->fields['inv_uuid'];
        $item_id = $res->fields['item_id'];
        $sql3 = "SELECT location_id FROM location_details WHERE poc_uuid = '" . $res->fields['inv_uuid'] . "'";
        $res3 = $global['db']->Execute($sql3);
        $location = _shn_rms_get_name($res3->fields['location_id']);
?>
        <tr>
        <td><?php print $item_name; ?></td>
        <td><?php print $amount; ?></td>
        <td><?php print $inventory_name; ?></td>
        <td><?php print $state; ?></td>
        <td><?php print $location; ?></td>
        <td><a href="index.php?mod=rms&act=fullfill_from_inventory&seq=<?php echo 'mng_ff_inventory'; ?>&item_id=<?php echo $item_id; ?>&inv_id=<?php echo $inv_id; ?>&ct_uuid=<?php echo $itm; ?>"><?php print ("Select"); ?></a></td>
        </tr>
    <?php
        $res->MoveNext();
    }
?>
    </tbody>
    </table>
    </div>
    <?php
    _shn_rms_mng_select_pledge_alternates();
}
function _shn_rms_mng_select_pledge_alternates() {
    global $global;
    echo '<div><h3><b><em>' . _("Alternate Items:") . '</em></b></h3></div>';
    $_SESSION['rms_plg_id'] = null;
    $itm = $_SESSION['rms_req_item'];
?>
<div id="result">

    <table>
    <caption><?php echo _('Items from inventories'); ?></caption>
    <thead>
        <td><?php echo _("Item") ?></td>
        <td><?php echo _("Amount") ?></td>
        <td><?php echo _("Inventory") ?></td>
        <td><?php echo _('Status') ?></td>
        <td><?php echo _('Location') ?></td>
        <td><?php echo _("Action") ?></td>
    </thead>
    <tbody>
    <?php
    $db = $global['db'];
    $sql = "SELECT item.item_id AS item_id, item.amount AS amount,
    item.state AS state, inventory.inv_uuid AS inv_uuid,
    inventory.inventory_name AS inventory_name,
    item.catalog_id
    FROM ims_item_records as item, ims_inventory_records AS inventory
    WHERE item.inv_id=inventory.inv_uuid
    AND item.state='available' AND item.amount!=0
    AND (item.catalog_id IN (SELECT alternate FROM ims_alternate WHERE catalog_id = '{$itm}')
        OR item.catalog_id IN (SELECT catalog_id FROM ims_alternate WHERE alternate = '{$itm}'))";
    $res = $db->Execute($sql);
    while (!$res == NULL && !$res->EOF) {
        $sql2 = "SELECT name FROM ct_catalogue WHERE ct_uuid='{$res->fields['catalog_id']}';";
        $res2 = $global['db']->Execute($sql2);
        $item_name = $res2->fields['name'];
        $amount = $res->fields['amount'];
        $inventory_name = $res->fields['inventory_name'];
        $state = $res->fields['state'];
        $inv_id = $res->fields['inv_uuid'];
        $item_id = $res->fields['item_id'];
        $sql3 = "SELECT location_id FROM location_details WHERE poc_uuid = '" . $res->fields['inv_uuid'] . "'";
        $res3 = $global['db']->Execute($sql3);
        $location = _shn_rms_get_name($res3->fields['location_id']);
?>
        <tr>
        <td><?php print $item_name; ?></td>
        <td><?php print $amount; ?></td>
        <td><?php print $inventory_name; ?></td>
        <td><?php print $state; ?></td>
        <td><?php print $location; ?></td>
        <td><a href="index.php?mod=rms&act=fullfill_from_inventory&seq=<?php echo 'mng_ff_inventory'; ?>&item_id=<?php echo $item_id; ?>&inv_id=<?php echo $inv_id; ?>&ct_uuid=<?php echo $itm; ?>"><?php print ("Select"); ?></a></td>
        </tr>
    <?php
        $res->MoveNext();
    }
?>
    </tbody>
    </table>
    </div>
    <?php
}
function _shn_rms_mng_addff($max_qty) {
    $req_id = $_SESSION['rms_req_id'];
    $req_item = $_SESSION['rms_req_item'];
    $plg_id = $_SESSION['rms_req_plg'];
    print '<div class="info">';
    print _("Allocate items from a pledge to fulfill a request, ");
    print _("The 'Maximum Allocatable Quantity' is the maximum you are allowed to allocate.");
    print '<br />';
    print '</div>';
    _shn_rms_view_requestff($req_id, $req_item);
    _shn_html_print_message(_("Allocate items from the selected pledge to the request"));
    shn_form_fopen("mng_ff", null, array('req_message' => false));
    shn_form_hidden(array('seq' => 'finish'));
    shn_form_fsopen(_("Allocate"));
    shn_form_text(_("Item"), 'itm_id', 'readonly="true"', array('value' => _shn_rms_get_itemname($req_item)));
    shn_form_text(_("Maximum Allocatable Quantity"), 'max_qty', 'readonly="true"', array('value' => $max_qty));
    shn_form_text(_("Quantity"), 'qty', null);
    shn_form_submit(_("Next"));
    shn_form_fsclose();
    shn_form_fclose();
    print '<br />';
    shn_form_fopen("req_ls", null, array('req_message' => false));
    shn_form_hidden(array('seq' => ''));
    shn_form_fsopen(_("Cancel Fulfillment Process"));
    shn_form_submit(_("Cancel"));
    shn_form_fsclose();
    shn_form_fclose();
}
function _shn_rms_allocate_item($ct_uuid, $inv_id) {
    global $global;
    $db = $global['db'];
    $sql = "SELECT name FROM ct_catalogue WHERE ct_uuid='$ct_uuid';";
    $res = $db->Execute($sql);
    $item_id = $_SESSION['rms']['item_id'];
    $sql1 = "SELECT amount FROM ims_item_records WHERE inv_id='$inv_id' AND item_id='$item_id';";
    $res1 = $db->Execute($sql1);
    //print $sql1;
    $sql2 = "SELECT inventory_name FROM ims_inventory_records WHERE inv_uuid='$inv_id';";
    $res2 = $db->Execute($sql2);
    _shn_rms_view_requestff($_SESSION['rms_req_id'], $_SESSION['rms_req_item']);
    shn_form_fopen('fullfill_from_inventory');
    shn_form_fsopen(_('Allocate'));
    shn_form_text(_('Item : '), 'item', 'size="25" readonly=true', array('value' => $res->fields['name']));
    shn_form_text(_('Allocatable quantity (Max) : '), 'max_quantity', 'size="25" readonly=true', array('value' => $res1->fields['amount']));
    shn_form_text(_('Inventory : '), 'inventory', 'size="25" readonly=true', array('value' => $res2->fields['inventory_name']));
    shn_form_text(_('Quantity : '), 'quantity', 'size="25"', null);
    shn_form_fsclose();
    shn_form_hidden(array('seq' => 'inventory_item_allocate_db'));
    shn_form_submit(_('Submit'));
    shn_form_fclose();
}
function _shn_rms_item_allocation_db() {
    global $global;
    $db = $global['db'];
    $item_id = $_SESSION['rms']['item_id'];
    $available_quantity = $_SESSION['rms']['max_quantity'] - $_SESSION['rms']['quantity'];
    $req_uuid = $_SESSION['rms_req_id'];
    $catalog_id = $_SESSION['rms_req_item'];
    $quantity = $_SESSION['rms']['quantity'];
    if ($quantity <= 0) {
        add_error("Quantity should be larger than 0");
        display_errors();
        _shn_rms_allocate_item($catalog_id, $item_id);
    } else {
        $sql = "UPDATE ims_item_records SET amount='$available_quantity' WHERE item_id='$item_id';";
        $res = $db->Execute($sql);
        $dt2 = date("Y-m-d H:i:s");
        $sql = "INSERT INTO rms_fulfil(req_uuid, item_uuid, quantity, ff_date) VALUES ('$req_uuid', '$catalog_id', '$quantity', '$dt2')";
        $res1 = $db->Execute($sql);
        if ($res1 === false) {
            $msg = $global['db']->ErrorMsg();
            add_error($msg . ' <br> ' . $sql);
            display_errors();
        } else {
            add_confirmation('Request successfully fulfilled using ' . $quantity . ' from inventory');
            $stat = _shn_rms_mng_fulfill_check($_SESSION['rms_req_id']);
            if ($stat) {
                _shn_rms_mng_change_status($_SESSION['rms_req_id']);
            }
            _shn_rms_view_reqff($_SESSION['rms_req_id']);
            echo '<br />[ <a href="index.php?mod=rms">' . _('RMS Home') . '</a> ] :: [ <a href="index.php?mod=rms&amp;act=req_ls&amp;seq=view_req&amp;req_id=' . $_SESSION['rms_req_id'] . '">' . _('Fullfil Another Item From the Same Request') . '</a> ] :: [ <a href="index.php?mod=rms&amp;act=req_ls">' . _('Fullfil Other Request') . '</a> ]';
        }
    }
}
$seq = isset($_REQUEST['seq']) ? $_REQUEST['seq'] : '';
switch ($seq) {
    case '':
        $_SESSION['rms_req_id'] = null;
        $_SESSION['rms_mng_ff_added'] = false;
        _shn_rms_mng_request();
    break;
    case 'get_req':
        $req_id = $_REQUEST['req_id'];
        $_SESSION['rms_req_id'] = $req_id;
        $_SESSION['rms_mng_ff_added'] = false;
        _shn_rms_mng_getrequest($req_id);
    break;
    case 'sel_itm':
        $_SESSION['rms_req_item'] = null;
        _shn_rms_mng_select_item();
    break;
    case 'sel_plg':
        $_SESSION['rms_req_id'] = $_REQUEST['req_id'];
        $_SESSION['rms_req_item'] = $_REQUEST['itm_id'];
        $req = _shn_rms_get_request($_SESSION['rms_req_id']);
        echo '<strong>' . _('Location : ') . '</strong>' . _shn_rms_get_name($req['loc_uuid']);
        _shn_rms_mng_select_pledge();
    break;
    case 'add_ff':
        $_SESSION['rms_req_plg'] = $_REQUEST['plg_id'];
        $max_qty = $_REQUEST['max_qty'];
        _shn_rms_mng_addff($max_qty);
    break;
    case 'mng_ff_inventory':
        $_SESSION['rms']['ct_uuid'] = $_REQUEST['ct_uuid'];
        $_SESSION['rms']['inv_id'] = $_REQUEST['inv_id'];
        $_SESSION['rms']['item_id'] = $_REQUEST['item_id'];
        _shn_rms_allocate_item($_SESSION['rms']['ct_uuid'], $_SESSION['rms']['inv_id']);
    break;
    case 'inventory_item_allocate_db':
        $_SESSION['rms']['quantity'] = $_POST['quantity'];
        $_SESSION['rms']['max_quantity'] = $_POST['max_quantity'];
        _shn_rms_item_allocation_db($_SESSION['rms']['quantity'], $_SESSION['rms']['max_quantity']);
    break;
    case 'chg_stat':
        _shn_mng_set_close($_SESSION['rms_req_id']);
    break;
    case 'finish';
    if ($_POST['qty'] > $_POST['max_qty']) {
        add_error(_("Please enter a valid 'Quantity', The 'Quantity' should be equal or less than the Maximum Value"));
        display_errors();
        _shn_rms_mng_addff($_POST['max_qty']);
        return false;
    }
    if ($_POST['qty'] <= 0) {
        add_error(_("Please enter a valid 'Quantity', The 'Quantity' should be larger than the 0"));
        display_errors();
        _shn_rms_mng_addff($_POST['max_qty']);
        return false;
    }
    if (!$_SESSION['rms_mng_ff_added']) {
        $req_id = $_SESSION['rms_req_id'];
        $req_item = $_SESSION['rms_req_item'];
        $plg_id = $_SESSION['rms_req_plg'];
        $qty = $_POST['qty'];
        $res = _shn_rms_add_ff($req_id, $req_item, $plg_id, $qty);
        $_SESSION['rms_mng_ff_added'] = $res;
    }
    if ($_SESSION['rms_mng_ff_added']) {
        add_confirmation(_("The Fulfillment Data was successfully added"));
        _shn_rms_view_reqff($_SESSION['rms_req_id']);
        $stat = _shn_rms_mng_fulfill_check($_SESSION['rms_req_id']);
        $_SESSION['rms_mng_ff_added'] = 0;
        if ($stat) {
            _shn_rms_mng_change_status($_SESSION['rms_req_id']);
        }
    }
    echo '<br />[ <a href="index.php?mod=rms">' . _('RMS Home') . '</a> ] :: [ <a href="index.php?mod=rms&amp;act=mng_ff&amp;seq=get_req&amp;req_id=' . $_SESSION['rms_req_id'] . '">' . _('Fullfil Another Item From the Same Request') . '</a> ] :: [ <a href="index.php?mod=rms&amp;act=req_ls">' . _('Fullfil Other Request') . '</a> ]';
break;
break;
}
?>
