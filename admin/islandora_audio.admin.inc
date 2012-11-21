<?php

/**
 * @file
 * This file contains all admin functions
 */
function islandora_audio_admin($form, &$form_state) {
  // add css
  drupal_add_css(drupal_get_path('module', 'islandora_audio') . '/css/islandora_audio.admin.css');

  if (isset($form_state['values']['islandora_lame_url'])) {
    $lame_url = $form_state['values']['islandora_lame_url'];
  }
  else {
    $lame_url = variable_get('islandora_lame_url', '/usr/bin/lame');
  }
  exec($lame_url, $output, $return_value);
  $confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/>'
      . t('LAME executable found at !lame_url', array('!lame_url' => "<strong>$lame_url</strong>"));
  if ($return_value != 1) {
    $confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/> '
        . t('Unable to find LAME executable at !lame_url', array('!lame_url' => "<strong>$lame_url</strong>"));
  }
  $form = array();
  $forms['islandora_lame_url'] = array(
    '#prefix' => '<div id = "lame_url">',
    '#suffix' => '</div>',
    '#type' => 'textfield',
    '#description' => t("This module requires !LAME to create derivative files", array('!LAME' => l('LAME', 'http://lame.sourceforge.net/'))),
    '#default_value' => $lame_url,
    '#size' => 20
  );

  // ajax wrapper for url checking
  $form['wrapper'] = array(
    '#prefix' => '<div id="islandora-url">',
    '#suffix' => '</div>',
    '#type' => 'markup',
  );

  $form['wrapper']['islandora_lame_url'] = array(
    '#type' => 'textfield',
    '#title' => t("Path to LAME"),
    '#default_value' => $lame_url,
    '#description' => t('Path to !LAME <br /> !confirmation_message',
        array(
          '!confirmation_message' => $confirmation_message,
          '!LAME' => l('LAME', 'http://lame.sourceforge.net/')
    )),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'islandora_update_lame_url_div',
      'wrapper' => 'islandora-url',
      'effect' => 'fade',
      'event' => 'blur',
      'progress' => array('type' => 'throbber'),
    ),
  );

  // include solution packs inc file
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  // get viewer table
  $viewer_table = islandora_viewers_form('islandora_audio_viewers', 'audio/mpeg');
  // add viewer table
  $form += $viewer_table;

  // actions
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 1,
    '#submit' => array('islandora_audio_admin_submit'),
  );

  return system_settings_form($form);
}

/**
 * Form validate
 */
function islandora_audio_admin_validate($form, &$form_state) {

}

/**
 * Form submit
 */
function islandora_audio_admin_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#id'];
  switch ($op) {
    case 'edit-reset':
      variable_del('islandora_audio_viewers');
      break;
  }
}

function islandora_update_lame_url_div($form, $form_state) {
  unset($form_state['submit_handlers']);
  $form_state['rebuild'] = TRUE;

  return $form['wrapper'];
}