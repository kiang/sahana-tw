<?php
/* $Id $ */
/** 
 * Missing Person Registry Common Libraries
 *
 * PHP version 4 and 5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *@author     Janaka wickramasinghe<janakaopensource.lk>
 * @author	   Isuru Samaraweera<isuru@opensource.lk>
 * @copyright  Lanka Software Foundation - http://www.opensource.lk
 * @package    module
 * @subpackage mpr
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 */
/**
 * Actual Presentaion of latest updates
 *
 * @param mixed $details
 * @access public
 * @return void
 */
function shn_mpr_search_show_verticle($details) {
    global $global;
    global $conf;
?>
<DIV ID="result">
<TABLE>
<?
    for ($i = 0;$i < count($details);$i+= 2) {
        if ($details[$i]) {
?>
    <TR>
        <TD>
<?php
            if (shn_show_thumb_url($details[$i]['p_uuid'], 'database', 'mpr', 'addmp_img', false)) echo '<br /><a href="index.php?mod=' . $global['module'] . '&act=editmp&id=' . $details[$i]['p_uuid'] . '">' . $details[$i]['family_name'] . $details[$i]['full_name'] . '</a><br />';
            else {
                echo '<br /><a href="index.php?mod=' . $global['module'] . '&act=editmp&id=' . $details[$i]['p_uuid'] . '">' . $details[$i]['full_name'] . '</a><br />';
                echo ($details[$i]['height'] ? _('Height : ') . $details[$i]['height'] . '<br>' : '');
                echo ($details[$i]['weight'] ? _('Weight : ') . $details[$i]['weight'] . '<br>' : '');
                echo ($details[$i]['opt_eye_color'] ? _('Eye Colour : ') . shn_get_field_opt($details[$i]['opt_eye_color'], 'opt_eye_color') . '<br>' : '');
                echo ($details[$i]['opt_skin_color'] ? _('Skin Colour : ') . shn_get_field_opt($details[$i]['opt_skin_color'], 'opt_skin_color') . '<br>' : '');
                echo ($details[$i]['opt_hair_color'] ? _('Hair Colour : ') . shn_get_field_opt($details[$i]['opt_hair_color'], 'opt_hair_color') . '<br>' : '');
                echo ($details[$i]['last_seen'] ? _('Last Seen : ') . $details[$i]['last_seen'] . '<br>' : '');
                echo ($details[$i]['last_clothing'] ? _('Last Clothing : ') . $details[$i]['last_clothing'] . '<br>' : '');
                echo ($details[$i]['comments'] ? _('Comments : ') . $details[$i]['comments'] : '');
            }
            echo '<br /><a href="index.php?mod=admin&act=ims_assign&id=' . $details[$i]['p_uuid'] . '" class="hideprint">' . _('Assign to more incidents') . '</a>';
?>
        </TD>
<?php
            $nextIndex = $i + 1;
            if (isset($details[$nextIndex])) {
?>
        <TD>
<?php
                if (shn_show_thumb_url($details[$nextIndex]['p_uuid'], 'database', 'mpr', 'addmp_img')) echo '<br /><a href="index.php?mod=' . $global['module'] . '&act=editmp&id=' . $details[$nextIndex]['p_uuid'] . '">' . $details[$nextIndex]['full_name'] . '</a><br />';
                else {
                    echo '<br /><a href="index.php?mod=' . $global['module'] . '&act=editmp&id=' . $details[$nextIndex]['p_uuid'] . '">' . $details[$nextIndex]['full_name'] . '</a><br />';
                    echo ($details[$nextIndex]['height'] ? _('Height : ') . $details[$nextIndex]['height'] . '<br>' : '');
                    echo ($details[$nextIndex]['weight'] ? _('Weight : ') . $details[$nextIndex]['weight'] . '<br>' : '');
                    echo ($details[$nextIndex]['opt_eye_color'] ? _('Eye Colour : ') . shn_get_field_opt($details[$nextIndex]['opt_eye_color'], 'opt_eye_color') . '<br>' : '');
                    echo ($details[$nextIndex]['opt_skin_color'] ? _('Skin Colour : ') . shn_get_field_opt($details[$nextIndex]['opt_skin_color'], 'opt_skin_color') . '<br>' : '');
                    echo ($details[$nextIndex]['opt_hair_color'] ? _('Hair Colour : ') . shn_get_field_opt($details[$nextIndex]['opt_hair_color'], 'opt_hair_color') . '<br>' : '');
                    echo ($details[$nextIndex]['last_seen'] ? _('Last Seen : ') . $details[$nextIndex]['last_seen'] . '<br>' : '');
                    echo ($details[$nextIndex]['last_clothing'] ? _('Last Clothing : ') . $details[$nextIndex]['last_clothing'] . '<br>' : '');
                    echo ($details[$nextIndex]['comments'] ? _('Comments : ') . $details[$nextIndex]['comments'] : '');
                }
                echo '<br /><a href="index.php?mod=admin&act=ims_assign&id=' . $details[$i]['p_uuid'] . '" class="hideprint">' . _('Assign to more incidents') . '</a>';
?>
        </TD>
<?php
            }
?>
    </TR>
<?php
        }
    }
?>
</TABLE>
</DIV>
<?php
}
/**
 * Shows the latest updates on missing and found perople
 *
 * @param string $type
 * @param int $limit
 * @access public
 * @return void
 */
