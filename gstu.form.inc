<?php

function gstu_add_token_help(&$element, $token_types = array('file'), $desc = NULL, $weight = NULL) {

  $desc = strlen($desc) ? $desc :
    t("The following placeholder tokens can be used in texts allowing tokens above. ".
      "When used, they will be replaced with the appropriate values.");
  if (module_exists('token')) {
    // Add token module replacements fields
    $element['tokens'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Placeholder tokens'),
      '#description' => $desc,
    );
    if ($weight) {
      $element['tokens']['#weight'] = $weight;
    }
    $token_type = array(
      'theme' => 'token_tree',
      'token_types' => $token_types,
      'global_types' => TRUE,
      'click_insert' => TRUE,
      'recursion_limit' => 3,
    );
    $element['tokens']['help'] = array(
      '#type' => 'markup',
      '#markup' => theme('token_tree', $token_type),
    );
  }
}

/**
* Helper function for cloning and drupal_render()'ing elements.
*/
function gstu_clone_form_field($elements) {
  static $instance;
  if (!isset($instance)) {
    $instance = 1;
  }
  $c_elements = $elements;
  if (isset($c_elements['#id'])) {
    $c_elements['#id'] = "{$c_elements['#id']}-{$instance}";
  }
  foreach (element_children($c_elements) as $key) {
    if (isset($c_elements[$key]['#id'])) {
      $c_elements[$key]['#id'] = "{$c_elements[$key]['#id']}-{$instance}";
    }
  }
  $instance++;
  return $c_elements;
}


/*
 * NOTE: We could handle this one of two ways:
* 1) We could do what we do before, pass the $element of the $form in as a REF (&)
*    and alter that $element directly
* 2) We could pass in the $form and iterate over the values as in form_state_values_clean()
*
* I chose #1 because in this case, it is simpler, but either way woudl work.
*/

function gstu_text_format_make_collapsible(&$element, $title = NULL, $collapsed = TRUE) {
  if (!isset($element) || !is_array($element)) { return; }
  
  //NOTE: This function can ONLY be used from a template file - you CANNOT use it from form_alter()
  // it does NOT work then because the widget is not sufficiently "built" by then.  
  if ($element['#type'] == 'container') {
    $lang = $element['#language'];
    $title = ($title) ? $title : $element[$lang]['#title'] . ' Text Format';
    $element[$lang][0]['format']['#title'] = $title;
    $element[$lang][0]['format']['#collapsible'] = TRUE;
    $element[$lang][0]['format']['#collapsed'] = $collapsed;
  }
  if ($element['#type'] == 'text_format') {
    $title = ($title) ? $title : $element['#title'] . ' Text Format';
    $element['format']['#title'] = $title;
    $element['format']['#collapsible'] = TRUE;
    $element['format']['#collapsed'] = $collapsed;    
  }
}

/**
 * Find a form-element BY NAME in a $form render-array and return a REFERENCE to that element
 * Usage: $body = &gst_form_element($form, 'body');
 * // Note: Only finds the following fields:
 *   body, title
 *   field_*: any field name beginning with 'field_'
 *   To add more field names to search, update gst_form_element_filter()
 * 
 *  * ** MAKE SURE YOU CALL THE FUNCTION WITH & to return a reference! **
 * 
 * @param array $form
 * @param string $name
 */
function &gstu_form_element(&$form, $name) {
  $children = element_children($form, TRUE);  
  if ($key = array_search($name, $children) !== FALSE) {
    return $form[$name];
  }
  $children = array_filter($children, 'gstu_form_element_filter');
  foreach ($children as $cname) {
    $child = &$form[$cname];
    if ($found = &gstu_form_element($child, $name)) { return $found; }
  }   
  $f = FALSE;
  return $f;
}

/**
 * Similar to gst_form_element() but actualy *performs* the action passed in 'action'
 * NOTE: only updates the following fields:
 * body, title
 * field_*: any field name beginning with 'field_'
 * To add more field names to search, update gst_form_element_filter()
 *  
 * @param array $form
 * @param string $name
 * @param string $action | options: 'hide', 'collapse'
 */
function gstu_form_element_action(&$form, $name, $action) {
  $children = element_children($form, TRUE);  
  if ($key = array_search($name, $children) !== FALSE) {
    switch ($action) {
      case 'hide':
        hide($form[$name]);
        break;
      case 'collapse':
        gstu_text_format_make_collapsible($form[$name]);
        break;
    }
  }
  $children = array_filter($children, 'gstu_form_element_filter');
  foreach ($children as $cname) {
    //$child = &$form[$cname];
    gstu_form_element_action($form[$cname], $name, $action);
  }
}

function gstu_form_element_filter($var) {
  $allowed = array('body', 'title');
  if (array_search($var, $allowed) !== FALSE) {
    return TRUE;
  }
  if (gstu_startsWith($var, 'field_')) {
    return TRUE;
  }
  if (gstu_startsWith($var, 'group_')) {
    return TRUE;
  }  
  if (gstu_startsWith($var, 'gstform_region_')) {
    return TRUE;
  }  
  return FALSE;
}

/**
 * Sets a Date's labels. This should be called from the HOOK_preprocess_node() function since
 *   it is only at that time that the Date render array will be complete and can be modified.
 *   
 * @param array $element
 * @param string $part_name The part to set: either 'value' or 'value2'
 * @param string $date_label What to set the Date title to (default: "Date")
 * @param string $time_label What to set the Time title to (default: "Time")
 */
function gstu_set_date_labels(&$element, $part_name, $date_label = 'Date', $time_label = 'Time') {
  if (!isset($element) || !is_array($element)) { return; }
  $lang = $element['#language'];
  $element[$lang][0][$part_name]['date']['#title'] = $date_label;
  $element[$lang][0][$part_name]['time']['#title'] = $time_label;  
}

/**
 * Sets the #attribute['class'] value of an $element *preserving* any existing values (if any)
 * @param array $element The element to perform the replacement in (passed by REFERENCE)
 * @param array $element_classes The array of classes (if a string, will be split on commas (,))
 */
function gstu_set_element_class(&$element, $element_classes) {
  if (!isset($element) || !is_array($element)) { return; }
  if (!is_array($element_classes)) { 
    $element_classes = explode(',', $element_classes);
  }
  $ele = array(
    '#attributes' => array(
      'class' => $element_classes,
    )
  );
  $element = array_merge_recursive($element, $ele);  
}

function gstu_set_multivalue_extra_rows(&$element, $extra_rows) {
  if (!isset($element) || !is_array($element)) { return; }
  $lang = $element['#language'];
  $max_delta = $element[$lang]['#max_delta'];
  if ($extra_rows == 0) {
   unset($element[$lang][$max_delta]); 
  }
  // By Default there is ALWAYS 1 row, so if they specify 1, they are already fine, for 2+, we copy what is in max_delta
  if ($extra_rows > 1) {
    for ($d = 2; $d <= $extra_rows; $d++) {
      $element[$lang][$max_delta + $d - 1] = $element[$lang][$max_delta];
    }
  }  
}