<?php
/**Search functinalities for CR
*
* PHP version 4 and 5
*
* LICENSE: This source file is subject to LGPL license
* that is available through the world-wide-web at the following URI:
* http://www.gnu.org/copyleft/lesser.html
*
* @package    Sahana - http://sahana.sourceforge.net
* @author   Mifan Careem <mifan@opensource.lk>
* @author   Chathra Hendehewa <chathra@opensource.lk>
* @copyright  Lanka Software Foundation - http://www.opensource.lk
*
* @patch author: isuru@opensource.lk
* @patch author:
*/
global $global;
include_once ($global['approot'] . '/inc/lib_form.inc');
include_once ($global['approot'] . '/inc/lib_paging.inc');
/**
 * Search by address
 */
function _shn_cr_srch_rst_address($camp_name, $type = 'all') {
    global $global;
    $db = $global['db'];
    if ($_REQUEST['edt'] == 'true') $edt = true;
    // Specific Type search
    $query = " select cg.c_uuid,cg.name,cg.address,cg.location_id," . " total,capacity from camp_general as cg, camp_reg as cr where cg.c_uuid=cr.c_uuid " . ((null == $camp_name) ? "" : " AND cg.address LIKE '%{$camp_name}%' ");
    //echo "q=".$query;
    // 	$res_cmp =$db->Execute($query);
    
?>
<h1><?php echo _("Search Results") ?></h1>
<?php
    $res_cmp = shn_paging_get_report($query, 10, null, array('post' => true));
?>

<div id="result">
<table>
	<thead>
		<td><?php echo _("Shelter Name") ?></td>
		<td><?php echo _("Location") ?></td>
		<td><?php echo _("Address") ?></td>
		<td><?php echo _("Shelter Type") ?></td>
		<td><?php echo _("Total Count") ?></td>
		<td><?php echo _("Capacity") ?></td>
	</thead>
	<tbody>
	<?php
    while (!$res_cmp->EOF) {
        $location_name = "";
        $loc_uuid = $res_cmp->fields['location_id'];
        if ($loc_uuid != '-1') {
            $sub_query = "SELECT loc.name AS loc_name FROM location AS loc WHERE loc.loc_uuid='" . $loc_uuid . "'";
            $sub_res = $db->Execute($sub_query);
            if (!$sub_res->EOF) {
                $sub_res->MoveFirst();
            }
            $location_name = $sub_res->fields['loc_name'];
        }
?>
		<tr>
			<td><a
				href="index.php?mod=cr&amp;act=ecmp&amp;seq=view&amp;cmp_id=<?php echo $res_cmp->fields['c_uuid'] ?>">
				<?php echo $res_cmp->fields['name'] ?>&nbsp;</a><?php if ($edt) ?> <a
				href="index.php?mod=cr&amp;act=ecmp&amp;seq=disp&amp;cmp_id=<?php echo $res_cmp->fields['c_uuid'] ?>">[<?php echo _("edit") ?>]</a>
			<a
				href="index.php?mod=cr&amp;act=eppl&amp;seq=disp&amp;cmp_id=<?php echo $res_cmp->fields['c_uuid'] ?>">[<?php echo _("edit people count") ?>]</a>

<!-- 			<a -->
<!-- 				href="index.php?mod=cr&act=ebed&seq=disp&cmp_id=<?php echo $res_cmp->fields['name'] ?>">[<?php echo _("ebdcnt") ?>]</a> -->




				<?php; ?></td>
			<td><?php echo $location_name ?></td>
			<td><?php echo $res_cmp->fields['address'] ?></td>
			<td><?php echo _("All") ?></td>
			<td><?php echo $res_cmp->fields['total'] ?></td>
			<td><?php echo $res_cmp->fields['capacity'] ?></td>
		</tr>
		<?php
        $res_cmp->MoveNext();
    }
?>
	</tbody>
</table>
</div>
<?php
}
/**
 * Display Search by name results
 * Does 'Like' search
 * @access private
 * @param mixed $camp_name Camp Name
 * @param mixed $type Camp type or All types
 */
