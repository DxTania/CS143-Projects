<!doctype html>
<!--[if IE 9]><html class="lt-ie10" lang="en" > <![endif]-->
<html class="no-js" lang="en" data-useragent="Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)">

<head>
  <meta charset="utf-8"/>
  <title>TheMovieDB - Add Movie</title>
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
    <h3>Add Movie</h3>

    Help us keep TheMovieDB up to date! Enter new information about movies here.<br/><br/>

    <form data-abide="ajax" id="movie-form">
      <div class="row">
        <div class="large-9 columns end">
          <label for="movie-title">Movie Title</label>
          <input id="movie-title" type="text" name="title" placeholder="Title" maxlength="100" required/>
          <small class="error">Movie title is required</small>
        </div>
      </div>
      <div class="row">
        <div class="large-3 small-3 columns">
          <label for="release-year">Release Year</label>
          <input id="release-year" type="text" name="year" pattern="^[0-9][0-9][0-9][0-9]$" maxlength="4" placeholder="YYYY"/>
          <small class="error">Release year must be valid</small>
        </div>
      </div>
      <div class="row">
        <div class="large-3 small-3 columns">
          <label>MPAA Rating
            <select name="rating">
              <?php
                $mysqli = new mysqli("localhost", "cs143", "", "CS143");
                $ratings = $mysqli->query("SELECT DISTINCT rating FROM Movie ORDER BY rating");

                while($row = $ratings->fetch_assoc()) {
                  $rating = $row["rating"];
                  echo "<option value='$rating'>$rating</option>";
                }
              ?>
            </select>
          </label>
        </div>
      </div>
      <div class="row">
        <div class="large-9 columns end">
          <label for="company">Production Company</label>
          <input id="company" type="text" name="company" placeholder="Company name" maxlength="50"/>
        </div>
      </div>
      <div class="row">
        <div class="large-9 columns end">
          <label>Genres</label>
          <?php
          $mysqli = new mysqli("localhost", "cs143", "", "CS143");
          $genres = $mysqli->query("SELECT DISTINCT genre FROM MovieGenre ORDER BY genre");

          while($row = $genres->fetch_assoc()) {
            $genre = $row["genre"];
            echo "<div class='inline-block'>
                    <input type='checkbox' name='genres[]' value='$genre' id='$genre'/>
                    <label for='$genre'>$genre</label>
                  </div>";
          }
          ?>
        </div>
      </div>
      <button type="submit" class="right small">Add Movie</button>
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
      <li class="active"><a href="addMovie.php">Add Movie</a></li>
      <li><a href="#">Add Relation</a></li>
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

<script>
  document.write('<script src=js/vendor/' +
      ('__proto__' in {} ? 'zepto' : 'jquery') +
      '.js><\/script>')
</script>
<script src="../js/vendor/jquery.js"></script>
<script src="../js/foundation.min.js"></script>
<script>
  $(document).foundation();

  $('#movie-form').on('valid.fndtn.abide', function () {
    $.post( "addMoviePost.php", $( "#add-form" ).serialize(), function(data) {
      alert(data); // TODO: go to show movie page
    });
  });

  $('input, textarea, select').off('.abide').on('blur.fndtn.abide change.fndtn.abide', function (e) {
    // do nothing
  });
</script>
</body>
</html>
