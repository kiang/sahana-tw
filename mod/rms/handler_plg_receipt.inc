<?php
global $global;
$page = isset($_REQUEST['page_no']) ? $_REQUEST['page_no'] : 1;
$db = $global['db'];

if (isset($_POST['date_begin'])) {
    $_SESSION['rms_pledge_receipt']['date_begin'] = $_POST['date_begin'];
}
if (isset($_POST['date_end'])) {
    $_SESSION['rms_pledge_receipt']['date_end'] = $_POST['date_end'];
}
if (isset($_POST['receipt_status'])) {
    $_SESSION['rms_pledge_receipt']['receipt_status'] = $_POST['receipt_status'];
}
if (isset($_POST['appreciation_status'])) {
    $_SESSION['rms_pledge_receipt']['appreciation_status'] = $_POST['appreciation_status'];
}
$sql = "SELECT * FROM rms_pledge WHERE 1";
if(!empty($_SESSION['rms_pledge_receipt']['date_begin']) && !empty($_SESSION['rms_pledge_receipt']['date_end'])) {
    $sql .= ' AND plg_date >= \'' . $_SESSION['rms_pledge_receipt']['date_begin'] . ' 00:00:00\'';
    $sql .= ' AND plg_date <= \'' . $_SESSION['rms_pledge_receipt']['date_end'] . ' 23:59:59\'';
} elseif(!empty($_SESSION['rms_pledge_receipt']['date_begin'])) {
    $sql .= ' AND plg_date >= \'' . $_SESSION['rms_pledge_receipt']['date_begin'] . ' 00:00:00\'';
    $sql .= ' AND plg_date <= \'' . $_SESSION['rms_pledge_receipt']['date_begin'] . ' 23:59:59\'';
} elseif(!empty($_SESSION['rms_pledge_receipt']['date_end'])) {
    $sql .= ' AND plg_date >= \'' . $_SESSION['rms_pledge_receipt']['date_end'] . ' 00:00:00\'';
    $sql .= ' AND plg_date <= \'' . $_SESSION['rms_pledge_receipt']['date_end'] . ' 23:59:59\'';
}
if(!empty($_SESSION['rms_pledge_receipt']['receipt_status']) && $_SESSION['rms_pledge_receipt']['receipt_status'] != 'all') {
    $sql .= ' AND receipt_status = \'' . $_SESSION['rms_pledge_receipt']['receipt_status'] . '\'';
}
if(!empty($_SESSION['rms_pledge_receipt']['appreciation_status']) && $_SESSION['rms_pledge_receipt']['appreciation_status'] != 'all') {
    $sql .= ' AND appreciation_status = \'' . $_SESSION['rms_pledge_receipt']['appreciation_status'] . '\'';
}

if(!empty($_POST['receipt'])) {
    foreach($_POST['receipt'] AS $key => $val) {
        if($val == '1') {
            $db->Execute('UPDATE rms_pledge
                SET receipt_status = \'receipt_taken\'
                WHERE plg_uuid = \'' . $key . '\'');
        }
    }
}
if(!empty($_POST['appreciation'])) {
    foreach($_POST['appreciation'] AS $key => $val) {
        if($val == '1') {
            $db->Execute('UPDATE rms_pledge
                SET appreciation_status = \'appreciation_taken\'
                WHERE plg_uuid = \'' . $key . '\'');
        }
    }
}

_shn_rms_print_hedder(_("Pledge receipt and certificate of appreciation"));

$paging = false;

// apply filters if any
$fstr = ''; //filter string

if (!isset($page) || $page == '' || !is_numeric($page))
    $page = 1;
$rows = 100;
// create pagnating links
$plg_count = _shn_rms_get_recordcount('rms_pledge', $fstr);
$page_count = ceil($plg_count / $rows);
if (1 < $page_count) {
    $paging = true;
    $page_list = '';
    if (1 < $page) {
        $page_list.= '<a href="index.php?mod=rms&amp;act=plg_receipt&amp;page_no=' . ($page - 1) . '"> ' . _("Previous") . '</a> ';
    }
    for ($i = 1; $i <= $page_count; $i++) {
        if ($page != $i) {
            $page_list.= ' <a href="index.php?mod=rms&amp;act=plg_receipt&amp;page_no=' . $i . '"> <small>' . $i . '</small></a> ';
        } else {
            $page_list.= '<small>' . $i . '</small>';
        }
    }
    if ($page_count != $page)
        $page_list.= '<a href="index.php?mod=rms&amp;act=plg_receipt&amp;page_no=' . ($page + 1) . '">' . _("Next") . '</a>';
}
//set order-by
$sql.= " ORDER BY plg_date DESC";

