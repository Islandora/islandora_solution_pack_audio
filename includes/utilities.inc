<?php

/**
 * @file
 * Utilities functions for the audio SP.
 */

/**
 * Determines if a datastream is valid for playback in the browser.
 *
 * @param AbstractDatastream $datastream
 *   The datastream to check.
 *
 * @return bool
 *   TRUE if the datastream can be played; FALSE otherwise.
 */
function islandora_audio_datastream_valid_for_playback(AbstractDatastream $datastream) {
  if ($datastream->mimetype == 'audio/mpeg' || $datastream->mimetype == 'audio/mp4') {
    return TRUE;
  }
  // @TODO: This is currently a simple check against types supported by all
  // browsers. We may want to be more granular in the future. We may also want
  // to offset this responsibility to individual viewers.
  return FALSE;
}