function shn_mpr_home_show_mpr($type = "missing", $limit = 5, $offset = 0) {
    global $global;
    #$incident_ids = shn_db_get_all_incidents($_SESSION['user_pref_ims_incident_id']);
    #$incident_sql = implode($incident_ids,"','");
    #$incident_sql = "'$incident_sql'";
    $sql = "SELECT
                a.p_uuid AS p_uuid, a.full_name AS full_name, a.family_name as family_name,
                b.height AS height, b.weight AS weight,
                b.opt_eye_color AS opt_eye_color,
                b.opt_skin_color AS opt_skin_color, b.opt_hair_color AS opt_hair_color,
                c.last_seen AS last_seen, c.last_clothing AS last_clothing,
                c.comments AS comments
            FROM person_uuid a
            LEFT OUTER JOIN person_physical b USING (p_uuid)
            INNER JOIN person_missing c USING (p_uuid)
            INNER JOIN person_status d USING (p_uuid)
            INNER JOIN resource_to_incident e ON a.p_uuid = e.x_uuid
            WHERE e.incident_id = '{$_SESSION['user_pref_ims_incident_id']}'";
    switch($type) {
        case 'found':
            $sql .= ' AND d.opt_status != \'mis\'';
            break;
        case 'injured':
            $sql .= ' AND d.opt_status = \'inj\'';
            break;
        case 'deceased':
            $sql .= ' AND d.opt_status = \'dec\'';
            break;
        case 'missing':
        default:
            $sql .= ' AND d.opt_status = \'mis\'';
    }
    $sql .= "ORDER BY d.updated DESC";
    $rs = $global['db']->SelectLimit($sql, $limit, $offset);
    if ($rs) {
        $arr = $rs->GetAll();
        //  shn_mpr_search_show_verticle($arr);
        $_SESSION['result'] = $arr;
    }
    $rs1 = $global['db']->Execute($sql);
    //print 'missing'.$rs1;
    $count = 0;
    while (!$rs1->EOF) {
        $count++;
        $rs1->MoveNext();
    }
    return $count;
}

