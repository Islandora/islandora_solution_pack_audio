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
                        '!msg' => islandora_executable_available_message($lame),
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

  // Viewers.
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form += islandora_viewers_form('islandora_audio_viewers', 'audio/mpeg');

  // Checksumming.
  $form['islandora_audio_checksum_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Checksum configuration'),
  );

  $form['islandora_audio_checksum_configuration']['islandora_audio_enable_checksum'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable checksum'),
    '#description' => t('Enables checksum creation on the OBJ datastream for digital preservation purposes.'),
    '#default_value' => variable_get('islandora_audio_enable_checksum', FALSE),
  );

  $form['islandora_audio_checksum_configuration']['islandora_audio_checksum_type'] = array(
    '#type' => 'fieldset',
    '#type' => 'select',
    '#title' => t('Checksum type'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_audio_enable_checksum"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => variable_get('islandora_audio_checksum_type', 'DISABLED'),
    '#multiple' => FALSE,
    '#options' => array(
      'MD5' => t('MD5'),
      'SHA-1' => t('SHA-1'),
      'SHA-256' => t('SHA-256'),
      'SHA-385' => t('SHA-385'),
      'SHA-512' => t('SHA-512'),
    ),
  );
  return system_settings_form($form);
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