$rs = $db->PageExecute($sql, $rows, $page);
$rs_arr = $rs->GetArray();
$th_res[] = array(_('Receipt'), _('Certificate of appreciation'), _("Donor"), _("Date"), _('Action'));
$receiptStatuses = array(
    'receipt_taken' => _("Receipt taken"),
    'send_receipt' => _("Need to send receipt"),
    'appreciation_taken' => _("Certificate of appreciation taken"),
    'send_appreciation' => _("Need to send certificate of appreciation"),
);
foreach ($rs_arr as $r) {
    $view_url = '<a href=index.php?mod=rms&amp;act=plg_receipt&amp;seq=view_plg&amp;plg_id=' . $r['plg_uuid'] . '>' . _("Details") . '</a>';
    if (!empty($r['organization'])) {
        $r['donor_name'] = $r['donor_name'] . ' ( ' . $r['organization'] . ' )';
    }
    $receipt = $appreciation = '';
    if (empty($r['receipt_status'])) {
        $receipt = ' - ';
    } else {
        $receipt = $receiptStatuses[$r['receipt_status']];
        if ($r['receipt_status'] == 'send_receipt') {
            $receipt .= ' | <input type="checkbox" name="receipt[' . $r['plg_uuid'] . ']" value="1" />' . _('Sent');
        }
    }
    if (empty($r['appreciation_status'])) {
        $appreciation = ' - ';
    } else {
        $appreciation = $receiptStatuses[$r['appreciation_status']];
        if ($r['appreciation_status'] == 'send_appreciation') {
            $appreciation .= ' | <input type="checkbox" name="appreciation[' . $r['plg_uuid'] . ']" value="1" />' . _('Sent');
        }
    }
    $tb_res[] = array($receipt, $appreciation, $r['donor_name'], $r['plg_date'], $view_url);
}

shn_form_fopen('plg_receipt', null, array('req_message' => false));
shn_form_fsopen(_('Filter'));
shn_form_date(_('Date begin:'), 'date_begin', array(
    'value' => isset($_SESSION['rms_pledge_receipt']['date_begin']) ?
            $_SESSION['rms_pledge_receipt']['date_begin'] : ''
));
shn_form_date(_('Date end:'), 'date_end', array(
    'value' => isset($_SESSION['rms_pledge_receipt']['date_end']) ?
            $_SESSION['rms_pledge_receipt']['date_end'] : ''
));
shn_form_radio(array(
    'all' => _('All'),
    'receipt_taken' => _("Receipt taken"),
    'send_receipt' => _("Need to send receipt"),
        ), _('Receipt :'), 'receipt_status', null, array(
    'value' => isset($_SESSION['rms_pledge_receipt']['receipt_status']) ?
            $_SESSION['rms_pledge_receipt']['receipt_status'] : ''
));
shn_form_radio(array(
    'all' => _('All'),
    'appreciation_taken' => _("Certificate of appreciation taken"),
    'send_appreciation' => _("Need to send certificate of appreciation"),
        ), _('Certificate of appreciation :'), 'appreciation_status', null, array(
    'value' => isset($_SESSION['rms_pledge_receipt']['appreciation_status']) ?
            $_SESSION['rms_pledge_receipt']['appreciation_status'] : ''
));
shn_form_fsclose();
shn_form_submit(_('Search'));
shn_form_fclose();

if (1 <= count($rs_arr)) {
    if ($paging)
        print $page_list;
    echo '<form method="POST" action="index.php?mod=rms&amp;act=plg_receipt&amp;page_no=' . $page . '">';
    shn_html_table($tb_res, $th_res, null, array('class' => 'wide'));
    echo '<input type="submit"></form>';
} else {
    _shn_html_print_alert(_("There are no Pledges on the Database that matches your criteria"));
}
print '<br />[ <a href="index.php?mod=rms">' . _("RMS Home") . '</a> ] :: [ <a href="index.php?mod=rms&amp;act=plg_ls">' . _("Back to Pledge List") . '</a> ]';