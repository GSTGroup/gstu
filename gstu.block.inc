<?php


/**
* User starting point block.
*/
function _gstu_block_account() {
  global $user;
  $block = array(); 
  
  // Add Supporting Files to page
  gstu_add_css();
  gstu_add_js();
    
  if ($user->uid) {
    // Button
    $block['subject'] = theme('user_picture', array('account' => $user)) . check_plain($user->name);
    
    // Acount Links Pane
    $block['content'] = "<div class='panes clear-block'><div class='pane-left'>";
    $links = gstu_account_links();
    // These links are intended to take the user to the ROOT of that SPACE NOT as a sub-space of THIS one (so disable PURL)
    //ADFHI: I can't use the following - it breaks OTHER PURL paths - I need to come up with a more "temporary" way
    // - purl_disable doesn't seam to "turn off" when I'm done
    ////if (module_exists('purl')) { purl_disable(TRUE); }
    $block['content'] .= "<h3>". t('My account') ."</h3>". theme('links', array('links' => $links));
    ////if (module_exists('purl')) { purl_disable(FALSE); }
    $block['content'] .= "</div>";

    // Groups Pane
    //ADFHI: Add Groups support? Perhaps make this check for "og" or be a SETTING?
//    if ($view = views_get_view('groups_listing_blocks')) {
//      $block['content'] .= "<div class='pane-right'>";
//      $block['content'] .= "<h3>". t('My groups') ."</h3>" . $view->execute_display('default');
//      $block['content'] .= "</div>";
//      $view->destroy();
//    }
    $block['content'] .= "</div>";
    return $block;
  }
  else {
    $item = menu_get_item('user/login');
    if ($item && $item['access']) {
      $item_title = $item['title'];
      $item_link = l("<span class='icon'></span>", $item['href'], array('html' => TRUE));
      $content = array();
      $content['#type'] = 'container';
      $content['user_login'] = drupal_get_form('user_login');
      $links = gstu_login_links();
      $content['links'] = array(
        '#markup' => "<h3>". t('Other Login Options') ."</h3>" . theme('links', array('links' => $links))
        );
      $block = array(
      	'subject' => 
      		"<div class='account-login'>
      		  $item_link 
      		</div>
          {$item_title}      		
      		",
      	'content' => $content,
      	'collapsiblock' => FALSE, // This prevents collapsiblock from modifying this block
      );
      return $block;
    }
  }
}


/**
 * 
 * Generate a block to be used by the "Add Content" Space-Tools button
 */
function _gstu_block_create_content() {
  //ADFTODO: Implemement a way to "restrict" the Content Types that show up in the list (perhaps on GST settings page?)
  $vars = array(
  );
  $feature_map = features_get_component_map('node');
  $item = menu_get_item('node/add');
  $links = array();
  foreach (system_admin_menu_block($item) as $menu_item) {
    $args = explode('/', $menu_item['href']);
    $type = str_replace('-', '_', array_pop($args));
    $menu_item['title'] = "<span class='spaces-feature node-{$type}'></span>{$menu_item['title']}";
    $menu_item['html'] = TRUE;
    $links["{$type}"] = $menu_item;
    /* We are not using the feature to set the icon - perhaps later I may do this, but for now, the $type sets the icon
    if (!empty($feature_map[$type])) {
      $feature = reset($feature_map[$type]);
      $menu_item['title'] = "<span class='spaces-feature feature-{$feature}'></span>{$menu_item['title']}";
      $menu_item['html'] = TRUE;
      $links["$feature-{$type}"] = $menu_item;
    }
    else {
      $menu_item['title'] = "<span class='spaces-feature node-{$type}'></span>{$menu_item['title']}";
      $menu_item['html'] = TRUE;
      $links["{$type}"] = $menu_item;
    }
    */
  }
  // Add Content List to top of Array  
  $item_gstc = menu_get_item('gst-content');
  if ($item_gstc && $item_gstc['access']) {
    $links['0-gst-content'] = array(
      'title' => "<span class='spaces-feature node-content-list'></span>Content list",
      'href' => $item_gstc['href'],
      'html' => TRUE,       
    );
  }
  ksort($links);
  
  $block = array(
  	'subject' => "<span class='icon'></span> {$item['title']}", 
  	'content' => theme('links', array('links' => $links)),
  	'collapsiblock' => FALSE, // This prevents collapsiblock from modifying this block
    'classes_array' => array('rounded-block'),
  );

  return $block;
}


// <editor-fold defaultstate="collapsed" desc="Block Util">

/**
 * Generates an array of account links suitable for use in theme_links().
 */
function gstu_account_links($purl = FALSE) {
  global $user;
  $links = array();

  // @TODO: If menu.inc had a real API function for this we would use it, but
  // as of now we'd have a copy hand paste hack job of menu_local_tasks()...
  $paths = array(
    "user/{$user->uid}" => t('Profile'),        
    "{$user->name}/dashboard" => '',
    "user/{$user->uid}/signups" => t('Signups'),
    "user/{$user->uid}/edit" => 'Account settings',
    "user/{$user->uid}/notifications" => '',
    'user/logout' => '',
  );
  foreach ($paths as $path => $title) {
    $item = menu_get_item($path);
    if ($item && $item['access']) {
      $link = array(
        'title' => !empty($title) ? $title : $item['title'],
        'href' => $item['href'],
      );
      if (!$purl) {
        $link['purl']['disabled'] = TRUE;
      }
      $links[] = $link;      
    }
  }
  drupal_alter('gstu_account_links', $links);
  return $links;
}

/*
 * Generate an array of links of login options suitable for use in theme_links()
 */
function gstu_login_links($purl = FALSE) {
  $links = array();
  
  $paths = array(
    'user/login' => 'Login Page',
  	'user/password' => '',
    'user/register' => '',
  );
  foreach ($paths as $path => $title) {
    $item = menu_get_item($path);
    if ($item && $item['access']) {
      $link = array(
        'title' => !empty($title) ? $title : $item['title'],
        'href' => $item['href'],
      );
      if (!$purl) {
        $link['purl']['disabled'] = TRUE;
      }
      $links[] = $link;
    }
  }
  drupal_alter('gst_login_links', $links);
  return $links;  
}

// </editor-fold> // Block Util
