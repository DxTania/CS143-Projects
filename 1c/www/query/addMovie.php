<?php
include "../common/json.php";

$mysqli = new mysqli("localhost", "cs143", "", "CS143");
if ($mysqli->connect_errno) {
  echo database_error();
} else {
  add_movie($mysqli);
  $mysqli->close();
}

/**
 * Add movie to Movie table
 *
 * @param $mysqli mysqli object
 */
function add_movie($mysqli) {
  $id = get_next_movie_id($mysqli);
  if ($id < 0) {
    echo database_error();
    return;
  }

  $title = $_POST['title'];
  $year = $_POST['year']; // TODO: not null in create ?
  $rating = $_POST['rating'];
  $company = $_POST['company'];
  $genres = isset($_POST['genres']) ? $_POST['genres'] : array();

  if ($stmt = $mysqli->prepare("INSERT INTO Movie(id, title, year, rating, company) VALUES($id, ?, ?, ?, ?)")) {
    $stmt->bind_param("siss", $title, $year, $rating, $company);

    if (!$stmt->execute()) {
      echo database_error();
    } else {
      echo create_result(1, "$title was added!");
    }

    $stmt->close();

    foreach ($genres as $genre) {
      if ($stmt = $mysqli->prepare("INSERT INTO MovieGenre(mid, genre) VALUES($id, ?)")) {
        $stmt->bind_param("s", $genre);
        $stmt->execute();
        $stmt->close();
      }
    }
  } else {
    echo database_error();
  }
}

/**
 * @param $mysqli mysqli object
 * @return integer id of next movie
 */
function get_next_movie_id($mysqli) {
  if ($result = $mysqli->query("SELECT id FROM MaxMovieID")) {
    $id = $result->fetch_assoc()['id'] + 1;
    if ($stmt = $mysqli->prepare("UPDATE MaxMovieID SET id=$id")) {
      $stmt->execute();
      $stmt->close();
      return $id;
    }
  }

  return -1;
}