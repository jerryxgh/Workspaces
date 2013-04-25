<option value="">choose color</option>
<?php
switch ($_REQUEST["style"]) {
	case "7141832": ?>
    <option value="bk">Black</option>
    <option value="br">Brown</option>
  <?php break; ?>
  <?php case '7173656': ?>
    <option value="bk">Black</option>
    <option value="br">Brown</option>
  <?php break; ?>
  <?php case '7141922': ?>
    <option value="bk">Black</option>
    <option value="br">Crazy Horse</option>
  <?php break; ?>
  <?php case '7177382': ?>
    <option value="hy">Honey</option>
    <option value="sd">Saddle</option>
  <?php break; ?>
  <?php case '7141730': ?>
    <option value="rw">Redwood</option>
    <option value="hy">Honey</option>
  <?php break; ?>
  <?php case '7141833': ?>
    <option value="bk">Black</option>
    <option value="br">Brown</option>
    <option value="rw">Redwood</option>
  <?php break; ?>
  <?php case '7257914': ?>
    <option value="rw">Brown</option>
  <?php break; ?>
  <?php case '7269643': ?>
    <option value="bk">Black</option>
  <?php break; ?>
  <?php default: ?>
    <option value="bk">Black</option>
<?php } ?>
