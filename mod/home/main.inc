<?PHP

include_once $global['approot']."/inc/lib_menu.inc";

function shn_text_home_help()
{
    if(empty($_GET['help_id'])){
        echo "var help_arr = new Array(".count($_SESSION['form_help']).");\n";
        if(isset($_SESSION['form_help']))
            foreach($_SESSION['form_help'] as $help_id => $value){
                $clean_value = preg_replace(array('@([\r\n])[\s]+@','@"@'), array(' ',"'"),$value);
                echo "help_arr[".$help_id."] = \"$clean_value\";\n";
            }
    }else{
        echo $_SESSION['form_help'][$_GET['help_id']];
    }
}


function shn_home_default()
{
    global $conf;
    
    ?>

    <div id="home">

    <?php echo "<h2>"._("Welcome to the Sahana FOSS Disaster Management System")."</h2>"?>
<!--        <img src="theme/default/img/home_pic1.png" align="left" /> -->
    <p><?php echo _("Sahana is a collection of web based disaster management applications that provides solutions to large-scale humanitarian coordination and collaboration in disaster situation and its aftermath. Sahana consists of several modules for following functionalities:"); ?></p>
        <ul>
    
<?php
        _shn_home_module_info('gis',_("Allows you to locate activities on a map providing current situation awareness."));
        _shn_home_module_info('mpr',_("Helps to report and search missing person."));
        _shn_home_module_info('dvr',_("Traces internally displaced people (IDPs) and their needs."));
    _shn_home_module_info('or',_("Lists 'who is doing what & where'. Allows relief agencies to self organize the activities rendering fine coordination among them."));
    _shn_home_module_info('rms',_("Tracks requests for aid and matches them against donars who have pledged aid."));
    _shn_home_module_info('cr',_("Tracks the location, distibution, capacity and breakdown of victims in shelter."));
    _shn_home_module_info('ims',_("Effectively and efficiently manage relief aid, enables  transfer of inventory items to different inventories and notify when items are required to refill."));
    _shn_home_module_info('msg',_("Allows communication by email and SMS text messaging to groups."));
    _shn_home_module_info('vm',_("Allows managing volunteers by capturing their skills, availability and allocation."));
    _shn_home_module_info('cs',_("Captures information on different catalogues and measurement units. These information are being used in systems such as Inventory Management System and Request Management System."));
    _shn_home_module_info('rs',_("Allows system to generate reports based on sahana database. It aggregates all the module reports in one place for making easier the user to view, search reports and charts."));
    _shn_home_module_info('sync',_("Allows data exchange between instances of Sahana by synchronization."));
    _shn_home_module_info('pref',_("Allows you to set any preferred configuration. Which will help you to customize Sahana to your preference."));
    _shn_home_module_info('admin',_("Allows you to configure and customize Sahana based on your requirements."));
?>

        </ul>
    <p>
    <?php
        echo _("For more detail on Sahana system, see ") .
                          '<a href="http://www.sahana.lk">' .
                          _("Sahana Website").'</a><br/>';
        echo _("For live help from the Sahana community on using this application, go to ").
              '<a href="http://www.sahana.lk/chat">'._("Sahana Community Chat")."</a><br/>";
        
        echo _("To access Sahana documentation, go to ").
              '<a href="http://wiki.sahana.lk/doku.php?id=doc:nwhome">'._("Sahana Wiki")."</a><br/>";

        if ($conf['root_name'] != '') {
            echo '<br/>'._("Administrator: ").$conf['root_name'];
        }
        if ($conf['root_email'] != '') {
            echo '.<br/>'._("Email")." : ".$conf['root_email'];
        }
        if ($conf['root_tel'] != '') {
            echo '<br/>'._("Telephone")." : ".$conf['root_tel'];
        }
    ?>
        </p>
    </div>

<?php
}
function _shn_home_module_info($module, $description) 
{
    global $conf;

    if ($conf['mod_'.$module.'_enabled']) {
        echo "<li><a href=\"index.php?mod=$module\" ><strong>".$conf['mod_'.$module.'_name'].'</strong></a> - ';
        echo _($description)."</li>\n";
    }

}

/**
 * Show a welcome page for first time users.
 *
 */
