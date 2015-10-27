<!doctype html>
<!--[if IE 9]><html class="lt-ie10" lang="en" > <![endif]-->
<html class="no-js" lang="en" data-useragent="Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)">

<head>
  <meta charset="utf-8"/>
  <title>TheMovieDB - Add Review</title>
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
      $stmt = $mysqli->prepare("SELECT id, title FROM Movie WHERE id = ?");
      $stmt->bind_param("i", $_GET['mid']);
      if (!$stmt->execute()) {
        echo "Failure";
      } else {
        $stmt->bind_result($mid, $title);
        $stmt->fetch();
    ?>
        <h3>Review for <?php echo $title ?></h3>

        <form data-abide="ajax" id="review-form">
          <div class="row">
            <div class="large-12 columns">
              <label for="name">Name</label>
              <input id="name" type="text" name="name" placeholder="Your name" maxlength="20" required/>
              <small class="error">Name is required</small>
            </div>
          </div>

          <div class="row">
            <div class="large-3 small-3 columns">
              <label>Rating
                <select name="rating">
                  <option value="5">5</option>
                  <option value="4">4</option>
                  <option value="3">3</option>
                  <option value="2">2</option>
                  <option value="1">1</option>
                </select>
              </label>
            </div>
          </div>

          <div class="row">
            <div class="large-12 columns">
              <label for="comment">Review</label>
              <textarea id="comment" name="comment" placeholder="Your comment" maxlength="500" rows="10"></textarea>
            </div>
          </div>

          <input type="hidden" name="mid" value="<?php echo $mid?>"/>

          <button type="submit" class="right small">Add Review</button>
        </form>
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
      <li><a href="addPerson.html">Add Person</a></li>
      <li class="active"><a href="addMovie.php">Add Movie</a></li>
      <li><a href="addActorRelation.php">Add Actor Relation</a></li>
      <li><a href="addDirectorRelation.php">Add Director Relation</a></li>
      <li><a href="../browse/movies.php">Browse Movies</a></li>
      <li><a href="../browse/actors.php">Browse Actors</a></li>
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

  $('#review-form').on('valid.fndtn.abide', function () {
    $.post('../query/addReview.php', $('#review-form').serialize(), function(data) {
      alert(data);
      window.location.href = '../index.html';
    });
  });

  $('input, textarea, select').off('.abide').on('blur.fndtn.abide change.fndtn.abide', function (e) {
    // do nothing
  });
</script>
</body>
</html>
