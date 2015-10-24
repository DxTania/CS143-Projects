<!doctype html>
<!--[if IE 9]><html class="lt-ie10" lang="en" > <![endif]-->
<html class="no-js" lang="en" data-useragent="Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)">

<head>
  <meta charset="utf-8"/>
  <title>TheMovieDB - View Movie</title>
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
    <?php
    $mysqli = new mysqli("localhost", "cs143", "", "CS143");
    if ($mysqli->connect_errno) {
      echo "Database error";
    } else {
      $stmt = $mysqli->prepare("SELECT id, title, year, rating, company FROM Movie WHERE id = ?");
      $stmt->bind_param("i", $_GET['id']);
      if (!$stmt->execute()) {
        echo "Failure";
      } else {
        $stmt->bind_result($mid, $title, $year, $rating, $company);
        $stmt->fetch();
        $stmt->close();
      ?>
        <span class="right rating"><?php echo $rating?></span>
        <h3><?php echo "$title ($year)" ?></h3>
        <b>Producer</b>: <?php echo $company ?><br/>
        <b>Directors</b>:
            <?php
            $stmt = $mysqli->prepare("SELECT first, last, dob
                                      FROM Director, MovieDirector
                                      WHERE mid = ? AND id = did");
            $stmt->bind_param("i", $mid);
            if (!$stmt->execute()) {
              echo "Failure";
            } else {
              $stmt->bind_result($first, $last, $dob);
              $result = '';
              while ($stmt->fetch()) {
                $result .= "$first $last ($dob), ";
              }
              echo substr($result, 0, strlen($result) - 2);
              $stmt->close();
            }
            ?>
        <br/>
        <b>Genres</b>:
        <?php
        $stmt = $mysqli->prepare("SELECT genre FROM MovieGenre WHERE mid = ?");
        $stmt->bind_param("i", $mid);
        if (!$stmt->execute()) {
          echo "Failure";
        } else {
          $stmt->bind_result($genre);
          $result = '';
          while ($stmt->fetch()) {
            $result .= "$genre, ";
          }
          echo substr($result, 0, strlen($result) - 2);
          $stmt->close();
        }
        ?>

        <br/><br/>
        <b>Cast</b><br/>
        <?php
        $stmt = $mysqli->prepare("SELECT aid, first, last, role
                                  FROM Actor, MovieActor
                                  WHERE id = aid AND mid = ?");
        $stmt->bind_param("i", $mid);
        if (!$stmt->execute()) {
          echo "Failure";
        } else {
          $stmt->bind_result($aid, $first, $last, $role);
          while ($stmt->fetch()) {
            echo "• <a href='actor.php?id=$aid'>$first $last</a> as \"$role\"<br/>";
          }
          $stmt->close();
        }
        ?>

        <br/>

        <?php
        $stmt = $mysqli->prepare("SELECT AVG(rating) FROM Review WHERE mid = ?");
        $stmt->bind_param("i", $mid);
        if (!$stmt->execute()) {
          echo "Failure";
        } else {
          $stmt->bind_result($avg);
          $stmt->fetch();
          $stmt->close();
          ?>
          <span class="right"><b>Avg Rating</b>:
          <?php
          if ($avg == null) {
            echo "N/A";
          } else {
            echo "$avg / 5";
          }
        }
        ?></span>
        <b>User Reviews</b><br/><br/>

        <?php
        $stmt = $mysqli->prepare("SELECT name, time, rating, comment
                                  FROM Review
                                  WHERE mid = ?
                                  ORDER BY time DESC");
        $stmt->bind_param("i", $mid);
        if (!$stmt->execute()) {
          echo "Failure";
        } else {
          $stmt->bind_result($name, $time, $usr_rating, $comment);
          while ($stmt->fetch()) {
            $datetime = date( 'F d, Y g:i A', strtotime($time));
            for ($i = 0; $i < $usr_rating; $i++) {
              echo "★";
            }
            echo "<br/>$datetime by $name<br/><br/>$comment<br/><br/><hr>";
          }
          $stmt->close();
        }
        ?>

        <a href="../add/addReview.php?mid=<?php echo $mid ?>" class="button small right">
          Add Review
        </a>

      <?php } } ?>

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