function shn_home_welcome(){
	global $global;
	echo _("<h2>Welcome to the Sahana FOSS Disaster Management System</h2>")
	?>
<h4><?php echo _("Congratulations !. You have successfully installed Sahana.")?></h4>
<p><?php echo _("You may need to customize sahana according to your requirements.") ?></p>
	<?php
	if(shn_acl_is_enabled()){
		
		if($_SESSION['logged_in']===true && $_SESSION['user']=="admin" && $_SESSION['user_id']=="1"){
			shn_admin_default();
			// unset it the first time flag so that this never appears again
			$_SESSION['first_time_run']=false;
		}else{
		
			$_SESSION['last_module']="admin";
			$_SESSION['last_action']="default";
		?>
<div class="message information"><strong> <?php
echo _("Please login by entering the administrator username and password in the respective text boxes on the left side
		panel to proceed customizing sahana.")
?> </strong></div>
<?php
		}
	}else{
		shn_admin_default();
		// unset it the first time flag so that this never appears again
		$_SESSION['first_time_run']=false;
	}
}

function shn_home_about(){
	global $global,$conf;
	echo _('<h2>About Sahana</h2>');
	if(file_exists($global['approot']."about.inc")){
		include_once $global['approot']."about.inc";
		
		global $about;		
		if(function_exists("apache_get_version")){
			$about['apache_version'] = apache_get_version();
		}else{
			$about['apache_version'] = _("Unable to determine Apache HTTPD version.");
		}
		
		$about['php_version'] = phpversion();
		
		$sql = "SHOW VARIABLES LIKE \"character_set_database\";";
		$res = $global['db']->Execute($sql);
		$about['database_charset']=$res->fields['Value'];

        $sql = "SHOW VARIABLES LIKE \"collation_database\";";
        $res = $global['db']->Execute($sql);
        $about['database_collation']=$res->fields['Value'];		
		
		$sql = "SELECT VERSION()";
		$res = $global['db']->Execute($sql);
		$about['mysql_version'] = $res->Fields(0);
		
		?>
		<table border="0" class="about-sahana">
		  <tr>
		      <th colspan="2" valign="center"><?php echo 'System Information' ?></td>
		  </tr>
		  <tr>
		      <td width="40%"><?php echo _("Sahana Version") ?></td>
		      <td><?php echo $about['version']?></td>
		  </tr>
		  <tr>
              <td><?php echo _("Release") ?></td>
              <td><?php echo $about['release'] ?></td>
          </tr>
          <tr>
              <td><?php echo _("Release Information")?></td>
              <td><?php echo $about['release_info'] ?></td>
          </tr>
          <tr>
              <td><?php echo _("Release Date")?></td>
              <td><?php echo $about['release_date'] ?></td>
          </tr>
          <tr>
            <th colspan="2" valign="center"><?php echo 'Database Information' ?></th>
          </tr>
          <tr>
              <td><?php echo _("Database Name")?></td>
              <td><?php echo $conf['db_name'] ?></td>
          </tr>
          <tr>
              <td><?php echo _("Database Character Set")?></td>
              <td><?php echo $about['database_charset'] ?></td>
          </tr>
          <tr>
              <td><?php echo _("Database Collation")?></td>
              <td><?php echo $about['database_collation'] ?></td>
          </tr>
          <tr>
              <th colspan="2" valign="center"><?php echo 'Environment Information' ?></td>
          </tr>
          <tr>
              <td><?php echo _("Apache HTTPD Version")?></td>
              <td><?php echo $about['apache_version'] ?></td>
          </tr>
          <tr>
              <td><?php echo _("PHP Version")?></td>
              <td><?php echo $about['php_version'] ?></td>
          </tr>
          <tr>
              <td><?php echo _("MySQL Version")?></td>
              <td><?php echo $about['mysql_version'] ?></td>
          </tr>
          <tr>
              <th colspan="2" valign="center"><?php echo 'System Administrator Information' ?></td>
          </tr>
          <tr>
              <td><?php echo _("Administrator Name")?></td>
              <td><?php echo ((($conf['root_name']!=null) && (strlen(trim($conf['root_name']))>0))?($conf['root_name']):_('Information not available')) ?></td>
          </tr>
          <tr>
              <td><?php echo _("Email")?></td>
              <td><?php echo ((($conf['root_email']!=null) && (strlen(trim($conf['root_email']))>0))?($conf['root_email']):_('Information not available')) ?></td>
          </tr>
          <tr>
              <td><?php echo _("Telephone")?></td>
              <td><?php echo ((($conf['root_tel']!=null) && (strlen(trim($conf['root_tel']))>0))?($conf['root_tel']):_('Information not available')) ?></td>
          </tr>          
		</table>
		<br/>
		<hr/>
		
		<?php
		  $readme = file_get_contents($global['approot']."README");
		?>
		      <script type="text/javascript">
		          var flag=true;
		          function showReadme(){
		              var readmeBlock = document.getElementById('readme');
                      if(readmeBlock){
		                  if(flag==true){
		                      readmeBlock.style.display='block';
		                      flag=false;
		                  }else{
		                      readmeBlock.style.display='none';
		                      flag=true;
		                  }
		              }
		          }
		      </script>
		      <a href="#" onclick="showReadme()"><?php echo _("Show/Hide Readme") ?></a>
		      <div id="readme" class="hide">
		          <pre>
		          <?php echo $readme; ?>
		          </pre>
		          <br/>
		          <a href="#" onclick="showReadme()"><?php echo _("Hide Readme") ?></a>
		      </div>  
		      
		      <br/>
		      <a href="http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html" target="_blank"><?php echo _("View License") ?></a>
		<?php
		
		return;
	}else{
		echo _("About page information cannot be generated.");
		return;
	}
}

function shn_home_mainmenu(){
	global $global,$conf;
	shn_mod_menuopen(_($conf['mod_home_name']));
	shn_mod_menuitem('about',_('About Sahana'),'home');
	shn_mod_menuclose();
	include $global['approot']."/inc/handler_mainmenu.inc";
}

?>