function _shn_cr_srch_rst($camp_name, $type) {
    global $global, $conf;
    $db = $global['db'];
    //$db->debug=true;
    if ($_REQUEST['edt'] == 'true') $edt = true;
    // Specific Type search
    $query = "SELECT cg.c_uuid, cg.name, cg.address, cg.location_id,
        fo.option_description, cr.total, cg.capacity
        FROM camp_general AS cg
        INNER JOIN camp_reg AS cr ON cr.c_uuid = cg.c_uuid
        INNER JOIN field_options as fo ON fo.option_code = cg.opt_camp_type
        AND fo.field_name = 'opt_camp_type'
        WHERE 1";
    if(!empty($type) && $type != 'all') {
        $query .= " AND cg.opt_camp_type = '{$type}'";
    }
    if(!empty($camp_name)) {
        $query .= " AND cg.name LIKE '%{$camp_name}%'";
    }
    
?>
	<h1><?php echo _("Search Results") ?></h1>
	<?php
    $res_cmp = shn_paging_get_report($query, 10, null, array('post' => true));
?>

<div id="result">
<table>
	<thead>
		<td><?php echo _("Shelter Name") ?></td>
		<td><?php echo _("Location") ?></td>
		<td><?php echo _("Address") ?></td>
		<td><?php echo _("Shelter Type") ?></td>
		<td><?php echo _("Total Count") ?></td>
		<td><?php echo _("Capacity") ?></td>
	</thead>
	<tbody>
	<?php
    while (!$res_cmp->EOF) {
        $location_name = "";
        $loc_uuid = $res_cmp->fields['location_id'];
        if ($loc_uuid != '-1') {
            $sub_query = "SELECT loc.name AS loc_name FROM location AS loc WHERE loc.loc_uuid='" . $loc_uuid . "'";
            $sub_res = $db->Execute($sub_query);
            if (!$sub_res->EOF) {
                $sub_res->MoveFirst();
            }
            $location_name = $sub_res->fields['loc_name'];
        }
?>
		<tr>
			<td><a
				href="index.php?mod=cr&act=ecmp&seq=view&cmp_id=<?php echo $res_cmp->fields['c_uuid'] ?>">
				<?php echo $res_cmp->fields['name'] ?>&nbsp;</a><?php if ($edt) ?> <a
				href="index.php?mod=cr&act=ecmp&seq=disp&cmp_id=<?php echo $res_cmp->fields['c_uuid'] ?>">[<?php echo _("edit") ?>]</a>

				<?php //Removing hospital functionality from search
         ?> <!--<a href="index.php?mod=cr&act=eppl&seq=disp&cmp_id=<?php echo $res_cmp->fields['name'] ?>">[<?php echo _("eplcnt") ?>]</a>
<a href="index.php?mod=cr&act=ebed&seq=disp&cmp_id=<?php echo $res_cmp->fields['name'] ?>">[<?php echo _("ebdcnt") ?>]</a>-->




				<?php; ?></td>
			<td><?php echo $location_name ?></td>
			<td><?php echo $res_cmp->fields['address'] ?></td>
			<td><?php /*echo (($type=="all")?"All": "{$res_cmp->fields['option_description']}")*/ ?>
			<?php echo $res_cmp->fields['option_description'] ?></td>
			<td><?php echo $res_cmp->fields['total'] ?></td>
			<td><?php echo $res_cmp->fields['capacity'] ?></td>
		</tr>
		<?php
        $res_cmp->MoveNext();
    }
?>
	</tbody>
</table>
</div>
<?php
}
/**
 * Basic Search form to display search input
 * @access private
 * @return void
 */
function _shn_cr_srch($edit = false) {
    global $conf;
?>
<h1><?php echo sprintf(_("Search for %s by Name"), $conf['mod_cr_alt']); ?></h1>
	<?php
    shn_form_fopen("srch", null, array('req_message' => false));
    shn_form_fsopen(_("Search"));
    shn_form_hidden(array('seq' => 'commit'));
    shn_form_text(_("Name of Shelter"), 'cmp_id', 'size="30"');
    shn_form_opt_select("opt_camp_type", _("Shelter Type"), null, array('all' => true));
    //set edit functionality in result
    if ($edit) shn_form_hidden(array('edt' => 'true'));
    shn_form_fsclose();
    shn_form_submit(_("Search"));
    shn_form_fclose();
}
/**
 * Basic search by address form
 * @access private
 * @return void
 * @param boolean $edit Controls edit functionality in result page
 */
function _shn_cr_srch_address($edit = false) {
    global $conf;
?>
<h1><?php echo _("Search Shelter by Address") ?></h1>
	<?php
    shn_form_fopen("srch_address", null, array('req_message' => false));
    shn_form_fsopen(_("Search"));
    shn_form_hidden(array('seq' => 'commit'));
    shn_form_text(_($conf['mod_cr_alt'] . " Address "), 'cmp_id', 'size="40"');
    if ($edit == 'true') shn_form_hidden(array('edt' => 'true'));
    shn_form_submit(_("Search"));
    shn_form_fclose();
}

include_once $global['approot'] . "/inc/lib_location.inc";
include_once $global['approot'] . "/inc/lib_errors.inc";
function _shn_cr_form_search() {
    global $global;
    global $conf;
    $db = $global["db"];
?>
<h2><?php echo _('Search for shelters by name, type, service and location') ?></h2>
    <div id="formcontainer">
<?php
    shn_form_fopen("search_cr", null, array('req_message' => false));
    shn_form_fsopen(_('Primary Details'));
    $extra_opts['req'] = false;
    shn_form_text(_('Name of Shelter'), 'org_name', 'size="40"', $extra_opts);
    $extra_opts['all'] = true;
    $extra_opts['value'] = 'all';
    shn_form_opt_select("opt_camp_type", _("Shelter Type"), null, $extra_opts);
    shn_form_fsclose();
    shn_form_fsopen(_("Base Location"));
    shn_location();
    shn_form_text(_("Address"), 'loc_txt', 'size="40"', NULL);
    shn_form_label('', _('Please input the name of road to search in address field. If you would like to search by city or area, please choose the location in the fields above.'));
    shn_form_fsclose();
?>
<br />
    <center>
<?php
    //create the submit button
    shn_form_submit(_("Search"));
?>
</center>
    <br />
<?php
    //close the form
    shn_form_fclose();
?>
</div>
<?php
}

