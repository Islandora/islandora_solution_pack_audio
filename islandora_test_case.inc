<?php

/**
 * @file
 * Generic test case to be extended to implement Islandora testing.
 */
abstract class IslandoraTestCase extends DrupalWebTestCase
{

  /**
   * User with the rights required to perform the test
   */
  protected $privileged_user;

  /**
   * Override this method to provide the short form name of the module,
   * e.g. the name of the .module file 
   *
   * @return string - The short form name of the module
   */
  abstract protected function getModuleShortFormName();

  /**
   * Override this method to create and login your own privileged user with the
   * permissions that are required to run the tests.  Defaults to logging in as 
   * user 1.
   */
  protected function authenticatePrivilegedUser() {
    $this->privileged_user = user_load( 1 );
    $this->drupalLogin($this->privileged_user); 
  }

  /**
   * Automatically generates all module dependencies and enables them in order.
   * Also logs in a privileged user with the appropriate permissions for the
   * test.
   *
   * If you need to override this method to provide extra functionality,
   * please be sure to call parent::setUp so dependency resolution and 
   * authentication still happen.
   */
  public function setUp() {    
    // Grab all the available modules
    $modules = module_rebuild_cache();

    // Grab the module to test's dependencies
    $dependencies = $modules[$this->getModuleShortFormName()]->info['dependencies'];

    // Sort them so they're in the correct order to enable
    $dependencies = array_reverse($dependencies);

    // Add our module to the end
    $dependencies[] = $this->getModuleShortFormName();
    
    // Enable the module's dependencies in order
    call_user_func_array('parent::setUp', $dependencies);

    // Authenticate the privileged user
    $this->authenticatePrivilegedUser();
  } 
}
