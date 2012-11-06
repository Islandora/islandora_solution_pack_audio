<?php

/**
 * @file
 * This file contains all admin functions
 */

function islandora_audio_admin() {
  // add css
  drupal_add_css(drupal_get_path('module', 'islandora_audio') . '/css/islandora_audio.admin.css');
  
  $form = array();
  $form['islandora_audio_use_for_default_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide the view for the default tab'),
    '#default_value' => variable_get('islandora_audio_use_for_default_tab', TRUE),
    '#description' => t('Should this module provide the view for the default view tab.  If you are seeing unexpected content
      on a view tab you may have multiple modules configured to provide content for the default tab.'),
    '#weight' => -10
  );

  // include solution packs inc file
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  // get viewer table
  $viewer_table = islandora_viewers_form('islandora_audio_viewers', 'audio/mpeg');
  // add viewer table
  $form += $viewer_table;

  return system_settings_form($form);
}
