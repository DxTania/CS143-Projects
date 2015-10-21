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

  if (strcmp($identity, 'director') === 0) {
    $sex=$_POST['sex'];
    $id=500009; // TODO: Get from MaxPersonID table?
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
      echo "$firstName $lastName was added $dob $dod!";
    }
  } else {
    echo "$identity, $status, $firstName, ";
  }

