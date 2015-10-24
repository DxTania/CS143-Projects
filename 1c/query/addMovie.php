<?php
$mysqli = new mysqli("localhost", "cs143", "", "CS143");
// TODO: error if db connection fails
add_movie($mysqli,
  $_POST['title'],
  $_POST['year'], // TODO: not null in create ?
  $_POST['rating'],
  $_POST['company'],
  isset($_POST['genres']) ? $_POST['genres'] : array());

$mysqli->close();

/**
 * Add move to Movie table
 * @param $mysqli mysqli object
 */
function add_movie($mysqli, $title, $year, $rating, $company, $genres) {
  $id = get_next_movie_id($mysqli);

  $stmt = $mysqli->prepare("INSERT INTO Movie(id, title, year, rating, company) VALUES($id, ?, ?, ?, ?)");
  $stmt->bind_param("siss", $title, $year, $rating, $company);

  if (!$stmt->execute()) {
    echo "Failure";
  } else {
    echo "$title was added!";
  }

  foreach ($genres as $genre) {
    $stmt = $mysqli->prepare("INSERT INTO MovieGenre(mid, genre) VALUES($id, ?)");
    $stmt->bind_param("s", $genre);
    $stmt->execute();
  }
}

/**
 * @param $mysqli mysqli object
 * @return integer id of next movie
 */
function get_next_movie_id($mysqli) {
  $result = $mysqli->query("SELECT id FROM MaxMovieID");
  $id = $result->fetch_assoc()['id'] + 1;
  $stmt = $mysqli->prepare("UPDATE MaxMovieID SET id=$id");
  $stmt->execute();
  return $id;
}