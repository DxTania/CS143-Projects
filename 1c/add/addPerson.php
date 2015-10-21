<?php

$firstName=$_POST['firstName'];
$lastName=$_POST['lastName'];
$identity=$_POST['identity'];
$status=$_POST['status'];
$dob=date("Y-m-d", strtotime($_POST['dob']));
$dod = null;
$sex=null;

if (strcmp($status, 'dead') === 0) {
  $dod = date("Y-m-d", strtotime($_POST['dod']));
}

$mysqli = new mysqli("localhost", "cs143", "", "CS143");
$id=500019; // TODO: Get from MaxPersonID table? first check if person name exists?

if (strcmp($identity, 'director') === 0) {
  // TODO: deal with director id = actor id, unique name?

  if ($dod != null) {
    $stmt = $mysqli->prepare("INSERT INTO Director(id, last, first, dob, dod) VALUES(?, ?, ?, ?, ?)");
    $stmt->bind_param("issss", $id, $lastName, $firstName, $dob, $dod);
  } else {
    $stmt = $mysqli->prepare("INSERT INTO Director(id, last, first, dob, dod) VALUES(?, ?, ?, ?, NULL)");
    $stmt->bind_param("isss", $id, $lastName, $firstName, $dob);
  }

  if (!$stmt->execute()) {
    echo "Failure";
  } else {
    echo "$firstName $lastName was added as a director!";
  }

} else if (strcmp($identity, 'actor') === 0){
  $sex=$_POST['sex'];

  if ($dod != null) {
    $stmt = $mysqli->prepare("INSERT INTO Actor(id, last, first, sex, dob, dod) VALUES(?, ?, ?, ?, ?, ?)");
    $stmt->bind_param("isssss", $id, $lastName, $firstName, $sex, $dob, $dod);
  } else {
    $stmt = $mysqli->prepare("INSERT INTO Actor(id, last, first, sex, dob, dod) VALUES(?, ?, ?, ?, ?, NULL)");
    $stmt->bind_param("issss", $id, $lastName, $firstName, $sex, $dob);
  }

  if (!$stmt->execute()) {
    echo "Failure";
  } else {
    echo "$firstName $lastName was added as an actor!";
  }

} else {
  echo "Error";
}

