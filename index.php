
<?php

require_once('include.inc');

if (isset($_COOKIE['avoid_block'])) {
    echo ($_COOKIE['avoid_block'] - time()) . ' másodperc  újra elérhető lesz';
}
else {
  
    if (isset($_POST['submit'])) {
      setcookie('avoid_block', time() + 10, time()+ 10);
      $data = curl_get_contents($_POST['url']);
      
      preg_match('/cBox-body--technical-data.*?>(.*?)mt-test-reports-link__container/sm', $data, $desc);
      $text = $desc[1];

      $translated = strip_tags(str_replace('</div>', '###', translate($text)));
      
      $parts = explode("###", $translated);

      $formatted = '';
      foreach ($parts as $part) {
        $formatted .= '<text:p text:style-name="Standard"><text:s/>' . $part . '</text:p>';
      }

      createODT($formatted);

      echo '<a href="./ready.odt">Letöltés</a>';
      echo '<a href="#" onclick="javascript:self.location=self.location">Újratöltés</a>';
      
    }
    else {
        ?>
      <form method="POST">
        URL:<br />
        <input type="text" name="url" size="100"><br />
        <input type="submit" value="translate" name="submit"><br />
      </form>
      <?php
    
  }

}
    
?>