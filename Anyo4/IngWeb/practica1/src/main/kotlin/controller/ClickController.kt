package es.unizar.webeng.hello.controller

import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping

/**
 * MVC Controller class that returns a simple response page when the button
 * on the main page is clicked.
 *
 * @constructor Create Clicker controller
 */
@Controller class ClickController {

    /**
     * Mapping for HTTP GET requests for /click route
     *
     * @return click HTML resource
     */
    @GetMapping("/click") fun clicker(): String {
        return "click"
    }

}