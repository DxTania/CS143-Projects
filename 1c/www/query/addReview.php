<?php
include "../common/json.php";

$mysqli = new mysqli("localhost", "cs143", "", "CS143");
if ($mysqli->connect_errno) {
  echo database_error();
} else {
  add_review($mysqli);
  $mysqli->close();
}

/**
 * Add review to Review table
 *
 * @param $mysqli mysqli object
 */
function add_review($mysqli) {
  $name = $_POST['name'];
  $mid = $_POST['mid'];
  $rating = $_POST['rating'];
  $comment = $_POST['comment'];

  if ($stmt = $mysqli->prepare("INSERT INTO Review(name, time, mid, rating, comment) VALUES(?, NOW(), ?, ?, ?)")) {
    $stmt->bind_param("siis", $name, $mid, $rating, $comment);

    if (!$stmt->execute()) {
      echo database_error();
    } else {
      echo create_result(1, "Review was added!");
    }

    $stmt->close();
  } else {
    echo database_error();
  }
}
