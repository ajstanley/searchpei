<?php

/**
 * Form to define parameters for moving volumes.
 *
 * @param array $form
 *   Drupal Form
 * @param array $form_state
 *   Drupal from state
 *
 * @return array
 *   Drupal form
 */
function searchpei_import_data_form($form, &$form_state) {
  $form = array();
  $form['source_dir'] = array(
    '#type' => 'textfield',
    '#title' => t("Volume Directory"),
    '#description' => t('Full path to volume directory'),
    '#default_value' => '/mnt/tmp/titlesearchpeipc6mqnhxfx/',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Ingest Volume"),
  );
  return $form;
}

/**902 450 1221
 * Validation handler.
 *
 * @param array $form
 *   Drupal Form
 * @param array $form_state
 *   Drupal form state
 */
function searchpei_import_data_form_validatex($form, &$form_state) {
  if (!is_dir($form_state['values']['source_dir'])) {
    form_set_error('source_dir', t('Not a valid file path.'));
  }
}

/**
 * Submit handler.
 *
 * @param array $form
 *   Drupal Form
 * @param array $form_state
 *   Drupal form state
 */
function searchpei_import_data_form_submit($form, &$form_state) {
  module_load_include('inc', 'searchpei', 'includes/utilities');
  module_load_include('inc', 'searchpei', 'includes/db');
//  $vocab = taxonomy_vocabulary_machine_name_load('collections');
//  $vid = $vocab->vid;
//  $files = searchpei_get_all_by_extension($form_state['values']['source_dir'], array('pdf'));
//  $lines = file($form_state['values']['source_dir']);
//  foreach ($lines as $line) {
//    searchpei_add_file($line);
//  }
//  $path_parts = explode('/', $form_state['values']['source_dir']);
//  $term_name = array_pop($path_parts);
//  $term = taxonomy_get_term_by_name($term_name);
//  if ($term) {
//    $tid = key($term);
//  }
//  else {
//    $new_term = (object) array(
//          'name' => $term_name,
//          'description' => 'Collection from Source',
//          'vid' => $vid,
//    );
//    taxonomy_term_save($new_term);
//    $tid = $new_term->tid;
//  }

  $pdfs = searchpei_get_all_by_extension($form_state['values']['source_dir'], 'pdf');
  $batch = array(
    'title' => t('Adding PDFs to Website'),
    'operations' => array(),
    'file' => drupal_get_path('module', 'searchpei') . '/includes/utilities.inc',
    'progress_message' => t('@current of @total PDFs ingested.'),
  );

  foreach ($pdfs as $pdf) {
    $batch['operations'][] = array('searchpei_build_entity', array(
        'file' => $pdf,
      )
    );
  }
  batch_set($batch);
  batch_process();
}

function searchpei_build_taxonomy() {
  $lines = file($form_state['values']['source_dir']);
  foreach ($lines as $line) {
    $dirs = explode('/', $line);
    array_unshift($dirs, '');
    foreach ($dirs as $index => $dir) {
      if ($index && $index < count($dirs) - 1) {
        $layers[$index][$dirs[$index]] = $dirs[$index - 1];
      }
    }
  }
  $file = fopen("/tmp/islandsearchtaxonomony.csv", 'a');
  fwrite($file, "PARENT,TERM_NAME \n");
  foreach ($layers as $index => $layer) {
    foreach ($layer as $term => $parent) {
      $entry = "$parent,$term\n";
      fwrite($file, "$parent,$term\n");
    }
  }
  fclose($file);
  return;
}
$dir = '/mnt/tmp/titlesearchpeipc6mqnhxfx/testbatch';
module_load_include('inc', 'searchpei', 'includes/utilities');
  module_load_include('inc', 'searchpei', 'includes/db');
 // $pdfs = searchpei_get_all_by_extension($dir , 'pdf');
//dsm($pdfs);