<?php
/**
 * $id$
 * Google Maps plugin for Sahana GIS
 *
 * PHP version 4 and 5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * Sahana - http://sahana.sourceforge.net
 * @author   Mifan Careem <mifan@opensource.lk>
 * @copyright  Lanka Software Foundation - http://www.opensource.lk
 * @package    module
 * @subpackage google_maps
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 */
/**
 * show basic map
 * called by shn_gis_map in gis api
 */
function show_map($opt = null) {
    global $global;
    global $conf;
    $key = $conf['mod_gis_google_key'];
?>
	<script src="http://maps.google.com/maps?file=api&v=2&key=<?php echo $key ?>" type="text/javascript"></script>
	<script type="text/javascript">
	  
	//<![CDATA[
        var onloadfunc = window.onload;
	window.onload=function show_map_gms()
	{
			
		//alert('map');
	
	       var map = new GMap2(document.getElementById("map"));
				 map.addControl(new GLargeMapControl());
	       //map.addControl(new GSmallMapControl());
				 map.addControl(new GMapTypeControl());//satellite image switch
				 
				map.setCenter(new GLatLng(<?=$conf['mod_gis_center_y'] != '' ? $conf['mod_gis_center_y'] : 8.5 ?>,<?=$conf['mod_gis_center_x'] != '' ? $conf['mod_gis_center_x'] : 79.5 ?>), <?=$conf['mod_gis_zoom'] ?>,G_SATELLITE_MAP);
		onloadfunc();
	}		
//]]>	
</script>
	<div id="map" style="width: <?=$conf['mod_gis_width'] ?>px; height: <?=$conf['mod_gis_height'] ?>px"></div>
<?
}
/**
 * show map with marker addition event
 * ability to enter markers on map
 * called by shn_gis_add_marker_map_form in gis api
 */
function show_add_marker_map($name, $extra_opts = null) {
    global $global;
    global $conf;
    $db = $global['db'];
    $key = $conf['mod_gis_google_key'];
?>
        <div id="map" style="width: <?=$conf['mod_gis_width'] ?>px; height: <?=$conf['mod_gis_height'] ?>px"></div>
	<script src="http://maps.google.com/maps?file=api&v=2&key=<?php echo $key ?>" type="text/javascript"></script>
	<script type="text/javascript">
	//<![CDATA[
        $(function() {
            var map = new GMap2($('#map').get(0));
            var gpsTextTrigger = false;
            <?php
            if(isset($extra_opts['marker']) && $extra_opts['marker'] === 'single') {
                echo 'var markerSingle = true;';
            } else {
                echo 'var markerSingle = false;';
            }
            ?>
            if($('input[name=gps_x]').length == 1) {
                gpsTextTrigger = true;
            }
            map.addControl(new GLargeMapControl());
            map.addControl(new GMapTypeControl());//satellite image switch
            map.setCenter(
                new GLatLng(<?php echo $conf['mod_gis_center_y']; ?>,<?php echo $conf['mod_gis_center_x']; ?>),
                <?php echo $conf['mod_gis_zoom']; ?>,G_SATELLITE_MAP);
            GEvent.addListener(map, 'click', function(overlay, point) {
                if (overlay) {
                    if(!markerSingle) {
                        map.removeOverlay(overlay);
                    }
                } else {
                    if(markerSingle) {
                        map.clearOverlays();
                    }
                    map.addOverlay(new GMarker(point));
                    // store x,y coords in hidden variables named loc_x, loc_y
                    // must be set via calling page
                    if(gpsTextTrigger) {
                        $('input[name=gps_x]').val(point.lng());
                        $('input[name=gps_y]').val(point.lat());
                    } else {
                        $('input[name=loc_x]').val(point.lng());
                        $('input[name=loc_y]').val(point.lat());
                    }
                }
            })
            if(gpsTextTrigger) {
                $('input[name=gps_x],input[name=gps_y]').change(function() {
                    var textPoint = new GLatLng($('input[name=gps_y]').val(), $('input[name=gps_x]').val());
                    if(markerSingle) {
                        map.clearOverlays();
                    }
                    map.addOverlay(new GMarker(textPoint));
                    map.setCenter(textPoint,<?php echo $conf['mod_gis_zoom']; ?>,G_SATELLITE_MAP);
                });
                if($('input[name=gps_x]').val() != 0) {
                    $('input[name=gps_x]').trigger('change');
                }
            }
        })
	//]]>	
	
	</script>
<?php
}
/**
 * show map with markers
 * called by shn_gis_map_with_markers in gis api
 *
 */
