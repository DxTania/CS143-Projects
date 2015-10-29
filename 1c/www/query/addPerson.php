<?php
include "../common/json.php";

$mysqli = new mysqli("localhost", "cs143", "", "CS143");
if ($mysqli->connect_errno) {
  echo database_error();
} else {
  add_person($mysqli);
  $mysqli->close();
}

/**
 * Add actor or director based on POST parameters
 * @param $mysqli mysqli object
 */
function add_person($mysqli) {
  $first_name = $_POST['firstName'];
  $last_name = $_POST['lastName'];
  $identity = $_POST['identity'];
  $status = $_POST['status'];
  $dob = get_date($_POST['dob']);
  $dod = null;
  $sex = null;

  if (strcmp($status, 'dead') === 0) {
    $dod = get_date($_POST['dod']);
  }

  $id = get_next_person_id($mysqli);
  if ($id < 0) {
    echo database_error();
    return;
  }

  if (strcmp($identity, 'director') === 0) {
    insert_director($mysqli, $id, $last_name, $first_name, $dob, $dod);

  } else if (strcmp($identity, 'actor') === 0){
    insert_actor($mysqli, $id, $last_name, $first_name, $_POST['sex'], $dob, $dod);

  } else {
    echo create_result(-1, "Invalid identity");
  }

}

/**
 * Insert new actor into Actor table
 * @param $mysqli mysqli object
 */
function insert_actor($mysqli, $id, $last, $first, $sex, $dob, $dod) {
  if ($dod != null) {
    if ($stmt = $mysqli->prepare("INSERT INTO Actor(id, last, first, sex, dob, dod) VALUES($id, ?, ?, ?, ?, ?)")) {
      $stmt->bind_param("sssss", $last, $first, $sex, $dob, $dod);
    } else {
      echo database_error();
      return;
    }
  } else {
    if ($stmt = $mysqli->prepare("INSERT INTO Actor(id, last, first, sex, dob, dod) VALUES($id, ?, ?, ?, ?, NULL)")) {
      $stmt->bind_param("ssss", $last, $first, $sex, $dob);
    } else {
      echo database_error();
      return;
    }
  }

  if (!$stmt->execute()) {
    echo database_error();
  } else {
    echo create_result(1, "$first $last was added as an actor!");
  }

  $stmt->close();
}

/**
 * Insert new director into Director table
 * @param $mysqli mysqli object
 */
function insert_director($mysqli, $id, $last, $first, $dob, $dod) {
  if ($dod != null) {
    if ($stmt = $mysqli->prepare("INSERT INTO Director(id, last, first, dob, dod) VALUES($id, ?, ?, ?, ?)")) {
      $stmt->bind_param("ssss", $last, $first, $dob, $dod);
    } else {
      echo database_error();
      return;
    }
  } else {
    if ($stmt = $mysqli->prepare("INSERT INTO Director(id, last, first, dob, dod) VALUES($id, ?, ?, ?, NULL)")) {
      $stmt->bind_param("sss", $last, $first, $dob);
    } else {
      echo database_error();
      return;
    }
  }

  if (!$stmt->execute()) {
    echo database_error();
  } else {
    echo create_result(1, "$first $last was added as a director!");
  }

  $stmt->close();
}

/**
 * @param $mysqli mysqli object
 * @return integer id of next person
 */
function get_next_person_id($mysqli) {
  if ($result = $mysqli->query("SELECT id FROM MaxPersonID")) {
    $id = $result->fetch_assoc()['id'] + 1;
    if ($stmt = $mysqli->prepare("UPDATE MaxPersonID SET id=$id")) {
      $stmt->execute();
      $stmt->close();
      return $id;
    }
  }
  return -1;
}

/**
 * @param $date string mm/dd/YYYY format
 * @return datetime YYYY-mm-dd format
 */
function get_date($date) {
  $datetime = DateTime::createFromFormat('m/d/Y', $date);
  return $datetime->format('Y-m-d');
}