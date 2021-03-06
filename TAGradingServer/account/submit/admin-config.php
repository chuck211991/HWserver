<?php

require_once "../../toolbox/functions.php";

check_administrator();

use \lib\Database;

Database::query("SELECT * FROM config");
foreach(Database::rows() as $config) {
    if (isset($_POST[$config['config_name']])) {
        $value = $_POST[$config['config_name']];
    }
    else {
        $value = null;
    }

    if ($config['config_type'] == 1) {
        $value = intval($value);
    }
    else if ($config['config_type'] == 2) {
        $value = floatval($value);
    }
    else if ($config['config_type'] == 3) {
        $value = ($value == "true") ? "true" : "false";
    }
    else if ($config['config_type'] == 4) {
        $value = ($value == null) ? "" : $value;
    }

    if ($value == $config['config_value']) {
        continue;
    }

    Database::query("UPDATE config SET config_value=? WHERE config_name=?", array($value, $config['config_name']));
}

header('Location: '.__BASE_URL__.'/account/admin-config.php?course='.$_GET['course']);