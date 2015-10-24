<?php
$mysqli = new mysqli("localhost", "cs143", "", "CS143");
// TODO: error if db connection fails
add_review($mysqli);
$mysqli->close();

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

  $stmt = $mysqli->prepare("INSERT INTO Review(name, time, mid, rating, comment) VALUES(?, NOW(), ?, ?, ?)");
  $stmt->bind_param("siis", $name, $mid, $rating, $comment);

  if (!$stmt->execute()) {
    echo "Failure";
  } else {
    echo "Review was added!";
  }

  $stmt->close();
}
