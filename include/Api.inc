<?php

class Api
{
  public static function call($url, $postData = array())
  {

    $defaults = array(
      CURLOPT_URL     => $url,
      CURLOPT_PORT    => 80,
      CURLOPT_SSL_VERIFYPEER => FALSE,
      CURLOPT_TIMEOUT => 120,
      CURLOPT_RETURNTRANSFER => TRUE,
    );

    // The method is POST
    if( count($postData) > 0)
    {
      $defaults[CURLOPT_POST] = TRUE;
      $defaults[CURLOPT_POSTFIELDS] = $postData;
    }
    else
    {
      $defaults[CURLOPT_HEADER] = FALSE;
      $defaults[CURLOPT_RETURNTRANSFER] = TRUE;
    }

    // Starting cURL
    $channel  = curl_init();
    curl_setopt_array($channel, $defaults);
    $data     = curl_exec($channel);
    $curlInfo = curl_getinfo($channel);

    $response = new stdClass();

    if( $data === FALSE)
    {
      $response->error  = curl_error($channel);
      $response->info   = $curlInfo;
    }
    else
    {
      $response->error  = '';
      $response->info   = $curlInfo;
    }

    $response->code = $curlInfo['http_code'];
    $response->data = $data;

    curl_close($channel);

    return $response;
  }
}
