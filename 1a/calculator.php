<h3>Calculator</h3>
<i>By Tania DePasquale</i>
<br/><br/>

<form method="GET">
  <input type="text" name="expr">
  <button type="submit">Calculate</button>
</form>

<?php

if (isset($_GET["expr"]) && (!empty($_GET["expr"]) || $_GET["expr"] === '0')) {
	$expr = str_replace(' ', '', $_GET["expr"]);

	preg_match('/-?[0-9]+(\.[0-9]+)?([-\+\*\/]-?[0-9]+(\.[0-9]+)?)*/', $expr, $matches);

	if (strcmp($matches[0], $expr) == 0 && strpos($expr, '/0') === false) {
		$equ = str_replace('--', '- -', $matches[0]);
		eval("\$ans = $equ ;");
		echo "$equ = ".$ans;
	} else if (strpos($expr, '/0') != false) {
		echo "Division by zero";
	} else {
		echo "Invalid input expression";
	}
}

?>