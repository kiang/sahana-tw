<?php
/**
 * Search Category
 *
 * PHP version 4 and 5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author       Sanjeewa Jayasinghe <sditfac@opensource.lk>
 * @copyright  Lanka Software Foundation - http://www.opensource.lk
 * @package    module
 * @subpackage cs
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 */
/**
 * This function shows a form to select the category.Submit the result to shn_cs_search_submit() in main.inc.
 * @access private
 * @return void
 */
function shn_cs_search_cat() {
    $option_array = array('none' => _('-----Select Search-----'), 'Item' => _('Item/Catalog'), 'Unit' => _('Unit'), 'Unit_type' => _('Unit Type'));
    $form_opts['name'] = "select_type_form";
    shn_form_fopen('select_type_submit', null, $form_opts);
    shn_form_fsopen(_('Select Search Type'));
    $select_opts = 'onChange="shn_cs_search_cat_js()"';
    shn_form_select($option_array, '', 'search_type', $select_opts, null);
    shn_form_fsclose();
    shn_form_fclose();
    shn_cs_search_cat_js();
}
/**
 * This function Displays the Form for selecting catalog.
 * @access private
 * @return void
 */
function _shn_cs_cat_form() {
    shn_form_fopen('search_item_submit');
    shn_form_fsopen(_('Search Details'));
    shn_form_hidden(array( 'search_type' => trim($_POST['search_type'])  ) );
    shn_form_text(_("Name   : "), 'cat_item_name', 'size="50"', array('req' => 'true', 'help' => _('Enter the Item/catalog to be searched')));
    $options = array('item' => _('Item'), 'catalogue' => _('Catalog'));
    shn_form_radio($options, '', 'item_cat_radio');
    shn_form_fsclose();
    shn_form_submit(_("Search"));
    shn_form_fclose();
}
/**
 * This function shows the result after processing the POST values from shn_cs_search_cat().
 *This function is called by shn_cs_search_submit() in main.inc
 * @access private
 * @return void
 */
function _shn_cs_search_item_cat_submit_result() {
    global $global;
    $db = $global["db"];
    $cat_item_name = trim($_POST['cat_item_name']);
    $item_cat_radio = trim($_POST['item_cat_radio']);
    if($item_cat_radio == 'item') {
        $query = "SELECT name,description FROM ct_catalogue WHERE name like'%" . $cat_item_name . "%' AND final_flag='1'";
        $res = $db->Execute($query);
        $name = $res->fields["name"];
        $description = $res->fields["description"];
    }
    else if ($item_cat_radio == 'catalogue') {
        $query = "SELECT name,description FROM ct_catalogue WHERE name like'%" . $cat_item_name . "%' AND final_flag='0'";
        $res = $db->Execute($query);

        $name = $res->fields["name"];
        $description = $res->fields["description"];
    }
    if ($name == null && $description == null) {
        print "<br /><br /><b>" . _("There is no match found.") . "</b>";
    } else {
?>
    <br><br>
    <div id="result">
    <table>
    <THEAD>
        <TD><strong><?php echo _("Category Name") ?></strong></TD>
        <TD><strong><?php echo _("Description") ?></strong></TD>
    </THEAD>
    <TBODY>

    <?php
        foreach ($res as $key => $value) {
            print"<tr>";
            for ($count = 0;$count < (count($value) / 2);$count++) {
                $item = $value['' . $count . ''];
                print"<td>$item</td>";
            }
            print"</tr>";
        }
        ?>

    </TBODY> 
    </table>
    </div>
    <?php
    }
}
/**
 * This function Displays the Form for selecting Measurement Unit Type.
 * @access private
 * @return void
 */
