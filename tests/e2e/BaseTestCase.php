<?php

namespace e2e;

/**
 * Gives us a common setup to use for all e2e tests
 *
 * Class BaseTestCase
 * @package e2e
 */
class BaseTestCase extends \Sauce\Sausage\WebDriverTestCase {
    protected $start_url = 'http://localhost/';
    protected $base_url = 'http://localhost/';

    public static $browsers;

    public static function setUpBeforeClass() {
        $_SERVER['PHP_AUTH_USER'] = 'pevelm';
    }
}

/*
 * Determine whether or not we're on TRAVIS as if we're not, we don't want to connect to
 * Sauce Labs and run on their Matrix
 */
if (getenv('TRAVIS') !== true) {
    BaseTestCase::$browsers = array(
        array(
            'browserName' => 'firefox',
            'local' => true,
            'sessionStrategy' => 'shared'
        )
    );
}
else {
    BaseTestCase::$browsers = array(
        // run FF15 on Windows 8 on Sauce
        array(
            'browserName' => 'firefox',
            'desiredCapabilities' => array(
                'version' => '15',
                'platform' => 'Windows 2012',
            )
        ),
        // run Chrome on Linux on Sauce
        array(
            'browserName' => 'chrome',
            'desiredCapabilities' => array(
                'platform' => 'Linux'
            )
        ),
    );
}