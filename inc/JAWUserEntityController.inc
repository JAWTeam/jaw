<?php
/**
 * @file        JAWUserEntityController.inc
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
 * @created     10.10.13
 */

if(!class_exists('DrupalDefaultEntityController')) {
    die('This script can not be executed directly.');
}

class JAWUserEntityController extends DrupalDefaultEntityController {

  public function create() {
    return (object) array(
      'id' => '',
      'uid' => '',
    );
  }
}