function _shn_cs_unit_form() {
    global $global;
    $db = $global["db"];
    $option_array = array();
    $query = "SELECT unit_type_uuid,name FROM ct_unit_type ";
    $res = $db->Execute($query);
    $option_array['none'] = _('----Select Unit type----');
    while (!$res == NULL && !$res->EOF) {
        $option_array[$res->fields['unit_type_uuid']] = $res->fields['name'];
        $res->MoveNext();
    }
    $form_opts['name'] = "unit_type";
    shn_form_fopen("search_unit_submit");
    shn_form_fsopen(_('Select Measurement Unit Type'));
    shn_form_hidden(array( 'search_type' => trim($_POST['search_type'])  ) );
    shn_form_select($option_array, '', 'measurement_type', $select_opts, null);
    shn_form_fsclose();
    shn_form_fsopen(_('Search Details'));
    //$extra_opts['req']=true;
    shn_form_text(_("Unit Name   : "),'unit_name','size="50"',array('help'=>_('Enter the Unit name to be searched.')));
    shn_form_fsclose();
    shn_form_submit(_("Search"));
    shn_form_fclose();
}
/**
 * This function shows the result after processing the POST values from _shn_cs_unit_form().
 *This function is called by shn_cs_search_unit_submit() in main.inc
 * @access private
 * @return void
 */
function _shn_cs_search_unit_submit_result() {
    global $global;
    $db = $global["db"];
    $search_name = trim($_POST['unit_name']);
    $unit_type_id = trim($_POST['measurement_type']);
    if ($unit_type_id == 'none' && $search_name == '') {
        $query = "SELECT * FROM ct_unit";
        $res = $db->Execute($query);
    } elseif ($search_name == '') {
        $query = "SELECT * FROM ct_unit WHERE unit_type_uuid='$unit_type_id' ";
        $res = $db->Execute($query);
    } else {
        $query = "SELECT * FROM ct_unit WHERE name='$search_name' ";
        $res = $db->Execute($query);
    }
?>
    <br><br>
    <div id="result">
    <table>
    <THEAD>
        <TD><strong><?php echo _("Unit Type") ?></strong></TD>
        <TD><strong><?php echo _("Unit") ?></strong></TD>
        <TD><strong><?php echo _("Multiplier") ?></strong></TD>
    </THEAD>
    <TBODY>
    <?php
    while (!$res == NULL && !$res->EOF) {
        $unit_name = $res->fields["name"];
        $multiplier = $res->fields["multiplier"];
        $temp = $res->fields["unit_type_uuid"];
        $query = "SELECT name FROM ct_unit_type WHERE unit_type_uuid='$temp' ";
        $res3 = $db->Execute($query);
        $unit_type_name = $res3->fields["name"];
?>
    <tr>
        <td><?php print $unit_type_name; ?></td>
        <td><?php print $unit_name; ?></td>
        <td><?php print $multiplier; ?></td>
    </tr>
    <?php
        $res->MoveNext();
    } //end of while loop
    
?>
    </TBODY>
    </table>
    </div>
    <?php
}
/**
 * This function Displays the Form for selecting Unit type
 * @access private
 * @return void
 */
function _shn_cs_unit_type_form() {
    shn_form_fopen('search_unit_type_submit');
    shn_form_fsopen(_('Search Details'));
    shn_form_hidden(array( 'search_type' => trim($_POST['search_type'])  ) );
    shn_form_text(_("Unit type   : "), 'unit_type_name', 'size="50"', array('req' => 'true', 'help' => _('Enter the unit type to be searched')));
    shn_form_fsclose();
    shn_form_submit(_("Search"));
    shn_form_fclose();
}
/**
 * This function shows the result after processing the POST values from shn_cs_search_cat().
 * This function is called by shn_cs_search_unit_type_submit() in main.inc
 * @access private
 * @return void
 */
function _shn_cs_search_unit_type_submit_result() {
    global $global;
    $db = $global["db"];
    $unit_type_name = trim($_POST['unit_type_name']);
    $query = "SELECT * FROM ct_unit_type WHERE name='$unit_type_name'";
    $res = $db->Execute($query);
    $name = $res->fields["name"];
    $description = $res->fields["description"];
?>
    <br /><br />
    <div id="result">
    <table>
    <THEAD>
        <TD><strong><?php echo _("Unit Type Name") ?></strong></TD>
        <TD><strong><?php echo _("Description") ?></strong></TD>
    </THEAD>
    <TBODY>

    <tr>
        <td><?php print $name; ?></td>
        <td><?php print $description; ?></td>
    </tr>

    </TBODY> 
    </table>
    </div>
	<?php
}
?>
