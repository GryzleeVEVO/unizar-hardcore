package es.unizar.webeng.hello

import org.springframework.boot.autoconfigure.SpringBootApplication
import org.springframework.boot.runApplication


/**
 * Main class of the Spring Boot application.
 *
 * @constructor Create a new Application.
 */
// @SpringBootApplication is self-explanatory: it tells that Spring Boot will be
// used, and the app will be automatically configured.
@SpringBootApplication class Application


/**
 * Entry point of program. Starts the Spring Boot application.
 *
 * @param args Command line arguments
 */
fun main(args: Array<String>) {
    runApplication<Application>(*args)
}