function shn_cr_search_process() {
    global $global, $conf;
    include_once $global['approot'] . "/inc/lib_location.inc";
    $edt = false;
    if (isset($_REQUEST['edt']) && $_REQUEST['edt'] == 'true') $edt = true;
    $loc = shn_location_get_form_submit_loc();
    $db = $global['db'];
    // Specific Type search
    $query = "SELECT cg.c_uuid, cg.name, cg.address, cg.location_id,
        fo.option_description, cr.total, cg.capacity
        FROM camp_general AS cg
        INNER JOIN camp_reg AS cr ON cr.c_uuid = cg.c_uuid
        INNER JOIN field_options as fo ON fo.option_code = cg.opt_camp_type
        AND fo.field_name = 'opt_camp_type'
        WHERE 1";

    if(!empty($_POST['opt_camp_type']) && $_POST['opt_camp_type'] != 'all') {
        $query .= " AND cg.opt_camp_type = '{$_POST['opt_camp_type']}'";
    }
    if(!empty($_POST['org_name'])) {
        $query .= " AND cg.name LIKE '%{$_POST['org_name']}%'";
    }
    if(!empty($_POST['loc_txt'])) {
        $query .= " AND cg.address LIKE '%{$_POST['loc_txt']}%'";
    }
    if ($loc != '-1') {
        $loc_arr = array($loc);
        $loc_arr = shn_location_get_descendants($loc, $loc_arr);
        $loc_str = shn_cr_create_database_IN_array($loc_arr);
        $query .= " AND cg.location_id IN {$loc_str}";
    }
?>
	<h1><?php echo _("Search Results") ?></h1>
	<?php
    $res_cmp = shn_paging_get_report($query, 10, null, array('post' => true));
?>

<div id="result">
<table>
	<thead>
		<td><?php echo _("Shelter Name") ?></td>
		<td><?php echo _("Location") ?></td>
		<td><?php echo _("Address") ?></td>
		<td><?php echo _("Shelter Type") ?></td>
		<td><?php echo _("Total Count") ?></td>
		<td><?php echo _("Capacity") ?></td>
	</thead>
	<tbody>
	<?php
    while (!$res_cmp->EOF) {
        $location_name = "";
        if ($res_cmp->fields['location_id'] != '-1') {
            $location_name = _shn_cr_location_string($res_cmp->fields['location_id']);
        } else {
            $location_name = '';
        }
?>
		<tr>
			<td><a href="index.php?mod=cr&act=ecmp&seq=view&cmp_id=<?php echo $res_cmp->fields['c_uuid']; ?>"><?php
                        echo $res_cmp->fields['name'];
                        ?>&nbsp;</a><?php
                        if ($edt): ?><a href="index.php?mod=cr&act=ecmp&seq=disp&cmp_id=<?php echo $res_cmp->fields['c_uuid'] ?>">[<?php echo _("edit") ?>]</a>
				<?php endif; ?></td>
			<td><?php echo $location_name ?></td>
			<td><?php echo $res_cmp->fields['address'] ?></td>
			<td><?php echo $res_cmp->fields['option_description'] ?></td>
			<td><?php echo $res_cmp->fields['total'] ?></td>
			<td><?php echo $res_cmp->fields['capacity'] ?></td>
		</tr>
		<?php
        $res_cmp->MoveNext();
    }
?>
	</tbody>
</table>
</div>
<?php
}

function shn_cr_create_database_IN_array($arr) {
    if ($arr != null) {
        $list = "(";
        $j = 0;
        foreach($arr as $i) {
            $val = "'$i'";
            if ($j == 0) {
                $list = $list . $val;
            } else {
                $list = $list . "," . $val;
            }
            $j++;
        }
        $list = $list . ")";
    }
    return $list;
}

function _shn_cr_location_string($loc) {
    global $global;
    $db = $global["db"];
    $loc_arr = array();
    shn_get_parents($loc, $loc_arr);
    if ($loc_arr[0] == 'unknown') {
        shn_form_label(_("Organization Location"), "Unknown");
    } else {
        $max = count($loc_arr) - 1;
        array_pop($loc_arr);
        $location = '';
        for ($count = 0;$count < $max;$count++) {
            $x = array_pop($loc_arr);
            $q = "select name from location where loc_uuid='{$x}'";
            $res = $db->Execute($q);
            if ($count == 0) {
                $location = $location . $res->fields["name"];
            } else {
                $location = $location . " -->" . $res->fields["name"];
            }
        }
    }
    return $location;
}