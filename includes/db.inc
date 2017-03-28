<?php

/**
 * @file
 * Database functions.
 */

/**
 * Adds metadata from CSV to database table
 * @param array $result
 *   Associative array of metadata values.
 */
function searchpei_add_file($path) {
  $dirs = explode('/', $path);
  $filename = array_pop($dirs);
  $collection = array_pop($dirs);
  $item_id = db_merge('searchpei_imports')
      ->key(array('filename' => $filename, 'collection' => $collection))
      ->fields(array(
        'filename' => $filename,
        'collection' => $collection ? $collection : '',
      ))
      ->execute();
}
