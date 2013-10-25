<?php
/**
 * @file        JAWTaskEntityController.inc
 * @description
 *
 * PHP Version  5.3.13
 *
 * @package 
 * @category
 * @plugin URI
 * @copyright   2013, Vadim Pshentsov. All Rights Reserved.
 * @license     http://www.apache.org/licenses/LICENSE-2.0 Apache License, Version 2.0
 * @author      Vadim Pshentsov <pshentsoff@gmail.com> 
 * @link        http://pshentsoff.ru Author's homepage
 * @link        http://blog.pshentsoff.ru Author's blog
 *
 * @created     08.10.13
 */

if(!class_exists('DrupalDefaultEntityController')) {
    die('This script can not be executed directly.');
}

class JAWTaskEntityController extends DrupalDefaultEntityController {

  public function create(){
    return (object) array(
      'id' => '',
      'label' => '',
      'pid' => 0,
      'tkid' => 1,

    );
  }

  public function save($jaw_task) {

    $transaction = db_transaction();
    try {
      global $user;
      $jaw_task->is_new = empty($jaw_task->id);
      if($jaw_task->is_new) { $jaw_task->created = REQUEST_TIME; }
      $jaw_task->changed = REQUEST_TIME;
      $update = true;

      field_attach_presave('jaw_task', $jaw_task);

      if($jaw_task->is_new) {
        drupal_write_record('jaw_tasks', $jaw_task);
        $op = 'insert';
      } else {
        drupal_write_record('jaw_tasks', $jaw_task, 'id');
        $op = 'update';
      }

      $function = 'field_attach_'.$op;
      $function('jaw_task', $jaw_task);

      module_invoke_all('entity_'.$op, $jaw_task, 'jaw_task');
      unset($jaw_task->is_new);

      return $jaw_task;

    } catch (Exception $e) {
      $transaction->rollback();

      watchdog_exception('jaw_task', $e);

      return false;
    }
  }

  public function delete($task_ids) {
    if(!empty($task_ids)) {
      $jaw_tasks = $this->load($task_ids, array());
      $transaction = db_transaction();
      try {
        db_delete('jaw_tasks')
          ->condition('id', $task_ids, 'IN')
          ->execute();

        //Delete attached fields values for each 'id'
        foreach ($jaw_tasks as $jaw_task) {
          field_attach_delete('jaw_tasks', $jaw_task);
          module_invoke_all('entity_delete', $jaw_task, 'jaw_tasks');
        }

        db_ignore_slave();

      } catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('jaw_tasks', $e);
        return false;
      }

      cache_clear_all();
      $this->resetCache();
    }

    return true;
  }

}
