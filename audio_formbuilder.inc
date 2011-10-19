<?php

/**
 * @file 
 * Audio form builder classes
 */
module_load_include('inc', 'fedora_repository', 'plugins/FormBuilder');

/**
 * AudioFormBuilder class
 * Implements methods from content model ingest form xml 
 * builds a dc metadata form
 */
class AudioFormBuilder extends FormBuilder {

  /**
   * Constructor
   */
  function AudioFormBuilder() {
    module_load_include('inc', 'Fedora_Repository', 'plugins/FormBuilder');
  }

  /**
   * Override Create Fedora Data Stream
   * @global type $base_url
   * @param type $form_values
   * @param type $dom
   * @param type $rootElement 
   */
  function createFedoraDataStreams($form_values, &$dom, &$rootElement) {
    module_load_include('inc', 'fedora_repository', 'api/MimeClass');
    global $base_url;
    $mimetype = new MimeClass();
    $server = NULL;
    $file = $form_values['ingest-file-location'];
    $dformat = $mimetype->getType($file);
    //$fileUrl = 'http://'.$_SERVER['HTTP_HOST'].$file;
    $fileUrl = $base_url . '/' . drupal_urlencode($file);
    $beginIndex = strrpos($fileUrl, '/');
    $dtitle = substr($fileUrl, $beginIndex + 1);
    $dtitle = substr($dtitle, 0, strpos($dtitle, "."));
    $ds1 = $dom->createElement("foxml:datastream");
    $ds1->setAttribute("ID", "OBJ");
    $ds1->setAttribute("STATE", "A");
    $ds1->setAttribute("CONTROL_GROUP", "M");
    $ds1v = $dom->createElement("foxml:datastreamVersion");
    $ds1v->setAttribute("ID", "OBJ.0");
    $ds1v->setAttribute("MIMETYPE", "$dformat");
    $ds1v->setAttribute("LABEL", "$dtitle");
    $ds1content = $dom->createElement('foxml:contentLocation');
    $ds1content->setAttribute("REF", "$fileUrl");
    $ds1content->setAttribute("TYPE", "URL");
    $ds1->appendChild($ds1v);
    $ds1v->appendChild($ds1content);
    $rootElement->appendChild($ds1);
  }

}

?>
