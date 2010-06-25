<h2>_("Project Closure Report")</h2>
{if $p_uuid == $project['manager']['p_uuid']}
<div id="submenu_v">
    <a href='?mod=vm&act=project&vm_action=display_closure_edit&proj_id={$project.proj_id}'>_("Edit")</a>
</div>
{/if}

<table align="center" width="85%">
    <tbody>
        <tr>
            <td><b>_("Project Name :")</b></td>
            <td>{$project.name}</td>
        </tr>
        <tr>
            <td><b>_("Date :")</b></td>
            <td>{$project.start_date} ~ {$project.end_date}</td>
        </tr>
        <tr>
            <td><b>_("Project manager :")</b></td>
            <td>{$project.manager.full_name}</td>
        </tr>
        <tr>
            <td><b>_("Last modified")</b></td>
            <td>{$project.modified}</td>
        </tr>
        <tr>
            <td><b>_("Description")</b></td>
            <td>{$project.projectDescription}</td>
        </tr>
    </tbody>
</table><br />
<h3>_("Volunteers attending status")</h3>
<table align="center" width="85%">
    <thead>
        <tr>
            <td>_("Volunteer name")</td>
            <td>_("Volunteer position")</td>
            <td>_("Attended hours")</td>
        </tr>
    </thead>
    <tbody>
        {foreach $project['positions'] as $position}
        <tr>
            <td>{$position.full_name}</td>
            <td>{$position.title}</td>
            <td>{$position.hours}</td>
        </tr>
        {/foreach}
    </tbody>
</table><br />
<h3>_("Description of the progress")</h3>
<div>{$project.description}</div><br />
<h3>_("Suggestion of this project")</h3>
<div>{$project.suggestion1}</div><br />
<div>{$project.suggestion2}</div><br />

<div align="center">
<a href='?mod=vm&act=project&vm_action=display_closure_reports'>_("Back to closure report list")</a>
&nbsp; &nbsp;
<a href='?mod=vm&act=default'>_("Back to index of volunteer module")</a>
</div>