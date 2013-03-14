<?php
/**
* DS: Node Submitted Picture Field
*/
function gstu_ds_node_submitted_picture_field($field) {
  // See gst.module:gst_preprocess_node() for how this gets "activated" - I could also use gst_add_beautytip()
  $entity = $field['entity'];
  $username = '';
  $node_user = user_load($entity->uid);
  $username = theme('username', array('account' => $node_user));
  $submitted_username = "<div class='submitted-username'>{$username}</div>";
  $date = module_exists('reldate') ? reldate_format_date($entity->created) : format_date($entity->created, 'small');
  $submitted_date = "<div class='submitted-date'>{$date}</div>";  
  $user_picture = ds_render_user_picture($field);
  $lmod_date = module_exists('reldate') ? reldate_format_date($entity->revision_timestamp) : format_date($entity->revision_timestamp, 'small');
  $lmod_user = theme('username', array('account' => user_load($entity->revision_uid)));
  
//  $output = "
//    <div class='meta submitted node-submitted'>
//      <div class='user-picture'>${user_picture}</div>
//      ${submitted_username}
//      ${submitted_date}
//    </div>  
//    <div class='hide beautytip'>
//    	<div class='node-submitted'>
//    		Published by ${username} at ${date}<br> 
//    		Last Modified by ${lmod_user} at ${lmod_date}<br>
//    	</div>
//    </div>
//  ";

  $content = "
    <div class='user-picture'>${user_picture}</div>
    ${submitted_username}
    ${submitted_date}
  ";

  $bt_content = "
    <div class='node-submitted'>
      Published by ${username} at ${date}<br> 
      Last Modified by ${lmod_user} at ${lmod_date}<br>
    </div>    
  ";
  $options = array(
    '#content' => $content,
    '#bt_content' => $bt_content, 
    '#attributes_content' => array('class' => array('meta', 'submitted', 'node-submitted')),
    //'trigger' => array('click', 'click'),
    //'trigger' => array('mouseover', 'click'), // For Testing Purposes
    //'positions' => array('most'),
    //'cssSelect' => '.node .field .node-submitted',
  );
  $output = gstu_add_beautytip($options);           
        
  return $output;
}

/**
* DS: Node Submitted Author line
 *    fmt: <content type-icon> <author> - submit date/time
*/
function gstu_ds_node_submitted_author($field) {
  // See gst.module:gst_preprocess_node() for how this gets "activated" - I could also use gst_add_beautytip()
  $entity = $field['entity'];
  $username = '';
  $node_user = user_load($entity->uid);
  $username = theme('username', array('account' => $node_user));
  $submitted_username = "<div class='submitted-username'>{$username}</div>";
  $date = module_exists('reldate') ? reldate_format_date($entity->created) : format_date($entity->created, 'small');
  $submitted_date = "<div class='submitted-date'>{$date}</div>";    
  $lmod_date = module_exists('reldate') ? reldate_format_date($entity->revision_timestamp) : format_date($entity->revision_timestamp, 'small');
  $lmod_user = theme('username', array('account' => user_load($entity->revision_uid)));

  // Create an icon to display *before* the author - submit date
  $ct_names = node_type_get_names();
  $type = $entity->type;
  $type_name = $ct_names[$type];
  $ctype_icon = "<span class='spaces-feature node-{$type}'></span>";
  $content = "
    <div class='meta submitted node-submitted-author inline'>
      ${ctype_icon} ${submitted_username} - ${submitted_date}
    </div>
  ";
      
  $bt_content = "      
    	<div class='node-submitted-author'>
        <b>{$type_name}</b><br>
    		Published by ${username} at ${date}<br> 
    		Last Modified by ${lmod_user} at ${lmod_date}<br>
    	</div>
  ";  
  
  $options = array(
    '#content' => $content,
    '#bt_content' => $bt_content, 
    //'trigger' => array('click', 'click'),
  );
  $output = gstu_add_beautytip($options);           
   
  return $output;  
}
