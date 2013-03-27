<?php

/** islandora audio admin settings
 * @file islandora audio admin settings
 * @param array $form_state
 * @return array
 */

function islandora_audio_admin_settings(&$form_state) {

  $lame_path = isset($form_state['values']['islandora_audio_lame_path']) ? $form_state['values']['islandora_audio_lame_path'] : variable_get('islandora_audio_lame_path', '/usr/bin/lame');
  $lame_avail = exec("ls $lame_path");

  $confirmation_message = ($lame_avail ? '<img src="' . url('misc/watchdog-ok.png') . '"/>'
    . t('LAME found at !lame_path', array('!lame_path' => $lame_path)) : '<img src="'
    . url('misc/watchdog-error.png') . '"/> '
    . t('Unable to locate LAME at !lame_path</p>', array('!lame_path' => $lame_path)));

  $lame = isset($form_state['values']['islandora_audio_do_lame']) ? $form_state['values']['islandora_audio_do_lame'] : variable_get('islandora_audio_do_lame', TRUE);
  $lame_preset = isset($form_state['values']['islandora_audio_lame_preset']) ? $form_state['values']['islandora_audio_lame_preset'] : variable_get('islandora_audio_lame_preset', array('standard'));

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
      'effect' => 'none',
      'event' => 'change'),
  );

  $form['islandora_audio_lame_preset'] = array(
    '#type' => 'radios',
    '#title' => t('LAME preset'),
    '#description' => t('Select the LAME quality preset for converting to MP3'),
    '#default_value' => variable_get('islandora_audio_lame_preset', array('standard')),
    '#options' => array(
      'insane' => t('Insane: This preset will usually be overkill for most people and most situations, but if you must have the absolute highest quality with no regard to filesize, this is the way to go.'),
      'extreme' => t('Extreme: If you have extremely good hearing and similar equipment, this preset will generally provide slightly higher quality than the standard mode.'),
      'standard' => t('Standard: This preset should generally be transparent to most people on most music and is already quite high in quality.'),
      'medium' => t('Medium: This preset should provide near transparency to most people on most music.')),
    '#ahah' => array(
      'path' => 'islandora/audio/lame',
      'wrapper' => 'iaudio-url',
      'effect' => 'none',
      'event' => 'change'),
  );

  if ($lame) {
    $form['audio_ahah_wrapper']['islandora_audio_lame_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to LAME executable'),
      '#description' => t('Path to LAME program on your server'),
      '#default_value' => $lame_path,
      '#ahah' => array(
        'path' => 'islandora/audio/lame',
        'wrapper' => 'iaudio-url',
        'effect' => 'none',
        'event' => 'change'),
    );

    $form['audio_ahah_wrapper']['infobox'] = array(
      '#type' => 'item',
      '#value' => $confirmation_message
    );
  }

  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save Configuration'));
  $form['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults'));
  $form['#submit'][] = 'audio_settings_form_submit';
  $form['#theme'] = 'system_settings_form';
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  return system_settings_form($form);
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
  // Enable the submit/validate handlers to determine whether AHAH-submitted.
  $form_state['ahah_submission'] = TRUE;
  $form['#programmed'] = $form['#redirect'] = FALSE;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  return $form;
}

/**
 * audio admin form submit
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
 * video admin refresh
 * @param array $form
 * @param array $form_state
 */
function audio_admin_refresh($form, &$form_state) {
  $values = $form_state['values'];
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['rebuild'] = TRUE;
}
