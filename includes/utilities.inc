<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Gets all files with a given extension in a recursively searched directory.
 *
 * @param string $dir_path
 *   Full path to directory
 * @param array $extensions
 *   array of allowable extensions
 *
 * @return array
 *   All files with supllied extensions.
 */
function searchpei_get_all_by_extension($dir_path, $extensions) {
  $dirs = array();
  $files = array();
  if (!is_array($extensions)) {
    $extensions = array($extensions);
  }
  foreach ($iterator = new RecursiveIteratorIterator(
  new RecursiveDirectoryIterator($dir_path, RecursiveDirectoryIterator::SKIP_DOTS), RecursiveIteratorIterator::SELF_FIRST) as $item) {
    $subpath = $iterator->getSubPathName();
    $ext = pathinfo($subpath, PATHINFO_EXTENSION);
    if (in_array($ext, $extensions)) {
      $files[] = "$dir_path/$subpath";
    }
  }
  sort($files);
  return $files;
}

function searchpei_build_vocabularies($paths) {
  foreach ($paths as $path) {
    $collection = array_pop($path_parts);
    foreach ($path_parts as $term) {
      
    }
  }
}

/**
 * Contructs PDF entitiy from known inputs
 *
 * @param type $file
 * @return type
 */
function searchpei_build_entity($file) {
  global $user;
  $path_parts = pathinfo($file);
  $file_name = $path_parts['basename'];
  $folder_name = 'searchpei';
  $nodes = array();
  preg_match_all('/\d+/', $file, $matches);
  if (isset($matches[0])) {
    foreach ($matches[0] as $match) {
      $new_nodes = node_load_multiple(array(), array('title' => $match), FALSE);
      $nodes = array_merge($nodes, $new_nodes);
    }
  }

  if (file_exists($file)) {
    // create a file object for Drupal
    $dfile = (object) array(
          'uri' => $file,
          'filemime' => file_get_mimetype($file),
          'status' => 1,
          'display' => 1,
          'decription' => t('Loaded'),
    );
    $uri = "public://$folder_name/$file_name";
    $drupalfile = file_copy($dfile, $uri, FILE_EXISTS_REPLACE);
    $fid = $drupalfile->fid;
    if (!$nodes) {
      $path_parts = explode('/', $file);
      $name = array_pop($path_parts);
      $entity_type = 'node';
      $entity = entity_create($entity_type, array('type' => 'title_search'));
      $wrapper = entity_metadata_wrapper($entity_type, $entity);
      $wrapper->title->set($name);
      $wrapper->author->set($user->uid);
      $wrapper->save();
      $nodes[] = $wrapper->value();
    }
    foreach ($nodes as $node) {
      $wrapper = entity_metadata_wrapper('node', $node);
      $associated = array(
        array('fid' => $fid, 'display' => TRUE)
      );
      $current = $wrapper->field_associated_files->value();
      foreach ($current as $cur) {
        $associated[] = array('fid' => $cur['fid'], 'display' => TRUE);
      }
      $wrapper->field_associated_files->set($associated);
      $wrapper->save();
    }
    return;
  }
}

function searchpei_get_address($title) {
  $nodes = node_load_multiple(NULL, array("title" => $title));
  if ($nodes) {
    $node = current($nodes);
    $nid = $node->nid;
    return drupal_get_path_alias("node/$nid");
  }
}