function show_map_with_markers($array) {
    global $global;
    global $conf;
    $db = $global['db'];
    $key = $conf['mod_gis_google_key'];
?>
	<script src="http://maps.google.com/maps?file=api&v=2&key=<?php echo $key ?>" type="text/javascript"></script>
	<script type="text/javascript">
	  
	//<![CDATA[
        var onloadfunc = window.onload;
        var bounds = new GLatLngBounds();
	window.onload=function show_map_markers_js()
	{
            if(GBrowserIsCompatible()) {
            function createMarker(point,html) {
                var marker = new GMarker(point);
                GEvent.addListener(marker, "click", function() {
                    marker.openInfoWindowHtml(html);
                });
                return marker;
            }

            var map = new GMap2(document.getElementById("map"));
            map.addControl(new GLargeMapControl());
            //map.addControl(new GSmallMapControl());
            map.addControl(new GMapTypeControl());//satellite image switch
            var initialPoint = new GLatLng(<?=$conf['mod_gis_center_y'] ?>,<?=$conf['mod_gis_center_x'] ?>);
            bounds.extend(initialPoint);
            map.setCenter(initialPoint, <?=$conf['mod_gis_zoom'] ?>,G_SATELLITE_MAP);
            var point;
            var marker;
<?php
    $pre_url = "index.php?";
    for ($i = 0;$i < sizeof($array);$i++) {
        $lat = $array[$i]["lat"];
        $lon = $array[$i]["lon"];
        $name = $array[$i]["name"];
        $url = $pre_url . $array[$i]["url"];
?>	
            point = new GLatLng(<?php echo $lon; ?>,<?php echo $lat; ?>);
            bounds.extend(point);
            marker = createMarker(point,
            '<div id="wiki_info"><strong><?php echo _lc(str_replace("'", "\'", "$name")); ?></strong><br><a href="<?php echo $url; ?>">View</a></div>');
            map.addOverlay(marker);
<?php
    }
?>
            map.setCenter(bounds.getCenter(), map.getBoundsZoomLevel(bounds));
      	}
        else {
      			alert("Incompatible");
      			}      			
		onloadfunc();
	}		
	//]]>	
	
	</script>
<div id="map" style="width: <?=$conf['mod_gis_width'] ?>px; height: <?=$conf['mod_gis_height'] ?>px"></div>
<?php
}
/**
 * show map with wiki style markers
 * called by shn_gis_map_with_wiki_markers in gis api
 */
function show_map_with_wiki_marker($array) {
    global $global, $conf;
    $db = $global['db'];
    $key = $conf['mod_gis_google_key'];
?>
	<script src="http://maps.google.com/maps?file=api&v=2&key=<?php echo $key ?>" type="text/javascript"></script>
	<script type="text/javascript">
	  
	//<![CDATA[
        var onloadfunc = window.onload;
	window.onload=function show_map_wiki_js()
	{
			if(GBrowserIsCompatible())
			{
				function createMarker(point,html) {
        			var marker = new GMarker(point);
        			GEvent.addListener(marker, "click", function() {
          				marker.openInfoWindowHtml(html);
        			});
        		return marker;
      			}
      			
      			var map = new GMap2(document.getElementById("map"));
      			map.addControl(new GLargeMapControl());
	       		//map.addControl(new GSmallMapControl());
				map.addControl(new GMapTypeControl());//satellite image switch
				
				map.setCenter(new GLatLng(<?=$conf['mod_gis_center_y'] ?>,<?=$conf['mod_gis_center_x'] ?>), <?=$conf['mod_gis_zoom'] ?>,G_SATELLITE_MAP);
<?php
    for ($i = 0;$i < sizeof($array);$i++) {
        $lat = $array[$i]["lat"];
        $lon = $array[$i]["lon"];
        $name = $array[$i]["name"];
        $desc = $array[$i]["desc"];
        $url = $array[$i]["url"];
        if (!(($array[$i]["date"]) == "0000-00-00 00:00:00")) {
            $date = _('Date: ') . date('l dS \of F Y', strtotime($array[$i]["date"]));
        } else {
            $date = "";
        }
        $author = ($array[$i]["author"] != "") ? $array[$i]["author"] : _("anonymous");
        $edit = $array[$i]["edit"];
        if ($array[$i]["image"] != null) {
            $image = $array[$i]["image"];
            //$image="{<img src='{$image}' width='100' height='100' >}";
            $image = ($image ? "<img src=$image width=100 height=100 ><br>" : '');
        } else $image = '';
        //$edit="help";
        
?>	
					var point = new GLatLng(<?=$lon
?>,<?=$lat
?>);
      				var marker = createMarker(point,'<div id="wiki_info"><strong><?=_lc($name)?></strong><br><?=_lc($desc)?><br><?=$image?><? echo ( trim($url)!='' ? '<a href="'.$url.'" target="_blank">'.$url.'</a>':'') ?><br><?=$date?><br><strong>Author:</strong><?=$author?><br><?=$edit?></div>');
      				map.addOverlay(marker);
<?php
    }
    $image = null;
?>
      		}
      		else{
      			alert("Incompatible");
      			}      			
		onloadfunc();
	}		
	//]]>	
	
	</script>
<div id="map" style="width: <?=$conf['mod_gis_width'] ?>px; height: <?=$conf['mod_gis_height'] ?>px"></div>
<?php
}
/**
 * Map with custom markers
 * called by shn_gis_density_map in gis api
 /**
 * map with custom markers
 * called by shn_gis_density_map in gis api
 */
