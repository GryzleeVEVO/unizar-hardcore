package es.unizar.webeng.hello.controller

import org.assertj.core.api.Assertions.assertThat
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.Test

/**
 * Test class for the Hello World controller
 *
 * @constructor Create Hello controller unit tests
 */
class HelloControllerUnitTests {
    private lateinit var controller: HelloController

    /**
     * Sets up controller before testing.
     */
    @BeforeEach fun setup() {
        controller = HelloController()
    }

    /**
     * Tests if the view returned by the controller is welcome.html and
     * contains "Hello World" in the "message" field.
     */
    @Test fun testMessage() {
        val map = mutableMapOf<String, Any>()
        val view = controller.welcome(map)
        assertThat(view).isEqualTo("welcome")
        assertThat(map.containsKey("message")).isTrue
        assertThat(map["message"]).isEqualTo("Hello World")
    }
}
