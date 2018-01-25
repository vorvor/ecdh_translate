<?php

function zip($source, $destination)
  {
    if (!extension_loaded('zip') || !file_exists($source)) {
        return false;
    }

    $zip = new ZipArchive();
    if (!$zip->open($destination, ZIPARCHIVE::CREATE)) {
        return false;
    }

    $source = str_replace('\\', '/', realpath($source));

    if (is_dir($source) === true)
    {
        $files = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($source), RecursiveIteratorIterator::SELF_FIRST);

        foreach ($files as $file)
        {
            $file = str_replace('\\', '/', $file);

            // Ignore "." and ".." folders
            if( in_array(substr($file, strrpos($file, '/')+1), array('.', '..')) )
                continue;

            $file = realpath($file);

            if (is_dir($file) === true)
            {
                $zip->addEmptyDir(str_replace($source . '/', '', $file . '/'));
            }
            else if (is_file($file) === true)
            {
                $zip->addFromString(str_replace($source . '/', '', $file), file_get_contents($file));
            }
        }
    }
    else if (is_file($source) === true)
    {
        $zip->addFromString(basename($source), file_get_contents($source));
    }

    return $zip->close();
}

function createODT($translated) {

    $data = file_get_contents('ecdh.odt');

    $zip = new ZipArchive;
    $res = $zip->open('ecdh.odt');
    if ($res === TRUE) {
      $zip->extractTo('./unzip/unzipped');
      $zip->close();

      
      $data = file_get_contents('./unzip/unzipped/content.xml');
      $data = str_replace('<text:p text:style-name="Standard"><text:s/>[%value-to-replace%]</text:p>', $translated, $data);

      file_put_contents('./unzip/unzipped/content.xml', $data);
      
      zip('./unzip/unzipped', './ready.odt'); 

      
    
      


    } 
    else {
      echo 'Zip Archive error';
    }
}

function translate($text) {
    $apiKey = 'AIzaSyDuKd3sYC5jGDOZC49hnt4XtGOH8uz7p34';
        
    $url = 'https://www.googleapis.com/language/translate/v2?key=' . $apiKey . '&q=' . rawurlencode($text) . '&source=de&target=hu';

    $handle = curl_init();

    curl_setopt($handle, CURLOPT_URL,'https://www.googleapis.com/language/translate/v2');
    curl_setopt($handle, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($handle, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($handle, CURLOPT_POSTFIELDS, array('key'=> $apiKey, 'q' => $text, 'source' => 'de', 'target' => 'hu'));
    curl_setopt($handle,CURLOPT_HTTPHEADER,array('X-HTTP-Method-Override: GET'));
    $response = curl_exec($handle);

    curl_close ($handle);

    $translation = json_decode($response);

    return $translation->data->translations[0]->translatedText;
}