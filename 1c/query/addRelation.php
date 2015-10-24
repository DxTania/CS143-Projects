<?php

$mysqli = new mysqli("localhost", "cs143", "", "CS143");
add_relation($mysqli);
$mysqli->close();

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
    echo 'Error';
  }
}

/**
 * Insert into MovieActor
 *
 * @param $mysqli mysqli object
 */
function add_actor_relation($mysqli, $mid, $aid, $role) {
  $stmt = $mysqli->prepare("INSERT INTO MovieActor(mid, aid, role) VALUES(?, ?, ?)");
  $stmt->bind_param("iis", $mid, $aid, $role);

  if (!$stmt->execute()) {
    echo "Failure";
  } else {
    echo "Actor was added!";
  }
}

/**
 * Insert into MovieDirector
 *
 * @param $mysqli mysqli object
 */
function add_director_relation($mysqli, $mid, $did) {
  $stmt = $mysqli->prepare("INSERT INTO MovieDirector(mid, did) VALUES(?, ?)");
  $stmt->bind_param("ii", $mid, $did);

  if (!$stmt->execute()) {
    echo "Failure";
  } else {
    echo "Director was added!";
  }
}