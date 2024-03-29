<?php
include "../common/json.php";

$mysqli = new mysqli("localhost", "cs143", "", "CS143");
if ($mysqli->connect_errno) {
  echo database_error();
} else {
  add_relation($mysqli);
  $mysqli->close();
}

/**
 * Add actor or director relation based on type parameter in post
 *
 * @param $mysqli mysqli object
 */
function add_relation($mysqli) {
  $mid = $_POST['movie'];
  $type = $_POST['type'];

  if (strcmp($type, 'actor') === 0) {
    $aid = $_POST['actor'];
    $role = $_POST['role'];
    add_actor_relation($mysqli, $mid, $aid, $role);

  } else if (strcmp($type, 'director') === 0) {
    $did = $_POST['director'];
    add_director_relation($mysqli, $mid, $did);

  } else {
    echo create_result(-1, "Invalid type");
  }
}

/**
 * Insert into MovieActor
 *
 * @param $mysqli mysqli object
 */
function add_actor_relation($mysqli, $mid, $aid, $role) {
  if ($stmt = $mysqli->prepare("INSERT INTO MovieActor(mid, aid, role) VALUES(?, ?, ?)")) {
    $stmt->bind_param("iis", $mid, $aid, $role);

    if (!$stmt->execute()) {
      echo database_error();
    } else {
      echo create_result(1, "Actor was added!");
    }

    $stmt->close();
  } else {
    echo database_error();
  }
}

/**
 * Insert into MovieDirector
 *
 * @param $mysqli mysqli object
 */
function add_director_relation($mysqli, $mid, $did) {
  if ($stmt = $mysqli->prepare("INSERT INTO MovieDirector(mid, did) VALUES(?, ?)")) {
    $stmt->bind_param("ii", $mid, $did);

    if (!$stmt->execute()) {
      echo database_error();
    } else {
      echo create_result(1, "Director was added!");
    }

    $stmt->close();
  } else {
    echo database_error();
  }
}