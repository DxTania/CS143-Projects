<?php

$title = $_POST['title'];
$year = $_POST['year'];
$rating = $_POST['rating']; // TODO: not null
$company = $_POST['company'];
$genres = isset($_POST['genres']) ? $_POST['genres'] : array();

$mysqli = new mysqli("localhost", "cs143", "", "CS143");
$id = 5002; // TODO: Get from MaxMovieId table

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

$mysqli->close();