package es.unizar.webeng.hello.controller

import org.springframework.beans.factory.annotation.Value
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping


/**
 * Controller for the root page of the application. It consists of a simple
 * hello world greeting page.
 *
 * @constructor Create Hello controller
 */
// The controller annotation indicates the class will be a controller in a
// Model-View-Controller context.
// The controller part of an MVC architecture handles requests for resources
// from a model (normally, a database), while the view is responsible for
// displaying the results (e.g. returning HTML to the browser).
@Controller class HelloController {

    // The @Value annotation tells Spring Boot to store the value of app.message
    // (in application.properties) in the message variable. "Hello World" is
    // a fallback value, but the app won't compile if app.message is not defined
    @Value("\${app.message}") private var message: String = "Hello World"

    /**
     * On an HTTP GET request, returns the resource welcome.html
     *
     * @param model
     * @return The welcome.html page
     */
    @GetMapping("/") fun welcome(model: MutableMap<String, Any>): String {
        // Substitute the "message" placeholder with the variable
        // Otherwise, the message will be empty
        model["message"] = message

        // Returns the HTML file
        return "welcome"
    }
}
