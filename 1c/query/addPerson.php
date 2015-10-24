<?php

$mysqli = new mysqli("localhost", "cs143", "", "CS143");
if ($mysqli->connect_errno) {
  echo "Database error";
} else {
  add_person($mysqli);
}
$mysqli->close();

/**
 * Add actor or director based on POST parameters
 * @param $mysqli mysqli object
 */
function add_person($mysqli) {
  $first_name = $_POST['firstName'];
  $last_name = $_POST['lastName'];
  $identity = $_POST['identity'];
  $status = $_POST['status'];
  $dob = date("Y-m-d", strtotime($_POST['dob']));
  $dod = null;
  $sex = null;

  if (strcmp($status, 'dead') === 0) {
    $dod = date("Y-m-d", strtotime($_POST['dod']));
  }

  $id = get_next_person_id($mysqli); // TODO: first check if person name exists?

  if (strcmp($identity, 'director') === 0) {
    insert_director($mysqli, $id, $last_name, $first_name, $dob, $dod);

  } else if (strcmp($identity, 'actor') === 0){
    insert_actor($mysqli, $id, $last_name, $first_name, $_POST['sex'], $dob, $dod);

  } else {
    echo "Error";
  }

}

/**
 * Insert new actor into Actor table
 * @param $mysqli mysqli object
 */
function insert_actor($mysqli, $id, $last, $first, $sex, $dob, $dod) {
  if ($dod != null) {
    $stmt = $mysqli->prepare("INSERT INTO Actor(id, last, first, sex, dob, dod) VALUES($id, ?, ?, ?, ?, ?)");
    $stmt->bind_param("sssss", $last, $first, $sex, $dob, $dod);
  } else {
    $stmt = $mysqli->prepare("INSERT INTO Actor(id, last, first, sex, dob, dod) VALUES($id, ?, ?, ?, ?, NULL)");
    $stmt->bind_param("ssss", $last, $first, $sex, $dob);
  }

  if (!$stmt->execute()) {
    echo "Failure";
  } else {
    echo "$first $last was added as an actor!";
  }

  $stmt->close();
}

/**
 * Insert new director into Director table
 * @param $mysqli mysqli object
 */
function insert_director($mysqli, $id, $last, $first, $dob, $dod) {
  // TODO: deal with director id = actor id, unique name?
  if ($dod != null) {
    $stmt = $mysqli->prepare("INSERT INTO Director(id, last, first, dob, dod) VALUES($id, ?, ?, ?, ?)");
    $stmt->bind_param("ssss", $last, $first, $dob, $dod);
  } else {
    $stmt = $mysqli->prepare("INSERT INTO Director(id, last, first, dob, dod) VALUES($id, ?, ?, ?, NULL)");
    $stmt->bind_param("sss", $last, $first, $dob);
  }

  if (!$stmt->execute()) {
    echo "Failure";
  } else {
    echo "$first $last was added as a director!";
  }

  $stmt->close();
}

/**
 * @param $mysqli mysqli object
 * @return integer id of next person
 */
function get_next_person_id($mysqli) {
  $result = $mysqli->query("SELECT id FROM MaxPersonID");
  $id = $result->fetch_assoc()['id'] + 1;
  $stmt = $mysqli->prepare("UPDATE MaxPersonID SET id=$id");
  $stmt->execute();
  $stmt->close();
  return $id;
}