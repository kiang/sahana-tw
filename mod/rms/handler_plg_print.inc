<html>
    <head>
        <style type="text/css">
table {
    width: 85%;
    margin: auto;
    border-bottom: 1px solid black;
    border-right: 1px solid black;
}
td {
    border-left: 1px solid black;
    border-top: 1px solid black;
}
        </style>
    </head>
    <body>
<?php
$plg_id = $_REQUEST['plg_id'];
$plg = _shn_rms_get_pledge($plg_id);
_shn_rms_view_pledge($plg_id);
echo '<div align="center"> ~ ' . _('Blank below') . ' ~ </div>';
?>
</body></html>