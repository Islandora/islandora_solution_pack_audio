
<?php

// $Id$

class Exiftool {

  private $pid = NULL;
  private $item = NULL;

  function __construct($pid = null) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    if (!empty($pid)) {
      $this->pid = $pid;
      $this->item = new Fedora_Item($pid);
    }
  }

  function extractMetadata($parameterArray, $dsid, $file, $file_ext) {
    $file_suffix = '_' . $dsid . '.xml';
    $returnValue = TRUE;
    $output = array();
    exec('exiftool -X ' . escapeshellarg($file) . '', $output);
    file_put_contents($file . $file_suffix, implode("\n", $output));
    $_SESSION['fedora_ingest_files']["$dsid"] = $file . $file_suffix;
    return TRUE;
  }

  function displayMetadata() {
    $output = '';
    $exif = $this->item->get_datastream_dissemination('EXIF');
    if (trim($exif) != '') {
      $exifDom = DOMDocument::loadXML($this->item->get_datastream_dissemination('EXIF'));
      if ($exifDom != NULL) {
        $description = $exifDom->getElementsByTagNameNS('http://www.w3.org/1999/02/22-rdf-syntax-ns#', 'Description');
        if ($description->length > 0) {
          $description = $description->item(0);
          $output .= '<div class="fedora_technical_metadata"><ul>';
          for ($i = 0; $i < $description->childNodes->length; $i++) {
            $name = $description->childNodes->item($i)->nodeName;
            $value = $description->childNodes->item($i)->nodeValue;
            if ($name != '#text' && !preg_match('/^System\:.*$/', $name) && trim($value) != '') {
              list($type, $name) = preg_split('/\:/', $name);
              $name = trim(preg_replace('/(?<!^)([A-Z][a-z]|(?<=[a-z])[A-Z])/', " $1", $name));
              $output .= '<li><b>' . $name . '</b>:  ' . $value . ' </li>';
            }
          }
          $output.='</ul></div>';

          $fieldset = array(
            '#title' => t("!text", array('!text' => 'Technical Metadata')),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#value' => $output
          );
          $output = theme('fieldset', $fieldset);
        }
      }
    }
    return $output;
  }

}

class AudioManipulation {

  function AudioManipulation() {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
  }

  function createMP3($parameterArray = NULL, $dsid, $file, $file_ext) {
    $file_suffix = "_$dsid.$file_ext";
    $return_value = TRUE;

    $output = array();
    $command_string = "lame -V5 --vbr-new '${file}' '${file}.mp3'";

    exec($command_string, $output, $returnValue);

    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files']["PROXY_MP3"] = $file . '.mp3';
      return true;
    }
    else {
      ObjectHelper::warnIfMisconfigured('LAME'); /// need to add this to ObjectHelper
    }
  }

}

class SWFAudioPlayer {

  private $pid = NULL;

  function __construct($pid) {
    $this->pid = $pid;
  }

  function showMP3() {

    global $base_url;
    $swfURL = $base_url .  '/fedora/repository/' . $this->pid . '/PROXY_MP3/MP3.mp3';
    $swfImageURL = $base_url .  '/fedora/repository/' . $this->pid . '/TN/tn.jpg';


    //PROXY_MP3 is the datastream id
    $path = drupal_get_path('module', 'Fedora_Repository');

    $content = "";
    $pathTojs = "/sites/all/libraries/mediaplayer4/jwplayer.js";
    drupal_add_js("$pathTojs");

    //swfobject.js is being loaded from swftools module but we are adding the path to jwplayer.js as it is needed as well
    // if swftools are not enabled uncomment this to here drupal_add_js("sites/all/libraries/swfobject/swfobject.js");
    // drupal_add_js("sites/all/libraries/swfobject/swfobject.js");

    $content .= '<video id="flashContent"><a href="http://www.macromedia.com/go/getflashplayer">Get the Flash Player</a> to see this player.</video>';
    $content .= '<div id="dl_link"><a href="' . $swfURL . '">Download MP3</a></div>';

// player bits

    $js_string = <<<ENDJS

var swfVersionStr = "10.0.0";
var flashvars = { file:escape("$swfURL"),image:escape("$swfImageURL"),autostart:'false' };
var params = { allowfullscreen:'false', allowscriptaccess:'false' };
var attributes = { id:'AudioViewer', name:'AudioViewer' };
swfobject.embedSWF('/sites/all/libraries/mediaplayer4/player.swf','flashContent','400','200','9.0.115','false',flashvars, params, attributes);
swfobject.createCSS("#flashContent", "display:block;text-align:left;");
ENDJS;
    drupal_add_js($js_string, 'inline', 'header');

    $collection_fieldset = array(
      '#title' => t('Audio'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#value' => $content);
    return theme('fieldset', $collection_fieldset);
  }

}