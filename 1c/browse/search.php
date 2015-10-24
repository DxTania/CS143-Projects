<!doctype html>
<!--[if IE 9]><html class="lt-ie10" lang="en" > <![endif]-->
<html class="no-js" lang="en" data-useragent="Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)">

<head>
  <meta charset="utf-8"/>
  <title>TheMovieDB - Search Results</title>
  <link rel="stylesheet" href="../css/foundation.css"/>
  <script src="../js/vendor/modernizr.js"></script>
</head>

<body>

<div class="row">
  <div class="large-12 columns">
    <h1><img src="../img/logo.png"/></h1>
  </div>
</div>

<div class="row">
  <div class="medium-9 large-9 push-3 columns">
    <h3>Search Results</h3>

    <div class="row">
      <div class="large-6 columns">
        <h5><b>Actors</b></h5>
        <?php
        $mysqli = new mysqli("localhost", "cs143", "", "CS143");
        $query = $mysqli->real_escape_string($_GET['query']);
        $words = explode(" ", $query);
        $conditions = '';
        foreach ($words as $word) {
          $conditions = $conditions."(first LIKE '%{$word}%' OR last LIKE '%{$word}%') AND";
        }
        $conditions = $conditions." TRUE";

        $actors = $mysqli->query("SELECT first, last, dob
                                  FROM Actor
                                  WHERE $conditions
                                  ORDER BY first, last");

        while($row = $actors->fetch_assoc()) {
          $first = $row["first"];
          $last = $row["last"];
          $dob = $row["dob"];
          echo "$first $last ($dob)<br/>"; // TODO: make these links
        }

        ?>

      </div>
      <div class='large-6 columns'>
        <h5><b>Movies</b></h5>

        <?php

        $conditions = '';
        foreach ($words as $word) {
          $conditions = $conditions."(title LIKE '%{$word}%') AND";
        }
        $conditions = $conditions." TRUE";

        $movies = $mysqli->query("SELECT title, year
                                  FROM Movie
                                  WHERE $conditions
                                  ORDER BY title");

        while($row = $movies->fetch_assoc()) {
          $title = $row["title"];
          $year = $row["year"];
          echo "$title ($year)<br/>"; // TODO: make these links
        }

        $mysqli->close();
        ?>
      </div>
    </div>
  </div>

  <div class="medium-3 large-3 pull-9 columns">
    <form action="search.php" method="get">
      <div class="row collapse">
        <div class="small-9 columns">
          <input type="text" placeholder="Search" name="query">
        </div>
        <div class="small-3 columns">
          <button type="submit" class="button postfix">Go</button>
        </div>
      </div>
    </form>

    <ul class="side-nav">
      <li><a href="../">Home</a></li>
      <li><a href="../add/addPerson.html">Add Person</a></li>
      <li><a href="../add/addMovie.php">Add Movie</a></li>
      <li><a href="../add/addActorRelation.php">Add Actor Relation</a></li>
      <li><a href="../add/addDirectorRelation.php">Add Director Relation</a></li>
      <li><a href="#">Browse Movies</a></li>
      <li><a href="#">Browse Actors</a></li>
    </ul>
  </div>
</div>

<footer class="row">
  <div class="large-12 columns">
    <hr/>
    <div class="row">
      <div class="large-6 columns">
        <p>&copy; Copyright no one at all. Go to town.</p>
      </div>
      <div class="large-6 columns">
      </div>
    </div>
  </div>
</footer>

<script src="../js/vendor/jquery.js"></script>
<script src="../js/foundation.min.js"></script>
<script>
  $(document).foundation();

  $('input, textarea, select').off('.abide').on('blur.fndtn.abide change.fndtn.abide', function (e) {
    // do nothing
  });
</script>
</body>
</html>