
<?php
/* $Id: home.inc,v 1.18.12.5 2008/05/12 03:02:35 ishanul Exp $ */
/** 
 * Home Page of the Missing Person Registry
 *
 * PHP version 4 and 5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author	   Janaka Wickramasinghe <janaka@opensource.lk>
 * @copyright  Lanka Software Foundation - http://www.opensource.lk
 * @package    module
 * @subpackage mpr
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 */
?>
<div id="home">
<h2><?php echo _('Missing Person Registry'); ?></h2>
<?php
global $global;
if (file_exists($global['approot'] . '/www/tmp/mprfound.png')) {
?>
<img src="tmp/mprfound.png"/>
<img src="tmp/mprfound.png"/>
<?php
}
?>
<p>
<?php echo _('The Missing person registry is an online bulletin board of missing and found people.
It captures information about the people missing and found, as well as information of the person seeking them, increasing the chances
of people finding each other. For example if two members of the a family unit is
looking for the head of the family, we can use this data at least to connect those two
family members.'); ?>

</p>

<br></br>
    <div><h3><b><em><?=(_("Features Include:")) ?></em></b></h3></div>
<br></br>


<ul>
    <li><?php echo _('Submitting information about the individual such as identification numbers, physical appearance, last seen location, status, etc'); ?></li>
    <li><?php echo _('\'Sounds-like\' name search allowing search even the spelling of the name is not known exactly'); ?></li>
    <li><?php echo _('Facilitate uploading of missing person\'s photograph'); ?></li>
    <li><?php echo _('Grouping by \'Family Unit\' or other group category'); ?></li>
</ul>

<strong><?php echo _("Description of Actions:"); ?></strong><br/>
    <ul>
        <li><a href="index.php?mod=mpr&act=search&type=all"><b><?php echo _('Search for a person'); ?> </b></a><br/><?php echo _('Search to see whether a person is lost contact with people known to him or her'); ?></li>
        <li><a href="index.php?mod=mpr&act=search&type=missing"><b><?php echo _('Report a Missing Person'); ?> </b> </a><br/><?php echo _('Report details about a missing person'); ?></li>
        <li><a href="index.php?mod=mpr&act=search&type=missing"><b><?php echo _('Edit missing person'); ?></b></a><br/><?php echo _('Update the details of missing person entered to the database previously'); ?></li>
        <li><a href="index.php?mod=mpr&act=search&type=found"><b><?php echo _('Report a Found Person'); ?></b></a><br/><?php echo _('Report information if the displaced person is found'); ?>
        <li><a href="index.php?mod=mpr&act=list_missing"><b><?php echo _('List missing people'); ?></b></a><br/><?php echo _('List all the missing person information available in the database'); ?></li>
        <li><a href="index.php?mod=mpr&act=list_found"><b><?php echo _('List found people'); ?></b></a><br/><?php echo _('Lias all the information of found displaced people'); ?></li>
        <li><a href="index.php?mod=mpr&act=list_injured"><b><?php echo _('List injured people'); ?></b></a><br/><?php echo _('Lias all the information of found injured people'); ?></li>
        <li><a href="index.php?mod=mpr&act=list_deceased"><b><?php echo _('List deceased people'); ?></b></a><br/><?php echo _('Lias all the information of found deceased people'); ?></li>
</ul>
<br/>


</div>
<?php
/*
*  Recent Found People people Top 6
*/
global $global;
require ($global['approot'] . 'mod/mpr/mpr_lib.inc');
?>
<div id="home_recent">
<h1><?php echo _('Latest updates on Found People'); ?></h1>
<?php
$limit = 6;
$type = "found";
$rs = shn_mpr_home_show_mpr($type, $limit, 0);
shn_mpr_search_show_verticle($_SESSION['result']);
?>
<?php
/*
*  Recent Missing People Top 6
*/
?>
<h1><?php echo _('Latest updates on Missing People'); ?></h1>
<?php
$limit = 6;
$type = "missing";
shn_mpr_home_show_mpr($type, $limit, 0);
shn_mpr_search_show_verticle($_SESSION['result']);
?>
</div>
<?php
/**
 * Shows the latest updates on missing and found perople
 *
 * @param string $type
 * @param int $limit
 * @access public
 * @return void
 */
?>

