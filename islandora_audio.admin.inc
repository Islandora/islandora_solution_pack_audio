<?php

/**
 * islandora audio admin settings
 * @param array $form_state
 * @return array
 */
function islandora_audio_admin_settings(&$form_state) {

	//get the path for lame
	
  $lame_path = isset($form_state['values']['islandora_lame_path']) ? $form_state['values']['islandora_lame_path'] : variable_get('islandora_lame_path', 'lame');
  

  /*
  if ($lame_path == 'lame' || $lame_path == '')
  {
  		//set the path name
		$lame_path = 'lame';
		
		$which_lame = exec("which $lame_path");
		$lame_avail = exec("ls $which_lame");
  }
  else {
  		$lame_avail = exec("ls $lame_path");
  }
   *
   */
  exec("$lame_path --help", $output, $return);
  dsm($output);
  dsm($return);
	
  //we want to see if the lame path is available
  /*
  if($return == 0) {
    $confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/>' . t('Lame found at !lame_path', array('!lame_path' => $lame_path));
  }
  else {
    $confirmation_message = '<img src="' 
          . url('misc/watchdog-error.png') . '"/> '
          . t('Unable to locate Lame at !lame_path</p>', array('!lame_path' => $lame_path)));
  }
   * 
   */

  $confirmation_message = ($lame_avail ? '<img src="' . url('misc/watchdog-ok.png') . '"/>'
          . t('Lame found at !lame_path', array('!lame_path' => $lame_path)) : '<img src="'
          . url('misc/watchdog-error.png') . '"/> '
          . t('Unable to locate Lame at !lame_path</p>', array('!lame_path' => $lame_path)));

  $lame = isset($form_state['values']['islandora_audio_do_lame']) ? $form_state['values']['islandora_audio_do_lame'] : variable_get('islandora_audio_do_lame', TRUE);

  $form = array();
  
 $form['audio_ahah_wrapper'] = array(
    '#prefix' => '<div id="iaudio-url">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  
   $form['audio_ahah_wrapper']['islandora_audio_do_lame'] = array(
    '#type' => 'checkbox',
    '#title' => t('Transform files locally ?'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => $lame,
    '#ahah' => array(
      'path' => 'islandora/audio/lame',
      'wrapper' => 'iaudio-url',
      'effect' => 'fade',
      'event' => 'change'),
  );

	if ($lame) {	
		$form['audio_ahah_wrapper']['islandora_lame_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to lame executable'),
      '#description' => t('Path to lame program on your server'),
      '#default_value' => $lame_path,
      '#ahah' => array(
        'path' => 'islandora/audio/lame',
        'wrapper' => 'iaudio-url',
        'keypress'=> true,
        'effect' => 'fade',
        'event' => 'change'),
    	);
    }
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save Configuration'));
  $form['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults'));
  $form['#submit'][] = 'audio_settings_form_submit';
  $form['#theme'] = 'system_settings_form';
  
   $form['audio_ahah_wrapper']['infobox'] = array(
      '#type' => 'item',
      '#value' => $confirmation_message,
    );
  
  return ($form);
}

/**
 * update lame div
 */
function update_lame_div() {

  $form = audio_callback_prep();
  $changed_elements = $form['audio_ahah_wrapper'];
  unset($changed_elements['#prefix'], $changed_elements['#suffix']);
  $output = theme('status_messages') . drupal_render($changed_elements);
  drupal_json(array(
    'status' => TRUE,
    'data' => $output,
  ));
}


/**
 * audio callback prep
 * @return type
 */
function audio_callback_prep() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE, 'rebuild' => TRUE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  // Enable the submit/validate handlers to determine whether AHAH-submittted.
  $form_state['ahah_submission'] = TRUE;
  $form['#programmed'] = $form['#redirect'] = FALSE;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  return $form;
}

/**
 * audio settings form submit ??
 * @param type $form
 * @param type $form_state
 * @return type
 */
function audio_settings_form_submit($form, &$form_state) {

  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($form_state['ahah_submission'] == TRUE) {
    $form_state['rebuild'] = TRUE;
    return;
  }
  if ($form_state['clicked_button']['#id'] != 'edit-submit' && $op != t('Reset to defaults')) {
    $form_state['rebuild'] = TRUE;
    return;
  }


  // Exclude unnecessary elements.
  unset($form_state['values']['submit'], $form_state['values']['reset'], $form_state['values']['form_id'], $form_state['values']['op'], $form_state['values']['form_token'], $form_state['values']['form_build_id']);

  foreach ($form_state['values'] as $key => $value) {
    if ($op == t('Reset to defaults')) {
      variable_del($key);
    }
    else {
      if (is_array($value) && isset($form_state['values']['array_filter'])) {
        $value = array_keys(array_filter($value));
      }
      variable_set($key, $value);
    }
  }
  if ($op == t('Reset to defaults')) {
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
  else {
    drupal_set_message(t('The Islandora Audio configuration options have been saved.'));
  }

  cache_clear_all();
  drupal_rebuild_theme_registry();
}
/**
 * audio admin refresh
 * @param array $form
 * @param array $form_state
 */
function audio_admin_refresh($form, &$form_state) {
  $values = $form_state['values'];
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['rebuild'] = TRUE;
  }
  