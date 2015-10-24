<!doctype html>
<!--[if IE 9]><html class="lt-ie10" lang="en" > <![endif]-->
<html class="no-js" lang="en" data-useragent="Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)">

<head>
  <meta charset="utf-8"/>
  <title>TheMovieDB - Add Relation</title>
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
    <h3>Add Relation</h3>

    Help us keep TheMovieDB up to date! Enter new information about director relations here.<br/><br/>

    <form data-abide="ajax" id="director-form">
      <div class="row">
        <div class="large-12 columns">
          <label>Movie
            <select name="movie">
              <?php
              $mysqli = new mysqli("localhost", "cs143", "", "CS143");
              $movies = $mysqli->query("SELECT id, title, year FROM Movie ORDER BY title");

              while($row = $movies->fetch_assoc()) {
                $id = $row['id'];
                $title = $row['title'];
                $year = $row['year'];
                echo "<option value='$id'>$title ($year)</option>";
              }
              $mysqli->close();
              ?>
            </select>
          </label>
        </div>
      </div>

      <div class="row">
        <div class="large-12 columns">
          <label>Director
            <select name="director">
              <?php
              $mysqli = new mysqli("localhost", "cs143", "", "CS143");
              $directors = $mysqli->query("SELECT id, first, last, dob FROM Director ORDER BY first, last");

              while($row = $directors->fetch_assoc()) {
                $id = $row['id'];
                $first = $row['first'];
                $last = $row['last'];
                $dob = $row['dob'];
                echo "<option value='$id'>$first $last ($dob)</option>";
              }
              $mysqli->close();
              ?>
            </select>
          </label>
        </div>
      </div>

      <input type="hidden" name="type" value="director"/>

      <button type="submit" class="right small">Add Relation</button>
    </form>
  </div>

  <div class="medium-3 large-3 pull-9 columns">
    <div class="row collapse">
      <div class="small-9 columns">
        <input type="text" placeholder="Search">
      </div>
      <div class="small-3 columns">
        <a href="#" class="button postfix">Go</a>
      </div>
    </div>

    <ul class="side-nav">
      <li><a href="../">Home</a></li>
      <li><a href="addPerson.html">Add Person</a></li>
      <li><a href="addMovie.php">Add Movie</a></li>
      <li><a href="addActorRelation.php">Add Actor Relation</a></li>
      <li class="active"><a href="addDirectorRelation.php">Add Director Relation</a></li>
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

  $('#director-form').on('valid.fndtn.abide', function () {
    $.post( "addRelation.php", $( "#director-form" ).serialize(), function(data) {
      alert(data); // TODO: go to show movie page
    });
  });

  $('input, textarea, select').off('.abide').on('blur.fndtn.abide change.fndtn.abide', function (e) {
    // do nothing
  });
</script>
</body>
</html>
