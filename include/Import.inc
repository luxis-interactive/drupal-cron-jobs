<?php

final class Import extends Api
{
  /**
   * Get cars from API
   *
   * @return Object
  **/
  private static function getCarsFromApi()
  {
    $url = 'http://dev-prueba-luxis.pantheon.io/cars';

    $response = self::call($url);

    if( $response->code != 200)
    {
      throw new Exception("API unresponsive: {$response->error}");
    }

    return json_decode($response->data);
  }

  /**
   * Add item on queue
   *
   * @throws Exception
  **/
  public static function addItemsToQueue()
  {
    $queue = DrupalQueue::create('cron_jobs');

    try {
      $cars = self::getCarsFromApi();

      if( count($result) > 0)
      {
        foreach($cars as $car)
        {
          $queue->createItem($car);
        }
      }
    } catch(Exception $e) {

      watchdog_exception('cron_jobs_exception', $e);
    }
  }

  /**
   * Process all items on queue
  **/
  public static function processItemsOnQueue()
  {
    $queue = DrupalQueue::create('cron_jobs');

    if( $queue->numberOfItems() > 0 )
    {
      while( $car = $queue->claimItem() )
      {
        $transaction = db_transaction(__FUNCTION__);

        try {
          self::saveCarOnDrupal($car);
          $queue->deleteItem($car);

        } catch( Exception $e) {

          $transaction->rollback();
          $queue->deleteItem($car);
          watchdog_exception('cron_jobs_exception', $e);
        }

        unset($transaction);
      }
    }
  }

  private static function saveCarOnDrupal($car)
  {

  }
}
