<?php

require_once(__DIR__ . '/../TAGradingServer/lib/AutoLoader.php');
require_once(__DIR__ . '/test.php');
use lib\AutoLoader;
AutoLoader::registerDirectory(__DIR__."/../TAGradingServer/lib", true, "lib");
AutoLoader::registerDirectory(__DIR__."/../TAGradingServer/app", true, "app");
AutoLoader::registerDirectory(__DIR__."/e2e", true, "e2e");
AutoLoader::registerDirectory(__DIR__."/sausage/src/Sauce", true, "Sauce");