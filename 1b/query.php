<html>
<h3>Project 1B</h3>
<i>By Tania DePasquale</i>
<br/><br/>

<form action="#" method="GET">
  <textarea name="query" cols="60" rows="8"></textarea>
  <br/>
  <button type="submit">Submit</button>
</form>

<?php

if (isset($_GET["query"])) {
  $query = $_GET["query"];
  echo "<hr>Query: $query<hr>";

  $mysqli = new mysqli("localhost", "cs143", "", "CS143");
  $result = $mysqli->query($query);

  $rows= array();
  while($row = $result->fetch_assoc()) {
    $rows[] = $row;
  }

  # list out column header names using first result
  $columns = array_keys($rows[0]);
  echo "<table border='1'><tr>";
  foreach($columns as $col) {
    echo "<th>$col</th>";
  }
  echo "</tr>";

  # list data in table
  foreach($rows as $row) {
    echo "<tr>";
    foreach($columns as $col) {
      echo "<td>$row[$col]</td>";
    }
    echo "</tr>";
  }

  echo "</table>";

  $result->close();
  $mysqli->close();
}

?>

</html>
