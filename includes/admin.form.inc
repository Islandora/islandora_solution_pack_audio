<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_audio_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $get_default_value = function($name, $default) use(&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };
  $lame = $get_default_value('islandora_lame_url', '/usr/bin/lame');
  $form = array(
    'islandora_lame_url' => array(
      '#type' => 'textfield',
      '#title' => t("Path to LAME"),
      '#description' => t('!LAME is required to create derivatives files.<br/>!msg',
                      array(
                        '!LAME' => l(t('LAME'), 'http://lame.sourceforge.net/'),
                        '!msg' =>  t("Put @no_deriv in the text field to indicate that derivatives should not be produced on ingest.<br/>", array('@no_deriv' => ISLANDORA_AUDIO_NO_TRIGGER_DERIVATIVE)).islandora_audio_admin_form_check_lame_for_no_derivative_message($lame),
                      )),
      '#default_value' => $lame,
      '#size' => 20,
      '#prefix' => '<div id="lame-wrapper">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'callback' => 'islandora_audio_admin_form_lame_ajax_callback',
        'wrapper' => 'lame-wrapper',
        'effect' => 'fade',
        'event' => 'change',
      ),
    ),
  );
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form += islandora_viewers_form('islandora_audio_viewers', 'audio/mpeg');
  return system_settings_form($form);
}

/**
 * Determine the message to show to the user based on whether the path indicator
 * is set to not run the derivative
 *
 * @param string $path
 *   The absolute path to an executable to check for availability, or the special set indicating that a derivative should not be produced
 * 
 * @return string
 *   A message in html detailing if the given executable is accessible, or that the user indicated that a derviative should not be produced
 */
function islandora_audio_admin_form_check_lame_for_no_derivative_message($lame){
  if ($lame !== ISLANDORA_AUDIO_NO_TRIGGER_DERIVATIVE){
    return islandora_executable_available_message($lame);
  }else{
    return t("Derivative creation will not be attempted.");
  }
}

/**
 * Ajax callback for the Lame textfield.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The element to render as part the ajax callback.
 */
function islandora_audio_admin_form_lame_ajax_callback(array $form, array $form_state) {
  return $form['islandora_lame_url'];
}
