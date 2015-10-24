<?php
$mysqli = new mysqli("localhost", "cs143", "", "CS143");
// TODO: error if db connection fails
add_movie($mysqli);
$mysqli->close();

/**
 * Add movie to Movie table
 *
 * @param $mysqli mysqli object
 */
function add_movie($mysqli) {
  $id = get_next_movie_id($mysqli);
  $title = $_POST['title'];
  $year = $_POST['year']; // TODO: not null in create ?
  $rating = $_POST['rating'];
  $company = $_POST['company'];
  $genres = isset($_POST['genres']) ? $_POST['genres'] : array();

  $stmt = $mysqli->prepare("INSERT INTO Movie(id, title, year, rating, company) VALUES($id, ?, ?, ?, ?)");
  $stmt->bind_param("siss", $title, $year, $rating, $company);

  if (!$stmt->execute()) {
    echo "Failure";
  } else {
    echo "$title was added!";
  }

  $stmt->close();

  foreach ($genres as $genre) {
    $stmt = $mysqli->prepare("INSERT INTO MovieGenre(mid, genre) VALUES($id, ?)");
    $stmt->bind_param("s", $genre);
    $stmt->execute();
    $stmt->close();
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
  $stmt->close();
  return $id;
}