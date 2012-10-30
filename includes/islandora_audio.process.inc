<?php

/**
 * @file
 * This file contains all functions to process derivatives of audio objects
 */

/**
 * Creates the derivatives for this content model type.
 * @param object $object
 *   tuque FedoraObject
 * @return string
 */
function islandora_audio_create_all_derivatives($object) {
  // create derivatives
  module_load_include("inc", "islandora", "includes/mime.detect");

  if (!isset($object["OBJ"])) {
    drupal_set_message(t("Could not create derivatives for %s.  No file was uploaded.", array("%s" => $object->id), "error"));
    return FALSE;
  }

  $ext = pathinfo($object["OBJ"]->uri, PATHINFO_EXTENSION);
  $file_name = str_replace(":", "-", $object->id);
  $original_file = file_save_data($object["OBJ"]->content, "temporary://" . $file_name . "OBJ." . $ext);
  //$mp3_file = file_copy($original_file, "temporary://" . $file_name . "MP3." . $ext);
  // create MP3
  $mp3 = islandora_audio_create_mp3($original_file);
  if ($mp3) {
    islandora_audio_add_datastream($object, "PROXY_MP3", $mp3);
  }
  // create exif
  $exif = islandora_audio_create_exif($original_file);
  if ($exif) {
    islandora_audio_add_datastream($object, "TECHMD", $exif);
  }

  file_delete($original_file);
  return TRUE;
}

/**
 *
 */
function islandora_audio_create_mp3($file_object) {
  $output = array();
  $file = drupal_realpath($file_object->uri);
  $outfile = $file . ".mp3";
  $command_string = "lame -V5 --vbr-new '${file}' '${outfile}'";
  $return_value = FALSE;
  exec($command_string, $output, $return_value);

  if ($return_value == '0') {
    // succeeded in creating derivative mp3
    return $outfile;
  }

  return FALSE;
}

/**
 * Creates file derivatives as xml.
 */
function islandora_audio_create_exif($file_object) {
  $output = array();
  $file = drupal_realpath($file_object->uri);
  $outfile = $file . "_exif.xml";
  $command_string = "exiftool -X " . $file;
  $return_value = FALSE;
  exec($command_string, $output, $return_value);
  if ($return_value == '0') {
    // succeeded in creating derivative exif
//    file_put_contents($outfile, implode("\n", $output));
    return $outfile;
  }
  return FALSE;
}

/**
 * adds a datastream and deletes the tmp file from the local file system
 * @param object $object
 * @param string $dsid
 * @param object $file
 */
function islandora_audio_add_datastream($object, $dsid, $file) {
  try {
    $mime_detector_thingy = new MimeDetect(); // this class is wrong - it should be a singleton
    $ds = $object->constructDatastream($dsid, "M");
    $ds->label = $dsid;
    $ds->mimetype = $mime_detector_thingy->getMimetype($file);
    $ds->setContentFromFile(drupal_realpath($file));
    $object->ingestDatastream($ds);
//    file_delete($file);
  }
  catch (exception $e) {
    drupal_set_message(t("@message", array("@message" => $e->getMessage())));
  }
}
