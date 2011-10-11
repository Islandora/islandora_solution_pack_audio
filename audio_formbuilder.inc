<?php

module_load_include('inc', 'islandora_repository', 'plugins/FormBuilder');
/*
 * implements methods from content model ingest form xml
 * builds a dc metadata form
 */

class AudioFormBuilder extends FormBuilder {

  function AudioFormBuilder() {
    module_load_include('inc', 'Fedora_Repository', 'plugins/FormBuilder');
  }

  /*
   * method overrides method in FormBuilder.
   */

  function createFedoraDataStreams($form_values, &$dom, &$rootElement) {
    module_load_include('inc', 'islandora_repository', 'MimeClass');
    global $base_url;
    $mimetype = new MimeClass();
    $server = null;
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
