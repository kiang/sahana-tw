<?php
/**
 * List Injured Persons with paging support
 *
 * PHP version 4 and 5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author Isuru Samaraweera(isuru@opensource.lk)
 * @copyright  Lanka Software Foundation - http://www.opensource.lk
 * @package    module
 * @subpackage mpr
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 */
?>
<div id="home_recent">
    <h1><?php echo _('Injured People'); ?></h1>
    <?php
    global $global;
    global $conf;
    require ($global['approot'] . 'mod/mpr/mpr_lib.inc');
    //$limit = $conf['mod_mpr_pictures_per_page'];
    $limit = 20;
    $type = "injured";
    $offset = isset($_GET['offset']) ? $_GET['offset'] : 0;
    $count = shn_mpr_search_process($type, $limit, $offset);
    if ($count > $limit) {
        $tmp = $count % $limit;
        if ($tmp > 0)
            $no_of_pages_required = floor($count / $limit) + 1;
        else
            $no_of_pages_required = round($count / $limit);
        if ($offset > 0) {
            $previous = $offset - $limit;
            echo '<a href="index.php?mod=mpr&act=list_found&offset=' . $previous .
            '" class="hideprint">' . _('Previous') . '</a>';
        } else {
            $previous = 0;
        }
        for ($i = 0; $i < $no_of_pages_required; $i++) {
            echo ' <a href="index.php?mod=mpr&act=list_found&offset=' . ($limit * $i) .
            '" class="hideprint">' . ($i + 1) . '</a> ';
        }
        if ($count > $offset + $limit) {
            $next = $limit + $offset;
            echo '<a href="index.php?mod=mpr&act=list_found&offset=' . $next .
            '" class="hideprint">' . _('Next') . '</a>';
        } else {
            $next = 0;
        }
        shn_mpr_search_result_list($_SESSION['result']);
        if ($offset > 0) {
            echo '<a href="index.php?mod=mpr&act=list_found&offset=' . $previous .
            '" class="hideprint">' . _('Previous') . '</a>';
        }
        for ($i = 0; $i < $no_of_pages_required; $i++) {
            echo ' <a href="index.php?mod=mpr&act=list_found&offset=' . ($limit * $i) .
            '" class="hideprint">' . ($i + 1) . '</a> ';
        }
        if ($next > 0) {
            echo '<a href="index.php?mod=mpr&act=list_found&offset=' . $next .
            '" class="hideprint">' . _('Next') . '</a>';
        }
    } else {
        shn_mpr_search_result_list($_SESSION['result']);
    }
    echo '<br /><br />';
    echo '<input type="button"  value="' . _('Print Current Page') . '" onClick="self.print()" class="hideprint"/>';
    ?>
</div>