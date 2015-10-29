<?php

function create_result($status, $message) {
  return json_encode(array('status' => $status, 'message' => $message));
}

function database_error() {
  return json_encode(array('status' => -1, 'message' => 'Database error.'));
}