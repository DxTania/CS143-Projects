<!doctype html>
<!--[if IE 9]><html class="lt-ie10" lang="en" > <![endif]-->
<html class="no-js" lang="en" data-useragent="Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)">

<head>
  <meta charset="utf-8"/>
  <title>TheMovieDB - Movies</title>
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
    <h3>Movies</h3>
    <?php
    $mysqli = new mysqli("localhost", "cs143", "", "CS143");
    if ($mysqli->connect_errno) {
      echo "Database error";
    } else {
      $stmt = $mysqli->prepare("SELECT id, title, year FROM Movie ORDER BY title LIMIT ?, 20");
      $page = isset($_GET['page']) ? (int) $_GET['page'] : 0;
      $lim_start = $page * 20;
      $stmt->bind_param("i", $lim_start);
      if (!$stmt->execute()) {
        echo "Failure";
      } else {
        $stmt->bind_result($mid, $title, $year);
        while ($stmt->fetch()) {
          echo "• <a href='movie.php?id=$mid'>$title ($year)</a><br/>";
        }
        $stmt->close();
      }
    }
    ?>

    <br/>
    <?php if ($page > 0) { ?>
    <a href="?page=<?php echo $page - 1; ?>" class="button small left">Previous</a>
    <?php } ?>
    <a href="?page=<?php echo $page + 1; ?>" class="button small right">Next</a>
  </div>

  <div class="medium-3 large-3 pull-9 columns">
    <form action="../browse/search.php" method="get">
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
      <li class="active"><a href="movies.php">Browse Movies</a></li>
      <li><a href="actors.php">Browse Actors</a></li>
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