function show_map_with_custom_markers($array) {
    global $global;
    global $conf;
    $db = $global['db'];
    $key = $conf['mod_gis_google_key'];
?>
	<script src="http://maps.google.com/maps?file=api&v=2&key=<?php echo $key ?>" type="text/javascript"></script>
	<script type="text/javascript">
	  
	//<![CDATA[
        var onloadfunc = window.onload;
	window.onload=function show_map_markers_js()
	{
			if(GBrowserIsCompatible())
			{
				var icon = new GIcon();
				//icon.image = <?=$global['approot'] . $conf['mod_gis_marker_folder'] . "marker_r1.png" ?>;
				//icon.shadow = "http://labs.google.com/ridefinder/images/mm_20_shadow.png";
				icon.iconSize = new GSize(20, 34);
				//icon.shadowSize = new GSize(22, 20);
				icon.iconAnchor = new GPoint(5, 34);
				icon.infoWindowAnchor = new GPoint(5, 2);
				
				
				
				function createMarker(point,html,markerName) {
        			var myicon=new GIcon(icon);
        			myicon.image = "<?=$conf['mod_gis_marker_folder'] ?>"+markerName+".png";
        			//myicon.image="http://localhost/~nazgul/sahana-main/sahana-phase2-pan/www/theme/default/img/"+markerName+".png";
        			//myicon.image="http://www.google.com/mapfiles/marker.png";
        			var marker = new GMarker(point,myicon);
        			GEvent.addListener(marker, "click", function() {
          				marker.openInfoWindowHtml(html);
        			});
        		return marker;
      			}
      			
      			var map = new GMap2(document.getElementById("map"));
      			map.addControl(new GLargeMapControl());
	       		//map.addControl(new GSmallMapControl());
				map.addControl(new GMapTypeControl());//satellite image switch
				
				map.setCenter(new GLatLng(<?=$conf['mod_gis_center_y'] ?>,<?=$conf['mod_gis_center_x'] ?>), <?=$conf['mod_gis_zoom'] ?>,G_SATELLITE_MAP);
<?php
    for ($i = 0;$i < sizeof($array);$i++) {
        $lat = $array[$i]["lat"];
        $lon = $array[$i]["lon"];
        $name = $array[$i]["name"];
        $url = $array[$i]["id"];
        $c_uuid = $array[$i]["id"];
        $pre_url = "index.php?mod=cr&act=ecmp&seq=view&cmp_id=";
        $url = $pre_url . $c_uuid;
        $desc = (isset($array[$i]["desc"])) ? $array[$i]["desc"] : "";
        $marker_name = (isset($array[$i]["marker"])) ? $array[$i]["marker"] : "marker";
?>	
					var point = new GLatLng(<?=$lon
?>,<?=$lat
?>);
      				var marker = createMarker(point,'<div id="wiki_info"><strong><?=_lc($name) ?></strong><br><?=$desc ?><br><a href="<?=$url ?>">View</a></div>',"<?=$marker_name ?>");
      				map.addOverlay(marker);
<?php
    }
?>
      		}
      		else{
      			alert("Incompatible");
      			}      			
		onloadfunc();
	}		
	//]]>	
	
	</script>
	<div id="map" style="width: <?=$conf['mod_gis_width'] ?>px; height: <?=$conf['mod_gis_height'] ?>px"></div>
<?php
}
?>
