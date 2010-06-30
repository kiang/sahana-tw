{php}
echo '<h3>' . _('Project name:') . $project['name'];
if(!empty($project['manager']['full_name'])) {
    echo ' &nbsp; &nbsp; ' . _('Project manager:') . $project['manager']['full_name'];
}
echo '</h3>';
shn_form_fopen('project&vm_action=display_closure_edit&proj_id=' . $project['proj_id']);
        if(!empty($project['positions'])) {
            shn_form_fsopen(_('How many hours the volunteers had attended'));
            echo '<table align="center" width="85%">';
            echo '<thead>
        <tr>
            <td>' . _('Volunteer name') . '</td>
            <td>' . _('Volunteer position') . '</td>
            <td>' . _('Attended or absence') . '</td>
            <td>' . _('Attended hours') . '</td>
        </tr>
            </thead><tbody>';
            foreach($project['positions'] AS $position) {
                echo '<tr>
            <td>' . $position['full_name'] . '</td>
            <td>' . $position['title'] . '</td>
            <td>
                <input type="radio" name="attended[' . $position['pos_id'] . '][' . $position['p_uuid'] . ']" value="1"
                ' . (($position['is_attended'] == 1) ? 'checked="checked"' : '') . ' />
                ' . _('Attended') . '
                <input type="radio" name="attended[' . $position['pos_id'] . '][' . $position['p_uuid'] . ']" value="0"
                ' . (($position['is_attended'] == 1) ? '' : 'checked="checked"') . ' />
                ' . _('Absence') . '
            </td>
            <td><input type="text" name="hours[' . $position['pos_id'] . '][' . $position['p_uuid'] . ']" value="' . $position['hours'] . '" /></td>
        </tr>';
            }
            echo '</tbody></table>';
            shn_form_fsclose();
        }

	shn_form_fsopen(_('Description of the progress'));
        shn_form_textarea('', "description", '', array(
            'value'=>$project['description'],
            'cols' => 60, 'rows' => 6,
        ));
	shn_form_fsclose();

	shn_form_fsopen(_('Suggestion of this project'));
        shn_form_textarea('', "suggestion", '', array(
            'value'=>$project['suggestion'],
            'cols' => 60, 'rows' => 6,
        ));
	shn_form_fsclose();

	shn_form_submit(_('Submit'));
	shn_form_button(_('Cancel'), "onClick='history.go(-1);'");
shn_form_fclose();
{/php}