function shn_mpr_search_process($type = "missing", $limit = 5, $offset = 0) {
    global $global;
    $fields = 'pu.p_uuid, pu.full_name, pd.opt_gender, foa.option_description AS age,
        ps.location_type, ps.location_value, fos.option_description AS status';
    $sql = 'SELECT %s FROM person_uuid AS pu
        INNER JOIN person_details AS pd ON pd.p_uuid = pu.p_uuid
        INNER JOIN person_status AS ps ON ps.p_uuid = pu.p_uuid
        INNER JOIN resource_to_incident ri ON ri.x_uuid = pu.p_uuid
        LEFT JOIN field_options AS fos ON fos.option_code = ps.opt_status
            AND fos.field_name = \'opt_status\'
        LEFT JOIN field_options AS foa ON foa.option_code = pd.opt_age_group
            AND foa.field_name = \'opt_age_group\'
        WHERE ri.incident_id = \'' . $_SESSION['user_pref_ims_incident_id'] . '\'';
    switch($type) {
        case 'found':
            $sql .= ' AND ps.opt_status IN (\'inj\',\'inh\',\'inp\',\'ali\',\'anp\')';
            break;
        case 'injured':
            $sql .= ' AND ps.opt_status IN (\'inj\',\'inh\',\'inp\')';
            break;
        case 'deceased':
            $sql .= ' AND ps.opt_status = \'dec\'';
            break;
        case 'other':
            $sql .= ' AND ps.opt_status NOT IN (\'ali\',\'anp\',\'inj\',\'inh\',
                \'inp\',\'mis\',\'dec\')';
            break;
        case 'missing':
        default:
            $sql .= ' AND ps.opt_status = \'mis\'';
    }
    $sql .= " ORDER BY pu.p_uuid DESC";
    $rs = $global['db']->SelectLimit(sprintf($sql, $fields), $limit, $offset);
    if ($rs) {
        $arr = $rs->GetAll();
        foreach($arr AS $key => $item) {
            if($item['opt_gender'] == 'mal' || $item['opt_gender'] == 'M') {
                $arr[$key]['opt_gender'] = _('Male');
            } else {
                $arr[$key]['opt_gender'] = _('Female');
            }
            switch($item['location_type']) {
                case 'loc_camp':
                    $rs = $global['db']->Execute('SELECT name FROM camp_general
                        WHERE c_uuid = \'' . $item['location_value'] . '\'');
                    $arr[$key]['location'] = $rs->fields['name'];
                    break;
                case 'loc_org':
                    $rs = $global['db']->Execute('SELECT name FROM org_main
                        WHERE o_uuid = \'' . $item['location_value'] . '\'');
                    $arr[$key]['location'] = $rs->fields['name'];
                    break;
                case 'loc_manually':
                    $rs = $global['db']->Execute('SELECT name FROM location
                        WHERE loc_uuid = \'' . $item['location_value'] . '\'');
                    $arr[$key]['location'] = $rs->fields['name'];
                    break;
                default:
                    $arr[$key]['location'] = '--';
            }
        }
        $_SESSION['result'] = $arr;
    }
    $rs1 = $global['db']->Execute(sprintf($sql, 'COUNT(*) AS count'));
    $count = 0;
    if(!empty($rs1->fields['count'])) {
        $count = $rs1->fields['count'];
    }
    return $count;
}

function shn_mpr_search_result_list($details) {
    if(!empty($details)) {
        echo '<br /><br /><table style="width: 90%; margin: auto;">';
        echo '<tr><td><strong>' . _('Full name') . '</strong></td>
            <td><strong>' . _('Gender') . '</strong></td>
            <td><strong>' . _('Age Group') . '</strong></td>
            <td><strong>' . _('Location') . '</strong></td>
            <td><strong>' . _('Status') . '</strong></td></tr>';
        foreach($details AS $item) {
            echo '<tr><td><a href="index.php?mod=mpr&act=editmp&id=' .
            $item['p_uuid'] . '">' . $item['full_name'] . '</a></td>
            <td>' . $item['opt_gender'] . '</td>
            <td>' . $item['age'] . '</td>
            <td>' . $item['location'] . '</td>
            <td>' . $item['status'] . '</td></tr>';
        }
        echo '</table><br />';
    }
}