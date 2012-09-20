<?php

/**
 * Creates the derivatives for this content model type.
 * @param object $object
 *   tuque FedoraObject
 * @return string
 */
function islandora_audio_create_all_derivatives($object) {
dsm("creating derivatives");
  module_load_include("inc", "islandora", "includes/mime.detect");

  $mime_class = new MimeDetect();
  if (!isset($object["OBJ"])) {
    drupal_set_message(t("Could not create derivatives for %s.  No file was uploaded.", array("%s" => $object->id), "error"));
    return FALSE;
  }

  $ext = $mime_class->getExtension($object["OBJ"]->mimeType);
  $file_name = str_replace(":", "-", $object->id);
  $original_file = file_save_data($object["OBJ"]->content, "temporary://" . $file_name . "OBJ." . $ext);
  $mp3_file = file_copy($original_file, "temporary://" . $file_name . "MP3." . $ext);
  if (islandora_audio_create_mp3($mp3_file)) {
    islandora_audio_add_datastream($object, "PROXY_MP3", $mp3_file);
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
  $command_string = "lame -V5 --vbr-new '${file}' '${file}.mp3'";
  $return_value = FALSE;
  exec($command_string, $output, $return_value);

  if ($return_value == '0') {
    // succeeded in creating derivative mp3
    return TRUE;
  }

  return $return_value;
}

/**
 * adds a datastream and deletes the tmp file from the local file system
 * @param object $object
 * @param string $dsid
 * @param object $file
 */
function islandora_audio_add_datastream($object, $dsid, $file) {
  try {
    $ds = $object->constructDatastream($dsid, "M");
    $ds->label = $dsid;
    $ds->mimetype = $object["OBJ"]->mimeType;
    $ds->setContentFromFile(drupal_realpath($file->uri));
    $object->ingestDatastream($ds);
    file_delete($file);
  }
  catch (exception $e) {
    drupal_set_message(t("@message", array("@message" => $e->getMessage())));
  }
}
