<?php
/**
 * This file is to add,edit,delete locations through the admin interface
 *
 * PHP version 4 and 5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @package    Sahana - http://sahana.sourceforge.net
 * @author     Ravindra De Silva
 * @copyright  Lanka Software Foundation - http://www.opensource.lk
 */
global $global;
include_once $global['approot'] . "/inc/handler_form.inc";
include_once $global['approot'] . "/inc/lib_location.inc";
function _shn_admin_location_view_form() {
?>
<h1 align="center"><?php echo _("First select a location to Edit or Delete") ?></h1>

	<?php
    if (!empty($error)) display_errors();
?>
<div id="formcontainer"><?php
    shn_form_fopen("view_loc_cr", '', array('req_message' => false));
    _shn_or_update_loc();
?>
<div id="info"><?php
    // removed multiline due to a get text issue.
    echo _("Select location and then select the level (e.g. Country, State, City),") . _(" the selected location field at the end of the form will be updated") ?>
</div>
<?php
    $select_opts = "onClick='update_loc(this.options[this.selectedIndex].value,this.options[this.selectedIndex].text)'";
    shn_form_hidden(array('loc_sel' => '0'));
    shn_form_fsclose();
    // base location
    shn_form_fsopen(_("Location"));
    $range = shn_get_range();
    shn_location($range, null, null);;
    //shn_form_fsclose();
    
?>

<!-- <fieldset><?php
    //shn_form_opt_select('opt_location_type',_('Location Level'),$select_opts,null,null);
    
?></fieldset> -->
<?php
    /*
    shn_form_fsopen(_("Selected Location for Edit/Delete"));
    $extra_options['value']=_("All Locations");
    $extra_options['size']=60;
    shn_form_text(_("location"),'loc',"size=50 readonly",$extra_options);
    shn_form_fsclose(); */
?> <br />
<center><?php
    shn_form_submit(_("Next"));
    shn_form_fsclose();
?></center>
<br />
<?php
    //close the form
    shn_form_fclose(false);
?></div>
<?php
    // end of form
    
}
function _shn_admin_location_edit_form($loc) {
    global $global;
    $db = $global['db'];
    $q = "SELECT name,description,iso_code FROM location WHERE loc_uuid='{$loc}'";
    $res = $db->Execute($q);
?>
<h1 align="center"><?php echo _("Edit Location") ?></h1>
<div id="note"><?php echo _("Fields marked with * are required (entry is compulsory)") ?>
</div>
	<?php
    if (!empty($error)) display_errors();
?>
<div id="formcontainer"><?php
    $form_opts['name'] = 'view';
    shn_form_fopen("edit_loc_cr", null, $form_opts);
    shn_form_fsopen(_("Location Details"));
    $extra_opts['req'] = true;
    $extra_opts['value'] = ($res->fields["name"] != 'NULL') ? $res->fields["name"] : "";
    shn_form_text(_("Location Name : "), 'loc_name', 'size="50"', $extra_opts);
    $extra_opts['req'] = false;
    $extra_opts['value'] = ($res->fields["description"] != 'NULL') ? $res->fields["description"] : "";
    shn_form_text(_("Description : "), 'desc', 'size="50"', $extra_opts);
    $extra_opts['value'] = ($res->fields["iso_code"] != 'NULL') ? $res->fields["iso_code"] : "";
    shn_form_text(_("ISO code : "), 'iso', 'size="50"', $extra_opts);
    shn_form_fsclose();
    //create the submit button
    $extra_opts['br'] = false;
    $extra_opts['req'] = false;
    shn_form_button(_("Close"), "onClick='change_action(\"close\")'", $extra_opts);
    shn_form_button(_("Save"), "onClick='change_action(\"edit\")'", $extra_opts);
    shn_form_button(_("Delete"), "onClick='change_action(\"del\");'", $extra_opts);
    shn_form_hidden(array('action' => '0'));
    shn_form_hidden(array('loc_id' => $loc));
    _shn_admin_action_change_javascript("action");
    //close the form
    
?> <br />
<?php
    shn_form_fclose(false);
?></div>
<?php
    // end of form
    
}
function _shn_admin_levels_form() {
    global $global;
    include_once ($global['approot'] . '/inc/lib_misc.inc');
?>
<h2><?php echo _("Location Levels") ?></h2>
<div id="info"><?php echo _("The Location you add will be added after the remaining ones") ?>
</div>
	<?php
    if (!empty($error)) display_errors();
?>
<div id="formcontainer"><?php
    $loc_arr = shn_admin_location_level_list();
    $form_opts['name'] = 'add_loc_lvl';
    shn_form_fopen("loc_lvl_cr", null, $form_opts);
    shn_add_remove_javascript("opt_location_type[]", $loc_arr, false);
    shn_form_fsopen(_("Add New Level Information"));
    shn_form_text(_("New Level : "), 'type', 'size="50"', null);
    shn_form_hidden(array("added" => ','));
    shn_form_hidden(array("removed" => ','));
    shn_form_button(_("Add"), "onClick='add_types()'");
    shn_form_fsclose();
?> <?php
    shn_form_fsopen(_("Remove Levels"));
    $select_opts = "multiple='true'";
    $extra_opts["sort"] = true;
    shn_form_opt_multi_select('opt_location_type', '', $select_opts, $extra_opts);
    //_shn_or_display_org_type($error,true);
    shn_form_button(_("Remove"), "onClick='remove_types()'");
    shn_form_fsclose();
?> <br />
<center><?php
    $extra_opts['br'] = false;
    $extra_opts['req'] = false;
    shn_form_button(_("Save"), "onClick='save()'", $extra_opts);
    shn_form_button(_("Reset"), "onClick='change_action(\"reset\")'", $extra_opts);
    shn_form_hidden(array('action' => '0'));
    shn_form_hidden(array('loc_str' => ','));
    shn_reset_form_javascript("add_loc_lvl", "opt_location_type[]");
    _shn_save_form_javascript("add_loc_lvl", "opt_location_type[]", "loc_str");
?></center>
<br />
<?php
    //close the form
    shn_form_fclose();
?></div>
<?php
    // end of form
    
}
/*theres a generic form reset and submit javascript in lib_misc
,but for locations there is the need to fill a string with a values
from a multi select box,therefore following function is there*/
function _shn_save_form_javascript($form, $save_what, $fill) {
?>
<script type="text/javascript">
    	function save()
    	{
    		var tmp_str=",";
    		var x=document.getElementsByName("<?php echo $save_what ?>");
    		for(i=(x[0].options.length); i>0; i--) {
    			tmp_str=tmp_str+x[0].options[(x[0].options.length)-i].text+",";
			}
            
        	var y=document.getElementsByName("<?php echo $fill ?>");
        	y[0].value=tmp_str;
        	document.<?php echo $form ?>.submit();
        	return;	
    	} 
	</script>
	<?php
}
function _shn_adm_loc_lvl_cr() {
    if ("," == $_POST["added"] && "," == $_POST["removed"]) {
        add_error("Please Add or Remove new level first.");
        _shn_admin_levels_form();
    } else {
        global $global;
        include_once $global['approot'] . 'inc/lib_misc.inc';
        $db = $global['db'];
        $add = explode(",", $_POST['loc_str']);
        $duplicate = false;
        $dup_val = "";
        for ($j = 0;$j < count($add);$j++) {
            if (strlen($add[$j]) > 0 && preg_match("/\w+/", $add[$j])) for ($k = $j;$k < count($add);$k++) {
                if ($j != $k) {
                    // check for duplicates
                    $duplicate = ($add[$j] === $add[$k]);
                    if ($duplicate) {
                        $dup_val = $add[$j];
                        break 2;
                    }
                }
            }
        }
        if (!$duplicate) {
            $q = "delete from field_options where field_name='opt_location_type'";
            $res = $db->Execute($q);
            $errors = false;
            $min = 1;
            $max = 0;
            for ($i = 1;$i < count($add) - 1;$i++) {
                $q = "insert into field_options(field_name,option_code,option_description) values('opt_location_type','$i','$add[$i]')";
                $res = $db->Execute($q);
                if ($db->ErrorNo() > 0) {
                    $errors = true;
                    break;
                } else {
                    $max = $i;
                }
            }
            if (!$errors) {
                add_confirmation(_("Location levels were modified successfully."));
                $range = shn_location_get_loc_range();
                if ($max > $range['end']) {
                    add_warning(_("The ending level is set to \"") . "<strong>" . _lc(shn_location_get_level_description($range['end'])) . "</strong>" . _("\". You may need to change it for the location level modification to take effect.") . "<br/><a href=\"index.php?mod=admin&act=loc_range\">" . _("Click Here to change the ending level") . "</a>");
                }
                if ($min < $range['start']) {
                    add_warning(_("The starting level is set to \"") . "<strong>" . _lc(shn_location_get_level_description($range['start'])) . "</strong>" . _("\". You may need to change it for the location level modification to take effect.") . "<br/><a href=\"index.php?mod=admin&act=loc_range\">" . _("Click Here to change the starting level") . "</a>");
                }
            } else {
                add_error(_("Location level modification failed."));
            }
        } else {
            add_error(_("The value ") . $dup_val . _(" is duplicating."));
        }
        _shn_admin_levels_form();
    }
}
function shn_admin_location_level_list() {
    global $global;
    $q = "select * from field_options where field_name='opt_location_type'";
    $res = $global['db']->Execute($q);
    $loc_arr = array();
    while ((!$res == NULL) && (!$res->EOF)) {
        array_push($loc_arr, $res->fields["option_description"]);
        $res->MoveNext();
    }
    return $loc_arr;
}
function _shn_admin_location_range_form() {
    global $global;
    $db = $global["db"];
?>
<h2><?php echo _("Set the Location Range") ?></h2>
<div id="note"><?php echo _("In a Disaster, all the location levels might not be important ,set the range here") ?>
</div>
	<?php
    if (!empty($error)) display_errors();
?>
<div id="formcontainer"><?php
    $range = shn_location_get_loc_range();
    shn_form_fopen("loc_range_cr", NULL, array('req_message' => false));
    shn_location_level_range_form('start', _('Set Starting Level'), $range["start"]);
    shn_location_level_range_form('end', _('Set End Level'), $range["end"]);
?> <br />
<center><?php
    //create the submit button
    shn_form_submit(_("Save"));
?></center>
<br />
<?php
    //close the form
    shn_form_fclose();
?></div>
<?php
    // end of form
    
}
function _shn_admin_loc_range_cr() {
    global $global;
    $db = $global['db'];
    $start = $_POST['start'];
    $end = $_POST['end'];
    if ($start >= $end) {
        add_error(_("Starting level should be a level above the ending level."));
        _shn_admin_location_range_form();
        return;
    }
    $q = "SELECT value FROM config WHERE module_id='admin' AND confkey='loc_range_start'";
    $res = $db->Execute($q);
    if (is_object($res) && $res->fields[0] == NULL) {
        $q = "INSERT INTO config(module_id,confkey,value ) VALUES ( 'admin', 'loc_range_start', '{$start}')";
    } else {
        $q = "UPDATE config SET value={$start} WHERE module_id='admin' AND confkey='loc_range_start'";
    }
    $db->Execute($q);
    $q = "SELECT value FROM config WHERE module_id='admin' AND confkey='loc_range_end'";
    $res = $db->Execute($q);
    if (is_object($res) && $res->fields[0] == NULL) {
        $q = "INSERT INTO config(module_id,confkey,value ) VALUES ( 'admin', 'loc_range_end', '{$end}')";
    } else {
        $q = "UPDATE config SET value={$end} WHERE module_id='admin' AND confkey='loc_range_end'";
    }
    add_confirmation(_("Starting and ending levels were updated successfully."));
    $db->Execute($q);
    _shn_admin_location_range_form();
}
function _shn_admin_location_start_form() {
    global $global;
    $db = $global["db"];
?>
<h2><?php echo _("Set the Default Location")?></h2>
<div id="note"><?php echo _("When a Disaster happens, typically only organizations and people located in a certain region are involved. Therefore, when you select a location it is inefficient to select all locations. For example, if you know the Country, specify it here in this form, and you will only see sub-divisions of that Country. Furthermore, if the Disaster is confined to a smaller geographical area, select the appropriate location to start with - such as the State or Country")?>
</div>
	<?php
    if (!empty($error)) display_errors();
?>
<div id="formcontainer"><?php
    shn_form_fopen("loc_start_cr", null, array('req_message' => false));
    shn_form_fsopen(_("Select the Location"));
    //$range= shn_location_get_loc_range();
    $extra_opts["unknown"] = false;
    shn_location(null, null, null, $extra_opts);
    shn_form_fsclose();
    //shn_form_add_component_list($location_inf,$section=true,$legend='Now Select the Location',$return=false,$default_val_req=$error);
    
?> <br />
<center><?php
    //create the submit button
    shn_form_submit(_("Save"));
?></center>
<br />
<?php
    //close the form
    shn_form_fclose();
?></div>
<?php
    // end of form
    
}
function _shn_admin_loc_start_cr() {
    global $global;
    $db = $global['db'];
    $loc = shn_location_get_form_submit_loc();
    if ($loc == - 1) {
        //$error=true;
        //add_error(SHN_ERR_OR_LOCATION_INCOMPLETE);
        
    } else {
        $q = "select value from config where module_id='admin' and confkey='loc_start'";
        $res = $db->Execute($q);
        if (($res != null) && (!$res->EOF)) {
            $q = "update config set value='{$loc}' where module_id='admin' and confkey='loc_start'";
        } else {
            $q = "insert into  config(module_id,confkey,value) values('admin','loc_start','{$loc}')";
        }
        $res = $db->Execute($q);
        if(!$res){
            add_error(_('Error on updating starting location'));
        } else {
            add_confirmation(_('Starting location updated sucessfully.'));
        }
    }
    _shn_admin_location_start_form();
}
function _shn_admin_location_form() {
    global $global;
    $db = $global["db"];
?>
<h1 align="center"><?php echo _("Add Locations") ?></h1>

	<?php
    if (!empty($error)) display_errors();
?>
<div id="formcontainer"><?php
    $form_opts = array('id' => 'add_loc_cr_submit');
    shn_form_fopen("add_loc_cr", null, $form_opts);
    $sql = 'SELECT COUNT(*) FROM location';
    $res = $db->Execute($sql);
    $loc_count = $res->fields[0];
    if ($loc_count > 0) {
        shn_form_fsopen(_("Now Select the parent Location"));
?>
<div class="info"><?php echo _("Select the parent or super region (e.g. Country, State, City) of the location you have typed above. If no parent region is given, the location being added will become a top most region (usually a Country)");?></div>
<br />
	<?php
        $range = shn_get_range();
        $range['end']-= 1;
        shn_location($range, null, null, array("unknown" => false));
        shn_form_fsclose();
    }
    shn_form_fsopen(_("New Location Details"));
    $extra_opts['req'] = true;
    shn_form_text(_("Location Name : "), 'loc_name', 'size="50"', $extra_opts);
    $extra_opts['req'] = false;
    shn_form_text(_("Description : "), 'desc', 'size="50"', $extra_opts);
    shn_form_text(_("ISO code : "), 'iso', 'size="50"', $extra_opts);
    shn_form_fsclose();
    /*
    shn_form_fsopen(_("Location Type (level)"));
    $extra_opts["req"]=true;
    $extra_opts["sort"]="option_code";
    shn_form_opt_select('opt_location_type','',$select_opts,$extra_opts);
    shn_form_fsclose();
    */
?> <input type='hidden' id='loc_sub_ans' name='loc_sub_ans' /> <br />
<center><?php
    shn_form_submit(_("Submit"));
?></center>
<br />
<?php
    //close the form
    shn_form_fclose(false);
?></div>
<?php
    // end of form
    
}
function _shn_or_update_loc() {
?>
<script type="text/javascript">

    	function update_loc(old_action,desc)
    	{
    		action='loc_select_'+old_action
        	var x=document.getElementsByName(action);
        	var loc_val=x[0].options[x[0].selectedIndex].value;
        	var loc_desc=x[0].options[x[0].selectedIndex].text;
        	var x=document.getElementsByName('loc_sel');
        	x[0].value=loc_val;
        	var loc_desc=desc+"->"+loc_desc;
        	var x=document.getElementsByName('loc');
        	x[0].value=loc_desc;
        	return;
    	}
	</script>
	<?php
}
function _shn_admin_action_change_javascript($change) {
?>
<script type="text/javascript">
    	function change_action(action)
    	{
    		if(action=='del'){
    			var loc=document.getElementById("loc_name").value;
    			var answer=confirm('Are you sure that you want to delete '+loc+' and all its sub-locations?');
    			if(answer){
    				var x=document.getElementsByName("<?php echo $change ?>");
         	        x[0].value=action;
         	        document.view.submit();
         	        return;
    			}
    			
    		}
    		else{
        	var x=document.getElementsByName("<?php echo $change ?>");
         	x[0].value=action;
         	document.view.submit();
         	return;
    		}
    	}
	</script>
	<?php
}
function _shn_admin_loc_javascript($name, $last) {
?>
<script type="text/javascript">
<?php if ($last == NULL) {
        $last = 0;
    }
    echo "var last=" . $last . ";" ?>;
 		// sort function - ascending (case-insensitive)
        function sortFuncAsc(record1, record2)
        {
            var value1 = record1.optValue.toLowerCase();
            var value2 = record2.optValue.toLowerCase();
            if (value1 > value2) return(1);
            if (value1 < value2) return(-1);
            return(0);
        }

        // sort function - descending (case-insensitive)
        function sortFuncDesc(record1, record2) 
        {
            var value1 = record1.optValue.toLowerCase();
            var value2 = record2.optValue.toLowerCase();
            if (value1 > value2) return(-1);
            if (value1 < value2) return(1);
            return(0);
        }

        function sortSelect(selectToSort, ascendingOrder) 
        {
            if(arguments.length == 1) ascendingOrder = true;    // default to ascending sort

            // copy options into an array
            var myOptions = [];
            for (var loop=0; loop<selectToSort.options.length; loop++) {
                myOptions[loop] = { optText:selectToSort.options[loop].text, optValue:selectToSort.options[loop].value };
            }

            // sort array
            if(ascendingOrder) {
                myOptions.sort(sortFuncAsc);
            }else {
                myOptions.sort(sortFuncDesc);
            }

            // copy sorted options from array back to select box
            selectToSort.options.length = 0;
            for(var loop=0; loop<myOptions.length; loop++) {
                var optObj = document.createElement('option');
                optObj.text = myOptions[loop].optText;
                optObj.value = myOptions[loop].optValue;
                selectToSort.options.add(optObj);
            }
        }

        function add_types()
        {
        	last=last+1;
            var y=document.getElementsByName("type");
            var add=document.getElementsByName("added");
            var remove=document.getElementsByName("removed");
            
            if(last>1) {
            	var exist=search(add[0].value,last,true,y[0].value);
            	if(exist) {
                	alert("The Type Exists,you just added it");
                	return;
            	}
            }
            var x=document.getElementsByName("<?php echo $name ?>");
            if(last>0) {
            	exist=search_select_box(x[0],last,true,y[0].value);
            	if(exist) {
                	alert("The Type Exists in the DataBase");
                	return;
            	}
            	exist=search(remove[0].value,last,true,y[0].value);
            	if(exist) {
                	remove[0]=del(remove[0].value,last);
                return;
            	}
            }
            opt = document.createElement("option") ;
            opt.text = y[0].value ;
            opt.value = last ;
            var k=x[0].options.length;
            x[0].options[k]=opt;
            sortSelect(x[0], true) ;
            add[0].value= add[0].value+":"+last+"|"+y[0].value;
            y[0].value=null;
            //z[0].value=null
        }


        function remove_types()
        {
            var x=document.getElementsByName("<?php echo $name ?>");
            removeSelectedOptions(x[0]);
            sortSelect(x[0], true) ;
        }

        function hasOptions(obj) 
        {
    	    if (obj!=null && obj.options!=null) { return true; }
	            return false;
	    }
	
        function removeSelectedOptions(from) 
        { 
	        if (!hasOptions(from)) { return; }
	        if (from.type=="select-one") {
		        from.options[from.selectedIndex] = null;
		    }else {
		        var add=document.getElementsByName("added");
                var remove=document.getElementsByName("removed");
                for (var i=(from.options.length-1); i>=0; i--) { 
        			var o=from.options[i]; 
			        if (o.selected) { 
					    var exist=search(add[0].value,o.value,false);
            			if(exist) {
					        add[0].value=del(add[0].value,o.value);
                        }else {
                         	remove[0].value= remove[0].value+":"+o.value+"|"+o.text;
					    }
				        from.options[i] = null; 
				    }
            	}
            }
             	from.selectedIndex = -1; 
	    } 

        function search(arr,value,both,desc)
        {
            if (window.RegExp) {
                var re = new RegExp(value);
                var temp = new Array(); 
                temp = arr.split(':');
                if (temp.length==1){
                    return false;
                }
                for (var i=0; i<temp.length; i++) {
                    var options = new Array(); 
                    options= temp[i].split('|');
                    var re = new RegExp(value);
                    if (re.test(options[0])) {
                        return true;
                    }
				    if(both) {
		                re = new RegExp(desc);
                        if (re.test(options[1])) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        
        function search_select_box(obj,value,both,desc) 
        {
	        if (window.RegExp) {
        		if (!hasOptions(obj)) { return false; }
		        for (var i=0; i<obj.options.length; i++) {
		            var re = new RegExp(value);
                    if (re.test(obj.options[i].value)) {
                        return true;
                    }
				    if(both) {
		                re = new RegExp(desc);
                        if (re.test(obj.options[i].text)) {
                            return true;
                        }
		            }
                }
	        }
            return false;
        }
        
        
        function del(from,what)
        {
            var temp = new Array();
            temp = from.split(':');
            from=null;
            if (temp.length==1) {
                return false;
            }
            for (var i=1; i<temp.length; i++) {
                var options = new Array(); 
                options= temp[i].split('|');
                if(options[0]!=what) {
                    
                    from= from+":"+options[0]+"|"+options[1];
                }
            }
            
            return from;
        }
	
</script>
		<?php
}
?>