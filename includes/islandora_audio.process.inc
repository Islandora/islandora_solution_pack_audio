<?php

/**
 * @file
 *
 * This file contains all functions used to generate derivatives of audio objects.
 */

/**
 * Creates all audio based the derivatives as datastreams for given object.
 *
 * @param FedoraObject $object
 *   The object that will be used to generate/store the derivatives.
 *
 * @return boolean
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_audio_create_all_derivatives($object) {
  if (!isset($object["OBJ"])) {
    drupal_set_message(t("Could not create derivatives for %s.  No file was uploaded.", array("%s" => $object->id), "error"));
    return FALSE;
  }
  $mime_detect = new MimeDetect();
  $ext = $mime_detect->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(":", "-", $object->id);
  $file_uri = file_unmanaged_save_data($object['OBJ']->content, "temporary://{$file_name}.OBJ.{$ext}");
  if (($mp3_uri = islandora_audio_create_mp3($file_uri)) !== FALSE) {
    islandora_audio_add_datastream($object, 'PROXY_MP3', $mp3_uri);
  }
  if (($exif_uri = islandora_audio_create_exif($file_uri)) !== FALSE) {
    islandora_audio_add_datastream($object, 'TECHMD', $exif_uri);
  }
  file_unmanaged_delete($file_uri);
  islandora_audio_add_datastream($object, 'TN', drupal_get_path('module', 'islandora_audio') . '/images/audio-TN.jpg');
  return TRUE;
}

/**
 * Creates the mp3 derivative from the given audio file.
 *
 * @param string $file_uri
 *   The URI to the audio file the derivative will be generated from.
 *
 * @return string
 *   A URI to the generated derivative if successful, FALSE otherwise.
 */
function islandora_audio_create_mp3($file_uri) {
  $output = array();
  $file = drupal_realpath($file_uri);
  $outfile = $file . '.mp3';
  $command_string = "lame -V5 --vbr-new '${file}' '${outfile}'";
  $ret = FALSE;
  exec($command_string, $output, $ret);
  if ($ret == '0') {
    return $outfile;
  }
  return FALSE;
}

/**
 * Creates the exif derivative from the given audio file.
 *
 * @param string $file_uri
 *   The URI to the audio file the derivative will be generated from.
 *
 * @return string
 *   A URI to the generated derivative if successful, FALSE otherwise.
 */
function islandora_audio_create_exif ($file_uri) {
  $output = array();
  $file = drupal_realpath($file_uri);
  $outfile = $file . '_exif.xml';
  $command_string = "exiftool -X $file > $outfile";
  $ret = FALSE;
  exec($command_string, $output, $ret);
  if ($ret == '0') {
    return $outfile;
  }
  return FALSE;
}

/**
 * Adds the given file as a datastream to the given object using the given datastream id to identify it.
 *
 * @param FedoraObject $object
 *   The object to add the datasteam to.
 * @param string $datastream_id
 *   The datastream id of the added datastream.
 * @param string $file_uri
 *   A URI to the file containing the content for the datastream.
 */
function islandora_audio_add_datastream($object, $datastream_id, $file_uri) {
  try {
    $mime_detector = new MimeDetect();
    $ds = $object->constructDatastream($datastream_id, "M");
    $ds->label = $datastream_id;
    $ds->mimetype = $mime_detector->getMimetype($file_uri);
    $ds->setContentFromFile(drupal_realpath($file_uri), FALSE);
    $object->ingestDatastream($ds);
  }
  catch (exception $e) {
    drupal_set_message(t("@message", array("@message" => $e->getMessage())));
  }
}
