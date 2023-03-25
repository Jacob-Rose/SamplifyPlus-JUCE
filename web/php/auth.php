<?php
function sendResponse() {                                               // [1]
 
    $response  = '<?xml version="1.0" encoding="utf-8"?>';              // [2]
    if($_POST["email"] === "test@juce.com" and $_POST["pw"] === "test") // [3]
        $response .= '<MESSAGE message="Thanks for registering our product!"><KEY>INSERT_KEY_HERE</KEY></MESSAGE>';
    else
        $response .= '<ERROR error="Sorry, we were not able to authorise your request. Please provide a valid email address and password."></ERROR>';
 
    return $response;
}
 
header("Content-type: text/xml; charset=utf-8");                        // [4]
echo sendResponse();
?>